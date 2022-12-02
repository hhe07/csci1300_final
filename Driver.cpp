// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Driver.cpp
#include "Driver.h"

void split(string in, vector<string>& res, string delim){
    /* 
        inspired by:
        https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    */
    // declare temporary string to store the modified string
    string tmp = in;

    // clear the results vector: prevents unexpected values from being in tere
    res.clear();
    
    // while the delimiter can still be found:
    while (tmp.find(delim) != string::npos)
    {
        // push back a substring up to the delimiter position
        res.push_back(tmp.substr(0,tmp.find(delim)));
        // then, remove that segment from the string
        tmp = tmp.erase(0,tmp.find(delim) + 1);
    }
    // push back the trailing end after the last delimiter.
    res.push_back(tmp);
}

// constructor: needs asset because it is a reference.
Driver::Driver(Assets& a): 
party_(a), curr_merchant_(false, assets_,options_), assets_(a)
{
    // above line passes reference to all classes that require it (compiler requires)
    assets_ = a;
    party_ = Party(a); // initialise a Party object
    curr_puzzle = Puzzle(); // initialise an empty Puzzle object
}

// returns result of attempting party's addMember
bool Driver::addMember(string name)
{
    return party_.addMember(name);
}

// returns result of attempting party's setPlayer
bool Driver::setPlayer(string name){
    return party_.setPlayer(name);
}

// initialise map and set all relevant parameters to initial values
void Driver::init()
{
    // attempt to add 5 rooms
    for (int i = 0; i < 5; i++)
    {
        // if adding a room at a given random position fails, try again
        if (!map_.addRoom(randX(),randY()))
        {
            i--;
        }
    }

    // attempt to add 5 NPCs
    for (int i = 0; i < 5; i++)
    {
        // if adding a NPC at a given random position fails, try again
        if (!map_.addNPC(randX(),randY()))
        {
            i--;
        }
    }

    // set all values to initial values, and make sure not in any menus
    explored_ct_ = 0;
    curr_monster_lvl_ = 1;
    monster_ct_ = 0;
    is_fighting_ = false;
    was_defeated_ = false;
    is_puzzle_ = false;
    is_merchant_ = false;
    sorcerer_anger_ = 0;
    sorcerer_defeated_ = false;
    given_up_ = false;
    last_locked_ = false;
    in_move_menu_ = false;
    in_cook_menu_ = -1;
    in_inventory_menu_ = -1;
    ingredients_sel = -1;
    allowed_fight = false;
    turns_ct_ = 0;
}


// return random x coordinate bounded by number of rows on map
int Driver::randX()
{
    srand(time(0));
    return rand() % map_.getNumRows();
}

// returns random y coordinate bounded by number of columns on map
int Driver::randY()
{
    srand(time(0));
    return rand() % map_.getNumCols();
}

// changes current monster to a random monster
void Driver::randomMonster()
{
    string monster = assets_.randomMonster(); // get random monster name
    curr_monster_lvl_ = min(party_.getRooms() + 1,5); // update current monster level: min of rooms cleared or 5
    // replace level and name
    curr_monster_.replaceLevel(curr_monster_lvl_);
    curr_monster_.replaceName(monster);
    
    // indicate that in fight
    is_fighting_ = true;
}

// attempt investigation and return results
string Driver::investigate()
{
    string res; // results string
    srand(time(0)); // seed random
    int chance = (rand() % 10) + 1; // chance from 1 to 10
    
    if (chance <= 1)
    {
        // 10% for key: add a key and associated result
        party_.addKey();
        res += "You found a key!\n";
    }
    else if (chance <= 3)
    {
        // 20% chance for treasure: add the treasure if not "" and add associated result
        string rand_treasure = assets_.randomTreasure();
        if (rand_treasure != ""){
            assets_.changeTreasures(rand_treasure,1);
            res += "You found treasure " + rand_treasure + "!\n";
        }
    } else if (chance <= 5)
    {
        // 20% chance for random monster: use random monster function and add associated result
        randomMonster();
        res += "You encountered a monster!";
    }
    else if (chance <= 7)
    {
        // 20% chance for a random amount of food: add a random amount of food between 1 and 10
        int rand_food = (rand() % 10) + 1;
        assets_.changeIngredients(rand_food);
    }
    
    // if not fighting,
    if (!is_fighting_)
    {
        // attempt a post-fight style fullness decrease
        string fullness_dec = postFight();
        // if the result wasn't nothing, add it to the result
        if (fullness_dec != "")
        {
            res += fullness_dec + " ";
        }
    }
    // return result
    return res;
}

