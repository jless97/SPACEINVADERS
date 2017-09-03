/*******************************************************************************
 *GameController.c
 *
 * This routine implements is the game engine for Space Invaders
 *
 *
 * $LastChangedBy: jless $
 * $LastChangedDate: 2017-09-02 11:11:11 -0700 (Sat, 02 Sep 2017) $
 *
 ******************************************************************************/

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------INCLUDES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

#include "freeglut.h"
#include "GameController.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "SoundFX.h"
#include "SpriteManager.h"
#include <string>
#include <map>
#include <utility>
#include <cstdlib>
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////
/////////////////////-----------GLOBALS--------------//////////////////////
///////////////////////////////////////////////////////////////////////////

/*
spriteWidth = .67
spritesPerRow = 16

RowWidth = spriteWidth*spritesPerRow = 10.72
PixelWidth = RowWidth/256 = .041875
newSpriteWidth = PixelWidth * NumPixels

spriteHeight = .54
spritesPerRow = 16

RowHeight = spriteHeight*spritesPerCol = 8.64

PixelHeight = RowHeight/256 = .03375

newSpriteHeight = PixelHeight * NumPixels
*/

static const int WINDOW_WIDTH = 768; //1024;
static const int WINDOW_HEIGHT = 768;

static const int PERSPECTIVE_NEAR_PLANE = 4;
static const int PERSPECTIVE_FAR_PLANE	= 22;

static const double VISIBLE_MIN_X = -2.15; // -2.04375; //	-2.4375; //-3.25;
static const double VISIBLE_MAX_X = 1.85; //  2.04375;	  //1.65;// 2.4375; //3.25;
static const double VISIBLE_MIN_Y = -2.1;
static const double VISIBLE_MAX_Y = 1.9;
static const double VISIBLE_MIN_Z = -20;
//static const double VISIBLE_MAX_Z = -6;

static const double FONT_SCALEDOWN = 760.0;

static const double SCORE_Y = 3.8;
static const double SCORE_Z = -10;

static const int MS_PER_FRAME = 1;

//static const double PI = 4 * atan(1.0);

struct SpriteInfo
{
	unsigned int image_id;
	unsigned int frame_num;
	std::string	 tga_filename;
};

///////////////////////////////////////////////////////////////////////////
//////////////////-----------GAMECONTROLLER--------------//////////////////
///////////////////////////////////////////////////////////////////////////

static void convert_to_glut_coords(double x, double y, double& gx, double& gy, double& gz);
static void draw_prompt(string main_message, string second_message);
static void draw_score_and_lives(string);

///////////////////////////////////////////////////////////////////////////
/////////-----------GAMECONTROLLER PRIVATE FUNCTIONS--------------/////////
///////////////////////////////////////////////////////////////////////////

