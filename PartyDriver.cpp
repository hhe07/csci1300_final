#include "Party.h"
#include <cassert>

int main(){

    map<string, Weapon> weapons = {
        pair<string,Weapon>{"Test Weapon 1", Weapon{1,3,0,10}},
        pair<string,Weapon>{"Test Weapon 2", Weapon{2,3,0,10}},
        pair<string,Weapon>{"Test Weapon 3", Weapon{3,3,0,10}},
    };

    map<string,Treasure> treasures = {
        pair<string,Treasure>{"Treasure 1", Treasure{10,2}},
        pair<string,Treasure>{"Treasure 2", Treasure{5,2}},
        pair<string,Treasure>{"Treasure 3", Treasure{2,2}},
    };
    
    map<string,Cookware> cookware = {
        pair<string,Cookware>{"Cookware 1",Cookware{20,2,10}},
        pair<string,Cookware>{"Cookware 2",Cookware{10,2,20}},
    };

    vector<string> output;
    // member testing
    
    /*  Member m1 = Member();

    cout << m1.toStr() << endl; */


    Party p = Party(weapons,cookware,treasures);
    assert(p.addMember("Player 1"));
    assert(!p.addMember("Player 1"));
    p.addMember("Player 2");
    p.addMember("Player 3");
    p.setPlayer("Player 1");

    //cout << p.listBasicStats() << endl;
    /* p.listPartyInfo(output);
    for (string s : output){
        cout << s << endl;
    }

    p.listInventory(output);
    for (string s : output){
        cout << s << endl;
    } */

    /* // check armor functions
    p.addArmor(1);
    assert(p.changeArmor("Player 1",true));
    assert(p.changeArmor("Player 1",false));
    // should have reset to 1.
    
    assert(p.changeArmor("Player 1", true));
    assert(!p.changeArmor("Player 2", true));
    // should not remove anything.
    p.removeArmor();

    // should now remove armor;
    p.changeArmor("Player 1", false);
    p.removeArmor();
    for (string s : output){
        cout << s << endl;
    }
    
    
    p.listPartyInfo(output);
    
    for (string s : output){
        cout << s << endl;
    }
    p.listInventory(output);
    for (string s : output){
        cout << s << endl;
    } */ 

    /* // check weapon functions
    assert(p.changeWeapons("Player 1", "Test Weapon 1"));
    
    assert(p.changeWeapons("Player 1", "(none)"));

    
    p.removeWeapon("Test Weapon 1");
    p.removeWeapon("Test Weapon 1");

    assert(p.changeWeapons("Player 1", "Test Weapon 1"));
    p.removeWeapon("Test Weapon 1");
    
    p.listPartyInfo(output);
    for (string s : output){
        cout << s << endl;
    }
    
    p.listInventory(output);
    for (string s : output){
        cout << s << endl;
    } */

    /* // check kill / trap functionality.

    p.killMember("Player 2");
    p.trapMember("Player 3");
    p.listPartyInfo(output);
    for (string s : output){
        cout << s << endl;
    } */

    
    /* // check random player functionality.
    cout << p.randomPlayer(false) << endl;
    cout << p.randomPlayer(true) << endl; */

    /* // check cooking, reduceFullness, warnFullness
    p.reduceFullness("ALL",41);
    cout << p.warnFullness() << endl;
    p.reduceFullness("Player 2", 10);
    cout << p.warnFullness() << endl;
    p.listPartyInfo(output);
    for (string s : output){
        cout << s << endl;
    }
    p.reduceIngredients(-20);
    cout << p.attemptCook("Cookware 2",10) << endl; */
    
    // TODO: check transaction

    /* // check getCombatModifier
    cout << p.getCombatModifier() << endl;
    p.changeWeapons("Player 1","Test Weapon 3");
    p.changeWeapons("Player 2","Test Weapon 3");
    p.changeWeapons("Player 3","Test Weapon 3");
    cout << p.getCombatModifier() << endl; // expecting mults of 12

    p.changeWeapons("Player 1","Test Weapon 3");
    p.changeWeapons("Player 2","Test Weapon 2");
    p.changeWeapons("Player 3","Test Weapon 1");
    cout << p.getCombatModifier() << endl; // expecting 4 + mults of 9

    p.changeWeapons("Player 1","(none)");
    p.changeWeapons("Player 2","Test Weapon 2");
    p.changeWeapons("Player 3","Test Weapon 1");
    cout << p.getCombatModifier() << endl; // expecting 4 + mults of 5 */

}