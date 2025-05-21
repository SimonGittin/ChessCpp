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
    std::vector<std::pair<int, int>> tempMoves;

    bool isWhiteTurn = true;
    bool isWhitePiece = true;
    
    // Init board
    void initializeBoard();
    
    // Print board
    void printBoard();
    
    // Option opening or piece placement (for testing)
    void customBoard(int option = 0);
    
    // Simple coordination to notation function => {2, 3} => d6
    std::string pointToPos(std::pair<int, int>&);

    
    // Check validation when moved from point a to b
    // Queen == Rook + Bishop
    bool Pawn(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Rook(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Bishop(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Knight(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool King(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    
    // Promotion
    void promotion(std::pair<int, int>&);
    
    void makeMove(std::string& pos);
    
    // Returns a list of all possible moves for player at that round
    void findAllPossibleMoves();
        
    // Read the value
    void printAllPossibleMoves();
    
    std::vector<std::pair<int, int>> pieceLogic(bool checkNeeded, char (&bd)[8][8], char piece, std::pair<int, int>& place);
        
    // Under check or not
    bool atCheck(char piece, std::pair<int, int>& from, std::pair<int, int>& toPoint);
    
    // Checkmate and stalemates
    bool isGameEnded();

    
public:
    // Start game
    void newGame();
    
    // When turn begins, check under checks first
    
};


#endif /* Chess_hpp */
