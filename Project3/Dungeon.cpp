//
//  Dungeon.cpp
//  Project3
//
//  Created by Thomas Huang on 5/15/20.
//  Copyright © 2020 Thomas Huang. All rights reserved.
//

#include "Dungeon.h"
#include <iostream>

Dungeon::Dungeon(){
    m_player = nullptr;
    m_row = MAXROWS;
    m_col = MAXCOLS;
    m_stairCol = 0;
    m_stairRow = 0;
    m_playerRow = 0;
    m_playerCol = 0;
    
    for(int i = 0; i < row(); i++){
            dungeon[i][0] = '#';
        dungeon[i][col()-2] = '#';
    }
    for(int j = 0; j < col(); j++){
        dungeon[0][j] = '#';
        dungeon[row()-2][j] = '#';
    }
}

Dungeon::~Dungeon(){
    
}

void Dungeon::display(){
    for(int i = 0; i < row(); i++){
        for(int j = 0; j < col(); j++){
            std::cout << dungeon[i][j];
        }
        std::cout<< std::endl;
    }
}

bool Dungeon::addPlayer(){
    if(m_player == nullptr){
        m_player = new Player(this, m_playerRow, m_playerCol);
    }
    else{

    }
}
/*
bool Dungeon::addMonster(int num){
    
}
bool Dungeon::addItem(int r,int c, int item){
    
}

bool Dungeon::validMove(int r, int c){
    
}
bool Dungeon::isWall(int r, int c){
    
}

bool Dungeon::pickUpItem(std::string &item){
    
}

bool Dungeon::checkObjectPos(int r, int c){
    
}

void Dungeon::makeRoom(int &row, int &col){
    
}
*/
