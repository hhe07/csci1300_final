// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Assets.cpp

#include "Assets.h"

void Assets::split(string in, vector<string>& res, string delim)
{
    /* 
        inspired by:
        https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    */
    // declare temporary string to store the modified string
    string tmp = in;

    // clear the results vector: prevents unexpected values from being in tere
    res.clear();
    
    // while the delimiter can still be found:
    while (tmp.find(delim) != string::npos)
    {
        // push back a substring up to the delimiter position
        res.push_back(tmp.substr(0,tmp.find(delim)));
        // then, remove that segment from the string
        tmp = tmp.erase(0,tmp.find(delim) + 1);
    }
    // push back the trailing end after the last delimiter.
    res.push_back(tmp);
}

Assets::Assets()
{
    // none of the member variables require complex initialisation.
    // int values are already given a value in the header.
}

// returns whether weapons were successfully loaded
bool Assets::loadWeapons(string filename)
{
    // declare filestream variable and attempt to open filename
    ifstream fin;
    fin.open(filename);

    // if file opening failed, indicate failure
    if (fin.fail())
    {
        return false;
    }
    
    // declare variables:
    string line; // stores current line in file
    vector<string> split_res; // store split result

    // while there are still lines in the file to read, read them to line and:
    while (getline(fin,line))
    {
        // split line to split_res
        split(line,split_res,"~");
        // if there aren't the right number of parameters, indicate failure
        if (split_res.size() != 5)
        {
            return false;
        }

        // create and add a new pair to weapons_ using the parameters in split_res
        pair<string, Weapon> data = pair(split_res[0],Weapon{
            stoi(split_res[1]),
            stoi(split_res[2]),
            stoi(split_res[3]),
            stoi(split_res[4]),
        });
        weapons_.insert(data);
    }
    // also include the "none" option for indicating whether players have no weapons equipped
    weapons_["(none)"] = Weapon{0,0,5,0};
    // close file and indicate success
    fin.close();
    return true;
}

// returns whether puzzles were successfully loaded
bool Assets::loadPuzzles(string filename)
{
    // declare filestream variable and attempt to open filename
    ifstream fin;
    fin.open(filename);

    // if file opening failed, indicate failure
    if (fin.fail())
    {
        return false;
    }

    // declare string variable to store line from file
    string line;

    // while there are still lines in the file to read, read them to line and:
    while (getline(fin,line))
    {
        // insert line into puzzles
        puzzles_.push_back(line);
    }
    // close file and indicate success
    fin.close();
    return true;
}

// returns whether cookwares were successfully loaded
bool Assets::loadCookwares(string filename)
{
    // declare filestream variable and attempt to open filename
    ifstream fin; 
    fin.open(filename);

    // if file opening failed, indicate failure
    if (fin.fail())
    {
        return false;
    }

    // declare variables:
    string line; // stores current line in file
    vector<string> split_res; // store split result

    // while there are still lines in the file to read, read them to line and:
    while (getline(fin,line))
    {
        // split line to split_res 
        split(line,split_res,"~");
        // if there aren't the right number of paramters, indicate failure
        if (split_res.size() != 4)
        {
            return false;
        }

        // create and add a new pair to cookwares_ using the parameters in split_res
        pair<string,Cookware> data = pair(split_res[0],Cookware{
            stoi(split_res[1]),
            stoi(split_res[2]),
            stoi(split_res[3]),
        });
        cookware_.insert(data);
    }
    // close file and indicate success
    fin.close();
    return true;
}

// returns whether treasures were successfully loaded
bool Assets::loadTreasures(string filename)
{
    // declare filestream variables and attempt to open filename
    ifstream fin; 
    fin.open(filename);

    // if file opening failed, indicate failure
    if (fin.fail())
    {
        return false;
    }

    // declare variables:
    string line; // stores current line in file
    vector<string> split_res; // store split result

    // while there are still lines in the file to read, read them to line and:
    while (getline(fin,line))
    {
        // split line to split_res
        split(line,split_res,"~");
        
        // if there aren't the right number of parameters, indicate failure
        if (split_res.size() != 3)
        {
            return false;
        }

        // create and add a new pair to treasures_ using the parameters in split_res
        pair<string,Treasure> data = pair(split_res[0],Treasure{
            stoi(split_res[1]),
            stoi(split_res[2]),
        });
        treasures_.insert(data);
    }
    // close file and indicate success
    fin.close();
    return true;
}

