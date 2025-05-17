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

std::string Chess::toPosition(std::pair<int, int>& coordination){
    
    // (6, 7) => h2, (2, 0) => a6
    switch (coordination.second) {
        case 0:
            return "a" + std::to_string(8 - coordination.first);
        case 1:
            return "b" + std::to_string(8 - coordination.first);
        case 2:
            return "c" + std::to_string(8 - coordination.first);
        case 3:
            return "d" + std::to_string(8 - coordination.first);
        case 4:
            return "e" + std::to_string(8 - coordination.first);
        case 5:
            return "f" + std::to_string(8 - coordination.first);
        case 6:
            return "g" + std::to_string(8 - coordination.first);
        case 7:
            return "h" + std::to_string(8 - coordination.first);
        default:
            return "";
    }
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

void Chess::twoPiecePossible(char piece, std::vector<std::pair<int, int>> possibleMoves){
    std::string position; // User decides the correct piece
    std::string options; // Concat of options
    std::pair<int, int> chosenCoordination;
    for (auto& move : possibleMoves){
        options.append(toPosition(move));
        options += ", ";
    }
    
    std::cout << "Which " << fullName(piece) << " are you inferring to? " << options << std::endl ;
    while (true) {
        std::cin >> position;
        
        // Make move
        toNumberAndIsPawnMove(position, chosenCoordination);
        for (auto& move : possibleMoves){
            if (chosenCoordination == move){
                board[chosenCoordination.first][chosenCoordination.second] = '.';
                return;
            }
        }
        
        std::cout << position << " is not one of the options, choose again: " << options << std::endl;
    }
}

std::vector<std::string>& Chess::findAllPossibleMoves(){
    // Using coordination, find pieces from left-top to right-top, ends at right-bottom
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (isWhiteTurn && std::islower(board[i][j])) {
                for (auto& move : pieceLogic(board[i][j], {i, j})) // Return vector of pairs
                {
                    if (pieceRule(move)) allPossibleMoves.push_back(toPosition(move));
                }
                    
            }
            
            if (!isWhiteTurn && std::isupper(board[i][j])) {
                <#statements#>
            }

        }
    }
    
    
    
    return allPossibleMoves;
}

std::vector<std::pair<int, int>> Chess::pieceLogic(char piece, std::pair<int, int> coordination){
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
            // diagonal 1 move left, right, a move forward and 2 moves forward (4)
            //
            // If forward one move is invalid, then no move is possible, return function
            //
            // a9 -> invalid,
            if (isWhiteTurn && (coordination.first - 1) < 0) {
                return allMoveForThisPiece;
            }
            
            // a0 -> invalid,
            if (!isWhiteTurn && (coordination.first - 1) > 7) {
                return allMoveForThisPiece;
            }
            
            // a8 -> promotion
            if (isWhiteTurn && (coordination.first - 1) == 0) {
                // promotion();
                break;
            }
            
            // a8 -> promotion
            if (!isWhiteTurn && (coordination.first - 1) == 7) {
                // promotion();
                break;
            }
            
            // One square moves
            for (int i = -1; i <= 1; i++) {
                // left of a1 or i1 -> invalid
                if (coordination.second + i < 0 || coordination.second + i > 7) {
                    continue;
                }
                allMoveForThisPiece.push_back({isWhiteTurn? coordination.first - 1:
                    coordination.first + 1, coordination.second + i});
            }
            
            // Long move
            if (isWhiteTurn && coordination.first == 6) {
                allMoveForThisPiece.push_back({4, coordination.second});
            }
            
            if (!isWhiteTurn && coordination.first == 1) {
                allMoveForThisPiece.push_back({3, coordination.second});
            }
            break;
        default:
            break;
    }
    
    return allMoveForThisPiece; // Return coordination
}

bool Chess::pieceRule(std::pair<int, int> coordination){
    
}
    

