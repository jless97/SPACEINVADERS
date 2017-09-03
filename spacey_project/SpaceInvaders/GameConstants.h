#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

///////////////////////////////////////////////////////////////////////////
///////////////////////-----------IMAGES--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

const int IID_WATER_SPURT	   = 15;
const int IID_BOULDER		   = 16;        // (Large Invader)
const int IID_BARREL		   = 17;        // (Medium Invader)
const int IID_DIRT			   = 18;
const int IID_SONAR			   = 20;
const int IID_WATER_POOL	   = 21;

const int IID_SPACESHIP        = 0;
const int IID_LARGE_INVADER    = 1;
const int IID_MEDIUM_INVADER   = 2;
const int IID_SMALL_INVADER    = 3;
const int IID_FLYING_SAUCER    = 4;
const int IID_BARRIER          = 5;
const int IID_PLAYER_LASER     = 6;
const int IID_SLOW_LASER       = 7;
const int IID_MEDIUM_LASER     = 8;
const int IID_FAST_LASER       = 9;
const int IID_PLAYER_KILLED    = 10;
const int IID_INVADER_KILLED   = 11;

///////////////////////////////////////////////////////////////////////////
//////////////////////----------- SOUNDS--------------/////////////////////
///////////////////////////////////////////////////////////////////////////

const int SOUND_THEME					= 9;
const int SOUND_DIG						= 15;
const int SOUND_FINISHED_LEVEL			= 16;

const int SOUND_ALIEN_MOVE_1            = 0;
const int SOUND_ALIEN_MOVE_2            = 1;
const int SOUND_ALIEN_MOVE_3            = 2;
const int SOUND_ALIEN_MOVE_4            = 3;
const int SOUND_PLAYER_KILLED           = 4;
const int SOUND_ALIEN_KILLED            = 5;
const int SOUND_LASER                   = 6;
const int SOUND_SAUCER_MOVE_1           = 7;
const int SOUND_SAUCER_MOVE_2           = 8;

const int SOUND_NONE				   = -1;

///////////////////////////////////////////////////////////////////////////
///////////////////----------- PLAYER INPUT--------------//////////////////
///////////////////////////////////////////////////////////////////////////

const int KEY_PRESS_LEFT  = 1000;
const int KEY_PRESS_RIGHT = 1001;
const int KEY_PRESS_UP	  = 1002;
const int KEY_PRESS_DOWN  = 1003;
const int KEY_PRESS_SPACE = ' ';
const int KEY_PRESS_ESCAPE = '\x1b';
const int KEY_PRESS_TAB	  = '\t';
const int KEY_PRESS_Z     = 'Z';
const int KEY_PRESS_z     = 'z';

///////////////////////////////////////////////////////////////////////////
///////////----------- BOARD AND SPRITE DIMENSIONS--------------///////////
///////////////////////////////////////////////////////////////////////////
const int VIEW_WIDTH = 64; //256;
const int VIEW_HEIGHT = 64;	 //256;

const int SPRITE_WIDTH = VIEW_WIDTH/16;
const int SPRITE_HEIGHT = VIEW_HEIGHT/16;

const double SPRITE_WIDTH_GL = .5; // note - this is tied implicitly to SPRITE_WIDTH due to carey's sloppy openGL programming
const double SPRITE_HEIGHT_GL = .5; // note - this is tied implicitly to SPRITE_HEIGHT due to carey's sloppy openGL programming

///////////////////////////////////////////////////////////////////////////
///////////////----------- STATUS OF EACH TICK--------------///////////////
///////////////////////////////////////////////////////////////////////////

const int GWSTATUS_PLAYER_DIED	 = 0;
const int GWSTATUS_CONTINUE_GAME = 1;
const int GWSTATUS_PLAYER_WON	 = 2;
const int GWSTATUS_FINISHED_LEVEL= 3;
const int GWSTATUS_LEVEL_ERROR	 = 4;

#endif // GAMECONSTANTS_H_
