//  ____  _             _        _           _       
// |  _ \| |          | |      | |         (_)      
// | |_) | | ___   ___| | _____| |__   __ _ _ _ __  
// |  _ <| |/ _ \ / __| |/ / __| '_ \ / _` | | '_ \    Demo blockchain C++ (Nâng cấp RSA)
// | |_) | | (_) | (__|   < (__| | | | (_| | | | | |   Thư viện: Crypto++ 
// |____/|_|\___/ \___|_|\_\___|_| |_|\__,_|_|_| |_|   Tác giả: Nguyễn Trường Chinh
// 
// Bản quyền: MIT LICENSE 2026

#include "header.hpp"

const vector<string> names = {
    "Truong Chinh", "Truong An", "Duy Minh", "Duy Quang", "Ngoc Tu",
    "Thanh Phuc", "Manh Hung", "The An", "Thien An", "Gia Bao", 
    "Khanh Dang", "Hai Dang", "Khanh Duy", "Bao Khanh", "Manh Cuong"
};

string time_now(){
    return datetime::now().strftime("%H:%M:%S - %d/%m/%Y");
}

class UserBlockchain {
    public:
        static void save_all_user_keys() {
            AutoSeededRandomPool rng;
            json public_data;
            json private_data;

            for (const string& name : names) {
                InvertibleRSAFunction parameters;
                parameters.GenerateRandomWithKeySize(rng, 1024);
                CryptoPP::RSA::PrivateKey privateKey(parameters);
                CryptoPP::RSA::PublicKey publicKey(parameters);
                string privateStr, publicStr;
                Base64Encoder privSink(new StringSink(privateStr));
                privateKey.DEREncode(privSink);
                privSink.MessageEnd();
                Base64Encoder pubSink(new StringSink(publicStr));
                publicKey.DEREncode(pubSink);
                pubSink.MessageEnd();
                public_data[name] = { {"public_key", publicStr} };
                private_data[name] = { {"private_key", privateStr} };
            }

            ofstream publicfile("public_keys.json");
            publicfile << public_data.dump(4);
            ofstream privatefile("private_keys.json");
            privatefile << private_data.dump(4);
        }
};

class TransactionManager {
    public:
        string clean_key(string key) {
            if(key.length() > 30) {
                return key.substr(0, 15) + "..." + key.substr(key.length() - 15);
            }
            return key;
        }
        json create_transaction() {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, names.size() - 1);
            string send_name = names[dis(gen)];
            string receive_name = names[dis(gen)];
            while (send_name == receive_name) {
                receive_name = names[dis(gen)];
            }

            ifstream publicfile("public_keys.json");
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

            ifstream privatefile("private_keys.json");
            json private_keys = json::parse(privatefile);
            string send_priv_b64 = private_keys[send_name]["private_key"];

            CryptoPP::RSA::PrivateKey privateKey;
            StringSource privSource(send_priv_b64, true, new Base64Decoder);
            privateKey.BERDecode(privSource);

            string message = tx_data.dump(); 

            AutoSeededRandomPool rng;
            RSASS<PKCS1v15, CryptoPP::SHA256>::Signer signer(privateKey);
            
            string signature;
            StringSource(message, true, 
                new SignerFilter(rng, signer, 
                    new HexEncoder(new StringSink(signature)) 
                )
            );

            tx_data["Signature"] = signature;
            return tx_data;
        }

        bool verify_transaction(json transaction) {
            try {
                string send_name = transaction["Send name"];
                string hex_signature = transaction["Signature"];

                json verify_data = transaction;
                verify_data.erase("Signature");
                string message = verify_data.dump();

                ifstream pubFile("public_keys.json");
                json public_keys = json::parse(pubFile);
                string send_pub_b64 = public_keys[send_name]["public_key"];

                CryptoPP::RSA::PublicKey publicKey;
                StringSource pubSource(send_pub_b64, true, new Base64Decoder);
                publicKey.BERDecode(pubSource);

                RSASS<PKCS1v15, CryptoPP::SHA256>::Verifier verifier(publicKey);
                
                string binary_signature;
                StringSource(hex_signature, true, new HexDecoder(new StringSink(binary_signature)));

                bool result = false;
                StringSource(message + binary_signature, true,
                    new SignatureVerificationFilter(
                        verifier,
                        new ArraySink((CryptoPP::byte*)&result, sizeof(result)),
                        SignatureVerificationFilter::PUT_RESULT | SignatureVerificationFilter::SIGNATURE_AT_END
                    )
                );
                return result;
            } 
            catch (...) {
                return false;
            }
        }
};

class Block {
    private:
        int block_id;
        json block_data;
        string block_prev_hash;
        string block_curr_hash;
        string block_time;
        int block_nonce;
        int mining_time;
        bool isvalid;

    public:
        Block(json data) {
            this->block_data = data;
            this->block_id = 0;
            this->block_nonce = 0;
            this->isvalid = false;
        }

