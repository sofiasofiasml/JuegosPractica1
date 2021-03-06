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
	keyboard.loadTGA("data/keyboard.tga"); 
	map[0] = map[0]->loadGameMap("data/mymapL1.map");
	map[1] = map[1]->loadGameMap("data/mymapL2.map");
	level = 0; 
	nextLevel = false; 
	timeGameing = 0.0f; 
	contMov = 0; 
	moviment = Vector2(16, -8);
	objectEscalera = false;
	nextImageIntro = false; 
	
	objectsRock = 0;
	
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
		Insgame->current_stage = Insgame->explic_stage;
	}
}

//Explicacion

void Explication::render(Image& framebuffer) {

	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;
	Insgame->synth.stopAll(); 
	framebuffer.fill(Color(106, 151, 111));	//fills the image with one color
	string text = "Tienes 10 segundos para jugar con el    jugador de la realidad incial paralela  y despues 10 segundos para el segundo   jugador de la otra realidad. ";
	string text2 = "Debes desbloquear el puzzle para llegar a la entrada.";
	string cad; 
	if (InsWorld->nextImageIntro == false) {
		for (int i = 0; i < 6; i++)
		{
			if (i < 4) 
				cad = text.substr(40 * i, 39 * (i + 1));
			else 
				cad = text2.substr(40 * (i - 4), 39 * (i - 3));

			framebuffer.drawText(cad, 1, 10 * (i + 1), InsWorld->minifont, 4, 6);
		}
		if ((int)Insgame->time % 2)
			framebuffer.drawText("Pulsa Enter para continuar", 50, 110, InsWorld->minifont, 4, 6);
	}
	else 
	{
		framebuffer.drawText("Teclado ", 63, 4, InsWorld->minifont, 4, 6);
		framebuffer.drawText("Saltar ", 51, 79, InsWorld->minifont, 4, 6);
		framebuffer.drawText("Solo en la ", 31, 51, InsWorld->minifont, 4, 6);
		framebuffer.drawText("R ", 14, 94, InsWorld->minifont, 4, 6);
		framebuffer.drawText("Reiniciar el juego ", 33, 95, InsWorld->minifont, 4, 6);
		
		framebuffer.drawImage(InsWorld->keyboard, 0, 0, framebuffer.width, framebuffer.height);			//draws a scaled image
		//stairs 
		framebuffer.drawImage(InsWorld->objects, 71, 40, Area(30, 0, 15, 27));
		if ((int)Insgame->time % 2)
			framebuffer.drawText("Pulsa Enter para continuar", 50, 110, InsWorld->minifont, 4, 6);
	}	
}

