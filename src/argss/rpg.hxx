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

#ifndef _ARGSS_RPG_HXX_
#define _ARGSS_RPG_HXX_

////////////////////////////////////////////////////////////
/// Headers
////////////////////////////////////////////////////////////
#include <argss/ruby.hxx>

////////////////////////////////////////////////////////////
/// ARGSS RPG namespace
////////////////////////////////////////////////////////////
namespace ARGSS
{
	namespace ARPG
	{
		VALUE& getID();

		void Init();

		VALUE rmap_initialize(VALUE self, VALUE width, VALUE height);
		VALUE rmapinfo_initialize(VALUE self);
		VALUE revent_initialize(VALUE self, VALUE x, VALUE y);
		VALUE reventpage_initialize(VALUE self);
		VALUE reventpagecondition_initialize(VALUE self);
		VALUE reventpagegraphic_initialize(VALUE self);
		VALUE reventcommand_initialize(int argc, VALUE* argv, VALUE self);
		VALUE rmoveroute_initialize(VALUE self);
		VALUE rmovecommand_initialize(int argc, VALUE* argv, VALUE self);
		VALUE ractor_initialize(VALUE self);
		VALUE rclass_initialize(VALUE self);
		VALUE rclasslearning_initialize(VALUE self);
		VALUE rskill_initialize(VALUE self);
		VALUE ritem_initialize(VALUE self);
		VALUE rweapon_initialize(VALUE self);
		VALUE rarmor_initialize(VALUE self);
		VALUE renemy_initialize(VALUE self);
		VALUE renemyaction_initialize(VALUE self);
		VALUE rtroop_initialize(VALUE self);
		VALUE rtroopmember_initialize(VALUE self);
		VALUE rtrooppage_initialize(VALUE self);
		VALUE rtrooppagecondition_initialize(VALUE self);
		VALUE rstate_initialize(VALUE self);
		VALUE ranimation_initialize(VALUE self);
		VALUE ranimationframe_initialize(VALUE self);
		VALUE ranimationtiming_initialize(VALUE self);
		VALUE rtileset_initialize(VALUE self);
		VALUE rcommonevent_initialize(VALUE self);
		VALUE rsystem_initialize(VALUE self);
		VALUE rsystemwords_initialize(VALUE self);
		VALUE rsystemtestbattler_initialize(VALUE self);
		VALUE raudiofile_initialize(int argc, VALUE* argv, VALUE self);
	} // namespace ARPG
} // namespace ARGSS

#endif
