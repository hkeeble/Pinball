/*
	File: BACKGROUNDMUSIC.CPP
	Author: Henri Keeble
	Desc: Defines functions for a class used to play background music in a game using BASS 2.4.
*/
#include "backgroundmusic.h"

namespace GameFramework
{
	BackgroundMusic::BackgroundMusic() : Sample()
	{
		isPlaying = false;
	}

	BackgroundMusic::BackgroundMusic(const char* filename) : Sample(filename)
	{
		isPlaying = false;
	}

	BackgroundMusic::BackgroundMusic(const BackgroundMusic& param) : Sample(param)
	{
		isPlaying = param.isPlaying;
	}

	BackgroundMusic& BackgroundMusic::operator=(const BackgroundMusic& param)
	{
		if (this == &param)
			return *this;
		else
		{
			isPlaying = param.isPlaying;
			Sample::operator=(param);
			return *this;
		}
	}

	BackgroundMusic::~BackgroundMusic()
	{

	}

	void BackgroundMusic::Update()
	{
		if (BASS_ChannelIsActive(GetChannel()) != BASS_ACTIVE_PLAYING && isPlaying == true)
		{
			isPlaying = false;
			Play();
		}
	}

	void BackgroundMusic::Play()
	{
		if (isPlaying == false)
		{
			isPlaying = true;
			Sample::Play();
		}
		else
			return;
	}

	void BackgroundMusic::Stop()
	{
		if (isPlaying == true)
		{
			isPlaying = false;
			Sample::Stop();
		}
		else
			return;
	}
}