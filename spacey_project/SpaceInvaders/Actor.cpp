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
#include <random>
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

void Actor::set_alive(void) { m_alive = true; set_visible(true); }

StudentWorld* Actor::world(void) { return m_world; }

Actor::~Actor() { set_visible(false); }

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------BORDER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

Border::Border(StudentWorld* world, int start_x, int start_y)
: Actor(world, IID_BORDER, start_x, start_y, Direction::right, 0.25, 1) {}

void Border::do_something(void) {}

Border::~Border() {}

///////////////////////////////////////////////////////////////////////////
////////////////-----------INVADER EXPLOSION--------------/////////////////
///////////////////////////////////////////////////////////////////////////

InvaderExplosion::InvaderExplosion(StudentWorld* world, int start_x, int start_y, int image_id, int image_duration)
: Actor(world, image_id, start_x, start_y, Direction::right, 1.0, 0), m_display(image_duration) { world->add_actor(this); }

void InvaderExplosion::do_something(void)
{
  if (m_display <= 0) { set_dead(); return; } // The explosion image should only be displayed for a given duration
  
  move_to(get_x(), get_y()); // Move image in place to generate the two sprite images
  
  m_display--; // Decrement the count to disappear
}

InvaderExplosion::~InvaderExplosion() {}

///////////////////////////////////////////////////////////////////////////
/////////////////-----------PLAYER EXPLOSION--------------/////////////////
///////////////////////////////////////////////////////////////////////////

PlayerExplosion::PlayerExplosion(StudentWorld* world, int start_x, int start_y, int image_duration)
: InvaderExplosion(world, start_x, start_y, IID_PLAYER_KILLED, image_duration) {}

PlayerExplosion::~PlayerExplosion() {}

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------SPACESHIP--------------////////////////////
///////////////////////////////////////////////////////////////////////////

Spaceship::Spaceship(StudentWorld* world, int start_x, int start_y, int image_id, double image_size)
: Actor(world, image_id, start_x, start_y, Direction::right, image_size, 0) {}

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
        if (x > 4) { move_to(x - 4, y); }
        break;
      // Right
      case KEY_PRESS_RIGHT:
        if (x < 60) { move_to(x + 4, y); }
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

LargeInvader::LargeInvader(StudentWorld* world, int start_x, int start_y, int row, int image_id, double image_size, int dir)
: Spaceship(world, start_x, start_y, image_id, image_size), m_direction(dir), m_next_direction(0), m_ticks(0), m_max_ticks(25), m_row_number(row)
{ world->add_actor(this); }

