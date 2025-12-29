#include "LibraryManager.h"

#include <algorithm>
#include <ctime>
#include <cctype>

#include <QDateTime>

// ===== 工具 =====
static std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return r;
}

bool LibraryManager::containsIgnoreCase(const std::string& src,
    const std::string& key) {
    if (key.empty()) return true;
    return toLower(src).find(toLower(key)) != std::string::npos;
}

// ===== 图书管理 =====
bool LibraryManager::addBook(const Book& book) {
    if (findBook(book.isbn())) return false;
    books_.push_back(book);
    return true;
}

bool LibraryManager::deleteBookByISBN(const std::string& isbn) {
    // 有未归还记录不能删
    for (const auto& r : records_) {
        if (r.isbn() == isbn && !r.returned())
            return false;
    }

    auto it = std::remove_if(books_.begin(), books_.end(),
        [&](const Book& b) { return b.isbn() == isbn; });
    if (it == books_.end()) return false;

    books_.erase(it, books_.end());
    return true;
}

bool LibraryManager::modifyBookByISBN(const std::string& isbn,
    const Book& newBook) {
    Book* b = findBook(isbn);
    if (!b) return false;
    *b = newBook;
    return true;
}

// ===== 图书查询 =====
std::vector<Book> LibraryManager::searchBooks(const std::string& titleKey,
    const std::string& authorKey,
    const std::string& categoryKey) const {
    std::vector<Book> res;
    for (const auto& b : books_) {
        if (containsIgnoreCase(b.title(), titleKey) &&
            containsIgnoreCase(b.author(), authorKey) &&
            containsIgnoreCase(b.category(), categoryKey)) {
            res.push_back(b);
        }
    }
    return res;
}

std::vector<Book> LibraryManager::getAllBooks() const {
    return books_;
}

std::vector<BorrowRecord> LibraryManager::getAllRecords() const {
    return records_;
}

// ===== 借阅 =====
bool LibraryManager::borrowBook(const std::string& username,
    const std::string& isbn,
    std::string& outRecordID) {
    Book* b = findBook(isbn);
    if (!b || b->stock() <= 0) return false;

    b->setStock(b->stock() - 1);

    std::string id = generateRecordID();
    records_.emplace_back(id, username, isbn, QDateTime::currentDateTime());
    outRecordID = id;
    return true;
}

bool LibraryManager::returnBookByRecordID(const std::string& recordID,
    int& outDays,
    bool& outOverdue) {
    BorrowRecord* r = findRecord(recordID);
    if (!r || r->returned()) return false;

    r->markReturned(QDateTime::currentDateTime());

    Book* b = findBook(r->isbn());
    if (b) b->setStock(b->stock() + 1);

    qint64 secs = r->borrowTime().secsTo(r->returnTime());
    outDays = static_cast<int>(secs / (24 * 3600));
    outOverdue = outDays > 30;

    return true;
}

// ===== 查询 =====
std::vector<BorrowRecord> LibraryManager::recordsByUser(const std::string& username) const {
    std::vector<BorrowRecord> res;
    for (const auto& r : records_) {
        if (r.username() == username)
            res.push_back(r);
    }
    return res;
}

std::vector<BorrowRecord> LibraryManager::recordsByISBN(const std::string& isbn) const {
    std::vector<BorrowRecord> res;
    for (const auto& r : records_) {
        if (r.isbn() == isbn)
            res.push_back(r);
    }
    return res;
}

// ===== 支撑 UserManager =====
bool LibraryManager::hasUnreturned(const std::string& username) const {
    for (const auto& r : records_) {
        if (r.username() == username && !r.returned())
            return true;
    }
    return false;
}

int LibraryManager::currentBorrowCount(const std::string& username) const {
    int cnt = 0;
    for (const auto& r : records_) {
        if (r.username() == username && !r.returned())
            ++cnt;
    }
    return cnt;
}

// ===== 内部查找 =====
Book* LibraryManager::findBook(const std::string& isbn) {
    for (auto& b : books_) {
        if (b.isbn() == isbn)
            return &b;
    }
    return nullptr;
}

BorrowRecord* LibraryManager::findRecord(const std::string& id) {
    for (auto& r : records_) {
        if (r.id() == id)
            return &r;
    }
    return nullptr;
}

std::string LibraryManager::generateRecordID() const {
    return "R" + std::to_string(std::time(nullptr)) +
        "_" + std::to_string(records_.size() + 1);
}
