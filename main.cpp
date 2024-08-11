#include<iostream>
#include "Game.h"
using namespace std;
int main(){
    Game Chess;
    while (true)
    {
        Chess.printBoard();

       Chess.Game_input();
    }
}