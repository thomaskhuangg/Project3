#include "Actor.h"
#include "Dungeon.h"
#include "GameObject.h"

Actor::Actor(Dungeon* level, int r, int c, int hp, GameObject* weapon, int armor, int str, int dex, int sleepTime, std::string name, char symb){ //Constructor for arbitrary Actor; sets member variables
    
    m_level = level;
    m_row = r;
    m_col = c;
    m_hp = hp;
    m_weapon = weapon;
    m_armor = armor;
    m_str = str;
    m_dex = dex;
    m_sleepTime = sleepTime;
    m_name = name;
    m_symb = symb;
    
}

void Actor::placeActor(Dungeon *level ,int r, int c){
    m_level = level;
    m_row = r;
    m_col = c;
}


bool Actor::isDead() const{ //If health < 0, character dies
    if(hp() <= 0){
        return true;
    }
    else{
        return false;
    }
}

void Actor::changeHP(int num){ //all change____ functions alter health, armor, strength, dex depending on what action needs it
    m_hp += num;
    
    if(hp() > maxHealth){
        m_hp = maxHealth;
    }
}

void Actor::changeArmor(int num){
    m_armor += num;
    
    if(armor() > maxArmor){
        m_armor = maxArmor;
    }
}

void Actor::changeStr(int num){
    m_str += num;
    
    if(m_str > maxStrength){
        m_str = maxStrength;
    }
}

void Actor::changeDex(int num){
    m_dex += num;
    
    if(m_dex > maxDex){
        m_dex = maxDex;
    }
}

void Actor::changeSleepTime(int num){
    m_sleepTime += num;
    
    if(m_sleepTime > maxSleepTime){
        m_sleepTime = maxSleepTime;
    }
}

void Actor::setWeapon(GameObject* obj){ //Sets actor's weapon to whatever is passed in
    m_weapon = obj;
}


Player::Player(Dungeon* level, int r, int c) : Actor(level, r, c, 20, nullptr, 2,2,2,0, "Player", '@'){ //Creates a new player, and new players have a short-sword on spawn, and in their inventory. Default stats are provided in Initializer List
    m_inventory.push_back(new ShortSword);
    setWeapon(m_inventory[0]);
}

Player::~Player(){ //Destructor
    
}

void Actor::cheat(){ //Cheat function for testing
    m_str = 9;
    m_hp = 50;
}

void Actor::attack(int r, int c){
    if (getLevel()->player()->row() == r && getLevel()->player()->col() == c) {
        
    }
}

void Player::readScroll(GameObject* obj){ //t is teleport, a is armor, etc. Used in inventory, if they choose to use it, the stat is changed/the player is teleported
    switch(obj->symbol()){
        case 't':
            //Do this after u finish dungeon
            break;
        case 'a':
            changeArmor(obj->bonus());
            break;
        case 's':
            changeStr(obj->bonus());
            break;
        case 'h':
            changeHP(obj->bonus());
        case 'd':
            changeHP(obj->bonus());
        default:
            break;
    }
}

void Player::displayInventory(char c){ //Display inventory
    clearScreen(); //Clears screen
    
    std::cout << "Inventory:" << std::endl; //First part to print inventory
    std::string alpha = "abcdefghijklmnopqrstuvwxyz"; //since inventory is labeled a-z, made a string of the alphabet to be called in a : alpha[i] for 0 <= i <= 25
    
    for(int i = 0; i < m_inventory.size(); i++){ //loops through inventory and prints out the corresponding spot in the inventory (a-z) and whatever item it is
        if(m_inventory[i] ->symbol() == '?'){
            std::cout << alpha[i] << ": " << "A scroll called " << m_inventory[i]->name() << std::endl; //Case of scroll
        }
        else{
            std::cout << alpha[i] << ": " << m_inventory[i]->name() << std::endl; //Case of weapon
        }
    }
    
    char c1 = getCharacter(); //Uses getCharacter of user input
    
    int desiredObject = 99;
    if(isalpha(c1)){
        //If the character provided by the user is in the alphabet (I.E. an item in the inventory)
        for(int j = 0; j < m_inventory.size(); j++){
            //Finds position in the inventory (integer) that corresponds to the desired object
            if(alpha[j] == c1){
                desiredObject = j;
                break;
            }
        }
        if(desiredObject == 99){
            //We know that if the above holds, and the inventory contains c1, then desiredObject can only be 0->25. So if it stays @ 99, we know that it wasn't a character, and we should just return from the function.
            return;
        }
        if(c == 'w'){ //If user wants to choose weapon
            if(m_inventory[desiredObject] == getWeapon()){ //If they choose the weapon they already have equipped, just return out of the function.
                return;
            }
            else if(m_inventory[desiredObject]->symbol() == ')'){ //If chosen item is a weapon, we swap it and print what weapon they are now currently wielding
                setWeapon(m_inventory[desiredObject]); //Sets weapon to the weapon that they chose
                std::cout << "You are wielding " << getWeapon()->name() << "." << std::endl; //Prints what weapon they are wielding
            }
            else{
                std::cout << "You can't wield " << m_inventory[desiredObject]->name()  << "." << std::endl; //If it's not a weapon, then we tell the user that they can't wield the current item
            }
        }
        else if( c == 'r'){ //If user wants to use a scroll
            if(m_inventory[desiredObject]->symbol() == '?'){ //If the item in the inventory is a scroll-type, we continue
                std::cout << "You read a scroll of " << m_inventory[desiredObject]->name() << "." << " " << m_inventory[desiredObject]->action() << "." << std::endl; //Prints what type of scroll they use
                std::vector<GameObject*>::iterator temp = m_inventory.begin() + desiredObject; //We create a temp to erase from the inventory
                readScroll(m_inventory[desiredObject]); //we read the scroll and undergo the effect of the scroll
                m_inventory.erase(temp); //erase the GameObject from the inventory
            }
            else{
                std::cout << "You can't read a " << m_inventory[desiredObject] << "." << std::endl; //If it's not a scroll, then we tell the user that they can't read the current item
            }
        }
    }
    else{
        return; //If the inputted character isn't in the alphabet, we return.
    }
}

