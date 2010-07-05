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
#include "argss_input.hxx"
#include "argss_ruby.hxx"

#include "input.hxx"


namespace ARGSS
{
	namespace AInput
	{
		namespace
		{
			////////////////////////////////////////////////////////////
			/// Global Variables
			////////////////////////////////////////////////////////////
			VALUE id;
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Input ruby functions
		////////////////////////////////////////////////////////////
		VALUE rupdate(VALUE self)
		{
			Input::Update();
			return Qnil;
		}
		VALUE rpressQ(VALUE self, VALUE button)
		{
			Check_Type(button, T_FIXNUM);
			return INT2BOOL(Input::IsPressed(button));
		}
		VALUE rtriggerQ(VALUE self, VALUE button)
		{
			Check_Type(button, T_FIXNUM);
			return INT2BOOL(Input::IsTriggered(button));
		}
		VALUE rrepeatQ(VALUE self, VALUE button)
		{
			Check_Type(button, T_FIXNUM);
			return INT2BOOL(Input::IsRepeated(button));
		}
		VALUE rreleaseQ(VALUE self, VALUE button)
		{
			Check_Type(button, T_FIXNUM);
			return INT2BOOL(Input::IsReleased(button));
		}
		VALUE rdir4(VALUE self)
		{
			return INT2NUM( Input::getDir4() );
		}
		VALUE rdir8(VALUE self)
		{
			return INT2NUM( Input::getDir8() );
		}
		VALUE rpressed(VALUE self)
		{
			return Input::getPressed();
		}
		VALUE rtriggered(VALUE self)
		{
			return Input::getTriggered();
		}
		VALUE rrepeated(VALUE self)
		{
			return Input::getRepeated();
		}
		VALUE rreleased(VALUE self)
		{
			return Input::getReleased();
		}

		////////////////////////////////////////////////////////////
		/// ARGSS Input initialize
		////////////////////////////////////////////////////////////
		void Init()
		{
			id = rb_define_module("Input");
			rb_define_singleton_method(id, ARGSS_FUNC(update), 0);
			rb_define_singleton_method(id, ARGSS_Q(press), 1);
			rb_define_singleton_method(id, ARGSS_Q(trigger), 1);
			rb_define_singleton_method(id, ARGSS_Q(repeat), 1);
			rb_define_singleton_method(id, ARGSS_Q(release), 1);
			rb_define_singleton_method(id, ARGSS_FUNC(dir4), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(dir8), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(pressed), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(triggered), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(repeated), 0);
			rb_define_singleton_method(id, ARGSS_FUNC(released), 0);
			rb_define_const(id, "DOWN", INT2FIX(2));
			rb_define_const(id, "LEFT", INT2FIX(4));
			rb_define_const(id, "RIGHT", INT2FIX(6));
			rb_define_const(id, "UP", INT2FIX(8));
			rb_define_const(id, "A", INT2FIX(11));
			rb_define_const(id, "B", INT2FIX(12));
			rb_define_const(id, "C", INT2FIX(13));
			rb_define_const(id, "X", INT2FIX(14));
			rb_define_const(id, "Y", INT2FIX(15));
			rb_define_const(id, "Z", INT2FIX(16));
			rb_define_const(id, "L", INT2FIX(17));
			rb_define_const(id, "R", INT2FIX(18));
			rb_define_const(id, "SHIFT", INT2FIX(21));
			rb_define_const(id, "CTRL", INT2FIX(22));
			rb_define_const(id, "ALT", INT2FIX(23));
			rb_define_const(id, "F5", INT2FIX(25));
			rb_define_const(id, "F6", INT2FIX(26));
			rb_define_const(id, "F7", INT2FIX(27));
			rb_define_const(id, "F8", INT2FIX(28));
			rb_define_const(id, "F9", INT2FIX(29));
		}
	} // namespace AInput
} // namespace ARGSS
