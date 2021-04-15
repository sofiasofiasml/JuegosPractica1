#include "game.h"
#include "mygame.h"
#include "utils.h"
#include "input.h"
#include "image.h"


void IntroStage::render(Image& framebuffer, World* my_world) {

		
	framebuffer.fill(Color::CYAN);								//fills the image with one color
//framebuffer.drawLine( 0, 0, 100,100, Color::RED );		//draws a line
//framebuffer.drawImage( sprite, 0, 0 );					//draws full image
	framebuffer.drawImage(my_world->sprite, 0, 0, framebuffer.width, framebuffer.height);			//draws a scaled image
	//framebuffer.drawImage( sprite, 0, 0, Area(0,0,14,18) );	//draws only a part of an image
	framebuffer.drawText("4 DIMENSION", framebuffer.width / 4, framebuffer.height / 5, my_world->font);
	//draws some text using a bitmap font in an image (assuming every char is 7x9)
	//framebuffer.drawText(toString(time), 1, 10, minifont, 4, 6);	//draws some text using a bitmap font in an image (assuming every char is 4x6)
	//void drawText( std::string text, int x, int y, const Image& bitmapfont, int font_w = 7, int font_h = 9, int first_char = 32);


	//audio intro
	Game::instance->synth.playSample("data/lassambience1.wav", 2, true);
	

	framebuffer.drawRectangle(framebuffer.width / 2 - 13, framebuffer.height / 2 + framebuffer.height / 3 - 3, 29, 11, (255, 80, 100));
	framebuffer.drawRectangle(framebuffer.width / 2 - 12, framebuffer.height / 2 + framebuffer.height / 3 - 2, 27, 9, Color::RED);
	framebuffer.drawText("Inicio", framebuffer.width / 2 - 10, framebuffer.height / 2 + framebuffer.height / 3, my_world->minifont, 4, 6);

}

//Vector2* MyGame::ValueFrameworkToWindow(float frameworkX, float frameworkY, Image& framebuffer) {
//	int window_width, window_height;
//	SDL_Window* window;
//	SDL_GetWindowSize(window, &window_width, &window_height);
//	Vector2* WindowVector = new Vector2(); 
//
//	WindowVector->x = (frameworkX * window_width) / framebuffer.width; 
//	WindowVector->y = (frameworkY * window_height) / framebuffer.height; 
//
//	return WindowVector; 
//}

