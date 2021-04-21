#include "game.h"
#include "mygame.h"
#include "utils.h"
#include "input.h"
#include "image.h"

#include <cmath>

//constructor

sPlayer::sPlayer()
{
	this->dir = eDIRECTION::RIGHT; 
	this->pos = Vector2(14.5, 94.5);
	this->player_velocity = 50;
	this->animLenght = 4;
	this->animation_velocity = 10.0f;
	this->jump = 0.0f;
	this->pixelToJump = 3;
	this->moving = false;
	this->Implayer = Image(); 
}

sPlayer::sPlayer(Image& playerIm)
{
	this->dir = eDIRECTION::RIGHT;
	this->pos = Vector2(14.5, 94.5);
	this->player_velocity = 50;
	this->animLenght = 4;
	this->animation_velocity = 10.0f;
	this->jump = 0.0f;
	this->pixelToJump = 3;
	this->moving = false;
	this->Implayer = playerIm;
}

World::World()
{
	playerReal.loadTGA("data/spritesheet.tga");
	playerAlpha.loadTGA("data/spritesheetAlpha.tga");
	font.loadTGA("data/bitmap-font-black.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-black-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/background2.tga"); //example to load an sprite
	objects.loadTGA("data/objects.tga");
	tileset.loadTGA("data/tileset.tga");
	map[0] = map[0]->loadGameMap("data/mymapL1.map");
	map[1] = map[1]->loadGameMap("data/mymapL2.map");
	level = 0; 
	timeGameing = 0.0f; 
	contMov = 0; 

	for (int i = 0; i < N_PLAYER; i++) {
		this->player[i] = sPlayer(playerReal);
	}
}


//function
//Intro
void IntroStage::render(Image& framebuffer) {

	Game* Insgame = Game::instance; //singelton	
	World* InsWorld= Game::instance->my_world;

	framebuffer.fill(Color::CYAN);								//fills the image with one color
	framebuffer.drawImage(InsWorld->sprite, 0, 0, framebuffer.width, framebuffer.height);			//draws a scaled image
	
	framebuffer.drawText("4 DIMENSION", framebuffer.width / 4, framebuffer.height / 5, InsWorld->font);
	//draws some text using a bitmap font in an image (assuming every char is 7x9)
	//framebuffer.drawText(toString(Game::instance->time), 1, 10, my_world->minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	//framebuffer.drawText(toString(Insgame->time), 1, 10, InsWorld->minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)

    //void drawText( std::string text, int x, int y, const Image& bitmapfont, int font_w = 7, int font_h = 9, int first_char = 32);


	//audio intro
	Insgame->synth.playSample("data/lassambience1.wav", 2, true);

	bottonIntro(framebuffer);
}


void IntroStage::bottonIntro(Image& framebuffer)
{
	//coordenate framework to window 
	Game* Insgame = Game::instance; //singelton
	World* InsWorld = Game::instance->my_world;
	sButton* Insbutton = &Game::instance->my_world->inicio;
	
	Insbutton->WsquareIni_PointW = (Insbutton->FsquareBig_PointW * Insgame->window_width) / framebuffer.width;
	Insbutton->WsquareIni_PointH = (Insbutton->FsquareBig_PointH * Insgame->window_height) / framebuffer.height;
	Insbutton->WsquareFin_W = ((Insbutton->FsquareBig_W + Insbutton->FsquareBig_PointW) * Insgame->window_width) / framebuffer.width;
	Insbutton->WsquareFin_H = ((Insbutton->FsquareBig_H + Insbutton->FsquareBig_PointH)* Insgame->window_height) / framebuffer.height;

	//rectangle big
	framebuffer.drawRectangle(Insbutton->FsquareBig_PointW, Insbutton->FsquareBig_PointH, Insbutton->FsquareBig_W, Insbutton->FsquareBig_H, Color (0, 0, 0));
	//rectangle small
	framebuffer.drawRectangle(Insbutton->FsquareBig_PointW + 1, Insbutton->FsquareBig_PointH + 1, Insbutton->FsquareBig_W - 2, Insbutton->FsquareBig_H - 2, Color (86, 130, 91));

	Vector2 v2_mouse = Input::mouse_position;
	
	// Change button color if mouse position is button
	if (v2_mouse.x > Insbutton->WsquareIni_PointW && v2_mouse.y > Insbutton->WsquareIni_PointH && //
		v2_mouse.x < Insbutton->WsquareFin_W && v2_mouse.y < Insbutton->WsquareFin_H)
	{
		framebuffer.drawRectangle(Insbutton->FsquareBig_PointW + 1, Insbutton->FsquareBig_PointH + 1, Insbutton->FsquareBig_W - 2, Insbutton->FsquareBig_H - 2, Color (129, 194, 137));
	}

	// Text to button 
	framebuffer.drawText("Inicio", framebuffer.width / 2 - 10, framebuffer.height / 2 + framebuffer.height / 3, InsWorld->minifont, 4, 6);

}


