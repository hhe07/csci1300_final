// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Merchant.cpp

#include "Merchant.h"

// Constructor:
Merchant::Merchant(bool hasPuzzle, Assets& assets, map<string, string>& options)
: assets_(assets), options_(options) // initialise reference values using parameters passed in
{
    has_puzzle_ = hasPuzzle; // set puzzle status
    reset(); // call reset, which initialises the other variables by proxy
}

// return has_puzzle_
bool Merchant::hasPuzzle()
{
    return has_puzzle_;
}

// return reference to puzzle
Puzzle& Merchant::givePuzzle()
{
    return unlocking_puzzle_;
}

// return whether has monster
bool Merchant::hasMonster()
{
    return has_monster_;
}

// translates a pair of name:count into a printable string stored in s
void Merchant::transactionItemToString(string& s, map<string, int>& m)
{
    // if m is empty, s is none
    if (m.size() == 0){
        s = "(none)";
    }
    // otherwise, list out name : amount
    for (pair<string, int> p : m)
    {
        s += p.first;
        s += ": ";
        s += to_string(p.second);
        s += " | ";
    }
}

// apply options to options_
void Merchant::getOptions()
{
    options_.clear(); // clear options
    string cookware_string; // cookware representation
    string weapons_string; // weapons representation
    string treasure_string; // treasure representation
    int ct = 0; // index
    switch(submenu_)
    {
        case 0: // main menu: print options
            options_["HEADER"] = "Choose one of the following:";
            options_["1"] = "Change Ingredients";
            options_["2"] = "Change Cookware";
            options_["3"] = "Change Weapons";
            options_["4"] = "Change Armor";
            options_["5"] = "Change Treasures sold";
            options_["6"] = "View transaction";
            options_["7"] = "Purchase transaction";
            options_["8"] = "Leave";
            break;
        case 1: // ingredients change: print prompt and cost info
            options_["HEADER"] = "How many kg of ingredients this time? They cost: "+ to_string(int(assets_.getIngredientPrice()*price_modifier_)) + " gold per kg. Enter a non-negative value";
            break;
        case 2: // cookware change: print prompt, list out cookwares
            options_["HEADER"] = "Which type of cookware?\nChoose one of the following:";
            assets_.allCookware(options_,price_modifier_);
            break;
        case 3: // weapon change: print prompt, list out modifiers
            options_["HEADER"] = "Which type of weapon?\nChoose one of the following:";
            assets_.allWeapons(options_,price_modifier_);
            break;
        case 4: // armor change: print prompt and cost info
            options_["HEADER"] = "How many armor? They cost: "+ to_string(int(assets_.getArmorPrice() * price_modifier_))+ " gold. Enter a non-negative value.";
            break;
        case 5: // treasure seling: print prompt, list out treasures
            options_["HEADER"] = "Which type of treasure?\nChoose one of the following:";
            assets_.allTreasures(options_);
            break;
        case 6: // view transaction details:
            options_["HEADER"] = "Transaction Details:\nEnter any number to exit.";
            // display change to gold and ingredients
            options_["1. Change to gold:"] = to_string(-1 * curr_transaction_.cost_);
            options_["2. Ingredients:"] = to_string(curr_transaction_.ingredients_);

            // convert cookware part of curr_transaction to a string and add it in
            transactionItemToString(cookware_string,curr_transaction_.cookware_);
            options_["3. Cookware:"] = cookware_string;

            // convert weapon part of curr_transaction to a string and add it in
            transactionItemToString(weapons_string,curr_transaction_.weapons_);
            options_["4. Weapons:"] = weapons_string;

            // display change to armor
            options_["5. Armor:"] = to_string(curr_transaction_.armor_);

            // convert treasure part of curr_transaction to a string and add it in
            transactionItemToString(treasure_string,curr_transaction_.treasures_);
            options_["6. Treasures:"] = treasure_string;
            break;
        case 7: // apply transaction: prompt user
            options_["HEADER"] = "Apply Transaction? Enter 1 to apply, enter any other value to exit.";
            break;
        case 8: // exit shop: prompt user
            options_["HEADER"] = "Exiting shop... Enter any numerical key...";
            is_closed_ = true;
            break;
        case 21: // cookware / weapon quantity: prompt user for amount
        case 31:
            options_["HEADER"] = "How many " + curr_sel_ + " do you want? Enter a non-negative value.";
            break;
        case 51: // treasures sold quantity: prompt user for amount
            options_["HEADER"] = "How many " + curr_sel_ + " do you want to sell? Enter a non-negative value.";
            break;
    }
    // if in a submenu and not in transaction / exit submenus, add a return to main merchant menu option
    if (submenu_ > 0 && submenu_ != 8 && submenu_ != 7)
    {
        options_["0"] = "Return to main menu.";
    }
}


