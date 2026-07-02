//  ____  _             _        _           _       
// |  _ \| |          | |      | |         (_)      
// | |_) | | ___   ___| | _____| |__   __ _ _ _ __  
// |  _ <| |/ _ \ / __| |/ / __| '_ \ / _` | | '_ \    Demo blockchain C++ (Nâng cấp RSA)
// | |_) | | (_) | (__|   < (__| | | | (_| | | | | |   Thư viện: Crypto++ 
// |____/|_|\___/ \___|_|\_\___|_| |_|\__,_|_|_| |_|   Tác giả: Nguyễn Trường Chinh
// 
// Bản quyền: MIT LICENSE 2026

/* DOC
────────────────────────────────────────────────────────────────────────────────────────────
Hoàn thành ngày 01/07/2026                                  
Demo đào block và thêm block vào blockchain kết hợp chữ ký số RSA

Tìm hiểu hash sha256 là gì ?
  - Hash là hàm băm dữ liệu với đầu vào con người có thể đọc hiểu 
    nhưng đầu ra là 1 chuỗi được mã hóa dài 256(bit) ~ 64 ký tự   
  - Mã băm không thể giải mã (hàm 1 chiều)
  - Chỉ cần thay đổi nhỏ ở input -> output thay đổi hoàn toàn
  - Hiệu ứng thác đổ (Avalanche Effect): Thay đổi dù chỉ 1 dấu chấm, 
    toàn bộ chuỗi hash đầu ra sẽ biến đổi hoàn toàn không theo quy luật.
  - Phân biệt trong code: MD5 dùng để băm Transaction Code (nhanh, ngắn gọn),
    còn SHA256 dùng để băm toàn bộ Block bảo vệ chuỗi liên kết.

Cơ chế chữ ký số (RSA Signature)
  - Mỗi User tham gia mạng lưới sở hữu một cặp khóa: Private và Public Key
  - Người gửi ký giao dịch bằng Private Key -> Tạo ra Signature dạng chuỗi
  - Toàn bộ mạng lưới sử dụng Public Key để kiểm tra (Verify) giao dịch đó
  - Đảm bảo tính bảo mật, chống gian lận và mạo danh ví người khác
  - Chống chối bỏ (Non-repudiation): Khi một giao dịch đã được ký bằng 
    Private Key hợp lệ, người gửi không thể phủ nhận giao dịch đó do chính họ tạo ra.
  - Mã hóa Hex: Chữ ký nhị phân thô (Raw bytes) sinh ra từ bộ ký sẽ được mã hóa
    sang dạng chuỗi ký tự Hex nhằm truyền tải và lưu trữ an toàn trong tệp JSON.

Cách hoạt động
  - Khi 1 block yêu cầu vào blockchain, hệ thống sẽ:
    - Gom dữ liệu (transactions) đã được xác thực chữ ký thành công
    - Lấy hash của block trước (previous hash) để liên kết chuỗi
    - Thêm timestamp (thời gian khởi tạo khối)
    - Thêm nonce (số thử bắt đầu từ 0)

  - Validator (hoặc miner) sẽ:
    - Thử nhiều giá trị nonce khác nhau
    - Mỗi lần thử sẽ tính lại hash của block
    - Mục tiêu: tìm được hash thỏa điều kiện (ví dụ: bắt đầu bằng "0000")
    -> Quá trình này gọi là Proof of Work (PoW)

Ví dụ:
  hash = SHA256(block_id + data + previous_hash + time + nonce)

  Validator sẽ lặp:
    nonce = 0,1,2,3,... cho đến khi:
    hash thỏa điều kiện độ khó (difficulty)
    Ví dụ nếu difficulty = 4
      -> khi hash ra a6957asd.... (hash chưa thỏa)
      -> Khi hash ra 000089a5.... (hash thỏa) 

Độ khó (difficulty):
  - Quy định số lượng số 0 ở đầu hash
  - Difficulty càng cao -> càng khó tìm -> càng tốn thời gian
  - Tính bất đối xứng: Việc tìm ra Nonce hợp lệ cực kỳ khó và tốn tài nguyên phần cứng,
    nhưng khi đã tìm ra, các node khác chỉ mất một phép toán để kiểm tra lại (Verify nhanh).

Khi tìm được hash hợp lệ:
  - Block được "mine" thành công
  - Được thêm vào blockchain
  - Validator nhận reward (phần thưởng)
  -> Quá trình này thường được gọi là đào coin

Cấu trúc Genesis Block (Khối nguyên thủy)
  - Là khối đầu tiên (Index = 0) đặt nền móng cho toàn bộ Blockchain.
  - Không có khối đứng trước, vì vậy Previous Hash được gán mặc định chuỗi 64 ký tự '0'.
  - Chứa thông điệp khởi thủy của nhà sáng lập mạng lưới.

Đặc điểm quan trọng:
  - Không thể đoán trước hash
  - Chỉ có thể brute-force (thử sai liên tục)
  - Dễ verify (kiểm tra nhanh), khó tạo (tính lâu)
  - Tính bất biến (Immutability): Nếu bất kỳ ai cố tình chỉnh sửa dù chỉ 1 byte dữ liệu ở một khối cũ,
    mã hash khối đó thay đổi -> gãy liên kết với khối tiếp theo -> toàn bộ chuỗi phía sau trở nên vô hiệu.

Ứng dụng:
  - Blockchain (Bitcoin, Ethereum, Smart Contract)
  - Bảo mật mật khẩu, chữ ký điện tử doanh nghiệp
  - Kiểm tra toàn vẹn dữ liệu phần mềm, file tải về

Tóm tắt:
  Mining = tìm nonce sao cho hash hợp lệ
  -> đảm bảo tính bảo mật, toàn vẹn dữ liệu và phi tập trung của blockchain
────────────────────────────────────────────────────────────────────────────────────────────
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <random>
#include <fstream>
#include <thread>
#include <atomic>

// thư viện tóm tắt dùng hash trong openssl gọn như python
// xem hướng dẫn và tải tại: https://github.com/trgchinhh/hashlib-cpp
#include "lib/hashlib.h"

// Thư viện phụ dùng cho json 
#include "lib/json.hpp"

// Thư viện Crypto++ cho RSA
// Cài từ Ming64
// Lệnh cài: pacman -S mingw-w64-x86_64-cryptopp
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <cryptopp/pssr.h>

// Thư viện indicator cho progress bar
// Tải thư viện tại: https://github.com/p-ranav/indicators
#include <indicators/progress_bar.hpp>
#include <indicators/cursor_control.hpp>

using namespace std;
using namespace hashlib;
using namespace chrono;
using namespace CryptoPP;
using namespace indicators;
using namespace indicators::option;
using json = nlohmann::ordered_json;

const vector<string> names = {
    "Truong Chinh", "Truong An", "Duy Minh", "Duy Quang", "Ngoc Tu",
    "Thanh Phuc", "Manh Hung", "The An", "Thien An", "Gia Bao", 
    "Khanh Dang", "Hai Dang", "Khanh Duy", "Bao Khanh", "Manh Cuong"
};

string time_now(){
    auto now = system_clock::now();
    time_t time = system_clock::to_time_t(now);
    tm* local_tm = localtime(&time);
    ostringstream oss;
    oss << put_time(local_tm, "%H:%M:%S - %d/%m/%Y");
    return oss.str();
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

public:
    Block(json data) {
        this->block_data = data;
        this->block_id = 0;
        this->block_nonce = 0;
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
        this->chain.push_back(genesis_block);
        this->print_block(genesis_block);
    }

    void add_block(json data){
        Block block(data);
        block.block_id = chain.size();
        block.block_time = time_now();
        block.block_prev_hash = this->chain.back().block_curr_hash;
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
        this->chain.push_back(block);
        this->print_block(block);
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

void countdown(int seconds){
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

    countdown(3);
    cout << "Bắt đầu đào " << number_blocks << " block với độ khó " << difficulty << "\n\n";
    this_thread::sleep_for(seconds(2));

    for(int i = 0; i < number_blocks; i++) {
        atomic<bool> done = false;
        json ts = tx_manager.create_transaction();
        if(tx_manager.verify_transaction(ts)) {
            cout << "Đã xác thực giao dịch " << (i + 1) << endl;
            blockchain.add_block(ts);
        } else {
            cout << "Không thể xác thực giao dịch " << (i + 1) << endl;
        }
    }

    cout << "Hoàn tất đào " << number_blocks 
         << " block với " << blockchain.total_time << " giây" << endl;

    show_console_cursor(false);
    return 0;
}
