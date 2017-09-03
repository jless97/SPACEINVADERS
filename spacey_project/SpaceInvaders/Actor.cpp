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

Actor::~Actor() {}

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------SPACESHIP--------------////////////////////
///////////////////////////////////////////////////////////////////////////

Spaceship::Spaceship(StudentWorld* current_world, int start_x, int start_y)
: Actor(current_world, IID_PLAYER, start_x, start_y, Direction::up, 1.0, 0) {}

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
        if (!space_world->get_spaceship_laser_count()) { new Laser(space_world, x, y + 1, true); }
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
///////////////////////-----------ALIEN--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////-----------FLYINGSAUCER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------BARRIER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------LASER--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

Laser::Laser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id, Direction dir)
: Actor(world, image_id, start_x, start_y, dir, 0.50, 1), m_spaceship_laser(true)
{
  world->add_actor(this);
  if (get_projectile_viewpoint()) { world->update_spaceship_laser_count(true); }
  else { world->update_alien_laser_count(1); }
}

void Laser::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the laser object
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the bullet object
  
  StudentWorld* laser_world = world(); // Grab a pointer to the StudentWorld
  
  // Player projectile
  if (get_projectile_viewpoint())
  {
    laser_world->check_collision(this, true, false); // Check if the laser collided with any aliens
    if (!is_alive()) { return; } // Check the current status of the laser
    move_to(x, y + 1); // If no collision, then update position
    if (y >= VIEW_HEIGHT - 1) { set_dead(); }
    if (!is_alive()) { return; } // Check the current status of the laser
    laser_world->check_collision(this, true, false); // Check if the laser collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the laser
  }
  // Enemy projectile
  else
  {
    laser_world->check_collision(this, false, true); // Check if the laser collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the laser
    move_to(x, y - 1); // If no collision, then update position
    if (y <= 0) { set_dead(); }
    if (!is_alive()) { return; } // Check the current status of the laser
    laser_world->check_collision(this, false, true); // Check if the laser collided with any spaceships
    if (!is_alive()) { return; } // Check the current status of the laser
  }
}

bool Laser::get_projectile_viewpoint(void) const { return m_spaceship_laser == true; }

unsigned int Laser::laser_speed(void) const { return 0; }

Laser::~Laser()
{
  if (get_projectile_viewpoint()) { world()->update_spaceship_laser_count(false); }
  else { world()->update_alien_laser_count(-1); }
}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------SLOW LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

SlowLaser::SlowLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id, Direction dir)
: Laser(world, start_x, start_y, false, image_id, dir) {}

unsigned int SlowLaser::laser_speed(void) const { return 1; }

SlowLaser::~SlowLaser() {}

///////////////////////////////////////////////////////////////////////////
///////////////////-----------MEDIUM LASER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

MediumLaser::MediumLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id, Direction dir)
: Laser(world, start_x, start_y, false, image_id, dir) {}

unsigned int MediumLaser::laser_speed(void) const { return 2; }

MediumLaser::~MediumLaser() {}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------FAST LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

FastLaser::FastLaser(StudentWorld* world, int start_x, int start_y, bool player_spaceship_laser, int image_id, Direction dir)
: Laser(world, start_x, start_y, false, image_id, dir) {}

unsigned int FastLaser::laser_speed(void) const { return 3; }

FastLaser::~FastLaser() {}

/* End of Actors */
