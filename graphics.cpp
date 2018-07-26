#include "WIZ/graphics.h"

#include <SOIL.h>

#include <cstdio>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <cmath>

Color Color::BLACK = Color(0, 0, 0, 1);
Color Color::WHITE = Color(1, 1, 1, 1);
Color Color::RED = Color(1, 0, 0, 1);
Color Color::GREEN = Color(0, 1, 0, 1);
Color Color::BLUE =Color(0, 0, 1, 1);
Color Color::YELLOW = Color(1, 1, 0, 1);
Color Color::CYAN = Color(0, 1, 1, 1);
Color Color::ORANGE = Color(1, 0.6f, 0, 1);
Color Color::PURPLE = Color(0.6f, 0, 1, 1);
Color Color::GOLD = Color(0.6f, 0.3f, 1, 1);
Color Color::TRANSPARENCY = Color(0, 0, 0, 0);
Color Color::TEAL = Color(0, 0.6f, 0.4f, 1);
Color Color::GRAY = Color(0.4f, 0.4f, 0.4f, 1);
Color Color::LIGHT_GRAY = Color(0.85f, 0.85f, 0.85f, 1);
Color Color::DARK_GRAY = Color(0.1f, 0.1f, 0.1f, 1);


Blend *Blend::ALPHA = new Blend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, true);

Blend *Blend::ADDITIVE = new Blend(GL_ONE, GL_ONE, true);

Blend *Blend::ALPHA_DISABLED = new Blend(GL_ONE, GL_ONE, false);




Shader::Shader(const char* vsource, const char* fsource) {
	// this->errorLog = nullptr;
	// this->vshaderId = -1;
	// this->fshaderId = -1;
	this->vshaderId = createShaderObject(vsource, GL_VERTEX_SHADER);
	this->fshaderId = createShaderObject(fsource, GL_FRAGMENT_SHADER);
	
	if (this->vshaderId < 1 || this->fshaderId < 1) {
		//LOGI("shader errors sssssssssssssssssss1: %d", 2);
		return;
	}
	
	
	this->programId = createShaderProgram();
	
	if (this->programId > 0 && linkShaderProgram() > 0) {
		
	}
}

Shader::~Shader() {
	glDeleteShader(this->vshaderId);
	glDeleteShader(this->fshaderId);
	glDeleteProgram(this->programId);
}


int Shader::createShaderObject(const char* source, GLenum type) {
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);
	
	int compiled = -1;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	
	if (compiled == 0) {
		GLint lenght = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		//int lenght1 = lenght;
		errorLog = new char[lenght];
		
		glGetShaderInfoLog(id, 100, &lenght, errorLog);
		return -1;
	}
	return id;
}

int Shader::createShaderProgram() {
	GLuint id = glCreateProgram();
	glAttachShader(id, this->vshaderId);
	glAttachShader(id, this->fshaderId);
	return id;
}
	
int Shader::linkShaderProgram() {
	glLinkProgram(this->programId);
	GLint status = 0;
	glGetProgramiv(this->programId, GL_LINK_STATUS, &status);
	
	if (status == 0) {
		
		GLint lenght = 0;
		glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &lenght);
		errorLog = new char[lenght];
		
		glGetProgramInfoLog(this->programId, 50, &lenght, errorLog);
		//LOGI("shader on link errors: %s", errorLog);
		return -1;
		
	}
	return 1;
}

void Shader::begin() {
	glUseProgram(programId);
}

void Shader::end() {
	glUseProgram(0);
}

int Shader::getShaderProgram() {
	return this->programId;
};

int Shader::getFragmentShaderObject() {
	return this->fshaderId;
};
		
int Shader::getVertexShaderObject() {
	return this->vshaderId;
};
		
bool Shader::hasErrors() {
	return this->errorLog != nullptr;
};

char *Shader::getErrorLog() {
	return this->errorLog;
}






/*
	
		T E X T U R E ::::::::::::::::::::::::::::::::::::::::::::


*/