void Explication::update(double seconds_elapsed)
{
	//singelton
	Game* Insgame = Game::instance;
	sPlayer* Insplayer1 = &Game::instance->my_world->player[0];
	World* InsWorld = Game::instance->my_world;
	
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && InsWorld->nextImageIntro == false) //if key ENTER
	{
		InsWorld->nextImageIntro = true; 
		InsWorld->timeGameing = Insgame->time;
	}
	//press enter 2 times
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN) && InsWorld->nextImageIntro == true && (InsWorld->timeGameing+0.2) < Insgame->time) //if key ENTER
	{
		InsWorld->timeGameing = Insgame->time;

		Insgame->current_stage = Insgame->play_stage;
	}
}
//Player
void PlayStage::render(Image& framebuffer)
{
	Game* Insgame = Game::instance;

	Insgame->synth.stopAll();
	
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
	sPlayer* Insplayer1 = InstancePlayer();
	sPlayer* InsplayerAlpha = &Game::instance->my_world->player[0];
	
	if (Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2))
	{
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
	// If the ladder disappears, the character falls (gravity)
	if (InsWorld->level ==0 && InsWorld->objectEscalera == false && Insplayer1->pos.x >25 && Insplayer1->pos.x <= 41 && Insplayer1->pos.y > 25 && Insplayer1->pos.y < 90) 
	{
		Insplayer1->pos.y = 93; 
	}
	int currentAnimP1 = Insplayer1->moving ? (int(Insgame->time * Insplayer1->animation_velocity) % Insplayer1->animLenght) : 0;
	framebuffer.drawImage(Insplayer1->Implayer, Insplayer1->pos.x, Insplayer1->pos.y, Area(14 * currentAnimP1, 18 * (int)Insplayer1->dir, 14, 18));
	framebuffer.drawText(toString((int)(Insgame->time - InsWorld->timeGameing)), 1, 10, InsWorld->minifont, 4, 6);
	enterTheCave(Insplayer1);

}
sPlayer* PlayStage::InstancePlayer() 
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;


	if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
	{
		return &Game::instance->my_world->player[0];
	}
	if (Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2))
	{
		return &Game::instance->my_world->player[1];
	}
}
void PlayStage::renderCells(Image& framebuffer)
{
	World* InsWorld = Game::instance->my_world;
	GameMap* Insmap = Game::instance->my_world->map[InsWorld->level];
	int cs = InsWorld->tileset.width / 16;

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
			int screenx = (x * cs) + InsWorld->moviment.x; //place offset here if you want
			int screeny = (y * cs) + InsWorld->moviment.y;
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
	sPlayer* Insplayer1 = InstancePlayer();
	vector<Vector2> InsList = Game::instance->my_world->movPlayer1;
	int cont = Game::instance->my_world->contMov;

	//player's feet
	float x = Insplayer1->pos.x + 14;
	float y = Insplayer1->pos.y + 18;

	//bool object
	InsWorld->objectEscalera = false;
	InsWorld->objectsRock = 0;

	//LEVEL 1
	if (InsWorld->level == 0) 
	{
		//First player
		if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
		{
			// if it is on the platform
			if (x >= 118 && x <= 146 && y >= 107 && y <= 112) {
				//stairs
				framebuffer.drawImage(InsWorld->objects, 30, 40, Area(0, 0, 11, 72));
				//platform
				framebuffer.drawRectangle(115, 110, 20, 2, Color(147, 157, 148));
				InsWorld->objectEscalera = true;
			}
			else
				framebuffer.drawRectangle(115, 107, 20, 5, Color(147, 157, 148));
		}
		//Alpha player, second player
		if (InsList.size() != 0 && cont < InsList.size() && Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER && 
			Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2)){
			// playerAlpha feet
			float Alphx = InsList[cont].x + 14;
			float Alphy = InsList[cont].y + 18;
			//if it is on the platform
			if (Alphx >= 118 && Alphx <= 146 && Alphy >= 107 && Alphy <= 112 ||
				x >= 118 && x <= 146 && y >= 107 && y <= 112) {
				//stairs
				framebuffer.drawImage(InsWorld->objects, 30, 40, Area(0, 0, 11, 72));
				InsWorld->objectEscalera = true; 
				//platform
				framebuffer.drawRectangle(115, 110, 20, 2, Color(147, 157, 148));
			}
			else
				framebuffer.drawRectangle(115, 107, 20, 5, Color(147, 157, 148));
		}
	}
	//LEVEL 2
	if (InsWorld->level == 1)
	{
		//stairs
		framebuffer.drawImage(InsWorld->objects, 15, 88, Area(30, 0, 15, 27));
		//rock
		rock_and_Plataform(framebuffer, 30, 110, 50, 112, 30, 1); 
		rock_and_Plataform(framebuffer, 70, 110, 90, 112, 55, 2);
		rock_and_Plataform(framebuffer, 110, 110, 130, 112, 80, 3);
	}
}

