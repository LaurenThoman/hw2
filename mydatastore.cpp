#include "mydatastore.h"
#include "datastore.h" 
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

MyDataStore::MyDataStore() {
}

MyDataStore::~MyDataStore() {
  //  cleans up allocated pointers
  for (auto p : products_) {
    delete p;
  }
  //  cleans up allocated user pointers
  for (auto pair : users_) {
    delete pair.second;
  }
}

User* MyDataStore::getUser(const std::string& username) {
  auto it = users_.find(username);
  if (it != users_.end()) {
    return it->second;
  }
  return nullptr;
}

void MyDataStore::addProduct(Product* p) {
  products_.push_back(p);
  //  gets keywords for product
  set<string> keys = p->keywords();
  //  for each keyword, inserts product into keyword map
  for (const string& key : keys) {
      keywordMap_[key].insert(p);
  }
}

void MyDataStore::addUser(User* u) {
  users_[u->getName()] = u;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
  set<Product*> result;

  if (terms.empty())
    return vector<Product*>();

  //  starts with first term
  string term = terms[0];
  if (keywordMap_.find(term) != keywordMap_.end()) {
    result = keywordMap_[term];
  }

  if (type == 0) { // intersects the sets for each term.
    for (size_t i = 1; i < terms.size(); i++) {
      string t = terms[i];
      if (keywordMap_.find(t) != keywordMap_.end()) {
        result = setIntersection(result, keywordMap_[t]);
      } else {
        //  the intersection is empty if none found
        result.clear();
        break;
      }
    }
  } else { // or union sets
    for (size_t i = 1; i < terms.size(); i++) {
      string t = terms[i];
      if (keywordMap_.find(t) != keywordMap_.end()) {
        result = setUnion(result, keywordMap_[t]);
      }
    }
  }

  //  convert result set to vector
  return vector<Product*>(result.begin(), result.end());
}

void MyDataStore::dump(std::ostream& ofile) {
  ofile << "<products>" << endl;
  for (auto p : products_) {
    p->dump(ofile);
  }
  ofile << "</products>" << endl;
  ofile << "<users>" << endl;
  for (auto pair : users_) {
    pair.second->dump(ofile);
  }
  ofile << "</users>" << endl;
}

void MyDataStore::addToCart(const std::string& username, int hitIndex, const std::vector<Product*>& hits) {
  //  check if exists
  if (users_.find(username) == users_.end() || hitIndex < 1 || hitIndex > (int)hits.size()) {
    cout << "Invalid request" << endl;
    return;
  }
  carts_[username].push(hits[hitIndex - 1]);
}

void MyDataStore::viewCart(const std::string& username) {
  if (users_.find(username) == users_.end()) {
    cout << "Invalid username" << endl;
    return;
  }
  // temp copy
  queue<Product*> tempCart = carts_[username];
  int index = 1;
  while (!tempCart.empty()) {
    cout << "Item " << index << ":" << endl;
    cout << tempCart.front()->displayString() << endl;
    tempCart.pop();
    index++;
  }
}

void MyDataStore::buyCart(const std::string& username) {
  if (users_.find(username) == users_.end()) {
    cout << "Invalid username" << endl;
    return;
  }
  //  reference user cart
  queue<Product*>& userCart = carts_[username];
  queue<Product*> newCart; // for products that cant be purchased

  while (!userCart.empty()) {
    Product* prod = userCart.front();
    userCart.pop();
    User* u = users_[username];

    if (prod->getQty() > 0 && u->getBalance() >= prod->getPrice()) {
      //  subtract quant and deduct the price.
      prod->subtractQty(1);
      u->deductAmount(prod->getPrice());
    } else {
      //  no purchase, then leave it in cart
      newCart.push(prod);
    }
  }
  //  updates cart
  carts_[username] = newCart;
}
