#include "msgbox_qt.h"
#include "player.h"

#include <QtGui/QMessageBox>

void MsgBox::OK(std::string const& msg, std::string const& title)
{
	QMessageBox::information( &Player::getMainWindow(), title.c_str(), msg.c_str() );
}
void MsgBox::Error(std::string const& msg, std::string const& title)
{
	QMessageBox::critical( &Player::getMainWindow(), title.c_str(), msg.c_str() );
}
void MsgBox::Warning(std::string const& msg, std::string const& title)
{
	QMessageBox::warning( &Player::getMainWindow(), title.c_str(), msg.c_str() );
}