// returns whether selection of option was successful
bool Merchant::selectOption(int sel)
{
    // prevent negative input
    if (sel < 0)
    {
        return false;
    }
    // if at main menu,
    if (submenu_ == 0)
    {
        if (sel > 8) // prevent selection out of bounds
        {
            return false;
        }
        if (sel == 6) // before displaying transaction, update cost
        {
            updateCost();
        }
        submenu_ = sel; // select the next submenu to display
        return true;
    }

    // otherwise, if in a relevant submenu and 0 selected, return to main menu
    if (sel == 0 && submenu_ != 8 && submenu_ != 7)
    {
        submenu_ = 0;
        return true;
    }
    switch (submenu_){
        case 1:
            // change transaction's amount of ingredients.
            curr_transaction_.ingredients_ = sel;
            break;
        case 2:
            // change current selection to type of cookware at index
            curr_sel_ = assets_.getCookwareAt(sel);
            if (curr_sel_ == "") // if empty string, notify of invalid input
            {
                return false;
            }
            submenu_ = 21; // continue to next submenu
            return true;
        case 3:
            // change current selection to type of weapon at index
            curr_sel_ = assets_.getWeapAt(sel, true);
            if (curr_sel_ == "") // if empty string, notify of invalid input
            {
                return false;
            }
            submenu_ = 31; // continue to next submenu
            return true;
        case 4:
            // change transaction's amount of armor
            curr_transaction_.armor_ = sel;
            break;
        case 5:
            // change current selection to type of treasure at index
            curr_sel_ = assets_.getTreasureAt(sel);
            if (curr_sel_ == "") // if empty string, notify of invalid input
            {
                return false;
            }
            submenu_ = 51; // continue to next submenu
            return true;
        case 7:
            // if selected yes, update cost and update ready_to_transact
            if (sel == 1)
            {
                updateCost();
                ready_to_transact_ = true;
            }
            break;
        case 8:
            // set is_closed_, allowing exit out of loop
            is_closed_ = true;
            return true;
        case 21:
            // update number of cookware to purchase
            curr_transaction_.cookware_[curr_sel_] = sel;
            break;
        case 31:
            // update number of weapons to purchase
            curr_transaction_.weapons_[curr_sel_] = sel;
            break;
        case 51:
            // if selection exceeds the number of this type of treasure the player has, return false
            if (sel > assets_.getTreasure(curr_sel_).count_)
            {
                return false;
            }
            // update number of treasures to remove
            curr_transaction_.treasures_[curr_sel_] = -1 * sel;
            break;
    }
    // otherwise for case 1 and 4, return to original and return true
    // them and case 6 is implicit in the following.
    submenu_ = 0; // return to main menu after reading selection
    return true;
}

// update price modifier: add 20% for every room cleared
void Merchant::updatePrices(int rooms_cleared)
{
    price_modifier_ += (0.2) * rooms_cleared;
}


// replace puzzle:
void Merchant::replacePuzzle(string puzzle)
{
    // split like in other instances, but without the generalisation
    // https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    has_puzzle_ = true; // set puzzle if it hasn't been already
    // get first segment - riddle
    string prompt = puzzle.substr(0,puzzle.find("~"));
    puzzle.erase(0,puzzle.find("~") + 1);

    // get secong segment - solutiion
    string sol = puzzle.substr(0,puzzle.find("~"));
    puzzle.erase(0,puzzle.find("~") + 1);
    unlocking_puzzle_ = Puzzle(prompt,sol,3,false); // initialise with 3 attempts
}

// set whether has a monster
void Merchant::setMonster(bool b)
{
    has_monster_ = b;
}

// get whether ready to transact
bool Merchant::readyToTransact()
{
    return ready_to_transact_;
}

// get whether is closed
bool Merchant::isClosed()
{
    return is_closed_;
}

// reset:
void Merchant::reset()
{
    has_monster_ = false; // no more monster
    curr_transaction_ = Transaction(); // reset transaction
    submenu_ = 0; // return to main menu
    // reset all other misc. parameters
    is_closed_ = false;
    ready_to_transact_ = false;
    curr_sel_ = "";
    price_modifier_ = 1.0;
}

// updates cost:
void Merchant::updateCost()
{
    // second in pair is always the amount
    int cost = 0;
    cost += (curr_transaction_.ingredients_ * assets_.getIngredientPrice() * price_modifier_); // calculate ingredient cost
    for (pair<string, int> p : curr_transaction_.cookware_) // for every pair in cookware purchased:
    {
        // add the unit cost * amount * price modifier to cost
        cost += assets_.getCookware(p.first).cost_ * p.second * price_modifier_;
    }
    for (pair<string, int> p : curr_transaction_.weapons_) // for every pair in weapons purchased:
    {
        // add the unit cost * amount * price modifier to cost
        cost += assets_.getWeapon(p.first).cost_ * p.second * price_modifier_;
    }
    cost += curr_transaction_.armor_ * assets_.getArmorPrice() * price_modifier_; // add armor cost
    for (pair<string, int> p : curr_transaction_.treasures_) // for every pair in treasures sold:
    {
        // "add" the unit cost * amount (amount is negative so cost is reduced)
        cost += assets_.getTreasure(p.first).cost_ * p.second; 
    }
    curr_transaction_.cost_ = cost; // apply new cost
}

// return curr_transaction_
Transaction& Merchant::getTransaction()
{
    return curr_transaction_;
}

// confirm transaction by resetting ready_to_transact and the transaction object
void Merchant::gotTransaction(){
    ready_to_transact_ = false;
    curr_transaction_ = Transaction();
}