void GameController::init_drawers_and_sounds(void)
{
	SpriteInfo drawers[] = {
    { IID_SPACESHIP,        0, "player.tga" },
    { IID_LARGE_INVADER,    0, "large_invader.tga" },
    { IID_LARGE_INVADER,    1, "large_invader2.tga" },
    { IID_MEDIUM_INVADER,   0, "medium_invader.tga" },
    { IID_MEDIUM_INVADER,   1, "medium_invader2.tga" },
    { IID_SMALL_INVADER,    0, "small_invader.tga" },
    { IID_SMALL_INVADER,    1, "small_invader2.tga" },
    { IID_FLYING_SAUCER,    0, "flying_saucer.tga" },
//    { IID_BARRIER,          0, "barrier.tga" },
    { IID_PLAYER_LASER,     0, "player_laser.tga" },
    { IID_SLOW_LASER,       0, "slow_laser.tga" },
    { IID_SLOW_LASER,       1, "slow_laser2.tga" },
    { IID_MEDIUM_LASER,     0, "medium_laser.tga" },
//    { IID_MEDIUM_LASER,     1, "medium_laser2.tga" },
    { IID_FAST_LASER,       0, "fast_laser.tga" },
    { IID_FAST_LASER,       1, "fast_laser2.tga" },
    { IID_PLAYER_KILLED,    0, "player_explosion.tga" },
    { IID_PLAYER_KILLED,    1, "player_explosion2.tga" },
    { IID_INVADER_KILLED,   0, "invaderkilled.tga" },
    { IID_BORDER,           0, "border.tga"},
  };

	SoundMapType::value_type sounds[] = {    
		make_pair(SOUND_THEME					, "theme.wav"),
    make_pair(SOUND_ALIEN_MOVE_1,   "fastinvader1.wav"),
    make_pair(SOUND_ALIEN_MOVE_2,   "fastinvader2.wav"),
    make_pair(SOUND_ALIEN_MOVE_3,   "fastinvader3.wav"),
    make_pair(SOUND_ALIEN_MOVE_4,   "fastinvader4.wav"),
    make_pair(SOUND_PLAYER_KILLED,  "explosion.wav"),
    make_pair(SOUND_ALIEN_KILLED,   "invaderkilled.wav"),
    make_pair(SOUND_LASER,          "shoot.wav"),
    make_pair(SOUND_SAUCER_MOVE_1,  "ufo_lowpitch.wav"),
    make_pair(SOUND_SAUCER_MOVE_2,  "ufo_highpitch.wav"),
	};

	for (int k = 0; k < sizeof(drawers)/sizeof(drawers[0]); k++)
	{
		string path = m_gw->asset_directory();
    if (!path.empty()) { path += '/'; }
		const SpriteInfo& d = drawers[k];
    if (!m_sprite_manager.load_sprite(path + d.tga_filename, d.image_id, d.frame_num)) { cout << "ERROR2" << endl; exit(0); }
	}
  for (int k = 0; k < sizeof(sounds)/sizeof(sounds[0]); k++) { m_sound_map[sounds[k].first] = sounds[k].second; }
}

void GameController::display_gameplay(void)
{
  glEnable(GL_DEPTH_TEST); // must be done each time before displaying graphics or gets disabled for some reason
  glLoadIdentity();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
  
  for (int i = NUM_LAYERS - 1; i >= 0; --i)
  {
    std::set<GraphObject*> &graph_objects = GraphObject::get_graph_objects(i);
    
    for (auto it = graph_objects.begin(); it != graph_objects.end(); it++)
    {
      GraphObject* cur = *it;
      if (cur->is_visible())
      {
        cur->animate();
        
        double x, y, gx, gy, gz;
        cur->get_animation_location(x, y);
        convert_to_glut_coords(x, y, gx, gy, gz);
        
        SpriteManager::Angle angle;
        switch (cur->get_direction())
        {
          case GraphObject::up:
            angle = SpriteManager::face_up;
            break;
          case GraphObject::down:
            angle = SpriteManager::face_down;
            break;
          case GraphObject::left:
            angle = SpriteManager::face_left;
            break;
          case GraphObject::right:
          case GraphObject::none:
          default:
            angle = SpriteManager::face_right;
            break;
        }
        
        int image_id = cur->get_id();
        
        m_sprite_manager.plot_sprite(image_id, cur->get_animation_number() % m_sprite_manager.get_num_frames(image_id), gx, gy, gz, angle, cur->get_size());
      }
    }
  }
  
  draw_score_and_lives(m_game_stat_text);
  
  glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////
////////////////////-----------GAME ENGINE--------------///////////////////
///////////////////////////////////////////////////////////////////////////

static void do_somethingCallback() { Game().do_something(); }

static void reshapeCallback(int w, int h) { Game().reshape(w, h); }

static void keyboard_eventCallback(unsigned char key, int x, int y) { Game().keyboard_event(key, x, y); }

static void specialkeyboard_eventCallback(int key, int x, int y) { Game().specialkeyboard_event(key, x, y); }

static void timerFuncCallback(int val) { Game().do_something(); glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0); }

