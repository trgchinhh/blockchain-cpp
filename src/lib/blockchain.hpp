#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "header.h"
#include "block.h"
#include "utils.h"

class Blockchain {
private:
    int difficulty;
    vector<Block> chain;
    string str_difficulty;

public:
    int total_time = 0;
    Blockchain(int difficulty);

    void genesis_block();
    void add_block(json data);
    bool isvalid_block(Block block, Block prev_block);
    bool isvalid_chain();
    void print_block(Block block);
};

#endif 
