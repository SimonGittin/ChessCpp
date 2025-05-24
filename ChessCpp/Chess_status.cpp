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
    customBoard(4);
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
    
    // Make castling
    // 0 1 2 3
    // 4 5 6 7
    // 8   9
    //
    // White king side
    if (canCastle[8] && canCastle[0] && canCastle[4] && pos == "oo" && isWhiteTurn) {
        board[7][4] = '.';
        board[7][7] = '.';
        board[7][6] = 'k';
        board[7][5] = 'r';
        canCastle[8] = 0;
        return;
    }
    
    // White queen side
    if (canCastle[8] && canCastle[1] && canCastle[5] && pos == "ooo" && isWhiteTurn) {
        board[7][0] = '.';
        board[7][4] = '.';
        board[7][2] = 'k';
        board[7][3] = 'r';
        canCastle[8] = 0;
        return;
    }
    
    // Black king side
    if (canCastle[9] && canCastle[2] && canCastle[6] && pos == "oo" && !isWhiteTurn) {
        board[0][4] = '.';
        board[0][7] = '.';
        board[0][6] = 'K';
        board[0][5] = 'R';
        canCastle[9] = 0;
        return;
    }
    
    // Black quen side
    if (canCastle[9] && canCastle[3] && canCastle[7] && pos == "ooo" && !isWhiteTurn) {
        board[0][0] = '.';
        board[0][4] = '.';
        board[0][2] = 'K';
        board[0][3] = 'R';
        canCastle[9] = 0;
        return;
    }
    
    
    // Iterate through pieceMove to get index of pm.note == position
    auto it = std::find_if(moves.begin(), moves.end(), [&](const struct PieceMove& pm) {return pm.note == pos;});
    if (it != moves.end()) {
        
        // Get the move's index
        size_t idx = std::distance(moves.begin(), it);
        
        // Promotion of pawn
        if (moves[idx].pieceT == (isWhiteTurn ? 'p': 'P') && moves[idx].endPoint.first == (isWhiteTurn? 0: 7)) {
            promotion(moves[idx].endPoint);
        }
        
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
                    enPassant(board, point, {point.first, point.second - i});
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
                    enPassant(board, point, {point.first, point.second - i});
                }
            }
        }
    }
}

void Chess::castling(){
    // Priority
    // canCastle[8, 9] > [0 - 3] > [4 - 7]
    //
    // Everytime this function is called, update temporary bool to true
    canCastle[4] = 1;
    canCastle[5] = 1;
    canCastle[6] = 1;
    canCastle[7] = 1;

    
    // 1. Rooks and king must at position and cannot be moved, otherwise lose castle right for that side (during game)
    if (board[7][7] != 'r') canCastle[0] = 0;
    if (board[7][0] != 'r') canCastle[1] = 0;
    if (board[0][7] != 'R') canCastle[2] = 0;
    if (board[0][0] != 'R') canCastle[3] = 0;
    if (board[7][4] != 'k') canCastle[8] = 0;
    if (board[0][4] != 'K') canCastle[9] = 0;

    // 2. King's left and right 2 blocks are not occupied or can be reached by opponent (temporary)
    if (board[7][5] != '.' || board[7][6] != '.') canCastle[4] = 0;
    if (board[7][2] != '.' || board[7][3] != '.') canCastle[5] = 0;
    if (board[0][5] != '.' || board[0][6] != '.') canCastle[6] = 0;
    if (board[0][2] != '.' || board[0][3] != '.') canCastle[7] = 0;
    
    for (auto& move: blackMoves) {
        if (move.endPoint == std::pair<int, int>{7,5} || move.endPoint == std::pair<int, int>{7,6}) canCastle[4] = 0;
        if (move.endPoint == std::pair<int, int>{7,2} || move.endPoint == std::pair<int, int>{7,3}) canCastle[5] = 0;
        if (move.endPoint == std::pair<int, int>{7,4}) {canCastle[4] = 0; canCastle[5] = 0;}
    }
    for (auto& move: whiteMoves) {
        if (move.endPoint == std::pair<int, int>{0,5} || move.endPoint == std::pair<int, int>{0,6}) canCastle[6] = 0;
        if (move.endPoint == std::pair<int, int>{0,2} || move.endPoint == std::pair<int, int>{0,3}) canCastle[7] = 0;
        if (move.endPoint == std::pair<int, int>{0,4}) {canCastle[6] = 0; canCastle[7] = 0;}
    }
}
