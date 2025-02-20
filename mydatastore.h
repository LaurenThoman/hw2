#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"
#include "util.h"
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <iostream>

class MyDataStore : public DataStore {
public:
    MyDataStore();
    ~MyDataStore();

    // Implementations of the DataStore interface:
    void addProduct(Product* p) override;
    void addUser(User* u) override;
    std::vector<Product*> search(std::vector<std::string>& terms, int type) override;
    void dump(std::ostream& ofile) override;

    // Additional shopping cart functions:
    void addToCart(const std::string& username, int hitIndex, const std::vector<Product*>& hits);
    void viewCart(const std::string& username);
    void buyCart(const std::string& username);

    User* getUser(const std::string& username); 

private:
    std::vector<Product*> products_;
    std::map<std::string, User*> users_;
    std::map<std::string, std::set<Product*>> keywordMap_;
    std::map<std::string, std::queue<Product*>> carts_;
};

#endif
