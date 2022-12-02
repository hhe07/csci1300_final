// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Party.cpp

#include "Party.h"

// Constructor:
Party::Party(Assets& assets)
: assets_(assets) // initialise assets_ to parameter
{
    // clear numerical parameters
    rooms_cleared_ = 0;
    keys_ = 0;
    effective_members_ = 0;
}

// attempt to add a member.
bool Party::addMember(string name)
{
    // if name is found in members, return false
    if (members_.find(name) != members_.end())
    {
        return false;
    }
    // otherwise, declare new member at position, increment effective members, return true
    members_[name] = Member();
    effective_members_++;
    return true;
}

// attempt to set player
bool Party::setPlayer(string name)
{
    // if name not found, return false
    if (members_.find(name) == members_.end())
    {
        return false;
    }
    // set player name and return true
    player_name_ = name;
    return true;
}

// return basic string of rooms cleared and key count
string Party::listBasicStats()
{
    string res = "Rooms cleared: ";
    res += to_string(rooms_cleared_);
    res += " | Keys: ";
    res += to_string(keys_);
    return res;
}

// list basic party info to vector
void Party::listPartyInfo(vector<string>& v)
{
    v.clear(); // clear vector
    string tmp;
    v.push_back("Effective members: " + to_string(effective_members_)); // put in header
    for (pair<string, Member> p : members_) // for ever name:info pair,
    {
        // add name, then string of info to tmp, then add to vector
        tmp = p.first;
        if (p.first == player_name_)
        {
            tmp += " (you) ";
        } else
        {
            tmp += "       ";
        }
        tmp += p.second.toStr();
        v.push_back(tmp);
    }
}

// list inventory to vector v
void Party::listInventory(vector<string>& v){
    v.clear(); // clear vector

    // begin strings for cookware, weapons, and treasure
    string cookware_string = "Cookware: \n";
    string weapons_string = "Weapons: \n";
    string treasure_string = "Treasures: \n";
    
    // temporary map for getting info from assets
    map<string, string> tmp;

    // put in gold / ingredients
    v.push_back("Gold: " + to_string(assets_.getGold()));
    v.push_back("Ingredients: " + to_string(assets_.getIngredients()) + " kg");

    // read cookwares to tmp,
    assets_.allCookware(tmp);
    if (tmp.size() == 0) // if have no cookware, indicate that
    {
        cookware_string += "    (none)";
    }
    
    // for every pair in tmp:
    for (pair<string, string> p : tmp)
    {
        // only add the second part, the selection number isn't important
        cookware_string += "    " + p.second + "\n";
    }
    v.push_back(cookware_string); // add cookware_string to vector

    tmp.clear(); // clear tmp, then load weapons using same process as above
    assets_.allWeapons(tmp);
    for (pair<string, string> p : tmp){
        if (p.second == "(none)"){
            continue;
        }
        weapons_string += "    " + p.second + "\n";
    }
    v.push_back(weapons_string);


    // put in total / equipped armor counts
    v.push_back("Total armor: " + to_string(assets_.getTotalArmor()));
    v.push_back("Equipped armor: " + to_string(assets_.getUsedArmor()));

    tmp.clear(); // clear tmp, then load treasures using same process as above
    assets_.allTreasures(tmp);
    for (pair<string, string> p : tmp)
    {
        treasure_string += "    " + p.second + "\n";
    }
    v.push_back(treasure_string); // add treasure string to vector
}

// attempt to change name's armor to status
bool Party::changeArmor(string name, bool status)
{
    // if name not found, return false
    if (members_.find(name) == members_.end()){
        return false;
    }
    // if want to equip and name doesn't have and still have armor to equip, change member and assets armor
    if (status && !members_.at(name).hasArmor() && assets_.getTotalArmor() > assets_.getUsedArmor())
    {
        members_.at(name).equipArmor();
        return assets_.changeEquipArmor(true); // return result of change
    }
    // if want to unequip and name has, change member and assets armor
    if (!status && members_.at(name).hasArmor())
    {
        members_.at(name).removeArmor();
        return assets_.changeEquipArmor(false); // return result of change
    }
    return false; // return false
}

