#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <iostream>
#include "Dungeon.h"
#include "GameObject.h"
#include "utilities.h"
#include "Game.h"

class Dungeon;
class GameObject;
class Game;

class Actor{
public:
    Actor(Dungeon* level, int r, int c, int hp, GameObject* weapon, int armor, int str, int dex, int sleepTime, std::string name, char symb); //Constructor
    
    virtual ~Actor(){delete m_weapon;} //Destructor
    
    void move(char direction);
    
    void changeCoords(int r, int c);
    
    void placeActor(Dungeon *level, int r, int c);
    
    Dungeon* getLevel() const { return m_level; } //Accessors
    
    int row() const { return m_row; }
    
    int col() const { return m_col; }
    
    int hp() const { return m_hp; }
    
    GameObject* getWeapon() const {return m_weapon; }
    
    int armor() const {return m_armor; }
    
    int str() const{return m_str; }
    
    int dex() const { return m_dex; }
    
    int sleepTime() const { return m_sleepTime; }
    
    std::string name() const { return m_name; }
    
    char symb() const { return m_symb; }
    
    bool isDead() const;
    
    void changeHP(int num); //Mutators
    
    void changeArmor(int num);
    
    void changeStr(int num);
    
    void changeDex(int num);
    
    void changeSleepTime(int num);
    
    void setWeapon(GameObject* obj);
    
    //Cheat function
    void cheat();
    
    virtual void heal() {}; //Default implementation
    virtual void attack(int r, int c);  //Default implementation
    virtual void drop(int r, int c) {} //Default implementation
    virtual char movePlace(int r, int c) {return ' '; } //Default implementation
    virtual bool checkDist() {return true; }    //Default implementation
    
private:
    
    int m_row, m_col, m_hp, m_armor, m_str, m_dex, m_sleepTime; //Member stats of the actor
    
    std::string m_name; //Name of actor
    
    char m_symb; //Symbol associated w/ the actor
    
    Dungeon* m_level; //level of dungeon
    
    GameObject* m_weapon; //Current wielded weapon
    
    int maxHealth = 99, maxArmor = 99, maxDex = 99, maxStrength = 99, maxSleepTime = 9; //Max stat values
    
    bool m_dead; //True if user is dead, false otherwise.
    
};

class Player : public Actor{
public:
    Player(Dungeon* level, int r, int c); //Constructor for player
    virtual ~Player();//Destructor
    
    int numObjects() const { return m_objects; } //Tells us how many items are in the inventory
    
    std::vector<GameObject*> m_inventory; //Vector of the player's inventory
    void displayInventory(char c);
    
    void readScroll(GameObject* obj);
    
    virtual void heal();
    virtual void attack(int r, int c);
private:
    int m_objects; //how many objects
};

class Bogeyman : public Actor   //Bogeyman class
{
public:
    Bogeyman(Dungeon* level, int r, int c);   //Constructor for placing in a level
    virtual void drop(int r, int c);
    
    virtual ~Bogeyman();
    
private:
    
};

class Snakewomen : public Actor     //Snakewomen class
{
public:
    Snakewomen(Dungeon* level, int r, int c);       //Constructor for placing in a level
    virtual void drop(int r, int c);
    
    virtual ~Snakewomen();
};

class Dragon : public Actor     //Dragon class
{
public:
    Dragon(Dungeon* level, int r, int c);   //Constructor for placing in a level
    virtual void drop(int r, int c);
    virtual void heal();
    
    virtual ~Dragon();
};

class Goblin : public Actor     //Goblin class
{
public:
    Goblin(Dungeon* level, int r, int c);       //Constructor for placing in a level
    
    virtual void drop(int r, int c);
    virtual ~Goblin();
};

#endif /*ACTOR_H*/
