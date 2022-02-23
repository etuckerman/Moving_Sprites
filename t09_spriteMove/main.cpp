#include <assert.h>
#include <string>
#include <math.h> 
#include <stdio.h>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "Game.h"

using namespace sf;
using namespace std;


/*a2 + b2 = c2 see how close two 2D positions are, see if they are colliding
Precon – pos1+pos2 are 2D positions representing objects moving around
IN: pos1, pos2 – a pair of 2D coordinates
IN: minDist – if they two coordinates are closer or as close as this->collision
RETURNS: true if collision detected
Postcon – two circles are either colliding or not
*/
bool CircleToCircle(const Vector2f& pos1, const Vector2f& pos2, float minDist)
{
	float dist = (pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y);
	dist = sqrtf(dist);
	return dist <= minDist;
}

void Textures::LoadTextures()
{
	stringstream ss;
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		ss.str("");
		ss << "data/backgroundLayers/mountains01_00" << i << ".png";

		Texture& t = Get(static_cast<Tex>(i));
		if (!t.loadFromFile(ss.str()))
			assert(false);
		t.setRepeated(true);
	}
}

void Textures::ScrollBgnd(Tex texId, sf::RenderWindow& window, int inc)
{
	sf::Sprite spr(Get(texId));
	sf::IntRect scroll = spr.getTextureRect();
	scroll.left += inc;
	spr.setTextureRect(scroll);
	spr.setScale(window.getSize().x / (float)scroll.width, window.getSize().y / (float)scroll.height);
	spr.setPosition(0, 0);
	window.draw(spr);
}


void Textures::DrawBgnd(float elapsed, sf::RenderWindow& window)
{
	//background order of sprites
	Textures::Tex ids[MAX_TEXTURES]{
		Textures::BACK7,
		Textures::BACK5,
		Textures::BACK4,
		Textures::BACK3,
		Textures::BACK2,
		Textures::BACK1,
		Textures::BACK0,
		Textures::BACK6,
	};

	float spd = GC::BACK_SPEED * elapsed;
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		bgndSpds[i] += spd;
		ScrollBgnd(ids[i], window, (int)bgndSpds[i]);
		spd += GC::BACK_LAYER_SPEEDINC * elapsed;
	}
}

/*
* Gameobject could be...
*	1. ship - flies around under player control
*	2. asteroid - flies in randomly from offscreen and is an obstacle
*	3. bullet - shot by the player, goes right in a straight line
*/
struct GameObject {
	Sprite spr;
	enum class ObjT { Ship, Rock, Bullet, HealthPickup };
	ObjT myType = ObjT::Ship;
	int radius = 0;
	bool colliding = false;
	bool alive = true;

	void Init(RenderWindow& window, Texture& tex, ObjT type) {
		myType = type;
		switch (myType)
		{
		case ObjT::Ship:
			InitShip(window, tex);
			break;
		case ObjT::Rock:
			InitRock(window, tex);
			break;
		default:
			assert(false);
		}
	}
	void Update(const Vector2u& screenSz, float elapsed) {
		//let each type of object move around

		switch (myType) {
		case ObjT::Ship:
			UpdateShip(screenSz, elapsed);
			break;
		case ObjT::Rock:
			UpdateRock(screenSz, elapsed);
			break;
		default:
			assert(false);
		}


	//see if it hit anything
	}
	void Render(RenderWindow& window) {
		if (alive) {
			window.draw(spr);

			sf::CircleShape debugCircle;
			debugCircle.setPosition(spr.getPosition());
			debugCircle.setRadius(radius);
			if (colliding)
				debugCircle.setOrigin(50.f, 50.f);
			else
				debugCircle.setOutlineColor(sf::Color::Transparent);
			debugCircle.setOrigin(radius, radius);
			debugCircle.setFillColor(sf::Color::Transparent);
			debugCircle.setOutlineThickness(3);
			window.draw(debugCircle);
		}
	}


	//custom functions - ship
	void InitShip(RenderWindow& window, Texture& tex) {
		//Sprite spr(tex);
		spr.setTexture(tex);
		IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setScale(0.1f, 0.1f);
		spr.setRotation(90);
		spr.setPosition(window.getSize().x * 0.05f, window.getSize().y / 2.f);
		radius = 30;
	}

