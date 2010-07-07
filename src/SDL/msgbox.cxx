#include "msgbox.hxx"
#include "../player.hxx"

#include <iostream>

void MsgBox::OK(std::string const& msg, std::string const& title)
{
	std::cout << "Message: " << title << ", " << msg << std::endl;
	std::cin.get();
}
void MsgBox::Error(std::string const& msg, std::string const& title)
{
	std::cout << "Error: " << title << ", " << msg << std::endl;
	std::cin.get();
}
void MsgBox::Warning(std::string const& msg, std::string const& title)
{
	std::cout << "Warning: " << title << ", " << msg << std::endl;
	std::cin.get();
}