Texture::Texture(int id, int width, int height) {
	textureObject = id;
	
	if (textureObject == 0) {
		//GLuint t = 0;
		////glGenTextures(GL_TEXTURE_2D, &t);
		//glGenBuffers(1, &t);
		//textureObject = t;
		this->textureObject = SOIL_create_OGL_texture(
			0, 
			0, 
			0, 
			SOIL_LOAD_RGBA, 
			SOIL_CREATE_NEW_ID,
			0);
		
		//printf("texture id %d\n", textureObject);
		
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureObject);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	this->width = width;
	this->height = height;
}

Texture::Texture(const char *path) {
	
	this->width = 0;
	this->height = 0;
	this->textureObject = 0;
	
	int channels = 0;
	
	unsigned char* buffer = SOIL_load_image(
		path,
		&this->width,
		&this->height,
		&channels, 
		SOIL_LOAD_AUTO);
	
	if (buffer == NULL) {
		throw std::runtime_error("WIZ: TEXTURE the texture could't be read: " + std::string(path));
	}
	
	
	
	this->textureObject = SOIL_create_OGL_texture(
		buffer, 
		this->width, 
		this->height, 
		channels, 
		SOIL_CREATE_NEW_ID, 
		0);
	//glBindTexture(GL_TEXTURE_2D, textureObject);
	//glBindTexture(GL_TEXTURE_2D, 0);
	free(buffer);

}


// dispose

Texture::~Texture() {
	const GLuint textures = this->textureObject;
	glDeleteTextures(1, &textures);
}

	
int Texture::getTextureObject() {
	return this->textureObject;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, this->textureObject);
}

void Texture::bindTo(int t) {
	glActiveTexture(GL_TEXTURE0 + t);
	this->bind();
	glActiveTexture(GL_TEXTURE0);
}

int Texture::getWidth() {
	return this->width;
}

int Texture::getHeight() {
	return this->height;
}





/*

	S P R I T E    B A T C H

*/


SpriteBatch::SpriteBatch(int maxSprites) {
	
	// this->indexBufferObject = -1;
	
	const char* vshader = "uniform mat4 uPVM;\nattribute vec2 vPosition;\nattribute vec2 vUv; \nattribute vec4 vColor;\nvarying vec2 uv;\nvarying vec4 color;\nvoid main() {\ncolor=vColor;\nuv=vUv;\ngl_Position= uPVM * vec4(vPosition, 1, 1);\n}";
	const char* fshader = "#ifdef GL_ES \
						  \n#define LOWP lowp \
						  \nprecision mediump float;  \
						  \n#else  \
						  \n#define LOWP  \
						  \n#endif \
						\nvarying vec2 uv;\nvarying vec4 color;\nuniform sampler2D utexture0;\nvoid main() {\n gl_FragColor=texture2D(utexture0, uv) * color; \n}";
	
	this->shader = new Shader(vshader, fshader);
	this->originalShader = shader;
	if (this->shader->hasErrors())
		printf("batch shader errors: %s", shader->getErrorLog());
	
	// vertexCount = 0;
	// textureObject = -1;
	// v2 uv2 color4
	this->maxSprites = maxSprites;
	this->vertexSize = 2 + 2 + 4;
	this->vertex = new float[this->maxSprites * 4 * this->vertexSize];
	
	GLshort index[this->maxSprites * 6];
	for (int i = 0, j = 0; i < maxSprites * 6; i += 6, j += 4) {
		index[i] = j;
		index[i+1] = (GLshort)(j + 1);
		index[i+2] = (GLshort)(j + 2);
		index[i+3] = (GLshort)(j + 2);
		index[i+4] = (GLshort)(j + 3);
		index[i+5] = (GLshort)(j );
	}
	
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), &index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
	// 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	this->projectionData = new GLfloat[16];
	
	// font = 0;
	// Texture *t = new Texture(0, 1, 1);
	
	// glBindTexture(GL_TEXTURE_2D, t->getTextureObject());
	// //unsigned char c = 255 << 24 | 255 << 16 | 255 << 8 | 255 << 0;
	
	// char colors[]{ (char)(255), (char)(255), (char)255,(char)(255) };
	// //glTexSubImage2D(GL_TEXTURE_2D, 0,  0, 0,   1, 1,  GL_RGBA, GL_FLOAT, colors);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glBindTexture(GL_TEXTURE_2D, 0);
	// this->ownPixel = new TexturePart(t, 0, 0, 1, 1);
	// this->pixel = ownPixel;
}

