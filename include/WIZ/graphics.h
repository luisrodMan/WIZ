#include <GL/glew.h>

#include "WIZ/math.h"

#ifndef __WIZ_GRAPHICS__
#define __WIZ_GRAPHICS__

class Color {
	
	public:
		float r;
		float g;
		float b;
		float a;
		
		static Color WHITE;
		static Color BLACK;
		static Color RED;
		static Color GREEN;
		static Color BLUE;
		static Color YELLOW;
		static Color CYAN;
		static Color ORANGE;
		static Color PURPLE;
		static Color GOLD;
		static Color TRANSPARENCY;
		static Color TEAL;
		static Color GRAY;
		static Color LIGHT_GRAY;
		static Color DARK_GRAY;
	
	Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	
	void set(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	
	void set(Color &c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}
	
};

class Blend {
	
	public:
		
		static Blend *ALPHA;
		static Blend *ADDITIVE;
		static Blend *ALPHA_DISABLED;
	
		int source;
		int dest;
		
		bool alphaEnabled;
		
		Blend(int source, int dest, bool alphaEnabled) {
			this->source = source;
			this->dest = dest;
			this->alphaEnabled = alphaEnabled;
		}	
};


class Shader {

private:

	GLuint programId = -1;
	GLuint vshaderId = -1;
	GLuint fshaderId = -1;
	char *errorLog = nullptr;

	int createShaderObject(const char* source, GLenum type);

	int createShaderProgram();
	
	int linkShaderProgram();

public:

	Shader(const char* vsource, const char* fsource);

	~Shader();

	void begin();

	void end();

	int getShaderProgram();

	int getVertexShaderObject();

	int getFragmentShaderObject();

	bool hasErrors();

	char *getErrorLog();

};






//   T E X T U R E

class Texture {
	protected:
		int width;
		int height;
		int textureObject;
	
	public:
	
		Texture(const char *path);
		
		Texture(int id, int width, int height);
		
		~Texture();
	
		int getTextureObject();
	
		void bind();
		
		void bindTo(int t);
		
		int getWidth();
		
		int getHeight();
	
};




// S P R I T E   B A T C H


class SpriteBatch {

private:
	GLuint indexBufferObject = 0;
	float *vertex = nullptr;

	Matrix projectionMatrix;
	Matrix transformMatrix;
	Matrix pvMatrix;
	
	Shader *shader;
	Shader *originalShader;
	int vertexCount = 0;
	int maxSprites = 100;
	int vertexSize;
	int textureObject = -1;
	
	Color color=Color(1, 1, 1, 1);
	
	Blend *blend;
	
	GLfloat *projectionData;
	
	bool drawing = false;
	
	// draw shapes
	// TexturePart *pixel;
	// TexturePart *ownPixel;
	
	float lineWidth = 1;
	
	
	//BitmapFont *font;

public:

		SpriteBatch(int maxSprites=100);
		
		~SpriteBatch();
		
		
		void setBlend(Blend *blend);
		
		Blend *getBlend();
		
		// void setFont(BitmapFont *font);
		
		// BitmapFont *getFont();

		void setColor(Color &color);
		
		void setColor(float r, float g, float b, float a);

		Color getColor();

		void setProjectionMatrix(Matrix &prj);
		
		void setTransformMatrix(Matrix &trans);

		Matrix* getProjectionMatrix();
		
		Matrix *getTransformMatrix();

		void setShader(Shader *shader);
		
		Shader *getShader();

		void begin(Blend *blend);
		
		void begin(Blend *blend, Shader *shader);
		
		void begin();
		
		void end();
		
		void flush();

		void draw(Texture &texture, float x, float y, float w, float h);
		
		void draw(
			Texture &texture, 
			float x, float y, 
			float rx, float ry, float rw, float rh, 
			float originx, float originy,
			float rotation,
			float scalex, 
			float scaley, 
			bool flipx, bool flipy, 
			Color &color);
};



#endif