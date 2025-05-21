//
//  qfc.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/21/25.
//

#include "qfc.hpp"




void qfc::findKing(std::pair<int, int>& kPoint, char (&bd)[8][8], char (&piece)){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (bd[i][j] == piece) {
                kPoint = {i, j};
                break;
            }
        }
    }
}


std::vector<int> qfc::repeatedIndices(const std::vector<std::string>& vec){
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
    } return result;
}

std::vector<std::string> qfc::processMoves(std::vector<std::string>& rawMoves) {
    
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
    
    std::vector<std::string> condensedMoves;
    
    std::vector<std::string> pieceResolvedCol;
    std::vector<std::string> pieceResolvedRow;
    std::vector<bool> rowChecker(rawMoves.size(), false);
    std::vector<bool> colChecker(rawMoves.size(), false);
    
    std::vector<std::string> finalMoves;

    std::vector<int> repetitionIndices;
    
    // 1. Store condensed moves and check the repetition
    for(auto& rawMove : rawMoves){
        if (rawMove.size() > 2) {
            if (rawMove.size() == 4) condensedMoves.push_back(rawMove.substr(rawMove.size() - 2));
            if (rawMove.size() == 5) condensedMoves.push_back(rawMove[0] + rawMove.substr(rawMove.size() - 2));
        }
        // 2. Return int vec of repeated items' indices
        repetitionIndices = repeatedIndices(condensedMoves);
    }
    
    for (int i = 0; i < rawMoves.size(); ++i) {
        // 3. Pawn
        // If repeated
        if (rawMoves[i].size() == 4) {
            if (find(repetitionIndices.begin(), repetitionIndices.end(), i) != repetitionIndices.end()) {
                std::string combined;
                combined.push_back(rawMoves[i][0]); // [point a col]
                combined+=condensedMoves[i]; // [point b]
                pieceResolvedCol.push_back(combined);
                pieceResolvedRow.push_back(combined);
            }
            // If unique
            else{
                pieceResolvedCol.push_back(condensedMoves[i]); // [point b]
                pieceResolvedRow.push_back(condensedMoves[i]);
            }
        }
        
        // 4. Other piece
        // If repeated
        if (rawMoves[i].size() == 5) {
            
            // Store in container A & B, and cross check
            if (find(repetitionIndices.begin(), repetitionIndices.end(), i) != repetitionIndices.end()) {
                
                // Same row
                std::string combined;
                combined.push_back(rawMoves[i][0]); // [piece]
                combined.push_back(rawMoves[i][1]); // [point a col]
                combined+=condensedMoves[i][1]; // [point b col]
                combined+=condensedMoves[i][2]; // [point b row]
                pieceResolvedCol.push_back(combined); // => [piece][point a col][point b]
                
                // Same col
                combined[1] = rawMoves[i][2]; // Change [point a col] to [point a row]
                pieceResolvedRow.push_back(combined);
                
            }
            // If unique
            else{
                std::string combined;
                combined.push_back(rawMoves[i][0]); // [piece]
                combined+=condensedMoves[i][1]; // [point b col]
                combined+=condensedMoves[i][2]; // [point b row]
                pieceResolvedCol.push_back(combined); // [piece][point b]
                pieceResolvedRow.push_back(combined);
            }
        }
    }
    
    // 5. Level 2 repetition check, store repetitions as TRUE into bool vecs
    for (int i = 0; i < 2; i++) {
        // Clean for second use
        repetitionIndices.clear();
        
        if(i == 0) repetitionIndices = repeatedIndices(pieceResolvedCol);
        if(i == 1) repetitionIndices = repeatedIndices(pieceResolvedRow);
        
        // If repetition exists, change to TRUE for that spot
        if (!repetitionIndices.empty()) {
            for (auto& j: repetitionIndices){
                if(i == 0) colChecker[j] = true;
                if(i == 1) rowChecker[j] = true;
            }
        }
    }
    
    // 6. Cross validation
    for (int i = 0; i < rawMoves.size(); i++) {
        
        // Unique: Ke3e4 => Ke4
        if (!rowChecker[i] && !colChecker[i]) finalMoves.push_back(pieceResolvedCol[i]);
        
        // Same row: Qa2b3 & Qc2b3 => Qab3 & Qcb3
        if (rowChecker[i] && !colChecker[i]) finalMoves.push_back(pieceResolvedCol[i]);
        
        // Same col: Qa2b3 & Qa4b3 => Q2b3 & Q4b3
        if (!rowChecker[i] && colChecker[i]) finalMoves.push_back(pieceResolvedRow[i]);
        
        // All same: Qf7f6 & Qg7f6 & Qg6f6 => Qff6 & Qg7f6 & Q6f6
        if (rowChecker[i] && colChecker[i]) finalMoves.push_back(rawMoves[i]);
    }
//    for (int i = 0; i < rawMoves.size(); i++){
//        rawMoves[i] = finalMoves[i];
//    }
    rawMoves = finalMoves;
    return finalMoves;
}
