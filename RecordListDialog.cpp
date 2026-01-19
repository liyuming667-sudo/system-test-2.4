#include "RecordListDialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStringList>

// 构造函数
RecordListDialog::RecordListDialog(const std::vector<BorrowRecord>& records, QWidget* parent)
    : QDialog(parent), records_(records)
{
    setWindowTitle("借阅记录列表");
    resize(900, 600);
    initUI();
    loadData();
}

RecordListDialog::~RecordListDialog()
{
}

void RecordListDialog::initUI() {
    auto* layout = new QVBoxLayout(this);

    table_ = new QTableWidget(this);
    // 设置列：记录ID, 借阅人, ISBN, 借阅时间, 状态
    table_->setColumnCount(5);
    QStringList headers;
    headers << "记录ID" << "借阅人账号" << "图书ISBN" << "借阅时间" << "状态";
    table_->setHorizontalHeaderLabels(headers);

    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addWidget(table_);
}

void RecordListDialog::loadData() {
    table_->setRowCount(static_cast<int>(records_.size()));
    int row = 0;
    for (const auto& r : records_) {
        // ID
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(r.id())));
        // User
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(r.username())));
        // ISBN
        table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(r.isbn())));
        // Borrow Time
        table_->setItem(row, 3, new QTableWidgetItem(r.borrowTime().toString("yyyy-MM-dd HH:mm")));
        
        // Status
        QString status = "未归还";
        if (r.returned()) {
            status = QString("已归还 (%1)").arg(r.returnTime().toString("yyyy-MM-dd"));
        }
        table_->setItem(row, 4, new QTableWidgetItem(status));

        row++;
    }
}
