/*
 * RGSSAD Extracter's C++ Implementatio
 * thank's for http://twitter.com/rgssws4m 's ruby implementation
 */

// for debug
#include <iostream>

#include "rgssad.hxx"


uint8_t  RgssAdExtracter::get_uint8_t () { return 0x0; }
uint32_t RgssAdExtracter::get_uint32_t() { return 0x0; }

std::string RgssAdExtracter::get_string(unsigned int size)
{
	return std::string();
}

void RgssAdExtracter::update_magic() { magic_ = 0x0; }
std::string RgssAdExtracter::decrypt_string(std::string const& str)
{
	return std::string();
}
uint32_t RgssAdExtracter::decrypt_uint32_t(uint32_t number)
{
	return 0x0;
}

std::vector< uint8_t > RgssAdExtracter::extract(Entry const& ent)
{
	return std::vector< uint8_t >(ent.size);
}

void RgssAdExtracter::init()
{
}
RgssAdExtracter::RgssAdExtracter(std::string const& name)
: fileName_(name), filePointer_(NULL), magic_(0x0)
{
}
