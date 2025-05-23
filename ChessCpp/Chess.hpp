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
    std::vector<PieceMove> whiteMoves;
    std::vector<PieceMove> blackMoves;
    std::vector<PieceMove> specialMoves; // En passant, castle, according to turns
    std::vector<std::pair<int, int>> tempMoves;

    bool isWhiteTurn = true;
    bool isWhitePiece = true;
    bool canEnPassant =false;
    bool isSimulatingEnPassant = false;

    // White king side, white queen side, black king side, black queen side [4] -> Whole game [4] + temporary [4] + castled [2]
    bool canCastle[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    
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
   void enPassant(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int> pointB);

    // Chess_status.cpp
    // Init
    void initializeBoard();
    
    // Checkmate and stalemates
    bool isGameEnded();
    
    // Make a move
    void makeMove(std::string& pos);

    // Check castling
    void castling();
    
    
    
    
    
    
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
    
    // Engine part
    int engine_PieceCount(char (&bd)[8][8]);
    
public:
    // Start game
    void newGame();
    
};
#endif /* Chess_hpp */
