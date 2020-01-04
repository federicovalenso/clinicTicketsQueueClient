#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonDocument>
#include <QMainWindow>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QPrinter>
#include <QString>
#include <QTimer>
#include <memory>
#include "printblockingdialog.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override = default;

 protected:
  virtual void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void on_btnBook_clicked();
  void on_btnDisp_clicked();
  void on_btnReceipt_clicked();
  void on_btnEmergency_clicked();
  void on_btnReport_clicked();
  void on_btnVaccine_clicked();
  void on_btnOther_clicked();
  void replyFinished(QNetworkReply *);

 private:
  static const QString kLogin;
  static const QString kLoginPath;
  static const QString kName;
  static const QString kPassword;
  static const QString kTicketNumber;
  static const QString kAction;
  static const QString kTicketAction;
  static const QString kBook;
  static const QString kDisp;
  static const QString kReceipt;
  static const QString kEmergency;
  static const QString kReport;
  static const QString kVaccine;
  static const QString kOther;
  static const QByteArray kSetCookieHeader;
  static const QByteArray kSessionId;
  static const QMap<QString, QString> kActionsDescribes;

  std::unique_ptr<Ui::MainWindow> ui_;
  QNetworkAccessManager *network_manager_;
  PrintBlockingDialog *print_blocking_dialog_;

  mutable struct LastRequest {
    QNetworkRequest request;
    QByteArray data;
  } last_request_;

  void sendLoginRequest(const QString &name, const QString &password) const
      noexcept;
  void sendTicketRequest(const QString &action) const noexcept;
  QNetworkCookie getCookie(const QNetworkReply &reply) const noexcept;
  void printTicket(const QJsonDocument &ticket);
  void showPrintDialog();
  void setButtonsEnabled(bool mode);
};

#endif  // MAINWINDOW_H
