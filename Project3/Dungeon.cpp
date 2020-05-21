//
//  Dungeon.cpp
//  Project3
//
//  Created by Thomas Huang on 5/15/20.
//  Copyright © 2020 Thomas Huang. All rights reserved.
//

#include "Dungeon.h"
#include <iostream>

Dungeon::Dungeon() {
	m_player = nullptr;
	m_row = MAXROWS;
	m_col = MAXCOLS;
	m_stairCol = 0;
	m_stairRow = 0;
	m_playerRow = 0;
	m_playerCol = 0;

	for (int i = 0; i < row(); i++) {
		dungeon[i][0] = '#';
		dungeon[i][col() - 2] = '#';
	}
	for (int j = 0; j < col(); j++) {
		dungeon[0][j] = '#';
		dungeon[row() - 2][j] = '#';
	}
}

Dungeon::~Dungeon() {

}

bool Dungeon::validMove(int r, int c)
{
	dungeon[player()->row()][player()->col()] = '@';

	if (dungeon[r][c] != '#' && dungeon[r][c] != '?' && dungeon[r][c] != '>' && dungeon[r][c] != ' ')
		return false;

	for (int i = 0; i < m_monsters.size(); i++)
	{
		if (r == m_monsters[i]->row() && c == m_monsters[i]->col())  
			return false;
	}
	return true; 
}


void Dungeon::display() {
	for (int i = 0; i < row(); i++) {
		for (int j = 0; j < col(); j++) {
			std::cout << dungeon[i][j];
		}
		std::cout << std::endl;
	}
}

Actor* Dungeon::monster(int r, int c) const
{
	for (int i = 0; i < m_monsters.size(); i++) {
		if (r == m_monsters[i]->row() && c == m_monsters[i]->col()) {
			return m_monsters[i];
		}
	}
	return nullptr;
}


bool Dungeon::addPlayer() {
	if (m_player == nullptr) {
		m_player = new Player(this, m_playerRow, m_playerCol);
		return true;
	}
	else {
		m_player->placeActor(this, m_playerRow, m_playerCol);
		return false;
	}
}

void Dungeon::addMonster(int mon_num) {
	int r_pos, c_pos;
	randomPos(r_pos, c_pos);

	switch (mon_num) {
	case 0:
		m_monsters.push_back(new Bogeyman(this, r_pos, c_pos));
		break;
	case 1:
		m_monsters.push_back(new Snakewomen(this, r_pos, c_pos));
		break;
	case 2:
		m_monsters.push_back(new Dragon(this, r_pos, c_pos));
		break;
	case 3:
		m_monsters.push_back(new Goblin(this, r_pos, c_pos));
		break;
	default:
		break;
	}
}

void Dungeon::killMonsters() {
	for (int i = 0; i < m_monsters.size(); i++) {
		if (m_monsters[i]->isDead()) {
			m_monsters[i]->drop(m_monsters[i]->row(), m_monsters[i]->col());
			std::vector<Actor*>::iterator temp = m_monsters.begin() + i;
			m_monsters.erase(temp);
			break;
		}
	}
}


void Dungeon::addItem(int r, int c, int item_num)    //Add objects to the dungeon using int key
{
	switch (item_num)
	{
	case 0:
		m_gameObjects.push_back(new Mace(r, c));
		break;
	case 1:
		m_gameObjects.push_back(new ShortSword(r, c));
		break;
	case 2:
		m_gameObjects.push_back(new LongSword(r, c));
		break;
	case 3:
		m_gameObjects.push_back(new MagicAxe(r, c));
		break;
	case 4:
		m_gameObjects.push_back(new MagicFang(r, c));
		break;
	case 5:
		m_gameObjects.push_back(new Teleport(r, c));
		break;
	case 6:
		m_gameObjects.push_back(new ImproveArmor(r, c));
		break;
	case 7:
		m_gameObjects.push_back(new RaiseStrength(r, c));
		break;
	case 8:
		m_gameObjects.push_back(new EnhanceHealth(r, c));
		break;
	case 9:
		m_gameObjects.push_back(new EnhanceDex(r, c));
		break;
	default:
		break;
	}
}

void Dungeon::pickUpItem(std::string& item) {
	if (m_player->row() == m_idol->row() && m_player->col() == m_idol->col()) {
		std::cout << std::endl;
		std::cout << "You pick up the golden idol" << std::endl;
		std::cout << "Congratulations, you won!" << std::endl;
		std::cout << "Press q to exit game." << std::endl;
		//exit game w/ getCharacter and q
		return;
	}
	if (player()->m_inventory.size() > 25) {
		std::cout << "Your knapsack is full; you can't pick that up." << std::endl;
		return;
	}
	else {
		for (int i = 0; i < m_gameObjects.size(); i++) {
			if (m_player->row() == m_gameObjects[i]->row() && m_player->col() == m_gameObjects[i]->col()) {
				if (m_gameObjects[i]->symbol() == '?') {
					std::cout << "You pick up a scroll called " << m_gameObjects[i]->name() << std::endl;
					player()->m_inventory.push_back(m_gameObjects[i]);
					std::vector<GameObject*>::iterator temp = m_gameObjects.begin() + i;
					m_gameObjects.erase(temp);
					return;
				}
			}
		}
	}
}
bool Dungeon::checkItemPos(int r, int c)
{
	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (m_gameObjects[i]->row() == r && m_gameObjects[i]->col() == c) {
			return false;
		}
	}
	return true;
}

bool Dungeon::isWall(int r, int c){
	if (dungeon[r][c] == '#')
		return true;
	return false;
}


void Dungeon::randomPos(int& r, int& c) {
	while (dungeon[r][c] != ' ')
	{
		r = 1 + randInt(MAXROWS - 2);
		c = 1 + randInt(MAXCOLS - 2);
	}
}

char Dungeon::currentPos(int r, int c) {
	return dungeon[r][c];
}