        string hash_sha256(){
            string hash_data = to_string(this->block_id) 
                             + this->block_data.dump() 
                             + this->block_prev_hash
                             + this->block_time
                             + to_string(this->block_nonce);
            return sha256(hash_data).hexdigest();                    
        }
        friend class Blockchain;
};

class Blockchain {
    private:
        int difficulty;
        vector<Block> chain;
        string str_difficulty;

    public:
        int total_time = 0;
        Blockchain(int difficulty){
            this->difficulty = difficulty;
            this->str_difficulty = string(this->difficulty, '0');
            this->genesis_block();
        }

        void genesis_block(){
            json genesis_data;
            genesis_data["msg"] = "This is a genesis block in chain coding by NTC++"; 
            genesis_data["time"] = time_now();
            Block genesis_block(genesis_data);
            genesis_block.block_time = time_now();
            genesis_block.block_prev_hash = string(64, '0');
            genesis_block.block_curr_hash = genesis_block.hash_sha256();
            genesis_block.isvalid = true;
            this->chain.push_back(genesis_block);
            this->print_block(genesis_block);
        }

        void add_block(json data){
            Block block(data);
            Block prev_block = this->chain.back();
            block.block_id = chain.size();
            block.block_time = time_now();
            block.block_prev_hash = prev_block.block_curr_hash;
            block.block_curr_hash = block.hash_sha256();
            auto start_time = high_resolution_clock::now();
            while(block.block_curr_hash.substr(0, this->difficulty) != this->str_difficulty){
                block.block_nonce++;
                cout << "\rNonce: " << block.block_nonce;
                block.block_curr_hash = block.hash_sha256();            
            }    
            auto end_time = high_resolution_clock::now();
            block.mining_time = duration_cast<seconds>((end_time - start_time)).count();
            this->total_time += block.mining_time;
            cout << endl;
            block.isvalid = this->isvalid_block(block, prev_block);
            this->chain.push_back(block);
            this->print_block(block);
        }

        bool isvalid_block(Block block, Block prev_block){
            if(block.block_prev_hash != prev_block.block_curr_hash){
                return false;
            }
            if(block.block_curr_hash != block.hash_sha256()){
                return false;
            }
            return true;
        }

        bool isvalid_chain(){
            for(int i = 0; i < this->chain.size(); i++){
                Block block = this->chain[i];
                if(!block.isvalid){
                    cout << "Block: " << i << " không thể xác thực" << endl;
                    return false;
                }
            }
            cout << "Toàn bộ block đã được xác thực" << endl;
            return true;
        }

        void print_block(Block block){
            json display_data = block.block_data;
            if (display_data.contains("Signature")) {
                string sig = display_data["Signature"];
                display_data["Signature"] = sig.substr(0, 10) + "..." + sig.substr(sig.length() - 10);
            }

            cout << "Index: " << block.block_id << endl;
            cout << "Data: " << display_data.dump(4) << endl;
            cout << "Prev hash: " << block.block_prev_hash << endl;
            cout << "Curr hash: " << block.block_curr_hash << endl;
            cout << "Time: " << block.block_time << endl;
            cout << "Mining time: " << block.mining_time << " s" << endl;
            cout << "\n" << endl;
        }
};

void loading_bar(int seconds){
    ProgressBar bar{
        BarWidth{40},
        Start{"["},
        Fill{"#"},
        Lead{"#"},
        Remainder{"-"},
        End{"]"},
        ShowPercentage{true},
        PrefixText{"Đang chuẩn bị giao dịch: "},
    };
    const int total = seconds * 25;
    for(int i = 0; i <= total; i++){
        bar.set_progress(i*100/total);
        this_thread::sleep_for(milliseconds(40));
    }
}

int main(){
    show_console_cursor(false);
    cout << "Đang khởi tạo địa chỉ ví cho toàn user tham gia..." << endl;
    UserBlockchain userblockchain;
    userblockchain.save_all_user_keys();

    int number_blocks = 5;
    int difficulty = 4; 
    Blockchain blockchain(difficulty);
    TransactionManager tx_manager;

    loading_bar(3);
    cout << "Bắt đầu đào " << number_blocks << " block với độ khó " << difficulty << "\n\n";
    this_thread::sleep_for(seconds(2));

    for(int i = 0; i < number_blocks; i++) {
        json ts = tx_manager.create_transaction();
        if(tx_manager.verify_transaction(ts)) {
            cout << "Đã xác thực giao dịch " << (i + 1) << endl;
            blockchain.add_block(ts);
        } else {
            cout << "Không thể xác thực giao dịch " << (i + 1) << endl;
        }
    }

    blockchain.isvalid_chain();
    cout << "Hoàn tất đào " << number_blocks 
         << " block với " << blockchain.total_time << " giây" << endl;

    show_console_cursor(false);
    return 0;
}
