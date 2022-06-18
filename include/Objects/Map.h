#pragma once

#include "Objects/Tile.h"
#include "Core/DeltaTime.h"

class Map {

public:
	Map();
	void Initialize(uint32_t sizeX, uint32_t sizeY);
	void Generate(uint32_t sizeX, uint32_t sizeY);
	void Draw(std::shared_ptr<Shader> shader);
	void ImGuiRender(DeltaTime deltaTime);

private:
	int OptionsNumber(int x, int y);
	void LoadTiles();
	void Propagate(int x, int y, int from);
	uint8_t RandomConfiguration(uint8_t configurations);

	std::vector<std::shared_ptr<Tile>> _tileRegister;
	std::vector<std::pair<std::shared_ptr<Tile>, uint8_t>>** _options;
	std::pair<std::shared_ptr<Tile>, uint8_t>** _map;
	uint32_t _sizeX;
	uint32_t _sizeY;

};