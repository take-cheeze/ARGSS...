#ifndef _TABLE_HXX_
#define _TABLE_HXX_

#include <boost/noncopyable.hpp>
#include <cassert>
#include <stdint.h>
#include <vector>


template< typename IntegerType >
class TableTemplate : public boost::noncopyable
{
public:
	typedef std::vector< unsigned int >  SizeType;
	typedef std::vector< unsigned int > IndexType;
private:
	SizeType size_;
	std::vector< IntegerType > data_;
protected:
	bool outOfRange(IndexType const& arg, SizeType const& size) const
	{
		for(unsigned int i = 0; i < arg.size(); i++) {
			assert( size[i] );
			if( arg[i] >= size[i] ) return true;
		}
		return false;
	}
	bool outOfRange(IndexType const& arg) const { return outOfRange(arg, size_); }

	unsigned int getIndex(IndexType const& arg, SizeType const& size) const
	{
		unsigned int ret = arg.back();
		for(int i = arg.size() - 2; i >= 0; i--) { ret *= size[i]; ret += arg[i]; }
		return ret;
	}
	unsigned int getIndex(IndexType const& arg) const { return getIndex(arg, size_); }
	unsigned int fullSize(SizeType const& size) const
	{
		int ret = 1;
		for(unsigned int i = 0; i < size.size(); i++) ret *= size[i];
		return ret;
	}
public:
	unsigned int fullSize() const { return fullSize(size_); }

	void resize(SizeType const& arg)
	{
		std::vector< IntegerType > const& src = data_;
		std::vector< IntegerType > dst( fullSize() );
		SizeType const& srcSize = size_;
		SizeType const& dstSize = arg;

		unsigned int cpySize = sizeof(IntegerType);
		cpySize *= dstSize.front() > srcSize.front() ? srcSize.front() : dstSize.front();

		int j_len = dstSize.size() < srcSize.size() ? srcSize.size() : dstSize.size();
		for(int i = 0, i_len = fullSize(srcSize) / srcSize.front(); i < i_len; i++) {
			IndexType dstIndex( j_len );
			unsigned int buf = i;
			for(int j = 1; j < j_len; j++) {
				dstIndex[j] = buf % srcSize[j];
				buf /= srcSize[j];
			}
			IndexType srcIndex = dstIndex;
			srcIndex.resize( srcSize.size() );
			dstIndex.resize( dstSize.size() );
			if( outOfRange(dstIndex, dstSize) ) continue;
			std::memcpy( &(dst[ getIndex(dstIndex, dstSize) ]), &(src[ getIndex(srcIndex, srcSize) ]), cpySize );
		}

		size_ = dstSize;
		data_ = dst;
	}

	TableTemplate(SizeType const& sizes) : size_(sizes), data_( fullSize(sizes) )
	{
	}
	unsigned int size(unsigned int dim) const
	{
		if( dim < size_.size() ) return size_[dim];
		else return 1;
	}
	unsigned int xsize() const { return size(0); }
	unsigned int ysize() const { return size(1); }
	unsigned int zsize() const { return size(2); }

	IntegerType& operator [](IndexType const& arg)
	{
		assert( arg.size() == size_.size() );
		assert( !outOfRange(arg) );
		return data_[ getIndex(arg) ];
	}
	IntegerType const& operator [](IndexType const& arg) const
	{
		assert( arg.size() == size_.size() );
		assert( !outOfRange(arg) );
		return data_[ getIndex(arg) ];
	}

	IntegerType& operator [](unsigned int index) { return data_[index]; }
	IntegerType const& operator [](unsigned int index) const { return data_[index]; }
	unsigned int const dimension() const { return size_.size(); }
};
typedef TableTemplate< int16_t > Table;

#endif // _TABLE_HXX_
