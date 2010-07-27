//////////////////////////////////////////////////////////////////////////////////
/// ARGSS - Copyright (c) 2009 - 2010, Alejandro Marzini (vgvgf)
///         All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///     * Redistributions of source code must retain the above copyright
///       notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above copyright
///       notice, this list of conditions and the following disclaimer in the
///       documentation and/or other materials provided with the distribution.
///
/// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <boost/smart_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <cassert>
#include <climits>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>

#include "table.hxx"
#include "../output.hxx"


namespace ARGSS
{
	namespace ATable
	{
		namespace
		{
			int TABLE_MAX = 32767, TABLE_MIN = -32768;

			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
			boost::unordered_map< VALUE, boost::shared_ptr< Table > > tables_;

			std::vector< unsigned int > val2arg(int argc, VALUE* argv)
			{
				Table::IndexType ret(argc);
				for(int i = 0; i < argc; i++) ret[i] = NUM2INT(argv[i]);
				return ret;
			}
		}
		VALUE& getID() { return id; }
		Table& getTable(VALUE self)
		{
			assert( tables_.find(self) != tables_.end() );
			return *( tables_.find(self)->second );
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Table ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc < 1) raise_argn(argc, 1);
			else if (argc > 3) raise_argn(argc, 3);

			assert( tables_.insert(
				boost::unordered_map< VALUE, boost::shared_ptr< Table > >::value_type(
					self, boost::shared_ptr< Table >( new Table( val2arg(argc, argv) ) )
				)
			).second );

