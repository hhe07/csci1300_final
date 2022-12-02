#include "Puzzle.h"

int main(){
    // first, test a non-door puzzle.
    // initialise puzzle
    Puzzle p1 = Puzzle("Enter \"Correct\"", "Correct",2,false);
    int res = -1;
    string input;
    // handle attempts
    do{
        cout << p1.getPrompt() << endl;
        getline(cin,input);
        res = p1.acceptAttempt(input);
    } while (res <1);
    // handle final result
    switch (res){
        case 1:
            cout << "Correct!" << endl;
            break; 
        case 2:
            cout << "Incorrect!" << endl;
            break;
    }

    // second, test a door puzzle
    Puzzle p2 = Puzzle("","",0,true);
    res = -1;
    do{
        cout << p2.getPrompt() << endl;
        getline(cin, input);
        res = p2.acceptAttempt(input);
        if (res == -1){
            cout << "Invalid input." << endl;
        } else if (res == 0){
            cout << "Tie. Try again." << endl;
        }
    } while (res < 1);
    switch (res){
        case 1:
            cout << "Correct!" << endl;
            break; 
        case 2:
            cout << "Incorrect!" << endl;
            break;
    }

    
}