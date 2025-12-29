#pragma once
#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QString>
#include <optional>

class QLabel;
class QPushButton;
class QLineEdit;
class QWidget;
class UserManager;

class WelcomeDialog : public QDialog {
    Q_OBJECT
public:
    explicit WelcomeDialog(UserManager* users, QWidget* parent = nullptr);

    // 登录成功后，由外部（AppController / main）读取
    std::optional<QString> loggedInUser() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onExitClicked();

    void doLogin();
    void doRegister();
    void onBack();

private:
    void initUI();
    void showForm(bool loginMode);

private:
    UserManager* users_ = nullptr;

    // ===== 标题 =====
    QLabel* titleLabel_ = nullptr;

    // ===== 初始三个按钮 =====
    QPushButton* loginBtn_ = nullptr;
    QPushButton* registerBtn_ = nullptr;
    QPushButton* exitBtn_ = nullptr;

    // ===== 输入区域 =====
    QWidget* formWidget_ = nullptr;

    QLabel* userLabel_ = nullptr;
    QLineEdit* userEdit_ = nullptr;

    QLabel* passLabel_ = nullptr;
    QLineEdit* passEdit_ = nullptr;

    QLabel* nameLabel_ = nullptr;
    QLineEdit* nameEdit_ = nullptr;

    QLabel* contactLabel_ = nullptr;
    QLineEdit* contactEdit_ = nullptr;

    QPushButton* submitBtn_ = nullptr;
    QPushButton* backBtn_ = nullptr;

    bool loginMode_ = true; // true=登录 false=注册
    std::optional<QString> loggedUser_;
};

#endif // WELCOMEDIALOG_H
