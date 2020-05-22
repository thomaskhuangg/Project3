#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "utilities.h"

class GameObject {
public:
    GameObject(); //default constructor
    GameObject(int r, int c); //Placing GameObject @ (r,c)
    GameObject(int r, int c, std::string name, std::string action, int dex, int damage, char symb); //weapons
    GameObject(int r, int c, std::string name, std::string action, int bonus, char first, char symb); //scrolls

    //Accessors

    int row() const { return m_row; }

    int col() const { return m_col; }

    int dex() const { return m_dex; }
    
    int bonus() const { return m_bonus; }

    int damage() const { return m_damage; }

    char first() const { return m_first; }
    
    char symbol() const { return m_symb; }

    std::string name() const { return m_name; }
    
    std::string action() const {return m_action; }

private:
    //member variables
    int m_row, m_col;

    int m_dex, m_damage, m_add, m_bonus;

    char m_symb, m_first;

    std::string m_name, m_action;
};

//All GameObjects create a new GameObject @ some (r,c), other than the default short-sword constructor, which spawns a shortsword w/ the Player @ beginning of game

class Mace : public GameObject {
public:
    Mace(int r, int c);
};

class ShortSword : public GameObject {
public:
    ShortSword();
    ShortSword(int r, int c);
};

class LongSword : public GameObject {
public:
    LongSword(int r, int c);
};

class MagicAxe : public GameObject {
public:
    MagicAxe(int r, int c);
};

class MagicFang : public GameObject {
public:
    MagicFang(int r, int c);
};

class Teleport : public GameObject {
public:
    Teleport(int r, int c);
};

class ImproveArmor : public GameObject {
public:
    ImproveArmor(int r, int c);
};

class RaiseStrength : public GameObject {
public:
    RaiseStrength(int r, int c);
};

class EnhanceHealth : public GameObject {
public:
    EnhanceHealth(int r, int c);
};

class EnhanceDex : public GameObject {
public:
    EnhanceDex(int r, int c);

};


#endif /*GAMEOBJECT_H*/
