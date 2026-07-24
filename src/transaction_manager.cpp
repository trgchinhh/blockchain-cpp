#include "lib//transaction_manager.h"

string TransactionManager::clean_key(string key) {
    if(key.length() > 30) {
        return key.substr(0, 15) + "..." + key.substr(key.length() - 15);
    }
    return key;
}

json TransactionManager::create_transaction() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, names.size() - 1);
    string send_name = names[dis(gen)];
    string receive_name = names[dis(gen)];
    while (send_name == receive_name) {
        receive_name = names[dis(gen)];
    }

    ifstream publicfile(path_publickeys);
    json public_keys = json::parse(publicfile);
    string sender_public_key = public_keys[send_name]["public_key"];
    string receiver_public_key = public_keys[receive_name]["public_key"];
    uniform_real_distribution<> amount_dis(100000, 10000000);
    double amount = round(amount_dis(gen) * 1000.0) / 1000.0;
    double fee = round((amount * 0.005) * 1000000.0) / 1000000.0;
    string t_time = time_now();
    string code_data = send_name + receive_name + to_string(amount) + t_time;
    string tx_code = md5(code_data).hexdigest(); 
    json tx_data;
    tx_data["Send name"] = send_name;
    tx_data["Id send"] = clean_key(sender_public_key);
    tx_data["Receive name"] = receive_name;
    tx_data["Id receive"] = clean_key(receiver_public_key);
    tx_data["Amount"] = amount;
    tx_data["Fee"] = fee;
    tx_data["Transaction code"] = tx_code;
    tx_data["Time"] = t_time;
    ifstream privatefile(path_privatekeys);
    json private_keys = json::parse(privatefile);
    string send_priv_b64 = private_keys[send_name]["private_key"];
    string message = tx_data.dump(); 
    tx_data["Signature"] = RSAHelper::sign_message(message, send_priv_b64);
    return tx_data;
}

bool TransactionManager::verify_transaction(json transaction) {
    string send_name = transaction["Send name"];
    string hex_signature = transaction["Signature"];
    json verify_data = transaction;
    verify_data.erase("Signature");
    string message = verify_data.dump();
    ifstream pubFile(path_publickeys);
    json public_keys = json::parse(pubFile);
    string send_pub_b64 = public_keys[send_name]["public_key"];
    return RSAHelper::verify_signature(message, hex_signature, send_pub_b64);
}
