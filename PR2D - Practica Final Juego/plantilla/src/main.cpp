
#include <direct.h>

#include "Vec2.h"     // Archivo de cabecera local
#include "Sprite.h"   // Archivo de cabecera local
#include "Collider.h" // Archivo de cabecera local
#include "Font.h"     // Archivo de cabecera local
#include "World.h"

#include <glfw3.h>    // Biblioteca externa
#include <iostream>   // Biblioteca estándar de C++
#include <cstdlib>

#define LITE_GFX_IMPLEMENTATION
#include <litegfx.h>  // Biblioteca externa LiteGFX

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // Biblioteca externa
#include <vector>

#include <pugixml.hpp>

using namespace std;

ltex_t* loadTexture(const char* filename) {

	int x; //puntero con el ancho de la imagen
	int y; //punter con alto de la imagen
	int comp; //puntero con numero de componentes del buffer
	int req_comp = 4; //Componentes de cada pixel, 4 para que sea RGBA

	unsigned char* img = stbi_load(filename, &x, &y, &comp, req_comp);
	ltex_t* texture = ltex_alloc(x, y, 1);

	// Volcar los píxeles en la textura
	ltex_setpixels(texture, img);
	// Liberar la memoria de la imagen
	stbi_image_free(img);
	return texture;
}

Vec2 getRandomDirection() {
	int direction = std::rand() % 4;
	Vec2 amount;
	switch (direction) {
	case 0: amount.x = 50; break;  // Derecha
	case 1: amount.x = -50; break; // Izquierda
	case 2: amount.y = 50; break;  // Abajo
	case 3: amount.y = -50; break; // Arriba
	}
	return amount;
}

void moveSpriteRandomly(World& world, Sprite& sprite, Vec2& currentDirection, float deltaTime) {
	Vec2 amount = currentDirection * deltaTime;

	// Intentar mover el sprite en la dirección actual
	if (world.moveSprite(sprite, amount)) {
		// Si hay colisión, cambiar de dirección
		currentDirection = getRandomDirection();
	}
}

void moveCharacter(World& world, Sprite& sprite, ltex_t* texture, ltex_t* texture2, float deltaTime, GLFWwindow* window)
{
	//Personaje:
	Vec2 amount;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		sprite.setTexture(texture, 2, 1);
		sprite.setFps(2);
		amount.x -= 100 * deltaTime;

	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		sprite.setTexture(texture, 2, 1);
		sprite.setFps(2);
		amount.x += 100 * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		sprite.setTexture(texture, 2, 1);
		sprite.setFps(2);
		amount.y -= 100 * deltaTime;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		sprite.setTexture(texture, 2, 1);
		sprite.setFps(2);
		amount.y += 100 * deltaTime;
	}
	else
	{
		sprite.setTexture(texture2, 1, 1);
		sprite.setFps(1);
	}
	world.moveSprite(sprite, amount);
}

void generateLives(World& world, int livesNum, std::vector<Sprite*>& lives)
{
	Vec2 livePos(0, 16);
	ltex_t* tLiveTexture = loadTexture("data/heart.png");

	for (int i = 0; i <= livesNum; ++i)  // Cambié a `livesNum` para que puedas generar el número de vidas que quieras
	{
		Sprite* sprite = new Sprite(4, tLiveTexture, 1, 1);
		Vec2 scale(0.05, 0.05);
		sprite->setScale(scale);
		sprite->setFps(1);
		livePos.x += 50;
		sprite->setPosition(livePos);
		sprite->setBlend(BLEND_ALPHA);
		sprite->setColor(1, 1, 1, 1);
		lives.push_back(sprite);  // Modificación sobre el vector original
		world.addSprite(*sprite);
	}
}

void generateMonsters(World& world, std::vector<Sprite*>& monster, Vec2 vMonsterPos, std::vector<Vec2>& currentDirections)
{
	Vec2 currentDirection(0, -50);
	ltex_t* tMonsterTexture = loadTexture("data/monster.png");

	for (int i = 0; i < 10; ++i)
	{
		Sprite* sprite = new Sprite(1, tMonsterTexture, 3, 1);
		sprite->setFps(3);
		sprite->setPosition(vMonsterPos);
		sprite->setBlend(BLEND_ALPHA);
		sprite->setColor(1, 1, 1, 1);
		sprite->setCollisionType(COLLISION_PIXELS);
		monster.push_back(sprite);  // Modificación sobre el vector original
		world.addSprite(*sprite);
		currentDirections.push_back(currentDirection);  // Añadimos la dirección al vector
	}
}

