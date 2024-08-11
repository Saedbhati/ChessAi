
#include<iostream>
#include <cmath>
#include <map>



using namespace std;
typedef uint64_t U64;
bool isWhiteKingCheck(U64 blackPieces,map<string,U64> pieces){
    if(blackPieces & pieces["whiteKing"]){
        cout<<"White is check";
    }


}