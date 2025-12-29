//
#pragma once
#ifndef BOOKLISTDIALOG_H  
#define BOOKLISTDIALOG_H

#include <QDialog>
#include <vector>
#include "LibraryManager.h" 

class QTableWidget;

// ★★★ 检查这里！类名必须是 BookListDialog
class BookListDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数
    explicit BookListDialog(LibraryManager* mgr, QWidget* parent = nullptr);
    ~BookListDialog();

private:
    void initUI();
    void loadData();

    QTableWidget* table_ = nullptr;
    LibraryManager* manager_ = nullptr;
};

#endif // BOOKLISTDIALOG_H