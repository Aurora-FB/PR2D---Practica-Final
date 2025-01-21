
#include "World.h"

#include <vector>
#include <algorithm>
#include <litegfx.h>
#include <iostream>
#include <stb_image.h>
#include <pugixml.hpp>

World::World(float clearRed,float clearGreen,float clearBlue,const ltex_t* back0,const ltex_t* back1,const ltex_t* back2,const ltex_t* back3):
	clearRed(clearRed), clearGreen(clearGreen), clearBlue(clearBlue),cameraPos(0.0f, 0.0f)
{
	backgrounds[0] = back0;
	backgrounds[1] = back1;
	backgrounds[2] = back2;
	backgrounds[3] = back3;

	for (size_t i = 0; i < 4; ++i) {
		scrollRatios[i] = 1.0f;
		scrollSpeeds[i] = Vec2(0.0f, 0.0f);
	}
}

float World::getClearRed() const
{
	return clearRed;
}

float World::getClearGreen() const
{
	return clearGreen;
}

float World::getClearBlue() const
{
	return clearBlue;
}

const ltex_t* World::getBackground(size_t layer) const
{
	if (layer < 4) return backgrounds[layer];
	return nullptr;
}

float World::getScrollRatio(size_t layer) const
{
	if (layer < 4) return scrollRatios[layer];
	return 1.0f;
}

void World::setScrollRatio(size_t layer, float ratio)
{
	if (layer < 4) scrollRatios[layer] = ratio;
}

const Vec2& World::getScrollSpeed(size_t layer) const
{
	if (layer < 4) 
	{
		return scrollSpeeds[layer];
	}
	static Vec2 defaultSpeed(0.0f, 0.0f);
	return defaultSpeed;
}

void World::setScrollSpeed(size_t layer, const Vec2& speed)
{
	if (layer < 4) scrollSpeeds[layer] = speed;
}

const Vec2& World::getCameraPosition() const
{
	return cameraPos;
}
void World::setCameraPosition(const Vec2& pos)
{
	cameraPos = pos;
}

void World::addSprite(Sprite& sprite)
{
	sprites.push_back(&sprite);
}

void World::removeSprite(Sprite& sprite)
{
	// Buscar el sprite por su id en el vector sprites
	auto it = std::find_if(sprites.begin(), sprites.end(), [&sprite](Sprite* s) {
		return s->getId() == sprite.getId(); // Comparar por id
		});

	if (it != sprites.end()) // Si el sprite con el id correspondiente se encuentra
	{
		delete* it;  // Si eres dueño del objeto `Sprite`, libéralo
		sprites.erase(it); // Eliminar el puntero del vector
		std::cout << "Sprite con ID " << sprite.getId() << " eliminado correctamente.\n";
	}
	else
	{
		std::cout << "Error: Sprite con ID " << sprite.getId() << " no encontrado en el vector.\n";
	}
}

inline std::string extractPath(const std::string& filename)
{
	std::string filenameCopy = filename;
	while (filenameCopy.find("\\") != std::string::npos)
	{
		filenameCopy.replace(filenameCopy.find("\\"), 1, "/");
	}
	filenameCopy = filenameCopy.substr(0, filenameCopy.rfind('/'));
	if (filenameCopy.size() > 0) filenameCopy += "/";
	return filenameCopy;
}

Vec2 World::getMapSize() const
{
	return mapSize;
}

