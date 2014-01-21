/*
	File: BACKGROUNDMUSIC.H
	Author: Henri Keeble
	Desc: Declares a background music class using BASS.
*/
#ifndef _BACKGROUND_MUSIC_H_
#define _BACKGROUND_MUSIC_H_

#include "BASS\bass.h"
#include "log.h"
#include "sample.h"
#include <string>

namespace GameFramework
{
	class BackgroundMusic : public Sample
	{
	private:
		bool isPlaying;
	public:
		BackgroundMusic();
		BackgroundMusic(const char* filename);
		BackgroundMusic(const BackgroundMusic& param);
		BackgroundMusic& operator=(const BackgroundMusic& param);
		~BackgroundMusic();

		/* Used to check if the current loop is finished */
		void Update();

		/* Play and stop overrides, as the isPlaying variable must be monitored and set */
		virtual void Play() override final;
		virtual void Stop() override final;
	};
}

#endif // _BACKGROUND_MUSIC_H_