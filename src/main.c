
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


int validate_pawn_move(int x1, int y1, int x2, int y2) {
    return 0;
}

int validate_knight_move(int x1, int y1, int x2, int y2) { return 0; }

int validate_bishop_move(int x1, int y1, int x2, int y2) {
    if (+(x2 - x1) == +(y2 - y1)) {
        return 1;
    }

    return 0;
}

int validate_rook_move(int x1, int y1, int x2, int y2) { return 0; }
int validate_queen_move(int x1, int y1, int x2, int y2) { return 0; }
int validate_king_move(int x1, int y1, int x2, int y2) { return 0; }

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
    int source_position_file = file_to_board_index(in[0]);
    int source_position_rank = rank_to_board_index(in[1]);
    int target_position_file = file_to_board_index(in[2]);
    int target_position_rank = rank_to_board_index(in[3]);

    square* source = board[source_position_file][source_position_rank];
    // square* target = board[target_position_rank][target_position_file];

    if (source == NULL) {
        return -3;
    }

    piece_move_validator validate = NULL;
    switch ((source->flags & ~LIGHT) & ~DARK) {
        case PAWN: printf("pawn!\n"); break;
        case KNIGHT: printf("knight!\n"); break;
        case BISHOP: printf("bishop!\n"); validate = validate_bishop_move; break;
        case ROOK: printf("rook!\n"); break;
        case QUEEN: printf("queen!\n"); break;
        case KING: printf("king!\n"); break;
    }

    printf("valid? %d\n", validate(source_position_file, source_position_rank, target_position_file, target_position_rank));

    board[target_position_file][target_position_rank] = source;
    board[source_position_file][source_position_rank] = NULL;

    return 0;
}

int main(void) {
    setlocale(LC_CTYPE, "");

    char in[6];
    do {
        // printf("\033[2J\033[;H\n\n\n");
        printf("\n\n");
        draw_board_unicode();

        printf("\n\tmake moves by typing them in coordinate notation ('e2e4', for example)\n\t\033[5m%lc%c\033[0m", 0x2588, 0x8);

        if (fgets(in, 6, stdin) == NULL) {
            return 1;
        }

        switch (handle_move(in)) {
            case -1: printf("first character should be A-H\n"); continue;
            case -2: printf("second character should be 1-8\n"); continue;
            case -3: printf("there isn't a piece on that square!\n"); continue;
        }

    } while (1);

    return 0;
}

