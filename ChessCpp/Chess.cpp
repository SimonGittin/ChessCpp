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
    customBoard(1);
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

bool Chess::isValidNote(std::string position){
    bool isPawnMove = false;
    
    // King/Queen side castling
    if(std::string("OOO").find(position) != std::string::npos){
        return true;
    }
    
    // First letter of the position
    if(position.length() > 3){
        std::cout << "Invalid input" << "\n";
        return false;
    }
    
    // input notes e.g. e4(pawn) ne4(knight) be4(bishop) re4(rook) qe4(queen) ke4(king)
    if(position.length() == 3 && std::string("nbrqk").find(position[0]) == std::string::npos){
        std::cout << "Invalid move" << "\n";
        return false;
    }
    
    // Pawn move
    if(position.length() == 2){
        isPawnMove = true;
        if(std::string("abcdefgh").find(position[0]) == std::string::npos){
            std::cout << "Invalid pawn move" << "\n";
            return false;
        }
    }else isPawnMove = false;
    
    // Second and third letter of the position
    //
    if(!isPawnMove){
        if(std::string("abcdefgh").find(position[1]) == std::string::npos || std::string("12345678").find(position[2]) == std::string::npos)
        {
            std::cout << "Invalid move" << "\n";
            return false;
        }
    }else{
        if(std::string("12345678").find(position[1]) == std::string::npos){
            std::cout << "Invalid pawn move B check" << "\n";
            return false;
        }
    }
    return true;
}

bool Chess::toNumberAndIsPawnMove(std::string position, std::pair<int, int>& coordination){
        
    bool isPawnMove = ((position.length() == 2) ? true : false);
    
    // Row
    coordination.first = 8 - ((isPawnMove ? position[1] : position[2]) - '0');
    
    // Col
    switch (isPawnMove ? position[0] : position[1]) {
        case 'a':
            coordination.second = 0;
            break;
        case 'b':
            coordination.second = 1;
            break;
        case 'c':
            coordination.second = 2;
            break;
        case 'd':
            coordination.second = 3;
            break;
        case 'e':
            coordination.second = 4;
            break;
        case 'f':
            coordination.second = 5;
            break;
        case 'g':
            coordination.second = 6;
            break;
        case 'h':
            coordination.second = 7;
            break;
        default:
            break;
    }
    
    
    return isPawnMove;
}

std::string Chess::pieceToPosition(char piece, std::pair<int, int> pointA, std::pair<int, int> pointB){
    std::string note;
    
    // Pawn move do not need to specify pe4
    if (piece != 'p' && piece != 'P') note += piece;
    
    // (6, 7) => h2, (2, 0) => a6
    // Queen from a1 to a2 -> Qa1a2
    std::string columns = "abcdefgh";
    for (int i = 0; i < 8; ++i) {
        if (pointA.second == i) {
            note += std::string(1, columns[i]) + std::to_string(8 - pointA.first);
        }
    }
    
    for (int i = 0; i < 8; ++i) {
        if (pointB.second == i) {
            note += std::string(1, columns[i]) + std::to_string(8 - pointB.first);
        }
    }
    return note;
}

bool Chess::isCapturing(std::pair<int, int> &coordination){
    if(board[coordination.first][coordination.second] != '.'){
        return true;
    }
    return false;
}

bool Chess::isLegal(std::pair<int, int>& coordination){
    // White takes piece that is in upper case, vice versa
    if (isWhiteTurn) {
        if (isupper(board[coordination.first][coordination.second])){
            return true;
        };
    }else{if (islower(board[coordination.first][coordination.second])) return true;}
    return false;
}

//void Chess::twoPiecePossible(char piece, std::pair<int, int> origin, std::pair<int, int> destination){
//    // Find all pieces that conflicts
//    std::string note;
//    if (piece != 'p' && piece != 'P') {
//        note += piece;
//    }
//    // Same row conflict -> Qbb2 & Qcb2 / ab3 & cb3 ==> ALSO happens on diff row & col but same destination
//    if (origin.first == destination.first || (origin.first != destination.first && origin.second != destination.second)) {
//        note +=  'h';
//    }
//    // Same col conflict -> Q1b2 & Q2b2
//    if (origin.second == destination.second && ) {
//        <#statements#>
//    }
//    // Same row and same column (more than 2 queens) -> Qb1b2 & Qb3b2 & Qa1b2
//}