SpriteBatch::~SpriteBatch() {
	delete this->originalShader;
	// delete ownPixel->getTexture();
	// delete ownPixel;
	glDeleteBuffers(1, &this->indexBufferObject);
}

// void SpriteBatch::setFont(BitmapFont *font) {
// 	this->font = font;
// }

// BitmapFont *SpriteBatch::getFont() {
// 	return font;
// }

void SpriteBatch::setBlend(Blend *blend) {
	this->blend = blend;
}

Blend *SpriteBatch::getBlend() {
	return blend;
}

Color SpriteBatch::getColor() {
	return color;
}

void SpriteBatch::setColor(Color &color) {
	setColor(color.r, color.g, color.b, color.a);
}

void SpriteBatch::setColor(float r, float g, float b, float a) {
	this->color.a = a;
	this->color.r = r;
	this->color.g = g;
	this->color.b = b;
}


Matrix* SpriteBatch::getTransformMatrix() {
	return &transformMatrix;
}

Matrix* SpriteBatch::getProjectionMatrix() {
	return &projectionMatrix;
}

void SpriteBatch::setProjectionMatrix(Matrix &prj) {
	if (drawing) flush();
	this->projectionMatrix.set(prj);
	projectionMatrix.transform(transformMatrix, pvMatrix);
}

void SpriteBatch::setTransformMatrix(Matrix &trans) {
	if (drawing) flush();
	transformMatrix.set(trans);
	projectionMatrix.transform(transformMatrix, pvMatrix);
}

void SpriteBatch::setShader(Shader *shader) {
	if (drawing) {
		flush();
		shader->begin();
	}
	this->shader = shader;
}

Shader *SpriteBatch::getShader() {
	return shader;
}

void SpriteBatch::begin(Blend *blend) {
	begin(blend, originalShader);
}

void SpriteBatch::begin(Blend *blend, Shader *shader) {
	shader->begin();
	if (blend->alphaEnabled) {
		glEnable(GL_BLEND);
		glBlendFunc(blend->source, blend->dest);
	} else 
		glDisable(GL_BLEND);
	this->shader = shader;
	drawing = true;
}

void SpriteBatch::begin() {
	begin(Blend::ALPHA, originalShader);
}

void SpriteBatch::end() {
	flush();
	glUseProgram(0);
	drawing = false;
	textureObject = -1;// quitar!
}

void SpriteBatch::draw(Texture &texture, float x, float y, float w, float h) {
	draw(texture, x, y,
		0, 0, texture.getWidth(), texture.getHeight(),
		0, 0,
		0,
		w/texture.getWidth(), h/texture.getHeight(),
		false, false,
		color
	);
}

