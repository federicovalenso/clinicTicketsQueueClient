#include "mainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMessageBox>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QPainter>
#include <QTextDocument>
#include <QUrl>
#include <QUrlQuery>
#include "appsettings.h"
#include "dlgsettings.h"

const QString MainWindow::kLogin = "login";
const QString MainWindow::kLoginPath = "/" + MainWindow::kLogin;
const QString MainWindow::kName = "name";
const QString MainWindow::kPassword = "password";
const QString MainWindow::kTicketNumber = "ticket_number";
const QString MainWindow::kAction = "action";
const QString MainWindow::kTicketAction = "action_name";
const QString MainWindow::kBook = "book";
const QString MainWindow::kDisp = "disp";
const QString MainWindow::kReceipt = "receipt";
const QString MainWindow::kEmergency = "emergency";
const QString MainWindow::kReport = "report";
const QString MainWindow::kVaccine = "vaccine";
const QString MainWindow::kOther = "other";
const QByteArray MainWindow::kSetCookieHeader = "Set-Cookie";
const QByteArray MainWindow::kSessionId = "sessionid";
const QMap<QString, QString> MainWindow::kActionsDescribes = {
    {kBook, "Плановая запись"},
    {kDisp, "Диспансеризация"},
    {kReceipt, "Выписка льготного рецепта"},
    {kEmergency, "На приём по острому состоянию"},
    {kReport, "Санаторно-курортная карта, операция, справка, выписка"},
    {kVaccine, "Вакцинация (клещ, грип)"},
    {kOther, "Прочее"},
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  network_manager_ = new QNetworkAccessManager(this);
  connect(network_manager_, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));
  QTimer::singleShot(1000, this, SLOT(showFullScreen()));

  QFile styleFile;
  styleFile.setFileName(":style/qss/style.qss");
  if (!styleFile.open(QFile::ReadOnly)) {
    qDebug() << "Не найден файл стилей";
  } else {
    QString stylesheet = styleFile.readAll();
    setStyleSheet(stylesheet);
  }
  print_blocking_dialog_ = new PrintBlockingDialog(this);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->modifiers() == Qt::KeyboardModifier::AltModifier) {
    if (event->key() == Qt::Key::Key_S) {
      DlgSettings dlgSettins(this);
      dlgSettins.exec();
    }
  }
}

