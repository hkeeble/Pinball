/*
File: SOUND.CPP
Author: Henri Keeble
Desc: Defines a simple sample class using BASS.
*/
#include "sample.h"

namespace GameFramework
{
	Sample::Sample()
	{
		sound = 0;
	}
	
	Sample::Sample(const char* filename)
	{
		Load(filename);
	}

	Sample::Sample(const Sample& param)
	{
		this->sound = param.sound;
		this->channel = param.channel;
	}

	Sample& Sample::operator=(const Sample& param)
	{
		if (this == &param)
			return *this;
		else
		{
			this->sound = param.sound;
			this->channel = param.channel;
			return *this;
		}
	}

	Sample::~Sample()
	{

	}

	void Sample::Play()
	{
		// If no sound, do not attempt to play
		if (sound != 0)
		{
			if (BASS_ChannelIsActive(GetChannel()) != BASS_ACTIVE_PLAYING) // Only play if not already playing
				channel = BASS_SampleGetChannel(sound, false); // Create channel and play
			BASS_ChannelPlay(channel, false);
		}
		else
			Log::Write("Error playing sound file, sound not loaded!\n", ENGINE_LOG);
	}

	void Sample::Stop()
	{
		if (sound != 0)
			BASS_ChannelStop(channel);
		else
			Log::Write("Error playing sound file, sound not loaded!\n", ENGINE_LOG);
	}

	HSAMPLE Sample::GetSample() const
	{
		return sound;
	}

	HCHANNEL Sample::GetChannel() const
	{
		return channel;
	}

	void Sample::Load(const char* filename)
	{
		if (sound != 0) // If playing, stop
			BASS_SampleStop(sound);

		// Load the sample with the given file name
		sound = BASS_SampleLoad(false, filename, 0, 0, 1, BASS_SAMPLE_OVER_POS);

		// Report of error in loading
		if (sound == 0)
		{
			Log::Write("Sound ", ENGINE_LOG);
			Log::Write(filename, ENGINE_LOG);
			Log::Write(" not loaded correctly!\n", ENGINE_LOG);
		}
	}
}