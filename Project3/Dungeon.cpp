//
//  Dungeon.cpp
//  Project3
//
//  Created by Thomas Huang on 5/15/20.
//  Copyright © 2020 Thomas Huang. All rights reserved.
//

#include "Dungeon.h"
#include <iostream>

Dungeon::Dungeon() { //Default constructor; essentially for creating the first level.
	m_player = nullptr;
	m_row = MAXROWS;
	m_col = MAXCOLS;
	m_level = 0;
	m_stairCol = 0;
	m_stairRow = 0;
	m_playerRow = 0;
	m_playerCol = 0;

	for (int i = 0; i < row(); i++) {
		for (int j = 0; j < col(); j++) {
			dungeon[i][j] = '#';
		}
	}
	populateDungeon();

	randomPos(m_playerRow, m_playerCol);
	randomPos(m_stairRow, m_stairCol);
}

Dungeon::Dungeon(int level, Player* user) //Constructor for levels 1-4
{
	m_player = user;
	m_row = MAXROWS;
	m_col = MAXCOLS;
	m_level = level;
	m_stairCol = 0;
	m_stairRow = 0;
	m_playerRow = 0;
	m_playerCol = 0;

	for (int i = 0; i < row(); i++) {
		for (int j = 0; j < col(); j++) {
			dungeon[i][j] = '#';
		}
	}
	populateDungeon();

	randomPos(m_stairRow, m_stairCol);
	if (m_level == 4) {
		int r = 0;
		int c = 0;
		randomPos(r, c);
		m_idol = GameObject(r, c);
	}
	randomPos(m_playerRow, m_playerCol);
}

Dungeon::~Dungeon() { //Destructor, removing monsters and items
	for (int i = 0; i < m_gameObjects.size(); i++) {
		delete m_gameObjects[i];
	}
	for (int j = 0; j < m_monsters.size(); j++) {
		delete m_monsters[j];
	}
	delete m_player;
}


void Dungeon::populateDungeon() { //Populate dungeon

	for (int i = 1; i < row() - 1; i++) { //Here we make the rooms
		for (int j = 1; j < col() - 1; j++) {
			dungeon[i][j] = ' ';
		}
	}

	int M = randInt(2, 5 * (m_level + 1) + 1); //Choose a number of monsters from formula in specs
	for (int i = 0; i < M; i++) { //Loop through the number of monsters 
		int monsterNum; //Depending on the level, certain mobs can spawn, which is accounted for in my addMonster which takes in an int and uses a switch statement
		if (m_level >= 3) { //Monsters 0,1 are snakewomen and bogeyman, 2 is dragons, and 3 is goblins. 
			monsterNum = randInt(4); //All monsters can spawn
			addMonster(monsterNum);
		}
		else if (m_level == 2) {
			monsterNum = randInt(3); //All monsters except for dragon
			addMonster(monsterNum);
		}
		else {
			monsterNum = randInt(2); //Only snakewomen and bogeyman
			addMonster(monsterNum);
		}
	}

	int numItems = randInt(2, 3); //Only 2 or 3 items spawn per level
	for (int i = 0; i < numItems; i++) {
		int r_pos = 0, c_pos = 0;
		int randItem = randInt(0, 6); //Choose from the assortment of items that CAN drop naturally as opposed to by mobs
		addItem(r_pos, c_pos, randItem);
	}

}

void Dungeon::display() { //Display the dungeon
	//Displays items and stairs before the player so the player can stand over them
	for (int i = 0; i < m_gameObjects.size(); i++) {  //Put the items on the grid from the vector
		dungeon[m_gameObjects[i]->row()][m_gameObjects[i]->col()] = m_gameObjects[i]->symbol();
	}

	if (m_level < 4) { //For levels 0-3, spawn stairs
		dungeon[m_stairRow][m_stairCol] = '>';
	}

	else if (m_level == 4) { //At level 4, spawn the golden 
		dungeon[m_idol.row()][m_idol.col()] = '&';
	}


	if (m_player != nullptr) { //Spawn player as long as it's not nullptr
		if (dungeon[m_player->row()][m_player->col()] != '#') {
			dungeon[m_player->row()][m_player->col()] = '@';
		}
	}


	for (int i = 0; i < m_monsters.size(); i++) { //Put monsters on grid from the vector
		dungeon[m_monsters[i]->row()][m_monsters[i]->col()] = m_monsters[i]->symb();
	}

	for (int i = 0; i < row(); i++) { //Now print out the dungeon array
		for (int j = 0; j < col(); j++) {
			std::cout << dungeon[i][j];
		}
		std::cout << std::endl;
	}
	//This is the player's stats, dungeon level, etc.
	std::cout << "Dungeon Level: " << m_level << ", Hit points: " << m_player->hp() << ", Armor: " << m_player->armor() << ", Strength: " << m_player->str() << ", Dexterity: " << m_player->dex() << std::endl;
}

