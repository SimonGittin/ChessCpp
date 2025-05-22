//
//  Chess.hpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/16/25.
//

#ifndef Chess_hpp
#define Chess_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <utility>
#include "qfc.hpp"




// pawn move forward a step, takes pieces diagonally
// bishop moves diagonally
// rook moves h v
// queen moves h v d
// king moves h v d one block
// knight moves 1 and 2 blocks
//
// pieces cannot move outside the board, cannot take own pieces
// if undercheck, must get out of check first
// Shows legal move for pieces every turn

struct PieceMove {
    // Position (string) h3, Qd4
    std::string startPos;
    std::string endPos;
    
    // Coordination
    std::pair<int, int> startPoint;
    std::pair<int, int> endPoint;
    
    // Piece type
    char pieceT;
    
    std::string note;
};


class Chess {
    
    char board[8][8];
    std::string position;
    std::pair<int, int> coordination;
    std::vector<PieceMove> whiteMoves;
    std::vector<PieceMove> blackMoves;
    std::vector<PieceMove> specialMoves; // En passant, castle, according to turns
    std::vector<std::pair<int, int>> tempMoves;

    bool isWhiteTurn = true;
    bool isWhitePiece = true;
    bool canEnPassant =false;

    
    
    
    
    
    // Chess_log.cpp
    void printBoard();
    void printAllPossibleMoves();
    void printSpecialMoves();
    void printTempMoves(char(&bd)[8][8]);
    
    
    
    
    // Chess_movement.cpp
    //
    // Check validation
    std::vector<std::pair<int, int>> pieceLogic(bool checkNeeded, char (&bd)[8][8], char piece, std::pair<int, int>& place);
    void diagonals(bool, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit);
    bool bishopCheck(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    void straights(bool, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit);
    bool rookCheck(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    void knight(bool, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit);
    void king(bool, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit);
    bool pieceCheck(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    void pawn(bool, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit);
    bool pawnCheck(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    // Promotion
    void promotion(std::pair<int, int>&);
   void EnPassant(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int> pointB);

    // Chess_status.cpp
    // Init
    void initializeBoard();
    void initTurn();
    
    // Checkmate and stalemates
    bool isGameEnded();
    
    // Make a move
    void makeMove(std::string& pos);

    // Make en passant
    void makeEnPassant(std::string& pos);
    
    
    
    
    
    
    // Chess.cpp
    //
    // Option opening or piece placement (for testing)
    void customBoard(int option = 0);
    
    // Simple coordination to notation function => {2, 3} => d6
    std::string pointToPos(std::pair<int, int>&);
    
    // Returns a list of all possible moves for player at that round
    void findAllPossibleMoves();
                
    // Under check or not
    bool atCheck(char piece, std::pair<int, int>& from, std::pair<int, int>& toPoint);
    
    

    
public:
    // Start game
    void newGame();
    
    // When turn begins, check under checks first
    
};


#endif /* Chess_hpp */
