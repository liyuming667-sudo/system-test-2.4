//
#include "ReaderListDialog.h"

// 【关键】补全 Qt 头文件，特别是 QTableWidgetItem
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem> 
#include <QHeaderView>
#include <QStringList>

// 构造函数
ReaderListDialog::ReaderListDialog(const std::vector<User>& users,
    LibraryManager* library,
    QWidget* parent)
    : QDialog(parent), users_(users), library_(library)
{
    setWindowTitle("用户列表管理");
    resize(800, 500);
    initUI();
    loadData();
}

void ReaderListDialog::initUI() {
    auto* layout = new QVBoxLayout(this);

    // 【关键】此时编译器已经认识 QTableWidget 了
    table_ = new QTableWidget(this);

    // 设置列：账号, 姓名, 联系方式, 身份, 状态, 最大借书数
    table_->setColumnCount(6);
    QStringList headers;
    headers << "账号" << "姓名" << "联系方式" << "身份" << "状态" << "借书额度";
    table_->setHorizontalHeaderLabels(headers);

    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(table_);
}

void ReaderListDialog::loadData() {
    table_->setRowCount(static_cast<int>(users_.size()));
    int row = 0;
    for (const auto& u : users_) {
        // 解析身份
        QString roleStr;
        if (u.isSuperAdmin()) roleStr = "最高管理员";
        else if (u.isAdmin()) roleStr = "管理员";
        else roleStr = "读者";

        // 解析状态
        QString status = u.banned() ? "已封禁" : "正常";

        // 【关键】此时编译器已经认识 QTableWidgetItem 了
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(u.username())));
        table_->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(u.name())));
        table_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(u.contact())));
        table_->setItem(row, 3, new QTableWidgetItem(roleStr));
        table_->setItem(row, 4, new QTableWidgetItem(status));
        table_->setItem(row, 5, new QTableWidgetItem(QString::number(u.maxBorrow())));

        ++row;
    }
}