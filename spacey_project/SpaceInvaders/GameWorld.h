/*******************************************************************************
 *GameWorld.h
 *
 * This routine was created by Professor Smallberg to handle the game engine,
 * setting the frequency that the StudentWorld class is accessed and updated,
 * and accessing the assets (i.e. sound effects, images), and updating graphics
 *
 *
 ******************************************************************************/

#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameConstants.h"
#include <string>

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

const int START_PLAYER_LIVES = 3;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GAMEWORLD--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class GameController;

class GameWorld {
public:
  /* Standard Control Flow Functions */
  GameWorld(std::string asset_dir);
  virtual int init() = 0;                           // Initialize the game world
  virtual int move() = 0;                           // Ask each actor to do something
  virtual void clean_up() = 0;                      // Clean up actors
  void set_game_stat_text(std::string text);        // Initialize the scoreboard
  bool get_key(int& value);                         // Get user input from keyboard
  void play_sound(int sound_id);                    // Play object sound
  /* Functions to be utilized by StudentWorld (and to update game) */
  unsigned int get_level(void) const;               // Get the current level
  unsigned int get_lives(void) const;               // Get the current lives of the player
  void dec_lives(void);                             // Decrement the player lives count
  void inc_lives(void);                             // Increment the player lives count
  unsigned int get_score(void) const;               // Get the current score of the player
  void increase_score(unsigned int how_much);       // Increase the score of the player every clock tick (if necessary)
  /* NOTE: The following should be used by only the framework, not the student */
  bool is_game_over(void) const;                    // Check if the player has run out of lives
  void advance_to_next_level(void);                 // If the player has killed all 55 aliens, advance to the next level
  void set_controller(GameController* controller);  // Game engine stuff
  std::string asset_directory(void) const;          // Access of the images and sound effects in the assets directory
  virtual ~GameWorld();

private:
  unsigned int      m_lives;        // Player lives
  unsigned int      m_score;        // Player score
  unsigned int      m_level;        // Current level
  GameController*   m_controller;   // Game engine control
  std::string		    m_asset_dir;    // Assets directory (i.e. game objects and sound effects)
};

#endif // GAMEWORLD_H_