void GameController::run(int argc, char* argv[], GameWorld* gw, string window_title)
{
	gw->set_controller(this);
	m_gw = gw;
	set_game_state(welcome);
	m_last_key_hit = INVALID_KEY;
	m_singleStep = false;
	m_cur_intra_frame_tick = 0;
	m_player_won = false;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(window_title.c_str());

	init_drawers_and_sounds();

	glutKeyboardFunc(keyboard_eventCallback);
	glutSpecialFunc(specialkeyboard_eventCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(do_somethingCallback);
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
	delete m_gw;
}

bool GameController::get_last_key(int& value)
{
		if (m_last_key_hit != INVALID_KEY)
    {
      value = m_last_key_hit;
      m_last_key_hit = INVALID_KEY;
      return true;
    }
		return false;
}

void GameController::keyboard_event(unsigned char key, int /* x */, int /* y */)
{
	switch (key)
	{
		case 'a': case '4': m_last_key_hit = KEY_PRESS_LEFT;	break;
		case 'd': case '6': m_last_key_hit = KEY_PRESS_RIGHT; break;
		case 'w': case '8': m_last_key_hit = KEY_PRESS_UP;    break;
		case 's': case '2': m_last_key_hit = KEY_PRESS_DOWN;	break;
		case 't':           m_last_key_hit = KEY_PRESS_TAB;   break;
		case 'f':           m_singleStep = true;              break;
		case 'r':           m_singleStep = false;             break;
		case 'q': case 'Q': set_game_state(quit);             break;
		default:            m_last_key_hit = key;             break;
	}
}

void GameController::specialkeyboard_event(int key, int /* x */, int /* y */)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:	 m_last_key_hit = KEY_PRESS_LEFT;	  break;
		case GLUT_KEY_RIGHT: m_last_key_hit = KEY_PRESS_RIGHT;  break;
		case GLUT_KEY_UP:    m_last_key_hit = KEY_PRESS_UP;     break;
		case GLUT_KEY_DOWN:	 m_last_key_hit = KEY_PRESS_DOWN;	  break;
		default:             m_last_key_hit = INVALID_KEY;      break;
	}
}

void GameController::play_sound(int sound_id)
{
  if (sound_id == SOUND_NONE) { return; }

	SoundMapType::const_iterator p = m_sound_map.find(sound_id);
	if (p != m_sound_map.end())
	{
		string path = m_gw->asset_directory();
    if (!path.empty()) { path += '/'; }
		SoundFX().play_clip(path + p->second);
	}
}

void GameController::do_something()
{
	switch (m_game_state)
	{
		case not_applicable:
			break;
		case welcome:
			//play_sound(SOUND_THEME);
			m_main_message = "Space Invaders";
			m_second_message = "Press Enter to begin play...";
			set_game_state(prompt);
			m_next_state_after_prompt = init;
			break;
		case contgame:
			m_main_message = "You lost a life!";
			m_second_message = "Press Enter to continue playing...";
			set_game_state(prompt);
			m_next_state_after_prompt = clean_up;
			break;
		case finishedlevel:
			m_main_message = "Woot! You finished the level!";
			m_second_message = "Press Enter to continue playing...";
			set_game_state(prompt);
			m_next_state_after_prompt = clean_up;
			break;
		case makemove:
			m_cur_intra_frame_tick = ANIMATION_POSITIONS_PER_TICK;
			m_next_state_after_animate = not_applicable;
			{
				int status = m_gw->move();
				if (status == GWSTATUS_PLAYER_DIED)
				{
					  // animate one last frame so the player can see what happened
					m_next_state_after_animate = (m_gw->is_game_over() ? gameover : contgame);
				}
				else if (status == GWSTATUS_FINISHED_LEVEL)
				{
					m_gw->advance_to_next_level();
					  // animate one last frame so the player can see what happened
					m_next_state_after_animate = finishedlevel;
				}
			}
			set_game_state(animate);
			break;
		case animate:
			display_gameplay();
			if (m_cur_intra_frame_tick-- <= 0)
			{
				if (m_next_state_after_animate != not_applicable)
					set_game_state(m_next_state_after_animate);
				else
				{
					int key;
					if (!m_singleStep  ||  get_last_key(key))
						set_game_state(makemove);
				}
			}
			break;
		case clean_up:
			m_gw->clean_up();
			set_game_state(init);
			break;
		case gameover:
			{
				ostringstream oss;
				oss << (m_player_won ? "You won the game!" : "Game Over!")
					<< " Final score: " << m_gw->get_score() << "!";
				m_main_message = oss.str();
			}
			m_second_message = "Press Enter to quit...";
			set_game_state(prompt);
			m_next_state_after_prompt = quit;
			break;
		case prompt:
			draw_prompt(m_main_message, m_second_message);
			{
				int key;
				if (get_last_key(key) && key == '\r')
					set_game_state(m_next_state_after_prompt);
			}
			break;
		case init:
			{
				int status = m_gw->init();
				SoundFX().abort_clip();
				if (status == GWSTATUS_PLAYER_WON)
				{
					m_player_won = true;
					set_game_state(gameover);
				}
				else if (status == GWSTATUS_LEVEL_ERROR)
				{
					m_main_message = "Error in level data file encoding!";
					m_second_message = "Press Enter to quit...";
					set_game_state(prompt);
					m_next_state_after_prompt = quit;
				}
				else
					set_game_state(makemove);
			}
			break;
		case quit:
			glutLeaveMainLoop();
			break;
	}
}