// returns whether names of monsters were successfully changed to correspond to a given difficulty level
bool Assets::changeMonsters(string fn_temp, int level)
{
    // declare filestream variable and attempt to open filename
    string filename = fn_temp + to_string(level) + ".txt";
    ifstream fin;
    
    // if file opening failed, indicate failure
    fin.open(filename);
    if (fin.fail())
    {
        return false;
    }

    // clear both vectors related to monsters
    monsters_remaining_.clear();
    defeated_monsters_.clear();

    // declare string variable to store line from file
    string line;
    
    // while there are still lines in the file to read, read them to line and:
    while (getline(fin, line))
    {
        // insert line into remaining_monsters
        monsters_remaining_.push_back(line);
    }

    // indicate success
    return true;
}

void Assets::refreshMonsters()
{
    // if there are no more monsters that the player hasn't defeated, begin repeating

    // declare variable to store name of monster being moved from defeated to remaining
    string monster_name;
    
    // while there are still monsters in defeated_monsters,
    while (defeated_monsters_.size() > 0)
    {
        // get the last monster name and remove it from defeated_monsters
        monster_name = defeated_monsters_.back();
        defeated_monsters_.pop_back();
        
        // then, append it to monsters_remaining.
        monsters_remaining_.push_back(monster_name);
    }
}

// removes monster from monsters_remaining
void Assets::removeMonster(string monster)
{
    // for every index in monsters_remaining:
    for (int i = 0; i < monsters_remaining_.size(); i++)
    {
        // if the monster at that position is the monster, erase at that position
        if (monsters_remaining_[i] == monster)
        {
            monsters_remaining_.erase(monsters_remaining_.begin() + i);
        }
    }
    // add the monster to the vector of defeated monsters.
    defeated_monsters_.push_back(monster);
}

// apply results of transaction to results
// returns whether the operation was successful
bool Assets::handleTransaction(Transaction& t)
{
    // if the cost exceeds the amount of gold, indicate failure
    if (t.cost_ > gold_)
    {
        return false;
    }
    // remove the cost from gold
    gold_ -= t.cost_;
    // add the amount of ingredients in the transaction
    ingredients_ += t.ingredients_;
    
    /* 
        for every sub-asset that the transaction stores, change the amount
        of the sub-asset by the amount purchased / sold
    */
    for (pair<string, int> p : t.cookware_)
    {
        cookware_.at(p.first).count_ += p.second;
    }

    for (pair<string, int> p : t.weapons_)
    {
        weapons_.at(p.first).count_ += p.second;
    }

    // increase armor count
    armor_ct_ += t.armor_;

    for (pair<string, int> p : t.treasures_){
        treasures_.at(p.first).count_ += p.second;
    }
    // indicate success
    return true;
}

// change gold
void Assets::changeGold(int amt)
{
    /* 
        if the amount is negative (removing gold) and the amount removed will
        be larger than the current amount of gold, just set gold to 0
    */
    if (-1 * amt >= gold_){
        gold_ = 0;
    }
    // change gold by amount
    gold_ += amt;
}

int Assets::getGold()
{
    // return amount of gold
    return gold_;
}

// returns name of random weapon, or empty string if failure
string Assets::randomWeapon()
{
    srand(time(0));// seed random

    // declare and initialise variables
    int max_val = weapons_.size(); // maximum possible index to generate
    int key = rand() % max_val; // index of random weapon in weapons_
    int ct = 0; // current position in weapons_
    
    // for every name-object pair in weapons_:
    for (pair<string,Weapon> p : weapons_)
    {
        // if the index is correct, return the name
        if (ct == key)
        {
            return p.first;
        }
        // increment index
        ct++;
    }

    // return an empty string
    return "";
}

// returns name of random cookware, or empty string if failure
string Assets::randomCookware()
{
    srand(time(0)); // seed random

    // declare and initialise variables
    int max_val = cookware_.size(); // maximum possible index to generate
    int key = rand() % max_val; // index of random cookware in cookwares_
    int ct = 0; // current position in cookwares_

    // for every name-object pair in cookware_:
    for (pair<string,Cookware> p : cookware_)
    {
        // if the index is correct, return the name
        if (ct == key)
        {
            return p.first;
        }
        // increment index
        ct++;
    }

    // return an empty string
    return "";
}


// returns name of random treasure, or empty string if failure
string Assets::randomTreasure()
{
    srand(time(0)); // seed random

    // declare and initialise variables
    int max_val = treasures_.size(); // maximum possible index to generate
    int key = rand() % max_val; // index of random treasure in treasures_
    int ct = 0; // current position in treasures_

    // for every name-object pair in treasures_:
    for (pair<string,Treasure> p : treasures_)
    {
        // if the index is correct, return the name
        if (ct == key)
        {
            return p.first;
        }
        // increment index
        ct++;
    }

    // return an empty string
    return "";
}

