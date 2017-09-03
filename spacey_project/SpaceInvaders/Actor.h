/*******************************************************************************
 *Actor.h
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-23 11:11:11 -0700 (Wed, 23 Aug 2017) $
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
  Spaceship(StudentWorld* current_world, int start_x=30, int start_y=5);
  virtual void do_something(void);
  virtual ~Spaceship();
  
private:
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------LARGE INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

// IID_BOULDER
class LargeInvader : public Spaceship
{
public:
  LargeInvader(StudentWorld* world, int start_x, int start_y);
  virtual void do_something(void);
  virtual ~LargeInvader();
  
private:

};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MEDIUM INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

// IID_BARREL
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

// IID_PROTESTER
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
class FlyingSaucer : public LargeInvader
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
  Laser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id=IID_WATER_SPURT, Direction dir=up);
  virtual void do_something(void);
  virtual unsigned int laser_speed(void) const;   // Returns the class of the laser (i.e. player laser, aliens slow, medium, or fast laser
  bool get_projectile_viewpoint(void) const;      // Returns true if the laser belongs to the player spaceship (and false if it is an alien's)
  ~Laser();
  
private:
  bool m_spaceship_laser;
};

///////////////////////////////////////////////////////////////////////////
////////////////////-----------SLOW LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class SlowLaser : public Laser
{
public:
  SlowLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id=IID_WATER_SPURT, Direction dir=down);
  virtual unsigned int laser_speed(void) const;   // Returns the class of the laser (i.e. player laser, aliens slow, medium, or fast laser
  bool get_projectile_viewpoint(void) const;      // Returns true if the laser belongs to the player spaceship (and false if it is an alien's)
  ~SlowLaser();
  
private:
};

///////////////////////////////////////////////////////////////////////////
///////////////////-----------MEDIUM LASER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

class MediumLaser : public Laser
{
public:
  MediumLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id=IID_WATER_SPURT, Direction dir=down);
  virtual unsigned int laser_speed(void) const;   // Returns the class of the laser (i.e. player laser, aliens slow, medium, or fast laser
  bool get_projectile_viewpoint(void) const;      // Returns true if the laser belongs to the player spaceship (and false if it is an alien's)
  ~MediumLaser();
  
private:
};

///////////////////////////////////////////////////////////////////////////
////////////////////-----------FAST LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

class FastLaser : public Laser
{
public:
  FastLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id=IID_WATER_SPURT, Direction dir=down);
  virtual unsigned int laser_speed(void) const;   // Returns the class of the laser (i.e. player laser, aliens slow, medium, or fast laser
  bool get_projectile_viewpoint(void) const;      // Returns true if the laser belongs to the player spaceship (and false if it is an alien's)
  ~FastLaser();
  
private:
};

#endif // ACTOR_H_
