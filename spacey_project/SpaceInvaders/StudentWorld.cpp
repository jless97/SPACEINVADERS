/*******************************************************************************
 *StudentWorld.cpp
 *
 * This routine is responsible for setting up the configuration of the game
 * (i.e. (de)initializing actors, updating actors)
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-09-02 11:11:11 -0700 (Sat, 02 Sep 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "StudentWorld.h"
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <cmath>
#include "math.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
///////////////////-----------STUDENTWORLD--------------///////////////////
///////////////////////////////////////////////////////////////////////////

GameWorld* createStudentWorld(string asset_dir) { return new StudentWorld(asset_dir); }

///////////////////////////////////////////////////////////////////////////
/////////////-----------CONSTRUCTOR/DESTRUCTOR--------------///////////////
///////////////////////////////////////////////////////////////////////////

StudentWorld::StudentWorld(std::string asset_dir)
:GameWorld(asset_dir), m_spaceship_laser_count(false), m_alien_laser_count(0) {}

StudentWorld::~StudentWorld() { clean_up(); }

///////////////////////////////////////////////////////////////////////////
//////////-----------GAMEWORLD VIRTUAL FUNCTIONS--------------/////////////
///////////////////////////////////////////////////////////////////////////

int StudentWorld::init()
{
  // Initialize frackman player
  m_spaceship = new Spaceship(this);

  // Add initial actors to the current level (i.e. the aliens and barriers)
  add_initial_actors();

  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
  // Update scoreboard
  update_scoreboard();
  
  // Give the player a chance to do something
  m_spaceship->do_something();
  
  // Give all other actors a chance to do something
  for (int i = 0; i < m_actors.size(); i++) { m_actors[i]->do_something(); }
  
  // Remove newly-dead actors after each tick
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
  {
    if (!(*it)->is_alive()) { delete *it; it = m_actors.erase(it); }
    else { it++; }
  }
  
  // If frackman died during this tick, decrement lives, and if out of lives (GameWorld goes to game over screen)
  if (!m_spaceship->is_alive())
  {
    dec_lives();
    return GWSTATUS_PLAYER_DIED;
  }
  
  // Add additional actors (i.e. flying saucer)
  add_additional_actors();
  
  // Continue with the level until one of the above conditions happens
  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::clean_up()
{
  // Remove actors
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
  {
    delete *it;
    it = m_actors.erase(it);
  }
  
  // Remove player spaceship
  delete m_spaceship;
  
  return;
}

///////////////////////////////////////////////////////////////////////////
//////////////-----------ADDING ACTORS FUNCTIONS-------------//////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::add_actor(Actor* actor) { m_actors.push_back(actor); }

void StudentWorld::add_initial_actors(void)
{
  /// TODO: add aliens and barriers
  return;
}

void StudentWorld::add_additional_actors(void)
{
  // TODO: add flying saucer
  return;
}

///////////////////////////////////////////////////////////////////////////
////////////////-----------SCOREBOARD FUNCTIONS-------------///////////////
///////////////////////////////////////////////////////////////////////////
void StudentWorld::update_scoreboard()
{
  // Update scoreboard parameters
  int score = get_score();
  int level = get_level();
  int lives = get_lives();
  // Convert parameters to strings
  string score_text = "Scr: " + std::string(6 - std::to_string(score).size(), '0') + std::to_string(score) + " ";
  string level_text = "Lvl: " + std::string(2 - std::to_string(level).size(), ' ') + std::to_string(level) + " ";
  string lives_text = "Lives: " + std::to_string(lives) + " ";

  string text = score_text + level_text + lives_text;
  // Display updated scoreboard
  set_game_stat_text(text);
  return;
}

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MUTATOR FUNCTIONS-------------////////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::update_spaceship_laser_count(bool how_much) { m_spaceship_laser_count = how_much; }

void StudentWorld::update_alien_laser_count(int how_much) { m_alien_laser_count += how_much; }

///////////////////////////////////////////////////////////////////////////
/////////////////-----------ACCESSOR FUNCTIONS-------------////////////////
///////////////////////////////////////////////////////////////////////////

bool StudentWorld::get_spaceship_laser_count(void) const { return m_spaceship_laser_count; }

int StudentWorld::get_alien_laser_count(void) const { return m_alien_laser_count; }

///////////////////////////////////////////////////////////////////////////
////////////-----------COLLISION-HANDLING FUNCTION-------------////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::check_collision(Actor* actor, bool is_player, bool is_alien) {
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (!m_actors[i]->is_alive()) { continue; } // To avoid doubling of one of the effects below (temporary fix)
    // Check if player spaceship's laser hit any aliens (or flying saucer)
    if (is_player)
    {
      if (m_actors[i]->get_id() == IID_PROTESTER || m_actors[i]->get_id() == IID_HARD_CORE_PROTESTER || m_actors[i]->get_id() == IID_GOLD)
      {
        // Player projectile hit an alien spaceship
        if (actor->get_x() == m_actors[i]->get_x() && actor->get_y() == m_actors[i]->get_y())
        {
          /// TODO: have three four different scores (depending on the type of invader (or flying saucer)
          if (m_actors[i]->get_id() == IID_PROTESTER) { increase_score(10); }
          else if (m_actors[i]->get_id() == IID_HARD_CORE_PROTESTER) { increase_score(20); }
          else if (m_actors[i]->get_id() == IID_BOULDER) { increase_score(30); }
          else if (m_actors[i]->get_id() == IID_GOLD) { increase_score(100); } /// TODO: Flying Saucer has different points
          actor->set_dead();
          play_sound(SOUND_ALIEN_KILLED);
        }
      }
    }
  }
  // Check if alien laser hits the player spaceship /// TODO: barrier too
  if (is_alien)
  {
    // Alien projectile hit player spaceship
    if (actor->get_x() == m_spaceship->get_x() && actor->get_y() == m_spaceship->get_y())
    {
      m_spaceship->set_dead();
      play_sound(SOUND_PLAYER_KILLED);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
/////////////-----------MATH/MATH HELPER FUNCTIONS-------------////////////
///////////////////////////////////////////////////////////////////////////

int StudentWorld::max(int x, int y) { if (x > y) { return x; } else { return y; } }

int StudentWorld::min(int x, int y) { if (x < y) { return x; } else { return y; } }

//Generate a random number (Equation used from Project 1 (no need to reinvent the wheel))
int StudentWorld::rand_int(int min, int max) const
{
  if (max < min) { swap(max, min); }
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distro(min, max);
  return distro(generator);
}
