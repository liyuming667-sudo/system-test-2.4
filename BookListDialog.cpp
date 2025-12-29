//
#include "BookListDialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>

// 构造函数
BookListDialog::BookListDialog(LibraryManager* mgr, QWidget* parent)
    : QDialog(parent), manager_(mgr)
{
    initUI();
    loadData();
}

BookListDialog::~BookListDialog()
{
}

void BookListDialog::initUI()
{
    this->setWindowTitle("图书列表");
    this->resize(900, 600);

    QVBoxLayout* layout = new QVBoxLayout(this);

    table_ = new QTableWidget(this);
    // 修正：Book 类有7个属性，我们展示主要的
    // 列：ISBN, 书名, 作者, 出版社, 年份, 类别, 库存
    table_->setColumnCount(7);

    QStringList headers;
    headers << "ISBN" << "书名" << "作者" << "出版社" << "年份" << "类别" << "库存";
    table_->setHorizontalHeaderLabels(headers);

    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    layout->addWidget(table_);
}

void BookListDialog::loadData()
{
    if (!manager_) return;

    // 获取所有图书数据
    // 注意：getAllBooks 返回的是 std::vector<Book>，不是 Qt 的 QVector，根据你的 LibraryManager.h
    std::vector<Book> books = manager_->getAllBooks();

    table_->setRowCount(static_cast<int>(books.size()));

    for (int i = 0; i < books.size(); ++i) {
        const Book& book = books[i];

        // 【修正】使用正确的 Getter 方法，没有 "get" 前缀
        table_->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(book.isbn())));
        table_->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(book.title())));
        table_->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(book.author())));
        table_->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(book.publisher())));
        table_->setItem(i, 4, new QTableWidgetItem(QString::number(book.publishYear())));
        table_->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(book.category())));
        table_->setItem(i, 6, new QTableWidgetItem(QString::number(book.stock())));
    }
}