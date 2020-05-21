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
    m_level = 0;
    m_stairCol = 0;
    m_stairRow = 0;
    m_playerRow = 0;
    m_playerCol = 0;
    
    for(int i = 0; i < row(); i++){
        for(int j = 0;  j < col(); j++){
            dungeon[i][j] = '#';
        }
    }
    populateDungeon();
    
    randomPos(m_playerRow, m_playerCol);
}

Dungeon::~Dungeon() {
    
}

bool Dungeon::validMove(int r, int c)
{
    dungeon[player()->row()][player()->col()] = '@';
    
    if (dungeon[r][c] != '?' && dungeon[r][c] != '>' && dungeon[r][c] != ')' && dungeon[r][c] != ' '){
        return false;
    }
    
    for (int i = 0; i < m_monsters.size(); i++)
    {
        if (r == m_monsters[i]->row() && c == m_monsters[i]->col()){
            return false;
        }
    }
    return true;
}

void Dungeon::populateDungeon(){
    
    for(int i = 1; i < row()-1; i++){
        for(int j = 1; j < col()-1; j++){
            dungeon[i][j] = ' ';
        }
    }
    
    int M = randInt(2,5*(m_level+1)+1);
    for(int i = 0; i < M; i++){
        int monsterNum;
        if(m_level >= 3){
            monsterNum = randInt(4);
            addMonster(monsterNum);
        }
        else if(m_level == 2){
            monsterNum = randInt(3);
            addMonster(monsterNum);
        }
        else{
            monsterNum=randInt(2);
            addMonster(monsterNum);
        }
    }
    
    int numItems = randInt(2,3);
    for(int i = 0; i < numItems; i++){
        int randItem = randInt(0,7);
        addItem(randItem);
    }
    
}

void Dungeon::display() {
    for(int i = 0; i < m_gameObjects.size(); i++){
        dungeon[m_gameObjects[i]->row()][m_gameObjects[i]->col()] = m_gameObjects[i]->symbol();
    }
    
    if(m_player != nullptr){
        if(dungeon[m_player->row()][m_player->col()] != '#'){
            dungeon[m_player->row()][m_player->col()] = '@';
        }
    }
    for(int i = 0; i < m_monsters.size(); i++){
        dungeon[m_monsters[i]->row()][m_monsters[i]->col()] = m_monsters[i]->symb();
    }
    
    for (int i = 0; i < row(); i++) {
        for (int j = 0; j < col(); j++) {
            std::cout << dungeon[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "Dungeon Level: " << m_level << ", Hit points: " << m_player->hp() << ", Armor: " << m_player->armor() << ", Strength: " << m_player->str() << ", Dexterity: " << m_player->dex() << std::endl;
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
    int r_pos = 0, c_pos = 0;
    randomPos(r_pos, c_pos);
    
    switch (mon_num) {
        case 0:
            m_monsters.push_back(new Goblin(this, r_pos, c_pos));
            break;
        case 1:
            m_monsters.push_back(new Snakewomen(this, r_pos, c_pos));
            break;
        case 2:
            m_monsters.push_back(new Bogeyman(this, r_pos, c_pos));
            break;
        case 3:
            m_monsters.push_back(new Dragon(this, r_pos, c_pos));
            break;
        default:
            break;
    }
}

void Dungeon::killMonsters() {
    for(std::vector<Actor*>::iterator p = m_monsters.begin(); p != m_monsters.end(); p++){
        Actor* mob = *p;
        if(mob->isDead()){
            //drop item
            clearPoint(mob->row(), mob->col());
            delete mob;
            m_monsters.erase(p);
            return;
        }
    }
}


void Dungeon::addItem(int item_num)    //Add objects to the dungeon using int key
{
    int r_pos = 0, c_pos = 0;
    randomPos(r_pos, c_pos);
    
    switch (item_num)
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

void Dungeon::pickUpItem() {
    /*
     if (m_player->row() == m_idol->row() && m_player->col() == m_idol->col()) {
     std::cout << std::endl;
     std::cout << "You pick up the golden idol" << std::endl;
     std::cout << "Congratulations, you won!" << std::endl;
     std::cout << "Press q to exit game." << std::endl;
     //exit game w/ getCharacter and q
     return;
     }
     */
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
                else if(m_gameObjects[i]->symbol() == ')'){
                    std::cout << "You pick up " << m_gameObjects[i]->name() << std::endl;
                    player()->m_inventory.push_back(m_gameObjects[i]);
                    std::vector<GameObject*>::iterator temp = m_gameObjects.begin() + i;
                    m_gameObjects.erase(temp);
                }
            }
        }
    }
}

bool Dungeon::isMonster(int r, int c){
    if(isalpha(dungeon[r][c])){
        return true;
    }
    return false;
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

void Dungeon::clearPoint(int r, int c){
    if(!checkItemPos(r,c)){
        for(int i = 0; i < m_gameObjects.size(); i++){
            if(m_gameObjects[i]->row() == r && m_gameObjects[i]->col() == c){
                dungeon[r][c] = m_gameObjects[i]->symbol();
                return;
            }
        }
    }
    dungeon[r][c] = ' ';
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

