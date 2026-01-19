#include "BookListDialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>

BookListDialog::BookListDialog(LibraryManager* mgr, QWidget* parent)
    : QDialog(parent), manager_(mgr)
{
    initUI();
    loadData(); // 默认尝试加载所有书
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
    // 如果没有 manager（比如搜索模式），则不默认加载数据，等待 setBooks 被调用
    if (!manager_) return;
    setBooks(manager_->getAllBooks());
}

// 【新增】核心的数据填充逻辑
void BookListDialog::setBooks(const std::vector<Book>& books)
{
    table_->setRowCount(static_cast<int>(books.size()));

    for (int i = 0; i < books.size(); ++i) {
        const Book& book = books[i];
        table_->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(book.isbn())));
        table_->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(book.title())));
        table_->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(book.author())));
        table_->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(book.publisher())));
        table_->setItem(i, 4, new QTableWidgetItem(QString::number(book.publishYear())));
        table_->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(book.category())));
        table_->setItem(i, 6, new QTableWidgetItem(QString::number(book.stock())));
    }
}
