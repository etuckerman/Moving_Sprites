#pragma once
#include "SFML/Graphics.hpp"

//dimensions in 2D that are whole numbers
struct Dim2Di
{
	int x, y;
};

//dimensions in 2D that are floating point numbers
struct Dim2Df
{
	float x, y;
};


/*
A box to put Games Constants in.
These are special numbers with important meanings (screen width,
ascii code for the escape key, number of lives a player starts with,
the name of the title screen music track, etc.
*/
namespace GC
{
	//game play related constants to tweak
	const Dim2Di SCREEN_RES{ 800,600 };

	const char ESCAPE_KEY{ 27 };

	const float ROCK_MIN_DIST = 2.15f;	//stop them getting too close
	const int NUM_ROCKS = 50;		//how many to place
	const int PLACE_TRIES = 10;		//how many times to try and place

	const float BACK_SPEED = 10;		//start speed for scrolling
	const float BACK_LAYER_SPEEDINC = 7;//each layer is a bit faster towards the front
}

//all the textures we need
struct Textures
{
	//give each texture an ID
	typedef enum {
		BACK0 = 0, //large backdrop sprites
		BACK1 = 1,
		BACK2 = 2,
		BACK3 = 3,
		BACK4 = 4,
		BACK5 = 5,
		BACK6 = 6,
		BACK7 = 7,
	} Tex;
	static const int MAX_TEXTURES = 8;
	sf::Texture tex[MAX_TEXTURES];					//a variable to hold each texture
	float bgndSpds[MAX_TEXTURES] = { 0,0,0,0,0,0,0,0 };	//keep track of the scroll position of each bgnd layer

	//given a texture ID, get the actual texture
	sf::Texture& Get(Tex id) {
		return tex[static_cast<int>(id)];
	}

	/*
	LoadTextures- load all textures
	precon		- no textures loaded
	postcon		- all textures loaded
	*/
	void LoadTextures();

	/*
	ScrollBgnd	- scroll a texture as a screen background
	window		- rendering handle
	inc			- how fast to scroll
	precon		- the texture is set to repeat and inc is a time adjusted smooth value
				- note that the increment cannot be float
	postcon		- teh textures scrolls based on vlaue of inc
	*/
	void ScrollBgnd(Tex texId, sf::RenderWindow& window, int inc);

	/*
	DrawBgnd	- draw all background layers
	elased		- time in secs since last call to this function
	window		- for drawing
	precon		- window and textures are ready
	postcon		- all 7 layers of the background drawn full screen
				  and each one scrolling at a different speed
	*/
	void DrawBgnd(float elapsed, sf::RenderWindow& window);

};