// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Member.cpp

#include "Member.h"


// default constructor:
Member::Member()
{
    // set all bools to false
    is_trapped_ = false;
    is_dead_ = false;
    has_armor_ = false;
    weapon_ = "(none)"; // none is a valid weapon name
    fullness_ = 50; // start with 50 fullness
}

// set has_armor_ to true
void Member::equipArmor()
{
    has_armor_ = true;
}

// set has_armor_ to false
void Member::removeArmor()
{
    has_armor_ = false;
}

// return has_armor_
bool Member::hasArmor()
{
    return has_armor_;
}

// set weapon_ to s
void Member::equipWeapon(string s)
{
    weapon_ = s;
}

// remove weapon
string Member::removeWeapon()
{
    string tmp = weapon_; // temporarily keep weapon name here
    weapon_ = "(none)"; // set weapon_ to none
    return tmp; // return original weapon name
}

// string representation:
string Member::toStr()
{
    string res = ": ";
    // if status effects, apply those first
    if (is_dead_)
    {
        res += "dead.";
    }
    else if (is_trapped_)
    {
        res += "trapped. ";
    }
    else // otherwise, if no status effects,
    {
        // add fullness info
        res += "fullness: ";
        res += to_string(fullness_);

        // add weapon and armor info
        res += " | weapon: ";
        res += weapon_;
        if (has_armor_)
        {
            res += " | has armor.";
        }
        else
        {
            res += " | no armor.";
        }
    }
    return res; //return result
}

// get fullness_
int Member::getFullness()
{
    return fullness_;
}

// add f to fullness. if exceeds 50, reset to 50.
void Member::addFullness(int f)
{
    fullness_ += f;
    if (fullness_ > 50){
        fullness_ = 50;
    }
}

// remove f from fullness. if less than 0, set dead
void Member::removeFullness(int f)
{
    fullness_ -= f;
    if (fullness_ < 0)
    {
        is_dead_ = true;
    }
}

// return is_trapped_
bool Member::isHostage()
{
    return is_trapped_;
}

// return whether has a weapon. if weapon isn't (none), return true. otherwise, return false
bool Member::hasWeapon()
{
    if (weapon_ != "(none)"){
        return true;
    }
    return false;
}

// return weapon_
string Member::getWeapon()
{
    return weapon_;
}

// set is_trapped to true
void Member::holdHostage()
{
    is_trapped_ = true;
}

// set is_trapped to false
void Member::releaseHostage()
{
    is_trapped_ = false;
}

// return is_dead_
bool Member::isDead()
{
    return is_dead_;
}

// set is_dead_ to true
void Member::setDead()
{
    is_dead_ = true;
}