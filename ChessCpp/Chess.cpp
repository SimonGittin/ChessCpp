//
//  Chess.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/16/25.
//

#include "Chess.hpp"

void Chess::initializeBoard(){
    // Fill board with .
    memset(board, '.', sizeof(board));
    customBoard(3);
}

void Chess::customBoard(int option){
    switch (option) {
        case 0:
            // Classic layout
            for (int j = 0; j < 8; ++j) {
                board[0][j] = "RNBQKBNR"[j]; // white back rank
                board[1][j] = 'P';          // white pawns
                board[6][j] = 'p';          // black pawns
                board[7][j] = "rnbqkbnr"[j]; // black back rank
            }
            break;
        case 1:
            // Pawn testing
            // Double choice and avoid forward move capturing
            board[6][0] = 'p';
            board[6][1] = 'p';
            board[6][2] = 'p';
            board[5][1] = 'P';
            
            // Long move
            board[6][3] = 'p';
            
            // Long move with barrier at .3 and .4
            board[6][4] = 'p';
            board[5][4] = 'P';
            board[6][5] = 'p';
            board[4][5] = 'P';
            
            // Illegal move prevention and diagonal eat
            board[6][6] = 'p';
            board[5][6] = 'P';
            board[5][7] = 'P';
            
            // Position: b3, d4, e4, f4, g3, g4
            break;
        case 2:
            board[3][4] = 'r';
            board[7][4] = 'n';
            board[3][6] = 'P';
            board[5][2] = 'b';
            board[5][6] = 'p';
            board[1][1] = 'n';
            board[2][3] = 'q';
            board[4][3] = 'r';
            break;
        case 3:
            board[3][3] = 'Q';
            board[4][4] = 'p';
            board[6][6] = 'k';
            board[2][3] = 'K';
            board[6][3] = 'r';
        default:
            break;
    }
}

void Chess::printBoard(){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << '\n';
    }
}


void Chess::printAllPossibleMoves(){
    
//    for (auto& move: (isWhiteTurn ? whiteMoves: blackMoves)){
//        std::cout << move.note << "  ";
//    }
    std::cout << "White moves: ";
    for (auto& move: whiteMoves){
        std::cout <<  move.note << "  ";
    }
    std::cout << "\nBlack moves: ";
    
    for (auto& move: blackMoves){
        std::cout << move.note << "  ";
    }
    
    
    
//    for (auto& i: tempMoves) std::cout << pointToPos(i) << " ";
    
    std::cout << "\n";
}