void MainWindow::sendLoginRequest(const QString &name,
                                  const QString &password) const noexcept {
  AppSettings &settings = AppSettings::getInstance();
  QString address = QString("http://%1:%2/%3")
                        .arg(settings.getServerAddress())
                        .arg(settings.getPort())
                        .arg(kLogin);
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  QUrlQuery params;
  params.addQueryItem(kName, name);
  params.addQueryItem(kPassword, password);
  network_manager_->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void MainWindow::sendTicketRequest(const QString &action) const noexcept {
  AppSettings &settings = AppSettings::getInstance();
  QString address = QString("http://%1:%2/api/tickets")
                        .arg(settings.getServerAddress())
                        .arg(settings.getPort());
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  QUrlQuery params;
  params.addQueryItem(kAction, action);
  last_request_ = {request, params.toString(QUrl::FullyEncoded).toUtf8()};
  network_manager_->post(last_request_.request, last_request_.data);
}

void MainWindow::printTicket(const QJsonDocument &ticket) {
  if (ticket.isNull()) {
    QMessageBox::critical(this, tr("Ошибка"), tr("Неверный формат талона"),
                          QMessageBox::Close);
  } else {
    QFile ticketTemplateFile;
    ticketTemplateFile.setFileName(
        ":/printTemplates/printTemplates/ticket.html");

    if (!ticketTemplateFile.open(QFile::ReadOnly)) {
      qDebug() << "Не найден файл шаблона чека";
    } else {
      showPrintDialog();
      QJsonObject jsonTicket = ticket.object();
      AppSettings &settings = AppSettings::getInstance();
      int ticketWidth = settings.getTicketWidth();
      int ticketHeight = settings.getTicketHeight();
      QString templateHtml = ticketTemplateFile.readAll();
      QString ticketNumber =
          jsonTicket.value(kTicketNumber).toVariant().toString();
      templateHtml.replace("{ticket}", std::move(ticketNumber));
      QString actionName =
          jsonTicket.value(kTicketAction).toVariant().toString();
      templateHtml.replace("{action}",
                           kActionsDescribes.find(actionName).value());
      QString currentDateTime =
          QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");
      templateHtml.replace("{datetime}", std::move(currentDateTime));
      QTextDocument docToPrint;
      docToPrint.setHtml(templateHtml);
      QPrinter printer(QPrinter::ScreenResolution);
      printer.setOutputFormat(QPrinter::NativeFormat);
      QPageSize pageSize =
          QPageSize(QSizeF(ticketWidth, ticketHeight), QPageSize::Millimeter);
      printer.setPageSize(pageSize);
      printer.setPaperSize(QSizeF(ticketWidth, ticketHeight),
                           QPrinter::Millimeter);
      printer.setPageMargins(QMarginsF(0, 2, 0, 2), QPageLayout::Millimeter);
      docToPrint.setPageSize(printer.pageRect().size());
      docToPrint.print(&printer);
    }
  }
}

void MainWindow::showPrintDialog() {
  print_blocking_dialog_->show();
  setButtonsEnabled(false);
  QTimer::singleShot(4000, print_blocking_dialog_, SLOT(hide()));
  QTimer::singleShot(4000, this, [=]() { this->setButtonsEnabled(true); });
}

void MainWindow::setButtonsEnabled(bool mode) {
  ui_->btnBook->setEnabled(mode);
  ui_->btnDisp->setEnabled(mode);
  ui_->btnEmergency->setEnabled(mode);
  ui_->btnOther->setEnabled(mode);
  ui_->btnReceipt->setEnabled(mode);
  ui_->btnReceipt->setEnabled(mode);
  ui_->btnReport->setEnabled(mode);
  ui_->btnVaccine->setEnabled(mode);
}

void MainWindow::on_btnBook_clicked() { sendTicketRequest(kBook); }

void MainWindow::on_btnDisp_clicked() { sendTicketRequest(kDisp); }

void MainWindow::on_btnReceipt_clicked() { sendTicketRequest(kReceipt); }

void MainWindow::on_btnEmergency_clicked() { sendTicketRequest(kEmergency); }

void MainWindow::on_btnReport_clicked() { sendTicketRequest(kReport); }

void MainWindow::on_btnVaccine_clicked() { sendTicketRequest(kVaccine); }

void MainWindow::on_btnOther_clicked() { sendTicketRequest(kOther); }

QNetworkCookie MainWindow::getCookie(const QNetworkReply &reply) const
    noexcept {
  QNetworkCookie result;
  if (reply.hasRawHeader(kSetCookieHeader)) {
    auto cookie_header = reply.rawHeader(kSetCookieHeader);
    auto cookie_parts = cookie_header.split(';');
    for (const auto &part : cookie_parts) {
      auto params = part.split('=');
      if (params.size() == 2) {
        if (params.first() == kSessionId) {
          result.setName(params.first());
          result.setValue(params.last());
        }
      }
    }
  }
  return result;
}

void MainWindow::replyFinished(QNetworkReply *reply) {
  auto error = reply->error();
  if (error != QNetworkReply::NetworkError::NoError) {
    qDebug() << error;
    using NetError = QNetworkReply::NetworkError;
    if (error == NetError::ContentAccessDenied ||
        error == NetError::ContentOperationNotPermittedError) {
      const auto &settings = AppSettings::getInstance();
      sendLoginRequest(settings.getUser(), settings.getPassword());
      qInfo() << "Sending login request...";
    } else {
      QMessageBox::critical(
          this, tr("Ошибка"),
          "Произошла ошибка при попытке получить номер талона",
          QMessageBox::Close);
    }

  } else {
    if (reply->url().path() == kLoginPath) {
      qInfo() << "Login's succesfull";
      auto cookie = getCookie(*reply);
      network_manager_->cookieJar()->insertCookie(cookie);
      network_manager_->post(last_request_.request, last_request_.data);
    } else {
      auto data = reply->readAll();
      qInfo() << data;
      printTicket(QJsonDocument::fromJson(data));
    }
  }
}
