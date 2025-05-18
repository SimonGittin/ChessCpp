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
    customBoard(2);
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
            break;
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

std::string Chess::fullName(char piece){
    switch (piece) {
        case 'k':
            return "king";
        case 'q':
            return "queen";
        case 'b':
            return "bishop";
        case 'n':
            return "knight";
        case 'r':
            return "rook";
        default:
            return "pawn";
            break;
    }
    return "";
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


std::string Chess::pointToPos(std::pair<int, int> point){
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
    pieceMove.clear();
    struct PieceMove pm;
    std::vector<std::string> notes;
    
    
    
    // Using coordination, find pieces from left-top to right-top, ends at right-bottom
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            // A loop a piece
            // White turn, find white pieces
            if (isWhiteTurn && std::islower(board[i][j])) {
                
                // pieceLogic takes the piece type & current place, returns a vector of all possible moves(pair)
                // A loop a move for a piece
                for (auto& move : pieceLogic(board[i][j], {i, j})) // Return vector of pairs of legal moves
                {
                    // pos => pointToPos(coord), ...
                    // coord => {i, j}, pieceLogic(...)[i]
                    // piece => board[i][j]
                    // note = piece + startPos + endPos
                    pm.pieceT = board[i][j];
                    pm.startPoint = {i, j};
                    pm.endPoint = move;
                    pm.startPos = pointToPos({i, j});
                    pm.endPos = pointToPos(move);
                    
                    notes.push_back((pm.pieceT == 'p' ? "" : std::string(1, pm.pieceT)) + pm.startPos + pm.endPos);
                    pieceMove.push_back(pm);

//                    notes.push_back(pieceToPosition(board[i][j], {i, j}, move));
//                    allMovesWithAtoB.push_back({{i, j}, move});
                }
            }
        }
    }
    processMoves(notes);
    for (size_t i = 0; i < pieceMove.size(); i++) {
        pieceMove[i].note = notes[i];
    }
}


void Chess::printAllPossibleMoves(){
        
    for (auto& move: pieceMove){
        std::cout << move.note << "  ";
    }
    
    std::cout << "\n";
}

std::vector<std::pair<int, int>> Chess::pieceLogic(char piece, std::pair<int, int> place){
    std::vector<std::pair<int, int>> allMoveForThisPiece;
    allMoveForThisPiece.clear();
    switch (piece) {
        case 'k':
            //
            break;
        case 'q':
            //
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
                    if (Bishop(place, move)) allMoveForThisPiece.push_back(move);
                }
                
                // LB to RT
                if (place.first - i >= 0 && place.first - i <=7 && place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first - i, place.second + i};
                    if (Bishop(place, move)) allMoveForThisPiece.push_back(move);
                }
            }
            break;
        }
        case 'n':
            //
            break;
        case 'r':
        {
            // Straight line
            std::pair<int, int> move;
            for (int i = -9; i < 9; i++) {
                if (i == 0) continue;
                
                // All move on row
                if (place.first + i >= 0 && place.first + i <=7) {
                    move = {place.first + i, place.second};
                    if (Rook(place, move)) allMoveForThisPiece.push_back(move);
                }
                
                // All move on col
                if (place.second + i >= 0 && place.second + i <=7) {
                    move = {place.first, place.second + i};
                    if(Rook(place, move)) allMoveForThisPiece.push_back(move);
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
            if (isWhiteTurn && (place.first - 1) < 0) {
                return allMoveForThisPiece;
            }
            
            // a0 -> invalid,
            if (!isWhiteTurn && (place.first - 1) > 7) {
                return allMoveForThisPiece;
            }
            
            // a8 -> promotion
            if (isWhiteTurn && (place.first - 1) == 0) {
                // promotion();
                break;
            }
            
            // a8 -> promotion
            if (!isWhiteTurn && (place.first - 1) == 7) {
                // promotion();
                break;
            }
            
            std::pair<int, int> move;
            
            // One square moves
            for (int i = -1; i <= 1; i++) {
                
                // Too left or too right -> invalid
                if (place.second + i < 0 || coordination.second + i > 7) {
                    continue;
                }
                
                move = {isWhiteTurn? place.first - 1:place.first + 1, place.second + i};
                
                if (Pawn(place, move)) {
                    allMoveForThisPiece.push_back(move);
                }
            }
            
            // Long move
            if (isWhiteTurn && place.first == 6) {
                move = {4, place.second};
                if (Pawn(place, move)){
                    allMoveForThisPiece.push_back(move);
                }
            }
            
            if (!isWhiteTurn && coordination.first == 1) {
                move = {3, place.second};
                if (Pawn(place, move)) {
                    allMoveForThisPiece.push_back(move);
                }
            }
            break;
        }
        default:
            break;
        
    } // End of switch
    
    return allMoveForThisPiece; // Return coordination
}

bool Chess::pieceRule(std::pair<int, int> coordination){
    return false;
}