std::vector<std::string>& Chess::findAllPossibleMoves(){
    // Clean every time it's called
    allPossibleMoves.clear();
    
    // Using coordination, find pieces from left-top to right-top, ends at right-bottom
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            
            // White turn, find white pieces
            if (isWhiteTurn && std::islower(board[i][j])) {
                
                // pieceLogic takes the piece type & current place, returns a vector of all possible moves(pair)
                for (auto& move : pieceLogic(board[i][j], {i, j})) // Return vector of pairs of legal moves
                {
                    if (std::find(allPossibleMoves.begin(), allPossibleMoves.end(), pieceToPosition(board[i][j], {i, j}, move)) != allPossibleMoves.end()) {
                    }
                    allPossibleMoves.push_back(pieceToPosition(board[i][j], {i, j}, move));
                }
                    
            }
        }
    }
    printAllPossibleMoves();
    std::vector<std::string> tempContainer;
    
    std::cout << "size: " << allPossibleMoves.size() << std::endl;
    
    // Resolve for confliction
    for (int i = 0; i < allPossibleMoves.size(); i++){
        // Pawn move
        if (allPossibleMoves[i].length() == 4) {
            for (std::string& note : allPossibleMoves) {
                
                // Skip self
                if (note == allPossibleMoves[i]){
                    std::string combined;
                    combined.push_back(note[2]);
                    combined.push_back(note[3]);
                    tempContainer.push_back(combined);
                    continue;
                }
                
                // Destination is equal, a1b2, c1b2 => ab2, cb2
                if (allPossibleMoves[i][2] == note[2] && allPossibleMoves[i][3] == note[3]) {
                    std::string combined;
                    combined.push_back(allPossibleMoves[i][0]);
                    combined.push_back(note[2]);
                    combined.push_back(note[3]);
                    tempContainer.push_back(combined);
                    // = allPossibleMoves[i][0] + note[2] + note[3];
                }
                
                // Destination is not equal, a1b2, c1c2 => b2, c2
                else{
                    std::string combined;
                    combined.push_back(note[2]);
                    combined.push_back(note[3]);
                    tempContainer.push_back(combined);
                }
            }
        }
        allPossibleMoves.clear();
        allPossibleMoves = tempContainer;
    }
    return allPossibleMoves;
}


void Chess::printAllPossibleMoves(){
    for (auto& move : allPossibleMoves){
        std::cout << move << ", ";
    }
    
    std::cout << "\n";
}

std::vector<std::pair<int, int>> Chess::pieceLogic(char piece, std::pair<int, int> place){
    std::vector<std::pair<int, int>> allMoveForThisPiece;
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


void Chess::newGame(){
    
    std::cout << "Game starts!"<< std::endl;
    initializeBoard();
    printBoard();
    // Updates every turn
    while (true) {
        // Calculate all possible moves
        findAllPossibleMoves(); // Return vector of positions
        printAllPossibleMoves();
        
        // Get input position
        std::cout << "Enter position: ";
        std::cin >> position;
        
        // If position is in allMove list, then it's legal move
        if (std::find(allPossibleMoves.begin(), allPossibleMoves.end(), position) != allPossibleMoves.end()) {
            // Legal
            std::cout << "it's legal move\n";
        }
        
        // Check if input is valid -> moves is inside board
//        if(!isValidNote(position)) continue;
        
        // Convert notes into numerical expression and return if it's pawn move
//        bool isPawnMove = toNumberAndIsPawnMove(position, coordination);
        
//        std::cout << coordination.first << coordination.second << std::endl;
        
        // Make move since it's been checked as a legal move
//        makeMove();
        
        // If there is capturing, check legality
//        if((isCapturing(coordination))) {
//            if(!isLegal(coordination)){
//                std::cout << "You cannot take your own piece\n\n";
//                continue;
//            }
//            
//            // Capturing move
//            if(!(isPawnMove ? Pawn(true, coordination) : PieceMove(position[0], true))) {
//                std::cout << "Illegal move \n";
//                continue;
//            }
//        }else{
//            if(!(isPawnMove ? Pawn(false, coordination) : PieceMove(position[0], false))){
//                std::cout << "Illegal move \n";
//                continue;
//            }
//
//        }
        
        printBoard();
        
    }
}