void IntroStage::update(double seconds_elapsed)
{
	//singelton
	Game* Insgame = Game::instance;
	sPlayer* Insplayer1 = &Game::instance->my_world->player[0];
	World* InsWorld = Game::instance->my_world;
	/*
	sPlayer* Insplayer2 = &Game::instance->my_world->player2;*/
	sButton* Insbutton = &Game::instance->my_world->inicio;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
	}

	//example of 'was pressed'
	//if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	//{
	//	Game::instance->current_stage = Game::instance->play_stage;
	//	Game::instance->synth.stopAll();
	//}

	//press button inicio
	if (Input::mouse_state==1 &&(Input::mouse_position.x > Insbutton->WsquareIni_PointW && 
		Input::mouse_position.y > Insbutton->WsquareIni_PointH && Input::mouse_position.x < Insbutton->WsquareFin_W 
		&& Input::mouse_position.y < Insbutton->WsquareFin_H))
	{
		//reset players
		for (int i = 0; i < N_PLAYER; i++)
		{
			Game::instance->my_world->player[i] = sPlayer(InsWorld->playerReal); //incialitze player
		}

		Insgame->current_stage = Insgame->play_stage;
		InsWorld->timeGameing = Insgame->time; 
		Insgame->synth.stopAll();
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		//bgcolor.set(0, 255, 0);
	}

}

//PLayer
void PlayStage::render(Image& framebuffer)
{
	//singelton
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	sPlayer* Insplayer1;
	sPlayer* InsplayerAlpha;
	

	//sPlayer* Insplayer2 = &Game::instance->my_world->player[1];
	GameMap* Insmap = Game::instance->my_world->map[InsWorld->level];
	
	
	Vector2 moviment = Vector2(16, -8);
	int cs = InsWorld->tileset.width / 16;
	framebuffer.fill(Color::CYAN);								//fills the image with one color

	//for every cell
	for (int x = 0; x < Insmap->width; ++x)
		for (int y = 0; y < Insmap->height; ++y)
		{
			//get cell info
			sCell& cell = Insmap->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			int type = (int)cell.type;
			//compute tile pos in tileset image
			int tilex = (type % 16) * cs; 	//x pos in tileset
			int tiley = floor(type / 16) * cs;	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = x * cs + moviment.x; //place offset here if you want
			int screeny = y * cs + moviment.y;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			framebuffer.drawImage(InsWorld->tileset, screenx, screeny, area); //image //pos in screen	//area
		}
	//render objects
	//framebuffer.drawImage(InsWorld->objects, 30, 40, Area(0, 0, 11, 72));
	//rocas
	//framebuffer.drawImage(InsWorld->objects, 30, 30, Area(12, 0, 8, 15));

	//Render Players

	/*Vector2 v2_mouse = Input::mouse_position;*/
	if (Insgame->time - InsWorld->timeGameing < 10.0f)
	{
		Insplayer1 = &Game::instance->my_world->player[0];
	}
	if (Insgame->time - InsWorld->timeGameing >= 10.0f && Insgame->time - InsWorld->timeGameing < 20.0f)
	{
		Insplayer1 = &Game::instance->my_world->player[1];
		InsplayerAlpha = &Game::instance->my_world->player[0];
		//PLotear player1 con los movimientos hechos anteriormente
		if (InsWorld->contMov < InsWorld->movPlayer1.size()-1)
		{
			//InsWorld->movPlayer1[0] = Vector2(20, 94.5f); 
			MovPlayerAlpha(InsplayerAlpha);
			int currentAnimAlpha = InsplayerAlpha->moving ? (int(Insgame->time * InsplayerAlpha->animation_velocity) % InsplayerAlpha->animLenght) : 0;
			framebuffer.drawImage(InsWorld->playerAlpha, InsWorld->movPlayer1[InsWorld->contMov].x, InsWorld->movPlayer1[InsWorld->contMov].y, 
				Area(14 * currentAnimAlpha, 18 * (int)InsplayerAlpha->dir, 14, 18));
		}
		InsWorld->contMov += 1; 
	}
	if (Insgame->time - InsWorld->timeGameing >= 20.0f)
	{
		InsWorld->timeGameing = Insgame->time;
		Insplayer1 = &Game::instance->my_world->player[0];

	}
	int currentAnimP1 = Insplayer1->moving ? (int(Insgame->time * Insplayer1->animation_velocity) % Insplayer1->animLenght) : 0;
	framebuffer.drawImage(Insplayer1->Implayer, Insplayer1->pos.x, Insplayer1->pos.y, Area(14 * currentAnimP1, 18 * (int)Insplayer1->dir, 14, 18));
	//std::cout <<"INICIAR" << "\n";
	//for (int i = 0; i < InsWorld->movPlayer1.size(); i++)
	//	std::cout << InsWorld->movPlayer1[i].x << " "<<InsWorld->movPlayer1[i].y<< "\n";
	//escalera
	framebuffer.drawText(toString((int)(Insgame->time - InsWorld->timeGameing)), 1, 10, InsWorld->minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)


}

