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

#ifndef _ARGSS_RUBY_HXX_
#define _ARGSS_RUBY_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <rubybind/rubybind.hxx>
#include <string>

////////////////////////////////////////////////////////////
/// ARGSS::ARuby namespace
////////////////////////////////////////////////////////////
namespace ARGSS
{
	typedef VALUE (*RubyFunc)(...);
	struct FuncTableElement { char const* name; RubyFunc func; unsigned int argc; };
	typedef FuncTableElement FuncTable[];
	struct AttrTableElement { char const* name; bool read, write; };
	typedef AttrTableElement AttrTable[];

	#define ARGSS_FUNC(name) #name, RubyFunc(r##name)
	#define ARGSS_E(name) #name "=", RubyFunc(r##name##E)
	#define ARGSS_Q(name) #name "?", RubyFunc(r##name##Q)

	#define ARGSS_GETTER_SETTER(name) { ARGSS_FUNC(name), 0 }, { ARGSS_E(name), 1 }

	void defineMethodsImplement(VALUE klassID, FuncTableElement const* table, unsigned int elmNum);
	template< unsigned int ElmNum >
	void defineMethods(VALUE klassID, FuncTableElement const (&table)[ElmNum])
	{
		defineMethodsImplement(klassID, table, ElmNum);
	}
	void defineAttributesImplement(VALUE klassID, AttrTableElement const* table, unsigned int elmNum);
	template< unsigned int ElmNum >
	void defineAttributes(VALUE klassID, AttrTableElement const (&table)[ElmNum])
	{
		defineMethodsImplement(klassID, table, ElmNum);
	}

	inline VALUE BOOL2NUM(bool val) { return val ? Qtrue : Qfalse; }
	inline VALUE INT2BOOL(int x) { return (x != 0) ? Qtrue : Qfalse; } // (x == 0 ? (VALUE)0 : (VALUE)2)
	inline bool NUM2BOOL(VALUE x) { return (x == Qtrue); } // (x == (VALUE)2)
	inline std::string VAL2STR(VALUE val) { return std::string( RSTRING_PTR(val), RSTRING_LEN(val) ); }
	#define raise_argn(a, n) (rb_raise(rb_eArgError, "wrong number of arguments(%i for %i)", a, n))

	namespace ARuby
	{
		void Init();
		void Run();

		void AddObject(VALUE id);
		void RemoveObject(VALUE id);

		VALUE rload_data(VALUE self, VALUE filename);
		VALUE rsave_data(VALUE self, VALUE obj, VALUE filename);
	} // namespace ARuby
} // namespace ARGSS

////////////////////////////////////////////////////////////
/// Object type checking
////////////////////////////////////////////////////////////
void Check_Kind(VALUE o, VALUE c);
void Check_Types2(VALUE x, VALUE t1, VALUE t2);
void Check_Bool(VALUE x);
void Check_Class(VALUE x, VALUE c);
void Check_Classes_N(VALUE x, VALUE c);

#endif // _ARGSS_RUBY_HXX_
