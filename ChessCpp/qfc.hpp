//
//  qfc.hpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/21/25.
//

#ifndef qfc_hpp
#define qfc_hpp

#include <stdio.h>
#include <iostream>
#include "Chess.hpp"




// Quick function
class qfc {
private:
    qfc() = delete;
    
    // Work with processMoves
    static std::vector<int> repeatedIndices(const std::vector<std::string>& vec);
    
    // Work with diagonals and straights
    bool Pawn(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Rook(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Bishop(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool Knight(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
    bool King(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB);
public:
    static void findKing(std::pair<int, int>&, char (&bd)[8][8], char (&piece));
    
    // Returns int vec of repeated indices, work with processMoves to simplify notations
    static std::vector<std::string> processMoves(std::vector<std::string>& rawMoves);
    
    void diagonals(char (&bd)[8][8], std::pair<int, int>&, std::vector<std::pair<int, int>>& vecToEdit);
    
    bool atCheck(char piece, std::pair<int, int>& from, std::pair<int, int>& toPoint);

    
    
};


#endif /* qfc_hpp */
