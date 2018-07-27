#include "platformgl.h"
#include "WIZ/wiz.h"

#include <iostream>
#include <stdexcept>

/*


	G R A P H I C S


*/


int DefaultGraphicsImp::getScreenWidth() {
	return screenWidth;
}

int DefaultGraphicsImp::getScreenHeight() {
	return screenHeight;
}

int DefaultGraphicsImp::getGameWidth() {
	return gameWidth;
}

int DefaultGraphicsImp::getGameHeight() {
	return gameHeight;
}

float DefaultGraphicsImp::getAspectRatio() {
	return gameWidth / gameHeight;
}

float DefaultGraphicsImp::getDeltaTime() {
	return deltaTime;
}

float DefaultGraphicsImp::getTotalTime() {
	return totalTime;
}

int DefaultGraphicsImp::getFPS() {
	return fps;
}

void DefaultGraphicsImp::getViewport(int &x, int &y, int &w, int &h) {
	x = viewportx;
	y = viewporty;
	w = viewportw;
	h = viewporth;
}

void DefaultGraphicsImp::setAutoUpdateViewport(bool value) {
	autoUpdateViewport = value;
}

bool DefaultGraphicsImp::isAutoUpdateViewportEnabled() {
	return autoUpdateViewport;
}

void DefaultGraphicsImp::setGameDimension(int w, int h) {
	std::cout << "Abstract Graphics Game dimension change: " << w << ", " << h << std::endl;
	gameWidth = w;
	gameHeight = h;
	if (screenWidth == 0 || screenHeight == 0) {
		screenWidth = gameWidth;
		screenHeight = gameHeight;
	}		
	updateViewport();
}

void DefaultGraphicsImp::toScreenCoords(float &viewportx, float &viewporty) {
	throw std::runtime_error("Not implemented!!!!");
}

void DefaultGraphicsImp::toViewportCoords(float &screenx, float &screeny) {
	screenx = (screenx - viewportx) * (viewportw / screenWidth);
	screeny = (screeny - viewporty) * (viewporth / screenHeight);
	updateViewport();
}


void DefaultGraphicsImp::updateViewport() {
	float sx = screenWidth / (float) gameWidth;
	float sy = screenHeight / (float) gameHeight;
	
	if (sy < sx) {
		float nwidth = gameWidth * sy;
		viewportx = (int) ((screenWidth - nwidth) / 2);
		viewportw = (int)nwidth;
		viewporth = screenHeight;
		viewporty = 0;

		std::cout << "here2 " << std::to_string(viewporty) << " h " <<std::to_string(viewporth) << std::endl;

	} else {
		float nheight = gameHeight * sx;
		viewporty = (int) ((screenHeight - nheight) / 2);
		viewporth = (int)nheight;
		viewportw = screenWidth;
		viewportx = 0;
	}
	
	// store the viewport
	glViewport(viewportx, viewporty, viewportw, viewporth);
	
	//Gdx.gl.glViewport((int)x, (int)y, width, height);
}



void DefaultGraphicsImp::updateScreenSize(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	if (gameWidth == 0 || gameHeight == 0) {
		gameWidth = w;
		gameHeight = h;
	}
	std::cout << "Abstract Graphics screen dimension change: " << w << ", " << h << std::endl;
	updateViewport();
}

void DefaultGraphicsImp::updateOthers(float deltaTime, float totalTime, int fps) {
	this->deltaTime = deltaTime;
	this->totalTime = totalTime;
	this->fps = fps;
}







/*



	T E X T U R E    M A N A G E R



*/

TextureManager::~TextureManager() {
	dispose();
}

void TextureManager::dispose() {
	for (auto t = textures.begin(); t != textures.end(); t++)
		delete t->second;
	clear();
}

void TextureManager::clear() {
	textures.clear();
}

Texture *TextureManager::loadTexture(std::string path) {
	Texture *texture = getTexture(path);
	if (texture == nullptr) {
		texture = new Texture(path.c_str());
		textures.insert(std::map<std::string, Texture*>::value_type(path, texture));
	}
	return texture;
}

Texture *TextureManager::getTexture(std::string path) {
	auto iterator = textures.find(path.c_str());
	return iterator != textures.end() ? iterator->second : nullptr;
}







/*



*/

GameStateManager::~GameStateManager() {
	while (!isEmpty()) {
		delete &getActive();
	}
}

IGameState &GameStateManager::getActive() {
	if (isEmpty())
		throw std::runtime_error("There's no active GameState ()");
	return *(states.back());
}

void GameStateManager::push(IGameState *gst) {
	states.push_back(gst);
}

IGameState *GameStateManager::pop() {
	if (isEmpty())
		throw std::runtime_error("No more GameState's to pop()");
	return states.back();
}

bool GameStateManager::isEmpty() {
	return getStatesCount() == 0;
}

int GameStateManager::getStatesCount() {
	return states.size();
}


/**



*/

void DefaultApplication::renderer(float delta) {
	update(delta);
	draw();
}

void DefaultApplication::update(float delta) {
	if (!WIZ::getGameStateManager().isEmpty())
		WIZ::getGameStateManager().getActive().update(delta);
}

void DefaultApplication::draw() {
	if (!WIZ::getGameStateManager().isEmpty())
		WIZ::getGameStateManager().getActive().draw();
}




/*



	W I Z




*/

IGraphics          *WIZ::graphics=nullptr;
IApplication       *WIZ::application;
TextureManager   	WIZ::textures;
GameStateManager   	WIZ::gameStateManager;

void WIZ::setup(IApplication *app, IGraphics *graphics) {
	WIZ::application = app;
	WIZ::graphics = graphics;
}

TextureManager &WIZ::getTextureManager() {
	return WIZ::textures;
}

GameStateManager &WIZ::getGameStateManager() {
	return WIZ::gameStateManager;
}

IGraphics &WIZ::getGraphics() {
	return *(WIZ::graphics);
}

IApplication &WIZ::getApplication() {
	return *(WIZ::application);
}
