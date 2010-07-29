#include <boost/smart_ptr.hpp>

#include <fstream>

#include "fileio.hxx"
#include "rgssad.hxx"

namespace FileIO
{
	std::string toSlash(std::string const& str)
	{
		std::string ret = str;
		for(std::string::iterator it = ret.begin(); it != ret.end(); ++it) if(*it == '\\') *it = '/';
		return ret;
	}
	std::string toYen(std::string const& str)
	{
		std::string ret = str;
		for(std::string::iterator it = ret.begin(); it != ret.end(); ++it) if(*it == '/') *it = '\\';
		return ret;
	}

	namespace
	{
		std::vector< boost::shared_ptr< RgssAdExtracter > > archives_;
		std::map< std::string, std::vector< uint8_t > > files_;

		char const* DEFAULT_ARCHIVE = "./Game.rgssad";
	} // namespace

	bool exists(std::string const& filename)
	{
		std::string yen = toYen(filename); // in a "rgssad" the separator is YEN sign
		for(std::vector< boost::shared_ptr< RgssAdExtracter > >::const_iterator it = archives_.begin(); it < archives_.end(); ++it) {
			if( (*it)->exists(yen) ) return true;
		}

		/*
		 * TODO: to absolute file path
		 */
		std::string slash = toSlash(filename);
		if( files_.find(slash) != files_.end() ) return true;

		std::ifstream file( slash.c_str() );
		return file.is_open();
	}

	std::vector< uint8_t > const& get(std::string const& name)
	{
		std::string slash = toSlash(name); // most filsystem uses slash as a separator
		assert( exists(slash) ); // the file mush exist

		std::string yen = toYen(name); // in a "rgssad" the separator is YEN sign
		for(std::vector< boost::shared_ptr< RgssAdExtracter > >::iterator it = archives_.begin(); it < archives_.end(); ++it) {
			if( (*it)->exists(yen) ) return (*it)->get(yen);
		}

		if( files_.find(slash) == files_.end() ) {
			std::FILE* fp = std::fopen( slash.c_str(), "rb" );
			assert(fp);
			int res;
			res = std::fseek(fp, 0, SEEK_END); assert(res == 0);
			unsigned int size = ftell(fp);
			res = std::fseek(fp, 0, SEEK_SET); assert(res == 0);
			files_[slash] = std::vector< uint8_t >(size);
			size_t ret = std::fread( &(files_[slash][0]), sizeof(uint8_t), size, fp ); assert(ret == size);
		}
		return files_.find(slash)->second;
	}
	void init()
	{
		if( exists(DEFAULT_ARCHIVE) ) {
			archives_.push_back(
				boost::shared_ptr< RgssAdExtracter >( new RgssAdExtracter(DEFAULT_ARCHIVE) )
			);
		}
	}

	std::vector< std::string > getRgssAdList()
	{
		std::vector< std::string > ret;
		for(std::vector< boost::shared_ptr< RgssAdExtracter > >::iterator it = archives_.begin(); it < archives_.end(); ++it) {
			for(std::multimap< std::string, RgssAdExtracter::Entry >::const_iterator it2 = (*it)->entry().begin(); it2 != (*it)->entry().end(); ++it2) {
				ret.push_back(it2->first);
			}
		}
		return ret;
	}
} // namespace FileIO
