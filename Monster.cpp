// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Monster.cpp

#include "Monster.h"

// default constructor: fill in blank values
Monster::Monster(){
    level_ = 0;
    name_ = "";
}

// paramterised constructor: fill in values passed in
Monster::Monster(int level, string name){
    level_ = level;
    name_ = name;
}

// return level
int Monster::getLevel(){
    return level_;
}

// returns r2 * c component.
int Monster::getCombatModifier(){
    srand(time(0));
    int r2 = (rand() % 6) + 1;  // gets r2 from 1 to 6
    return r2 * level_;
}

// return name
string Monster::getName(){
    return name_;
}

// replace level with input
void Monster::replaceLevel(int level){
    level_ = level;
}

// replace name with input
void Monster::replaceName(string name){
    name_ = name;
}