void SpriteBatch::draw(
	Texture &texture, 
	float x, float y, 
	float rx, float ry, float rw, float rh, 
	float originx, float originy,
	float rotation,
	float scalex, float scaley, 
	bool flipx, bool flipy, Color &color) {
	
	int used = this->vertexCount / (this->vertexSize * 4);
	bool otherTexture = this->textureObject != texture.getTextureObject();
	if (used + 1 > this->maxSprites || otherTexture)
		flush();
	if (otherTexture) {
		texture.bind();
		this->textureObject = texture.getTextureObject();
	}
	
	int textureWidth = texture.getWidth();
	int textureHeight = texture.getHeight();
	
	float hwidth = rw * 0.5f * scalex;
	float hheight = rh * 0.5f * scaley;
	
	rx /= textureWidth;
	ry = 1.0f - (ry + rh) / textureHeight;
	rw = rx + rw / textureWidth;
	rh = ry + rh / textureHeight; 

	float x1 = -hwidth;
	float y1 = -hheight;
	float x2 = x1;
	float y2 = hheight;
	float x3 = hwidth;
	float y3 = y2;
	float x4 = x3;
	float y4 = y1;
	
	originx = x1 + originx * scalex;
	originy = y1 + originy * scaley;

	if (rotation != 0) {
		
		rotation = rotation * 3.1415999999f / 180;
		double cos = std::cos(rotation);
		double sin = std::sin(rotation);
		float temp = x4;
		x4 = (float)(cos * x4 - sin * y4);
		y4 = (float)(sin * temp + cos * y4);
		temp = x3;
		x3 = (float)(cos * x3 - sin * y3);
		y3 = (float)(sin * temp + cos * y3);
		temp = originx;
		originx = (float)(cos * originx - sin * originy);
		originy = (float)(sin * temp + cos * originy);
		// 90°
		// nv = -vy, vx
		// ->  v(300,0) => nv(-0,300)
		// 180° => nv(-300, -0)
		x2 = -x4;
		y2 = -y4;
		x1 = -x3;
		y1 = -y3;
	}

	//if (flip != Flip.NONE) {
		float temp = rx;
		//if (flip == Flip.HORIZONTAL || flip == Flip.HORIZONTAL_AND_VERTICAL) {
		if (flipx) {
			rx = rw;
			rw = temp;
		} 
		//if (flip == Flip.VERTICAL || flip == Flip.HORIZONTAL_AND_VERTICAL) {
		if (flipy) {
			temp = ry;
			ry = rh;
			rh = temp;
		}
	//}
	
	

	this->vertex[this->vertexCount++] = x + x1 - originx; // 0          0 + 1
	this->vertex[this->vertexCount++] = y + y1 - originy; // 1			1 + 1

	this->vertex[this->vertexCount++] = rx;
	this->vertex[this->vertexCount++] = rh; // bottom lef

	this->vertex[this->vertexCount++] = color.r;
	this->vertex[this->vertexCount++] = color.g;
	this->vertex[this->vertexCount++] = color.b;
	this->vertex[this->vertexCount++] = color.a;

	this->vertex[this->vertexCount++] = x + x2 - originx;
	this->vertex[this->vertexCount++] = y + y2 - originy;

	this->vertex[this->vertexCount++] = rx;
	this->vertex[this->vertexCount++] = ry; // top lef

	this->vertex[this->vertexCount++] = color.r;
	this->vertex[this->vertexCount++] = color.g;
	this->vertex[this->vertexCount++] = color.b;
	this->vertex[this->vertexCount++] = color.a;

	this->vertex[this->vertexCount++] = x + x3 - originx;
	this->vertex[this->vertexCount++] = y + y3 - originy;

	this->vertex[this->vertexCount++] = rw;
	this->vertex[this->vertexCount++] = ry; // top lef

	this->vertex[this->vertexCount++] = color.r;
	this->vertex[this->vertexCount++] = color.g;
	this->vertex[this->vertexCount++] = color.b;
	this->vertex[this->vertexCount++] = color.a;

	this->vertex[this->vertexCount++] = x + x4 - originx;
	this->vertex[this->vertexCount++] = y + y4 - originy;

	this->vertex[this->vertexCount++] = rw;
	this->vertex[this->vertexCount++] = rh; // botom lef

	this->vertex[this->vertexCount++] = color.r;
	this->vertex[this->vertexCount++] = color.g;
	this->vertex[this->vertexCount++] = color.b;
	this->vertex[this->vertexCount++] = color.a;
	
	
}

void SpriteBatch::flush() {
	
	// when just change texture
	if (this->vertexCount < 1) return;
	
	int program = this->shader->getShaderProgram();
	int posLocation = glGetAttribLocation(program, "vPosition");
	int uvLocation = glGetAttribLocation(program, "vUv");
	int colorLocation = glGetAttribLocation(program, "vColor");
	
	int sampleLocation = glGetUniformLocation(program, "utexture0");
	int pvmLocation = glGetUniformLocation(program, "uPVM");

	//glEnableVertexAttribArray(
	this->pvMatrix.toGLArray(projectionData);
	glUniformMatrix4fv(pvmLocation, 1, false, projectionData);
	
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(uvLocation);
	glEnableVertexAttribArray(colorLocation);
	
	int stride = sizeof(GL_FLOAT) * this->vertexSize;
	
	glVertexAttribPointer(posLocation, 2, GL_FLOAT, false, stride, this->vertex);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, false, stride, this->vertex + 2);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, false, stride, this->vertex + 4);
	
	glUniform1i(sampleLocation, 0);
	
	// matrix
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
	glDrawElements(GL_TRIANGLES, this->vertexCount / 4 / this->vertexSize * 6, GL_UNSIGNED_SHORT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	this->vertexCount = 0;
}
