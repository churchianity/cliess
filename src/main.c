
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>


enum square_states {
    PAWN    = 0x1,
    KNIGHT  = 0x2,
    BISHOP  = 0x4,
    ROOK    = 0x8,
    QUEEN   = 0x10,
    KING    = 0x20,
    LIGHT   = 0x40,
    DARK    = 0x80
};

typedef struct {
    uint8_t flags;
} square;

static square *board[8][8] = {
    { /* A */
        &(square) { ROOK | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { ROOK | DARK }
    },
    { /* B */
        &(square) { KNIGHT | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { KNIGHT | DARK }
    },
    { /* C */
        &(square) { BISHOP | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { BISHOP | DARK }
    },
    { /* D */
        &(square) { QUEEN | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { QUEEN | DARK }
    },
    { /* E */
        &(square) { KING | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { KING | DARK }
    },
    { /* F */
        &(square) { BISHOP | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { BISHOP | DARK }
    },
    { /* G */
        &(square) { KNIGHT | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { KNIGHT | DARK }
    },
    { /* H */
        &(square) { ROOK | LIGHT },
        &(square) { PAWN | LIGHT },
        NULL,
        NULL,
        NULL,
        NULL,
        &(square) { PAWN | DARK },
        &(square) { ROOK | DARK }
    }
};

void draw_board_unicode() {
    for (int i = 0; i < 8; i++) {
        printf("\t%c ", '0' + 8 - i);

        for (int j = 0; j < 8; j++) {
            // reset the color settings
            printf("\033[0m");

            square* s = board[j][7 - i];

            // color the background of the squares
            if ((i + j + 1) % 2 == 0) {
                printf("\033[0;100m");
            } else {
                printf("\033[0;107m");
            }

            // fill in the blanks
            if (s == NULL) {
                printf("  ");
                continue;
            }

            // else, print the unicode character for the piece on that square
            switch (s->flags) {
                case PAWN | LIGHT: printf("%lc ", 0x2659); break;
                case ROOK | LIGHT: printf("%lc ", 0x2656); break;
                case KNIGHT | LIGHT: printf("%lc ", 0x2658); break;
                case BISHOP | LIGHT: printf("%lc ", 0x2657); break;
                case QUEEN | LIGHT: printf("%lc ", 0x2655); break;
                case KING | LIGHT: printf("%lc ", 0x2654); break;

                case PAWN | DARK: printf("%lc ", 0x265F); break;
                case ROOK | DARK: printf("%lc ", 0x265C); break;
                case KNIGHT | DARK: printf("%lc ", 0x265E); break;
                case BISHOP | DARK: printf("%lc ", 0x265D); break;
                case QUEEN | DARK: printf("%lc ", 0x265B); break;
                case KING | DARK: printf("%lc ", 0x265A); break;
            }
        }

        printf("\033[0m\n");
    }

    printf("\t  a b c d e f g h\n");
}

void print_square(square* s) {
    printf("square: %d\n", s ? s->flags : 0);
}

int validate_move(int x1, int y1, int x2, int y2) {
    if (board[x2][y2] && ((board[x1][y1]->flags | LIGHT) | DARK) == (((board[x2][y2]->flags | LIGHT) | DARK))) {
        return 0;
    }

    return 1;
}


int validate_pawn_move(int x1, int y1, int x2, int y2) {
    return validate_move(x1, y1, x2, y2);
}

int validate_knight_move(int x1, int y1, int x2, int y2) { return validate_move(x1, y1, x2, y2); }

int validate_bishop_move(int x1, int y1, int x2, int y2) {
    if (+(x2 - x1) == +(y2 - y1)) {
        return 1;
    }

    return validate_move(x1, y1, x2, y2);
}

int validate_rook_move(int x1, int y1, int x2, int y2) { return validate_move(x1, y1, x2, y2); }
int validate_queen_move(int x1, int y1, int x2, int y2) { return validate_move(x1, y1, x2, y2); }
int validate_king_move(int x1, int y1, int x2, int y2) { return validate_move(x1, y1, x2, y2); }

int file_to_board_index(char c) {
    if (c >= 'a') {
        return c - 'a';

    } else {
        return c - 'A';
    }
}

int rank_to_board_index(char c) {
    return c - '0' - 1;
}

typedef int (*piece_move_validator)(int, int, int, int);

int handle_move(char in[6]) {
    int ix1 = file_to_board_index(in[0]);
    int iy1  = rank_to_board_index(in[1]);
    int ix2 = file_to_board_index(in[2]);
    int iy2 = rank_to_board_index(in[3]);

    square* source = board[ix1][iy1];

    if (source == NULL) {
        return -1;
    }

    piece_move_validator validate = NULL;
    switch ((source->flags & ~LIGHT) & ~DARK) {
        case PAWN:      validate = validate_pawn_move; break;
        case KNIGHT:    validate = validate_knight_move; break;
        case BISHOP:    validate = validate_bishop_move; break;
        case ROOK:      validate = validate_rook_move; break;
        case QUEEN:     validate = validate_queen_move; break;
        case KING:      validate = validate_king_move; break;
    }

    if (!validate(ix1, iy1, ix2, iy2)) {
        return -2;
    }

    board[ix2][iy2] = source;
    board[ix1][iy1] = NULL;

    return 0;
}

int validate_coordinate_notation(char in[4]) {
    for (int i = 0; i < 2; i++) {
        switch (in[i * 2]) {
            default: return -1;

            case 'a': case 'A':
            case 'b': case 'B':
            case 'c': case 'C':
            case 'd': case 'D':
            case 'e': case 'E':
            case 'f': case 'F':
            case 'g': case 'G':
            case 'h': case 'H': break;
        }

        switch (in[i * 2 + 1]) {
            default: return -2;

            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': break;
        }
    }

    return 0;
}

int main(void) {
    setlocale(LC_CTYPE, "");

    char in[6];
    do {
        printf("\n\n");
        draw_board_unicode();

        printf("\n\tmake moves by typing them in non-hyphenated coordinate notation ('e2e4', for example)\n\t> \033[5m%lc%c\033[0m", 0x2588, 0x8);

        if (fgets(in, 6, stdin) == NULL) {
            return 1;
        }

        if (validate_coordinate_notation(in) != 0) {
            printf("\tinvalid coordinate notation.\n"); fflush(stdin); continue;
        }

        switch (handle_move(in)) {
            case -1: printf("\tthere isn't a piece on that square!\n"); continue;
            case -2: printf("\tyou have a piece where ur tryna go\n"); continue;
        }

    } while (1);

    return 0;
}

