#include <QMutexLocker>
#include "appsettings.h"

const QString AppSettings::CONNECTION_GROUP = "/connetion";
const QString AppSettings::SERVER_ADDR = "server_addr";
const QString AppSettings::PORT = "port";
const QString AppSettings::MAIN_WINDOW_GROUP = "/main_window";
const QString AppSettings::ACTION_FONT_SIZE = "action_font_size";
const QString AppSettings::TICKET_GROUP = "/ticket";
const QString AppSettings::TICKET_WIDTH = "ticket_width";
const QString AppSettings::TICKET_HEIGHT = "ticket_height";

AppSettings& AppSettings::getInstance()
{
    static AppSettings instance;
    return instance;
}

AppSettings::AppSettings()
{
    ReadSettings();
}

void AppSettings::setConnectionSettings(const QString& server_address,
                                        const int port)
{
    QMutexLocker locker(&m);
    server_address_ = server_address;
    port_ = port;
    settings_.beginGroup(CONNECTION_GROUP);
    settings_.setValue(SERVER_ADDR, server_address_);
    settings_.setValue(PORT, port_);
    settings_.endGroup();
}

void AppSettings::setMainWindowSettings(const int action_font_size)
{
    QMutexLocker locker(&m);
    action_font_size_ = action_font_size;
    settings_.beginGroup(MAIN_WINDOW_GROUP);
    settings_.setValue(ACTION_FONT_SIZE, action_font_size_);
    settings_.endGroup();
}

void AppSettings::setTicketSettings(const int ticket_width,
                                    const int ticket_height)
{
    QMutexLocker locker(&m);
    ticket_width_ = ticket_width;
    ticket_height_ = ticket_height;
    settings_.beginGroup(TICKET_GROUP);
    settings_.setValue(TICKET_WIDTH, ticket_width_);
    settings_.setValue(TICKET_HEIGHT, ticket_height_);
    settings_.endGroup();
}

QString AppSettings::getServerAddress() const
{
    return server_address_;
}

int AppSettings::getPort() const
{
    return port_;
}

int AppSettings::getActionFontSize() const
{
    return action_font_size_;
}

int AppSettings::getTicketWidth() const
{
    return ticket_width_;
}

int AppSettings::getTicketHeight() const
{
    return ticket_height_;
}

void AppSettings::ReadSettings()
{
    QMutexLocker locker(&m);
    settings_.beginGroup(CONNECTION_GROUP);
    server_address_ = settings_.value(SERVER_ADDR, "192.168.0.15").toString();
    port_ = settings_.value(PORT, 8085).toInt();
    settings_.endGroup();
    settings_.beginGroup(MAIN_WINDOW_GROUP);
    action_font_size_ = settings_.value(ACTION_FONT_SIZE, 48).toInt();
    settings_.endGroup();
    settings_.beginGroup(TICKET_GROUP);
    ticket_width_ = settings_.value(TICKET_WIDTH, 100).toInt();
    ticket_height_ = settings_.value(TICKET_HEIGHT, 90).toInt();
    settings_.endGroup();
}
