#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include "pieces.h"

using namespace std;

typedef uint64_t U64;

class Game {
    int turn=0;//0 for white and 1 for black
    map<string, U64> pieces;
    map<char, int> filemapping = {
        {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}
    };
    U64 blackPieces;
    U64 whitePieces;

public:
    Game() {
        pieces = {
            {"whitePawns", 0x000000000000FF00ULL},
            {"whiteKnights", 0x0000000000000042ULL},
            {"whiteBishops", 0x0000000000000024ULL},
            {"whiteRooks", 0x0000000000000081ULL},
            {"whiteQueens", 0x0000000000000008ULL},
            {"whiteKing", 0x0000000000000010ULL},
            {"blackPawns", 0x00FF000000000000ULL},
            {"blackKnights", 0x4200000000000000ULL},
            {"blackBishops", 0x2400000000000000ULL},
            {"blackRooks", 0x8100000000000000ULL},
            {"blackQueens", 0x0800000000000000ULL},
            {"blackKing", 0x1000000000000010ULL},
        };

        blackPieces = pieces["blackPawns"] | pieces["blackKnights"] | pieces["blackBishops"] | pieces["blackRooks"] | pieces["blackQueens"] | pieces["blackKing"];
        whitePieces = pieces["whitePawns"] | pieces["whiteKnights"] | pieces["whiteBishops"] | pieces["whiteRooks"] | pieces["whiteQueens"] | pieces["whiteKing"];
    }

    void printBoard() {
        U64 Bitboard = 0;
        for (const auto &piece : pieces) {
            Bitboard |= piece.second;
        }

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                U64 squareIndex = 1ULL << (8 * rank + file);
                if (Bitboard & squareIndex) {
                    char pieceChar = '.';
                    for (const auto &piece : pieces) {
                        if (piece.second & squareIndex) {
                            if (piece.first == "whitePawns") pieceChar = 'P';
                            else if (piece.first == "whiteKnights") pieceChar = 'N';
                            else if (piece.first == "whiteBishops") pieceChar = 'B';
                            else if (piece.first == "whiteRooks") pieceChar = 'R';
                            else if (piece.first == "whiteQueens") pieceChar = 'Q';
                            else if (piece.first == "whiteKing") pieceChar = 'K';
                            else if (piece.first == "blackPawns") pieceChar = 'p';
                            else if (piece.first == "blackKnights") pieceChar = 'n';
                            else if (piece.first == "blackBishops") pieceChar = 'b';
                            else if (piece.first == "blackRooks") pieceChar = 'r';
                            else if (piece.first == "blackQueens") pieceChar = 'q';
                            else if (piece.first == "blackKing") pieceChar = 'k';
                        }
                    }
                    cout << pieceChar << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }

    void Game_input() {
        string selectpiece;
        cout << "Enter file and rank to select a piece: ";
        cin >> selectpiece;
        if (selectpiece.size() == 2 && filemapping.count(selectpiece[0]) && isdigit(selectpiece[1])) {
            generateMove(selectpiece.c_str());
        } else {
            cout << "Invalid input. Please enter a valid file and rank (e.g., e2)." << endl;
        }
    }
    
    U64 generateMove(const char selectedpiece[]) {
    int rank = selectedpiece[1] - '1';
    int file = filemapping[selectedpiece[0]];
    cout << "File: " << file << ", Rank: " << rank << endl;

    U64 squareIndex = 1ULL << (8 * rank + file);
    cout << "Possible Move: ";

    if (squareIndex & pieces["whitePawns"]) {
        Move(whitePawn(rank, squareIndex, selectedpiece, filemapping, blackPieces), "whitePawns", squareIndex);
    }  
    else if (squareIndex & pieces["blackPawns"]) {
        Move(blackPawn(rank, squareIndex, selectedpiece, filemapping, whitePieces), "blackPawns", squareIndex);
    }
    else if (squareIndex & pieces["whiteKnights"]) {
        Move(Knight(rank, file, squareIndex, pieces, whitePieces, blackPieces), "whiteKnights", squareIndex);
    } 
    else if (squareIndex & pieces["blackKnights"]) {
        Move(Knight(rank, file, squareIndex, pieces, whitePieces, blackPieces), "blackKnights", squareIndex);
    }
    else if (squareIndex & pieces["whiteKing"]) {
        Move(King(rank, file, squareIndex, pieces, whitePieces, blackPieces), "whiteKing", squareIndex);


    }
    else if ( squareIndex & pieces["blackKing"]) {
        Move(King(rank, file, squareIndex, pieces, whitePieces, blackPieces), "blackKing", squareIndex);
    } else if (squareIndex & pieces["whiteRooks"]) {
        U64 lookup = Rook(rank, file, squareIndex, pieces, whitePieces, blackPieces);
        Move(Rook(rank, file, squareIndex, pieces, whitePieces, blackPieces), "whiteRooks", squareIndex);

    }
    else if ( squareIndex & pieces["blackRooks"]) {
        U64 lookup = Rook(rank, file, squareIndex, pieces, whitePieces, blackPieces);
        Move(Rook(rank,file,squareIndex, pieces,whitePieces,blackPieces),"blackRooks",squareIndex);
    }
    else if (squareIndex & pieces["blackBishops"] || squareIndex & pieces["whiteBishops"]) {
        U64 lookup = Bishop(rank, file, squareIndex, pieces, whitePieces, blackPieces);
        Move(lookup, (squareIndex & pieces["whiteBishops"]) ? "whiteBishops" : "blackBishops", squareIndex);
        return lookup;
    } else if (squareIndex & pieces["whiteQueens"] || squareIndex & pieces["blackQueens"]) {
        U64 lookup = 0;
        lookup |= Bishop(rank, file, squareIndex, pieces, whitePieces, blackPieces);
        lookup |= Rook(rank, file, squareIndex, pieces, whitePieces, blackPieces);
        Move(lookup, (squareIndex & pieces["whiteQueens"]) ? "whiteQueens" : "blackQueens", squareIndex);
        return lookup;
    }

    cout << endl;
}


   void Move(U64 lookup, string M_piece, U64 prev_state) {
    string to_square;
    cout << "Enter file and rank to move the piece: ";
    cin >> to_square;
    int rank = to_square[1] - '1';
    int file = filemapping[to_square[0]];
    U64 squareIndex = 1ULL << (8 * rank + file);
    if (lookup & squareIndex) {
        pieces[M_piece] &= ~prev_state;
        pieces[M_piece] |= squareIndex;

        
        if (M_piece.find("white") != string::npos) {
            whitePieces &= ~prev_state;
            whitePieces |= squareIndex;
            blackPieces &= ~squareIndex; 
        } else {
            blackPieces &= ~prev_state;
            blackPieces |= squareIndex;
            whitePieces &= ~squareIndex; 
        }
    } else {
        cout << "Invalid move. Try again." << endl;
    }
}

};