// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Driver.h

#ifndef DRIVER_H
    #define DRIVER_H

    #include "Party.h"
    #include "Monster.h"
    #include "Merchant.h"
    #include "Map.h"
    #include "Puzzle.h"
    #include <vector>
    #include <fstream>

    // Driver stores the core game data and provides the actual functions called by main
    class Driver
    {
    private:
        Party party_; // current party

        Map map_; // current map
        int explored_ct_; // number of explored spaces

        Assets& assets_; // reference to assets bundle

        Monster curr_monster_; // current monsters object
        int curr_monster_lvl_; // current monster level
        int monster_ct_; // number of monsters defeated
        bool is_fighting_; // whether or not currently fighting
        bool was_defeated_; // whether or not the monster was defeated

        Puzzle curr_puzzle; // current puzzle object
        bool is_puzzle_; // whether or not currently solving puzzle

        Merchant curr_merchant_; // current merchant object
        bool is_merchant_; // whether or not in merchant menu

        int sorcerer_anger_; // sorcerer's anger
        bool sorcerer_defeated_; // whether or not sorcerer was defeated
        bool given_up_; // whether or not given up
        bool last_locked_; // whether or not the last location was a locked room

        bool in_move_menu_; // whether or not in move menu
        int in_cook_menu_; // status in cook menu. 1:  choose cookware. 2: choose ingredients amount. -1: not in menu

        // status in inventory menu
        // -1: not in menu 0: main selection.  1: modify armor. 2: modify weapons
        // 11: choose player. 12: choose status
        // 21: choose player. 22: choose status.
        int in_inventory_menu_;
 
        string member_sel;// currently selected member name
        string status_sel; // currently selected status name
        string cookware_sel; // currently selected cookware name
        int ingredients_sel; // currently selected amount of ingredients

        bool allowed_fight; // whether or not player is allowed to fight

        map<string, string> options_; // options at current menu level

        int turns_ct_; // how many turns have passed
        string extra_; // extra information
        bool initial_end_; // whether or not initial merchant has passed

    public:
        Driver(Assets& a); // constructor: pass in assets loaded from file(s)

        void init(); // create map and set up all other data
        bool addMember(string name); // add a member to the party. 
        bool setPlayer(string name); // set the player in the party

        int randX(); // random x coord on map
        int randY(); // random y coord on map

        map<string,string>& getOptionsMerchant(); // get options if in a merchant
        bool receiveOptionMerchant(string option); // receive options if in a merchant
        bool initialEnd(); // returns if initial merchant is over

        map<string, string>& getOptions(); // get choices at current position
        // receive option at current position. if isn't valid for position, return false
        bool receiveOption(string option); // receive options from player input
        bool receiveRoomOption(string option); // receive options if in a room
        string getExtra(); // get extra information
        void clearExtra(); // clear extra information

        bool movePlayer(char dir); // move player in direction

        string investigate(); // attempt investigation and return results
        string interact(); // if the current room has an npc, give puzzle.
        string fight(); // attempt a fight and return results
        string surrender(); // attempt to surrender and return results
        string cook(string ware, int kg); // attempt to cook and return results
        string giveUp(); // attempt to give up and return results

        void printMap(); // print map


        string misfortune();// attempt random misfortune and return results
        string postFight(); // handle post fight actions and return results
        string postMove(); // handle post move actions and return results
        bool isGameOver(); // return whether game is over

        void randomMonster(); // set curr_monster_ to a random monster

        void updateMonsters(); // update monsters if level has changed
        string printLeaderboard(); // return leaderboard as string
        string printEndgame();// return endgame as string
        string getStatusUpdate(); // return status update as string

        bool inMenu(); // returns whether if currently in a menu

    };
    
    
#endif