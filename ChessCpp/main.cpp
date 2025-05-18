//
//  main.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/16/25.
//

#include <iostream>
#include "Chess.hpp"


using namespace std;


std::vector<int> repeatedIndices(const std::vector<std::string>& vec){
    // Store indices for each string
    std::unordered_map<std::string, std::vector<int>> indexMap;
    std::vector<int> result;
    for (int i = 0; i < vec.size(); ++i) {
        indexMap[vec[i]].push_back(i);
    }

    // Add indices that have more than one occurrence
    for (const auto& pair : indexMap) {
        if (pair.second.size() > 1) {
            result.insert(result.end(), pair.second.begin(), pair.second.end());
        }
    }
    
    return result;
}

vector<string> processMoves(const vector<string>& rawMoves) {
    
    // Inputs are vectors of length of 4 or 5 sized string,
    // 4 sized => pawn, 5 sized => other piece move
    // It's a structure of [piece][point a][point b] e.g. e3e4, qg4g5
    //
    // 1. Store it into [piece][point b] form => e.g. e4, qg5
    // 2. Check if any repetition happens => e.g. qg4g5 & qg6g5 CAUSE REPETITION => qg5
    // 3. For pawns, if repeated, simply add point A column on it. e.g. [point a col][point b] ab3 & cb3
    // 4. For others, try [piece][point a col][point b] & [piece][point a row][point b] and store it in 2 string vectors
    // 5. Returns other 2 bool vectors if repetition happens after revision => {false, false, true,....}
    // 6. In 2 bool vecs, cross validation => 00 == unique, 01 == same row, 10 same column, 11 == same row & col (same piece > 2)
    // 7. Store it in final container then return
    
    vector<string> condensedMoves;
    
    vector<string> pieceResolvedCol;
    vector<string> pieceResolvedRow;
    vector<bool> rowChecker(rawMoves.size(), false);
    vector<bool> colChecker(rawMoves.size(), false);
    
    vector<string> finalMoves;

    vector<int> repetitionIndices;
    
    // 1. Store condensed moves and check the repetition
    for(auto& rawMove : rawMoves){
        if (rawMove.size() > 2) {
            if (rawMove.size() == 4) condensedMoves.push_back(rawMove.substr(rawMove.size() - 2));
            if (rawMove.size() == 5) condensedMoves.push_back(rawMove[0] + rawMove.substr(rawMove.size() - 2));
        }
        
        // Return int vec of repeated items' indices
        repetitionIndices = repeatedIndices(condensedMoves);
    }
    
    for (int i = 0; i < rawMoves.size(); ++i) {
        // Pawn
        // If repeated
        if (rawMoves[i].size() == 4) {
            if (find(repetitionIndices.begin(), repetitionIndices.end(), i) != repetitionIndices.end()) {
                std::string combined;
                combined.push_back(rawMoves[i][0]);
                combined+=condensedMoves[i];
                pieceResolvedCol.push_back(combined);
                pieceResolvedRow.push_back(combined);
            }
            // If unique
            else{
                pieceResolvedCol.push_back(condensedMoves[i]);
                pieceResolvedRow.push_back(condensedMoves[i]);
            }
        }
        
        // Other piece
        // If repeated, method 1 -> same row or diff row&col
        if (rawMoves[i].size() == 5) {
            
            // Store in container A & B, and cross check
            if (find(repetitionIndices.begin(), repetitionIndices.end(), i) != repetitionIndices.end()) {
                
                // Same row
                std::string combined;
                combined.push_back(rawMoves[i][0]);
                combined.push_back(rawMoves[i][1]);
                combined+=condensedMoves[i][1];
                combined+=condensedMoves[i][2];
                pieceResolvedCol.push_back(combined);
                
                // Same col
                combined[1] = rawMoves[i][2];
                pieceResolvedRow.push_back(combined);
                
            }
            // If unique
            else{
                std::string combined;
                combined.push_back(rawMoves[i][0]);
                combined+=condensedMoves[i][1];
                combined+=condensedMoves[i][2];
                pieceResolvedCol.push_back(combined);
                pieceResolvedRow.push_back(combined);
            }
        }
    }
    
    for (int i = 0; i < 2; i++) {
        repetitionIndices.clear();
        
        // If same column conflict exists
        if(i == 0) repetitionIndices = repeatedIndices(pieceResolvedCol); // Check method 1 purity
        if(i == 1) repetitionIndices = repeatedIndices(pieceResolvedRow); // Check method 1 purity
        
        // If exists, find all pointB, for loop through noteC
        if (!repetitionIndices.empty()) {
            for (auto& j: repetitionIndices){
                if(i == 0) colChecker[j] = true;
                if(i == 1) rowChecker[j] = true;
            }
        }
    }
    
    for (int i = 0; i < rawMoves.size(); i++) {
        
        // Unique
        if (!rowChecker[i] && !colChecker[i]) finalMoves.push_back(pieceResolvedCol[i]);
        
        // Same row
        if (!rowChecker[i] && colChecker[i]) finalMoves.push_back(pieceResolvedRow[i]);
        
        // Same col
        if (rowChecker[i] && !colChecker[i]) finalMoves.push_back(pieceResolvedCol[i]);
        
        // All same
        if (rowChecker[i] && colChecker[i]) finalMoves.push_back(rawMoves[i]);
    }
    
    for (auto i: finalMoves){
        cout << i << endl;
    }
    return finalMoves;
}


int main(int argc, const char * argv[]) {
//    Chess chess;
//    chess.newGame();
    

    vector<string> input = {
        "rb5b3", "rb1b3", "nc5e4", "nc3e4", "nf2e4", "qf7f6", "qg7f6", "qg6f6", "f3g4", "h3g4", "ke3e4", "h6h7",
    };

    vector<string> output = processMoves(input);
//    
    


//    for (const string& s : output) {
//        cout << s.ends_with("b3") << endl;
//    }
    
//    std::string test = "ggg32";
//    cout << test.length() << test.size() << "\n";
    cout << endl;
}