bool World::loadMap(const char* filename, uint16_t firstColId)
{
	ltex_t* texture = nullptr;
	int tWidth = 0;  // Ancho de cada tile en píxeles
	int tHeight = 0; // Alto de cada tile en píxeles
	std::vector<std::vector<int>> tileLayers;
	int tColumns = 0; // Número de columnas en el tileset
	int tRows = 0;    // Número de filas en el tileset

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);

	if (!result) {
		std::cerr << "Error loading map file: " << result.description() << std::endl;
		return false;
	}

	// Obtener la ruta base del archivo
	std::string basePath = extractPath(filename);

	// Leer el elemento raíz <map>
	pugi::xml_node mapNode = doc.child("map");
	if (!mapNode) {
		std::cerr << "No <map> element found in TMX file." << std::endl;
		return false;
	}

	// Leer atributos de mapa
	int mapWidth = mapNode.attribute("width").as_int();
	int mapHeight = mapNode.attribute("height").as_int();
	Vec2 mSize(mapWidth, mapHeight);
	mapSize = mSize;
	tWidth = mapNode.attribute("tilewidth").as_int();
	tHeight = mapNode.attribute("tileheight").as_int();

	// Leer tilesets
	for (pugi::xml_node tilesetNode : mapNode.children("tileset")) {
		int firstGid = tilesetNode.attribute("firstgid").as_int();

		//Dirección de la imagen:
		const char* source = tilesetNode.child("image").attribute("source").as_string();
		std::string tilesetPathStr = basePath + source;
		const char* tilesetPath = tilesetPathStr.c_str();

		//Creación de la textura:
		int x;
		int y;
		int comp;
		int req_comp = 4;
		unsigned char* img = stbi_load(tilesetPath, &x, &y, &comp, req_comp);
		texture = ltex_alloc(x, y, 1);
		ltex_setpixels(texture, img);
		stbi_image_free(img);

		//Columnas y filas:
		tColumns = tilesetNode.attribute("columns").as_int();
		tRows = tilesetNode.attribute("tilecount").as_int() / tilesetNode.attribute("columns").as_int();
	}

	// Leer capas de tiles
	for (pugi::xml_node layerNode : mapNode.children("layer")) {
		int layerWidth = layerNode.attribute("width").as_int();
		int layerHeight = layerNode.attribute("height").as_int();
		pugi::xml_node dataNode = layerNode.child("data");

		if (!dataNode) {
			std::cerr << "Error: No se encontró el nodo <data> en la capa" << std::endl;
			continue;
		}

		// Crear una matriz para la capa actual
		std::vector<Tile*> layerData(layerWidth * layerHeight, nullptr);

		int x = 0;
		int y = 0;

		int tileIndex = 0;
		for (pugi::xml_node tileNode : dataNode.children("tile")) {
			int gid = tileNode.attribute("gid").as_int();
			if (gid > 0) {
				int tileId = gid - firstColId;
				int row = tileId / tColumns;
				int column = tileId % tColumns;
				Vec2 pos(x, y);
				Vec2 size(tWidth, tHeight);
				Tile* tile = new Tile(tileId, pos, texture, size, column, row);
				layerData[tileIndex] = tile;
			}
			x += 32;
			if (x >= mapSize.x * mapSize.y)
			{
				y += 32;
				x = 0;
			}
			tileIndex++;
		}
		tiles.push_back(layerData);
	}

	return true;
}

