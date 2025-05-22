//
//  Chess_log.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/22/25.
//

#pragma once
#include "Chess.hpp"

// print functions
void Chess::printBoard(){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << '\n';
    }
}


void Chess::printAllPossibleMoves(){
    
    std::cout << "White moves: ";
    for (auto& move: whiteMoves){
        std::cout <<  move.note << "  ";
    }
    std::cout << "\nBlack moves: ";
    
    for (auto& move: blackMoves){
        std::cout << move.note << "  ";
    }
    std::cout << "\n";
}