// interact with merchant and return result.
string Driver::interact()
{
    curr_merchant_.reset(); // reset merchant
    curr_merchant_.updatePrices(party_.getRooms()); // update their prices
    curr_merchant_.replacePuzzle(assets_.randomPuzzle()); // replace their puzzle with a random puzzle
    curr_puzzle = curr_merchant_.givePuzzle(); // set the current puzzle to their puzzle
    is_puzzle_ = true; // move into puzzles menu

    return "You speak to the merchant. Entering a puzzle..."; // return result
}

// clear extra_
void Driver::clearExtra()
{
    extra_ = "";
}

// attempt fight and return result as string
string Driver::fight()
{
    string res; // result string
    srand(time(0)); // seed random
    int chance = 0; // variable for storing random selections
    int b = 0; // b is the subtracted block for the monster
    
    // if the party has equipped armor, set b to (r2*level) / armor_count
    if (assets_.getUsedArmor() > 0)
    {
        b = curr_monster_.getCombatModifier()/assets_.getUsedArmor();
    }
    // otherwise, b is 0, preventing divide by 0

    int a = party_.getCombatModifier() - b; // subtract b from (r1*w + d)
    
    // if player wins:
    if (a > 0)
    {
        was_defeated_ = true; // set that monster was defeated
        res =  "You win! +"; // begin result string
        monster_ct_++; // increment monsters defeated count

        // calculate and apply gold / ingredients increases, add to result string
        int gold_ct = curr_monster_lvl_*10;
        int ingred_ct = curr_monster_lvl_*5;
        assets_.changeGold(gold_ct);
        assets_.changeIngredients(ingred_ct);

        res += to_string(gold_ct) + " gold, +";
        res += to_string(ingred_ct) + "kg ingredients.\n";

        // roll 10% chance of key
        chance = (rand() % 10) + 1; // 1 to 10
        if (chance == 1) // if hit 10%:
        {
            // add key and add to result string
            party_.addKey();
            res += " +1 key!";
        }
        
        // remove monster with monster's current name, add an endline to result, and return it
        assets_.removeMonster(curr_monster_.getName());
        res += "\n";
        return res;

    } else // if monster wins
    {
        was_defeated_ = false; // set that monter wasn't defeated
        res = "You lost! "; // begin result string

        // calculate and apply gold / ingredient decreases
        int gold_lost = -0.25*assets_.getGold();
        assets_.changeGold(gold_lost);
        res += "Now have " + to_string(assets_.getGold()) + " gold.\n";
        int ingred_lost = rand() % 30;
        assets_.changeIngredients(-1*ingred_lost);
        res += "Now have " + to_string(assets_.getIngredients()) + " kg ingredients.\n";

        vector<string> members = party_.getMembers(false); // get list of members excluding player
        for (string s : members)
        {
            // if the party member has armor:
            if (party_.hasArmor(s))
            {
                chance = (rand() % 100) + 1; // roll from 1 to 100
                if (chance <= 5) // if hit 5% chance:
                {
                    // kill member and add to result string
                    res += s;
                    res += " died. ";
                    party_.removeMember(s,false);
                }
            }
            else // otherwise,
            {
                chance = (rand() % 10) + 1; // roll from 1 to 10
                if (chance == 1) // if hit 10% chance:
                {
                    // kill member and add to result string
                    res += s;
                    res += " died. ";
                    party_.removeMember(s,false);
                }
            }
        }
        // add an endline to result and return it
        res += "\n";
        return res;
    }    
}

// attempt surrender and return result
string Driver::surrender()
{
    string member = party_.randomPlayer(false); // get random party member excluding player
    party_.removeMember(member, true); // trap party member
    return member + " surrendered. \n"; // return result string
}

// attempt cook and return result
string Driver::cook(string ware, int kg)
{
    int res = party_.attemptCook(ware,kg); // get result from attempting cook in party
    
    // if result is 0, return failure message
    if (res == 0)
    {
        return "Cooking failed. " + ware + " broke.";
    }
    else if (res > 0) // otherwise if result greater than 0, indicate success
    {
        return "Cooking succeeded! +" + to_string(res) + " fullness!";
    }
    else // otherwise notify of invalid input
    {
        return "Invalid ware or amount.";
    }
}

