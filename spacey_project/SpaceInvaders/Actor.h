/*******************************************************************************
 *Actor.h
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-09-02 11:11:11 -0700 (Sat, 02 Sep 2017) $
 *
 ******************************************************************************/

#ifndef ACTOR_H_
#define ACTOR_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "GraphObject.h"

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GLOBALS--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------ACTOR--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

class StudentWorld;

class Actor : public GraphObject
{
public:
  Actor(StudentWorld* current_world, int image_id, int start_x, int start_y, Direction dir, double image_size, unsigned int image_depth);
  virtual void do_something() = 0;
  bool is_alive(void);
  void set_dead(void);
  void set_alive(void);
  StudentWorld* world(void);
  virtual ~Actor();
  
private:
  StudentWorld* m_world;
  bool m_alive;
};

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------BORDER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

// IID_BORDER
class Border : public Actor
{
public:
  Border(StudentWorld* world, int start_x, int start_y);
  virtual void do_something(void);
  virtual ~Border();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////-----------INVADER EXPLOSION--------------/////////////////
///////////////////////////////////////////////////////////////////////////

// IID_INVADER_KILLED
class InvaderExplosion : public Actor
{
public:
  InvaderExplosion(StudentWorld* world, int start_x, int start_y, int image_id=IID_INVADER_KILLED, int image_duration=6);
  virtual void do_something(void);
  virtual ~InvaderExplosion();
  
private:
  int m_display;
};

///////////////////////////////////////////////////////////////////////////
/////////////////-----------PLAYER EXPLOSION--------------/////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PLAYER_KILLED
class PlayerExplosion : public InvaderExplosion
{
public:
  PlayerExplosion(StudentWorld* world, int start_x, int start_y, int image_duration=20);
  virtual ~PlayerExplosion();
  
private:
};

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------SPACESHIP--------------////////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PLAYER
class Spaceship : public Actor
{
public:
  Spaceship(StudentWorld* world, int start_x=34, int start_y=4, int image_id=IID_SPACESHIP, double image_size=1.0);
  virtual void do_something(void);
  virtual ~Spaceship();
  
private:
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------LARGE INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

// IID_LARGE_INVADER
class LargeInvader : public Spaceship
{
public:
  LargeInvader(StudentWorld* world, int start_x, int start_y, int row, int image_id=IID_LARGE_INVADER, double image_size=1.0, int dir=1);
  virtual void do_something(void);
  /* Mutator Functions */
  void update_ticks(int how_much);            // Updates the status of when the invaders can move again
  void set_movement_direction(int dir);       // Update the current direction the invaders are moving in
  void set_next_movement_direction(int dir);  // Update the next direction to move in (used after moving down)
  void set_ticks(int value);                  // Sets the status of whether the invaders can move this tick
  void set_max_ticks(int value);              // Sets the max ticks the invader has to wait before they can do something
  void set_row_number(int value);             // Sets the row number that the invader belongs to
  /* Accessor Functions */
  int get_movement_direction(void);           // Returns the current direction that the invaders are moving in
  int get_next_movement_direction(void);      // Returns the next direction that the invader will take (used after moving down)
  int get_ticks(void);                        // Returns the status of when the invaders get to move
  int get_max_ticks(void);                    // Returns the total ticks an invader has to wait for a given round before they can do something
  int get_row_number(void);                   // Returns the row number that the invader belongs to
  virtual ~LargeInvader();
  
private:
  int       m_direction;      // Direction the invaders are moving in (0: left, 1: right, 2: down)
  int       m_next_direction; // Variable that handles when the invaders will move down a row
  int       m_ticks;          // Counts the non-resting ticks before the invader can do something
  int       m_max_ticks;      // The max ticks for this round to determine the movement speed of the invader
  int       m_row_number;     // The row number that the invader belongs to (i.e. 1st, 2nd, 3rd, 4th, or 5th)
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MEDIUM INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

// IID_MEDIUM_INVADER
class MediumInvader : public LargeInvader
{
public:
  MediumInvader(StudentWorld* world, int start_x, int start_y, int row);
  virtual ~MediumInvader();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------SMALL INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

// IID_SMALL_INVADER
class SmallInvader : public LargeInvader
{
public:
  SmallInvader(StudentWorld* world, int start_x, int start_y, int row);
  virtual ~SmallInvader();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------FLYINGSAUCER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

// IID_FLYING_SAUCER
class FlyingSaucer : public Actor
{
public:
  FlyingSaucer(StudentWorld* world, int start_x, int start_y);
  virtual void do_something(void);
  /* Mutator Functions */
  void set_can_move(bool value);
  /* Accessor Functions */
  bool get_can_move(void) const;
  virtual ~FlyingSaucer();
  
private:
  bool m_can_move;
};

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------BARRIER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

// IID_BOULDER
class Barrier : public Actor
{
public:
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------LASER--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PLAYER_LASER
class Laser : public Actor
{
public:
  enum LaserClass
  {
    player_laser = 0,
    slow_laser = 1,
    medium_laser = 2,
    fast_laser = 3
  };
  Laser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class, int image_id=IID_PLAYER_LASER, Direction dir=right);
  virtual void do_something(void);
  Laser::LaserClass get_projectile_viewpoint(void) const; // Returns the class of the laser (i.e. player, slow, medium, or fast)
  virtual void set_laser_speed(Laser::LaserClass value);  // Sets the class of the laser (i.e. player, slow, medium, or fast)
  ~Laser();
  
private:
  LaserClass m_spaceship_laser;
};

///////////////////////////////////////////////////////////////////////////
////////////////////-----------SLOW LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class SlowLaser : public Laser
{
public:
  SlowLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class);
  ~SlowLaser();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------MEDIUM LASER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class MediumLaser : public Laser
{
public:
  MediumLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class);
  ~MediumLaser();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////-----------FAST LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class FastLaser : public Laser
{
public:
  FastLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class);
  ~FastLaser();
  
private:
};

#endif // ACTOR_H_