void PlayStage::MovPlayerAlpha(sPlayer* InsplayerAlpha)
{
	World* InsWorld = Game::instance->my_world;
	float x = InsWorld->movPlayer1[InsWorld->contMov+1].x - InsWorld->movPlayer1[InsWorld->contMov].x; 
	float y = InsWorld->movPlayer1[InsWorld->contMov+1].y - InsWorld->movPlayer1[InsWorld->contMov].y; 
	InsplayerAlpha->moving = true;
	if (x > 0.0f)
		InsplayerAlpha->dir = RIGHT; 
	if (y > 0.0f)
		InsplayerAlpha->dir = DOWN;
	if (x < 0.0f)
		InsplayerAlpha->dir = LEFT;
	if (y < 0.0f)
		InsplayerAlpha->dir = UP;
	if (x==0.0f && y==0.0f)
		InsplayerAlpha->moving = false; 
};

void PlayStage::update(double seconds_elapsed) { //movement of the character
	//singelton
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	/*sPlayer* Insplayer1 = &Game::instance->my_world->player[0];
	sPlayer* Insplayer2 = &Game::instance->my_world->player[1];*/
	sPlayer* Insplayer1;
	if (Insgame->time - InsWorld->timeGameing < 10.0f)
	{
		Insplayer1 = &Game::instance->my_world->player[0];
		InsWorld->movPlayer1.push_back(Insplayer1->pos);
	}
	if (Insgame->time - InsWorld->timeGameing >= 10.0f && Insgame->time - InsWorld->timeGameing < 20.0f)
	{
		Insplayer1 = &Game::instance->my_world->player[1];
	}
	if (Insgame->time - InsWorld->timeGameing >= 20.0f)
	{
		InsWorld->timeGameing = Insgame->time;
		Insplayer1 = &Game::instance->my_world->player[0];
		InsWorld->movPlayer1.clear(); 
		InsWorld->contMov = 0;
	}
	Insplayer1->moving = false;
	//list 

	

	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		Insplayer1->pos.y -= Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::UP;
		Insplayer1->moving = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		Insplayer1->pos.y += Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::DOWN;
		Insplayer1->moving = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		Insplayer1->pos.x += Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::RIGHT;
		Insplayer1->moving = true;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		Insplayer1->pos.x -= Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::LEFT;
		Insplayer1->moving = true;
	}
	//JUMP 
	// Si pasan 0.5 segundos de haber apretado el espacio para saltar
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == false && Insplayer1->jump != 0.0f && Insgame->time > Insplayer1->jump + 0.5f)
	{
		Insplayer1->pos.y += Insplayer1->pixelToJump; // Devolver en la misma posición original
		Insplayer1->moving = true;
		Insplayer1->jump = 0.0f;
	}
	// Solo puede apretar 1 vez el salto si aún no ha vuelto al puesto original
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && Insplayer1->jump==0.0f)
	{
		Insplayer1->pos.y -= Insplayer1->pixelToJump;
		Insplayer1->moving = true;
		Insplayer1->jump = Insgame->time;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) //if key Z was pressed state= intro
	{
		//Insplayer1->pos = Vector2(14.5, 94.5);
		//Game::instance->my_world->player[0].pos = Vector2(14.5, 94.5);
				
		Insgame->current_stage = Insgame->intro_stage;
		
		//game->my_world->movPlayer1.clear();
	}
}


//Gamemap
GameMap* GameMap::loadGameMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return NULL;
	else
		std::cout << " + File loaded: " << filename <<"\n"; 

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);
	assert(header.bytes == 1); //always control bad cases!!


	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w * header.h];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	GameMap* map = new GameMap(header.w, header.h);

	for (int x = 0; x < map->width; x++) {
		for (int y = 0; y < map->height; y++) {
			map->getCell(x, y).type = (eCellType)cells[x + y * map->width];

			//if (map->getCell(x, y).type == FLOOR)
			//	std::cout << "HOLA" << "\n";
		}
	}

	delete[] cells; //always free any memory allocated!

	return map;
}