// returns name of random monster
string Assets::randomMonster()
{

    // if there are no more monsters the player hasn't fought, recall all from defeated
    if (monsters_remaining_.size() == 0)
    {
        refreshMonsters();
    }
    
    // seed random
    srand(time(0));
    
    // declare and initialise variables
    int max_val = monsters_remaining_.size(); // maximum possible index to generate
    int key = rand() % max_val; // index of random monster

    return monsters_remaining_.at(key); // return the monster name at that position
}

// returns raw string of random puzzle
string Assets::randomPuzzle()
{
    // seed random
    srand(time(0));
    
    // declare / initialise variables
    int max_val = puzzles_.size(); // maximum possible index
    int key = rand() % max_val; // random index
    return puzzles_.at(key); // return the puzzle string at that position
}

// get the unmodified armor cost
int Assets::getArmorPrice()
{
    return armor_cost_;
}

// get the number of used armor sets
int Assets::getUsedArmor()
{
    return applied_armor_ct_;
}

// get the total number of armor sets purchased
int Assets::getTotalArmor()
{
    return armor_ct_;
}

// change the armor:
// false for unequipping a set, true for equipping a set
bool Assets::changeEquipArmor(bool status)
{
    // if wanting to equip and there are still armor to apply:
    if (status && armor_ct_ > applied_armor_ct_)
    {
        applied_armor_ct_++; // increase number of applied armor
        return true; // indicate success
    } else if (!status && applied_armor_ct_ > 0)
    {
        // if wanting to unequip and there is still armor to remove:
        applied_armor_ct_--; // decrease number of applied armor
        return true; // indicate success
    }
    return false; // indicate failure
}

// decrease the total amount of owned armor if it's not equipped on a player
bool Assets::removeArmor()
{
    // if there's still unequipped armor, remove it and return true
    if (armor_ct_ > applied_armor_ct_){
        armor_ct_--;
        return true;
    }
    return false;
}

// get number of players with weapons
int Assets::getNumWeapons()
{
    int res = 0; // stores result
    
    // for every pair in weapons_,
    for (pair<string, Weapon> p : weapons_)
    {
        // if the name is (none), skip.
        if (p.first == "(none)")
        {
            continue;
        }
        // otherwise, add the equipped parameter to result
        res += p.second.equipped_;
    }
    // return result
    return res;
}

// returns whether players have duplicate weapons
bool Assets::noDupWeapons()
{
    // for every pair in weapons_:
    for (pair<string, Weapon> p : weapons_)
    {
        // if more than 1 person has equipped the weapon, indicate failure
        if (p.second.equipped_ > 1)
        {
            return false;
        }
    }
    // indicate success
    return true;
}

// change the equipped count of a given weapon
// false for unequipping, true for equipping
bool Assets::changeEquipWeapon(string weap_name, bool status)
{
    // if weapon not found, indicate failure
    if (weapons_.find(weap_name) == weapons_.end())
    {
        return false;
    }

    // get weapon data of name
    Weapon& w = weapons_.at(weap_name);

    // if wanting to equip, and there are still weapons to equip or the weapon is none,
    if (status && (w.equipped_ < w.count_ || weap_name == "(none)"))
    {
        // increment equipped, indicate success
        w.equipped_++;
        return true;
    }
    else if (!status && w.equipped_ > 0) // otherwise, if wanting to unequip, and there are weapons equippedm
    {
        // decrement equipped, indicate success
        w.equipped_--;
        return true;
    }
    // indicate failure
    return false;
}

// remove a weapon, regardless of whether it's equipped or not
// returns whether removal was successful
bool Assets::removeWeapon(string weap_name)
{
    // if weapon not found, or the weapon name is none, indicate failure
    if (weapons_.find(weap_name) == weapons_.end() || weap_name == "(none)"){
        return false;
    }

    // get weapon data of name
    Weapon& w = weapons_.at(weap_name);

    // if the number of this weapon is larger than 0,
    if (w.count_ > 0)
    {
        // if all have been equipped, also decrement equipped
        if (w.count_ == w.equipped_)
        {
            w.equipped_--;
        }
        // decrement count and indicate success
        w.count_--;
        return true;
    }
    // indicate failure
    return false; 
}