void GameController::reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45.0, double(WINDOW_WIDTH) / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	glMatrixMode (GL_MODELVIEW);
}

static void convert_to_glut_coords(double x, double y, double& gx, double& gy, double& gz)
{
	x /= VIEW_WIDTH;
	y /= VIEW_HEIGHT;
	gx = 2 * VISIBLE_MIN_X + .3 + x * 2 * (VISIBLE_MAX_X - VISIBLE_MIN_X);
	gy = 2 * VISIBLE_MIN_Y +	  y * 2 * (VISIBLE_MAX_Y - VISIBLE_MIN_Y);
	gz = .6 * VISIBLE_MIN_Z;
}

static void do_output_stroke(double x, double y, double z, double size, const char* str, bool centered)
{
	if (centered)
	{
		double len = glutStrokeLength(GLUT_STROKE_ROMAN, reinterpret_cast<const unsigned char*>(str)) / FONT_SCALEDOWN;
		x = -len / 2;
		size = 1;
	}
	GLfloat scaled_size = static_cast<GLfloat>(size / FONT_SCALEDOWN);
	glPushMatrix();
	glLineWidth(1);
	glLoadIdentity();
	glTranslatef(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));
	glScalef(scaled_size, scaled_size, scaled_size);
  for ( ; *str != '\0'; str++) { glutStrokeCharacter(GLUT_STROKE_ROMAN, *str); }
	glPopMatrix();
}

//static void outputStroke(double x, double y, double z, double size, const char* str)
//{
//	do_output_stroke(x, y, z, size, str, false);
//}

static void output_stroke_centered(double y, double z, const char* str) { do_output_stroke(0, y, z, 1, str, true); }

static void draw_prompt(string main_message, string second_message)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	output_stroke_centered(1, -5, main_message.c_str());
	output_stroke_centered(-1, -5, second_message.c_str());
	glutSwapBuffers();
}

static void draw_score_and_lives(string game_stat_text)
{
	static int RATE = 1;
	static GLfloat rgb[3] =
		{ static_cast<GLfloat>(.6), static_cast<GLfloat>(.6), static_cast<GLfloat>(.6) };
	for (int k = 0; k < 3; k++)
	{
		double strength = rgb[k] + (-RATE + rand() % (2*RATE+1)) / 100.0;
		if (strength < .6)
			strength = .6;
		else if (strength > 1.0)
			strength = 1.0;
		rgb[k] = static_cast<GLfloat>(strength);
	}
	glColor3f(rgb[0], rgb[1], rgb[2]);
	output_stroke_centered(SCORE_Y, SCORE_Z, game_stat_text.c_str());
}
