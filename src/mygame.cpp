#include "game.h"
#include "mygame.h"
#include "utils.h"
#include "input.h"
#include "image.h"

#include <cmath>

void IntroStage::render(Image& framebuffer) {

	Game* game = Game::instance; //singelton	
	framebuffer.fill(Color::CYAN);								//fills the image with one color
	//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
	//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
	framebuffer.drawImage(Game::instance->my_world->sprite, 0, 0, framebuffer.width, framebuffer.height);			//draws a scaled image
	
	framebuffer.drawText("4 DIMENSION", framebuffer.width / 4, framebuffer.height / 5, Game::instance->my_world->font);
	//draws some text using a bitmap font in an image (assuming every char is 7x9)
	//framebuffer.drawText(toString(Game::instance->time), 1, 10, my_world->minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	
    //void drawText( std::string text, int x, int y, const Image& bitmapfont, int font_w = 7, int font_h = 9, int first_char = 32);


	//audio intro
	Game::instance->synth.playSample("data/lassambience1.wav", 2, true);

	bottonIntro(framebuffer);
}

void IntroStage::bottonIntro(Image& framebuffer)
{
	//coordenate framework to window 
	Game* game = Game::instance; //singelton
	
	game->my_world->inicio.WsquareIni_PointW = (game->my_world->inicio.FsquareBig_PointW * game->window_width) / framebuffer.width;
	game->my_world->inicio.WsquareIni_PointH = (game->my_world->inicio.FsquareBig_PointH * game->window_height) / framebuffer.height;
	game->my_world->inicio.WsquareFin_W = ((game->my_world->inicio.FsquareBig_W + game->my_world->inicio.FsquareBig_PointW) * game->window_width) / framebuffer.width;
	game->my_world->inicio.WsquareFin_H = ((game->my_world->inicio.FsquareBig_H + game->my_world->inicio.FsquareBig_PointH)* game->window_height) / framebuffer.height;

	//rectangle big
	framebuffer.drawRectangle(game->my_world->inicio.FsquareBig_PointW, game->my_world->inicio.FsquareBig_PointH, game->my_world->inicio.FsquareBig_W, game->my_world->inicio.FsquareBig_H, Color (0, 0, 0));
	//rectangle small
	framebuffer.drawRectangle(game->my_world->inicio.FsquareBig_PointW + 1, game->my_world->inicio.FsquareBig_PointH + 1, game->my_world->inicio.FsquareBig_W - 2, game->my_world->inicio.FsquareBig_H - 2, Color (86, 130, 91));

	Vector2 v2_mouse = Input::mouse_position;

	// Change button color if mouse position is button
	if (v2_mouse.x > game->my_world->inicio.WsquareIni_PointW && v2_mouse.y > game->my_world->inicio.WsquareIni_PointH && v2_mouse.x < game->my_world->inicio.WsquareFin_W && v2_mouse.y < game->my_world->inicio.WsquareFin_H)
	{
		framebuffer.drawRectangle(game->my_world->inicio.FsquareBig_PointW + 1, game->my_world->inicio.FsquareBig_PointH + 1, game->my_world->inicio.FsquareBig_W - 2, game->my_world->inicio.FsquareBig_H - 2, Color (129, 194, 137));
	}

	// Text to button 
	framebuffer.drawText("Inicio", framebuffer.width / 2 - 10, framebuffer.height / 2 + framebuffer.height / 3, game->my_world->minifont, 4, 6);

}


void IntroStage::update(double seconds_elapsed)
{
	Game* game = Game::instance;
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

	
	if (Input::mouse_state==1 &&(Input::mouse_position.x> game->my_world->inicio.WsquareIni_PointW && Input::mouse_position.y > game->my_world->inicio.WsquareIni_PointH && Input::mouse_position.x< game->my_world->inicio.WsquareFin_W && Input::mouse_position.y < game->my_world->inicio.WsquareFin_H))
	{
		game->current_stage = game->play_stage;
		game->synth.stopAll();
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

void PlayStage::render(Image& framebuffer)
{
	Game* game = Game::instance;
	framebuffer.drawImage(game->my_world->player1.Implayer, game->my_world->player1.pos.x, game->my_world->player1.pos.y, Area(0, 0, 14, 18));
	framebuffer.drawImage(game->my_world->player2.Implayer, game->my_world->player2.pos.x, game->my_world->player2.pos.y+10, Area(0, 0, 14, 18));

}
void PlayStage::update(double seconds_elapsed) { //movement of the character
	Game* game = Game::instance;
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		game->my_world->player1.pos.y -= game->my_world->player1.player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		game->my_world->player1.pos.y += game->my_world->player1.player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		game->my_world->player1.pos.x += game->my_world->player1.player_velocity * seconds_elapsed;
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		game->my_world->player1.pos.x -= game->my_world->player1.player_velocity * seconds_elapsed;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_R)) //if key Z was pressed state= intro
	{
		game->current_stage = game->intro_stage;
	}
};
