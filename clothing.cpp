#include "clothing.h"

Clothing::Clothing(const std::string& name, double price, int qty, const std::string& size, const std::string& brand)
    : Product("clothing", name, price, qty), size_(size), brand_(brand) {}

//   Clothing::keywords()
std::set<std::string> Clothing::keywords() const {
    std::set<std::string> keys = parseStringToWords(name_);
    std::set<std::string> brandKeys = parseStringToWords(brand_);
    keys = setUnion(keys, brandKeys);
    return keys;
}

//  Clothing::displayString()
std::string Clothing::displayString() const {
    return name_ + "\nSize: " + size_ + " Brand: " + brand_ + "\n" + 
           std::to_string(price_) + " " + std::to_string(qty_) + " left.";
}

//  Clothing::dump(std::ostream& os)
void Clothing::dump(std::ostream& os) const {
    os << "clothing\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" 
       << size_ << "\n" << brand_ << std::endl;
}
