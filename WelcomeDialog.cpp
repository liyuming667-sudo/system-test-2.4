#include "WelcomeDialog.h"
#include "UserManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>

WelcomeDialog::WelcomeDialog(UserManager* users, QWidget* parent)
    : QDialog(parent), users_(users)
{
    setWindowTitle("图书管理系统");
    resize(520, 360);
    setModal(true);

    initUI();
    formWidget_->setVisible(false); // 初始不显示输入框
}

std::optional<QString> WelcomeDialog::loggedInUser() const {
    return loggedUser_;
}

void WelcomeDialog::initUI() {
    auto* root = new QVBoxLayout(this);
    root->setSpacing(15);

    // ===== 标题 =====
    titleLabel_ = new QLabel("欢迎来到图书管理系统！", this);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("font-size:18px;font-weight:bold;");
    root->addWidget(titleLabel_);

    // ===== 初始按钮 =====
    auto* btnRow = new QHBoxLayout();
    loginBtn_ = new QPushButton("登录", this);
    registerBtn_ = new QPushButton("注册", this);
    exitBtn_ = new QPushButton("退出", this);

    loginBtn_->setMinimumHeight(40);
    registerBtn_->setMinimumHeight(40);
    exitBtn_->setMinimumHeight(40);

    btnRow->addWidget(loginBtn_);
    btnRow->addWidget(registerBtn_);
    btnRow->addWidget(exitBtn_);
    root->addLayout(btnRow);

    // ===== 表单 =====
    formWidget_ = new QWidget(this);
    auto* form = new QVBoxLayout(formWidget_);
    form->setSpacing(8);

    auto makeRow = [&](QLabel*& lab, QLineEdit*& edit, const QString& text) {
        auto* row = new QHBoxLayout();
        lab = new QLabel(text, formWidget_);
        edit = new QLineEdit(formWidget_);
        row->addWidget(lab);
        row->addWidget(edit);
        form->addLayout(row);
        };

    makeRow(userLabel_, userEdit_, "账号：");
    makeRow(passLabel_, passEdit_, "密码：");
    passEdit_->setEchoMode(QLineEdit::Password);

    makeRow(nameLabel_, nameEdit_, "姓名：");
    makeRow(contactLabel_, contactEdit_, "联系方式：");

    auto* actionRow = new QHBoxLayout();
    actionRow->addStretch();
    submitBtn_ = new QPushButton("确定", formWidget_);
    backBtn_ = new QPushButton("返回", formWidget_);
    actionRow->addWidget(submitBtn_);
    actionRow->addWidget(backBtn_);
    form->addLayout(actionRow);

    root->addWidget(formWidget_);

    // ===== 信号 =====
    connect(loginBtn_, &QPushButton::clicked, this, &WelcomeDialog::onLoginClicked);
    connect(registerBtn_, &QPushButton::clicked, this, &WelcomeDialog::onRegisterClicked);
    connect(exitBtn_, &QPushButton::clicked, this, &WelcomeDialog::onExitClicked);

    connect(submitBtn_, &QPushButton::clicked, this, [this]() {
        loginMode_ ? doLogin() : doRegister();
        });
    connect(backBtn_, &QPushButton::clicked, this, &WelcomeDialog::onBack);
}

void WelcomeDialog::showForm(bool loginMode) {
    loginMode_ = loginMode;
    formWidget_->setVisible(true);

    nameLabel_->setVisible(!loginMode_);
    nameEdit_->setVisible(!loginMode_);
    contactLabel_->setVisible(!loginMode_);
    contactEdit_->setVisible(!loginMode_);

    submitBtn_->setText(loginMode_ ? "登录" : "注册");
}

void WelcomeDialog::onLoginClicked() {
    showForm(true);
}

void WelcomeDialog::onRegisterClicked() {
    showForm(false);
}

void WelcomeDialog::onExitClicked() {
    reject(); // 结束程序
}

void WelcomeDialog::onBack() {
    formWidget_->setVisible(false);
    userEdit_->clear();
    passEdit_->clear();
    nameEdit_->clear();
    contactEdit_->clear();
}

void WelcomeDialog::doLogin() {
    QString u = userEdit_->text().trimmed();
    QString p = passEdit_->text();

    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "提示", "账号和密码不能为空。");
        return;
    }

    std::string msg;
    auto user = users_->login(u.toStdString(), p.toStdString(), msg);
    if (!user.has_value()) {
        QMessageBox::warning(this, "登录失败", QString::fromStdString(msg));
        return;
    }

    loggedUser_ = u;
    QMessageBox::information(this, "登录成功", "欢迎进入系统！");
    accept();
}

void WelcomeDialog::doRegister() {
    QString u = userEdit_->text().trimmed();
    QString p = passEdit_->text();
    QString n = nameEdit_->text().trimmed();
    QString c = contactEdit_->text().trimmed();

    if (u.isEmpty() || p.isEmpty() || n.isEmpty() || c.isEmpty()) {
        QMessageBox::warning(this, "提示", "请完整填写注册信息。");
        return;
    }

    std::string msg;
    if (!users_->registerReader(u.toStdString(),
        p.toStdString(),
        n.toStdString(),
        c.toStdString(),
        msg)) {
        QMessageBox::warning(this, "注册失败", QString::fromStdString(msg));
        return;
    }

    QMessageBox::information(this, "注册成功", "读者账号创建成功，请登录。");
    onBack();
    showForm(true);
}
