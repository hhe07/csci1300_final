// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Monster.h

#ifndef MONSTER_H
    #define MONSTER_H


    #include <iostream>
    using namespace std;

    /**
     * Monster class:
     * stores level, name, and handles the monster's part of the combat
     * equation.
     * 
    **/
    class Monster
    {
    private:
        int level_;
        string name_;
    public:
        Monster(/* args */);
        Monster(int level, string name);

        int getCombatModifier();
        int getLevel();
        string getName();
        void replaceLevel(int);
        void replaceName(string);

    };
    
#endif