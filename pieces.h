#include<iostream>
#include <cmath>
#include <map>



using namespace std;
typedef uint64_t U64;

U64 whitePawn(int rank,U64 squareIndex,const char selectedpiece[],map<char, int> filemapping,U64 blackPieces){
    U64 lookup=0;
    if (!(squareIndex << 8 & blackPieces)) {
                lookup |= squareIndex << 8;
                cout << selectedpiece[0] << rank + 2 << ' ';
                if (rank == 1 && !(squareIndex << 16 & blackPieces)) {
                    lookup |= squareIndex << 16;
                    cout << selectedpiece[0] << rank + 3 << ' ';
                }
            }

            if (squareIndex << 9 & blackPieces && int(log2(squareIndex << 9)) >> 3 == rank + 1) {
                lookup |= squareIndex << 9;
                auto it = filemapping.find(selectedpiece[0] + 1);
                if (it != filemapping.end()) {
                    cout << "Pawn can capture on: " << it->first << rank + 2 << endl;
                }
            }
            if (squareIndex << 7 & blackPieces && int(log2(squareIndex << 7)) >> 3 == rank + 1) {
                lookup |= squareIndex << 7;
                auto it = filemapping.find(selectedpiece[0] - 1);
                if (it != filemapping.end()) {
                    cout << "Pawn can capture on: " << it->first << rank + 2 << endl;
                }
            }

    return lookup;

}
U64 blackPawn(int rank,U64 squareIndex,const char selectedpiece[],map<char, int> filemapping,U64 whitePieces){
    U64 lookup=0;
    
    if (!(squareIndex >> 8 & whitePieces)) {
                lookup |= squareIndex >> 8;
                cout << selectedpiece[0] << rank << ' ';
                if (rank == 6 && !(squareIndex >> 16 & whitePieces)) {
                    lookup |= squareIndex >> 16;
                    cout << selectedpiece[0] << rank - 1 << ' ';
                }
            }

            if (squareIndex >> 9 & whitePieces && int(log2(squareIndex >> 9)) >> 3 == rank - 1) {
                lookup |= squareIndex >> 9;
                auto it = filemapping.find(selectedpiece[0] - 1);
                if (it != filemapping.end()) {
                    cout << "Pawn can capture on: " << it->first << rank << endl;
                }
            }
            if (squareIndex >> 7 & whitePieces && int(log2(squareIndex >> 7)) >> 3 == rank - 1) {
                lookup |= squareIndex >> 7;
                auto it = filemapping.find(selectedpiece[0] + 1);
                if (it != filemapping.end()) {
                    cout << "Pawn can capture on: " << it->first << rank << endl;
                }
            }
return lookup;

}

U64 Knight(int rank,int file, U64 squareIndex,map<string,U64> pieces,U64 whitePieces,U64 blackPieces){
    U64 lookup=0;
     int knightMoves[8][2] = {
                {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
            };

            for (auto &move : knightMoves) {
                int newRank = rank + move[0];
                int newFile = file + move[1];
                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                    U64 newSquareIndex = 1ULL << (8 * newRank + newFile);
                    if ((squareIndex & pieces["whiteKnights"] && !(newSquareIndex & whitePieces)) || 
                        (squareIndex & pieces["blackKnights"] && !(newSquareIndex & blackPieces))) {
                        lookup |= newSquareIndex;
                        cout << (char)('a' + newFile) << newRank + 1 << ' ';
                    }
                }
            }
    return lookup;
}

U64 King(int rank,int file,U64 squareIndex,map<string,U64> pieces,U64 whitePieces,U64 blackPieces){
    U64 lookup=0;
    int kingMoves[8][2] = {
                {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
            };

            for (auto &move : kingMoves) {
                int newRank = rank + move[0];
                int newFile = file + move[1];
                if (newRank >= 0 && newRank < 8 && newFile >= 0 && newFile < 8) {
                    U64 newSquareIndex = 1ULL << (8 * newRank + newFile);
                    if ((squareIndex & pieces["whiteKing"] && !(newSquareIndex & whitePieces)) || 
                        (squareIndex & pieces["blackKing"] && !(newSquareIndex & blackPieces))) {
                        lookup |= newSquareIndex;
                        cout << (char)('a' + newFile) << newRank + 1 << ' ';
                    }
                }
            }
    return lookup;

}

U64 Rook(int rank, int file, U64 squareIndex, map<string, U64> pieces, U64 whitePieces, U64 blackPieces) {
    U64 lookup = 0;
    int rookDirections[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    bool isWhite = (squareIndex & pieces["whiteRooks"]) || (squareIndex & pieces["whiteQueens"]);

    for (auto &direction : rookDirections) {
        int newRank = rank;
        int newFile = file;
        while (true) {
            newRank += direction[0];
            newFile += direction[1];
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) break;
            U64 newSquareIndex = 1ULL << (8 * newRank + newFile);
            if (isWhite) {
                if (newSquareIndex & whitePieces) break;
            } else {
                if (newSquareIndex & blackPieces) break;
            }
            lookup |= newSquareIndex;
            cout << (char)('a' + newFile) << newRank + 1 << ' ';
            if (isWhite && (newSquareIndex & blackPieces)) break;
            if (!isWhite && (newSquareIndex & whitePieces)) break;
        }
    }

    cout << endl;
    return lookup;
}
U64 Bishop(int rank, int file, U64 squareIndex, map<string, U64> pieces, U64 whitePieces, U64 blackPieces) {
    U64 lookup = 0;
    int bishopDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    bool isWhite = (squareIndex & pieces["whiteBishops"]) || (squareIndex & pieces["whiteQueens"]);

    for (auto &direction : bishopDirections) {
        int newRank = rank;
        int newFile = file;
        while (true) {
            newRank += direction[0];
            newFile += direction[1];
            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) break;
            U64 newSquareIndex = 1ULL << (8 * newRank + newFile);
            if (isWhite) {
                if (newSquareIndex & whitePieces) break;
            } else {
                if (newSquareIndex & blackPieces) break;
            }
            lookup |= newSquareIndex;
            cout << (char)('a' + newFile) << newRank + 1 << ' ';
            if (isWhite && (newSquareIndex & blackPieces)) break;
            if (!isWhite && (newSquareIndex & whitePieces)) break;
        }
    }

    cout << endl;
    return lookup;
}



    
   
