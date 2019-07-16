#include "appsettings.h"
#include <QMutexLocker>

const QString AppSettings::kConnectionGroup = "/connetion";
const QString AppSettings::kServerAddr = "server_addr";
const QString AppSettings::kPort = "port";
const QString AppSettings::kUser = "user";
const QString AppSettings::kPassword = "password";
const QString AppSettings::kMainWindowGroup = "/main_window";
const QString AppSettings::kActionFontSize = "action_font_size";
const QString AppSettings::kTicketGroup = "/ticket";
const QString AppSettings::kTicketWidth = "ticket_width";
const QString AppSettings::kTicketHeight = "ticket_height";

AppSettings& AppSettings::getInstance() {
  static AppSettings instance;
  return instance;
}

AppSettings::AppSettings() { ReadSettings(); }

void AppSettings::setConnectionSettings(const QString& server_address,
                                        const int port, const QString& user,
                                        const QString& password) {
  QMutexLocker locker(&m);
  server_address_ = server_address;
  port_ = port;
  user_ = user;
  password_ = password;
  settings_.beginGroup(kConnectionGroup);
  settings_.setValue(kServerAddr, server_address_);
  settings_.setValue(kPort, port_);
  settings_.setValue(kUser, user_);
  settings_.setValue(kPassword, password_);
  settings_.endGroup();
}

void AppSettings::setMainWindowSettings(const int action_font_size) {
  QMutexLocker locker(&m);
  action_font_size_ = action_font_size;
  settings_.beginGroup(kMainWindowGroup);
  settings_.setValue(kActionFontSize, action_font_size_);
  settings_.endGroup();
}

void AppSettings::setTicketSettings(const int ticket_width,
                                    const int ticket_height) {
  QMutexLocker locker(&m);
  ticket_width_ = ticket_width;
  ticket_height_ = ticket_height;
  settings_.beginGroup(kTicketGroup);
  settings_.setValue(kTicketWidth, ticket_width_);
  settings_.setValue(kTicketHeight, ticket_height_);
  settings_.endGroup();
  settings_.sync();
}

QString AppSettings::getServerAddress() const { return server_address_; }

int AppSettings::getPort() const { return port_; }

QString AppSettings::getUser() const { return user_; }

QString AppSettings::getPassword() const { return password_; }

int AppSettings::getActionFontSize() const { return action_font_size_; }

int AppSettings::getTicketWidth() const { return ticket_width_; }

int AppSettings::getTicketHeight() const { return ticket_height_; }

void AppSettings::ReadSettings() {
  QMutexLocker locker(&m);
  settings_.beginGroup(kConnectionGroup);
  server_address_ = settings_.value(kServerAddr, "192.168.0.9").toString();
  port_ = settings_.value(kPort, 8085).toInt();
  user_ = settings_.value(kUser, "add_ticket_service").toString();
  password_ = settings_.value(kPassword).toString();
  settings_.endGroup();
  settings_.beginGroup(kMainWindowGroup);
  action_font_size_ = settings_.value(kActionFontSize, 48).toInt();
  settings_.endGroup();
  settings_.beginGroup(kTicketGroup);
  ticket_width_ = settings_.value(kTicketWidth, 100).toInt();
  ticket_height_ = settings_.value(kTicketHeight, 90).toInt();
  settings_.endGroup();
}
