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

#endif /* Chess_hpp */


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
    std::vector<PieceMove> pieceMove;
    bool isWhiteTurn = true;
    
    // Init board
    void initializeBoard();
    
    // Print board
    void printBoard();
    
    // Option opening or piece placement (for testing)
    void customBoard(int option = 0);
    
    // e.g. n = knight
    std::string fullName(char piece);
    
    // Returns int vec of repeated indices, work with processMoves to simplify notations
    std::vector<int> repeatedIndices(const std::vector<std::string>& vec);
    std::vector<std::string> processMoves(std::vector<std::string>& rawMoves);
    
    // Simple coordination to notation function => {2, 3} => d6
    std::string pointToPos(std::pair<int, int>&);

    
    // Check validation when moved from point a to b
    // Queen == Rook + Bishop
    bool Pawn(std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Rook(std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Bishop(std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Knight(std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool King(std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    
    // Promotion
    void promotion(std::pair<int, int>&);
    
    void makeMove(std::string& pos);
    
    // Returns a list of all possible moves for player at that round
    void findAllPossibleMoves();
    
    // Read the value
    void printAllPossibleMoves();
    
    std::vector<std::pair<int, int>> pieceLogic(char piece, std::pair<int, int>& place);
        
    // Under check or not
    bool atCheck();
    
    // Checkmate
    bool isWin();
    
    // Draw
    bool isDraw();
    
    // Take turn and record current player
    void takeTurn();
public:
    // Start game
    void newGame();
    
    // When turn begins, check under checks first
    
};


