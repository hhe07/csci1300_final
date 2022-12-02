// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: Puzzle.h

#ifndef PUZZLE_H
    #define PUZZLE_H
    #include <iostream>
    using namespace std;

    /**
     * Puzzle class: stores puzzle information and provides functions to handle
     * user interaction more cleanly.
    **/
    class Puzzle
    {
    private:
    // private data members:
        string prompt_; // the riddle being asked, if the puzzle is one.
        string solution_; // the solution to the riddle
        int attempts_; // number of attempts the user has
        bool is_door_; // if the puzzle is a door puzzle, the above is ignored
    public:
    // public functions:
        // default constructor, sets all of above parameters to empty.
        Puzzle();

        // parameterised constructor, fills the class parameters in.
        Puzzle(string prompt, string sol, int attempts, bool is_door); 

        // returns riddle prompt. if the puzzle is a door puzzle, returns that dedicated prompt instead.
        string getPrompt();

        /*
            accepts a user attempt.  
            return 1 if answer is correct.
            return 0 if answer is incorrect but player still has attempts.
            return 2 if puzzle is not accepting solutions (incorrect).
            return -1 if invalid input on door puzzle.
        */
        int acceptAttempt(string attempt);

        // converts a door puzzle attempt to an int representation 
        // for easier processing.
        int strToKey(string attempt);
    };
    
    
#endif