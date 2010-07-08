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
#include <map>

#include <SDL.h>
#include <SDL_mixer.h>

#include <argss/error.hxx>
#include <argss/ruby.hxx>

#include "audio.hxx"
#include "filefinder.hxx"
#include "output.hxx"


namespace Audio
{
	namespace
	{
		////////////////////////////////////////////////////////////
		/// Global Variables
		////////////////////////////////////////////////////////////
		Mix_Music* bgm;
		int bgm_volume;
		bool bgm_playing;
		Mix_Chunk* bgs;
		int bgs_channel;
		Mix_Music* me;
		bool me_playing;
		std::map< int, Mix_Chunk* > sounds;
	} // namespace

	////////////////////////////////////////////////////////////
	/// Initialize
	////////////////////////////////////////////////////////////
	void Init()
	{
		if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) { 
			Output::Error("ARGSS couldn't initialize audio.\n%s\n", SDL_GetError());
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
			Output::Error("ARGSS couldn't initialize audio.\n%s\n", Mix_GetError());
		}
		bgm = NULL;
		bgs = NULL;
		me = NULL;
		bgm_playing = false;
		me_playing = false;
	}

	////////////////////////////////////////////////////////////
	/// Audio Pause
	////////////////////////////////////////////////////////////
	void Pause()
	{
		Mix_Pause(-1);
		Mix_PauseMusic();
	}

	////////////////////////////////////////////////////////////
	/// Audio Continue
	////////////////////////////////////////////////////////////
	void Continue()
	{
		Mix_Resume(-1);
		Mix_ResumeMusic();
	}

	////////////////////////////////////////////////////////////
	/// ME finish callback
	////////////////////////////////////////////////////////////
	void me_finish() {
		Mix_VolumeMusic(bgm_volume);
		Mix_FadeInMusic(bgm, -1, 1000);
		bgm_playing = true;
		me_playing = false;
		Mix_HookMusicFinished(NULL);
	}

	////////////////////////////////////////////////////////////
	/// BGM play
	////////////////////////////////////////////////////////////
	void BGM_Play(std::string const& file, int volume, int pitch)
	{
		std::string path = FileFinder::FindMusic(file);
		if ( path.empty() ) {
			VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
			rb_raise(enoent, "No such file or directory - %s", file.c_str());
		}
		if (bgm != NULL) Mix_FreeMusic(bgm);
		bgm = Mix_LoadMUS(path.c_str());
		if (!bgm) {
			rb_raise(ARGSS::AError::getID(), "couldn't load %s BGM.\n%s\n", file.c_str(), Mix_GetError());
		}
		bgm_volume = volume * MIX_MAX_VOLUME / 100;
		if (me_playing) Mix_HookMusicFinished(me_finish);
		else {
			bgm_playing = true;
			Mix_VolumeMusic(bgm_volume);
			if (Mix_PlayMusic(bgm, -1) == -1) {
				rb_raise(ARGSS::AError::getID(), "couldn't play %s BGM.\n%s\n", file.c_str(), Mix_GetError());
			}
			Mix_HookMusicFinished(NULL);
		}
	}

	////////////////////////////////////////////////////////////
	/// BGM stop
	////////////////////////////////////////////////////////////
	void BGM_Stop()
	{
		if (bgm_playing) {
			Mix_HaltMusic();
			bgm_playing = false;
		}
		Mix_HookMusicFinished(NULL);
	}

	////////////////////////////////////////////////////////////
	/// BGM fade
	////////////////////////////////////////////////////////////
	void BGM_Fade(int fade)
	{
		if (bgm_playing) {
			Mix_FadeOutMusic(fade);
			bgm_playing = false;
		}
		Mix_HookMusicFinished(NULL);
	}

	////////////////////////////////////////////////////////////
	/// BGS play
	////////////////////////////////////////////////////////////
	void BGS_Play(std::string const& file, int volume, int pitch)
	{
		std::string path = FileFinder::FindMusic(file);
		if ( path.empty() ) {
			VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
			rb_raise(enoent, "No such file or directory - %s", file.c_str());
		}
		if (bgs != NULL) Mix_FreeChunk(bgs);
		bgs = Mix_LoadWAV(path.c_str());
		if (!bgs) {
			rb_raise(ARGSS::AError::getID(), "couldn't load %s BGS.\n%s\n", file.c_str(), Mix_GetError());
		}
		bgs_channel = Mix_PlayChannel(-1, bgs, -1);
		Mix_Volume(bgs_channel, volume * MIX_MAX_VOLUME / 100);
		if (bgs_channel == -1) {
			rb_raise(ARGSS::AError::getID(), "couldn't play %s BGS.\n%s\n", file.c_str(), Mix_GetError());
		}
	}

	////////////////////////////////////////////////////////////
	/// BGS stop
	////////////////////////////////////////////////////////////
	void BGS_Stop()
	{
		if ( Mix_Playing(bgs_channel) ) Mix_HaltChannel(bgs_channel);
	}

	////////////////////////////////////////////////////////////
	/// BGS fade
	////////////////////////////////////////////////////////////
	void BGS_Fade(int fade)
	{
		Mix_FadeOutChannel(bgs_channel, fade);
	}

	////////////////////////////////////////////////////////////
	/// ME play
	////////////////////////////////////////////////////////////
	void ME_Play(std::string const& file, int volume, int pitch)
	{
		std::string path = FileFinder::FindMusic(file);
		if ( path.empty() ) {
			VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
			rb_raise(enoent, "No such file or directory - %s", file.c_str());
		}
		if (me != NULL) Mix_FreeMusic(me);
		me = Mix_LoadMUS(path.c_str());
		if (!me) {
			rb_raise(ARGSS::AError::getID(), "couldn't load %s ME.\n%s\n", file.c_str(), Mix_GetError());
		}
		Mix_VolumeMusic(volume * MIX_MAX_VOLUME / 100);
		if (Mix_PlayMusic(me, 1) == -1) {
			rb_raise(ARGSS::AError::getID(), "couldn't play %s ME.\n%s\n", file.c_str(), Mix_GetError());
		}
		me_playing = true;
		if (bgm_playing) {
			bgm_playing = false;
			Mix_HookMusicFinished(me_finish);
		}
	}

	////////////////////////////////////////////////////////////
	/// ME stop
	////////////////////////////////////////////////////////////
	void ME_Stop()
	{
		if (me_playing) {
			Mix_HaltMusic();
			me_playing = false;
		}
	}

	////////////////////////////////////////////////////////////
	/// ME fade
	////////////////////////////////////////////////////////////
	void ME_Fade(int fade)
	{
		if (me_playing) {
			Mix_FadeOutMusic(fade);
			me_playing = false;
		}
	}

	////////////////////////////////////////////////////////////
	/// SE play
	////////////////////////////////////////////////////////////
	void SE_Play(std::string const& file, int volume, int pitch)
	{
		;
		for(std::map< int, Mix_Chunk* >::iterator it = sounds.begin(); it != sounds.end(); ++it) {
			if (!Mix_Playing(it->first)) {
				Mix_FreeChunk(it->second);
				sounds.erase(it);
			}
		}
		if (sounds.size() >= 7) return;

		std::string path = FileFinder::FindMusic(file);
		if ( path.empty() ) {
			VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
			rb_raise(enoent, "No such file or directory - %s", file.c_str());
		}
		Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
		if (!sound) {
			rb_raise(ARGSS::AError::getID(), "couldn't load %s SE.\n%s\n", file.c_str(), Mix_GetError());
		}
		int channel = Mix_PlayChannel(-1, sound, 0);
		Mix_Volume(channel, volume * MIX_MAX_VOLUME / 100);
		if (channel == -1) {
			rb_raise(ARGSS::AError::getID(), "couldn't play %s SE.\n%s\n", file.c_str(), Mix_GetError());
		}
		sounds.insert( std::map< int, Mix_Chunk* >::value_type(channel, sound) );
	}

	////////////////////////////////////////////////////////////
	/// SE stop
	////////////////////////////////////////////////////////////
	void SE_Stop()
	{
		for (std::map< int, Mix_Chunk* >::iterator it = sounds.begin(); it != sounds.end(); ++it) {
			if (Mix_Playing(it->first)) Mix_HaltChannel(it->first);
			Mix_FreeChunk(it->second);
		}
		sounds.clear();
	}
} // namespace Audio
