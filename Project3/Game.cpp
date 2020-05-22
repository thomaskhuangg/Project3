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
    
    m_dungeons[0]->addPlayer();     //Add player to the dungeon
}

Game::~Game()
{
    for (int i = 0; i < 5 && m_dungeons[i] != nullptr; i++) {
        delete m_dungeons[i];
    }
}

void Game::play()
{
    Player* player_ptr = m_dungeons[0]->player();

    string displayMessage = "";
    
    while(!m_dungeons[m_currentLevel]->player()->isDead()){
        //check for dead mobs
        
        m_dungeons[m_currentLevel]->killMonsters();

        player_ptr->heal(); //the 1/10 chance for player to heal per turn
        
        clearScreen(); //clear screen for the display
        
        m_dungeons[m_currentLevel]->display(); //display current level's dungeon
        
        if (displayMessage.size() > 0) {
            std::cout << endl;
            std::cout << displayMessage;
        }

        displayMessage = "";

        char action = getCharacter();
        
        if(player_ptr->sleepTime() == 0){ //check if player is sleeping
            switch(action){
                case ARROW_UP:
                case ARROW_DOWN:
                case ARROW_RIGHT:
                case ARROW_LEFT:
                    displayMessage += player_ptr->move(action);
                    break;
                    //here u want to move player and display message
                    break;
                case 'c':
                    player_ptr->cheat();
                    break;
                case 'w':
                    displayMessage += player_ptr->displayInventory('w');
                    break;
                case 'r':
                    displayMessage += player_ptr->displayInventory('r');
                    break;
                case 'g':
                    displayMessage += m_dungeons[m_currentLevel]->pickUpItem();
                    break;
                case 'i':
                    displayMessage += player_ptr->displayInventory('i');
                    break;
                case '>':
                    if (m_dungeons[m_currentLevel]->nextLevel()) {
                        m_currentLevel++;
                        m_dungeons[m_currentLevel] = new Dungeon(m_currentLevel, player_ptr);
                        m_dungeons[m_currentLevel]->addPlayer();
                        break;
                    }
                    break;
                case 'q':
                    exit(1);
                default:
                    break;
            }
        }
        else {
            player_ptr->changeSleepTime(-1);
        }
        displayMessage += m_dungeons[m_currentLevel]->moveMonsters();
    }
    clearScreen();
    m_dungeons[m_currentLevel]->display();
    
    if (m_dungeons[m_currentLevel]->player()->isDead())
        std::cout << "YOU LOSE!" << endl;
    std::cout << endl;
    
    std::cout << "Press q to exit game." << endl;
    while (getCharacter() != 'q')
        ;
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