int World::checkCollision(Sprite& sprite, std::vector<std::vector<Tile*>> _tiles)
{

	for (size_t i = 0; i < _tiles.size(); ++i) // Recorrer filas
	{
		for (size_t j = 0; j < _tiles[i].size(); ++j) // Recorrer columnas
		{
			Tile* tile = _tiles[i][j];

			if (tile && tile->getCollider()->collides(*sprite.getCollider()))
			{
				if (tile->tileId == 25 && sprite.getId() == 0) // Si es el tile que queremos eliminar 
				{
					//Si el sprite es el de las abejas se devuelve la colision false, si no lo es se devuelve verdadera, para eso usamos una variable bool
					int* retrievedScore = static_cast<int*>(sprite.getUserData());
					if (retrievedScore)
					{
						*retrievedScore += 1;
					}
					
					sprite.setUserData(retrievedScore);
					delete tiles[i][j];
					tiles[i][j] = nullptr;

					return true;
				}
				else if (tile->tileId != 25)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool World::moveSprite(Sprite& sprite, const Vec2& amount)
{
	bool collisionDetected = false;

	Vec2 originalPosition = sprite.getPosition();
	Vec2 newPos = originalPosition;

	// **Intentar mover en X**
	newPos.x += amount.x;
	sprite.setPosition(newPos);
	while (checkCollision(sprite, tiles))
	{

		// Retrocede en X de 1 en 1 hasta que no haya colisión
		newPos.x -= (amount.x > 0) ? 1 : -1;
		sprite.setPosition(newPos);
		collisionDetected = true;
	}

	// **Intentar mover en Y**
	newPos.y += amount.y;
	sprite.setPosition(newPos);
	while (checkCollision(sprite, tiles))
	{
		// Retrocede en Y de 1 en 1 hasta que no haya colisión
		newPos.y -= (amount.y > 0) ? 1 : -1;
		sprite.setPosition(newPos);
		collisionDetected = true;
	}

	return collisionDetected;
}

void World::update(float deltaTime)
{
	for (size_t i = 0; i < 4; ++i) {
		if (backgrounds[i]) {
			Vec2 cameraMovement = cameraPos * scrollRatios[i];
			// Movimiento de fondo con velocidad constante
			Vec2 speedMovement = scrollSpeeds[i] * deltaTime;
			// Sumamos ambos movimientos
			scrollOffsets[i] = scrollOffsets[i] + speedMovement;
		}
	}

	for (Sprite* sprite : sprites) {
		if (sprite) {
			sprite->update(deltaTime);
		}
	}
}
void World::draw(const Vec2& screenSize)
{
	lgfx_clearcolorbuffer(clearRed, clearGreen, clearBlue);

	for (size_t i = 0; i < 4; ++i) {
		if (backgrounds[i]) {
			float texWidth = backgrounds[i]->width;
			float texHeight = backgrounds[i]->height;

			float texOffsetX = std::fmod(scrollOffsets[i].x + cameraPos.x * scrollRatios[i], texWidth);
			float texOffsetY = std::fmod(scrollOffsets[i].y + cameraPos.y * scrollRatios[i], texHeight);

			// Ajusta las coordenadas UV para que la textura se estire y ocupe toda la pantalla
			float u0 = texOffsetX / texWidth;
			float v0 = texOffsetY / texHeight;
			float u1 = (texOffsetX + screenSize.x) / texWidth;  // Ajuste de la coordenada u para cubrir la pantalla
			float v1 = (texOffsetY + screenSize.y) / texHeight; // Ajuste de la coordenada v para cubrir la pantalla

			lgfx_setblend(BLEND_ALPHA);
			lgfx_setcolor(1, 1, 1, 1);
			ltex_drawrotsized(backgrounds[i], cameraPos.x, cameraPos.y, 0.0f, 0.0f, 0.0f, screenSize.x, screenSize.y, u0, v0, u1, v1);

		}
	}

	//Dibujamos el mapa:
	for (std::vector<Tile*> i : tiles)
	{
		for (Tile* tile : i)
		{
			if (tile)
			{
				Vec2 size = tile->getSize();
				float u0 = (size.x / (float)tile->tilesetTexture->width) * (float)tile->tileColumn;
				float v0 = (size.y / (float)tile->tilesetTexture->height) * (float)tile->tileRow;
				float u1 = (size.x / (float)tile->tilesetTexture->width) * ((float)tile->tileColumn + 1.0f);
				float v1 = (size.y / (float)tile->tilesetTexture->height) * ((float)tile->tileRow + 1.0f);

				lgfx_setblend(BLEND_ALPHA);
				lgfx_setcolor(1, 1, 1, 1);
				ltex_drawrotsized(tile->tilesetTexture, tile->tilePos.x, tile->tilePos.y, 0.0f, 0.0f, 0.0f, size.x, size.y, u0, v0, u1, v1);
			}

		}

	}

	//Dibujamos los sprites:
	for (Sprite* s: sprites)
	{
		s->draw();
	}

}