#pragma once
#ifndef READERLISTDIALOG_H
#define READERLISTDIALOG_H

#include <QDialog>
#include <vector>
#include "User.h"

class QTableWidget;
class LibraryManager;

class ReaderListDialog : public QDialog {
    Q_OBJECT
public:
    // 修改：构造函数接收 std::vector<User>，不再需要 UserManager 指针
    explicit ReaderListDialog(const std::vector<User>& users,
        LibraryManager* library,
        QWidget* parent = nullptr);

private:
    void initUI();
    void loadData();

private:
    std::vector<User> users_; // 存储要显示的数据
    LibraryManager* library_ = nullptr;

    QTableWidget* table_ = nullptr;
};

#endif // READERLISTDIALOG_H