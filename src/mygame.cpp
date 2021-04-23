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
	this->pos = Vector2(14.5, 93);
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
	this->pos = Vector2(14.5, 93);
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
	nextLevel = false; 
	timeGameing = 0.0f; 
	contMov = 0; 

	ROCK = objects.getArea(40, 0, 20, 25); //creates an image given an area
	ROCK.scale(50, 50); 

	for (int i = 0; i < N_PLAYER; i++) {
		this->player[i] = sPlayer(playerReal);
	}
}


//function
//Intro
void IntroStage::render(Image& framebuffer) {

	Game* Insgame = Game::instance; //singelton	
	World* InsWorld= Game::instance->my_world;

	framebuffer.fill(Color::CYAN);	//fills the image with one color
	framebuffer.drawImage(InsWorld->sprite, 0, 0, framebuffer.width, framebuffer.height);			//draws a scaled image
	
	framebuffer.drawText("4 DIMENSION", framebuffer.width / 4, framebuffer.height / 5, InsWorld->font);
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
	sButton* Insbutton = &Game::instance->my_world->inicio;

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

		InsWorld->timeGameing = Insgame->time; 
		Insgame->synth.stopAll();
		Insgame->current_stage = Insgame->play_stage;
	}
}

//Player
void PlayStage::render(Image& framebuffer)
{
	
	framebuffer.fill(Color::CYAN);	//fills the image with one color
	// render cells
	renderCells(framebuffer); 
	
	//render objects
	AppearObjects(framebuffer);

	//Render Players
	renderPlayers(framebuffer); 
}

void PlayStage::renderPlayers(Image& framebuffer) 
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	sPlayer* Insplayer1;
	sPlayer* InsplayerAlpha;
	if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
	{
		Insplayer1 = &Game::instance->my_world->player[0];
	}
	if (Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2))
	{
		Insplayer1 = &Game::instance->my_world->player[1];
		InsplayerAlpha = &Game::instance->my_world->player[0];
		// Plot player1 with the movements made previously
		if (InsWorld->movPlayer1.size()!=0 && InsWorld->contMov < InsWorld->movPlayer1.size() - 1)
		{
			//Calculate player1 direction and movement, when playing player 2
			MovPlayerAlpha(InsplayerAlpha);
			int currentAnimAlpha = InsplayerAlpha->moving ? (int(Insgame->time * InsplayerAlpha->animation_velocity) % InsplayerAlpha->animLenght) : 0;
			framebuffer.drawImage(InsWorld->playerAlpha, InsWorld->movPlayer1[InsWorld->contMov].x, InsWorld->movPlayer1[InsWorld->contMov].y,
				Area(14 * currentAnimAlpha, 18 * (int)InsplayerAlpha->dir, 14, 18));
		}
		InsWorld->contMov += 1;
	}

	int currentAnimP1 = Insplayer1->moving ? (int(Insgame->time * Insplayer1->animation_velocity) % Insplayer1->animLenght) : 0;
	framebuffer.drawImage(Insplayer1->Implayer, Insplayer1->pos.x, Insplayer1->pos.y, Area(14 * currentAnimP1, 18 * (int)Insplayer1->dir, 14, 18));
	framebuffer.drawText(toString((int)(Insgame->time - InsWorld->timeGameing)), 1, 10, InsWorld->minifont, 4, 6);

}

void PlayStage::renderCells(Image& framebuffer)
{
	World* InsWorld = Game::instance->my_world;
	GameMap* Insmap = Game::instance->my_world->map[InsWorld->level];
	int cs = InsWorld->tileset.width / 16;
	Vector2 moviment = Vector2(16, -8);

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
			int screenx = (x * cs) + moviment.x; //place offset here if you want
			int screeny = (y * cs) + moviment.y;
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx > framebuffer.width ||
				screeny < -cs || screeny > framebuffer.height)
				continue;

			//draw region of tileset inside framebuffer
			framebuffer.drawImage(InsWorld->tileset, screenx, screeny, area); //image //pos in screen	//area
		}
}

