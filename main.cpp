//  ____  _            _        _           _       
// |  _ \| |          | |      | |         (_)      
// | |_) | | ___   ___| | _____| |__   __ _ _ _ __  
// |  _ <| |/ _ \ / __| |/ / __| '_ \ / _` | | '_ \    Demo blockchain C++
// | |_) | | (_) | (__|   < (__| | | | (_| | | | | |   https://github.com/trgchinhh/blockchain-cpp
// |____/|_|\___/ \___|_|\_\___|_| |_|\__,_|_|_| |_|   Tác giả: Nguyễn Trường Chinh
// 
// Bản quyền: MIT LICENSE 2026

/* DOC
──────────────────────────────────────────────────────────────────
Hoàn thành ngày 01/05/2026                                 
Demo đào block và thêm block vào blockchain

Tìm hiểu hash sha256 là gì ?
  - Hash là hàm băm dữ liệu với đầu vào con người có thể đọc hiểu 
    nhưng đầu ra là 1 chuỗi được mã hóa dài 256(bit) ~ 64 ký tự   
  - Mã băm không thể giải mã (hàm 1 chiều)
  - Chỉ cần thay đổi nhỏ ở input -> output thay đổi hoàn toàn

Cách hoạt động
  - Khi 1 block yêu cầu vào blockchain, hệ thống sẽ:
    - Gom dữ liệu (transactions)
    - Lấy hash của block trước (previous hash)
    - Thêm timestamp
    - Thêm nonce (số thử)

  - Validator (hoặc miner) sẽ:
    - Thử nhiều giá trị nonce khác nhau
    - Mỗi lần thử sẽ tính lại hash của block
    - Mục tiêu: tìm được hash thỏa điều kiện (ví dụ: bắt đầu bằng "0000")
    -> Quá trình này gọi là Proof of Work (PoW)

Ví dụ:
  hash = SHA256(data + previous_hash + nonce)

  Validator sẽ lặp:
    nonce = 0,1,2,3,... cho đến khi:
    hash thỏa điều kiện độ khó (difficulty)
    Ví dụ nếu difficulty = 4
      -> khi hash ra a6957asd.... (hash chưa thỏa)
      -> Khi hash ra 000089a5.... (hash thỏa) 

Độ khó (difficulty):
  - Quy định số lượng số 0 ở đầu hash
  - Difficulty càng cao -> càng khó tìm -> càng tốn thời gian

Khi tìm được hash hợp lệ:
  - Block được "mine" thành công
  - Được thêm vào blockchain
  - Validator nhận reward (phần thưởng)
  -> Quá trình này thường được gọi là đào coin

Đặc điểm quan trọng:
  - Không thể đoán trước hash
  - Chỉ có thể brute-force (thử sai)
  - Dễ verify (kiểm tra nhanh), khó tạo (tính lâu)

Ứng dụng:
  - Blockchain (Bitcoin, Ethereum)
  - Bảo mật mật khẩu
  - Kiểm tra toàn vẹn dữ liệu

Tóm tắt:
  Mining = tìm nonce sao cho hash hợp lệ
  -> đảm bảo tính bảo mật và phi tập trung của blockchain
──────────────────────────────────────────────────────────────────
*/


#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include "lib//hashlib.h"
#include "lib//json.hpp"

using namespace std;
using namespace hashlib;
using namespace chrono;
using json = nlohmann::ordered_json;
//using json = o

string time_now(){
    auto now = system_clock::now();
    time_t time = system_clock::to_time_t(now);
    tm* local_tm = localtime(&time);
    ostringstream oss;
    oss << put_time(local_tm, "%H:%M:%S %d/%m/%Y");
    return oss.str();
}

class Block {
private:
    int block_id;
    json block_data;
    string block_prev_hash;
    string block_curr_hash;
    string block_time;
    int block_nonce;

public:
    Block(json data) {
        this->block_data = data;
        this->block_id = 0;
        this->block_nonce = 0;
    }

    string hash_sha256(){
        string hash_data = to_string(this->block_id) 
                         + this->block_data.dump(4)
                         + this->block_prev_hash
                         + this->block_time
                         + to_string(this->block_nonce);
        return sha256(hash_data).hexdigest();                         
    }

