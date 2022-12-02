#include "Monster.h"

int main(){
    Monster m1 = Monster(2,"Test Monster");
    cout << m1.getName() << endl;
    cout << "Modifier: " << m1.getCombatModifier() << endl;
}