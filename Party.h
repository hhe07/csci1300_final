// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Party.h

#ifndef PARTY_H
    #define PARTY_H

    #include "Merchant.h"
    #include "Member.h"
    #include <iostream>
    #include <functional>
    #include <map>
    using namespace std;
    
    // Party: stores information about members, rooms cleared, and keys (stuff the party owns)
    // and provides functions for interacting with these
    class Party
    {
    private:
        int rooms_cleared_; // rooms cleared
        int keys_; // keys

        Assets& assets_; // reference to assets
        
        map<string, Member> members_; // members
        int effective_members_; // alive and not hostage count
        string player_name_; // player's name
        
    public:
        Party(Assets& assets_); // constructor
        bool addMember(string name); // tries to add member. false: repeat name. true: okay.
        bool setPlayer(string name); // tries to set player name. false: invalid name. true: okay.

        string listBasicStats(); // lists rooms cleared and keys. still needs anger level


        void listPartyInfo(vector<string>&); // list party member info to the vector.
        void listInventory(vector<string>&); // list inventory info to the vector

        bool changeArmor(string name, bool status); // change the armor on name. returns whether it was successful
        bool changeWeapons(string name, string weapon); // change the weapon on name. returns whether it was successful
        void removeWeapon(string name); // remove weapon

        string randomPlayer(bool include_player); // get random player
        
        bool reduceFullness(string name, int amt); // if name is ALL, apply to all returns whether anyone dies. true = someone died.

        int attemptCook(string ware_name, int kg); // calling here actually applies the fullness.

        string warnFullness(); // return warning if party members are at a critical level.

        // both return armor and weapon to inventory
        // false: kill. true: trap.
        void removeMember(string name, bool type);

        vector<string> getMembers(bool includePlayer); // get a list of member names

        int getCombatModifier(); // get combat modifier for party

        void addKey(); // add a key
        void incrementRooms(); // increment rooms cleared
        int getRooms(); // get rooms cleared
        bool removeKey(); // remove a key
        int keys(); // return number of keys

        bool hasArmor(string name); // return whether person has armor
        int getEffectiveMembers(); // returns number of effective members
        bool isPlayerDead(); // returns whether player died
        bool isEffective(string name); // returns whether name is effective
        string getPlayerName(); // returns player name

        bool isMember(string s); // returns whether name s is a member

        Party& operator=(Party&&); // override equals operator for compilation

    };
    

#endif