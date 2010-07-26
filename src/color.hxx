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

#ifndef _COLOR_HXX_
#define _COLOR_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <argss/ruby.hxx>

#include "defines.hxx"

////////////////////////////////////////////////////////////
/// Color class
////////////////////////////////////////////////////////////
class Color
{
public:
	Color();
	Color(VALUE color);
	Color(int ired, int igreen, int iblue, int ialpha = 255);
	Color(Color const& src);
	~Color();

	void set(VALUE color);
	VALUE getARGSS();
	Uint32 getUint32() const;

	float red;
	float green;
	float blue;
	float alpha;

	static Color NewUint32(Uint32 color);
}; // class Color

#define PP_operator(op) \
	inline Color operator op(Color const& lhs, Color const& rhs) \
	{ \
		return Color( \
			lhs.red   op rhs.red  , \
			lhs.green op rhs.green, \
			lhs.blue  op rhs.blue , \
			lhs.alpha op rhs.alpha  \
		); \
	} \
	template< typename T > \
	inline Color operator op(Color const& lhs, T const& rhs) \
	{ \
		return Color( \
			lhs.red   op rhs, \
			lhs.green op rhs, \
			lhs.blue  op rhs, \
			lhs.alpha op rhs  \
		); \
	} \

PP_operator(-)
PP_operator(+)
PP_operator(*)
PP_operator(/)

#undef PP_operator

#endif
