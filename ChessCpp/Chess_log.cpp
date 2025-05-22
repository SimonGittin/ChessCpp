//
//  Chess_log.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/22/25.
//

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


void Chess::printSpecialMoves(){
    
    std::cout << "Size: " << specialMoves.size() << std::endl;
    for (auto& move : specialMoves) {
//        std::cout << "A move from " << move.startPos << " to " << move.endPos << std::endl;
        std::cout << "Move: " << move.note << std::endl;
    }
}

void Chess::printTempMoves(char (&bd)[8][8]){
    std::cout << "print" << std::endl;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << bd[i][j] << " ";
        }
        std::cout << '\n';
        
        for (auto& move: tempMoves) {
            std::cout << pointToPos(move) << std::endl;
        }
    }
}
