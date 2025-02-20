#include "book.h"

Book::Book(const std::string& name, double price, int qty, const std::string& isbn, const std::string& author)
    : Product("book", name, price, qty), isbn_(isbn), author_(author) {}

//  Book::keywords()
std::set<std::string> Book::keywords() const {
    std::set<std::string> keys = parseStringToWords(name_);
    std::set<std::string> authorKeys = parseStringToWords(author_);
    keys.insert(isbn_);
    keys = setUnion(keys, authorKeys);
    return keys;
}

//  Book::displayString()
std::string Book::displayString() const {
    return name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + "\n" + 
           std::to_string(price_) + " " + std::to_string(qty_) + " left.";
}

//  Book::dump(std::ostream& os)
void Book::dump(std::ostream& os) const {
    os << "book\n" << name_ << "\n" << price_ << "\n" << qty_ << "\n" 
       << isbn_ << "\n" << author_ << std::endl;
}
