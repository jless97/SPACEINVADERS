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
  StudentWorld* world(void);
  virtual ~Actor();
  
private:
  StudentWorld* m_world;
  bool m_alive;
};

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------SPACESHIP--------------////////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PLAYER
class Spaceship : public Actor
{
public:
  Spaceship(StudentWorld* world, int start_x=30, int start_y=12, int image_id=IID_PLAYER, double image_size=1.0);
  virtual void do_something(void);
  virtual ~Spaceship();
  
private:
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------LARGE INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PROTESTER
class LargeInvader : public Spaceship
{
public:
  LargeInvader(StudentWorld* world, int start_x, int start_y, int image_id=IID_PROTESTER, double image_size=1.0, int dir=1);
  virtual void do_something(void);
  /* Mutator Functions */
  void update_can_move_status(int how_much);      // Updates the status of when the invaders can move again
  void set_movement_direction(int dir);           // Update the current direction the invaders are moving in
  void set_next_movement_direction(int dir);      // Update the next direction to move in (used after moving down)
  void set_can_move_status(int value);            // Sets the status of whether the invaders can move this tick
  /* Accessor Functions */
  int get_movement_direction(void);               // Returns the current direction that the invaders are moving in
  int get_next_movement_direction(void);          // Returns the next direction that the invader will take (used after moving down)
  int get_can_move_status(void) const;            // Returns the status of when the invaders get to move
  virtual ~LargeInvader();
  
private:
  int       m_direction;      // 0: left, 1: right, 2: down
  int       m_next_direction;
  int       m_can_move;
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MEDIUM INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

// IID_HARDCORE_PROTESTER
class MediumInvader : public LargeInvader
{
public:
  MediumInvader(StudentWorld* world, int start_x, int start_y);
  virtual ~MediumInvader();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------SMALL INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

// IID_PLAYER
class SmallInvader : public LargeInvader
{
public:
  SmallInvader(StudentWorld* world, int start_x, int start_y);
  virtual ~SmallInvader();
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------FLYINGSAUCER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

// IID_GOLD
class FlyingSaucer : public Actor
{
public:
  FlyingSaucer(StudentWorld* world, int start_x, int start_y);
  virtual ~FlyingSaucer();
  
private:
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

// IID_WATERSQUIRT
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
  Laser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class, int image_id=IID_WATER_SPURT, Direction dir=up);
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
