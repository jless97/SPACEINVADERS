#ifndef SOUNDFX_H_
#define SOUNDFX_H_

#include <string>

#if defined(_MSC_VER)

#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
#include <iostream>

class SoundFXController
{
public:
  SoundFXController(const SoundFXController&);
  SoundFXController& operator=(const SoundFXController&);
  void play_clip(std::string soundFile) { if (m_engine != nullptr) { m_engine->play2D(soundFile.c_str(), false); }
  void abort_clip() { if (m_engine != nullptr) { m_engine->stopAllSounds(); }
	static SoundFXController& get_instance();

private:
	irrklang::ISoundEngine* m_engine;
	SoundFXController()
	{
		m_engine = irrklang::createIrrKlangDevice();
    if (m_engine == nullptr) { std::cout << "Cannot create sound engine!  Game will be silent." << std::endl; }
	}
  ~SoundFXController() { if (m_engine != nullptr) { m_engine->drop(); } }
};

#elif defined(__APPLE__)

#include <cstdlib>

class SoundFXController
{
public:
	void play_clip(std::string soundFile) { std::system(("/usr/bin/afplay \"" + soundFile + "\" &").c_str()); }
	void abort_clip() {}
	static SoundFXController& get_instance();
};

#else  // forget about sound

class SoundFXController
{
public:
	void play_clip(std::string soundFile) {}
	void abort_clip() {}
	static SoundFXController& get_instance();
};

#endif

// Meyers singleton pattern
inline SoundFXController& SoundFXController::get_instance() { static SoundFXController instance; return instance; }
inline SoundFXController& SoundFX() { return SoundFXController::get_instance(); }

#endif // SOUNDFX_H_
