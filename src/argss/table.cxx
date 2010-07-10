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

#include <cassert>
#include <stdint.h>

#include <map>
#include <string>
#include <vector>

#include <argss/table.hxx>
#include <output.hxx>


namespace ARGSS
{
	namespace ATable
	{
		namespace
		{
			template< typename IntegerType >
			class TableTemplate
			{
			private:
				std::vector< unsigned int > size_;
				std::vector< std::vector< std::vector< IntegerType > > > data_;
			public:
				void resize(std::vector< unsigned int > const& arg)
				{
					unsigned int xsizeDst = 1, ysizeDst = 1, zsizeDst = 1;
					unsigned int xsizeSrc = data_.size(), ysizeSrc = data_[0].size(), zsizeSrc = 1;
					switch( arg.size() ) {
						case 3: zsizeDst = arg[2];
						case 2: ysizeDst = arg[1];
						case 1: xsizeDst = arg[0];
							break;
						default: assert(false);
					}
					data_.resize( zsizeDst, std::vector< std::vector< IntegerType > >( ysizeDst, std::vector< IntegerType >(xsizeDst) ) );

					if( zsizeDst < zsizeSrc ) zsizeSrc = zsizeDst;
					if( ysizeDst < ysizeSrc ) ysizeSrc = ysizeDst;
					if( xsizeDst < xsizeSrc ) xsizeSrc = xsizeDst;
					for(unsigned int z = 0; z < zsizeSrc; z++) {
						data_[z].resize(ysizeDst);
						for(unsigned int y = 0; y < ysizeSrc; y++) data_.resize(xsizeDst);
					}

					size_ = arg;
				}

				TableTemplate(std::vector< unsigned int > const& sizes)
				{
					resize(sizes);
				}
				unsigned int xsize() const { assert(size_.size() >= 1); return size_[0]; }
				unsigned int ysize() const { assert(size_.size() >= 2); return size_[1]; }
				unsigned int zsize() const { assert(size_.size() >= 3); return size_[2]; }
				unsigned int size(unsigned int dim) { assert( size_.size() < (dim+1) ); return size_[dim]; }

				bool outOfRange(std::vector< unsigned int > const& arg) const
				{
					if( size_.size() != arg.size() ) return true;
					for(unsigned int i = 0; i < arg.size(); i++) if( arg[i] >= size_[i] ) return true;
					return false;
				}

				IntegerType& element(std::vector< unsigned int > const& arg)
				{
					assert( ( arg.size() == size_.size() ) && !outOfRange(arg) );

					unsigned int x = 0, y = 0, z = 0;
					switch( arg.size() ) {
						case 3: z = arg[2];
						case 2: y = arg[1];
						case 1: x = arg[0];
							break;
						default: assert(false);
					}
					return data_[z][y][x];
				}
				IntegerType const& element(std::vector< unsigned int > const& arg) const
				{
					assert( ( arg.size() == size_.size() ) && !outOfRange(arg) );

					unsigned int x = 0, y = 0, z = 0;
					switch( arg.size() ) {
						case 3: z = arg[2];
						case 2: y = arg[1];
						case 1: x = arg[0];
							break;
						default: assert(false);
					}
					return data_[z][y][x];
				}
			};
			typedef TableTemplate< int16_t > Table;
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
			std::map< VALUE, boost::shared_ptr< Table > > tables_;
		}
		VALUE& getID() { return id; }

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
				}
				else {
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
		VALUE rdump(int argc, VALUE* argv, VALUE self)
		{
			if (argc > 1) raise_argn(argc, 1);
			VALUE str = rb_str_new2("");
			VALUE xsize = rb_iv_get(self, "@xsize");
			VALUE ysize = rb_iv_get(self, "@ysize");
			VALUE zsize = rb_iv_get(self, "@zsize");
			VALUE items = NUM2INT(xsize) * NUM2INT(ysize) *NUM2INT(zsize);
			VALUE arr = rb_ary_new3(5, rb_iv_get(self, "@dim"), xsize, ysize, zsize, INT2NUM(items));
			rb_str_concat(str, rb_funcall(arr, rb_intern("pack"), 1, rb_str_new2("L5")));
			rb_str_concat(str, rb_funcall(rb_iv_get(self, "@data"), rb_intern("pack"), 1, rb_str_times(rb_str_new2("S"), INT2NUM(items))));
			return str;
		}
		VALUE rload(VALUE self, VALUE str)
		{
			VALUE arr = rb_funcall(str, rb_intern("unpack"), 1, rb_str_new2("L5"));
			int dim = NUM2INT(rb_ary_entry(arr, 0));
			VALUE items = NUM2INT(rb_ary_entry(arr, 4));
			VALUE args[3] = {rb_ary_entry(arr, 1), rb_ary_entry(arr, 2), rb_ary_entry(arr, 3)};
			VALUE table = rb_class_new_instance(dim, args, id);
			VALUE data = rb_funcall(rb_str_substr(str, 20, items * 2), rb_intern("unpack"), 1, rb_str_times(rb_str_new2("S"), INT2NUM(items)));
			rb_iv_set(table, "@data", data);
			return table;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Console initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_class("Table", rb_cObject);
			static FuncTable funcTable =
			{
				{ ARGSS_FUNC(initialize), -1 },
				{ ARGSS_FUNC(resize), -1 },
				{ ARGSS_FUNC(xsize), 0 }, { ARGSS_FUNC(ysize), 0 }, { ARGSS_FUNC(zsize), 0 },
				{ "[]", RubyFunc(raref), -1 }, { "[]=", RubyFunc(raset), -1 },
				{ "_dump", RubyFunc(rdump), -1 },
			};
			defineMethods(id, funcTable);
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
