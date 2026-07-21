#include "lib//blockchain.hpp"

Blockchain::Blockchain(int difficulty) {
    this->difficulty = difficulty;
    this->str_difficulty = string(this->difficulty, '0');
    this->genesis_block();
}

void Blockchain::genesis_block() {
    json genesis_data;
    genesis_data["msg"] = "Đây là block khởi tạo !"; 
    genesis_data["time"] = time_now();
    Block genesis_block(genesis_data);
    genesis_block.block_time = time_now();
    genesis_block.block_prev_hash = string(64, '0');
    genesis_block.block_curr_hash = genesis_block.hash_sha256();
    genesis_block.isvalid = true;
    this->chain.push_back(genesis_block);
    this->print_block(genesis_block);
}

void Blockchain::add_block(json data) {
    Block block(data);
    Block prev_block = this->chain.back();
    block.block_id = chain.size();
    block.block_time = time_now();
    block.block_prev_hash = prev_block.block_curr_hash;
    block.block_curr_hash = block.hash_sha256();
    auto start_time = high_resolution_clock::now();
    while(block.block_curr_hash.substr(0, this->difficulty) != this->str_difficulty) {
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

bool Blockchain::isvalid_block(Block block, Block prev_block) {
    if(block.block_prev_hash != prev_block.block_curr_hash) {
        return false;
    }
    if(block.block_curr_hash != block.hash_sha256()) {
        return false;
    }
    return true;
}

bool Blockchain::isvalid_chain() {
    for(size_t i = 0; i < this->chain.size(); i++) {
        Block block = this->chain[i];
        if(!block.isvalid) {
            cout << "Block: " << i << " không thể xác thực" << endl;
            return false;
        }
    }
    cout << "Toàn bộ block đã được xác thực" << endl;
    return true;
}

void Blockchain::print_block(Block block) {
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
