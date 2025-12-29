#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <vector>
#include <functional>
#include <string>

#include "Book.h"
#include "BorrowRecord.h"
#include "User.h"

class QWidget;
class QVBoxLayout;
class UserManager;
class LibraryManager;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(UserManager* users,
        LibraryManager* library,
        const QString& me,
        QWidget* parent = nullptr);

signals:
    void logoutRequested();

private:
    enum class Page { Home, Books, Readers, Borrows };

    UserManager* users_ = nullptr;
    LibraryManager* lib_ = nullptr;
    QString me_;
    Page page_ = Page::Home;

    QWidget* central_ = nullptr;
    QVBoxLayout* layout_ = nullptr;

    bool isAdmin() const;
    bool isSuperAdmin() const;

    void rebuildUI();
    void addBtn(const QString& text, const std::function<void()>& fn);

private slots:
    void goHome();
    void goBooks();
    void goReaders();
    void goBorrows();
    void onLogout();

    void addBookUI();
    void deleteBookUI();
    void modifyBookUI();
    void searchBookUI();
    void showAllBooksUI();

    void listUsersUI();
    void editUserUI();
    void banUserUI();
    void unbanUserUI();
    void listBannedUI();
    void promoteToAdminUI();
    void demoteToReaderUI();
    void showReaderTableUI();

    void borrowBookUI();
    void returnBookUI();
    void myRecordsUI();
    void allRecordsUI();

    void deleteSelfUI();
};

#endif // MAINWINDOW_H