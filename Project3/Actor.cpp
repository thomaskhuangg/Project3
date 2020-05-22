#include "Actor.h"
#include "Dungeon.h"
#include "GameObject.h"

Actor::Actor(Dungeon* level, int r, int c, int hp, GameObject* weapon, int armor, int str, int dex, int sleepTime, std::string name, char symb) { //Constructor for arbitrary Actor; sets member variables
	//Constructor
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

void Actor::changeCoords(int r, int c) { //Change the coordinates of the actor
	m_row = r;
	m_col = c;
}

std::string Actor::move(char direction) {//move function

	if (direction == 'z') { //This is specific to a monster, which directs us to the moveMonster function
		direction = moveMonster(getLevel()->player()->row(), getLevel()->player()->col());
	}

	std::string message = "";
	switch (direction) {
	case ARROW_UP:
		if (row() > 0 && getLevel()->validMove(row() - 1, col())) { //If we are at a valid position,
			m_level->clearPoint(row(), col()); //Clear the point that it is at
			changeCoords(row() - 1, col()); //Move the actor to the desired point
			break;//break
		}
		else {
			message += attack(row() - 1, col()); //If it isn't valid, there's a monster there, or a wall, but eitherway we attack
			break;
		}
	case ARROW_DOWN:
		if (row() < getLevel()->row() - 1 && getLevel()->validMove(row() + 1, col())) {
			m_level->clearPoint(row(), col());
			changeCoords(row() + 1, col());
			break;
		}
		else {
			message += attack(row() + 1, col());
			break;
		}
	case ARROW_LEFT:
		if (col() > 0 && getLevel()->validMove(row(), col() - 1)) {
			m_level->clearPoint(row(), col());
			changeCoords(row(), col() - 1);
			break;
		}
		else {
			message += attack(row(), col() - 1);
			break;
		}
	case ARROW_RIGHT:
		if (col() < getLevel()->col() - 1 && getLevel()->validMove(row(), col() + 1)) {
			m_level->clearPoint(row(), col());
			changeCoords(row(), col() + 1);
			break;
		}
		else {
			message += attack(row(), col() + 1);
			break;
		}
	default:
		break;
	}
	return message;
}

std::string Actor::attack(int r, int c) { //This is the monster's attack method
	std::string message = "";

	bool hitBool = false; //Boolean to determine whether the attacker attacked or not
	bool isSleep = false;
	Actor* player = getLevel()->player(); //get player
	if (getLevel()->player()->row() == r && getLevel()->player()->col() == c) { //If the player is @ the position that they want to attack at
		int monsterStats = dex() + getWeapon()->dex(); //Monster stats
		int playerStats = player->dex() + player->armor(); //Player stats

		int randPlayerStats = randInt(1, playerStats);
		int randMonsterStats = randInt(1, monsterStats);

		if (randInt(1, randMonsterStats) >= randInt(1, randPlayerStats)) { //Determine whether the monster can attack 
			hitBool = true;
			player->changeHP(-1 * randInt(0, str() + getWeapon()->damage() - 1)); //Change the player's hp if the hit was successful

			if (getWeapon()->name() == "magic fangs") {
				if (randInt(4) == 0) {
					isSleep = true;
					int X = randInt(2, 6);
					if (player->sleepTime() > 0) {
						if (X > player->sleepTime()) {
							player->changeSleepTime(-1 * player->sleepTime());
							player->changeSleepTime(X);
						}
					}
					else {
						player->changeSleepTime(X);
					}
				}
			}

			if (player->isDead()) {
				message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + player->name() + " dealing a final blow." + "\n"; //Message if attack kills
			}
			else if (player->sleepTime() > 0 && isSleep) {
				message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + player->name() + " and hits, putting " + player->name() + " to sleep." + "\n";
			}
			else {
				message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + player->name() + " and hits." + "\n"; //message if attack hits
			}
		}
		else {
			message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + player->name() + " and misses." + "\n"; //message if attack fails
		}
	}

	return message;
}

void Actor::placeActor(Dungeon* level, int r, int c) { //Place actor on a certain level, row, and column
	m_level = level;
	m_row = r;
	m_col = c;
}

bool Actor::checkPlayerPos(int r, int c) const { //Check if player is at the desired position
	if (getLevel()->player()->row() == r && getLevel()->player()->col() == c) {
		return true;
	}
	return false;
}

bool Actor::isDead() const { //If health < 0, character dies
	if (hp() <= 0) {
		return true;
	}
	else {
		return false;
	}
}

void Actor::changeHP(int num) { //all change____ functions alter health, armor, strength, dex depending on what action needs it
	m_hp += num;

	if (hp() > maxHealth) {
		m_hp = maxHealth;
	}
}

void Actor::changeArmor(int num) {
	m_armor += num;

	if (armor() > maxArmor) {
		m_armor = maxArmor;
	}
}

void Actor::changeStr(int num) {
	m_str += num;

	if (m_str > maxStrength) {
		m_str = maxStrength;
	}
}

void Actor::changeDex(int num) {
	m_dex += num;

	if (m_dex > maxDex) {
		m_dex = maxDex;
	}
}

void Actor::changeSleepTime(int num) {
	m_sleepTime += num;

	if (m_sleepTime > maxSleepTime) {
		m_sleepTime = maxSleepTime;
	}
}

void Actor::setWeapon(GameObject* obj) { //Sets actor's weapon to whatever is passed in
	m_weapon = obj;
}


Player::Player(Dungeon* level, int r, int c) : Actor(level, r, c, 20, nullptr, 2, 2, 2, 0, "Player", '@') { //Creates a new player, and new players have a short-sword on spawn, and in their inventory. Default stats are provided in Initializer List
	m_inventory.push_back(new ShortSword);
	setWeapon(m_inventory[0]);
}

Player::~Player() { //Destructor

}

void Actor::cheat() { //Cheat function for testing
	m_str = 9;
	m_hp = 50;
}

void Player::readScroll(GameObject* obj) { //t is teleport, a is armor, etc. Used in inventory, if they choose to use it, the stat is changed/the player is teleported
	switch (obj->first()) {
	case 't': {
		int r_pos = 0;
		int c_pos = 0;
		getLevel()->randomPos(r_pos, c_pos);
		getLevel()->clearPoint(row(), col());
		changeCoords(r_pos, c_pos);
		break;
	}
	case 'a':
		changeArmor(obj->bonus());
		break;
	case 's':
		changeStr(obj->bonus());
		break;
	case 'h':
		changeHP(obj->bonus());
		break;
	case 'd':
		changeDex(obj->bonus());
		break;
	default:
		break;
	}
}

std::string Player::displayInventory(char c) { //Display inventory
	clearScreen(); //Clears screen

	std::cout << "Inventory:" << std::endl; //First part to print inventory
	std::string alpha = "abcdefghijklmnopqrstuvwxyz"; //since inventory is labeled a-z, made a string of the alphabet to be called in a : alpha[i] for 0 <= i <= 25

	for (int i = 0; i < m_inventory.size(); i++) { //loops through inventory and prints out the corresponding spot in the inventory (a-z) and whatever item it is
		if (m_inventory[i]->symbol() == '?') {
			std::cout << alpha[i] << ": " << "A scroll called " << m_inventory[i]->name() << std::endl; //Case of scroll
		}
		else {
			std::cout << alpha[i] << ": " << m_inventory[i]->name() << std::endl; //Case of weapon
		}
	}

	char c1 = getCharacter(); //Uses getCharacter of user input

	std::string message = "";

	int desiredObject = 99;
	if (isalpha(c1)) {
		//If the character provided by the user is in the alphabet (I.E. an item in the inventory)
		for (int j = 0; j < m_inventory.size(); j++) {
			//Finds position in the inventory (integer) that corresponds to the desired object
			if (alpha[j] == c1) {
				desiredObject = j;
			}
		}
		if (desiredObject == 99) {
			//We know that if the above holds, and the inventory contains c1, then desiredObject can only be 0->25. So if it stays @ 99, we know that it wasn't a character, and we should just return from the function.
			return "";
		}
		if (c == 'w') { //If user wants to choose weapon
			if (m_inventory[desiredObject] == getWeapon()) { //If they choose the weapon they already have equipped, just return out of the function.
				return "";
			}
			else if (m_inventory[desiredObject]->symbol() == ')') { //If chosen item is a weapon, we swap it and print what weapon they are now currently wielding
				setWeapon(m_inventory[desiredObject]); //Sets weapon to the weapon that they chose
				message += "You are wielding " + getWeapon()->name() + "\n"; //Prints what weapon they are wielding
			}
			else {
				message += "You can't wield " + m_inventory[desiredObject]->name() + "\n"; //If it's not a weapon, then we tell the user that they can't wield the current item
			}
		}
		else if (c == 'r') { //If user wants to use a scroll
			if (m_inventory[desiredObject]->symbol() == '?') { //If the item in the inventory is a scroll-type, we continue
				message += "You read a scroll of " + m_inventory[desiredObject]->name() + "." + " " + m_inventory[desiredObject]->action() + "." + "\n"; //Prints what type of scroll they use
				std::vector<GameObject*>::iterator temp = m_inventory.begin() + desiredObject; //We create a temp to erase from the inventory
				readScroll(m_inventory[desiredObject]); //we read the scroll and undergo the effect of the scroll
				m_inventory.erase(temp); //erase the GameObject from the inventory
			}
			else {
				message += "You can't read a " + m_inventory[desiredObject]->name() + "\n"; //If it's not a scroll, then we tell the user that they can't read the current item
			}
		}

	}
	return message;
}

std::string Player::attack(int r, int c) { //Same as monster attack essentially
	std::string message = "";
	char pos = getLevel()->currentPos(r, c);
	bool hitBool = false;
	bool isSleep = false;
	switch (pos) {
	case 'B':
	case 'S':
	case 'D':
	case 'G':
		// see what the user's next move is
		Actor * mob = getLevel()->monster(r, c);

		int playerStats = dex() + getWeapon()->dex();
		int monsterStats = mob->dex() + mob->armor();

		int randPlayerStats = randInt(1, playerStats);
		int randMonsterStats = randInt(1, monsterStats);

		if (randPlayerStats >= randMonsterStats) {
			hitBool = true;
			mob->changeHP(-1 * randInt(0, str() + getWeapon()->damage() - 1));

			if (getWeapon()->name() == "magic fangs") {
				if (randInt(4) == 0) {
					isSleep = true;
					int X = randInt(2, 6);
					if (mob->sleepTime() > 0) {
						if (X > mob->sleepTime()) {
							mob->changeSleepTime(-1 * mob->sleepTime());
							mob->changeSleepTime(X);
						}
					}
					else {
						mob->changeSleepTime(X);
					}
				}
			}
			if (hitBool) {
				if (mob->isDead()) {
					message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + mob->name() + " dealing a final blow." + "\n";
				}
				else if (mob->sleepTime() > 0 && isSleep) {
					message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + mob->name() + " and hits, putting " + mob->name() + " to sleep." + "\n";
				}
				else if (!mob->isDead()){
					message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + mob->name() + " and hits." + "\n";

				}
			}
		}
		else {
			message += name() + " " + getWeapon()->action() + " " + getWeapon()->name() + " at the " + mob->name() + " and misses." + "\n";
		}
	}
	return message;
}

void Player::heal() { //Random 1/10 chance for the user to heal
	if (randInt(10) == 1) {
		changeHP(1);
	}
}

Bogeyman::Bogeyman(Dungeon* level, int r, int c) :Actor(level, r, c, randInt(5, 10), new ShortSword(r, c), 2, randInt(2, 3), randInt(2, 3), 0, "Bogeyman", 'B') //Creates new Bogeyman @ pos (r,c) on level level
{

}

Bogeyman::~Bogeyman() {//Destructor

}

char Bogeyman::moveMonster(int r, int c) {
	if (abs(getLevel()->player()->row() - row()) + abs(getLevel()->player()->col() - col()) <= 5) { //Here is smell distance
		if (!getLevel()->isWall(row() - 1, col())) {
			//As long as there's no wall in the desired move location, and
			//If it's a valid move in the desired move location, and
			//If the actors position and the parmeters aren't exactly the same
			//OR if the player's position isn't directly at the position that you want them to move @
			if ((!getLevel()->isWall(row() - 1, col()) && (getLevel()->validMove(row() - 1, col())) && ((row() - r > 0))) || (checkPlayerPos(row() - 1, col())))
				return ARROW_UP;
			else if ((!getLevel()->isWall(row() + 1, col()) && (getLevel()->validMove(row() + 1, col())) && ((row() - r < 0))) || (checkPlayerPos(row() + 1, col())))
				return ARROW_DOWN;
			else if ((!getLevel()->isWall(row(), col() - 1) && (getLevel()->validMove(row(), col() - 1)) && ((col() - c > 0))) || (checkPlayerPos(row(), col() - 1)))
				return ARROW_LEFT;
			else if ((!getLevel()->isWall(row(), col() + 1) && (getLevel()->validMove(row(), col() + 1)) && ((row() - c < 0))) || (checkPlayerPos(row(), col() + 1)))
				return ARROW_RIGHT;
		}
	}
	return ' ';
}

void Bogeyman::drop(int r, int c) //If the level doesn't already have an item there, depending on the probability, add the item to the dungeon's item vector
{
	if (getLevel()->checkItemPos(r, c)) {
		if (randInt(9) == 0) {
			getLevel()->addItem(r, c, 7);
		}
	}
}

Snakewomen::Snakewomen(Dungeon* level, int r, int c) : Actor(level, r, c, randInt(3, 6), new MagicFang(r, c), 3, 2, 3, 0, "Snakewomen", 'S') //Creates new Snakewomen @ pos (r,c) on level level
{

}

char Snakewomen::moveMonster(int r, int c) {
	if (abs(getLevel()->player()->row() - row()) + abs(getLevel()->player()->col() - col()) <= 3) {
		if (!getLevel()->isWall(row() - 1, col())) {
			if ((!getLevel()->isWall(row() - 1, col()) && (getLevel()->validMove(row() - 1, col())) && ((row() - r > 0))) || (checkPlayerPos(row() - 1, col())))
				return ARROW_UP;
			else if ((!getLevel()->isWall(row() + 1, col()) && (getLevel()->validMove(row() + 1, col())) && ((row() - r < 0))) || (checkPlayerPos(row() + 1, col())))
				return ARROW_DOWN;
			else if ((!getLevel()->isWall(row(), col() - 1) && (getLevel()->validMove(row(), col() - 1)) && ((col() - c > 0))) || (checkPlayerPos(row(), col() - 1)))
				return ARROW_LEFT;
			else if ((!getLevel()->isWall(row(), col() + 1) && (getLevel()->validMove(row(), col() + 1)) && ((row() - c < 0))) || (checkPlayerPos(row(), col() + 1)))
				return ARROW_RIGHT;
		}
	}
	return ' ';
}

Snakewomen::~Snakewomen() { //Destructor


}

void Snakewomen::drop(int r, int c)
{
	if (getLevel()->checkItemPos(r, c)) {
		if (randInt(2) == 1) {
			getLevel()->addItem(r, c, 8);
		}
	}
}

Dragon::Dragon(Dungeon* level, int r, int c) : Actor(level, r, c, randInt(20, 25), new LongSword(r, c), 4, 4, 4, 0, "Dragon", 'D')  //Creates new Dragon @ pos (r,c) on level level
{

}

char Dragon::moveMonster(int r, int c) {
	if (abs(getLevel()->player()->row() - row()) + abs(getLevel()->player()->col() - col()) <= 1) {
		if (!getLevel()->isWall(row() - 1, col())) {
			if ((!getLevel()->isWall(row() - 1, col()) && (getLevel()->validMove(row() - 1, col())) && ((row() - r > 0))) || (checkPlayerPos(row() - 1, col())))
				return ARROW_UP;
			else if ((!getLevel()->isWall(row() + 1, col()) && (getLevel()->validMove(row() + 1, col())) && ((row() - r < 0))) || (checkPlayerPos(row() + 1, col())))
				return ARROW_DOWN;
			else if ((!getLevel()->isWall(row(), col() - 1) && (getLevel()->validMove(row(), col() - 1)) && ((col() - c > 0))) || (checkPlayerPos(row(), col() - 1)))
				return ARROW_LEFT;
			else if ((!getLevel()->isWall(row(), col() + 1) && (getLevel()->validMove(row(), col() + 1)) && ((row() - c < 0))) || (checkPlayerPos(row(), col() + 1)))
				return ARROW_RIGHT;
		}
	}
	return ' ';
}

Dragon::~Dragon() { //Destructor


}

void Dragon::drop(int r, int c)
{
	if (getLevel()->checkItemPos(r, c)) {
		getLevel()->addItem(r, c, 9);
	}
}

void Dragon::heal()//Heals exactly like player
{
	if (randInt(10) == 1) {
		changeHP(1);
	}
}

Goblin::Goblin(Dungeon* level, int r, int c) : Actor(level, r, c, randInt(15, 20), new ShortSword(r, c), 1, 3, 1, 0, "Goblin", 'G') //Creates new Goblin @ pos (r,c) on level level
{

}

Goblin::~Goblin() { //Destructor

}

bool Goblin::BFS(int r, int c) {
	if (abs(getLevel()->player()->row() - row()) + abs(getLevel()->player()->col() - col()) <= 15) {
		if (!getLevel()->isWall(row() - 1, col())) {
			if ((!getLevel()->isWall(row() - 1, col()) && (getLevel()->validMove(row() - 1, col())) && ((row() - r > 0))) || (checkPlayerPos(row() - 1, col())))
				return ARROW_UP;
			else if ((!getLevel()->isWall(row() + 1, col()) && (getLevel()->validMove(row() + 1, col())) && ((row() - r < 0))) || (checkPlayerPos(row() + 1, col())))
				return ARROW_DOWN;
			else if ((!getLevel()->isWall(row(), col() - 1) && (getLevel()->validMove(row(), col() - 1)) && ((col() - c > 0))) || (checkPlayerPos(row(), col() - 1)))
				return ARROW_LEFT;
			else if ((!getLevel()->isWall(row(), col() + 1) && (getLevel()->validMove(row(), col() + 1)) && ((row() - c < 0))) || (checkPlayerPos(row(), col() + 1)))
				return ARROW_RIGHT;
		}
	}
	return ' ';
}

void Goblin::drop(int r, int c)
{
	if (getLevel()->checkItemPos(r, c)) {
		if (randInt(2) == 1) {
			int itemDropped = randInt(7, 8);
			getLevel()->addItem(r, c, itemDropped);
		}
	}
}