void PlayStage::rock_and_Plataform(Image& framebuffer, int limitX, int limitY, int limitW, int limitH, int RoackX, int numRock)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	sPlayer* Insplayer1 = InstancePlayer();
	vector<Vector2> InsList = Game::instance->my_world->movPlayer1;
	int cont = Game::instance->my_world->contMov;
	float x = Insplayer1->pos.x + 14;
	float y = Insplayer1->pos.y + 18;
	//First player
	if (Insgame->time - InsWorld->timeGameing < TIME_GAME_PLAYER)
	{
		//if it is on the platform
		if (x >= limitX+3 && x <= limitW+11 && y >= limitY && y <= limitH) {
			framebuffer.drawImage(InsWorld->objects, RoackX, 26, Area(14, 0, 16, 33));
			framebuffer.drawRectangle(limitX, limitY, 20, 2, Color(147, 157, 148));
			InsWorld->objectsRock = numRock;

		}
		else
		{
			framebuffer.drawImage(InsWorld->objects, RoackX, 56, Area(14, 0, 16, 33));
			framebuffer.drawRectangle(limitX, 107, 20, 5, Color(147, 157, 148));
		}
	}
	//Alpha player, second player
	if (InsList.size() != 0 && cont < InsList.size() && Insgame->time - InsWorld->timeGameing >= TIME_GAME_PLAYER &&
		Insgame->time - InsWorld->timeGameing < (TIME_GAME_PLAYER * 2)) {
		float Alphx = InsList[cont].x + 14;
		float Alphy = InsList[cont].y + 18;
		//if it is on the platform
		if (Alphx >= limitX+3 && Alphx <= limitW+11 && Alphy >= limitY && Alphy <= limitH ||
			x >= limitX+3 && x <= limitW+11 && y >= limitY && y <= limitH) {
			framebuffer.drawImage(InsWorld->objects, RoackX, 26, Area(14, 0, 16, 33));
			framebuffer.drawRectangle(limitX, limitY, 20, 2, Color(147, 157, 148));
			InsWorld->objectsRock = numRock;
		}
		else
		{
			framebuffer.drawImage(InsWorld->objects, RoackX, 56, Area(14, 0, 16, 33));
			framebuffer.drawRectangle(limitX, 107, 20, 5, Color(147, 157, 148));
		}
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
	//LEVEL 1: You can only UP if you are in the position of the ladder and the platform is tight
	if (InsWorld->level == 0 && InsWorld->objectEscalera == true && target.x > 25 && target.x < 32 && target.y > 30)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
		{
			target.y -= Insplayer1->player_velocity * seconds_elapsed;
			Insplayer1->dir = eDIRECTION::UP;
			Insplayer1->moving = true;
		}	
	}
	if (InsWorld->level == 1 && target.x > 10 && target.x < 17 && target.y > 70)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
		{
			target.y -= Insplayer1->player_velocity * seconds_elapsed;
			Insplayer1->dir = eDIRECTION::UP;
			Insplayer1->moving = true;
		}
	}
	//LEVEL 1: You can only DOWN if you are in the position of the ladder and the platform is pressed

	if (InsWorld->level == 0 && InsWorld->objectEscalera == true && target.x > 25 && target.x < 32 && target.y > 20)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
		{
			target.y += Insplayer1->player_velocity * seconds_elapsed;
			Insplayer1->dir = eDIRECTION::DOWN;
			Insplayer1->moving = true;

		}
	}
	if (InsWorld->level == 1 && target.x > 10 && target.x < 17 && target.y > 60)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
		{
			target.y += Insplayer1->player_velocity * seconds_elapsed;
			Insplayer1->dir = eDIRECTION::DOWN;
			Insplayer1->moving = true;

		}
	}

	// if he's on the ladder he can't go to the right or to the left
	if ((InsWorld->level == 0 && (target.y < 35 || target.y > 92) )|| (InsWorld->level == 1 && (target.y < 70 || target.y > 92)) ) {
	
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
	
	}
	
	//JUMP 
	// If 0.2 seconds pass after pressing the space to jump
	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE) == false && Insplayer1->jump != 0.0f && Insgame->time > Insplayer1->jump + 0.2f)
	{
		target.y += Insplayer1->pixelToJump; // Devolver en la misma posici?n original
		Insplayer1->moving = true;
		Insplayer1->jump = 0.0f;
		
	}
	//You can only press the jump 1 time if you have not yet returned to the original position
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
	if (Input::wasKeyPressed(SDL_SCANCODE_W)) //if key W state: Win
	{
		Insgame->current_stage = Insgame->win_stage;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) //if key R state: intro
	{
		InsWorld->resetWorld();
		Insgame->current_stage = Insgame->intro_stage;

	}
	//collisions
	if (Insplayer1->isValid(target))
		Insplayer1->pos = target; 
}

