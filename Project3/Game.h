// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include "utilities.h"
#include "Actor.h"

class Dungeon;

class Game
{
public:
    Game(int goblinSmellDistance);  //Game constructor
    ~Game();
    
    int currentLevel() {return m_currentLevel; }    //level accessor
    
    void play();
    
    
private:
    Dungeon* m_dungeons[5];    //5 Levels deep Dungeon
    
    int m_currentLevel;  //Current Level
    
    int m_smellDistance;

};


#endif // GAME_INCLUDED