// set give up flag and return applicable message
string Driver::giveUp()
{
    given_up_ = true;
    return "You and your party lose yourselves in the depths of the dungeon. You never make it out.";
}

// call displayMap
void Driver::printMap()
{
    map_.displayMap();
}

// apply misfortune and return notification
string Driver::misfortune()
{
    string res; // result string
    string rand_item; // random item
    srand(time(0)); // seed random
    int type = (rand() % 10) + 1; // roll from 1 to 10
    if (type <= 3) // 30% robbery chance
    {
        res = "Your party was robbed. You lost: "; // add to result string
        int which_robbed = rand() % 4; // roll from 0 to 3
        switch (which_robbed)
        {
            case 0: // robbery: 0
            {
                int food_amt = ((rand() % 3) + 1)*5; // roll a random amount of food from 5 to 15
                assets_.changeIngredients(-1*food_amt); // remove food
                // add removal to result string
                res += to_string(food_amt); 
                res += " kg ingredients.";
                break;
            }
            case 1: // cookware: 1
                rand_item = assets_.randomCookware(); // roll a random cookware
                assets_.getCookware(rand_item).count_--; // remove cookware
                res += rand_item; // add removal to result string
                break;
            case 2: // armor: 3
                if (assets_.removeArmor()) // if there's non-equipped armor to equip,
                {
                    // remove it and add to result string
                    res += "1 piece of unequipped armor.";
                }
                break;
            case 3:
                rand_item = assets_.randomTreasure(); // roll a random treasure
                assets_.changeTreasures(rand_item, -1); // remove treasure
                res += rand_item; // add removal to result string
                break;
        }
    }
    else if (type <= 4) // otherwise 10% weapon breaking chance
    {
        rand_item = assets_.randomWeapon(); // get random weapon
        if (assets_.getWeapon(rand_item).count_ > 0) // if more than 1,
        {
            // break it and add to result string
            res = "A weapon broke. You lost: ";
            party_.removeWeapon(rand_item);
            res += assets_.weaponToString(rand_item);
        }
    }
    else if (type <= 7) // otherwise 30% food poisoning chance
    {
        res = "Food poisoning: ";
        string member = party_.randomPlayer(true); // get a random member including player
        // if someone has died, add it to result string
        if (party_.reduceFullness(member,10))
        {
            res += member + " died.";
        }
        else // otherwise, add normal -10 fullness result string
        {
            res += member + " lost 10 fullness.";
        }
    }
    else if (type <= 10 && last_locked_) // otherwise if 30% locked away chance and last room was locked,
    {
        last_locked_ = false; // reset last locked
        string member = party_.randomPlayer(false); // get random member including player
        party_.removeMember(member,true); // trap member and add to result string
        res = member + " was trapped in the previous room.";
    }
    return res; // return result
}

// apply post-fight hunger
string Driver::postFight()
{
    string res; // result string
    srand(time(0)); // seed random
    int chance; // chance
    vector<string> members = party_.getMembers(true);
    for (string s : members){
        chance = (rand() % 2); // 0 or 1
        if (chance == 0) // if 50% fullness reduction:
        {
            // apply fullness. if died, add to result, otherwise add normal notification
            if (party_.reduceFullness(s,1))
            {
                res += s + " died. ";
            }
            else
            {
                res += s + " lost 1 fullness. ";
            }
        }
    }
    return res; // return result
}

// apply post-move hunger: similar concept to above
string Driver::postMove()
{
    string res;
    srand(time(0));
    int chance;
    vector<string> members = party_.getMembers(true);
    for (string s : members)
    {
        chance = (rand() % 5); // 0 - 4
        if (chance == 0) // if 20% fullness reduction:
        {
            // apply fullness. if died, add to result, otherwise add normal notification
            if (party_.reduceFullness(s,1))
            {
                res += s + " died.";
            }
            else
            {
                res += s + " lost 1 fullness.";
            }
        }
    }
    return res;
}

// checks all relevant flags to determine if game end
bool Driver::isGameOver()
{
    if (party_.getEffectiveMembers() <= 2) // if less than 3 effective members
    {
        return true;
    }
    else if (sorcerer_anger_ >= 100) // if sorcerer anger excessive
    {
        return true;
    }
    else if (party_.isPlayerDead()) // if player dead
    {
        return true;
    }
    else if (party_.getRooms() >= 5 && map_.currentIsExit() && sorcerer_defeated_) // if rooms cleared greater than 5, on exit, and sorcerer defeated
    {
        return true;
    } else if (given_up_) // if given up
    {
        return true;
    }
    return false; // otherwise, return false
}

