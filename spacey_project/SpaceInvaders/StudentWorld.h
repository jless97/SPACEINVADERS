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
  virtual void revive();                            // Revive the player spaceship
  void add_actor(Actor* actor);                     // Add a new actor to StudentWorld
  void add_initial_actors(void);                    // Add initial actors
  void add_additional_actors(void);                 // Add additional actors
  void update_scoreboard(void);                     // Update the scoreboard display
  /* Mutator Functions */
  void update_spaceship_laser_count(bool how_much); // Updates the status of the player's laser in the space field
  void update_flying_saucer_count(bool how_much);   // Updates the status of the flying saucer in the space field
  void update_invader_laser_count(int how_much);    // Updates the current number of enemy lasers in the space field
  void update_current_invader_count(int how_much);  // Update the number of invaders left to destroy before advancing to the next level
  void update_invader_speed(int how_much);          // Update the speed of the invaders
  void set_spaceship_laser_count(void);             // Sets the status of the player's laser in the space field
  void set_flying_saucer_count(void);               // Sets the status of the flying saucer in the space field
  void set_invader_laser_count(void);               // Sets the current number of enemy lasers in the space field
  void set_current_invader_count(void);             // Resets the number of invaders to destroy per round (to 55)
  void set_invader_speed(void);                     // Sets the speed of the invaders
  /* Accessor Functions */
  bool get_spaceship_laser_count(void) const;       // Returns true if there is a player laser currently in the space field
  bool get_flying_saucer_count(void) const;         // Returns true if there is currently a flying saucer in the space field
  int get_invader_laser_count(void) const;          // Returns the number of lasers currently in the space field
  int get_current_invader_count(void) const;        // Returns the number of invaders left to destroy before completing the current level
  int get_invader_speed(void) const;                // Returns the speed of the invaders for a given round
  /* Laser Handling Functions */
  // Checks if there were any collisions between actor objects
  void check_collision(Actor* actor, bool is_player=true, bool is_invader=false, bool is_projectile=false);
  bool is_invader_below(LargeInvader* invader);     // Returns true if there is an invader in the same column, that is a row beneath the invader
  /* Invader Movement Functions */
  void reset_original_invader_speed(void);          // Resets each invader to their original movement speed upon the start of each new round
  /* Math Helper Functions */
  int max(int x, int y) ;                           // Returns the max of two numbers
  int min(int x, int y) ;                           // Returns the min of two numbers
  int rand_int(int min, int max) const;             // Generates a pseudo-random number in a given range
  virtual ~StudentWorld();
  
private:
  Spaceship*          m_spaceship;                  // Player Spaceship
  Border*             m_border[VIEW_WIDTH][1];      // Border around the screen
  std::vector<Actor*> m_actors;                     // All actor objects (i.e. space invaders, flying saucer, lasers, perhaps barriers too)
  bool                m_spaceship_laser_count;      // Variable to keep track if the player currently has a laser in the space field
  bool                m_flying_saucer_count;        // Variable to keep track if there is currently a flying saucer (UFO) in the space field
  int                 m_invader_laser_count;        // Variable to keep track of all of the invaders' lasers in the space field (as there is a cap)
  int                 m_current_invader_count;      // Variable to keep track of how many aliens have been killed by the player
  int                 m_invader_speed;              // Variable that controls the speed of the invaders for a given round
  int                 m_multiple;
  /* Private Functions */
  void init_border(void);                           // Initialize the border
  void deinit_border(void);                         // Deinitialize the border
};

#endif // STUDENTWORLD_H_