void PlayStage::enterTheCave(sPlayer* Insplayer1)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	int cs = InsWorld->tileset.width / 16;
	//player's feet
	int x[2];
	int y = ((Insplayer1->pos.y + (18 * 1) - InsWorld->moviment.y) / cs);

	for (int i = 0; i < 2; i++)
	{
		x[i] = ((Insplayer1->pos.x + (14 * i) - InsWorld->moviment.x) / cs);
		sCell celda = InsWorld->map[InsWorld->level]->getCell(x[i], y);
		// If he enters the cave
		if (celda.type >= 14 && celda.type <= 17)
		{
			// goes to Level 2, if it passes through the cave and is at level 1
			if (InsWorld->level == 0 && InsWorld->nextLevel == true)
				Insgame->current_stage = Insgame->pause_stage;
			else { // Win state, if it is at level 2 and goes through the cave
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
	// If it does not reach the cave
	if (InsWorld->nextLevel == false)
	{
		InsWorld->movPlayer1.clear();
		InsWorld->contMov = 0;
		Insgame->current_stage = Insgame->game_over;
	}

}

//PauseLevel1to2
void PauseLevel1to2::render(Image& framebuffer)
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color(106, 151, 111));	//fills the image with one color

	framebuffer.drawText("Nivel 2", 55, 50, InsWorld->font);
	//audio next level
	Insgame->synth.playSample("data/level.wav", 0.2, false);

	if ((int)Insgame->time % 2)
		framebuffer.drawText("Pulsa Enter para continuar", 50, 110, InsWorld->minifont, 4, 6);

}

void PauseLevel1to2::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key ENTER
	{
		InsWorld->resetWorld();
		for (int i = 0; i < N_PLAYER; i++)
		{
			Game::instance->my_world->player[i] = sPlayer(InsWorld->playerReal); //incialitze player
		}
		InsWorld->level = 1;
		Insgame->current_stage = Insgame->play_stage;
	}
}
//GameOver
void GameOver::render(Image& framebuffer) 
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color(106, 151, 111));								//fills the image with one color

	framebuffer.drawText("HAS PERDIDO", 45, 50, InsWorld->font);
	//audio game_over
	Insgame->synth.playSample("data/game_over.wav", 0.2, false);
	if ((int)Insgame->time % 2)
		framebuffer.drawText("Pulsa Enter para continuar", 50, 110, InsWorld->minifont, 4, 6);
}

void GameOver::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key ENTER
	{
		InsWorld->resetWorld();
		Insgame->current_stage = Insgame->intro_stage;		
	}
}
//Win
void Win::render(Image& framebuffer)
{
	Game* Insgame = Game::instance; //singelton	
	World* InsWorld = Game::instance->my_world;

	framebuffer.fill(Color(106, 151, 111));								//fills the image with one color
	framebuffer.drawText("HAS GANADO", 45, 50, InsWorld->font);
	//audio win
	Insgame->synth.playSample("data/win.wav", 0.2, false);
	if((int)Insgame->time % 2 )
		framebuffer.drawText("Pulsa Enter para continuar", 50, 110, InsWorld->minifont, 4, 6);
}

