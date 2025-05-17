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


class Chess {
    
    char board[8][8];
    std::string position;
    std::pair<int, int> coordination;
    std::vector<std::string> allPossibleMoves;
    bool isWhiteTurn = true;
    
    // Init board
    void initializeBoard();
    
    // Print board
    void printBoard();
    
    // Option opening or piece placement (for testing)
    void customBoard(int option = 0);
    
    // e.g. n = knight
    std::string fullName(char piece);
    
    // Valid notations
    bool isValidNote(std::string position);
    
    // Turn string position into number expressions (e4)=> (4, 4) also return true if it's a pawn move
    bool toNumberAndIsPawnMove(std::string position, std::pair<int, int>& coordination);
    
    // Turn int, int coordination to string position
    std::string toPosition(std::pair<int, int>& coordination);
    
    // Check if the move is a capture
    bool isCapturing(std::pair<int, int>& coordination);
    
    // Show legal move
    void legalMoves(int row, int col);
    
    // When is capturing => piece rule, cannot take own piece
    bool isLegal(std::pair<int, int>& coordination);
    
    // Pawn move, also check validation
    bool Pawn(bool isCapturing, std::pair<int, int>& coordination);
    
    // Other pieces move, also check validation
    bool PieceMove(char piece, bool isCapturing);
    
    // Two same pieces may be moved to the same block, ask user which to move
    void twoPiecePossible(char piece, std::vector<std::pair<int, int>> possibleMoves);
    
    // Returns a list of all possible moves for player at that round
    std::vector<std::string>& findAllPossibleMoves();
    
    std::vector<std::pair<int, int>> pieceLogic(char piece, std::pair<int, int> coordination);
    
    bool pieceRule(std::pair<int, int> coordination);
    
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
