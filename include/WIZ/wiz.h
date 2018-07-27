#ifndef __WIZ_WIZ__
#define __WIZ_WIZ__

#include <map>
#include <vector>
#include <string>

#include "WIZ/graphics.h"

class IApplication {

public:

	virtual ~IApplication(){};

	virtual void initialize() = 0;

	virtual void renderer(float delta) = 0;

	virtual void sizeChange(int width, int heigth) {};

};

class DefaultApplication : public IApplication {

public:
	void renderer(float delta);
	void update(float delta);
	void draw();

};

class IApplicationRunner {

protected:
	IApplication *application;

public:

	IApplicationRunner(IApplication *application) {this->application = application; };

	virtual ~IApplicationRunner(){};

	virtual void run() = 0;

};

class IGraphics {

public:

	virtual ~IGraphics(){};

	virtual int getScreenWidth() = 0;

	virtual int getScreenHeight() = 0;
	
	virtual int getGameWidth() = 0;

	virtual int getGameHeight() = 0;

	virtual float getAspectRatio() = 0;

	virtual float getDeltaTime() = 0;

	virtual float getTotalTime() = 0;

	virtual int getFPS() = 0;

	virtual void getViewport(int &x, int &y, int &w, int &h) = 0;

	virtual void setAutoUpdateViewport(bool value) = 0;

	virtual bool isAutoUpdateViewportEnabled() = 0;

	virtual void setGameDimension(int w, int h) = 0;

	virtual void toScreenCoords(float &viewportx, float &viewporty) = 0;

	virtual void toViewportCoords(float &screenx, float &screeny) = 0;

};

class DefaultGraphicsImp : public IGraphics {

private:
	int gameWidth=0, gameHeight=0, screenWidth=0, screenHeight=0;
	int viewportx=0, viewporty=0, viewportw=0, viewporth=0;
	bool autoUpdateViewport;
	int fps;
	float deltaTime, totalTime;

	void updateViewport();

public:
	int getScreenWidth();

	int getScreenHeight();
	
	int getGameWidth();

	int getGameHeight();

	float getAspectRatio();

	float getDeltaTime();

	float getTotalTime();

	int getFPS();

	void getViewport(int &x, int &y, int &w, int &h);

	void setAutoUpdateViewport(bool value);

	bool isAutoUpdateViewportEnabled();

	void setGameDimension(int w, int h);

	void toScreenCoords(float &viewportx, float &viewporty);

	void toViewportCoords(float &screenx, float &screeny);



	void updateScreenSize(int w, int h);

	void updateOthers(float deltaTime, float totalTime, int fps);

};



class TextureManager {

private:
	std::map<std::string, Texture*> textures;

public:

	~TextureManager();

	void dispose();

	void clear();

	Texture *loadTexture(std::string path);

	Texture *getTexture(std::string path);

};



class IGameState {

public:

	virtual ~IGameState(){}

	virtual void enter(){};

	virtual void exit(){};

	virtual void update(float delta)=0;

	virtual void draw()=0;

};


class GameStateManager {

private:
	std::vector<IGameState*> states;

public:

	~GameStateManager();

	IGameState &getActive();

	void push(IGameState *gst);

	IGameState *pop();

	bool isEmpty();

	int getStatesCount();

};




class WIZ {

private:

	static IGraphics *graphics;

	static TextureManager textures;

	static IApplication *application;

	static GameStateManager gameStateManager;

public:

	static void setup(IApplication *application, IGraphics *graphics);


	static IApplication &getApplication();

	static TextureManager &getTextureManager();

	static IGraphics &getGraphics();

	static GameStateManager &getGameStateManager();



};




#endif
