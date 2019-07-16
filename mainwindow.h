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
  static const QString ACTION_LOGIN;
  static const QString ACTION_LOGIN_PATH;
  static const QString NAME_PARAM;
  static const QString PASSWORD_PARAM;
  static const QString TICKET_NUMBER_PARAM;
  static const QString ACTION_PARAM;
  static const QString BOOK_ACTION;
  static const QString DISP_ACTION;
  static const QString RECEIPT_ACTION;
  static const QString EMERGENCY_ACTION;
  static const QString REPORT_ACTION;
  static const QString VACCINE_ACTION;
  static const QString OTHER_ACTION;
  static const QByteArray SET_COOKIE_HEADER;
  static const QByteArray SESSION_ID;
  static const QMap<QString, QString> ACTIONS_DESCRIBES;

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
