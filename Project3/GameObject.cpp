#include "GameObject.h"

GameObject::GameObject() //Default constructor
{
}

GameObject::GameObject(int r, int c) //
{
    m_row = r;
    m_col = c;
}

GameObject::GameObject(int r, int c, std::string name, std::string action, int dex, int damage, char symb)
{
    m_row = r;
    m_col = c;
    m_name = name;
    m_action = action;
    m_dex = dex;
    m_damage = damage;
    m_symb = symb;
}

 GameObject::GameObject(int r, int c, std::string name, std::string action, int bonus, char first, char symb){
    m_row = r;
    m_col = c;
    m_name = name;
     m_action = action;
    m_bonus = bonus;
     m_first = first;
    m_symb = symb;
}


Mace::Mace(int r, int c): GameObject(r,c, "mace", "swings", 0,2,')')
{
}

ShortSword::ShortSword() :GameObject(0, 0, "short sword", "slashes", 0, 2, ')')
{
}

ShortSword::ShortSword(int r, int c):GameObject(r, c, "short sword", "slashes",0, 2, ')')
{
}

LongSword::LongSword(int r, int c) : GameObject(r, c, "long sword","swings", 2, 4, ')')
{
}

MagicAxe::MagicAxe(int r, int c) : GameObject(r, c, "magic axe", "chops",5, 5, ')')
{
}


MagicFang::MagicFang(int r, int c):GameObject(r, c, "magic fangs", "strikes", 3, 5, ')')
{
}

Teleport::Teleport(int r, int c) : GameObject(r,c, "scroll of teleportation", "You feel your body wrenched in space and time.", 0, 't', '?')
{
}

ImproveArmor::ImproveArmor(int r, int c): GameObject(r,c, "scroll of improve armor", "Your armor glows blue.",randInt(1, 3), 'a', '?')
{
}

RaiseStrength::RaiseStrength(int r, int c) : GameObject(r,c, "scroll of raise strength","Your muscles bulge." , randInt(1,3), 's', '?')
{
}

EnhanceHealth::EnhanceHealth(int r, int c) : GameObject(r,c, "scroll of enhance health", "You feel your heart beating stronger.",randInt(3,8), 'h', '?')
{
}

EnhanceDex::EnhanceDex(int r, int c) : GameObject(r,c, "scroll of enhance dex","You feel like less of a klutz.", 1,'d','?')
{
}