void Player::attack(int r, int c) {
    char pos = getLevel()->currentPos(r, c);

    switch (pos) {
    case 'B':
    case 'S':
    case 'D':
    case 'G':
        Actor * mob = getLevel()->monster(r,c);

        int playerDmg = this->dex();
        int monsterStats = mob->dex() + mob->armor();

        int randPlayer = randInt(playerDmg);
        int randMonster = randInt(monsterStats);

    }
}

void Player::heal(){ //Random 1/10 chance for the user to heal
    if(randInt(10) == 1){
        changeHP(1);
    }
}

Bogeyman::Bogeyman(Dungeon* level, int r, int c):Actor(level, r, c, randInt(5,10), new ShortSword(r,c), 2, randInt(2,3), randInt(2,3), 0, "Bogeyman", 'B') //Creates new Bogeyman @ pos (r,c) on level level
{
    
}

Bogeyman::~Bogeyman(){//Destructor
    
    
}

void Bogeyman::drop(int r, int c)
{
    
    if (randInt(10) == 0)   //1 in 10 drop of magic axe. If no item already there.
    {/*
        if (!(Level()->checkObject(r, c)))
        {
            Level()->addItem(r, c, 4);
        }
      */
    }
}

Snakewomen::Snakewomen(Dungeon* level, int r, int c): Actor(level, r, c, randInt(3,6), new MagicFang(r,c), 3,2,3,0, "Snakewomen", 'S') //Creates new Snakewomen @ pos (r,c) on level level
{
    
}

Snakewomen::~Snakewomen(){ //Destructor
    
    
}

void Snakewomen::drop(int r, int c)
{
    
    if (randInt(3) == 1)    //1 in 3 drop of magic fangs of sleep. If no item already there
    {/*
        if (!(Level()->checkObject(r, c)))
        {
            Level()->addItem(r, c, (5 + randInt(5)));
        }
      */
    }
}

Dragon::Dragon(Dungeon* level, int r, int c): Actor(level, r, c, randInt(20,25), new LongSword(r,c), 4,4,4,0, "Dragon", 'D')  //Creates new Dragon @ pos (r,c) on level level
{
    
}

Dragon::~Dragon(){ //Destructor
    
    
}

void Dragon::drop(int r, int c)
{
    /*
    if (!(Level()->checkObject(r, c)))      //100% will drop a scroll of some kind, If no item already there.
    {
        int number = 0;
        
        number = randInt(20);
        
        //number = 1;
        
        if (number == 1)
            Level()->addItem(r, c, 10);
        else
            Level()->addItem(r, c, 5+randInt(5));
 
}
        */
}

void Dragon::heal()//Heals exactly like player
{
    if(randInt(10) == 1){
         changeHP(1);
     }
}

Goblin::Goblin(Dungeon* level, int r, int c): Actor(level, r, c, randInt(15,20), new ShortSword(r,c), 1,3,1,0,"Goblin", 'G') //Creates new Goblin @ pos (r,c) on level level
{
    
}

Goblin::~Goblin(){ //Destructor
    
    
}
void Goblin::drop(int r, int c)
{/*
    if (randInt(3) == 1)    //1 in 3 drop of magic axe or magic fangs of sleep if no item already there
    {
        if (!(Level()->checkObject(r, c)))
        {
            Level()->addItem(r, c, 3+randInt(2));
        }
    }
    else
    {
        int number = 0;
        
        number = randInt(25);
        
        //number = 1;
        
        if (number == 1)
        {
            if (!(Level()->checkObject(r, c)))
            {
                Level()->addItem(r, c, 11);
            }
        }
    }
    */
}
