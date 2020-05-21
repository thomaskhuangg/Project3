// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance)
:m_currentLevel(0), m_smellDistance(goblinSmellDistance)
{
    
    m_dungeons[0] = new Dungeon();  //Create Dungeon
    
    //m_dungeons[0]->addPlayer();     //Add player to the dungeon
}

Game::~Game()
{
    for (int i = 0; i < 5 && m_dungeons[i] != nullptr; i++)
        delete m_dungeons[i];
}

void Game::play()
{
 //check if there's no player !!!!
    Player* player_ptr = m_dungeons[0]->player();
    while(!m_dungeons[m_currentLevel]->player()->isDead()){
        //check for dead mobs
        player_ptr->heal(); //the 1/10 chance for player to heal per turn
        
        clearScreen(); //clear screen for the display
        
        m_dungeons[m_currentLevel]->display(); //display current level's dungeon
        
        char action = getCharacter();
        
        if(player_ptr->sleepTime() == 0){ //check if player is sleeping
            switch(action){
                case ARROW_UP:
                case ARROW_DOWN:
                case ARROW_RIGHT:
                case ARROW_LEFT:
                    //here u want to move player and display message
                    break;
                case 'c':
                    player_ptr->cheat();
                    break;
                case 'w':
                    player_ptr->displayInventory('w');
                    break;
                case 'r':
                    player_ptr->displayInventory('r');
                    break;
                case 'g':
                    //trigger pick up
                    break;
                case 'i':
                    player_ptr->displayInventory('i');
                    break;
                case '>':
                    //check if next level valid, increase level, create new dungeion ,and add player to it
                    break;
                default:
                    break;
            }
        }
    }
    clearScreen();
    m_dungeons[m_currentLevel]->display();
    
    if (m_dungeons[m_currentLevel]->player()->isDead())
        cout << "YOU LOSE!" << endl;
    cout << endl;
    
    cout << "Press q to exit game." << endl;
    while (getCharacter() != 'q')
        ;
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
