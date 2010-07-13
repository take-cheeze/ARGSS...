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
		for(std::vector< boost::shared_ptr< RgssAdExtracter > >::iterator it = archives_.begin(); it < archives_.end(); ++it) {
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
			assert( std::fseek(fp, 0, SEEK_END) == 0 );
			unsigned int size = ftell(fp);
			assert( std::fseek(fp, 0, SEEK_SET) == 0 );
			files_[slash] = std::vector< uint8_t >(size);
			assert( std::fread( &(files_[slash][0]), sizeof(uint8_t), size, fp ) == size );
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
} // namespace FileIO
