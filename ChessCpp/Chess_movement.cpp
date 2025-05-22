//
//  Chess_movement.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/22/25.
//
#pragma once
#include "Chess.hpp"


// Move logic functions: diagonal, straight, knight, king, pawn
// Functions for pieceLogic to use

void Chess::diagonals
 (bool checkNeeded, char &piece, char (&bd)[8][8], std::pair<int, int>& place, std::vector<std::pair<int, int>>& vecToEdit)
{
    std::pair<int, int> move;
    for (int i = -9; i < 9; i++) {
        if (i == 0) continue;
        
        // LT to RB
        if (place.first + i >= 0 && place.first + i <=7 && place.second + i >= 0 && place.second + i <=7) {
            move = {place.first + i, place.second + i};
            if (bishopCheck(bd, place, move)){
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
        
        // LB to RT
        if (place.first - i >= 0 && place.first - i <=7 && place.second + i >= 0 && place.second + i <=7) {
            move = {place.first - i, place.second + i};
            if (bishopCheck(bd, place, move)){
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
    }
}

bool Chess::bishopCheck(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){
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
    if (isWhitePiece ? islower(bd[pointB.first][pointB.second]):isupper(bd[pointB.first][pointB.second])) return false;

    
    // Can take enemy piece
    if (isWhitePiece ? isupper(bd[pointB.first][pointB.second]):islower(bd[pointB.first][pointB.second])) return true;
    return true;
}

void Chess::straights
 (bool checkNeeded, char &piece, char (&bd)[8][8], std::pair<int, int> &place, std::vector<std::pair<int, int>> &vecToEdit)
{
    std::pair<int, int> move;
    for (int i = -9; i < 9; i++) {
        if (i == 0) continue;
        
        // All move on row
        if (place.first + i >= 0 && place.first + i <=7) {
            move = {place.first + i, place.second};
            if (rookCheck(bd, place, move)){
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
        
        // All move on col
        if (place.second + i >= 0 && place.second + i <=7) {
            move = {place.first, place.second + i};
            if(rookCheck(bd, place, move)) {
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
    }
}

bool Chess::rookCheck(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){

    
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

void Chess::knight
 (bool checkNeeded, char &piece, char (&bd)[8][8], std::pair<int, int> &place, std::vector<std::pair<int, int>> &vecToEdit)
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
            if (pieceCheck(bd, place, move)){
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
    }
}

bool Chess::pieceCheck(char (&bd)[8][8], std::pair<int, int> &pointA, std::pair<int, int> &pointB){
    // Cannot take own piece
    if (isWhitePiece ? islower(bd[pointB.first][pointB.second]):isupper(bd[pointB.first][pointB.second])) return false;
    
    // Can take enemy piece
    if (isWhitePiece ? isupper(bd[pointB.first][pointB.second]):islower(bd[pointB.first][pointB.second])) return true;
    
    return true;
}


void Chess::king
 (bool checkNeeded, char &piece, char (&bd)[8][8], std::pair<int, int> &place, std::vector<std::pair<int, int>> &vecToEdit)
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
            if (pieceCheck(bd, place, move)){
                vecToEdit.push_back(move);
                if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
            }
        }
    }
}

void Chess::pawn
 (bool checkNeeded, char &piece, char (&bd)[8][8], std::pair<int, int> &place, std::vector<std::pair<int, int>> &vecToEdit)
{
    // diagonal 1 move left, right, a move forward and 2 moves forward (4)
    //
    // If forward one move is invalid, then no move is possible, return function
    //
    // a9 -> invalid,
    if (isWhitePiece && (place.first - 1) < 0) {
        return;
    }
    
    // a0 -> invalid,
    if (!isWhitePiece && (place.first + 1) > 7) {
        return;
    }

    std::pair<int, int> move;

    // One square moves
    for (int i = -1; i <= 1; i++) {
        
        // Too left or too right -> invalid
        if (place.second + i < 0 || place.second + i > 7) {
            continue;
        }
        
        move = {isWhitePiece? place.first - 1:place.first + 1, place.second + i};
        
        if (pawnCheck(bd, place, move)) {
            vecToEdit.push_back(move);
            if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
        }
    }
    
    // Long move
    if (isWhitePiece && place.first == 6) {
        move = {4, place.second};
        if (pawnCheck(bd, place, move)){
            vecToEdit.push_back(move);
            if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
        }

    }
    
    if (!isWhitePiece && place.first == 1) {
        move = {3, place.second};
        if (pawnCheck(bd, place, move)) {
            vecToEdit.push_back(move);
            if (checkNeeded){ if (atCheck(piece, place, move)) vecToEdit.pop_back();};
        }
    }
}

bool Chess::pawnCheck(char (&bd)[8][8], std::pair<int, int>& pointA, std::pair<int, int>& pointB){
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


// Handle promotions
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


/// Uncompleted
///
std::vector<std::pair<int, int>> Chess::pieceLogic(bool checkNeeded, char (&bd)[8][8], char piece, std::pair<int, int>& place){
    std::vector<std::pair<int, int>> allMoveForThisPiece;
    allMoveForThisPiece.clear();
    
    piece = tolower(piece);
    switch (piece) {
        case 'k':
        {
            king(checkNeeded, piece, bd, place, allMoveForThisPiece);
        }break;
        case 'q':
            
            diagonals(checkNeeded, piece, bd, place, allMoveForThisPiece);
            straights(checkNeeded, piece, bd, place, allMoveForThisPiece);
            break;
        case 'b':
            
            diagonals(checkNeeded, piece, bd, place, allMoveForThisPiece);
            break;
        case 'n':
        {
            knight(checkNeeded, piece, bd, place, allMoveForThisPiece);
            break;

        }
        case 'r':

            // Straight line
            straights(checkNeeded, piece, bd, place, allMoveForThisPiece);
            break;
        case 'p':
        {
            pawn(checkNeeded, piece, bd, place, allMoveForThisPiece);
            break;
        }
        default:
            break;
        
    } // End of switch
    
    return allMoveForThisPiece; // Return coordination
}


// Params: board, piece point that is going to en passant and going to get en passanted
bool Chess::EnPassant(char (&bd)[8][8], std::pair<int, int> &toEP, std::pair<int, int> getEP){
    if (isWhiteTurn) {
        std::cout << "Black pawn to EP is at " << pointToPos(toEP) << std::endl;
        std::cout << "White pawn get EP is at " << pointToPos(getEP) << std::endl;
        std::cout << canEnPassant << std::endl;
        
        

    }
    return false;
}
