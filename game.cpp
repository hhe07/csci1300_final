// CSCI 1300 Fall 2022
// Author: Leo Ge
// Recitation: 104 – Christopher Ojukwu
// Project 3: game.cpp

#include "Driver.h"

int main(){
    Assets a = Assets();
    
    // attempt to load all assets from files. if they fail, print a message and return -1
    if (!a.loadCookwares("cookware.txt"))
    {
        cout << "Invalid cookware file." << endl;
        return -1;
    }
    if (!a.loadPuzzles("riddles.txt"))
    {
        cout << "Invalid riddles file." << endl;
        return -1;
    }
    if (!a.loadTreasures("treasures.txt"))
    {
        cout << "Invalid treasures file." << endl;
        return -1;
    }
    if (!a.loadWeapons("weapons.txt"))
    {
        cout << "Invalid weapons file." << endl;
        return -1;
    }
    if (!a.changeMonsters("monsters",1)){
        cout << "Invalid monsters file." << endl;
        return -1;
    }

    Driver driver = Driver(a);
    cout << "Initialising..." << endl; // print a message while map is being generated
    driver.init();

    int ct = 0; // player count
    string input; // input
    map<string,string> s; // storage for options
    do
    {
        // prompt player for their / their party member's name
        cout << "What is";
        if (ct == 0)
        {
            cout << " your name?" << endl;
        }
        else
        {
            cout << " your next party member's name?" << endl;
        }
        cout << "> ";
        getline(cin,input); // getline into input

        if (ct == 0) // if first input, set player as well.
        {
            driver.addMember(input);
            driver.setPlayer(input);
            ct++;
        }
        else
        {
            // otherwise, if adding the member doesn't work, don't increment ct and print a message
            if (!driver.addMember(input))
            {
                cout << "Invalid input." << endl;
                continue;
            }
            ct++;
        }
    } while (ct < 5); //while less than 5 players

    // print prompt and begin initial selection
    cout << "\033[1mPlease make your initial equipment selections. You have 100 gold: \033[0m"<< endl;
    do
    {
        // print and clear extra
        cout << driver.getExtra() << endl; 
        driver.clearExtra();

        // get options
        s = driver.getOptionsMerchant();

        // print header and then erase it
        cout << "\033[1m" << s["HEADER"] << "\033[0m" << endl;
        s.erase("HEADER");

        // for every other option, print the key and then the information
        for (pair<string,string> p : s)
        {
            cout << p.first << " " << p.second << endl;
        }
        
        // if the phase is over, exit loop
        if (driver.initialEnd())
        {
            break;
        }

        cout << "> ";
        getline(cin, input); // getline into input
        
        // if receiving option fails, print invalid input
        if (!driver.receiveOptionMerchant(input))
        {
            cout << "Invalid input." << endl;
        }
    } while(!driver.initialEnd()); // while initial phase isn't over

    // core game loop
    do {
        // print and clear extra
        cout << driver.getExtra() << endl;
        driver.clearExtra();
        
        // if not in menu, print status update
        if (!driver.inMenu())
        {
            cout << driver.getStatusUpdate() << endl;
        }

        // get options
        s = driver.getOptions();
        
        // print header and then erase it
        cout << "\033[1m" << s["HEADER"] << "\033[0m" << endl;
        s.erase("HEADER");

        // for every other option, print the key and then the information
        for (pair<string,string> p : s)
        {
            cout << p.first << " " << p.second << endl;
        }
        
        cout << "> ";
        getline(cin, input); // getline into input

        // if receiving option fails, print invalid input
        if (!driver.receiveOption(input))
        {
            cout << "Invalid input." << endl;
        }
    } while (!driver.isGameOver()); // while game isn't over

    cout << driver.printEndgame() << endl; // print endgame
    cout << driver.printLeaderboard() << endl; // print leaderboard.
}