// returns weapon modifier in combat formula
int Assets::getWeaponMod()
{

    // result is initially 0
    int res = 0;
    // for every name-data pair in weapons:
    for (pair<string, Weapon> p : weapons_)
    {
        // if it's the (none) option, skip
        if (p.first == "(none)")
        {
            continue;
        }
        // if the weapon has been equipped, add its stat and count to result
        if (p.second.equipped_ > 0)
        {
            res += p.second.equipped_;
            res += p.second.stat_;
        }
    }
    // return result
    return res;
}

// returns default ingredient price
int Assets::getIngredientPrice()
{
    return ingredients_cost_;
}

// changes ingredients by amount
void Assets::changeIngredients(int amt)
{
    // if amount is negative and exceeds current stock, set to 0
    if (-1 * amt > ingredients_)
    {
        ingredients_ = 0;
    }
    // otherwise, apply change
    ingredients_ += amt;
}

// returns ingredients amount
int Assets::getIngredients(){
    return ingredients_;
}

// attempts to cook kg amount of food.
// -1: error. 0: no result produced. otherwise returns fullness
int Assets::attemptCook(string ware_name, int kg)
{
    // if the cookware not found, indicate error
    if (cookware_.find(ware_name) == cookware_.end())
    {
        return -1;
    }
    // if the amount cooked exceeds the ingredients, indicate error
    if (kg > ingredients_)
    {
        return -1;
    }
    // if the amount of ingredients isn't a multiple of 5, indicate error
    if (kg % 5 != 0){
        return -1;
    }

    int chance = cookware_.at(ware_name).break_chance_; // get break chance
    srand(time(0)); // seed random
    int rand_res = (rand() % 100)+1; // get random result from 1 to 100: like a "percent"
    ingredients_ -= kg; // subtract ingredients
    
    // if the random result was less than the break chance
    if (rand_res <= chance)
    {
        // break cookware, indicate no result
        cookware_.at(ware_name).count_--;
        return 0;
    }
    
    // return amount of fullness applied
    return (kg / 5);
}

// change treasure of treasure name by amount
// returns success / failure of operaton
bool Assets::changeTreasures(string treasure_name, int amt)
{
    // if the treasure isn't found, indicate failure
    if (treasures_.find(treasure_name) == treasures_.end())
    {
        return false;
    }
    
    // get treasure data for name
    Treasure& t = treasures_.at(treasure_name);
    t.count_ += amt; // apply change to amount
    return true; // indicate success
}

// converts an index to a weapon name
string Assets::getWeapAt(int idx, bool in_merchant)
{
    int ct = 0; // current "index" in map

    // 0 is (none).
    // if in merchant, 0 shouldn't be a valid weapon, otherwise check if within map bounds
    if ((in_merchant && idx == 0) || idx >= weapons_.size() || idx < 0)
    {
        return ""; // return nothing
    }
    
    // for every pair in weapons_:
    for (pair<string,Weapon> p : weapons_)
    {
        // if the current index is the desired index, return the name in the pair
        if (ct == idx)
        {
            return p.first;
        }
        ct++; // increment current index
    }
    return ""; // return nothing
}

// converts an index to a cookware name
string Assets::getCookwareAt(int idx)
{
    int ct = 1; // current "index" in map
    
    // if index is out of bounds:
    if (idx < 1 || idx >= cookware_.size())
    {
        return ""; // return nothing
    }
    
    // for every pair in cookware_:
    for (pair<string,Cookware> p : cookware_)
    {
        // if the current index is the desired index, return the name in the pair    
        if (ct == idx)
        {
            return p.first;
        }
        ct++; // increment current index
    }
    return ""; // return nothing
}

// converts an index to a treasure name
string Assets::getTreasureAt(int idx)
{
    int ct = 1; // current "index" in map

    // if index is out of bounds:
    if (idx < 1 || idx >= treasures_.size())
    {
        return ""; // return nothing
    }

    // for every pair in treasures_:
    for (pair<string,Treasure> p : treasures_)
    {
        // if the current index is the desired index, return the name in the pair
        if (ct == idx)
        {
            return p.first;
        }
        ct++;
    }
    return ""; // return nothing
}

// gets weapon data for given name (modifiable reference)
Weapon& Assets::getWeapon(string w)
{
    return weapons_.at(w);
}

// gets cookware data for given name (modifiable reference)
Cookware& Assets::getCookware(string c)
{
    return cookware_.at(c);
}

// gets treasure data for given name (modifiable reference)
Treasure& Assets::getTreasure(string t)
{
    return treasures_.at(t);
}