bool Dungeon::nextLevel()
{
	if ((m_player->row() == m_stairRow) && (m_player->col() == m_stairCol)) { //If the player is standing on '<', allow the player to go to the next level
		return true;
	}
	return false;
}

bool Dungeon::validMove(int r, int c)
{
	dungeon[player()->row()][player()->col()] = '@'; //Make the player's position a @

	if (dungeon[r][c] != '?' && dungeon[r][c] != '>' && dungeon[r][c] != ')' && dungeon[r][c] != ' ' && dungeon[r][c] != '&') { //If the spot that we're checking isnt a 
		//scroll, staircase, weapon, free space, or a golden idol return false.
		return false;
	}

	for (int i = 0; i < m_monsters.size(); i++) //Or if there's a monster on the spot, return fasle
	{
		if (r == m_monsters[i]->row() && c == m_monsters[i]->col()) {
			return false;
		}
	}
	return true; //Otherwise return true
}

bool Dungeon::isWall(int r, int c) { //If the place being checked is a wall,m return true, otherwise false
	if (dungeon[r][c] == '#')
		return true;
	return false;
}

void Dungeon::clearPoint(int r, int c) { //Clear a point, used in player movement so it doesn't trail. 
	//Players can walk over objects, so we refresh the spot to contain the item's symbol
	if (!checkItemPos(r, c)) {
		for (int i = 0; i < m_gameObjects.size(); i++) {
			if (m_gameObjects[i]->row() == r && m_gameObjects[i]->col() == c) {
				dungeon[r][c] = m_gameObjects[i]->symbol();
				return;
			}
		}
	}
	dungeon[r][c] = ' '; //Otherwise, return a free space
}

void Dungeon::randomPos(int& r, int& c) { //While the position isn't a free space, change the inputted row and column values randomly.
	while (dungeon[r][c] != ' ')
	{
		r = 1 + randInt(MAXROWS - 2);
		c = 1 + randInt(MAXCOLS - 2);
	}
}

char Dungeon::currentPos(int r, int c) { //returns the dungeon's current char at r,c
	return dungeon[r][c];
}



Actor* Dungeon::monster(int r, int c) const //Finding a monster at a certain row and position from the vector
{
	for (int i = 0; i < m_monsters.size(); i++) {
		if (r == m_monsters[i]->row() && c == m_monsters[i]->col()) {
			return m_monsters[i];
		}
	}
	return nullptr;
}


bool Dungeon::isMonster(int r, int c) { //The only symbols that are alphabetical are monsters, so we check if the symbol at that position is a monster, then return true, otherwise false
	if (isalpha(dungeon[r][c])) {
		return true;
	}
	return false;
}

void Dungeon::addMonster(int mon_num) { //Adding monsters to the dungeon, chooses a random point for them to spawn, and uses a number-code to differentiate between them
	//Case 0-1 can spawn on all levels, case 2 can spawn level 2 and higher, and case 3 spawns on level 3 and higher
	int r_pos = 0, c_pos = 0;
	randomPos(r_pos, c_pos);

	switch (mon_num) {
	case 0:
		m_monsters.push_back(new Bogeyman(this, r_pos, c_pos));
		break;
	case 1:
		m_monsters.push_back(new Snakewomen(this, r_pos, c_pos));
		break;
	case 2:
		m_monsters.push_back(new Goblin(this, r_pos, c_pos));
		break;
	case 3:
		m_monsters.push_back(new Dragon(this, r_pos, c_pos));
		break;
	default:
		break;
	}
}

void Dungeon::killMonsters() { //Iterate through the vectorm and if the mob is dead, we drop the mob's item (if it the probability stands) and erases the vector
	for (std::vector<Actor*>::iterator p = m_monsters.begin(); p != m_monsters.end(); p++) {
		Actor* mob = *p;
		if (mob->isDead()) {
			mob->drop(mob->row(), mob->col());
			clearPoint(mob->row(), mob->col());
			delete mob;
			m_monsters.erase(p);
			return;
		}
	}
}

