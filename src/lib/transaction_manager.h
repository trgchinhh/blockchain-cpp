#ifndef TRANSACTION_MANAGER_H
#define TRANSACTION_MANAGER_H

#include "header.h"
#include "utils.h"
#include "rsa.h"
#include "path.h"

class TransactionManager {
public:
    string clean_key(string key);
    json create_transaction();
    bool verify_transaction(json transaction);
};

#endif 