std::vector<int> Chess::repeatedIndices(const std::vector<std::string>& vec){
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

std::vector<std::string> Chess::processMoves(std::vector<std::string>& rawMoves) {
    
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


std::string Chess::pointToPos(std::pair<int, int>& point){
    std::string pos;
    for (int i = 0; i < 8; ++i) {
        if (point.second == i) {
            pos += std::string(1, "abcdefgh"[i]) + std::to_string(8 - point.first);
        }
    }
    return pos;
}


void Chess::findAllPossibleMoves(){
    // Clean every time it's called
    whiteMoves.clear();
    blackMoves.clear();
    struct PieceMove pm;
    std::vector<std::string> notes_w;
    std::vector<std::string> notes_b;

    
    
    
    // Using coordination, find pieces from left-top to right-top, ends at right-bottom
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            // A loop a piece
            // White turn, find white pieces
                
            // pieceLogic takes the piece type & current place, returns a vector of all possible moves(pair)
            // A loop a move for a piece
            std::pair<int, int> p = {i, j};
            
            if (board[i][j] == '.') continue;
            
            // Determine piece color
            if (islower(board[i][j])) isWhitePiece = true;
            if (isupper(board[i][j])) isWhitePiece = false;

            // Get legal moves, a loop is a move
            for (auto& move : pieceLogic(true, board, board[i][j], p)) // Return vector of pairs of legal moves
            {
                pm.pieceT = board[i][j];
                pm.startPoint = {i, j};
                pm.endPoint = move;
                pm.startPos = pointToPos(pm.startPoint);
                pm.endPos = pointToPos(move);
                
                // White moves
                if (std::islower(board[i][j])) {
                    notes_w.push_back((pm.pieceT == 'p' ? "" : std::string(1,toupper(pm.pieceT))) + pm.startPos + pm.endPos);
                    whiteMoves.push_back(pm);
                }
                
                // Black moves
                if (std::isupper(board[i][j])) {
                    notes_b.push_back((pm.pieceT == 'P' ? "" : std::string(1, pm.pieceT)) + pm.startPos + pm.endPos);
                    blackMoves.push_back(pm);

                }
            }
        }
    }
    
    
    
    
    /// HANDLE UNKNOWN PROBLEMS
    ///
    /// SOMETIMES IT RETURNS WITH STRING LENGTH != 4 OR 5, GET RID OF STRINGS LENGTH IS NOT 4 OR 5
    notes_b.erase( std::remove_if(notes_b.begin(), notes_b.end(),[](const std::string& s)
                                {return s.length() != 4 && s.length() != 5; }),notes_b.end());
    
    notes_w.erase( std::remove_if(notes_w.begin(), notes_w.end(),[](const std::string& s)
                                {return s.length() != 4 && s.length() != 5; }),notes_w.end());
    
    // Simplify notations and store it in Moves
    processMoves(notes_w);
    processMoves(notes_b);
    for (size_t i = 0; i < notes_w.size(); i++) {
        whiteMoves[i].note = notes_w[i];
    }
    
    for (size_t i = 0; i < notes_b.size(); i++) {
        blackMoves[i].note = notes_b[i];
    }
}

// Params: current piece (expressed by loswercase), from point to point & isWhitePiece globally
// atCheck examines EVERY move on board to see if its next move exposes own-side king, regardless of who's turn it is.
bool Chess::atCheck(char piece, std::pair<int, int>& from, std::pair<int, int>& toPoint){
    
    // Clear for every move
    tempMoves.clear();
    std::pair<int, int> place;
    std::pair<int, int> kingPoint;
    
    // tempBoard simulates the board AFTER move is made
    char tempBoard[8][8];
    memcpy(tempBoard, board, sizeof(tempBoard));
    
    // Simulate next move
    tempBoard[from.first][from.second] = '.';
    tempBoard[toPoint.first][toPoint.second] = (isWhitePiece ? piece: toupper(piece));
    
    // Find the king's position
    char kingChar = isWhitePiece ? 'k' : 'K';
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (tempBoard[i][j] == kingChar) {
                kingPoint = {i, j};
                break;
            }
        }
    }
    
    // Loop through each block to get opponent's moves
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            
            // If to-be-examined piece is white, find black, also skip empty blocks
            if (tempBoard[i][j] == '.') continue;
            if (isWhitePiece && islower(tempBoard[i][j])) continue;
            if (!isWhitePiece && isupper(tempBoard[i][j])) continue;
            
            isWhitePiece = !isWhitePiece;
            place = {i, j};
            
            // If piece is white,
            // &move is a move of a piece, type std::pair
            for (auto& move: pieceLogic(false, tempBoard, tempBoard[i][j], place)){
                
                // Add the move
                tempMoves.push_back(move); // tempMoves store every opponent's move without checkNeeded
            }
            
            isWhitePiece = !isWhitePiece;
        }
    }
    
    // If king point is in one of the moves meaning king is exposed to check
    if (std::find(tempMoves.begin(), tempMoves.end(), kingPoint) != tempMoves.end()) {
        return true;
    }
    return false;
}


