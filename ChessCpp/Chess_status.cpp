//
//  Chess_status.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/22/25.
//

#include "Chess.hpp"


void Chess::initializeBoard(){
    // Fill board with .
    memset(board, '.', sizeof(board));
    customBoard(0);
}

void Chess::initTurn(){
    canEnPassant = false;
    specialMoves.clear();
}

bool Chess::isGameEnded(){
    
    std::pair<int, int> kPoint;
    
    char kingChar = (isWhiteTurn ? 'k':'K');
    
    qfc::findKing(kPoint, board, kingChar);
        
    // During white's turn, if whiteMove has size of 0, it must be a stalemate or a checkmate by black
    if (isWhiteTurn && whiteMoves.size() == 0) {
        
        // If any of black's move's .endPoint has king's position, it's checkmate, else stalemate
        if (std::any_of(blackMoves.begin(), blackMoves.end(), [&](const struct PieceMove& pm) {return pm.endPoint == kPoint;})) {
            std::cout << "Checkmate by black!" << std::endl;
            return true;
        }else{
            std::cout << "Stalemate" << std::endl;
            return true;
        }
    }
    if (!isWhiteTurn && blackMoves.size() == 0){
        if (std::any_of(whiteMoves.begin(), whiteMoves.end(), [&](const struct PieceMove& pm) {return pm.endPoint == kPoint;})) {
            std::cout << "Checkmate by white!" << std::endl;
            return true;
        }else{
            std::cout << "Stalemate" << std::endl;
            return true;
        }
    }
    return false;
}

void Chess::makeMove(std::string& pos){
    
    std::vector<PieceMove> moves = isWhiteTurn ? whiteMoves : blackMoves;
    
    
    
    // Iterate through pieceMove to get index of pm.note == position
    auto it = std::find_if(moves.begin(), moves.end(), [&](const struct PieceMove& pm) {return pm.note == pos;});
    if (it != moves.end()) {
        
        // Get the move's index
        size_t idx = std::distance(moves.begin(), it);
        
        
        
        // Promotion of pawn
        if (moves[idx].pieceT == (isWhiteTurn ? 'p': 'P') && moves[idx].endPoint.first == (isWhiteTurn? 0: 7)) {
            promotion(moves[idx].endPoint);
        }
//        else if (moves[idx].pieceT == 'p' && moves[idx].endPoint.first == (isWhiteTurn? 0: 7)) {
//            promotion(moves[idx].endPoint);
//        }
        
        // Normal piece move
        else{
            // Point B's piece is point A's piece
            board[moves[idx].endPoint.first][moves[idx].endPoint.second] = board[moves[idx].startPoint.first][moves[idx].startPoint.second];
        }
                
        // Clear point A
        board[moves[idx].startPoint.first][moves[idx].startPoint.second] = '.';
        
        // En passant
        if
        (canEnPassant && std::any_of(moves.begin(), moves.end(), [&](const struct PieceMove& pm) {return pm.note == pos;}))
        {
            std::cout << moves[idx].startPos << "     " << moves[idx].endPos << std::endl;
            board[moves[idx].startPoint.first][moves[idx].endPoint.second] = '.';
            canEnPassant = false;
            return;
        }
        
        
        // track long move, white moves from rank 2 to 4
        if (isWhiteTurn && moves[idx].startPoint.first == 6 && moves[idx].endPoint.first == 4) {
            
            // Check if any opponent's piece is on two directions, same row, diff col
            for (int i = -1; i <= 1; i+=2) {
                
                // Long moved white pawn point
                std::pair<int, int> point = moves[idx].endPoint;
                point.second += i;
                
                // If left or right side has black pawn, En passant func
                if (board[point.first][point.second] == 'P') {
                    canEnPassant = true;
                    
                    // Takes opponent's pawn start and end point
                    EnPassant(board, point, {point.first, point.second - i});
                }
            }
        }
        
        // black moves from rank 7 to 5
        if (!isWhiteTurn && moves[idx].startPoint.first == 1 && moves[idx].endPoint.first == 3) {
            for (int i = -1; i <= 1; i+=2) {
                
                // Long moved black pawn point
                std::pair<int, int> point = moves[idx].endPoint;
                point.second += i;
                
                // If left or right side has black pawn, En passant func
                if (board[point.first][point.second] == 'p') {
                    canEnPassant = true;
                    
                    // Takes opponent's pawn start and end point
                    EnPassant(board, point, {point.first, point.second - i});
                }
            }
        }
    }
}


void Chess::makeEnPassant(std::string &pos)
{
    if (isWhiteTurn)
    if(std::any_of(whiteMoves.begin(), whiteMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;}))
    {
        
    }
    
    
}
