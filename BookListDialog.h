#pragma once
#ifndef BOOKLISTDIALOG_H  
#define BOOKLISTDIALOG_H

#include <QDialog>
#include <vector>
#include "LibraryManager.h" 
#include "Book.h" // 确保包含 Book 定义

class QTableWidget;

class BookListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookListDialog(LibraryManager* mgr, QWidget* parent = nullptr);
    ~BookListDialog();

    // 【新增】允许外部手动设置要显示的图书列表（用于搜索结果展示）
    void setBooks(const std::vector<Book>& books);

private:
    void initUI();
    void loadData(); // 默认加载逻辑

    QTableWidget* table_ = nullptr;
    LibraryManager* manager_ = nullptr;
};

#endif // BOOKLISTDIALOG_H
