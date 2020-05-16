//
//  Dungeon.h
//  Project3
//
//  Created by Thomas Huang on 5/15/20.
//  Copyright © 2020 Thomas Huang. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include "Actor.h"
#include "GameObject.h"
#include "Game.h"
#include "utilities.h"
#include <vector>

class Actor;
class Player;
class GameObject;
class Game;

const int MAXROWS = 18;
const int MAXCOLS = 70;

class Dungeon{
public:
    Dungeon();
    Dungeon(int level, Player* user);
    ~Dungeon();
    
    void display();
    
    bool addPlayer();
    bool addMonster(int num);
    bool addItem(int r,int c, int item);
    
    bool validMove(int r, int c);
    bool isWall(int r, int c);
    
    bool pickUpItem(std::string &item);
    
    bool checkObjectPos(int r, int c);
    
    void makeRoom(int &row, int &col);
    //Accessors
    
    int row() const { return m_row; }
    int col() const { return m_col; }
    Player* player() const {return m_player; }
private:
    //Private member variables
    Game* game;
    
    int m_row;
    int m_col;
    
    int m_stairRow;
    int m_stairCol;
    
    int m_playerRow;
    int m_playerCol;
    
    int m_level;
    
    Player* m_player;
    
    char dungeon[MAXROWS][MAXCOLS];
};
#endif /* Dungeon_h */
