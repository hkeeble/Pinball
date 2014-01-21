/*
File: SOUND.CPP
Author: Henri Keeble
Desc: Defines a simple sound class using BASS.
*/
#include "audiotrack.h"

namespace GameFramework
{
	AudioTrack::AudioTrack()
	{
		sound = 0;
	}

	AudioTrack::AudioTrack(const char* filename)
	{
		Load(filename);
	}

	AudioTrack::AudioTrack(const AudioTrack& param)
	{
		this->sound = param.sound;
	}

	AudioTrack& AudioTrack::operator=(const AudioTrack& param)
	{
		if (this == &param)
			return *this;
		else
		{
			this->sound = param.sound;
			return *this;
		}
	}

	AudioTrack::~AudioTrack()
	{

	}

	void AudioTrack::Play()
	{
		if (sound != 0)
		{
			HCHANNEL ch = BASS_SampleGetChannel(sound, false);
			BASS_ChannelPlay(ch, false);
		}
		else
			Log::Write("Error playing sound file, sound not loaded!\n", ENGINE_LOG);
	}

	void AudioTrack::Stop()
	{
		if (sound != 0)
			BASS_SampleStop(sound);
		else
			Log::Write("Error playing sound file, sound not loaded!\n", ENGINE_LOG);
	}

	void AudioTrack::Load(const char* filename)
	{
		if (sound != 0)
			BASS_SampleStop(sound);
		sound = BASS_SampleLoad(false, filename, 0, 0, 1, BASS_SAMPLE_OVER_POS);

		if (sound == 0)
		{
			Log::Write("Sound ", ENGINE_LOG);
			Log::Write(filename, ENGINE_LOG);
			Log::Write(" not loaded correctly!\n", ENGINE_LOG);
		}
	}
}