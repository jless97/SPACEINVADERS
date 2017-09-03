#ifndef SPRITEMANAGER_H_
#define SPRITEMANAGER_H_

#include "freeglut.h"

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

#include "GameConstants.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class SpriteManager
{
public:
	SpriteManager() : m_mip_mapped(true) {}
	void set_mip_mapping(bool status)	{ m_mip_mapped = status; }
	bool load_sprite(std::string filename_tga, int image_id, int frame_num)
	{
    // Load Texture Data From TGA File
		unsigned int sprite_id = getsprite_id(image_id, frame_num);
    if (INVALID_SPRITE_ID == sprite_id) { return false; }

		m_frame_count_per_sprite[image_id]++;	// keep track of how many frames per sprite we loaded

		std::string line;
		std::string contents = "";
		std::ifstream tga_file(filename_tga, std::ios::in|std::ios::binary);
    
    if (!tga_file) { return false; }

		char type[3];
		char info[6];
		unsigned char byte_count;
		unsigned int texture_width;
		unsigned int texture_height;
		long image_size;
		char* image_data = nullptr;

    // Read file header info
		tga_file.read(type, 3);
		tga_file.seekg(12);
		tga_file.read(info, 6);
		texture_width = static_cast<unsigned char>(info[0]) + static_cast<unsigned char>(info[1]) * 256;
		texture_height = static_cast<unsigned char>(info[2]) + static_cast<unsigned char>(info[3]) * 256;
		byte_count = static_cast<unsigned char>(info[4]) / 8;
		image_size = texture_width * texture_height * byte_count;
		image_data = new char[image_size];
		tga_file.seekg(18);

    // Read image data
		tga_file.read(image_data, image_size);
		if (!tga_file) { delete [] image_data; return false; }

    //image type either 2 (color) or 3 (greyscale)
    //if (type[1] != 0 || (type[2] != 2 && type[2] != 3)) { return false; }

    //if (byte_count != 3 && byte_count != 4) { return false; }
    
    // Transfer Texture To OpenGL
		glEnable(GL_DEPTH_TEST);

    // allocate a texture handle
		GLuint gl_texture_id;
		glGenTextures(1, &gl_texture_id);

    // bind our new texture
		glBindTexture(GL_TEXTURE_2D, gl_texture_id);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		if (m_mip_mapped)
		{
      // when texture area is small, bilinear filter the closest mipmap
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      // when texture area is large, bilinear filter the first mipmap
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

    // Have the texture wrap both vertically and horizontally.
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLfloat>(GL_REPEAT));
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLfloat>(GL_REPEAT));

		if (m_mip_mapped)
		{
      // build our texture mipmaps
      // byte_count of 3 means that BGR data is being supplied. byte_count of 4 means that BGRA data is being supplied.
      if (3 == byte_count) { gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture_width, texture_height, GL_BGR, GL_UNSIGNED_BYTE, image_data); }
      else if (4 == byte_count) { gluBuild2DMipmaps(GL_TEXTURE_2D, 4, texture_width, texture_height, GL_BGRA, GL_UNSIGNED_BYTE, image_data); }
		}
		else
		{
      // byte_count of 3 means that BGR data is being supplied. byte_count of 4 means that BGRA data is being supplied.
      if (3 == byte_count) { glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_BGR, GL_UNSIGNED_BYTE, image_data); }
			else if (4 == byte_count) { glTexImage2D(GL_TEXTURE_2D, 0, 4, texture_width, texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, image_data); }
		}

    delete [] image_data;
		m_image_map[sprite_id] = gl_texture_id;
		return true;
	}

	unsigned int get_num_frames(int image_id) const
	{
		auto it = m_frame_count_per_sprite.find(image_id);
    if (it == m_frame_count_per_sprite.end()) { return 0; }

		return it->second;
	}

	enum Angle
  {
		degrees_0 = 0, degrees_90 = 90, degrees_180 = 180, degrees_270 = 270,
		face_left = 1, face_right = 2, face_up = 3, face_down = 4
	};

	bool plot_sprite(int image_id, int frame, double gx, double gy, double gz, Angle angle_degrees, double size)
	{
		unsigned int sprite_id = getsprite_id(image_id,frame);
    if (INVALID_SPRITE_ID == sprite_id) { std::cout << "INVALID" << std::endl; return false; }

		auto it = m_image_map.find(sprite_id);
		if (it == m_image_map.end()) { std::cout << "NICE TRY" << std::endl; return false; }

		glPushMatrix();

		double final_width = SPRITE_WIDTH_GL * size, final_height = SPRITE_HEIGHT_GL * size;

		// object's x/y location is center-based, but sprite plotting is upper-left-corner based
		const double xoffset = final_width / 2;
		const double yoffset = final_height / 2;

		glTranslatef(static_cast<GLfloat>(gx-xoffset),static_cast<GLfloat>(gy-yoffset),static_cast<GLfloat>(gz));
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable (GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, it->second);

		glColor3f(1.0, 1.0, 1.0);

		double cx1,cx2,cx3,cx4;
		double cy1,cy2,cy3,cy4;

		switch (angle_degrees)
		{
      default:
      case degrees_0:
      case face_right:
        cx1 = 0; cy1 = 0;
        cx2 = 1; cy2 = 0;
        cx3 = 1; cy3 = 1;
        cx4 = 0; cy4 = 1;
        break;
      case face_left:
        cx1 = 1; cy1 = 0;
        cx2 = 0; cy2 = 0;
        cx3 = 0; cy3 = 1;
        cx4 = 1; cy4 = 1;
        break;
      case degrees_90:
      case face_up:
        cx1 = 0; cy1 = 1;
        cx2 = 0; cy2 = 0;
        cx3 = 1; cy3 = 0;
        cx4 = 1; cy4 = 1;
        break;
      case degrees_180:
        cx1 = 1; cy1 = 1;
        cx2 = 0; cy2 = 1;
        cx3 = 0; cy3 = 0;
        cx4 = 1; cy4 = 0;
        break;
      case degrees_270:
      case face_down:
        cx1 = 1; cy1 = 0;
        cx2 = 1; cy2 = 1;
        cx3 = 0; cy3 = 1;
        cx4 = 0; cy4 = 0;
        break;
		}

		glBegin (GL_QUADS);
		  glTexCoord2d(cx1, cy1);
		  glVertex3f(0, 0, 0);
		  glTexCoord2d(cx2, cy2);
		  glVertex3f(static_cast<GLfloat>(final_width), 0, 0);
		  glTexCoord2d(cx3, cy3);
		  glVertex3f(static_cast<GLfloat>(final_width), static_cast<GLfloat>(final_height), 0);
		  glTexCoord2d(cx4, cy4);
		  glVertex3f(0, static_cast<GLfloat>(final_height), 0);
		  glEnd();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);

		glPopAttrib();
		glPopMatrix();

		return true;
  }
  
	~SpriteManager()
	{
    for (auto it = m_image_map.begin(); it != m_image_map.end(); it++) { glDeleteTextures(1, &it->second); }
	}

private:
	bool                                    m_mip_mapped;
	std::map<unsigned int, GLuint>          m_image_map;
	std::map<unsigned int, unsigned int>		m_frame_count_per_sprite;

	static const int INVALID_SPRITE_ID = -1;
	static const int MAX_IMAGES = 1000;
	static const int MAX_FRAMES_PER_SPRITE = 100;

	int getsprite_id(unsigned int image_id, unsigned int frame) const
	{
    if (image_id >= MAX_IMAGES || frame >= MAX_FRAMES_PER_SPRITE) { std::cout << "INVALID SPRITE" << std::endl; return INVALID_SPRITE_ID; }
		return image_id * MAX_FRAMES_PER_SPRITE + frame;
	}
};

#endif // SPRITEMANAGER_H_
