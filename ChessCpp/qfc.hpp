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




// Quick function
class qfc {
private:
    qfc() = delete;
    
    // Work with processMoves
    static std::vector<int> repeatedIndices(const std::vector<std::string>& vec);
public:
    static void findKing(std::pair<int, int>&, char (&bd)[8][8], char (&piece));
    
    // Returns int vec of repeated indices, work with processMoves to simplify notations
    static std::vector<std::string> processMoves(std::vector<std::string>& rawMoves);
};


#endif /* qfc_hpp */
