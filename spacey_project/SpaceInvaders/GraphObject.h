#ifndef GRAPHOBJ_H_
#define GRAPHOBJ_H_

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "SpriteManager.h"
#include "GameConstants.h"
#include <set>
#include <cmath>

///////////////////////////////////////////////////////////////////////////
//////////////////////-----------GLOBALS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

const int ANIMATION_POSITIONS_PER_TICK = 1;
const int NUM_LAYERS = 4;

///////////////////////////////////////////////////////////////////////////
////////////////////-----------GRAPHOBJECT--------------///////////////////
///////////////////////////////////////////////////////////////////////////

inline int round_away_from_zero(double r)
{
	double result =	 (r > 0) ? (std::floor(r + 0.5)) : (std::ceil(r - 0.5));
	return int(result);
  // If C99 is available, this can be return std::round(r);
}

class GraphObject {
public:
  enum Direction
  {
    none = 0,
    up,
    down,
    left,
    right
  };
  GraphObject(int image_id, int start_x, int start_y, Direction dir=up, double size=1.0, unsigned int depth=0)
   : m_image_id(image_id), m_visible(false), m_x(start_x), m_y(start_y), m_destX(start_x), m_destY(start_y), m_brightness(1.0),
     m_animationNumber(0), m_direction(dir), m_size(size), m_depth(depth)
  { if (m_size <= 0) { m_size = 1; } get_graph_objects(m_depth).insert(this); }
  void set_visible(bool should_i_display) { m_visible = should_i_display; }     // Set the visibility state of the objects (i.e. visible or invisible)
  void set_brightness(double brightness) { m_brightness = brightness; }         // Set the brightness of the actor's image
  int get_x() const                                                             // Get the actor's X coordinate
  {
    // If already moved but not yet animated, use new location anyway.
    return round_away_from_zero(m_destX);
  }
  int get_y() const                                                             // Get the actor's Y coordinate
  {
    // If already moved but not yet animated, use new location anyway.
    return round_away_from_zero(m_destY);
  }
  void set_id(int image_id) { m_image_id = image_id; }
  void move_to(int x, int y) { m_destX = x; m_destY = y; increase_animation_number(); } // Update the loaction of the actor
  Direction get_direction() const { return m_direction; }                       // Get the current direction of the actor
  void set_direction(Direction d) { m_direction = d; }                          // Set the current direction of the actor
  double get_size() const { return m_size; }                                    // Get the size of the actor object
  /* The following should be used by only the framework, not the student */
  bool is_visible() const { return m_visible; }                                 // Is the actor object visible
  unsigned int get_id() const { return m_image_id; }                            // ID tag of the actor
  double get_brightness() const { return m_brightness; }                        // Brightness setting of the actor object
  unsigned int get_animation_number() const { return m_animationNumber; }       // Animation number
  void get_animation_location(double& x, double& y) const { x = m_x; y = m_y; } // Location of the actor object to be displayed
  void animate() { move_a_little(m_x, m_destX); move_a_little(m_y, m_destY); }  // Animate the actor object on the screen
  static std::set<GraphObject*>& get_graph_objects(unsigned int layer)          // Graph Object stuff
  {
    static std::set<GraphObject*> graph_objects[NUM_LAYERS];
    if (layer < NUM_LAYERS) { return graph_objects[layer]; }
    else { return graph_objects[0]; } // empty;
  }
  virtual ~GraphObject() { get_graph_objects(m_depth).erase(this); }

private:
  GraphObject(const GraphObject&);            // Prevent copying or assigning graph_objects
  GraphObject& operator=(const GraphObject&);
  // Actors are derived from the GraphObject (thus they all have the following attributes)
  int		    m_image_id;
  bool	    m_visible;
  double	  m_x;
  double	  m_y;
  double	  m_destX;
  double	  m_destY;
  double	  m_brightness;
  int		    m_animationNumber;
  Direction	m_direction;
  double	  m_size;
  int		    m_depth;
  /* The following should be used by only the framework, not the student */
  void move_a_little(double& from, double& to) {
    static const double DISTANCE = 1.0/ANIMATION_POSITIONS_PER_TICK;
    if (to - from >= DISTANCE) { from += DISTANCE; }
    else if (from - to >= DISTANCE) { from -= DISTANCE; }
    else { from = to; }
  }
  void increase_animation_number() { m_animationNumber++; } // Increment the animation number
};

#endif // GRAPHOBJ_H_
