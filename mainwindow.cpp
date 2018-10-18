#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextDocument>
#include <QDateTime>
#include <QFile>
#include <QPainter>
#include <QKeyEvent>
#include <QDebug>
#include "dlgsettings.h"
#include "appsettings.h"

const QString MainWindow::TICKET_NUMBER_PARAM = "ticket_number";
const QString MainWindow::ACTION_PARAM = "action";
const QString MainWindow::BOOK_ACTION = "book";
const QString MainWindow::DISP_ACTION = "disp";
const QString MainWindow::RECEIPT_ACTION = "receipt";
const QString MainWindow::EMERGENCY_ACTION = "emergency";
const QString MainWindow::REPORT_ACTION = "report";
const QString MainWindow::VACCINE_ACTION = "vaccine";
const QString MainWindow::OTHER_ACTION = "other";
const QMap<QString, QString> MainWindow::ACTIONS_DESCRIBES = {
    {BOOK_ACTION, "Плановая запись"},
    {DISP_ACTION, "Диспансеризация"},
    {RECEIPT_ACTION, "Выписка льготного рецепта"},
    {EMERGENCY_ACTION, "На приём по острому состоянию"},
    {REPORT_ACTION, "Санаторно-курортная карта, операция, справка, выписка"},
    {VACCINE_ACTION, "Вакцинация (клещ, грип)"},
    {OTHER_ACTION, "Прочее"},
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QTimer::singleShot(1000, this, SLOT(showFullScreen()));

    QFile styleFile;
    styleFile.setFileName(":style/qss/style.qss");
    if (styleFile.open(QFile::ReadOnly) == false) {
        qDebug() << "Не найден файл стилей";
    } else {
        QString stylesheet = styleFile.readAll();
        setStyleSheet(stylesheet);
    }
    printBlockingDialog = new PrintBlockingDialog(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::KeyboardModifier::AltModifier) {
        if (event->key() == Qt::Key::Key_S) {
            DlgSettings dlgSettins;
            dlgSettins.exec();
        }
    }
}

void MainWindow::sendTicketRequest(const QString &action)
{
    AppSettings& settings = AppSettings::getInstance();
    QString address = QString("http://%1:%2/tickets")
            .arg(settings.getServerAddress())
            .arg(settings.getPort());
    QUrl url(address);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery params;
    params.addQueryItem(ACTION_PARAM, action);
    mNetworkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void MainWindow::printTicket(const QJsonDocument &ticket)
{
    if (ticket.isNull() == true) {
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              tr("Неверный формат талона"),
                              QMessageBox::Close);
    } else {
        QFile ticketTemplateFile;
        ticketTemplateFile.setFileName(":/printTemplates/printTemplates/ticket.html");

        if (ticketTemplateFile.open(QFile::ReadOnly) == false) {
            qDebug() << "Не найден файл шаблона чека";
        } else {
            showPrintDialog();
            QJsonObject jsonTicket = ticket.object();
            AppSettings& settings = AppSettings::getInstance();
            int ticketWidth = settings.getTicketWidth();
            int ticketHeight = settings.getTicketHeight();
            QString templateHtml = ticketTemplateFile.readAll();
            QString ticketNumber = jsonTicket.value(TICKET_NUMBER_PARAM).toVariant().toString();
            templateHtml.replace("{ticket}", std::move(ticketNumber));
            QString actionName = jsonTicket.value(ACTION_PARAM).toVariant().toString();
            templateHtml.replace("{action}", ACTIONS_DESCRIBES.find(actionName).value());
            QString currentDateTime = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");
            templateHtml.replace("{datetime}", std::move(currentDateTime));
            QTextDocument docToPrint;
            docToPrint.setHtml(templateHtml);
            QPrinter printer(QPrinter::ScreenResolution);
            printer.setOutputFormat(QPrinter::NativeFormat);
            QPageSize pageSize = QPageSize(QSizeF(ticketWidth, ticketHeight), QPageSize::Millimeter);
            printer.setPageSize(pageSize);
            printer.setPaperSize(QSizeF(ticketWidth, ticketHeight), QPrinter::Millimeter);
            printer.setPageMargins(QMarginsF(0, 2, 0, 2), QPageLayout::Millimeter);
            docToPrint.setPageSize(printer.pageRect().size());
            docToPrint.print(&printer);
        }
    }
}

void MainWindow::showPrintDialog()
{
    printBlockingDialog->show();
    setButtonsEnabled(false);
    QTimer::singleShot(3000, printBlockingDialog, SLOT(hide()));
    QTimer::singleShot(3000, this, [=](){this->setButtonsEnabled(true);});
}

void MainWindow::setButtonsEnabled(bool mode)
{
    ui->btnBook->setEnabled(mode);
    ui->btnDisp->setEnabled(mode);
    ui->btnEmergency->setEnabled(mode);
    ui->btnOther->setEnabled(mode);
    ui->btnReceipt->setEnabled(mode);
    ui->btnReceipt->setEnabled(mode);
    ui->btnReport->setEnabled(mode);
    ui->btnVaccine->setEnabled(mode);
}

void MainWindow::on_btnBook_clicked()
{
    sendTicketRequest(BOOK_ACTION);
}

void MainWindow::on_btnDisp_clicked()
{
    sendTicketRequest(DISP_ACTION);
}

void MainWindow::on_btnReceipt_clicked()
{
    sendTicketRequest(RECEIPT_ACTION);
}

void MainWindow::on_btnEmergency_clicked()
{
    sendTicketRequest(EMERGENCY_ACTION);
}

void MainWindow::on_btnReport_clicked()
{
    sendTicketRequest(REPORT_ACTION);
}

void MainWindow::on_btnVaccine_clicked()
{
    sendTicketRequest(VACCINE_ACTION);
}

void MainWindow::on_btnOther_clicked()
{
    sendTicketRequest(OTHER_ACTION);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        QMessageBox::critical(this,
                              tr("Ошибка"),
                              "Произошла ошибка при попытке получить номер талона",
                              QMessageBox::Close);
    } else {
        auto data = reply->readAll();
        qDebug() << data;
        printTicket(QJsonDocument::fromJson(data));
    }
}
