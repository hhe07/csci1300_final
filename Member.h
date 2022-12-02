// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Member.h

#ifndef MEMBER_H
    #define MEMBER_H
    #include<string>
    using namespace std;

    // Member class: stores information related to individual party members
    // also provides setters / getters to that information
    class Member
    {
        private:
            bool is_trapped_ ; // either hostage or in previous room
            bool is_dead_; // whether is dead
            bool has_armor_; // whether has armor
            int fullness_; // fullness amount
            string weapon_; // weapon name

        public:
            Member(); // default constructor
            void equipArmor(); // remove an armor
            void removeArmor(); // equip an armor
            bool hasArmor(); // whether has armor

            void equipWeapon(string); // equip weapon name
            bool hasWeapon(); // whether has a weapon
            string removeWeapon(); // remove and return weapon name
            string getWeapon(); // get weapon name
            string toStr(); // return string representation

            bool isHostage(); // whether is hostage
            void holdHostage(); // set hostage
            void releaseHostage(); // release hostage

            bool isDead(); // whether is dead

            int getFullness(); // return fullness
            void addFullness(int f); // add fullness
            void removeFullness(int f); // remove fullness

            void setDead(); // set dead
    };
#endif