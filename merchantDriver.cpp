#include "Merchant.h"
#include <ctype.h>

bool isValid(string input){
    for (char c : input){
        if (!isdigit(c)){
            return false;
        }
    }
    return true;
}

int main(){
    map<string, Weapon> weapons = {
        pair<string,Weapon>{"Test Weapon 1", Weapon{1,0,0,10}},
        pair<string,Weapon>{"Test Weapon 2", Weapon{2,0,0,10}},
        pair<string,Weapon>{"Test Weapon 3", Weapon{3,0,0,10}},
    };

    map<string,Treasure> treasures = {
        pair<string,Treasure>{"Treasure 1", Treasure{10,0}},
        pair<string,Treasure>{"Treasure 2", Treasure{5,0}},
        pair<string,Treasure>{"Treasure 3", Treasure{2,0}},
    };
    
    map<string,Cookware> cookware = {
        pair<string,Cookware>{"Cookware 1",Cookware{20,10,0}},
        pair<string,Cookware>{"Cookware 2",Cookware{10,20,0}},
    };

    map<string,string> options;
    Merchant m1 = Merchant(false,weapons,cookware,treasures, options);
    //m1.replacePuzzle();
    // get monster name from driver
    string input;
    int iinput;
    do{
        m1.getOptions();
        for (pair<string,string> p : options){
            cout << p.first << p.second << endl;
        }
        getline(cin,input);
        if (!isValid(input)){
            cout << "Invalid input." << endl;
            continue;
        }
        iinput = stoi(input);
        bool success = m1.selectOption(iinput);
        if (!success){
            cout << "Invalid input." << endl;
        }
        if (m1.readyToTransact()){
            //Transaction t = m1.getTransaction();
            // player apply transaction, if result is bad then do whatever

            // extra_info_ = "Transaction failed.";
            // extra_info_ = "Transaction succeeded: " + to_string(-1*curr_transaction_.cost_) + " gold.";
        }
    } while (!m1.isClosed());
    m1.reset(); // clean up for next iteration
}