void PlayStage::AppearObjects(Image& framebuffer)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	sPlayer* Insplayer1 = &Game::instance->my_world->player[0];
	vector<Vector2> InsList = Game::instance->my_world->movPlayer1;
	int cont = Game::instance->my_world->contMov;
	//pies del player
	float x = Insplayer1->pos.x + 14;
	float y = Insplayer1->pos.y + 18;


	if (InsWorld->level == 0) 
	{
		if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
		{
			if (x >= 115 && x <= 135 && y >= 107 && y <= 112) {
				framebuffer.drawImage(InsWorld->objects, 30, 40, Area(0, 0, 11, 72));
				framebuffer.drawRectangle(115, 110, 20, 2, Color(147, 157, 148));
				//Insgame->synth.playSample("data/bip.wav", 0.5, false);

			}
			else
				framebuffer.drawRectangle(115, 107, 20, 5, Color(147, 157, 148));
		}
		if (InsList.size() != 0 && cont < InsList.size() && Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && 
			Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2)){

			// pies del playerAlpha
			float Alphx = InsList[cont].x + 14;
			float Alphy = InsList[cont].y + 18;
			if (Alphx >= 115 && Alphx <= 135 && Alphy >= 107 && Alphy <= 112) {
				framebuffer.drawImage(InsWorld->objects, 30, 40, Area(0, 0, 11, 72));
				framebuffer.drawRectangle(115, 110, 20, 2, Color(147, 157, 148));
				//Insgame->synth.playSample("data/bip.wav", 0.5, false);

			}
			else
				framebuffer.drawRectangle(115, 107, 20, 5, Color(147, 157, 148));
		}
	}
	//rocas
	if (InsWorld->level == 1)
	{
		framebuffer.drawImage(InsWorld->ROCK, 30, 56);
		framebuffer.drawImage(InsWorld->ROCK, 50, 56);
		framebuffer.drawImage(InsWorld->ROCK, 70, 56);
	}
}

//Calculate player1 direction and movement, when playing player 2
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
	sPlayer* Insplayer1;
	Vector2 moviment = Vector2(16, -8);


	if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
	{
		Insplayer1 = &Game::instance->my_world->player[0];
		InsWorld->movPlayer1.push_back(Insplayer1->pos);
	}
	if (Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER*2))
	{
		Insplayer1 = &Game::instance->my_world->player[1];
	}
	if (Insgame->time - InsWorld->timeGameing >= (TIME_GAME_PLAYER*2))
	{
		InsWorld->timeGameing = Insgame->time;
		Insplayer1 = &Game::instance->my_world->player[0];
		InsWorld->movPlayer1.clear(); 
		InsWorld->contMov = 0;
		nextSteep(); 
	}
	Insplayer1->moving = false;
		
	Vector2 target = Insplayer1->pos;
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		target.y -= Insplayer1->player_velocity * seconds_elapsed; 
		Insplayer1->dir = eDIRECTION::UP;
		Insplayer1->moving = true;		
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		target.y += Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::DOWN;
		Insplayer1->moving = true;
		
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		target.x += Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::RIGHT;
		Insplayer1->moving = true;
		
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		target.x -= Insplayer1->player_velocity * seconds_elapsed;
		Insplayer1->dir = eDIRECTION::LEFT;
		Insplayer1->moving = true;
		
	}
	//JUMP 
	// Si pasan 0.5 segundos de haber apretado el espacio para saltar
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == false && Insplayer1->jump != 0.0f && Insgame->time > Insplayer1->jump + 0.2f)
	{
		target.y += Insplayer1->pixelToJump; // Devolver en la misma posición original
		Insplayer1->moving = true;
		Insplayer1->jump = 0.0f;
		
	}
	// Solo puede apretar 1 vez el salto si aún no ha vuelto al puesto original
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) && Insplayer1->jump==0.0f)
	{
		target.y -= Insplayer1->pixelToJump;
		Insplayer1->moving = true;
		Insplayer1->jump = Insgame->time;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_O)) //if key O state: GAME OVER
	{
		Insgame->current_stage = Insgame->game_over;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) //if key R state: intro
	{
		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		InsWorld->level = 0; 
		InsWorld->nextLevel = false; 
		Insgame->current_stage = Insgame->intro_stage;

	}

	if (Insplayer1->isValid(target))
		Insplayer1->pos = target; 

	int x[2];
	int y = ((Insplayer1->pos.y + (18 * 1) - moviment.y) / 8);

	for (int i = 0; i < 2; i++)
	{
		x[i] = ((Insplayer1->pos.x + (14 * i) - moviment.x) / 8);
		sCell celda = InsWorld->map[InsWorld->level]->getCell(x[i], y);
		//Si entra a la cueva
		if (celda.type == 163 || celda.type == 164 || celda.type == 112 || celda.type == 128 || celda.type == 96 || celda.type == 144)
		{
			//va al Level 2, si pasa por la cueva y esta en el level 1
			if(InsWorld->level == 0 && InsWorld->nextLevel == true)
				Insgame->current_stage = Insgame->pause_stage;
			else { //Win state, si esta al level 2 y pasa por la cueva
				InsWorld->nextLevel = true;
				InsWorld->movPlayer1.clear();
				InsWorld->contMov = 0;
				if (InsWorld->level == 1 && InsWorld->nextLevel == true)
				{
					Insgame->current_stage = Insgame->win_stage;
				}
			}
		}
	}
}