// attempt to change name's weapon to weapon
bool Party::changeWeapons(string name, string weapon)
{
    // if name not found, return false
    if (members_.find(name) == members_.end()){
        return false;
    }
    // if weapon not found, return false
    if (assets_.weaponToString(weapon) == ""){
        return false;
    }
    
    Member& m = members_.at(name); // get reference to member at position
    if (m.hasWeapon()) // if member has a weapon,
    {
        // remove it, and put it back in unequipped
        string tmp = m.removeWeapon();
        if (!assets_.changeEquipWeapon(tmp, false)) // if unequipping fails, return false
        {
            return false;
        }
    }
    // if equipping the weapon in assets fails, return false
    if (!assets_.changeEquipWeapon(weapon,true))
    {
        return false;
    }
    // equip weapon to member and return true
    m.equipWeapon(weapon);
    return true;
}

// remove weapon with name
void Party::removeWeapon(string name)
{
    // if weapon isn't found, exit
    if (assets_.weaponToString(name) == "")
    {
        return;
    }
    // otherwise, if removing in assets succeeds:
    if (assets_.removeWeapon(name))
    {
        // for every member in members:
        for (pair<string, Member> p : members_)
        {
            if (p.second.getWeapon() == name) // if they're the first person with that weapon,
            {
                members_.at(p.first).removeWeapon(); //remove that weapon from them
                return;
            }
        }
    }
}


// get random player
string Party::randomPlayer(bool include_player)
{
    int max_val = effective_members_; // maximum index to generate
    if (!include_player) // if excluding player, decrease max_val by 1
    {
        max_val--;
    }
    if (max_val == 0) // if no players, return empty string
    {
        return "";
    }
    srand(time(0)); // seed random
    int ct = 0; // index of current name-data pair
    int key = rand() % max_val; // get a random desired index
    for (pair<string, Member> p : members_)
    {
        // if not including player and we've encountered the player name, skip the below
        if (!include_player && p.first == player_name_)
        {
            continue;
        }
        // if the current member's dead or hostage, skip the below
        if (p.second.isDead() || p.second.isHostage())
        {
            continue;
        }
        // if the current index matches the random desired index, return the name
        if (ct == key)
        {
            return p.first;
        } 
        ct++; // increment index
    }
    return ""; // return empty string if all else fails
}

// attempt to reduce fullness of players
bool Party::reduceFullness(string name, int amt)
{
    bool res = false; // whether anyone died
    if (name == "ALL") // if applying to all, 
    {
        // for every effective member in members_,
        for (pair<string, Member> p : members_)
        {
            if (isEffective(p.first))
            {
                members_.at(p.first).removeFullness(amt); // remove the amount of fullness
                if (p.second.isDead()) // if they died, remove them and set res to true
                {
                    removeMember(p.first,false);
                    res = true;
                }
            }
        }
        return res; // return result
    }
    // if name not found, return false
    if (members_.find(name) == members_.end()){
        return false;
    }
    members_.at(name).removeFullness(amt); // decrease specific name's fullness
    if (members_.at(name).isDead()) // if they died, remove them and set res to true
    {
        removeMember(name, false);
        return true;
    }
    return false;
}

// returns -1 if cookware not found or insufficient ingredients or invalid input. returns 0 if cookware breaks.
int Party::attemptCook(string ware_name, int kg)
{
    int cook_res = assets_.attemptCook(ware_name,kg); // attempt on assets side
    if (cook_res == -1) // if invalid input on assets side, return -1
    {
        return -1;
    }
    
    // for every member in members:
    for (pair<string,Member> p : members_)
    {
        // if isn't dead or out of party, apply fullness 
        if (!p.second.isDead() && !p.second.isHostage())
        {
            p.second.addFullness(cook_res);
        }
    }
    return cook_res; // return amount of fullness applied
}

