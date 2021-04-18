#pragma once

#ifndef MY_GAME_H
#define MY_GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "game.h"
#include <string>

using namespace std; 
#define N_PLAYER    2
#define N_NIVEL     2

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
	GameMap* loadGameMap(const char* filename); 
};

struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};
//example of parser of .map from rogued editor
	//point to rectangle 
struct sButton {
	int w_framework = 160;
	int h_framework = 120;
	float FsquareBig_PointW = w_framework / 2 - 13;
	float FsquareBig_PointH = h_framework / 2 + h_framework / 3 - 3;
	float FsquareBig_W = 29;
	float FsquareBig_H = 11;

	float WsquareIni_PointW;
	float WsquareIni_PointH;
	float WsquareFin_W;
	float WsquareFin_H;

};
enum eDIRECTION : uint8
{
	DOWN,
	RIGHT,
	LEFT,
	UP
};
struct sPlayer
{
public: 
	Vector2 pos;
	float player_velocity;
	Image Implayer;
	eDIRECTION dir;
	bool moving;
	int animLenght;
	float animation_velocity;
	float jump;
	int pixelToJump;

	sPlayer();

};

class World {
public:
	Image font;
	Image minifont;
	Image sprite;
	//Image LoreGame;
	Image objects; 
	Image playerReal;
	Image playerAlpha;
	
	//map
	Image tileset;
	GameMap* map[N_NIVEL];
	int level;
	//List moviments
	vector<string> movPlayer1;

	sPlayer player[N_PLAYER];
	sButton inicio;

	World(); 
};
class Stage {

public:
	virtual void render(Image& framebuffer) {}; //empty body
	virtual void update(double seconds_elapsed) {}; //empty body
}; 

class IntroStage : public Stage {
public:
	virtual void render(Image& framebuffer);
	void bottonIntro(Image& framebuffer);
	virtual void update(double seconds_elapsed);
}; 
class PlayStage : public Stage {
public:
	virtual void render(Image& framebuffer);
	virtual void update(double seconds_elapsed);
};



#endif