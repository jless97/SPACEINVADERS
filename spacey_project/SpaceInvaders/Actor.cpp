/*******************************************************************************
 *Actor.cpp
 *
 * This routine is responsible for all of the objects (actors) within the game
 * The main function of each actor is to do something each clock tick
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-08-23 11:11:11 -0700 (Wed, 23 Aug 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GLOBALS--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------ACTOR--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* current_world, int image_id, int start_x, int start_y, Direction dir, double image_size, unsigned int image_depth)
: GraphObject(image_id, start_x, start_y, dir, image_size, image_depth), m_world(current_world), m_alive(true) { set_visible(true); }

bool Actor::is_alive(void) { return m_alive; }

void Actor::set_dead(void) { m_alive = false; set_visible(false); }

StudentWorld* Actor::world(void) { return m_world; }

Actor::~Actor() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------SPACESHIP--------------////////////////////
///////////////////////////////////////////////////////////////////////////

Spaceship::Spaceship(StudentWorld* world, int start_x, int start_y, int image_id, double image_size)
: Actor(world, image_id, start_x, start_y, Direction::up, image_size, 0) {}

void Spaceship::do_something(void)
{
  if (!is_alive()) { return; } // Check if the player spaceship is alive
  
  int x = get_x(), y = get_y(); // Get the player spaceships' current coordinates
  
  StudentWorld* space_world = world(); // Get pointer to the StudentWorld
  
  // Check if player pressed a valid key
  int key;
  if(space_world->get_key(key))
  {
    switch(key)
    {
      // Player spaceship fires a laser
      case KEY_PRESS_SPACE:
        // If there is no player spaceship laser currently in the space field, shoot another
        if (!space_world->get_spaceship_laser_count()) { new Laser(space_world, x, y + 1, Laser::LaserClass::player_laser); }
        break;
      // Left
      case KEY_PRESS_LEFT:
        if (x > 0) { move_to(x - 1, y); }
        break;
      // Right
      case KEY_PRESS_RIGHT:
        if (x < VIEW_WIDTH - 1) { move_to(x + 1, y); }
        break;
      default:
        break;
    }
  }
}

Spaceship::~Spaceship() {}

///////////////////////////////////////////////////////////////////////////
//////////////////-----------LARGE INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

LargeInvader::LargeInvader(StudentWorld* world, int start_x, int start_y, int image_id, double image_size)
: Spaceship(world, start_x, start_y, IID_BOULDER, 1.0) { world->add_actor(this); }

void LargeInvader::do_something(void)
{
  
  
}

LargeInvader::~LargeInvader() {}
        
///////////////////////////////////////////////////////////////////////////
//////////////////-----------MEDIUM INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

MediumInvader::MediumInvader(StudentWorld* world, int start_x, int start_y)
: LargeInvader(world, start_x, start_y, IID_BARREL, 1.0) { world->add_actor(this); }

MediumInvader::~MediumInvader() {}

///////////////////////////////////////////////////////////////////////////
//////////////////-----------SMALL INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

SmallInvader::SmallInvader(StudentWorld* world, int start_x, int start_y)
: LargeInvader(world, start_x, start_y, IID_PROTESTER, 1.0) { world->add_actor(this); }

SmallInvader::~SmallInvader() {}

///////////////////////////////////////////////////////////////////////////
///////////////////-----------FLYINGSAUCER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------BARRIER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------LASER--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

Laser::Laser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class, int image_id, Direction dir)
: Actor(world, image_id, start_x, start_y, dir, 0.50, 1), m_spaceship_laser(LaserClass::player_laser)
{
  world->add_actor(this);
  world->play_sound(SOUND_LASER);
  if (get_projectile_viewpoint()) { world->update_spaceship_laser_count(true); }
  else { world->update_alien_laser_count(1); }
}

void Laser::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the laser object
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the bullet object
  
  StudentWorld* laser_world = world(); // Grab a pointer to the StudentWorld
  
  switch (get_projectile_viewpoint())
  {
    case LaserClass::player_laser:
      laser_world->check_collision(this, true, false); // Check if the laser collided with any aliens
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y + 1); // If no collision, then update position
      if (y >= VIEW_HEIGHT - 1) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, true, false); // Check if the laser collided with any spaceships
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::slow_laser:
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 1); // If no collision, then update position
      if (y <= 0) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::medium_laser:
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 2); // If no collision, then update position
      if (y <= 0) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::fast_laser:
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 3); // If no collision, then update position
      if (y <= 0) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, true, false); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    default:
      break;
  }
}

Laser::LaserClass Laser::get_projectile_viewpoint(void) const { return m_spaceship_laser; }

void Laser::set_laser_speed(Laser::LaserClass value) { m_spaceship_laser = value; }

Laser::~Laser()
{
  if (get_projectile_viewpoint() == LaserClass::player_laser) { world()->update_spaceship_laser_count(false); }
  else { world()->update_alien_laser_count(-1); }
}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------SLOW LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

SlowLaser::SlowLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_WATER_SPURT, Direction::down) { set_laser_speed(LaserClass::slow_laser); }

SlowLaser::~SlowLaser() {}

///////////////////////////////////////////////////////////////////////////
///////////////////-----------MEDIUM LASER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

MediumLaser::MediumLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_WATER_SPURT, Direction::down) { set_laser_speed(LaserClass::medium_laser); }

MediumLaser::~MediumLaser() {}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------FAST LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

FastLaser::FastLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_WATER_SPURT, Direction::down) { set_laser_speed(LaserClass::fast_laser); }

FastLaser::~FastLaser() {}

/* End of Actors */
