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
:GameWorld(asset_dir), m_spaceship_laser_count(false), m_flying_saucer_count(0), m_invader_laser_count(0), m_current_invader_count(55) {}

StudentWorld::~StudentWorld() { clean_up(); }

///////////////////////////////////////////////////////////////////////////
//////////-----------GAMEWORLD VIRTUAL FUNCTIONS--------------/////////////
///////////////////////////////////////////////////////////////////////////

int StudentWorld::init()
{
  m_spaceship = new Spaceship(this); // Initialize player spaceship
  
  init_border(); // Initialize the border
  
  add_initial_actors(); // Add initial actors to the current level (i.e. the invaders and barriers)
  
  set_spaceship_laser_count(); // Set the current player laser count in the space field to 0
  
  set_flying_saucer_count(); // Set the current flying saucer count in the space field to 0
  
  set_invader_laser_count(); // Set the current invaders' laser count in the space field to 0
  
  set_current_invader_count(); // Resets the number of invaders to destroy per round to 55
  
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
  update_scoreboard(); // Update scoreboard
  
  if (m_spaceship->is_alive()) { m_spaceship->do_something(); } // Give the player a chance to do something
  
  // Give all other actors a chance to do something
  for (int i = 0; i < m_actors.size(); i++)
  {
    m_actors[i]->do_something();
  }
  
  // Play invader sounds
  static int play_invader_sound = 0;
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (dynamic_cast<LargeInvader*>(m_actors[i])->get_can_move_status() == 0)
    {
      switch (play_invader_sound)
      {
        case 0: play_sound(SOUND_ALIEN_MOVE_1); play_invader_sound = 1; break;
        case 1: play_sound(SOUND_ALIEN_MOVE_2); play_invader_sound = 2; break;
        case 2: play_sound(SOUND_ALIEN_MOVE_3); play_invader_sound = 3; break;
        case 3: play_sound(SOUND_ALIEN_MOVE_4); play_invader_sound = 0; break;
        default:
          break;
      }
      break;
    }
  }
  
  // After letting all actors make a move, check to see if invaders should move down on the next turn
  static int counter = 0;
  bool move_down = false;
  if (counter > 100)
  {
    for (int i = 0; i < m_actors.size(); i++)
    {
      if (m_actors[i]->get_id() == IID_LARGE_INVADER || m_actors[i]->get_id() == IID_MEDIUM_INVADER || m_actors[i]->get_id() == IID_SMALL_INVADER)
      {
        if (m_actors[i]->get_x() == 0 || m_actors[i]->get_x() == VIEW_WIDTH - 1)
        {
          move_down = true;
          counter = 0;
        }
      }
    }
  }
  else { counter++; }
  // If invaders should move down, set their current direction to move as down, and their next direction to be in the opposite horizontal direction
  if (move_down)
  {
    move_down = false;
    for (int i = 0; i < m_actors.size(); i++)
    {
      if (dynamic_cast<LargeInvader*>(m_actors[i])->get_movement_direction() == 0) {
        dynamic_cast<LargeInvader*>(m_actors[i])->set_next_movement_direction(1);
      }
      else
      {
        dynamic_cast<LargeInvader*>(m_actors[i])->set_next_movement_direction(0);
      }
      dynamic_cast<LargeInvader*>(m_actors[i])->set_movement_direction(2);
    }
  }
  
  // If the player died during this tick, decrement lives, and if out of lives (GameWorld goes to game over screen)
  static int wait_to_spawn = 0;
  if (!m_spaceship->is_alive())
  {
    if (wait_to_spawn < 50) { wait_to_spawn++; }
    else { wait_to_spawn = 0; m_spaceship->move_to(30, 4); m_spaceship->set_alive(); }
  }
  
  // If the player destroyed all 55 aliens in a round, then advance to the next round
  if (get_current_invader_count() <= 54) { return GWSTATUS_FINISHED_LEVEL; }
  
  add_additional_actors(); // Add additional actors (i.e. flying saucer)
  
  // Remove newly-dead actors after each tick
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
  {
    if (!(*it)->is_alive()) { delete *it; it = m_actors.erase(it); }
    else { it++; }
  }
  
  if (get_lives() <= 0) { return GWSTATUS_PLAYER_DIED; }
  
  return GWSTATUS_CONTINUE_GAME; // Continue with the level until one of the above conditions happens
}

void StudentWorld::clean_up()
{
  // Remove actors
  for (vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); )
  {
    delete *it;
    it = m_actors.erase(it);
  }
  
  delete m_spaceship; // Remove player spaceship
  
  deinit_border(); // Remove border
}