// print leaderboard
string Driver::printLeaderboard()
{
    map<int, string> leaderboard; // score:name

    // open file
    ifstream fin;
    fin.open("leaderboard.txt");
    string line; // current line
    vector<string> split_res; // split of line
    while (getline(fin,line)) // while still lines to get:
    {
        // if line isn't empty, split and use first as number, second as name
        if (line.length() != 0)
        {
            split(line,split_res,"~");
            leaderboard[stoi(split_res[0])] = split_res[1];
        }
    }
    fin.close(); // close file
    
    // if the current player has defeated the sorcerer, add them to leaderboard
    if (sorcerer_defeated_)
    {
        int player_score = sorcerer_anger_; // lower the better
        leaderboard[player_score] = party_.getPlayerName();
    }
    
    // print leaderboard:
    string res = "Players who successfully defeated sorcerer by anger: (anger)\n";
    int ct = 1; // rank
    for (pair<int, string> p: leaderboard)
    {
        res += to_string(ct) + "(" + to_string(p.first) + "): "; // print rank and score
        res += p.second + "\n"; // print name
    }

    string output; // stores output string
    ofstream fout; // output file stream
    
    // add the information in the leaderboard to output
    for (pair<int, string> p: leaderboard)
    {
        output += to_string(p.first) + "~";
        output += p.second + "\n";
    }
    // open file, write to it, and close
    fout.open("leaderboard.txt");
    fout << output << endl;
    fout.close();

    // return result
    return res;
}

string Driver::printEndgame()
{
    // open output file
    ofstream fout;
    fout.open("results.txt");

    string res = extra_; // stores result
    map<string, string> tmp; // stores temporary data

    // add reason for endgame to res
    if (party_.getEffectiveMembers() < 2)
    {
        res = "Your party was not large enough. \n";
    }
    else if (sorcerer_anger_ >= 100)
    {
        res = "The sorcerer's anger exceeded 100. \n";
    }
    else if (party_.isPlayerDead())
    {
        res = "The player died. \n"; 
    }
    else if (party_.getRooms() >= 5 && map_.currentIsExit() && sorcerer_defeated_)
    {
        res = "The party defeated the sorcerer, found the exit and made it out of the dungeon! \n";
    }
    else if (given_up_)
    {
        res = extra_ + "\n";
    }

    // print out effective members
    res += "Members: ";
    res += party_.getPlayerName();
    res += " (you) ";

    vector<string> members = party_.getMembers(false);
    
    // for every member, if they're effective, add their name
    for (string s : members)
    {
        if (party_.isEffective(s))
        {
            res += s;
            res += " ";
        }
    }

    // add all other information
    res += "\nRooms cleared: ";
    res += to_string(party_.getRooms());
    res += "\nGold remaining: ";
    res += to_string(assets_.getGold());

    res += "\nTreasure items: \n";
    assets_.allTreasures(tmp);
    for (pair<string, string> p : tmp){
        res += "    " + p.second + "\n";
    }
    res += "\nSpaces explored: ";
    res += to_string(explored_ct_);
    res += "\nMonsters defeated: ";
    res += to_string(monster_ct_);
    res += "\nTurns:";
    res += to_string(turns_ct_);

    // write to file and close
    fout << res << endl;
    fout.close();

    return res; // return result
}

// attempt to move player in direction
bool Driver::movePlayer(char dir)
{
    return map_.move(dir);
}

// get options from merchant and return options
map<string,string>& Driver::getOptionsMerchant()
{
    curr_merchant_.getOptions();
    return options_;
}

// receive options for merchant
bool Driver::receiveOptionMerchant(string option)
{
    // if option isn't a number, return false
    for (char c : option)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    bool res = curr_merchant_.selectOption(stoi(option)); // attempt selection
    if (curr_merchant_.readyToTransact()) // if merchant's ready to transact:
    {
        // get transaction an attempt it
        Transaction& t = curr_merchant_.getTransaction();
        int cost = t.cost_;
        bool transaction_success = assets_.handleTransaction(t);

        // add relevant information to extra_ based on transaction, and notify merchant of transaction success
        if (transaction_success)
        {
            extra_ = "Transaction OK. Gold changed by " + to_string(cost) + ".";
            curr_merchant_.gotTransaction();
        }
        else
        {
            extra_ = "Transaction failed. Check cost.";
        }
    }
    else if (curr_merchant_.isClosed()) // otherwise if merchant is closed, add info to extra
    {
        extra_ = "Exiting merchant...";;
    }
    else // otherwise clear extra
    {
        extra_ = "";
    }
    return res; // return result
}

