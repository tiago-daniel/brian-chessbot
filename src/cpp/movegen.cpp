//
// Created by metta on 10/6/24.
//

#include "h/movegen.h"

#include <set>

#include "h/bitboard.h"

moveGen::moveGen() = default;

array<Move, 256> moveGen::pawnMove(Square_index square, Color color, Bitboard allies, Bitboard enemies, Square_index passant) {
    array<Move, 256> squares = {};
    uint8_t i = 0;
    Bitboard board = Bitboard(allies.getBitboard() | enemies.getBitboard());
    Move_type type = NORMAL;

    if (color == c_white and not (board.getBitboard() & Bit(square+8))) {
        if(square<(64-8) and square>=(64-16)) {
            type = PROMOTION;
        }
        // Single move
        squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+8),               // destination
                type                                                // type
        });

        // Double move
        if (square<16 and square>=8 and not (board.getBitboard() & Bit(square+16))) {
            squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+16),              // destination
                NORMAL                                              // type
            });
        }

        // Capture
        if(enemies.getBitboard() & Bit(square+7)) {
            squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+7),               // destination
                type                                                // type
            });
        }
        if(enemies.getBitboard() & Bit(square+9)) {
            squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+9),               // destination
                type                                                // type
            });
        }

        // En passant
        if (square == passant+1 or square == passant-1) {
            squares[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(passant+8),              // destination
                    EN_PASSANT                                          // type
            });
        }
        else if (not (board.getBitboard() & Bit(square-8))) {
            // Promotion
            if(square<(64-56) and square>=(64-48)) {
                type = PROMOTION;
            }

            // Single move
            squares[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-8),               // destination
                    type                                                // type
                });

            // Double move
            if(square<(64-8) and square>=(64-16) and not (board.getBitboard() & Bit(square-16))) {
                squares[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-16),              // destination
                    NORMAL                                              // type
                });
            }

            // Capture
            if(enemies.getBitboard() & Bit(square-7)) {
                squares[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-7),               // destination
                    type                                                // type
                });
            }
            if(enemies.getBitboard() & Bit(square-9)) {
                squares[i++] = (Move{
                    square,                                             // origin
                    static_cast<Square_index >(square-9),               // destination
                    type                                                // type
                });
            }

            // En passant
            if (square == passant+1 or square == passant-1) {
                squares[i++] = (Move{
                        square,                                             // origin
                        static_cast<Square_index >(passant-8),              // destination
                        EN_PASSANT                                          // type
                });
            }
        }

        return squares;
    }
}

array<Move, 256> moveGen::knightMove(Square_index square, Bitboard allies){
    vector<pair<int, int>> directions = {
        {-2, -1}, {-2, 1},  // Two up, one left/right
        {-1, -2}, {-1, 2},  // One up, two left/right
        {1, -2}, {1, 2},    // One down, two left/right
        {2, -1}, {2, 1}     // Two down, one left/right
    };
    return pieceMove(square, directions, false, allies);
}

array<Move, 256> moveGen::bishopMove(Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
    };

    return pieceMove(square, directions, true, allies, enemies);
}

array<Move, 256> moveGen::rookMove(Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return pieceMove(square, directions, true, allies, enemies);
}

array<Move, 256> moveGen::queenMove(Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return pieceMove(square, directions, true, allies, enemies);
}

array<Move, 256> moveGen::kingMove(Square_index square, Bitboard allies, Bitboard enemies) {
    vector<pair<int, int>> directions = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {0, -1},
        {-1, 0},
        {1, 0},
        {0, 1},
    };

    return pieceMove(square, directions, false, allies);
}

array<Move, 256> moveGen::CastleMove(Square_index square, uint8_t castlingRights) {
    array<Move, 256> squares = {};
    uint8_t i = 0;
    // Queen side White Castle
    if(castlingRights & 0b0010) {
        squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+2),               // destination
                CASTLE                                              // type
        });
    }
    //King side White Castle
    if(castlingRights & 0b0001) {
        squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square-2),               // destination
                CASTLE                                              // type
        });
    }

    // Queen side Black Castle
    if(castlingRights & 0b1000) {
        squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square-2),               // destination
                CASTLE                                              // type
        });
    }

    //King side Black Castle
    if(castlingRights & 0b0100) {
        squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index >(square+2),               // destination
                CASTLE                                              // type
        });
    }
    return squares;
}


array<Move, 256> moveGen::pieceMove(Square_index square, vector<pair<int, int>> directions,
    bool slide, Bitboard allies, Bitboard enemies) {
    array<Move, 256> squares ={};
    uint8_t i = 0;

    // Get the current rank and file
    int rank = square / 8; // Rows (0 to 7)
    int file = square % 8; // Columns (0 to 7)

    for (const auto& direction : directions) {
        int dRank = direction.first;
        int dFile = direction.second;
        int newRank = rank;
        int newFile = file;

        do {
            newRank += dRank;
            newFile += dFile;

            if (newRank < 0 || newRank >= 8 || newFile < 0 || newFile >= 8) {
                break;
            }
            if ( allies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
            squares[i++] = (Move{
                square,                                             // origin
                static_cast<Square_index>(newRank * 8 + newFile),     // destination
                NORMAL                                              // type
            });
            if (enemies.getBitboard() & Bit((newRank * 8 + newFile))) {
                break;
            }
        } while (slide);
    }
    return squares;
}