int main() {

	//--------------------VARIABLES--------------------------//
		//Tamaño de la ventana:
	Vec2 vWindow_scale(1024, 1024);

		//Camera Pos:
	Vec2 vCameraPos(0, 0);

		//Velocidad de desplazamiento:
	int iSpeed = 0;

		//Posicion de la abeja:
	Vec2 vCharInitPos(50, 100);
	Vec2 vMonsterPos(1024 / 2, 1024 / 2 - 30);

		//Score:
	int totalScore = 45;
		// Angulo:
	float fMovement_Angle = 32.0f;
	float fAngle = 0;

		//Tiempo
	float deltaTime = 0.0f;
	float previousTime = 0.0f;

	//--------------------CONFIGURACIÓN INICIAL--------------------------//

		//Inicializamos glfw:
	if (!glfwInit())
	{
		//Fallo al inicializar:
		cerr << "Error: No se pudo inicializar GLFW" << endl;
		return -1;
	}

	//Creamos ventana:
	GLFWwindow* window = glfwCreateWindow(vWindow_scale.x, vWindow_scale.y, "P2D_Practica1", NULL, NULL);
	if (!window)
	{
		//Fallo al crear ventana:
		cerr << "Error: No se pudo crear la ventana" << endl;
		glfwTerminate();
		return -1;
	}

	//Asociamos contexto de openGL a la ventana:
	glfwMakeContextCurrent(window);

	//Que LiteGFX se inicie:
	lgfx_setup2d(vWindow_scale.x, vWindow_scale.y);

	//--------------------CONFIGURACIÓN MUNDO--------------------------//

	ltex_t* background1 = loadTexture("data/floor.jpg");

	World world(0.5, 0.5, 0.5, background1);
	world.setScrollRatio(0, 1.0f);
	

	//--------------------CONFIGURACIÓN TEXTURAS--------------------------//
	//Vida:
	int livesNum = 2;
	std::vector<Sprite*> lives;
	generateLives(world, livesNum, lives);

	//Personaje
	ltex_t* tCharacterTexture = loadTexture("data/characterIdle.png");
	ltex_t* tCharacterRunTexture = loadTexture("data/characterRun.png");
	Sprite sCharacterSprite(0, tCharacterTexture, 1, 1);
	sCharacterSprite.setFps(1);
	sCharacterSprite.setPosition(vCharInitPos);
	sCharacterSprite.setBlend(BLEND_ALPHA);
	sCharacterSprite.setColor(1, 1, 1, 1);
	sCharacterSprite.setCollisionType(COLLISION_PIXELS);
	
	int score = 0;  // Puntuación
	sCharacterSprite.setUserData(new int(score));

	world.addSprite(sCharacterSprite);
	
	//Monstruos:
	std::vector<Sprite*> monsters;
	std::vector<Vec2> currentDirections;
	generateMonsters(world, monsters, vMonsterPos, currentDirections);

	//--------------------CONFIGURACIÓN FUENTES--------------------------//

	Font* points = Font::load("data/SFSlapstickComic.ttf", 28);
	Vec2 vFontPos(30, 1024/2-30);
	points->setPosition(vFontPos);

	//--------------------CONFIGURACIÓN MAPA--------------------------//
	world.loadMap("data/map.tmx", 1);
	Vec2 mapSize = world.getMapSize();
	
	//--------------------BUCLE PRINCIPAL--------------------------//

	
	bool gameOn = true;
	while (!glfwWindowShouldClose(window))
	{
		//Negar las Ys
		// Configuramos viewport:
		int iWidth, iHeight;
		glfwGetFramebufferSize(window, &iWidth, &iHeight);
		lgfx_setviewport(0, 0, iWidth, iHeight);
		Vec2 vCenterPosition(iWidth/2, iHeight/2);
		Vec2 vScreenSize(iWidth, iHeight);

		//Actualizamos el origen
		world.setCameraPosition(vCameraPos);
		/*lgfx_setorigin(vCameraPos.x, vCameraPos.y);*/
		
		// Actualizamos el tiempo:
		double fCurrentTime = glfwGetTime();
		deltaTime = fCurrentTime - previousTime;
		previousTime = fCurrentTime;

		////// Actualizar título de la ventana:
		int* retrievedScore = static_cast<int*>(sCharacterSprite.getUserData());
		const int bufferSize = 100;
		//char* title = new char[bufferSize];
		//sprintf_s(title, bufferSize, "Score: %i", *retrievedScore);
		glfwSetWindowTitle(window, "Final 2D Game");

		// Leer posición del ratón:
		/*double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		Vec2 vMousePos(mouseX, mouseY);*/

		// Limpiar backbuffer:
		lgfx_clearcolorbuffer(0, 0, 0);

		if (gameOn)
		{
			//Personaje
			moveCharacter(world, sCharacterSprite, tCharacterRunTexture, tCharacterTexture, deltaTime, window);

			//Abejas:
			for (int i = 0; i < 10; i++)
			{
				moveSpriteRandomly(world, *monsters[i], currentDirections[i], deltaTime);
				if (sCharacterSprite.collides(*monsters[i]))
				{
					world.removeSprite(*lives[livesNum]);
					livesNum -= 1;
					monsters[i]->setPosition(vMonsterPos);
					if (livesNum == -1)
					{
						gameOn = false;
					}
				}
			}
		}

		//Mundo
		world.update(deltaTime);
		world.draw(vScreenSize);

		//Fuente:
		retrievedScore = static_cast<int*>(sCharacterSprite.getUserData());
		

		//Fin del juego y volver a intentar:
		if (totalScore - *retrievedScore == 0)
		{
			points->setColor(1,0,0,1);
			gameOn = false;
			float p = points->getHeight()/2;
			vFontPos.x = vCenterPosition.x - points->getTextSize("¡Felicidades, has acabado el juego!").x / 2;
			vFontPos.y = vCenterPosition.y - p;
			points->draw("¡Felicidades, has acabado el minijuego!", vFontPos);
			
			
			gameOn = false;
		}
		else
		{
			char* charScore = new char[bufferSize];
			sprintf_s(charScore, bufferSize, "%i", totalScore - *retrievedScore);
			points->draw(charScore, points->getPosition());
		}

		if (!gameOn && livesNum <= -1)
		{
			float p = points->getHeight() / 2;
			vFontPos.x = vCenterPosition.x - points->getTextSize("¡Felicidades, has acabado el juego!").x / 2;
			vFontPos.y = vCenterPosition.y - p;
			points->draw("Una pena, ¡Vuelvelo a intentar!", vFontPos);
		}
		// Cambiar buffers:
		glfwSwapBuffers(window);
		// Procesar eventos:
		glfwPollEvents();
	}

	//ltex_free(tBeeTexture);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