void Win::update(double seconds_elapsed)
{
	Game* Insgame = Game::instance;
	World* InsWorld = Game::instance->my_world;
	if (Input::isKeyPressed(SDL_SCANCODE_RETURN)) //if key up
	{
		InsWorld->resetWorld();
		Insgame->current_stage = Insgame->intro_stage;
	}
}

//World
void World::resetWorld() 
{
	World* InsWorld = Game::instance->my_world;
	Game* Insgame = Game::instance;

	InsWorld->movPlayer1.clear();
	InsWorld->contMov = 0;
	InsWorld->timeGameing = Insgame->time;
	InsWorld->nextLevel = false;
	InsWorld->level = 0;
	InsWorld->nextImageIntro = false;

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
		}
	}

	delete[] cells; //always free any memory allocated!

	return map;
}

bool sPlayer::isValid(Vector2 positionPlayer)
{
	World* InsWorld = Game::instance->my_world;
	sButton* InsButton = &Game::instance->my_world->inicio; 
	int cs = InsWorld->tileset.width / 16;
	//pies del jugador
	int x[2]; 
	int y= ((positionPlayer.y + (18 * 1) - InsWorld->moviment.y) / cs);

	for (int i = 0; i < 2; i++)
	{
		x[i] = ((positionPlayer.x +(14 *i) - InsWorld->moviment.x) / cs);
	}	   
	for (int i = 0; i < 2; i++)
	{
		if (InsWorld->map[InsWorld->level]->getCell(x[i], y).type == FLOOR)
			return false;
		if (InsWorld->map[InsWorld->level]->getCell(x[i], y).type == PLATAFORM2 || InsWorld->map[InsWorld->level]->getCell(x[i], y).type == PLATAFORM)
			return false;
		
	}
	//LEVEL 2
	if (InsWorld->level == 1) 
	{
		for (int i = 0; i < 2; i++)
		{
			//rocas bajadas no puede pasar
			if (InsWorld->objectsRock == 0 && (((positionPlayer.y + 18) < 90 && (positionPlayer.y + 18) >= 56) &&
				((positionPlayer.x + (14 * i) >= 35 && (positionPlayer.x + (14 * i) <= 40))||
				(positionPlayer.x + (14 * i) >= 60 && (positionPlayer.x + (14 * i) <= 65)) || 
				(positionPlayer.x + (14 * i) >= 85 && (positionPlayer.x + (14 * i) <= 90)))))
				
				return false;

			if (InsWorld->objectsRock == 1 && (((positionPlayer.y + 18) < 90 && (positionPlayer.y + 18) >= 56) &&
				((positionPlayer.x + (14 * i) >= 60 && (positionPlayer.x + (14 * i) <= 65)) ||
					(positionPlayer.x + (14 * i) >= 85 && (positionPlayer.x + (14 * i) <= 90)))))
				return false;

			if (InsWorld->objectsRock == 2 && (((positionPlayer.y + 18) < 90 && (positionPlayer.y + 18) >= 56) &&
				((positionPlayer.x + (14 * i) >= 35 && (positionPlayer.x + (14 * i) <= 40)) ||
					(positionPlayer.x + (14 * i) >= 85 && (positionPlayer.x + (14 * i) <= 90)))))
				return false;

			if (InsWorld->objectsRock == 3 && (((positionPlayer.y + 18) < 90 && (positionPlayer.y + 18) >= 56) &&
				((positionPlayer.x + (14 * i) >= 35 && (positionPlayer.x + (14 * i) <= 40)) ||
					(positionPlayer.x + (14 * i) >= 60 && (positionPlayer.x + (14 * i) <= 65)))))
				return false;
		}	
	}
	//El personaje no puede salir del framework
	for (int i = 0; i < 2; i++)
	{
		if ((positionPlayer.y < 0 || (positionPlayer.y + 18) > InsButton->h_framework ||
			((positionPlayer.x + (14 * i) < 0 || (positionPlayer.x + (14 * i) >= InsButton->w_framework)))))
			return false;
	}

	return true;
}