///////////////////////////////////////////////////////////////////////////
//////////////-----------ADDING ACTORS FUNCTIONS-------------//////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::add_actor(Actor* actor) { m_actors.push_back(actor); }

void StudentWorld::add_initial_actors(void)
{
  /// TODO: add barriers
  
  // Add Large Invaders
  static int invader_row_height = 1;
  if (get_level() >= 10)
  {
    invader_row_height = (get_level() % 10) + 1;
  }
  else
  {
    invader_row_height = get_level();
  }
  int j = 0;
  for (int i = 5; (j++ < 11); i += 5)
  {
    new LargeInvader(this, i, 39 - ((invader_row_height - 1) * 3), 1);
  }
  j = 0;
  for (int i = 5; (j++ < 11); i += 5)
  {
    new LargeInvader(this, i, 44 - ((invader_row_height - 1) * 3), 2);
  }
    // Add Medium Invaders
  j = 0;
  for (int i = 5; (j++ < 11); i += 5)
  {
    new MediumInvader(this, i, 49 - ((invader_row_height - 1) * 3), 3);
  }
  j = 0;
  for (int i = 5; (j++ < 11); i += 5)
  {
    new MediumInvader(this, i, 54 - ((invader_row_height - 1) * 3), 4);
  }
    // Add Small Invaders
  j = 0;
  for (int i = 5; (j++ < 11); i += 5)
  {
    new SmallInvader(this, i, 59 - ((invader_row_height - 1) * 3), 5);
  }
}

void StudentWorld::add_additional_actors(void)
{
  // Calculate the chance that a flying saucer will appear
  if (rand() % 500 == 1)
  {
    if (!get_flying_saucer_count())
    {
      new FlyingSaucer(this, VIEW_WIDTH - 1, VIEW_HEIGHT - 1);
      play_sound(SOUND_SAUCER_MOVE_1);
    }
  }
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

void StudentWorld::update_flying_saucer_count(bool how_much) { m_flying_saucer_count = how_much; }

void StudentWorld::update_invader_laser_count(int how_much) { m_invader_laser_count += how_much; }

void StudentWorld::update_current_invader_count(int how_much) { m_current_invader_count += how_much; }

void StudentWorld::set_spaceship_laser_count(void) { m_spaceship_laser_count = false; }

void StudentWorld::set_flying_saucer_count(void) { m_flying_saucer_count = false; }

void StudentWorld::set_invader_laser_count(void) { m_invader_laser_count = 0; }

void StudentWorld::set_current_invader_count(void) { m_current_invader_count = 55; }

///////////////////////////////////////////////////////////////////////////
/////////////////-----------ACCESSOR FUNCTIONS-------------////////////////
///////////////////////////////////////////////////////////////////////////

bool StudentWorld::get_spaceship_laser_count(void) const { return m_spaceship_laser_count; }

bool StudentWorld::get_flying_saucer_count(void) const { return m_flying_saucer_count; }

int StudentWorld::get_invader_laser_count(void) const { return m_invader_laser_count; }

int StudentWorld::get_current_invader_count(void) const { return m_current_invader_count; }

///////////////////////////////////////////////////////////////////////////
//////////////-----------LASER-HANDLING FUNCTION-------------//////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::check_collision(Actor* actor, bool is_player, bool is_invader) {
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (!m_actors[i]->is_alive()) { continue; } // To avoid doubling of one of the effects below (temporary fix)
    // Check if player spaceship's laser hit any invaders (or flying saucer)
    if (is_player)
    {
      // If laser hit the large invader
      if (m_actors[i]->get_id() == IID_LARGE_INVADER)
      {
        if (actor->get_x() >= m_actors[i]->get_x() - 2 && actor->get_x() <= m_actors[i]->get_x() + 2 && actor->get_y() == m_actors[i]->get_y())
        {
          actor->set_dead();
          new InvaderExplosion(this, m_actors[i]->get_x(), m_actors[i]->get_y());
          m_actors[i]->set_dead();
          play_sound(SOUND_ALIEN_KILLED);
          increase_score(10);
        }
      }
      // If laser hit the medium invader
      if (m_actors[i]->get_id() == IID_MEDIUM_INVADER)
      {
        if (actor->get_x() >= m_actors[i]->get_x() - 1.5 && actor->get_x() <= m_actors[i]->get_x() + 1.5 && actor->get_y() == m_actors[i]->get_y())
        {
          actor->set_dead();
          new InvaderExplosion(this, m_actors[i]->get_x(), m_actors[i]->get_y());
          m_actors[i]->set_dead();
          play_sound(SOUND_ALIEN_KILLED);
          increase_score(20);
        }
      }
      // If laser hit the small invader
      if (m_actors[i]->get_id() == IID_SMALL_INVADER)
      {
        if (actor->get_x() >= m_actors[i]->get_x() - 1 && actor->get_x() <= m_actors[i]->get_x() + 1 && actor->get_y() == m_actors[i]->get_y())
        {
          actor->set_dead();
          new InvaderExplosion(this, m_actors[i]->get_x(), m_actors[i]->get_y());
          m_actors[i]->set_dead();
          play_sound(SOUND_ALIEN_KILLED);
          increase_score(30);
        }
      }
      if (m_actors[i]->get_id() == IID_FLYING_SAUCER)
      {
        if (actor->get_x() >= m_actors[i]->get_x() - 2 && actor->get_x() <= m_actors[i]->get_x() + 2 && actor->get_y() == m_actors[i]->get_y())
        {
          actor->set_dead();
          m_actors[i]->set_dead();
          play_sound(SOUND_SAUCER_MOVE_2);
          /// TODO: Fix. This is just temporary
          int flying_saucer_points = rand() % 10;
          if (flying_saucer_points >= 0 && flying_saucer_points <= 4) { increase_score(150); }
          else if (flying_saucer_points >=5 && flying_saucer_points <= 8) { increase_score(250); }
          else { increase_score(300); }
        }
      }
    }
  }
  // Check if invader laser hits the player spaceship /// TODO: barrier too
  int count = 0;
  if (is_invader)
  {
    // Invader projectile hit player spaceship
    if (actor->get_x() >= m_spaceship->get_x() - 2 && actor->get_x() <= m_spaceship->get_x() + 2 &&
        actor->get_y() + 2 >= m_spaceship->get_y() && actor->get_y() - 2 <= m_spaceship->get_y())
    {
      actor->set_dead();
      m_spaceship->set_dead();
      if (count == 0)
      {
        count = 1;
        dec_lives();
      }
      new PlayerExplosion(this, actor->get_x(), actor->get_y());
      play_sound(SOUND_PLAYER_KILLED);
    }
    // Invader projectile hit the bottom border
    if (actor->get_y() == BORDER_HEIGHT)
    {
      play_sound(SOUND_PLAYER_KILLED);
      new PlayerExplosion(this, actor->get_x(), actor->get_y() + 1);
    }
  }
}

