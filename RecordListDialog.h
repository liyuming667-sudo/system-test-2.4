//
#pragma once
#ifndef RECORDLISTDIALOG_H
#define RECORDLISTDIALOG_H

#include <QDialog>
#include <vector>
// 【重要】只包含数据类，不要包含 LibraryManager，绝不要包含 RecordListDialog.h 自身！
#include "BorrowRecord.h" 

class QTableWidget; // 前置声明

class RecordListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordListDialog(const std::vector<BorrowRecord>& records, QWidget* parent = nullptr);
    ~RecordListDialog();

private:
    void initUI();
    void loadData();

    QTableWidget* table_ = nullptr;
    std::vector<BorrowRecord> records_;
};

#endif // RECORDLISTDIALOG_H