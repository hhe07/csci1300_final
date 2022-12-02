// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Merchant.h

#ifndef MERCHANT_H
    #define MERCHANT_H

    #include "Puzzle.h"
    #include "Assets.h"
    #include <map>
    #include <vector>
    #include <stdio.h>


    // class Merchant: stores basic merchant information, and provides many functions for interacting with merchant
    class Merchant
    {
    private:
        bool has_puzzle_; // basically only for first merchant
        Puzzle unlocking_puzzle_; // riddle to gain access

        Assets& assets_; // reference to assets to view info

        map<string,string>& options_; // reference to driver's options for memory efficiency

        bool has_monster_; // whether the merchant has a monster
        double price_modifier_; // price modifier
        
        /*
            -1: invalid input.
            0: main menu
            1: edit ingredients
            2: edit cookware
            3: edit weapons
            4: edit armor
            5: edit selling of treasures
            6: view transaction
            7: purchase transaction
            8: leave

            21: cookware confirmation menu
            31: weapon confirmation menu
            51: treasure confirmation menu

        
        */
        int submenu_; 
        string curr_sel_; // current selection of cookware / weapon / treasure type.
        Transaction curr_transaction_; // current transaction 
        bool is_closed_; // whether merchant has closed

        bool ready_to_transact_; // whether player is ready to transact
    public:
        Merchant(bool hasPuzzle, Assets& assets, map<string, string>& options); // constructor, mostly passes references

        bool hasPuzzle(); // whether merchant requires unlocking puzzle
        Puzzle& givePuzzle(); // returns reference to current puzzle

        void replacePuzzle(string puzzle); // replace the puzzle off raw string representation

        bool hasMonster(); // returns whether has monster
        void setMonster(bool b); // sets whether has monster

        // first string is of form:
        // 1: (weapon name / info)
        void getOptions(); // adds current menu options to options_
        bool selectOption(int sel); // attempt to select option

        void updateCost(); // update cost of transaction

        Transaction& getTransaction(); // get reference to transaction

        void updatePrices(int rooms_cleared_); // update price modifier based on rooms cleared
        bool isClosed(); // whether merchant is closed

        void transactionItemToString(string& s, map<string,int>& m); // convert a map within transaction to a string

        bool readyToTransact(); // whether ready to transact
        void gotTransaction(); // reset transaction after receiving and applying
        void reset(); // reset for next encounter

    };

#endif