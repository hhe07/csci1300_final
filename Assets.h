// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Assets.h

#ifndef ASSET_H
    #define ASSET_H

    #include <iostream>
    #include <map>
    #include <vector>
    #include <fstream>
    using namespace std;
    
    // stores data for a weapon
    struct Weapon
    {
        int stat_;
        int count_;
        int equipped_;
        int cost_;
        Weapon() = default;
    };

    // stores data for a treasure
    struct Treasure
    {
        int cost_;
        int count_;
        Treasure() = default;
    };
    
    // stores data for a cookware
    struct Cookware
    {
        int break_chance_; // out of 100
        int count_;
        int cost_;
        Cookware() = default;
    };

    // stores transaction data, which is how a merchant applies changes to party's items
    struct Transaction
    {
        int cost_ = 0;
        int ingredients_ = 0;
        map<string, int> cookware_; // name - modification amount
        map<string, int> weapons_; // name - modification amount
        int armor_ = 0;
        map<string, int> treasures_; // name - modification amount
        Transaction() = default;
    };

    // Assets class: stores parts that need to be loaded from files and items
    // Also provides funtcions for facilitating access / modification of these items
    class Assets
    {
    private:
        // name - data maps
        map<string, Weapon> weapons_;
        map<string, Cookware> cookware_;
        map<string, Treasure> treasures_;

        int armor_ct_ = 0; // number of armor owned
        int applied_armor_ct_ = 0; // number of armor worn
        int armor_cost_ = 5; // default cost of armor
        
        int ingredients_ = 0; // number of kg of ingredients
        int ingredients_cost_ = 1; // default cost of ingredients

        int gold_ = 100; // amount of gold

        vector<string> monsters_remaining_; // monsters player hasn't fought
        vector<string> defeated_monsters_; // monsters player has defeated

        vector<string> puzzles_; // puzzles as raw string
    public:
        Assets(/* args */);
        void split(string in, vector<string>& res, string delim); // splits string to vector given delimiter

        // loading block
        // load the given sub-assets.
        bool loadWeapons(string filename);
        bool loadPuzzles(string filename);
        bool loadCookwares(string filename);
        bool loadTreasures(string filename);

        // monsters block
        bool changeMonsters(string fn_temp, int level); // change monsters file given name and new difficulty
        void refreshMonsters(); // refresh monsters if not enough monsters remaining
        void removeMonster(string monster_name); // remove monster from monsters remaining given name
        
        // transactions block
        bool handleTransaction(Transaction& t); // apply transaction if able, otherwise return false.

        // gold block
        void changeGold(int amt);
        int getGold();

        // get random name of the given sub-asset.
        string randomWeapon();
        string randomCookware();
        string randomTreasure();
        string randomMonster();
        string randomPuzzle();

        // armor block
        int getArmorPrice(); // get default armor price
        int getUsedArmor(); // get the number of equipped armor
        int getTotalArmor(); // get total armor count
        bool changeEquipArmor(bool status); // increases / decreases applied armor count
        bool removeArmor(); // reduces amount of armor if they aren't equipped.

        // weapons block
        int getNumWeapons(); // get number of players with weapons
        bool noDupWeapons(); // returns whether duplicate weapons are equipped
        bool changeEquipWeapon(string weap_name, bool status); // change whether to equip / unequip a weap_name
        bool removeWeapon(string weap_name); // remove an weapon if it isn't fully equipped.
        int getWeaponMod(); // get weapon modifier for combat equation

        // ingredients and cooking block
        int getIngredientPrice(); // get default ingredient price
        void changeIngredients(int amt); // if goes negative, defaults to 0
        int getIngredients(); // get kg of ingredients
        int attemptCook(string ware_name, int kg); // attempt to cook. returns -1 if invalid, 0 if fail, fullness if success.

        // treasures block
        bool changeTreasures(string treasure_name, int amt); // change the amount of the given treasure. returns success.

        // converts an index from a store / menu to the applicable asset name
        string getWeapAt(int idx, bool in_merchant); // in_merchant to remove (none), so no purchasing of none
        string getCookwareAt(int idx);
        string getTreasureAt(int idx);

        //! UNSAFE
        // get references to the sub-asset named (string)
        Weapon& getWeapon(string w);
        Cookware& getCookware(string c);
        Treasure& getTreasure(string t);

        // convert sub-asset name + data to a string.
        // inclusion of price_mod implicitly means that price should be printed
        string weaponToString(string weap_name);
        string weaponToString(string weap_name, double price_mod); 
        string cookwareToString(string ware_name);
        string cookwareToString(string ware_name, double price_mod);
        string treasureToString(string treasure_name); // implicitly includes treasure sell price

        // list representations of every sub-asset to a map.
        // inclusion of price_mod implies use in merchant, hence print price
        void allWeapons(map<string, string>& options);
        void allWeapons(map<string, string>& options, double price_mod);
        void allCookware(map<string, string>& options);
        void allCookware(map<string, string>& options, double price_mod);
        void allTreasures(map<string, string>& options);

    };
    

#endif