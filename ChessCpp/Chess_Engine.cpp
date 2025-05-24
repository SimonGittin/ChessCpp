//
//  Chess_Engine.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/23/25.
//

#include "Chess.hpp"

int Chess::engine_PieceCount(char (&bd)[8][8]){
    
    int pointState = 0;
    for (size_t i = 0; i < 8; i ++) {
        for (size_t j = 0; j < 8; j++) {
            if (bd[i][j] == 'q') pointState += 9;
            if (bd[i][j] == 'Q') pointState -= 9;
            if (bd[i][j] == 'r') pointState += 5;
            if (bd[i][j] == 'R') pointState -= 5;
            if (bd[i][j] == 'b') pointState += 3;
            if (bd[i][j] == 'B') pointState -= 3;
            if (bd[i][j] == 'n') pointState += 3;
            if (bd[i][j] == 'N') pointState -= 3;
            if (bd[i][j] == 'p') pointState += 1;
            if (bd[i][j] == 'P') pointState -= 1;
        }
    }
    return pointState;
}
