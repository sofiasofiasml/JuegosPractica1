#pragma once

#ifndef MY_GAME_H
#define MY_GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "includes.h"
#include "image.h"


enum eCellType : uint8 {
	EMPTY, START,
	WALL,
	DOOR,
	CHEST
};

enum eItemType : uint8 {
	NOTHING,
	SWORD,
	POTION
};

struct sCell {
	eCellType type;
	eItemType item;
};

class GameMap {
public:
	int width;
	int height;
	sCell* data;

	GameMap()
	{
		width = height = 0;
		data = NULL;
	}

	GameMap(int w, int h)
	{
		width = w;
		height = h;
		data = new sCell[w * h];
	}

	sCell& getCell(int x, int y)
	{
		return data[x + y * width];
	}
};

class World {
public:
	Image font;
	Image minifont;
	Image sprite;
};
class Stage {

public:
	//Synth synth;
	virtual void render(Image& framebuffer, World* my_world) {}; //empty body
	virtual void update() {}; //empty body
}; 

class IntroStage : public Stage {
public:
	virtual void render(Image& framebuffer, World* my_world);
	//virtual void update();
}; 
class PlayStage : public Stage {
public:
	//virtual void render(Image& framebuffer, World* my_world);
	//virtual void update();
};



#endif