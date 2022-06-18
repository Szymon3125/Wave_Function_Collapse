#include "pch.h"

#include "Objects/Map.h"

#include <iostream>

#include <ctime>
#include <stdlib.h>

Map::Map() {
	LoadTiles();
}

void Map::Initialize(uint32_t sizeX, uint32_t sizeY) {
	srand((int)time(nullptr));
	_sizeX = sizeX;
	_sizeY = sizeY;

	_map = new std::pair<std::shared_ptr<Tile>, uint8_t>*[_sizeX];
	for (uint32_t i = 0; i < _sizeX; i++) {
		_map[i] = new std::pair<std::shared_ptr<Tile>, uint8_t>[_sizeY];
	}
}

void Map::Generate(uint32_t sizeX, uint32_t sizeY) {
	Initialize(sizeX, sizeY);
	// inicjowanie list mo¿liwych kafelków
	_options = new std::vector<std::pair<std::shared_ptr<Tile>, uint8_t>>*[_sizeX];
	for (uint32_t i = 0; i < _sizeX; i++) {
		_options[i] = new std::vector<std::pair<std::shared_ptr<Tile>, uint8_t>>[_sizeY];
		for (uint32_t j = 0; j < _sizeY; j++) {
			_options[i][j] = std::vector<std::pair<std::shared_ptr<Tile>, uint8_t>>();
			for (std::shared_ptr<Tile> tile : _tileRegister) {
				_options[i][j].push_back(std::pair<std::shared_ptr<Tile>, uint8_t>(tile, tile->GetInitialConfigurations()));
			}
		}
	}
	
	bool completed = false;
	int iMin = -1, jMin = -1;
	while (!completed) {
		completed = true;
		iMin = -1;
		jMin = -1;
		for (uint32_t i = 0; i < _sizeX; i++) {
			for (uint32_t j = 0; j < _sizeY; j++) {
				int optionsNumber = OptionsNumber(i, j);
				if (optionsNumber == 0) {
					continue;
				}
				if (optionsNumber == 1) {
					_map[i][j].first = _options[i][j][0].first;
					_map[i][j].second = RandomConfiguration(_options[i][j][0].second);
					continue;
				}
				if (iMin < 0 || optionsNumber < OptionsNumber(iMin, jMin)) {
					iMin = i;
					jMin = j;
				}
				completed = false;
			}
		}
	
		if (iMin >= 0) {
			int sum = 0;
			for (std::pair<std::shared_ptr<Tile>, uint8_t> option : _options[iMin][jMin]) {
				sum += option.first->GetWeight();
			}
			int random = rand() % sum;
			int selected = 0;
			while (random >= _options[iMin][jMin][selected].first->GetWeight()) {
				random -= _options[iMin][jMin][selected].first->GetWeight();
				selected++;
			}
			uint8_t configuration = RandomConfiguration(_options[iMin][jMin][selected].second);
			_map[iMin][jMin] = std::pair<std::shared_ptr<Tile>, uint8_t>(_options[iMin][jMin][selected].first, configuration);
			_options[iMin][jMin].clear();
			_options[iMin][jMin].push_back(std::pair<std::shared_ptr<Tile>, uint8_t>(_map[iMin][jMin].first, 1 << configuration));
			Propagate(iMin, jMin + 1, 0);
			Propagate(iMin - 1, jMin, 1);
			Propagate(iMin, jMin - 1, 2);
			Propagate(iMin + 1, jMin, 3);
		}
	}
}

void Map::Draw(std::shared_ptr<Shader> shader) {
	for (uint32_t i = 0; i < _sizeX; i++) {
		for (uint32_t j = 0; j < _sizeY; j++) {
			if (_map[i][j].first.get() == nullptr) {
				continue;
			}
			_map[i][j].first->Draw(shader, glm::vec3((float)i, 0, (float)j), _map[i][j].second);
		}
	}
}

void Map::ImGuiRender(DeltaTime deltaTime) {
	for (std::shared_ptr<Tile> tile : _tileRegister) {
		tile->ImGuiRender(deltaTime);
	}
}

