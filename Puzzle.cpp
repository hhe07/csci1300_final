// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Party.cpp

#include "Puzzle.h"

/* 
    default constructor:
    creates a puzzle with no attempts that isn't a door puzzle.
    string elements are initialised to empty.
*/
Puzzle::Puzzle()
{
    attempts_ = 0;
    is_door_ = false;
}

/*
    paramterised constructor:
    replaces parameters with values passed in. 
*/
Puzzle::Puzzle(string prompt, string sol, int attempts, bool is_door){
    prompt_ = prompt;
    solution_ = sol;
    attempts_ = attempts;
    is_door_ = is_door;
}

/*
    getPrompt():
    returns the prompt string (plus some extra information if necessary)
    depending on the type of the puzzle.
*/
string Puzzle::getPrompt()
{
    // return prompt plus attempts information.
    if (is_door_){
        return "The challenge is Boulder - Parchment - Shears. \nBoulder beats Shears, Parchment beats Boulder, \nShears beats Parchment.";
    }
    return "You have " + to_string(attempts_) + " attempts.\n" + prompt_;
}


/*
    acceptAttempt():
    returns the "outcome" of a given attempt as an int:
    0 for tie or still have attempts.
    1 for player success
    2 for player failure
    -1 for invalid input

*/
int Puzzle::acceptAttempt(string attempt){
    // handle door puzzle
    if (is_door_)
    {
        srand(time(0));
        int move = rand() % 3; // values 0, 1, 2

        // convert to key.
        int player_move = strToKey(attempt);
        
        // if conversion was unsuccessful, indicate invalid input
        if (player_move == -1){
            return -1;
        }

        // otherwise, if the moves match, indicate "tie"
        if (player_move == move){
            return 0;
        }

        // determiner is a shortcut for figuring out the result of the attempt
        // +1 means player win, -1 means system win
        // +2 means system win, -2 means player win
        int determiner = player_move - move; 
        switch (determiner){
            case 1:
            case -2:
                return 1;
                break;
            case -1:
            case 2:
                // otherwise, if the player "lost" but still has attempts,
                if (attempts_ > 0){
                    // decrement attempts and indicate tie
                    attempts_--;
                    return 0;
                }
                return 2;
                break;
        }
    }
    // otherwise, if the puzzle is a door puzzle and the solution was correct,
    // indicate this result
    if (attempt == solution_){
        return 1;
    }
    // otherwise, decrement attempts
    attempts_--;

    // if attempts used up, indicate failure
    if (attempts_ <= 0){
        return 2;
    }
    // indicate tie (invalid input)
    return 0;
}


int Puzzle::strToKey(string attempt){
    // simple conversion of:
    // boulder -> 0
    // parchment -> 1
    // shears -> 2
    // otherwise if none of these return -1

    if (attempt == "Boulder"){
        return 0;
    } else if (attempt == "Parchment"){
        return 1;
    } else if (attempt == "Shears"){
        return 2;
    }
    return -1;
}