bool Chess::Pawn(bool isCapturing, std::pair<int, int>& coordination){
    // Diagonal move when capturing
    // Cannot move back
    // If pawn is on row 2 or 7, it can launch move with 2 blocks
    // En passant
    if(isCapturing){
        std::vector<std::pair<int, int>> possibleMoves;
        // En passant
        //
        
        // Capturing
        // Check double directions
        for (int i = -1; i <= 1; i+=2) {
            
            // (5, 2), (5, 4) are possible to go to (4, 3) for white
            if (isWhiteTurn && board[coordination.first + 1][coordination.second + i] == 'p'){
                possibleMoves.push_back({coordination.first + 1, coordination.second + i});
            }
            
            // (5, 2), (5, 4) are possible to go to (6, 3) for black
            if (!isWhiteTurn && board[coordination.first - 1][coordination.second + i] == 'P'){
                possibleMoves.push_back({coordination.first - 1, coordination.second + i});
            }
        }
        
        if (possibleMoves.size() > 1) {
            twoPiecePossible((isWhiteTurn ? 'p':'P'), possibleMoves);
            board[coordination.first][coordination.second] = isWhiteTurn ? 'p' : 'P'; // Fill new block
            return true;
        }else if (possibleMoves.size() == 1){
            // Make the move
            board[coordination.first][coordination.second] = isWhiteTurn ? 'p' : 'P'; // Fill new block
            board[possibleMoves[0].first][possibleMoves[0].second] = '.'; // Get rid of block
            return true;
        }else return false;
    }else{ // Not capturing
        
        // When white's turn, a move forward is (7, 2) to (6, 2), vice versa
        if(isWhiteTurn && board[coordination.first + 1][coordination.second] == 'p'){
            board[coordination.first][coordination.second] = 'p'; // Fill new block
            board[coordination.first + 1][coordination.second] = '.';
            return true;
        }
        
        if(!isWhiteTurn && board[coordination.first - 1][coordination.second] == 'P'){
            board[coordination.first][coordination.second] = 'P'; // Fill new block
            board[coordination.first - 1][coordination.second] = '.';
            return true;
        }
        
        // Long move
//        if (isWhiteTurn && board[coordination.first + 1][coordination.second] != '.')
        if(isWhiteTurn && coordination.first == 4 && board[coordination.first + 2][coordination.second] == 'p'){
            if (board[coordination.first + 1][coordination.second] != '.') return false; // No piece blocks way to long move
            board[coordination.first][coordination.second] = 'p'; // Fill new block
            board[coordination.first + 2][coordination.second] = '.';
            return true;
        }
        if(!isWhiteTurn && coordination.first == 3 && board[coordination.first - 2][coordination.second] == 'P'){
            if (board[coordination.first - 1][coordination.second] != '.') return false; // No piece blocks way to long move
            board[coordination.first][coordination.second] = 'P'; // Fill new block
            board[coordination.first - 2][coordination.second] = '.';
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
        
        // Get input position
        std::cout << "Enter position: ";
        std::cin >> position;
        
        // Check if input is valid -> moves is inside board
        if(!isValidNote(position)) continue;
        
        // Convert notes into numerical expression and return if it's pawn move
        bool isPawnMove = toNumberAndIsPawnMove(position, coordination);
        
//        std::cout << coordination.first << coordination.second << std::endl;
        
        // If there is capturing, check legality
        if((isCapturing(coordination))) {
            if(!isLegal(coordination)){
                std::cout << "You cannot take your own piece\n\n";
                continue;
            }
            
            // Capturing move
            if(!(isPawnMove ? Pawn(true, coordination) : PieceMove(position[0], true))) {
                std::cout << "Illegal move \n";
                continue;
            }
        }else{
            if(!(isPawnMove ? Pawn(false, coordination) : PieceMove(position[0], false))){
                std::cout << "Illegal move \n";
                continue;
            }

        }
        
        printBoard();
        
    }
}