void LargeInvader::do_something(void)
{  
  if (!is_alive()) { return; } // Check the current status of the invader object
  
  if (get_ticks() < get_max_ticks()) { update_ticks(1); return; }
  else { set_ticks(0); }

  StudentWorld* invader_world = world(); // Grab a pointer to the StudentWorld

  int x = get_x(), y = get_y(); // Get the current coordinates of the invader object
  
  if (!is_alive()) { return; } // Check the current status of the invader object
  
  // Update invader movement
  switch (get_movement_direction())
  {
    case 0:
      if (!is_alive()) { return; } // Check the current status of the invader object
      if (x > 0) { move_to(x - 1, y); }
      if (!is_alive()) { return; } // Check the current status of the invader object
      break;
    case 1:
      if (!is_alive()) { return; } // Check the current status of the invader object
      if (x < VIEW_WIDTH - 1) { move_to(x + 1, y); }
      if (!is_alive()) { return; } // Check the current status of the invader object
      break;
    case 2:
      if (!is_alive()) { return; } // Check the current status of the invader object
      move_to(x, y - 3);
      if (!is_alive()) { return; } // Check the current status of the invader object
      set_movement_direction(get_next_movement_direction());
    default:
      break;
  }
  
  // Check if invaders should shoot lasers
  if (invader_world->get_invader_laser_count() < 3)
  {
    // Chance an invader will shoot a laser
    if (rand() % 50 == 1)
    {
      // Chance it is a faster laser
      if (rand() % 10 == 1)
      {
        // Fast laser
        if (rand() % 10 == 1)
        {
          if (!invader_world->is_invader_below(this)) { new FastLaser(invader_world, x, y - 1, Laser::LaserClass::fast_laser); }
        }
        // Medium laser
        else
        {
          if (!invader_world->is_invader_below(this)) { new MediumLaser(invader_world, x, y - 1, Laser::LaserClass::medium_laser); }
        }
      }
      // Slow laser
      else
      {
        if (!invader_world->is_invader_below(this)) { new SlowLaser(invader_world, x, y - 1, Laser::LaserClass::slow_laser); }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//////////////////-----------MUTATOR FUNCTIONS-------------////////////////
///////////////////////////////////////////////////////////////////////////

void LargeInvader::update_ticks(int how_much) { if (this == nullptr) { return; }m_ticks += how_much; }
  
void LargeInvader::set_movement_direction(int dir) { if (this == nullptr) { return; } m_direction = dir; }

void LargeInvader::set_next_movement_direction(int dir) { if (this == nullptr) { return; } m_next_direction = dir; }

void LargeInvader::set_ticks(int value) { if (this == nullptr) { return; } m_ticks = value; }

void LargeInvader::set_max_ticks(int value) { if (this == nullptr) { return; } m_max_ticks = value; }

void LargeInvader::set_row_number(int value) { if (this == nullptr) { return; } m_row_number = value; }

///////////////////////////////////////////////////////////////////////////
/////////////////-----------ACCESSOR FUNCTIONS-------------////////////////
///////////////////////////////////////////////////////////////////////////

int LargeInvader::get_movement_direction(void) { if (this == nullptr) { return -1; } return m_direction; }

int LargeInvader::get_next_movement_direction(void) { if (this == nullptr) { return -1; } return m_next_direction; }

int LargeInvader::get_ticks(void) { if (this == nullptr) { return -1; } return m_ticks; }

int LargeInvader::get_max_ticks(void) { if (this == nullptr) { return -1; } return m_max_ticks; }

int LargeInvader::get_row_number(void) { if (this == nullptr) { return -1; } return m_row_number; }

LargeInvader::~LargeInvader() { world()->update_current_invader_count(-1); }
        
///////////////////////////////////////////////////////////////////////////
//////////////////-----------MEDIUM INVADER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

MediumInvader::MediumInvader(StudentWorld* world, int start_x, int start_y, int row)
: LargeInvader(world, start_x, start_y, row, IID_MEDIUM_INVADER, 0.9) {}

MediumInvader::~MediumInvader() {}

///////////////////////////////////////////////////////////////////////////
//////////////////-----------SMALL INVADER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

SmallInvader::SmallInvader(StudentWorld* world, int start_x, int start_y, int row)
: LargeInvader(world, start_x, start_y, row, IID_SMALL_INVADER, 0.8) {}

SmallInvader::~SmallInvader() {}

///////////////////////////////////////////////////////////////////////////
///////////////////-----------FLYINGSAUCER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

FlyingSaucer::FlyingSaucer(StudentWorld* world, int start_x, int start_y)
: Actor(world, IID_FLYING_SAUCER, start_x, start_y, Direction::right, 1.0, 0), m_can_move(true)
{ world->add_actor(this); world->update_flying_saucer_count(true); }

void FlyingSaucer::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the flying saucer
  
  if (!get_can_move()) { set_can_move(true); return; }
  else { set_can_move(false); }
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the flying saucer
  
  move_to(x - 1, y);
  
  if (x <= -5) { set_dead(); }
}

void FlyingSaucer::set_can_move(bool value) { m_can_move = value; }

bool FlyingSaucer::get_can_move(void) const { return m_can_move; }

FlyingSaucer::~FlyingSaucer() { world()->update_flying_saucer_count(false); }

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------BARRIER--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------LASER--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

Laser::Laser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class, int image_id, Direction dir)
: Actor(world, image_id, start_x, start_y, dir, 0.4, 1), m_spaceship_laser(laser_class)
{
  world->add_actor(this);
  world->play_sound(SOUND_LASER);
  if (get_projectile_viewpoint() == LaserClass::player_laser) { world->update_spaceship_laser_count(true); }
  else { world->update_invader_laser_count(1); }
}

void Laser::do_something(void)
{
  if (!is_alive()) { return; } // Check the current status of the laser object
  
  int x = get_x(), y = get_y(); // Get the current coordinates of the bullet object
  
  StudentWorld* laser_world = world(); // Grab a pointer to the StudentWorld
  
  switch (get_projectile_viewpoint())
  {
    case LaserClass::player_laser:
      laser_world->check_collision(this, true, false, true); // Check if the laser collided with any aliens
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y + 2); // If no collision, then update position
      if (y >= VIEW_HEIGHT - 1) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, true, false, true); // Check if the laser collided with any spaceships
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::slow_laser:
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 1); // If no collision, then update position
      if (y <= BORDER_HEIGHT) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::medium_laser:
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 1.5); // If no collision, then update position
      if (y <= BORDER_HEIGHT) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      break;
    case LaserClass::fast_laser:
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
      if (!is_alive()) { return; } // Check the current status of the laser
      move_to(x, y - 2); // If no collision, then update position
      if (y <= BORDER_HEIGHT) { set_dead(); }
      if (!is_alive()) { return; } // Check the current status of the laser
      laser_world->check_collision(this, false, true, true); // Check if the laser hit the player spaceship (or barrier)
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
  else { world()->update_invader_laser_count(-1); }
}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------SLOW LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

SlowLaser::SlowLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_SLOW_LASER, Direction::left) { set_laser_speed(LaserClass::slow_laser); }

SlowLaser::~SlowLaser() {}

///////////////////////////////////////////////////////////////////////////
///////////////////-----------MEDIUM LASER--------------///////////////////
///////////////////////////////////////////////////////////////////////////

MediumLaser::MediumLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_MEDIUM_LASER, Direction::left) { set_laser_speed(LaserClass::medium_laser); }

MediumLaser::~MediumLaser() {}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------FAST LASER--------------////////////////////
///////////////////////////////////////////////////////////////////////////

FastLaser::FastLaser(StudentWorld* world, int start_x, int start_y, Laser::LaserClass laser_class)
: Laser(world, start_x, start_y, laser_class, IID_FAST_LASER, Direction::left) { set_laser_speed(LaserClass::fast_laser); }

FastLaser::~FastLaser() {}

/* End of Actors */