// returns warning of low fullness
string Party::warnFullness()
{
    string res = "The following party member(s) are at low fullness: ";
    for (pair<string,Member> p : members_){
        // if isn't dead or out of party, check for fullness
        if (!p.second.isDead() && !p.second.isHostage()){
            // if less than 10 fullness, add to resu;t
            if (p.second.getFullness() < 10)
            {
                res += p.first;
                res += " | ";
            }
        }
    }
    return res; // return result
}

// true: hold hostage. false: kill
void Party::removeMember(string name, bool type){
    // if name not found, exit
    if (members_.find(name) == members_.end()){
        return;
    }
    // if member already removed, exit
    if (members_.at(name).isDead() || members_.at(name).isHostage()){
        return;
    }

    string weapon_name; // store their weapon if they have one
    if (type) // hold hostage
    {
        members_.at(name).holdHostage();
    } else // otherwise, kill
    {
        members_.at(name).setDead();
    }
    
    // if they have armor, remove from both member and assets
    if (members_.at(name).hasArmor())
    {
        members_.at(name).removeArmor();
        assets_.changeEquipArmor(false);
    }
    // reduce effective members
    effective_members_--;
    // remove weapon from both member and assets
    weapon_name = members_.at(name).removeWeapon();
    assets_.changeEquipWeapon(weapon_name,false);
}

// no weapon is counted as an exclusive weapon type
int Party::getCombatModifier()
{
    srand(time(0)); // seed random
    int r1 = 1 + (rand() % 6); // generate r1: 1-6
    int w = assets_.getWeaponMod(); // get weapon modifier w from assets_
    int res = r1*w; // create variable res = r1*w
    bool noDupes = assets_.noDupWeapons(); // ask assets_ if any duplicate weapons
    if (noDupes) // if no duplicates, add 4 to res
    {
        res += 4;
    }
    return res; // return result
}

// add a key
void Party::addKey()
{
    keys_++;
}

// attempt to remove key. if no more keys, return false, otherwise return true
bool Party::removeKey()
{
    if (keys_ == 0)
    {
        return false;
    }
    keys_--;
    return true;
}

// increment rooms cleared
void Party::incrementRooms()
{
    rooms_cleared_++;
}

// return rooms cleared
int Party::getRooms()
{
    return rooms_cleared_;
}

// get all member names
vector<string> Party::getMembers(bool includePlayer)
{
    vector<string> res; // vector for storing result
    // for every member in members_:
    for (pair<string,Member> p : members_)
    {
        // if at player_name_ and including player, add name to vector
        if (p.first == player_name_)
        {
            if (includePlayer){
                res.push_back(p.first);
            }
            else // otherwise, skip below
            {
                continue;
            }
        }
        else // otherwise, add name
        {
            res.push_back(p.first);
        }
    }
    return res; // return result
}

// return whether member has armor
bool Party::hasArmor(string name)
{
    // if member not found, return false
    if (members_.find(name) == members_.end())
    {
        return false;
    }
    // otherwise, return from members_
    return members_.at(name).hasArmor();
}

// get effective_members_
int Party::getEffectiveMembers()
{
    return effective_members_;
}

// get whether the player is dead
bool Party::isPlayerDead()
{
    return (members_.at(player_name_).isDead());
}

// get whether the person is effective: neither dead nor hostage
bool Party::isEffective(string name)
{
    return (!members_.at(name).isDead() && !members_.at(name).isHostage());
}

// get player name
string Party::getPlayerName()
{
    return player_name_;
}

// get whether string s is a member name
bool Party::isMember(string s)
{
    // if not found, return false
    if (members_.find(s) == members_.end())
    {
        return false;
    }
    return true; // else, return true
}

// return number of keys
int Party::keys()
{
    return keys_;
}

// overriding =:
Party& Party::operator=(Party&& p)
{
    Party tmp(p.assets_); // create temporary party using p.assets_
    // otherwise, transfer all remaining variables over
    tmp.rooms_cleared_ = p.rooms_cleared_;
    tmp.keys_ = p.keys_;
    tmp.members_ = p.members_;
    tmp.effective_members_ = p.effective_members_;
    tmp.player_name_ = p.player_name_;
    return tmp;
}