bool StudentWorld::is_invader_below(LargeInvader* invader)
{
  for (int i = 0; i < m_actors.size(); i++)
  {
    if (m_actors[i]->get_id() == IID_LARGE_INVADER || m_actors[i]->get_id() == IID_MEDIUM_INVADER || m_actors[i]->get_id() == IID_SMALL_INVADER)
    {
      switch (invader->get_row_number())
      {
        case 1:
          if ((invader != m_actors[i]) && (invader->get_x() == m_actors[i]->get_x())) { return true; }
          break;
        case 2:
          if ((invader != m_actors[i]) && (invader->get_x() == m_actors[i]->get_x()) && (invader->get_y() >= m_actors[i]->get_y())) { return true; }
          break;
        case 3:
          if ((invader != m_actors[i]) && (invader->get_x() == m_actors[i]->get_x()) && (invader->get_y() >= m_actors[i]->get_y())) { return true; }
          break;
        case 4:
          if ((invader != m_actors[i]) && (invader->get_x() == m_actors[i]->get_x()) && (invader->get_y() >= m_actors[i]->get_y())) { return true; }
          break;
        case 5:
          if ((invader != m_actors[i]) && (invader->get_x() == m_actors[i]->get_x()) && (invader->get_y() >= m_actors[i]->get_y())) { return true; }
          break;
        default:
          break;
      }
    }
  }
  
  return false;
}

///////////////////////////////////////////////////////////////////////////
/////////////-----------INVADER-MOVEMENT FUNCTION-------------/////////////
///////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////
////////-----------STUDENTWORLD PRIVATE FUNCTIONS--------------////////////
///////////////////////////////////////////////////////////////////////////

void StudentWorld::init_border(void)
{
  for (int i = 0; i < VIEW_WIDTH - 1; i++)
  {
    for (int j = 0; j < 1; j++)
    {
      m_border[i][j] = new Border(this, i, j + 1);
    }
  }
}

void StudentWorld::deinit_border(void)
{
  
}