// return whether initial merchant is over.
bool Driver::initialEnd()
{
    initial_end_ = true; // reset initial_end
    
    // if merchant is closed, reset merchant and return true
    if (curr_merchant_.isClosed())
    {
        curr_merchant_.reset();
        return true;
    }
    return false; // otherwise, return false
}


// get options for current menu
map<string,string>& Driver::getOptions()
{
    options_.clear(); // clear options
    int ct = 1; // set a counter to 1
    string cookware_string; // store cookware
    string weapons_string; // store weapons
    vector<string> results_; // store results of certain functions
    allowed_fight = false; // reset whether or not allowed to fight
    
    // if in fight:
    if (is_fighting_)
    {
        // get random monster and notify player
        randomMonster();
        options_["HEADER"] = "You encounter a " + curr_monster_.getName();

        // if number of weapons > 0, allow fight and notify player
        if (assets_.getNumWeapons() > 0)
        {
            allowed_fight = true;
            options_["1"] = "Fight";
        }
        options_["2"] = "Surrender"; // surrender option
    }
    else if (in_move_menu_) // if in move menu, print directions
    {
        options_["HEADER"] = "Which direction?";
        options_["w"] = "Up";
        options_["a"] = "Left";
        options_["s"] = "Down";
        options_["d"] = "Right"; 
        options_["E"] = "Exit to menu";
    }
    else if (in_cook_menu_ == 1) // if in first cook menu, prompt for cookware
    {
        options_["HEADER"] = "Choose a cookware: ";
        options_["E"] = "Exit.";
        assets_.allCookware(options_); // list cookware
    }
    else if (in_cook_menu_ == 2) // if in second cook menu, prompt for ingredients count
    {
        options_["HEADER"] = "How many ingredients? Enter a multiple of 5.";
    }
    else if (in_inventory_menu_ == 0) // if in inventory main, ask which type of operation
    {
        options_["HEADER"] = "Which inventory operation?";
        options_["1"] = "Modify Armor";
        options_["2"] = "Modify Weapons";
        options_["E"] = "Exit to main menu";
    }
    else if (in_inventory_menu_ == 11 || in_inventory_menu_ == 21) // if in first phase of inventory, ask for player name
    {
        // list relevant party info
        string tmp = "Which player?\n\033[0m";
        party_.listPartyInfo(results_);
        for (string s : results_)
        {
            tmp += s + "\n";
        }
        options_["HEADER"] = tmp;
        options_["E"] = "Go back a menu";
    }
    else if (in_inventory_menu_ == 12) // if in second phase of armor:
    {
        // list options and info
        options_["HEADER"] = "Which status? You have " + to_string(assets_.getTotalArmor() - assets_.getUsedArmor()) + " armor.";
        options_["0"] = "Remove armor";
        options_["1"] = "Add armor";
        options_["E"] = "Go back a menu";
    }
    else if (in_inventory_menu_ == 22) // if in second phase of weapon:
    {
        // list options and info
        string tmp =  "Which weapon?\n";
        assets_.allWeapons(options_);
        options_["E"] = "Go back a menu";
        options_["HEADER"] = tmp;
    }
    else if (is_puzzle_) // if in puzzle, list prompt
    {
        options_["HEADER"] = curr_puzzle.getPrompt();
    }
    else if (is_merchant_ && !curr_merchant_.isClosed()) // if in non-closed merchant, list their options
    {
        curr_merchant_.getOptions();
    }
    else // otherwise, if not in special case:
    {
        if (map_.currentIsNPC()) // if in NPC space, add interact option
        {
            options_["HEADER"] = "You see a merchant. Do you speak to them?";
            options_["1"] = "Yes";
        }
        else if (map_.currentIsFree()) // else if in free space,
        {
            options_["HEADER"] = "You are in a normal space.";
            if (!map_.currentIsExplored()) // if unexplored, add investigate option
            {
                options_["1"] = "Investigate";
            }
            // add fight and eat options
            options_["2"] = "Pick a Fight";
            options_["3"] = "Cook and Eat";
        }
        else if (map_.currentIsRoom()) // else if at room, add open door option
        {
            options_["HEADER"] = "You encounter a room.";
            options_["1"] = "Open the door";
        } else if (map_.currentIsExit()) // else if at exit, add exit info
        {
            options_["HEADER"] = "This is the exit.";
        }
        // add possible options at all spaces
        options_["0"] = "Move";
        options_["M"] = "Map"; 
        options_["Q"] = "Quit";
        options_["I"] = "Manage inventory";   
    }
    // return options_
    return options_;
}