void  PlayStage::nextSteep() 
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;


	//IF no llega a cueva
	if (InsWorld->nextLevel == false)
	{
		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		Insgame->current_stage = Insgame->game_over;

	}

}

void PauseLevel1to2::render(Image& framebuffer)
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color::CYAN);								//fills the image with one color

	framebuffer.drawText("Level 2", framebuffer.width / 4, framebuffer.height / 5, InsWorld->font);

}

void PauseLevel1to2::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key ENTER
	{
		InsWorld->nextLevel = true;
		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		for (int i = 0; i < N_PLAYER; i++)
		{
			Game::instance->my_world->player[i] = sPlayer(InsWorld->playerReal); //incialitze player
		}

		InsWorld->timeGameing = Insgame->time;
		InsWorld->nextLevel = false;
		InsWorld->level = 1;
		Insgame->current_stage = Insgame->play_stage;


	}
}

void GameOver::render(Image& framebuffer) 
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color::CYAN);								//fills the image with one color

	framebuffer.drawText("GAME OVER", framebuffer.width / 4, framebuffer.height / 5, InsWorld->font);

}

void GameOver::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key ENTER
	{
		
		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		InsWorld->timeGameing = Insgame->time;
		InsWorld->nextLevel = false;
		InsWorld->level = 0;
		Insgame->current_stage = Insgame->intro_stage;

		
	}
}

void Win::render(Image& framebuffer)
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color::CYAN);								//fills the image with one color
	framebuffer.drawText("You Win", framebuffer.width / 4, framebuffer.height / 5, InsWorld->font);

}

void Win::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key up
	{

		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		InsWorld->timeGameing = Insgame->time;
		InsWorld->nextLevel = false;
		InsWorld->level = 0;
		Insgame->current_stage = Insgame->intro_stage;


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

bool sPlayer::isValid(Vector2 positionPlayer)
{
	World* InsWorld = Game::instance->my_world;
	
	Vector2 moviment = Vector2(16, -8);
	int cs = InsWorld->tileset.width / 16;
	int x[2]; 
	int y= ((positionPlayer.y + (18 * 1) - moviment.y) / cs);

	for (int i = 0; i < 2; i++)
	{
		x[i] = ((positionPlayer.x +(14 *i) - moviment.x) / cs);

	}
	
	//std::cout << x[1] << " " << y << "\n";

	   
	for (int i = 0; i < 2; i++)
	{
		if (InsWorld->map[InsWorld->level]->getCell(x[i], y).type == 64)
			return false;
		
	}
	
	return true;
	//if (screenx < -cs || screenx > framebuffer.width ||
	//	screeny < -cs || screeny > framebuffer.height)
}