    friend class Blockchain;
};

class Owner{
private:
    string owner;
    int balance;
    int mined_block;

public:
    Owner(){
        this->owner = "";
        this->balance = 0;
        this->mined_block = 0;
    }

    Owner(string owner){
        this->owner = owner;
        this->balance = 0;
        this->mined_block = 0;
    }

    friend class Blockchain;
};

class Blockchain {
private:
    Owner owner;
    int difficulty;
    vector<Block> chain;
    string str_difficulty;

public:
    Blockchain(Owner& owner, int difficulty){
        this->owner = owner;
        this->difficulty = difficulty;
        this->str_difficulty = string(this->difficulty, '0');
        this->genesis_block();
    }

    void genesis_block(){
        json genesis_data;
        genesis_data["msg"] = "Create genesis block at " + time_now(); 
        Block genesis_block(genesis_data);
        genesis_block.block_time = time_now();
        genesis_block.block_prev_hash = string(64, '0');
        genesis_block.block_curr_hash = genesis_block.hash_sha256();
        this->chain.push_back(genesis_block);
        print_block(genesis_block);
    }

    void add_block(json data){
        Block block(data);
        block.block_id = chain.size();
        block.block_time = time_now();
        block.block_prev_hash = this->chain.back().block_curr_hash;
        block.block_curr_hash = block.hash_sha256();
        while(block.block_curr_hash.substr(0, this->difficulty) != this->str_difficulty){
            block.block_nonce++;
            cout << "\rNonce: " << block.block_nonce;
            block.block_curr_hash = block.hash_sha256();            
        }    
        cout << endl;
        this->chain.push_back(block);
        this->owner.mined_block += 1;
        this->owner.balance += 100;
        print_block(block);
    }

    bool is_valid(){
        for(int i = 1; i < chain.size(); i++){
            Block prev_block = this->chain[i-1];
            Block curr_block = this->chain[i];
            if(prev_block.block_curr_hash != curr_block.block_prev_hash)
                return false;
            if(curr_block.block_curr_hash != curr_block.hash_sha256())
                return false;
        }
        return true;
    }

    void print_block(Block block){
        cout << "Block: " << block.block_id << endl;
        cout << "Data: " << block.block_data.dump(4) << endl;
        cout << "Prev hash: " << block.block_prev_hash << endl;
        cout << "Curr hash: " << block.block_curr_hash << endl;
        cout << "Time: " << block.block_time << endl;
        cout << endl << endl;
    }

    void get_balance(Owner& owner){
        for(int i = 1; i < this->chain.size(); i++){
            Block block = this->chain[i];
            json datas = block.block_data;
            if(datas.is_array()){
                for(auto& tx : datas){
                    if(tx.contains("from") && tx.contains("to") && tx.contains("amount")){
                        string from = tx["from"];
                        string to = tx["to"];
                        int amount = tx["amount"];
                        if(from == owner.owner){
                            owner.balance -= amount;
                        } 
                        if(to == owner.owner){
                            owner.balance += amount;
                        } 
                    }
                }
            }
        }
        cout << "(-) Balance of " << owner.owner << ": " << owner.balance << endl; 
    }
};

int main(){
    cout << "[?] Enter ownername: ";
    string owner_name; getline(cin, owner_name);
    cout << "[?] Enter difficulty: ";
    int difficulty; cin >> difficulty;

    Owner owner(owner_name);
    Blockchain blockchain(owner, difficulty);

    blockchain.add_block({
        {{"from", "An"}, {"to", "Chinh"}, {"amount", 500}},
        {{"from", "Minh"}, {"to", "Chinh"}, {"amount", 300}},
        {{"from", "Quang"}, {"to", "Chinh"}, {"amount", 700}}
    });

    blockchain.add_block({
        {{"from", "Chinh"}, {"to", "An"}, {"amount", 200}},
        {{"from", "Chinh"}, {"to", "Quang"}, {"amount", 200}},
        {{"from", "Chinh"}, {"to", "An"}, {"amount", 100}}
    });

    blockchain.get_balance(owner);
    cout << boolalpha << "(?) Is valid: " << blockchain.is_valid() << endl;

    return 0;
}