// attempt to handle option. return whether succeeded or failed
bool Driver::receiveOption(string option)
{
    turns_ct_++; // any time an option is received, increment turns
    srand(time(0)); // seed random
    if (is_fighting_) // if fighting:
    {
        // if selected fight and allowed to fight:
        if (option == "1" && allowed_fight)
        {
            // set extra info to fight and post fight combined
            extra_ = fight() + postFight();
            
            // if at a room:
            if (map_.currentIsRoom())
            {
                int res = rand() % 10; // roll from 1 to 10
                if (party_.getRooms() == 4){
                    extra_ = "Encountering the sorcerer!";
                }

                if (was_defeated_) // if defeated monster:
                {
                    if (map_.currentIsNPC()) // if at NPC, remove NPC
                    {
                        map_.removeNPC(map_.getPlayerRow(), map_.getPlayerCol());
                    } else if (map_.currentIsRoom()) // if at room, say that room was cleared
                    {
                        party_.incrementRooms();
                        if (party_.getRooms() == 5) // if at last room, notify about sorcerer defeated
                        {
                            extra_ = "You defeated the sorcerer!";
                            sorcerer_defeated_ = true;
                        }
                        // remove the room, increment monster level and change monsters
                        map_.removeRoom(map_.getPlayerRow(), map_.getPlayerCol());
                        curr_monster_lvl_++;
                        assets_.changeMonsters("monsters",curr_monster_lvl_);
                        
                    }
                    if (res <= 5) // 50% chance of misfortune
                    {
                        extra_ += misfortune();
                    }
                }
                else if (!was_defeated_ && res <= 4) // if didn't defeat, 40% chance of misfortune
                {
                    extra_ += misfortune();
                }
            }
            // reset fighting-related flags
            is_fighting_ = false;
            allowed_fight = false;
        }
        else if (option == "2") // if chose to surrender, apply surrender and reset fighting flag
        {
            extra_ = surrender() + postFight();
            is_fighting_ = false;
        } else // else if invalid selection, return false
        {
            return false;
        }
        return true; // if either 1 or 2 was selected, return true
    } else if (in_move_menu_) // if in move menu:
    {
        // if option is any of the relevant directions:
        if (option == "w" || option == "a" || option == "s" || option == "d")
        {
            if (!movePlayer(option[0])) // if move failed, notify player
            {
                extra_ = "Move impossible.";
            } else // otherwise,
            {
                extra_ = postMove(); // apply post move hunger

                // if going into unexplored and sorcerer undefeated, increment anger
                if (!map_.currentIsExplored() && !sorcerer_defeated_)
                {
                    sorcerer_anger_++;
                }
                
                // reset last locked and in_move_menu
                last_locked_ = false;
                in_move_menu_ = false;
            }
        }
        else if (option == "E") // if chose to exit, reset menu flag
        {
            in_move_menu_ = false;
        } else // otherwise, if invalid input, return false
        {
            return false;
        }
        return true; // if any of the correct options, return true
    }
    else if (in_cook_menu_ == 1) // if in first cook menu,
    {
        // if chose to exit, reset flag and return true
        if (option == "E")
        {
            in_cook_menu_ = -1;
            return true;
        }
        else if (options_.find(option) == options_.end() || option == "HEADER") // otherwise, if cookware selection wasn't in options, return false
        {
            return false;
        }
        // attempt to get selection at option number
        cookware_sel = assets_.getCookwareAt(stoi(option));
        if (cookware_sel == "") // if selection was empty, return false
        {
            return false;
        } 
        if (assets_.getCookware(cookware_sel).count_ == 0)  // if the cookware's count is 0, notify player and return false
        {
            extra_ = "Not enough of cookware!";
            return false;
        }
        in_cook_menu_++; // go to next cook menu and return true
        return true;
    }
    else if (in_cook_menu_ == 2)
    {
        // if the selection isn't a number, return false
        for (char c : option)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }

        ingredients_sel = stoi(option); // convert to int
        
        // if option is outside of ingredient bounds, exit out of cook menu and return false
        if (ingredients_sel < 0 || ingredients_sel > assets_.getIngredients())
        {
            extra_ = "Invalid ingredients.";
            in_cook_menu_ = -1;
            return false;
        }
        
        // if not a multiple of 5, return false
        if (ingredients_sel % 5 != 0)
        {
            extra_ = "Please enter a multiple of 5.";
            return false;
        }
        // set extra_ to cook result, exit out of cook menu, and return true
        extra_ = cook(cookware_sel,ingredients_sel);
        in_cook_menu_ = -1;
        return true;
    }
    else if (in_inventory_menu_ == 0) // if in main of inventory menu:
    {
        // route to right next option:
        if (option == "1")
        {
            in_inventory_menu_ = 11;
        }
        else if (option == "2")
        {
            in_inventory_menu_ = 21;
        }
        else if(option == "E")
        {
            in_inventory_menu_ = -1;
        }
        else // if option not valid, return false
        {
            return false;
        }
        return true; // return true
    }
    else if (in_inventory_menu_ == 11 || in_inventory_menu_ == 21) // if in player selection submenu of inventory:
    {
        // if exiting, returning back a menu
        if (option == "E")
        {
            in_inventory_menu_ = 0;
        }
        else if (!party_.isMember(option) || !party_.isEffective(option)) // otherwise if option isn't an effective or existing player, return false
        {
            return false;
        } else // otherwise, go to the next submenu, store option and return true
        {
            in_inventory_menu_++; 
            member_sel = option;
            return true;
        }
    }
    else if (in_inventory_menu_ == 12) // if in second armor menu:
    {
        if (option == "0") // if trying to unequip, reset inventory menu and attempt unequip
        {
            in_inventory_menu_ = 0;
            if (party_.changeArmor(member_sel,false)) // if succeeded, return true
            {
                extra_ = "Unequipping success.";
                return true;
            }
        }
        else if (option == "1") // if trying to equip, reset inventory menu and attempt equip
        {
            in_inventory_menu_ = 0;
            if (party_.changeArmor(member_sel, true)) // if succeeded, return true
            {
                extra_ = "Equipping success.";
                return true;
            }
        }
        else if (option == "E") // if trying to exit, go back a menu and return true
        {
            in_inventory_menu_ = 11;
            return true;
        }
        return false; // if invalid input, return false
    }
    else if (in_inventory_menu_ == 22) // if in second inventory submenu
    {
        // if trying to exit, go back a menu and return true
        if (option == "E")
        {
            in_inventory_menu_ = 21;
            return true;
        }
        else if (options_.find(option) == options_.end() || option == "HEADER") // if option not found, return false
        {
            return false;
        }
        string weapon = assets_.getWeapAt(stoi(option), false); // get weapon name from index
        if (weapon == "") // if weapon not found, return false
        {
            return false;
        }
        // if weapon isn't none and all weapons of the type have been used, notify user and go back to inventory main, return true
        if (weapon != "(none)" && assets_.getWeapon(weapon).count_ == assets_.getWeapon(weapon).equipped_)
        {
            extra_ = "No weapons of this type to equip.";
            in_inventory_menu_ = 0;
            return true;
        }
        // reset inventory menu
        in_inventory_menu_ = 0;
        // attempt to change weapons. if successful, return true.
        if (party_.changeWeapons(member_sel, weapon))
        {
            extra_ = "Weapon change success.";
            return true;
        }
        return false; // otherwise, return false
    }
    else if (is_puzzle_) // if in a puzzle:
    {
        int res= curr_puzzle.acceptAttempt(option); // call puzzle's accept option
        switch(res){
            case -1: // if -1, return false
                return false;
            case 0: // if tie, indicate result and return true
                extra_ = "Tie. Try again.";
                return true;
            case 1: // if correct, indicate result and return true
                is_puzzle_ = false;
                extra_ = "Correct.";
                if (map_.currentIsNPC()) // if at NPC, go to merchant
                {
                    is_merchant_ = true;
                } else if (map_.currentIsRoom()) // if at room, go to fight
                {
                    is_fighting_ = true;
                }
                return true; // return true
            case 2: // if incorrect, apply consequences and return true
                extra_ = ""; // clear extra
                if (map_.currentIsNPC()) // if at NPC, start fight
                {
                    is_fighting_ = true;
                }
                else if (map_.currentIsRoom()) // if at room, kill a random player
                {
                    string rand_player = party_.randomPlayer(false);
                    party_.removeMember(rand_player, false);
                    extra_ = "Player " + rand_player + " was killed.";
                }
                is_puzzle_ = false; // exit puzzle
                extra_ += "Incorrect.";
                return true;
            default: // if none of these, return false
                return false;
        }
    }
    else if (is_merchant_) // if in merchant,
    {
        if (curr_merchant_.isClosed()) // if merchant is closed,
        {
            // explore space, remove NPC, reset merchant, exit menu, and return true
            map_.exploreSpace(map_.getPlayerRow(), map_.getPlayerCol());
            map_.removeNPC(map_.getPlayerRow(), map_.getPlayerCol());
            curr_merchant_.reset();
            is_merchant_ = false;
            return true;
        }
        return receiveOptionMerchant(option); // otherwise, call receiveOptionMerchhant
    }
    else
    {
        return receiveRoomOption(option); // otherwise, attempt to receiveRoomOption
    }
    return false; // otherwise, return false
}

