#ifndef _RGSSAD_EXTRACTER_
#define _RGSSAD_EXTRACTER_

/*
 * RGSSAD Extracter's C++ Implementatio
 * thank you for http://twitter.com/rgssws4m 's ruby implementation
 */

#include <cassert>
#include <stdint.h>
#include <cstdio>

#include <map>
#include <string>
#include <vector>


class RgssAdExtracter
{
private:
	std::string fileName_;
	std::FILE* filePointer_;
	uint8_t version_;
	uint32_t magic_;
public:
	struct Entry
	{
		std::string name;
		unsigned int pos;
		unsigned int size;
		uint32_t magic;
	};
private:
	std::multimap< std::string, Entry > entries_;
	std::map< std::string, std::vector< uint8_t > > data_;
protected:
	// little endian
	uint8_t  get_uint8_t ();
	uint32_t get_uint32_t();

	std::string get_string(unsigned int size);

	void update_magic();
	std::string decrypt_string(std::string const& str);
	uint32_t decrypt_uint32_t(uint32_t number);

	std::vector< uint8_t > extract(Entry const& ent);

	void init();
public:
	RgssAdExtracter(std::string const& name);
	~RgssAdExtracter() { if(filePointer_) std::fclose(filePointer_); }

	std::vector< uint8_t > const& get(std::string const& name)
	{
		assert( entries_.find(name) != entries_.end() );
		if( data_.find(name) == data_.end() ) data_[name] = extract( entries_.find(name)->second );
		return data_.find(name)->second;
	}
	bool exists(std::string const& name) const { return entries_.find(name) != entries_.end(); }

	std::multimap< std::string, Entry > const& entry() const { return entries_; }
	std::multimap< std::string, Entry >& entry() { return entries_; }
};

#endif