	void UpdateRock(const Vector2u& screenSz, float elapsed) {
		if (alive) {

			Vector2f pos = spr.getPosition();
			const float ROCK_SPEED = 50.f;
			pos.x -= ROCK_SPEED * elapsed;
			spr.setPosition(pos);

			if (pos.x < -50)				//stops asteroids spawning ON screen
				pos.x += screenSz.x + 100; 

			spr.setPosition(pos);
		}
	}


	void UpdateShip(const Vector2u& screenSz, float elapsed) {
		Vector2f pos = spr.getPosition();
		const float SPEED = 250.f;
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (pos.y > (screenSz.y * 0.05f))
				pos.y -= SPEED * elapsed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (pos.y < (screenSz.y * 0.95f))
				pos.y += SPEED * elapsed;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (pos.x > (screenSz.x * 0.05f))
				pos.x -= SPEED * elapsed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (pos.x < (screenSz.x * 0.95f))
				pos.x += SPEED * elapsed;
		}

		spr.setPosition(pos);
	}

	float GetRandomRange(float min, float max)
	{
		//alpha =  (float)rand()/RAND_MAX
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		return 1.f;
	}

	int GetRandomRange(int min, int max)
	{
		float alpha = GetRandomRange((float)min, (float)max);
		return round(alpha);
	}

	//custom functions - rock
	void InitRock(RenderWindow& window, Texture& tex) {
		const Dim2Df ROCK_RAD{ 10.f,40.f };
		spr.setTexture(tex);
		IntRect texR{ 0,0,92,92 };
		spr.setTextureRect(texR);
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		//spr.setScale(1.f, 1.f);
		float scale = GetRandomRange(0.75f, 1.5f);//0.75f * (radius / 25.f); 
		spr.setScale(scale, scale);
		radius = 35 * scale;// ROCK_RAD.x + (float)(rand() % (int)ROCK_RAD.y);

		//spr.setRotation(90);
		//spr.setPosition(window.getSize().x * 0.5f, window.getSize().y / 2.f);

		int x = rand() % GC::SCREEN_RES.x;
		int y = rand() % GC::SCREEN_RES.y;
		spr.setPosition(x, y);

		//radius = 35;
	}
};





bool LoadTexture(const string& file, Texture& tex)
{
	if (tex.loadFromFile(file))
	{
		tex.setSmooth(true);
		return true;
	}
	assert(false);
	return false;
}



int main()
{
	// Create the main window
	RenderWindow window(VideoMode(GC::SCREEN_RES.x, GC::SCREEN_RES.y), "Ship Game");

	srand(0); // (time(0));

	Texture shipTex, assTex;
	LoadTexture("data/ship.png", shipTex);
	LoadTexture("data/asteroid.png", assTex);

	/*Ship ship;
	ship.Init(window, shipTex);

	Asteroid asteroid;
	asteroid.Init(window, assTex);*/

	const int MAX_OBJS = 31;
	GameObject objects[MAX_OBJS];
	objects[0].Init(window, shipTex, GameObject::ObjT::Ship);
	for(int i = 1; i<31; ++i)
		objects[i].Init(window, assTex, GameObject::ObjT::Rock);


	/*GameObject ship;
	ship.Init(window, shipTex, GameObject::ObjT::Ship);
	GameObject asteroid;
	asteroid.Init(window, assTex, GameObject::ObjT::Rock);*/

	Clock clock;

	// Start the game loop 
	while (window.isOpen())
	{
		// Process events
		Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::TextEntered)
			{
				if (event.text.unicode == GC::ESCAPE_KEY)
					window.close();
			}
		}

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();
		Vector2u screenSz = window.getSize();
		//ship.Update(screenSz, elapsed);
		for (int i = 0; i < MAX_OBJS; ++i)
			objects[i].Update(screenSz, elapsed);

		float minDist = objects[0].radius + objects[0].radius;
		if (CircleToCircle(objects[0].spr.getPosition(), objects[1].spr.getPosition(), minDist))
		{
			objects[0].colliding = true;
			objects[1].colliding = true;
	
		}
		else
		{
			objects[0].colliding = false;
			objects[1].colliding = false;
		}


		// Clear screen
		window.clear();

		//ship.Render(window);
		//asteroid.Render(window);
		for (int i = 0; i < MAX_OBJS; ++i)
			objects[i].Render(window);

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}