// attempt to receive room option. return whether succeeeded or failed
bool Driver::receiveRoomOption(string option)
{
    // if length exceeds 1, return false
    if (option.length() > 1)
    {
        return false;
    }
    // if at NPC and selected interact, apply interact and return true
    if (map_.currentIsNPC())
    {
        if (option == "1"){
            extra_ = interact();
            return true;
        }
    }
    else if (map_.currentIsFree()) // if at normal space:
    {
        if (option == "1" && !map_.currentIsExplored()) // if selected investigate and not yet investiagted,
        {
            map_.exploreSpace(map_.getPlayerRow(), map_.getPlayerCol()); // explore the space
            extra_ = investigate() + "\nInvestigation complete.\n"; // apply investigate
            return true;
        }
        else if (option == "2") // if selected fight, go to fight menu
        {
            is_fighting_ = true;
            return true;
        }
        else if (option == "3") // if selected cook, go to cook menu
        {
            in_cook_menu_ = 1;
            return true;
        }
    }
    else if (map_.currentIsRoom()) // if at room:
    {
        if (option == "1") // if chose to enter:
        {
            if (party_.keys() >= 1) // if have keys, automatically use them
            {
                extra_ = "Used 1 key";
                party_.removeKey();
                last_locked_ = true; // last_locked set to true
                is_fighting_ = true;
            } else // otherwise, first divert to the door puzzle and menu
            {
                curr_puzzle = Puzzle("","",3,true);
                is_puzzle_ = true;
            }
            return true;
        }
    }
    // handle the options common to all spaces:
    switch (option[0])
    {
        case '0': // if move selected, go to move menu
            extra_ = party_.warnFullness(); // warn fullness
            in_move_menu_ = true;
            return true;
        case 'M': // if map selected, reset extra and call display map
            extra_ = "";
            map_.displayMap();
            return true;
        case 'Q': // if quit selected, attempt giveUp
            extra_ = giveUp();
            return true;
        case 'I': // if inventory management selected, go to inventory main menu
            in_inventory_menu_ = 0; 
            return true;
        default: // otherwise, return false
            return false;
    }
    return false; // return false
}


// get extra information
string Driver::getExtra()
{
    return extra_;
}

// get status block
string Driver::getStatusUpdate()
{
    // print basic info
    string res = "\033[1mSUMMARY:\033[0m\n" + party_.listBasicStats() + " | Anger level: " + to_string(sorcerer_anger_);
    
    // print party info
    vector<string> tmp;
    party_.listPartyInfo(tmp);
    res += "\nPARTY: \n";
    for (string s : tmp)
    {
        res += "    " + s + "\n";
    }
    
    // print inventory info
    res += "INVENTORY: \n";
    party_.listInventory(tmp);
    for (string s : tmp)
    {
        res += s + "\n";
    }
    return res; //return result
}

// returns whether in a menu
bool Driver::inMenu()
{
    // if in fighting, in puzzle, or in merchant, yes
    if (is_fighting_ || is_puzzle_ || is_merchant_)
    {
        return true;
    }
    // if in move, cook, or inventory menus, yes
    if (in_move_menu_ || in_cook_menu_ > -1 || in_inventory_menu_ > -1)
    {
        return true;
    }
    return false; // otherwise, no
}