			return self;
		}
		VALUE rdispose(VALUE self)
		{
			boost::unordered_map< VALUE, boost::shared_ptr< Table > >::iterator it = tables_.find(self);
			if( it != tables_.end() ) {
				tables_.erase(it);
				ARGSS::ARuby::RemoveObject(self);
				rb_gc_start();
			}
			return self;
		}
		VALUE rdisposedQ(VALUE self)
		{
			return INT2BOOL( tables_.find(self) == tables_.end() );
		}
		VALUE rresize(int argc, VALUE *argv, VALUE self)
		{
			getTable(self).resize( val2arg(argc, argv) );
			return self;
		}
		VALUE rxsize(VALUE self) { return INT2NUM( getTable(self).xsize() ); }
		VALUE rysize(VALUE self) { return INT2NUM( getTable(self).ysize() ); }
		VALUE rzsize(VALUE self) { return INT2NUM( getTable(self).zsize() ); }
		VALUE raref(int argc, VALUE *argv, VALUE self)
		{
			return INT2NUM( int( getTable(self)[ val2arg(argc, argv) ] ) );
		}
		VALUE raset(int argc, VALUE *argv, VALUE self)
		{
			Table& table = getTable(self);
			unsigned int const dim = table.dimension();
			assert( (dim + 1) == argc );

			int clamped = NUM2INT( argv[dim] );
			if(clamped > TABLE_MAX) clamped = TABLE_MAX;
			if(clamped < TABLE_MIN) clamped = TABLE_MIN;
			table[ val2arg(dim, argv) ] = clamped;

			return argv[dim];
		}
		// struct Table
		// {
		// 	uint32_t dim;
		// 	uint32_t xsize, ysize, zsize;
		// 	uint32_t data_size
		// 	int16_t data[data_size];
		// };
		template< typename T >
		void setLE(uint8_t* &dst, T const& val)
		{
			for(unsigned int i = 0; i < sizeof(T); i++) *(dst++) = ( val >> (CHAR_BIT * i) );
		}
		template< typename T >
		T getLE(uint8_t const* &src)
		{
			T ret = 0;
			for(unsigned int i = 0; i < sizeof(T); i++) ret |= ( *(src++) << (CHAR_BIT * i) );
			return ret;
		}
		VALUE rdump(int argc, VALUE* argv, VALUE self)
		{
			assert(argc == 1);
			Table const& table = getTable(self);
			std::vector< uint8_t > ret( sizeof(uint32_t) * 5 + sizeof(int16_t) * table.fullSize() );
			uint8_t* p = &(ret[0]);

			assert( 0 < table.dimension() && table.dimension() <= 3 );
			setLE( p, uint32_t( table.dimension() ) );
			for(int i = 0; i < 3; i++) {
				setLE( p, uint32_t( table.size(i) ) );
			}
			setLE( p, uint32_t( table.fullSize() ) );
			for(unsigned int i = 0; i < table.fullSize(); i++) setLE( p, int16_t( table[i] ) );
			return rb_str_new( reinterpret_cast< char const* >( &(ret[0]) ), ret.size() );
		}
		VALUE rload(VALUE self, VALUE str)
		{
			uint8_t const* p = reinterpret_cast< uint8_t const* >( RSTRING_PTR(str) );
			unsigned int dim = getLE< uint32_t >(p);
			assert( 0 < dim && dim <= 3 );
			VALUE argv[3];
			for(int i = 0; i < 3; i++) {
				argv[i] = INT2NUM( getLE< uint32_t >(p) );
			}
			VALUE table = rb_class_new_instance(dim, argv, getID());
			Table& tableRef = getTable(table);
			unsigned int data_size = getLE< uint32_t >(p);
			for(unsigned int i = 0; i < data_size; i++) tableRef[i] = getLE< int16_t >(p);
			return table;
		}
/*
		////////////////////////////////////////////////////////////
		/// ARGSS Table ruby functions
		////////////////////////////////////////////////////////////
		VALUE rinitialize(int argc, VALUE *argv, VALUE self)
		{
			if (argc < 1) raise_argn(argc, 1);
			else if (argc > 3) raise_argn(argc, 3);
			rb_iv_set(self, "@dim", INT2NUM(argc));
			int xsize = 1;
			int ysize = 1;
			int zsize = 1;
			switch(argc) {
				case 3: zsize = NUM2INT(argv[2]);
				case 2: ysize = NUM2INT(argv[1]);
				case 1: xsize = NUM2INT(argv[0]);
					break;
			}
			rb_iv_set(self, "@xsize", INT2NUM(xsize));
			rb_iv_set(self, "@ysize", INT2NUM(ysize));
			rb_iv_set(self, "@zsize", INT2NUM(zsize));
			VALUE arr = rb_ary_new3(xsize * ysize * zsize, INT2NUM(0));
			rb_iv_set(self, "@data", arr);
			rb_iv_set(self, "@modified", Qtrue);
			return self;
		}
		VALUE rresize(int argc, VALUE *argv, VALUE self)
		{
			int xsize = 1;
			int ysize = 1;
			int zsize = 1;
			switch(argc) {
				case 3: zsize = NUM2INT(argv[2]);
				case 2: ysize = NUM2INT(argv[1]);
				case 1: xsize = NUM2INT(argv[0]);
					break;
			}
			int nsize = xsize * ysize * zsize;
			int osize;
			osize = NUM2INT(rb_iv_get(self, "@xsize")) * NUM2INT(rb_iv_get(self, "@xsize")) *
					NUM2INT(rb_iv_get(self, "@xsize"));
			if (nsize != osize) {
				if (nsize > osize) {
					VALUE arr = rb_ary_new3(nsize - osize, INT2NUM(0));
					rb_ary_concat(rb_iv_get(self, "@data"), arr);
				} else {
					VALUE slice_argv[2];
					slice_argv[0] = INT2NUM(nsize);
					slice_argv[1] = INT2NUM(osize);
					rb_funcall2(rb_iv_get(self, "@data"), rb_intern("slice!"), 2, slice_argv);
				}
				rb_iv_set(self, "@modified", Qtrue);
			}
			return self;
		}
		VALUE rxsize(VALUE self) { return rb_iv_get(self, "@xsize"); }
		VALUE rysize(VALUE self) { return rb_iv_get(self, "@ysize"); }
		VALUE rzsize(VALUE self) { return rb_iv_get(self, "@zsize"); }
		VALUE raref(int argc, VALUE *argv, VALUE self)
		{
			int dim = NUM2INT(rb_iv_get(self, "@dim"));
			if (argc != dim) raise_argn(argc, dim);
			int x = 0;
			int y = 0;
			int z = 0;
			switch(argc) {
				case 3: z = NUM2INT(argv[2]);
				case 2: y = NUM2INT(argv[1]);
				case 1: x = NUM2INT(argv[0]);
					break;
			}
			VALUE data = rb_iv_get(self, "@data");
			int xsize = NUM2INT(rb_iv_get(self, "@xsize"));
			int ysize = NUM2INT(rb_iv_get(self, "@ysize"));
			int zsize = NUM2INT(rb_iv_get(self, "@zsize"));
			if (x >= xsize || y >= ysize || z >= zsize) return Qnil;
			else return rb_ary_entry(data, x + y * xsize + z * xsize * ysize);
		}
		VALUE raset(int argc, VALUE *argv, VALUE self)
		{
			int dim = NUM2INT(rb_iv_get(self, "@dim"));
			if (argc != (dim + 1)) raise_argn(argc, dim + 1);
			int x = 0;
			int y = 0;
			int z = 0;
			switch(argc) {
				case 3: z = NUM2INT(argv[2]);
				case 2: y = NUM2INT(argv[1]);
				case 1: x = NUM2INT(argv[0]);
					break;
			}
			int val = NUM2INT(argv[argc - 1]);
			if (val > 65535) val = 65535;
			else if (val < 0) val = 0;
			VALUE data = rb_iv_get(self, "@data");
			int xsize = NUM2INT(rb_iv_get(self, "@xsize"));
			int ysize = NUM2INT(rb_iv_get(self, "@ysize"));
			int zsize = NUM2INT(rb_iv_get(self, "@zsize"));
			if (x < xsize && y < ysize && z < zsize) {
				if (INT2NUM(val) != rb_ary_entry(data, x + y * xsize + z * xsize * ysize)) {
					rb_iv_set(self, "@modified", Qtrue);
				}
				rb_ary_store(data, x + y * xsize + z * xsize * ysize, INT2NUM(val));
			}
			return argv[argc - 1];
		}
		// struct Table
		// {
		// 	uint32_t dim;
		// 	uint32_t xsize, ysize, zsize;
		// 	uint32_t data_size
		// 	int16_t data[data_size];
		// };
		VALUE rdump(int argc, VALUE* argv, VALUE self)
		{
			if (argc > 1) raise_argn(argc, 1);
			VALUE str = rb_str_new2("");
			VALUE xsize = rb_iv_get(self, "@xsize");
			VALUE ysize = rb_iv_get(self, "@ysize");
			VALUE zsize = rb_iv_get(self, "@zsize");
			VALUE items = NUM2INT(xsize) * NUM2INT(ysize) *NUM2INT(zsize);
			VALUE arr = rb_ary_new3(5, rb_iv_get(self, "@dim"), xsize, ysize, zsize, INT2NUM(items));
			rb_str_concat(str, rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("V5")));
			rb_str_concat(str, rb_funcall(rb_iv_get(self, "@data"), rb_intern("pack"), 1, rb_str_times(rb_str_new2("v"), INT2NUM(items))));
			return str;
		}
		VALUE rload(VALUE self, VALUE str)
		{
			VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("V5"));
			int dim = NUM2INT(rb_ary_entry(arr, 0));
			VALUE items = NUM2INT(rb_ary_entry(arr, 4));
			VALUE args[3] = {rb_ary_entry(arr, 1), rb_ary_entry(arr, 2), rb_ary_entry(arr, 3)};
			VALUE table = rb_class_new_instance(dim, args, id);
			VALUE data = rb_funcall(rb_str_substr(str, 20, items * 2), rb_intern("unpack"), 1, rb_str_times(rb_str_new2("v"), INT2NUM(items)));
			rb_iv_set(table, "@data", data);
			return table;
		}
 */
		////////////////////////////////////////////////////////////
		/// ARGSS Console initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Table", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(dispose), 0 }, { ARGSS_Q(disposed), 0 },
				{ ARGSS_FUNC(resize), -1 },
				{ ARGSS_FUNC(xsize), 0 }, { ARGSS_FUNC(ysize), 0 }, { ARGSS_FUNC(zsize), 0 },
				{ "[]", RubyFunc(raref), -1 }, { "[]=", RubyFunc(raset), -1 },
				{ "_dump", RubyFunc(rdump), -1 },
			};
			defineMethods(getID(), funcTable);
			rb_define_singleton_method(id, "_load", RubyFunc(rload), 1);
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Table new ruby instance
		////////////////////////////////////////////////////////////
		VALUE New(int xsize)
		{
			VALUE args[1] = {INT2NUM(xsize)};
			return rb_class_new_instance(1, args, id);
		}
		VALUE New(int xsize, int ysize)
		{
			VALUE args[2] = {INT2NUM(xsize), INT2NUM(ysize)};
			return rb_class_new_instance(2, args, id);
		}
		VALUE New(int xsize, int ysize, int zsize)
		{
			VALUE args[3] = {INT2NUM(xsize), INT2NUM(ysize), INT2NUM(zsize)};
			return rb_class_new_instance(3, args, id);
		}
	} // namespace ATable
} // namespace ARGSS
