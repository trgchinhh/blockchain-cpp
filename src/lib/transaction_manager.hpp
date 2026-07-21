#ifndef TRANSACTION_MANAGER_HPP
#define TRANSACTION_MANAGER_HPP

#include "header.hpp"
#include "utils.hpp"
#include "rsa.hpp"
#include "path.hpp"

class TransactionManager {
public:
    string clean_key(string key);
    json create_transaction();
    bool verify_transaction(json transaction);
};

#endif 