bool Chess::Bishop(std::pair<int, int> &pointA, std::pair<int, int> &pointB){
    // Cannot take own piece
    // Cannot move through pieces
    
    // Cannot take own piece
    if (isWhiteTurn ? islower(board[pointB.first][pointB.second]):isupper(board[pointB.first][pointB.second])) return false;
    
    // Can take enemy piece
    if (isWhiteTurn ? isupper(board[pointB.first][pointB.second]):islower(board[pointB.first][pointB.second])) return true;
    
    // No pieces is occupying the way
    int disY = pointB.first - pointA.first;
    int disX = pointB.second - pointA.second;
    
    std::pair<bool, bool> isNegaDists;
    
    isNegaDists.first = disY < 0 ? true: false;
    isNegaDists.second = disX < 0? true: false;
    
    disX = abs(disX);
    disY = abs(disY);
    
    
    
    for (int i = 1; i < disX; i++) {
        if (board[pointA.first + (isNegaDists.first? -i: i)][pointA.second + (isNegaDists.second? -i: i)] != '.') {
            return false;
        }
    }
    
    
    return true;
}

    
bool Chess::Rook(std::pair<int, int> &pointA, std::pair<int, int> &pointB){
    // Cannot take own piece
    // Cannot move through pieces
    
    // Cannot take own piece
    if (isWhiteTurn ? islower(board[pointB.first][pointB.second]):isupper(board[pointB.first][pointB.second])) return false;
    
    // Can take enemy piece
    if (isWhiteTurn ? isupper(board[pointB.first][pointB.second]):islower(board[pointB.first][pointB.second])) return true;
    
    // No pieces is occupying the way
    int distance;
    bool isSameRow = (pointB.first - pointA.first == 0) ? true:false;
    
    if (pointB.first - pointA.first == 0) distance = pointB.second - pointA.second;
    else distance = pointB.first - pointA.first;

    bool isNegaDist = (distance < 0? true : false);
    distance = abs(distance);
    
    for (int i = 1; i < distance; i++) {
        if (isSameRow) {
            if (board[pointA.first][pointA.second + (!isNegaDist? i : -i)] != '.') return false;
        }else{
            if (board[pointA.first + (!isNegaDist? i : -i)][pointA.second] != '.') return false;
        }
    }
    return true;
}

bool Chess::Pawn(std::pair<int, int>& pointA, std::pair<int, int>& pointB){
    // If pawn is on row 2 or 7, it can launch move with 2 blocks
    // En passant
    
    // One square move for WHITE
    if (isWhiteTurn && (pointB.first - pointA.first) == -1) {
        // Capturing diagonally
        if ((pointB.second - pointA.second == 1 || pointB.second - pointA.second == -1)) {
            // Opponent's piece
            if (std::string("KQRNBP").find(board[pointB.first][pointB.second]) != std::string::npos) {
                return true;
            }
        }
        
        // Forward move and not capturing
        if (pointB.second == pointA.second && board[pointB.first][pointB.second] == '.') {
            return true;
        }
    }
    
    // One square move for BLACK
    if (!isWhiteTurn && (pointB.first - pointA.first) == 1) {
        // Capturing
        if ((pointB.second - pointA.second == 1 || pointB.second - pointA.second == -1)) {
            // Opponent's piece
            if (std::string("kqrnbp").find(board[pointB.first][pointB.second]) != std::string::npos) {
                return true;
            }
        }
        
        // Forward move and not capturing
        if (pointB.second == pointA.second && board[pointB.first][pointB.second] == '.') {
            return true;
        }
    }
    
    // Long move
    // If it's a 2 square move...
    if ((isWhiteTurn && (pointB.first - pointA.first) == -2) || (!isWhiteTurn && (pointB.first - pointA.first) == 2)) {
        
        // Two blocks ahead are unoccupied and is currently on rank 1 or 6
        if (isWhiteTurn && board[pointA.first - 1][pointA.second] == '.' &&
            board[pointA.first - 2][pointA.second] == '.' && pointA.first == 6) {
            return true;
        }
        
        if (!isWhiteTurn && board[pointA.first + 1][pointA.second] == '.' &&
            board[pointA.first + 2][pointA.second] == '.' && pointA.first == 1) {
            return true;
        }
    }
    return false;
    
}

bool Chess::PieceMove(char piece, bool isCapturing){
    switch (piece) {
        case 'k':
            //
            break;
        case 'q':
            //
            break;
        case 'b':
            //
            break;
        case 'n':
            //
            break;
        case 'r':
            //
            break;
            
        default:
            break;
    }
    return false;
}

void Chess::makeMove(std::string pos){
    
    // Iterate through pieceMove to get index of pm.note == position
    auto it = std::find_if(pieceMove.begin(), pieceMove.end(), [&](const struct PieceMove& pm) {return pm.note == pos;});
    if (it != pieceMove.end()) {
        size_t idx = std::distance(pieceMove.begin(), it);
        std::cout << pieceMove[idx].startPoint.second << std::endl;
        // Point B's piece is point A's piece
        board[pieceMove[idx].endPoint.first][pieceMove[idx].endPoint.second] = board[pieceMove[idx].startPoint.first][pieceMove[idx].startPoint.second];
        
        // Clear point A
        board[pieceMove[idx].startPoint.first][pieceMove[idx].startPoint.second] = '.';
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
        
        // If position is in allMove list, then it's legal move
        if (std::any_of(pieceMove.begin(), pieceMove.end(), [&](const struct PieceMove& pm) {return pm.note == position;})) {
            // Legal
            makeMove(position);
        }else{
            // Probably illegal
        }
        
        printBoard();
        
    }
}
