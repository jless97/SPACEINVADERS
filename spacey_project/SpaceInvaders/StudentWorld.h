/*******************************************************************************
 *StudentWorld.h
 *
 * This routine is responsible for setting up the configuration of the game
 * (i.e. (de)initializing actors, updating actors)
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-09-02 11:11:11 -0700 (Sat, 02 Sep 2017) $
 *
 ******************************************************************************/

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <iostream> /// DEBUGGING PURPOSES

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

// Space Field dimensions
#define GRID_HEIGHT 64
#define GRID_WIDTH 64

///////////////////////////////////////////////////////////////////////////
///////////////////-----------STUDENTWORLD--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class StudentWorld : public GameWorld
{
public:
  /* Standard Control Flow Functions */
  StudentWorld(std::string asset_dir);
  virtual int init();                               // Initialize the FrackMan universe (and initial actor objects)
  virtual int move();                               // Update the actor objects in the current level
  virtual void clean_up();                          // Reset a level (due to a player death or completion of a level)
  void add_actor(Actor* actor);                     // Add a new actor to StudentWorld
  void add_initial_actors(void);                    // Add initial actors
  void add_additional_actors(void);                 // Add additional actors
  void update_scoreboard(void);                     // Update the scoreboard display
  /* Mutator Functions */
  void update_spaceship_laser_count(bool how_much); // Updates the status of the player's laser in the space field
  void update_invader_laser_count(int how_much);    // Updates the current number of enemy lasers in the space field
  void update_current_invader_count(int how_much);  // Update the number of invaders left to destroy before advancing to the next level
  void set_spaceship_laser_count(void);             // Sets the status of the player's laser in the space field
  void set_invader_laser_count(void);               // Sets the current number of enemy lasers in the space field
  void set_current_invader_count(void);             // Resets the number of invaders to destroy per round (to 55)
  /* Accessor Functions */
  bool get_spaceship_laser_count(void) const;       // Returns if there is a player laser currently in the space field
  int get_invader_laser_count(void) const;          // Returns the number of lasers currently in the space field
  int get_current_invader_count(void) const;        // Returns the number of invaders left to destroy before completing the current level
  /* Laser Collision Functions */
  void check_collision(Actor* actor, bool is_player=true, bool is_invader=false); // Checks if there were any collisions between actor objects
  /* Invader Movement Functions */
  /* Math Helper Functions */
  int max(int x, int y) ;                           // Returns the max of two numbers
  int min(int x, int y) ;                           // Returns the min of two numbers
  int rand_int(int min, int max) const;             // Generates a pseudo-random number in a given range
  virtual ~StudentWorld();
  
private:
  Spaceship*          m_spaceship;                  // Player Spaceship
  std::vector<Actor*> m_actors;                     // All actor objects (i.e. space invaders, flying saucer, lasers, perhaps barriers too)
  bool                m_spaceship_laser_count;      // Variable to keep track if the player currently has a laser in the space field
  int                 m_invader_laser_count;        // Variable to keep track of all of the invaders' lasers in the space field (as there is a cap)
  int                 m_current_invader_count;      // Variable to keep track of how many aliens have been killed by the player
};

#endif // STUDENTWORLD_H_