std::vector<std::pair<int, int>> Chess::pieceLogic(bool checkNeeded, char (&bd)[8][8], char piece, std::pair<int, int>& place){
    std::vector<std::pair<int, int>> allMoveForThisPiece;
    allMoveForThisPiece.clear();
    
    piece = tolower(piece);
    switch (piece) {
        case 'k':
        {
            std::pair<int, int> kingMoves[] = {
                {-1, -1}, {-1, 1}, {1, -1}, {1, 1},
                {0, -1}, {0, 1}, {-1, 0}, {1, 0}
            };

            for (auto& offset : kingMoves) {
                int newX = place.first + offset.first;
                int newY = place.second + offset.second;

                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    std::pair<int, int> move = {newX, newY};
                    
                    // Cannot take own piece && can take enemy piece
                    if (Knight(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
        }break;
        case 'q':
        {
            // Queen == Rook + Bishop
            // Diagonal line
            std::pair<int, int> move;
            for (int i = -9; i < 9; i++) {
                if (i == 0) continue;
                
                // LT to RB
                if (place.first + i >= 0 && place.first + i <=7 && place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first + i, place.second + i};
                    if (Bishop(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
                
                // LB to RT
                if (place.first - i >= 0 && place.first - i <=7 && place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first - i, place.second + i};
                    if (Bishop(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
            
            // Straight line
            for (int i = -9; i < 9; i++) {
                if (i == 0) continue;
                
                // All move on row
                if (place.first + i >= 0 && place.first + i <=7) {
                    move = {place.first + i, place.second};
                    if (Rook(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
                
                // All move on col
                if (place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first, place.second + i};
                    if(Rook(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
        }
            break;
        case 'b':
        {
            // Diagonal line
            std::pair<int, int> move;
            for (int i = -9; i < 9; i++) {
                if (i == 0) continue;
                
                // LT to RB
                if (place.first + i >= 0 && place.first + i <=7 && place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first + i, place.second + i};
                    if (Bishop(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
                
                // LB to RT
                if (place.first - i >= 0 && place.first - i <=7 && place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first - i, place.second + i};
                    if (Bishop(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
            break;
        }
        case 'n':
        {
            std::pair<int, int> knightMoves[] = {
                {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                {2, -1}, {2, 1}, {1, -2}, {1, 2}
            };

            for (const auto& offset : knightMoves) {
                int newX = place.first + offset.first;
                int newY = place.second + offset.second;

                if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                    std::pair<int, int> move = {newX, newY};
                    if (Knight(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
            break;

        }
        case 'r':
        {
            // Straight line
            std::pair<int, int> move;
            for (int i = -9; i < 9; i++) {
                if (i == 0) continue;
                
                // All move on row
                if (place.first + i >= 0 && place.first + i <=7) {
                    move = {place.first + i, place.second};
                    if (Rook(bd, place, move)){
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
                
                // All move on col
                if (place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first, place.second + i};
                    if(Rook(bd, place, move)) {
                        allMoveForThisPiece.push_back(move);
                        if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                    }
                }
            }
            break;
        }
        case 'p':
        {
            // diagonal 1 move left, right, a move forward and 2 moves forward (4)
            //
            // If forward one move is invalid, then no move is possible, return function
            //
            // a9 -> invalid,
            if (isWhitePiece && (place.first - 1) < 0) {
                return allMoveForThisPiece;
            }
            
            // a0 -> invalid,
            if (!isWhitePiece && (place.first + 1) > 7) {
                return allMoveForThisPiece;
            }

            std::pair<int, int> move;

            // One square moves
            for (int i = -1; i <= 1; i++) {
                
                // Too left or too right -> invalid
                if (place.second + i < 0 || place.second + i > 7) {
                    continue;
                }
                
                move = {isWhitePiece? place.first - 1:place.first + 1, place.second + i};
                
                if (Pawn(bd, place, move)) {
                    allMoveForThisPiece.push_back(move);
                    if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                }
            }
            
            // Long move
            if (isWhitePiece && place.first == 6) {
                move = {4, place.second};
                if (Pawn(bd, place, move)){
                    allMoveForThisPiece.push_back(move);
                    if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                }

            }
            
            if (!isWhitePiece && place.first == 1) {
                move = {3, place.second};
                if (Pawn(bd, place, move)) {
                    allMoveForThisPiece.push_back(move);
                    if (checkNeeded){ if (atCheck(piece, place, move)) allMoveForThisPiece.pop_back();};
                }
                
            }
            break;
        }
        default:
            break;
        
    } // End of switch
    
    return allMoveForThisPiece; // Return coordination
}

void Chess::promotion(std::pair<int, int>& pawnOnFinalRank){
    char pieceToPromoteAs;
    if (pawnOnFinalRank.first != (isWhitePiece? 0: 7)) {
        // False promotion
    }else{
        // Get user input of desired piece
        std::cout << "Which piec to turn into: " << std::endl;
        while (true) {
            std::cin >> pieceToPromoteAs;
            
            if (std::string("qrnb").find(pieceToPromoteAs) != std::string::npos) {
                board[pawnOnFinalRank.first][pawnOnFinalRank.second] = pieceToPromoteAs;
                break;
            }else std::cout << "Please enter a valid piece: " << std::endl;
        }
    }
}

bool Chess::Knight(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){
    // Cannot take own piece
    if (isWhitePiece ? islower(bd[pointB.first][pointB.second]):isupper(bd[pointB.first][pointB.second])) return false;
    
    // Can take enemy piece
    if (isWhitePiece ? isupper(bd[pointB.first][pointB.second]):islower(bd[pointB.first][pointB.second])) return true;
    
    return true;
}

bool Chess::Bishop(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){
    // Cannot take own piece
    // Cannot move through pieces
    
    
    // No pieces is occupying the way
    int disY = pointB.first - pointA.first;
    int disX = pointB.second - pointA.second;
    
    std::pair<bool, bool> isNegaDists;
    
    isNegaDists.first = disY < 0 ? true: false;
    isNegaDists.second = disX < 0? true: false;
    
    disX = abs(disX);
    disY = abs(disY);
    
    
    
    for (int i = 1; i < disX; i++) {
        if (bd[pointA.first + (isNegaDists.first? -i: i)][pointA.second + (isNegaDists.second? -i: i)] != '.') {
            return false;
        }
    }
    
    
    // Cannot take own piece
    if (isWhitePiece ? islower(bd[pointB.first][pointB.second]):isupper(bd[pointB.first][pointB.second])){
        std::cout << "Take" << std::endl;
        return false;
    }
    
    // Can take enemy piece
    if (isWhitePiece ? isupper(bd[pointB.first][pointB.second]):islower(bd[pointB.first][pointB.second])) return true;
    return true;
}

    
bool Chess::Rook(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){

    
    // No pieces is occupying the way
    int distance;
    bool isSameRow = (pointB.first - pointA.first == 0) ? true:false;
    
    if (pointB.first - pointA.first == 0) distance = pointB.second - pointA.second;
    else distance = pointB.first - pointA.first;

    bool isNegaDist = distance < 0;
    distance = abs(distance);
    
    for (int i = 1; i < distance; i++) {
        if (isSameRow) {
            if (bd[pointA.first][pointA.second + (!isNegaDist? i : -i)] != '.') return false;
        }else{
            if (bd[pointA.first + (!isNegaDist? i : -i)][pointA.second] != '.') return false;
        }
    }
    
    // Cannot take own piece
    if (isWhitePiece ? islower(bd[pointB.first][pointB.second]):isupper(bd[pointB.first][pointB.second])) return false;
    
    // Can take enemy piece
    if (isWhitePiece ? isupper(bd[pointB.first][pointB.second]):islower(bd[pointB.first][pointB.second])) return true;
    return true;
}

bool Chess::Pawn(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB){
    // If pawn is on row 2 or 7, it can launch move with 2 blocks
    // En passant
    
    // One square move for WHITE
    if (isWhitePiece && (pointB.first - pointA.first) == -1) {
        // Capturing diagonally
        if ((pointB.second - pointA.second == 1 || pointB.second - pointA.second == -1)) {
            // Opponent's piece
            if (std::string("KQRNBP").find(board[pointB.first][pointB.second]) != std::string::npos) {
                return true;
            }
        }
        
        // Forward move and not capturing
        if (pointB.second == pointA.second && bd[pointB.first][pointB.second] == '.') {
            return true;
        }
    }
    
    // One square move for BLACK
    if (!isWhitePiece && (pointB.first - pointA.first) == 1) {
        // Capturing
        if ((pointB.second - pointA.second == 1 || pointB.second - pointA.second == -1)) {
            // Opponent's piece
            if (std::string("kqrnbp").find(bd[pointB.first][pointB.second]) != std::string::npos) {
                return true;
            }
        }
        
        // Forward move and not capturing
        if (pointB.second == pointA.second && bd[pointB.first][pointB.second] == '.') {
            return true;
        }
    }
    
    // Long move
    // If it's a 2 square move...
    if ((isWhitePiece && (pointB.first - pointA.first) == -2) || (!isWhitePiece && (pointB.first - pointA.first) == 2)) {
        
        // Two blocks ahead are unoccupied and is currently on rank 1 or 6
        if (isWhitePiece && bd[pointA.first - 1][pointA.second] == '.' &&
            bd[pointA.first - 2][pointA.second] == '.' && pointA.first == 6) {
            return true;
        }
        
        if (!isWhitePiece && bd[pointA.first + 1][pointA.second] == '.' &&
            bd[pointA.first + 2][pointA.second] == '.' && pointA.first == 1) {
            return true;
        }
    }
    return false;
    
}



///UNCOMPLETED FUNCTIONS
///
///
///
///
///
///
///TODO: Update notations, e.g. qa2 -> Qa2, user input qa2 or Qa2 should be fine, atCheck() if possible
///

void Chess::makeMove(std::string& pos){
    
    std::vector<PieceMove> moves = isWhiteTurn ? whiteMoves : blackMoves;
    
    // Iterate through pieceMove to get index of pm.note == position
    auto it = std::find_if(moves.begin(), moves.end(), [&](const struct PieceMove& pm) {return pm.note == pos;});
    if (it != moves.end()) {
        
        // Get the move's index
        size_t idx = std::distance(moves.begin(), it);
        
        // Promotion of pawn
        if (moves[idx].pieceT == 'p' && moves[idx].endPoint.first == (isWhiteTurn? 0: 7)) {
            promotion(moves[idx].endPoint);
        }
        
        // Noraml piece move
        else{
            // Point B's piece is point A's piece
            board[moves[idx].endPoint.first][moves[idx].endPoint.second] = board[moves[idx].startPoint.first][moves[idx].startPoint.second];
        }
        
        // Clear point A
        board[moves[idx].startPoint.first][moves[idx].startPoint.second] = '.';
    }
}


void Chess::newGame(){
    
    std::cout << "Game starts!"<< std::endl;
    initializeBoard();
    printBoard();
    // Updates every turn
    while (true) {
        // Calculate all possible moves
        findAllPossibleMoves(); // Return vector of positions
        printAllPossibleMoves();
//        std::cout << pieceMove.size() << std::endl;
        // Get input position
        std::cout << "Enter position: ";
        std::cin >> position;
        
        // Who's turn
        if (isWhiteTurn) {
            // If position is in allMove list, then it's legal move
            if (std::any_of(whiteMoves.begin(), whiteMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;})) {
                // Legal
                makeMove(position);
                std::cout << "Move made" << std::endl;
            }else continue;
        }
        
        // Black's turn
        else{
            // If position is in allMove list, then it's legal move
            if (std::any_of(blackMoves.begin(), blackMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;})) {
                // Legal
                makeMove(position);
                std::cout << "Move made" << std::endl;
            }else continue;
        }
        
//        if (atCheck()) std::cout << "Check" << std::endl;
        
        printBoard();
        isWhiteTurn = !isWhiteTurn;
        
    }
}
