#pragma once

#ifndef MY_GAME_H
#define MY_GAME_H

#include "includes.h"
#include "image.h"
#include "utils.h"
#include "synth.h"
#include "includes.h"
#include "image.h"


class Stage {

public:
	//Synth synth;
	virtual void render(Image& framebuffer, Image minifont, Image sprite, Image font) {}; //empty body
	virtual void update() {}; //empty body
}; 

class IntroStage : public Stage {
public:
	virtual void render(Image& framebuffer, Image minifont, Image sprite, Image font);
	//virtual void update();
}; 
class PlayStage : public Stage {
public:
	//virtual void render(Image& framebuffer, Image minifont, Image sprite, Image font);
	//virtual void update();
};

class World {
	
};

#endif