int Map::OptionsNumber(int x, int y) {
	int result = 0;
	for (std::pair<std::shared_ptr<Tile>, uint8_t> tile : _options[x][y]) {
		uint32_t configurations = tile.second;
		for (int i = 0; i < 8; i++) {
			result += configurations % 2;
			configurations >>= 1;
		}
	}
	return result;
}

void Map::LoadTiles() {
	_tileRegister.push_back(std::make_shared<Tile>("Grass", 0, 0, 0, 0, 1));
	_tileRegister.push_back(std::make_shared<Tile>("Tree", 0, 0, 0, 0, 1));
	_tileRegister.push_back(std::make_shared<Tile>("Bush", 0, 0, 0, 0, UINT8_MAX));
	
	_tileRegister.push_back(std::make_shared<Tile>("Path1", 1, 1, 0, 0, 17));
	_tileRegister.push_back(std::make_shared<Tile>("Path2", 1, 1, 1, 1, 1));
	_tileRegister.push_back(std::make_shared<Tile>("Path3", 1, 1, 0, 1, 51));
	_tileRegister.push_back(std::make_shared<Tile>("Path4", 0, 1, 0, 1, 15));
	
	_tileRegister.push_back(std::make_shared<Tile>("Hedge1", 2, 2, 0, 0, 17));
	_tileRegister.push_back(std::make_shared<Tile>("Hedge2", 2, 2, 2, 2, 1));
	_tileRegister.push_back(std::make_shared<Tile>("Hedge3", 2, 2, 0, 2, 51));
	_tileRegister.push_back(std::make_shared<Tile>("Hedge4", 0, 2, 0, 2, 15));
	
	_tileRegister.push_back(std::make_shared<Tile>("Gate", 2, 2, 1, 1, 17));
}

void Map::Propagate(int x, int y, int from) {
	if (x < 0 || x >= (int)_sizeX || y < 0 || y >= (int)_sizeY) {
		return;
	}
	if (OptionsNumber(x, y) < 2) {
		return;
	}

	bool changes = false;

	std::vector<std::shared_ptr<Tile>> toRemove;
	for (int t = 0; t < _options[x][y].size(); t++) {
		std::vector<std::pair<std::shared_ptr<Tile>, uint8_t>> others;
		switch (from) {
			case 0: others = _options[x][y - 1]; break;
			case 1: others = _options[x + 1][y]; break;
			case 2: others = _options[x][y + 1]; break;
			case 3: others = _options[x - 1][y]; break;
		}
		uint8_t newConfigurations = 0;
		for (int i = 0; i < 8; i++) {
			if (!(_options[x][y][t].second & (1 << i))) {
				continue;
			}
			for (std::pair<std::shared_ptr<Tile>, uint8_t> other : others) {
				for (int j = 0; j < 8; j++) {
					if (!(other.second & (1 << j))) {
						continue;
					}
					bool a = _options[x][y][t].first->ValidateConnection(i, from, other.first, j);
					newConfigurations |= _options[x][y][t].first->ValidateConnection(i, from, other.first, j) << i;
				}
			}
		}
		if (newConfigurations != _options[x][y][t].second) {
			changes = true;
			_options[x][y][t].second = newConfigurations;
			if (!_options[x][y][t].second) {
				toRemove.push_back(_options[x][y][t].first);
			}
		}
	}

	for (std::shared_ptr<Tile> tile : toRemove) {
		for (int i = 0; i < _options[x][y].size(); i++) {
			if (tile.get() == _options[x][y][i].first.get()) {
				_options[x][y].erase(_options[x][y].begin() + i);
				break;
			}
		}
	}

	if (changes) {
		Propagate(x, y + 1, 0);
		Propagate(x - 1, y, 1);
		Propagate(x, y - 1, 2);
		Propagate(x + 1, y, 3);
	}
}

uint8_t Map::RandomConfiguration(uint8_t configurations) {
	uint8_t copy = configurations;
	int sum = 0;
	for (int i = 0; i < 8; i++) {
		sum += configurations % 2;
		configurations >>= 1;
	}
	configurations = copy;
	int random = rand() % sum;
	for (int i = 0; i < 8; i++) {
		if (random == 0 && configurations % 2) {
			return i;
		}
		random -= configurations % 2;
		configurations >>= 1;
	}
	return 0;
}
