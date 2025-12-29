//
#pragma once
#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <vector>

// 【检查】这里绝不能有 class BookListDialog 的定义！

class Book {
public:
    Book() = default;
    Book(const std::string& isbn,
        const std::string& title,
        const std::string& author,
        const std::string& publisher,
        int publishYear,
        const std::string& category,
        int stock);

    const std::string& isbn() const;
    const std::string& title() const;
    const std::string& author() const;
    const std::string& publisher() const;
    int publishYear() const;
    const std::string& category() const;
    int stock() const;

    bool canBorrow() const;
    bool borrowOne();
    void returnOne();

    void setTitle(const std::string& v) { title_ = v; }
    void setAuthor(const std::string& v) { author_ = v; }
    void setPublisher(const std::string& v) { publisher_ = v; }
    void setPublishYear(int v) { publishYear_ = v; }
    void setCategory(const std::string& v) { category_ = v; }
    void setStock(int v) { stock_ = v; }

private:
    std::string isbn_;
    std::string title_;
    std::string author_;
    std::string publisher_;
    int publishYear_ = 0;
    std::string category_;
    int stock_ = 0;
};

#endif