// converts given weapon name's name and data into string.
string Assets::weaponToString(string weap_name)
{
    // "none" weapon has no extra data, return back
    if (weap_name == "(none)")
    {
        return weap_name;
    }

    // if weapon not found, return empty string
    if (weapons_.find(weap_name) == weapons_.end())
    {
        return "";
    }
    Weapon& w = getWeapon(weap_name); // get weapon data
    string res = weap_name; // start result with weapon name
    // add stat bonus
    res += " (+"; 
    res += to_string(w.stat_);
    res += " bonus) ";
    
    // add owned and applied count and return result
    res += "have: ";
    res += to_string(w.count_);
    res += ". equipped: ";
    res += to_string(w.equipped_);
    return res;
}

// converts given weapon name to string in a merchant setting
string Assets::weaponToString(string weap_name, double price_mod)
{
    // "none" weapon has no extra data, return back
    if (weap_name == "(none)")
    {
        return weap_name;
    }
    // get result of normal function
    string res = weaponToString(weap_name);

    // if result was empty, return an empty string
    if (res == "")
    {
        return "";
    }
    Weapon& w = getWeapon(weap_name);// get weapon data

    // add cost to string and return result
    res += ". costs ";
    res += to_string(int(w.cost_ * price_mod));
    return res;
}

// converts given cookware name to string
string Assets::cookwareToString(string ware_name)
{
    // if cookware not found, return empty string
    if (cookware_.find(ware_name) == cookware_.end())
    {
        return "";
    }
    Cookware& c = getCookware(ware_name); // get cookware data
    string res = ware_name; // start result with ware name

    // add break chance
    res += " (";
    res += to_string(c.break_chance_);
    
    // add owned count, return result
    res += "% break chance) have: ";
    res += to_string(c.count_);
    return res;
}

// converts given cookware name to string in merchant context
string Assets::cookwareToString(string ware_name, double price_mod)
{
    // get result of normal function
    string res = cookwareToString(ware_name);
    
    // if result was empty, return an empty string
    if (res == "")
    {
        return "";
    }
    Cookware& c = getCookware(ware_name); // get cookware data

    // add cost to string and return result
    res += ". costs: ";
    res += to_string(int(c.cost_ * price_mod));
    return res;
}

// converts given treasure name to string
string Assets::treasureToString(string treasure_name)
{
    // if treasure not found, return empty string
    if (treasures_.find(treasure_name) == treasures_.end())
    {
        return "";
    }
    Treasure& t = getTreasure(treasure_name); // get treasure data
    string res = treasure_name; // start result with treasure name

    // add owned count to result
    res += " have: ";
    res += to_string(t.count_);

    // add cost to result, return result
    res += ". sold for: ";
    res += to_string(t.cost_);
    return res;
}

// fill the map options with option number  - weapon information pairs.
void Assets::allWeapons(map<string, string>& options)
{
    int ct = 0; // current option number in weapons_
    // for every pair in weapons_:
    for (pair<string,Weapon> p : weapons_)
    {
        // set option number to string representation, increment option number
        options[to_string(ct)] = weaponToString(p.first);
        ct++;
    }
}

// fill the map options with option number-weapon information pairs in merchant
void Assets::allWeapons(map<string, string>& options, double price_mod)
{
    int ct = 1; // current option number
    // for every pair in weapons_:
    for (pair<string,Weapon> p : weapons_)
    {
        // if the pair is the (none) option, skip
        if (p.first == "(none)")
        {
            continue;
        }
        // set option number to string representation, increment option number
        options[to_string(ct)] = weaponToString(p.first, price_mod);
        ct++;
    }
}

// fill the map options with option number - cookware information pairs
void Assets::allCookware(map<string, string>& options)
{
    int ct = 1; // current option number
    // for every pair in cookware_:
    for (pair<string,Cookware> p : cookware_)
    {
        // set option number to string representation, increment option number
        options[to_string(ct)] = cookwareToString(p.first);
        ct++;
    }
}

// fill the map options with option number - cookware information pairs in merchant
void Assets::allCookware(map<string, string>& options, double price_mod)
{
    int ct = 1; // current option number
    // for every pair in cookware_:
    for (pair<string,Cookware> p : cookware_)
    {
        // set option number to string representation, increment option number
        options[to_string(ct)] = cookwareToString(p.first, price_mod);
        ct++;
    }
}

// fill the map options with option number - treasure information pairs
void Assets::allTreasures(map<string, string>& options)
{
    int ct = 1; // current option number
    // for every pair in treasures_:
    for (pair<string,Treasure> p : treasures_)
    {
        // set option number to string representation, increment option number
        options[to_string(ct)] = treasureToString(p.first);
        ct++;
    }
}