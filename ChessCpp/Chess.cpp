//
//  Chess.cpp
//  ChessCpp
//
//  Created by 吳承翰 on 5/16/25.
//

#include "Chess.hpp"

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
            board[0][4] = 'K';
            board[1][1] = 'r';
            board[2][3] = 'p';
            board[7][7] = 'k';
            board[2][5] = 'Q';
//            board[1][7] = 'R';
            break;
        case 4:
            board[3][7] = 'k';
            board[3][5] = 'p';
            board[3][0] = 'R';
            board[1][6] = 'P';
            board[4][7] = 'r';
            board[4][4] = 'K';
            board[4][5] = 'P';
            board[6][6] = 'p';
            
            break;
        default:
            break;
    }
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
    
    // Add en passant move, determined by whose turn
    if (canEnPassant) {
        for (auto& move: specialMoves)
        {
            if (isWhiteTurn) {
                whiteMoves.push_back(move);
                notes_w.push_back(move.note);
            }else{
                blackMoves.push_back(move);
                notes_b.push_back(move.note);
            }
        }
    }
    
    // Process notes in specialMoves
    {
        std::vector<std::string> tempNote;
        for (auto& move: specialMoves) {
            tempNote.push_back(move.note);
        }
        qfc::processMoves(tempNote);
        for (size_t i = 0; i < tempNote.size(); i++){
            specialMoves[i].note = tempNote[i];
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
    qfc::processMoves(notes_w);
    qfc::processMoves(notes_b);
    for (size_t i = 0; i < notes_w.size(); i++) {
        whiteMoves[i].note = notes_w[i];
    }
    
    for (size_t i = 0; i < notes_b.size(); i++) {
        blackMoves[i].note = notes_b[i];
    }
    
    // Is castling possible?
    castling(); // Check
    if (canCastle[8] && canCastle[0] && canCastle[4]) {
        pm.note = "oo";
        whiteMoves.push_back(pm);
    }
    
    if (canCastle[8] && canCastle[1] && canCastle[5]) {
        pm.note = "ooo";
        whiteMoves.push_back(pm);
    }
    
    if (canCastle[9] && canCastle[2] && canCastle[6]) {
        pm.note = "oo";
        blackMoves.push_back(pm);
    }
    
    if (canCastle[9] && canCastle[3] && canCastle[7]) {
        pm.note = "ooo";
        blackMoves.push_back(pm);
    }
}



// Params: current piece (expressed by loswercase), from point to point & isWhitePiece to determine color globally
// atCheck examines EVERY move on board to see if its next move exposes own-side king, regardless of who's turn it is.
bool Chess::atCheck(char piece, std::pair<int, int>& from, std::pair<int, int>& toPoint){
    
    // Clear for every move
    tempMoves.clear();
    std::pair<int, int> kingPoint;
    
    // tempBoard simulates the board AFTER move is made
    char tempBoard[8][8];
    memcpy(tempBoard, board, sizeof(tempBoard));
    
    piece = (isWhitePiece ? piece: toupper(piece));
    
    // Simulate next move
    tempBoard[from.first][from.second] = '.';
    tempBoard[toPoint.first][toPoint.second] = piece;
    
    // En passant
    if (isSimulatingEnPassant) tempBoard[from.first][toPoint.second] = '.';
    isSimulatingEnPassant = false;
    
    // Find the own-side king's position
    if (piece == 'k' || piece == 'K') kingPoint = toPoint;
    else{
        char kingChar = isWhitePiece ? 'k' : 'K';
        qfc::findKing(kingPoint, tempBoard, kingChar);
    }
    
    // Loop through each block to get opponent's moves
    for (size_t i = 0; i < 8; i++) {
        for (size_t j = 0; j < 8; j++) {
            
            // If to-be-examined piece is white, find black, also skip empty blocks
            if (tempBoard[i][j] == '.') continue;
            if (isWhitePiece && islower(tempBoard[i][j])) continue;
            if (!isWhitePiece && isupper(tempBoard[i][j])) continue;
            
            { // ## Scope starts
                std::pair<int, int> place;

                isWhitePiece = !isWhitePiece;
                place = {i, j};
                
                // If piece is white,
                // &move is a move of a piece, type std::pair, tempBoard[i][j] is opponent's piece
                for (auto& move: pieceLogic(false, tempBoard, tempBoard[i][j], place)){
                    
                    // Add the move
                    tempMoves.push_back(move); // tempMoves store every opponent's move without checkNeeded
                }
                isWhitePiece = !isWhitePiece;
            } // ## Scope ends
        }
    }
    
    // If king point is in one of the moves meaning king is exposed to check
    if (std::find(tempMoves.begin(), tempMoves.end(), kingPoint) != tempMoves.end()) {
        return true;
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
        findAllPossibleMoves();
        printAllPossibleMoves();

        // Track game status
        if (isGameEnded()) {
            return;
        }
                        
        // Get input position
        std::cout << "Enter position: ";
        std::cin >> position;
        
        // If en passant, see if move is in specialMoves vec
        if (std::any_of(specialMoves.begin(), specialMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;}))
        {
            std::cout << "Can en passant!" << std::endl;
            makeMove(position);
            if (canEnPassant == false) {
                
                // Reset
                canEnPassant = false;
                specialMoves.clear();
                printBoard();
                isWhiteTurn = !isWhiteTurn;
                continue;
            }
        }
        
        // Reset
        canEnPassant = false;
        specialMoves.clear();
        
        // Who's turn
        if (isWhiteTurn) {
            // If position is in allMove list, then it's legal move
            if (std::any_of(whiteMoves.begin(), whiteMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;})) {
                // Legal
                makeMove(position);
            }else continue;
        }
        
        // Black's turn
        else{
            // If position is in allMove list, then it's legal move
            if (std::any_of(blackMoves.begin(), blackMoves.end(), [&](const struct PieceMove& pm) {return pm.note == position;})) {
                // Legal
                makeMove(position);
            }else continue;
        }
                        
        printBoard();
        isWhiteTurn = !isWhiteTurn;
        
    }
}
