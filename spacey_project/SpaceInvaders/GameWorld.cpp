/*******************************************************************************
 *GameWorld.cpp
 *
 * This routine is responsible for controlling the StudentWorld setup by accessing the
 * asset directory, updating the score, handling player lives, and getting user input
 *
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-09-02 11:11:11 -0700 (Sat, 02 Sep 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameWorld.h"
#include "GameController.h"
#include <string>
#include <cstdlib>
using namespace std;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GAMEWORLD--------------////////////////////
///////////////////////////////////////////////////////////////////////////

GameWorld::GameWorld(std::string asset_dir)
: m_lives(START_PLAYER_LIVES), m_score(0), m_level(1), m_controller(nullptr), m_asset_dir(asset_dir) {}

void GameWorld::set_game_stat_text(string text) { m_controller->set_game_stat_text(text); }

bool GameWorld::get_key(int& value) {
	bool gotKey = m_controller->get_last_key(value);

	if (gotKey)
  {
    if (value == 'q') { m_controller->quit_game(); }
    else if (value == '\x03') { exit(0); } // CTRL-C
	}
  
	return gotKey;
}

void GameWorld::play_sound(int sound_id) { m_controller->play_sound(sound_id); }

unsigned int GameWorld::get_level(void) const { return m_level; }

unsigned int GameWorld::get_lives(void) const { return m_lives; }

void GameWorld::dec_lives(void) { m_lives--; }

void GameWorld::inc_lives(void) { m_lives++; }

unsigned int GameWorld::get_score(void) const { return m_score; }

void GameWorld::increase_score(unsigned int how_much) { m_score += how_much; }

bool GameWorld::is_game_over(void) const { return m_lives == 0; }

void GameWorld::advance_to_next_level(void) { ++m_level; }

void GameWorld::set_controller(GameController* controller) { m_controller = controller; }

std::string GameWorld::asset_directory(void) const { return m_asset_dir; }

GameWorld::~GameWorld() {}