std::string Dungeon::moveMonsters() { //Move the monsters
	std::string message;
	for (int i = 0; i < m_monsters.size(); i++) { //Loop through the vectors
		if (m_monsters[i]->sleepTime() > 0) {
			m_monsters[i]->changeSleepTime(-1);
		}
		else {
			message += m_monsters[i]->move('z'); //Move the monsters and return the message output by the monster's attack (called in move)
		}
	}
	return message;
}


bool Dungeon::addPlayer() { //If at level 0, which player is nullptr, create a new one, and otherwise, place the existing player at its row and column
	if (m_player == nullptr) {
		m_player = new Player(this, m_playerRow, m_playerCol);
		return true;
	}
	else {
		m_player->placeActor(this, m_playerRow, m_playerCol);
		return false;
	}
}


bool Dungeon::checkItemPos(int r, int c) //If there's an item at the row and column in the parameter, then return false, otherwise true
{
	for (int i = 0; i < m_gameObjects.size(); i++) {
		if (m_gameObjects[i]->row() == r && m_gameObjects[i]->col() == c) {
			return false;
		}
	}
	return true;
}


std::string Dungeon::pickUpItem() {

	std::string message = "";

	//If player is on the idol and picks it up, end the game.
	if (m_level == 4 && ((m_player->row() == m_idol.row()) && (m_player->col() == m_idol.col()))) {
		std::cout << std::endl;
		std::cout << "You pick up the golden idol" << std::endl;
		std::cout << "Congratulations, you won!" << std::endl;
		std::cout << "Press q to exit game." << std::endl;
		char c;
		c = getCharacter();
		if (c == 'q') {
			exit(1);
		}
		while (c != 'q')
		{
			c = getCharacter();
			if (c == 'q')
				exit(1);
		}
		return "";
	}

	if (player()->m_inventory.size() >= 25) { //If the inventory is greater than 25, we say that their napsack is full and not add it
		message = message + "Your knapsack is full; you can't pick that up." + "\n";
	}
	else {
		for (int i = 0; i < m_gameObjects.size(); i++) { //Loop through the game objects vector (associated with the items on the floor of the dungeon)
			if (m_player->row() == m_gameObjects[i]->row() && m_player->col() == m_gameObjects[i]->col()) { //if the row that they're at corresponds to an item
				if (m_gameObjects[i]->symbol() == '?') { //If it's a scroll, print out its message and put it in the player's inventory, and erase it from the dungeon
					message += "You pick up a scroll called " + m_gameObjects[i]->name() + "\n";
					player()->m_inventory.push_back(m_gameObjects[i]);
					std::vector<GameObject*>::iterator temp = m_gameObjects.begin() + i;
					m_gameObjects.erase(temp);
					break;
				}
				else if (m_gameObjects[i]->symbol() == ')') {//If it's a wieldable weapon, print out its message and put it in the player's inventory, and erase it from the dungeon
					message += "You pick up " + m_gameObjects[i]->name() + "\n";
					player()->m_inventory.push_back(m_gameObjects[i]);
					std::vector<GameObject*>::iterator temp = m_gameObjects.begin() + i;
					m_gameObjects.erase(temp);
					break;
				}
			}
		}
	}
	return message;
}

void Dungeon::addItem(int r_pos, int c_pos, int item_num)    //Add objects to the dungeon using int key
{
	if (r_pos == 0 && c_pos == 0) { //i make r_pos and c_pos 0 when I am generating items randomyl in the dungeon
		randomPos(r_pos, c_pos);
	}

	switch (item_num) //Items case 0-6 can be dropped naturally, 7-9 are only dropped by mobs
	{
	case 0:
		m_gameObjects.push_back(new Mace(r_pos, c_pos));
		break;
	case 1:
		m_gameObjects.push_back(new ShortSword(r_pos, c_pos));
		break;
	case 2:
		m_gameObjects.push_back(new LongSword(r_pos, c_pos));
		break;
	case 3:
		m_gameObjects.push_back(new RaiseStrength(r_pos, c_pos));
		break;
	case 4:
		m_gameObjects.push_back(new EnhanceHealth(r_pos, c_pos));
		break;
	case 5:
		m_gameObjects.push_back(new EnhanceDex(r_pos, c_pos));
		break;
	case 6:
		m_gameObjects.push_back(new ImproveArmor(r_pos, c_pos));
		break;
	case 7:
		m_gameObjects.push_back(new MagicAxe(r_pos, c_pos));
		break;
	case 8:
		m_gameObjects.push_back(new MagicFang(r_pos, c_pos));
		break;
	case 9:
		m_gameObjects.push_back(new Teleport(r_pos, c_pos));
		break;
	default:
		break;
	}
}
