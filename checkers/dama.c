#include "dama.h"

void init_piece(Piece * piece, _Bool black) {
    piece->dama = 0;
    piece->black = black;
}

void init_player(Player * player, _Bool _black) {
    for (int i = 0; i < 20; i++) {
        init_piece(&player->pieces[i], _black);
    }
    player->black = _black;
}

void init_board(Board * board, Player * white_player, Player * black_player) {

    int w_count = 0;
    int b_count = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {

            if (j == 4 || j == 5) board->body[i][j] = NULL; else {

                if (i%2 == j%2) {

                    if (j < 4) {
                        board->body[i][j] = &white_player->pieces[w_count];
                        w_count++;
                    } else {
                        board->body[i][j] = &black_player->pieces[b_count];
                        b_count++;
                    }

                } else board->body[i][j] = NULL;
            }
        }
    }
}

void print_board(Board * board) {

    int white = 0;
    int black = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (board->body[i][j] != NULL) {
                if (board->body[i][j]->black) black++; else {
                    white++;
                }
            }
        }
    }

    if (white + black < 40) {
        printf("\n-----------------------------------------------------------\n");
        printf("Player 1 'O/θ' -> ");
        for (int i = 0; i < 20-black; i++) {
            printf("X ");
        }
        printf("\nPlayer 2 'X/Ẋ' -> ");
        for (int i = 0; i < 20-white; i++) {
            printf("O ");
        }
        printf("\n-----------------------------------------------------------");
    }

    printf("\n    _________________________________________\n");
    for (int i = 9; i > -1; i--) {
        if (i == 9) printf(" %d |", i+1); else {
            printf("  %d |", i+1);
        }
        for (int j = 0; j < 10; j++) {
            if (i%2 == j%2) {
                if (board->body[j][i] == NULL) printf("___|"); else {
                    if (!board->body[j][i]->dama) {
                        if (board->body[j][i]->black) printf("_X_|"); else {
                            printf("_O_|");
                        }
                    } else {
                        if (board->body[j][i]->black) printf("_Ẋ_|"); else {
                            printf("_θ_|");
                        }
                    }
                }
            } else printf("___|");
        }
        printf(" \n");
    }
    printf("      A   B   C   D   E   F   G   H   I   L  ");
}

int letter_to_number(char letter) {
    if (letter == 'a' || letter == 'A') return 0;
    if (letter == 'b' || letter == 'B') return 1;
    if (letter == 'c' || letter == 'C') return 2;
    if (letter == 'd' || letter == 'D') return 3;
    if (letter == 'e' || letter == 'E') return 4;
    if (letter == 'f' || letter == 'F') return 5;
    if (letter == 'g' || letter == 'G') return 6;
    if (letter == 'h' || letter == 'H') return 7;
    if (letter == 'i' || letter == 'I') return 8;
    if (letter == 'l' || letter == 'L') return 9;
    return -1;
}

char number_to_letter(int number) {
    if (number == 0 ) return 'a';
    if (number == 1 ) return 'b';
    if (number == 2 ) return 'c';
    if (number == 3 ) return 'd';
    if (number == 4 ) return 'e';
    if (number == 5 ) return 'f';
    if (number == 6 ) return 'g';
    if (number == 7 ) return 'h';
    if (number == 8 ) return 'i';
    if (number == 9 ) return 'l';
    return 'z';
}

int available_moves(Board * board, int l, int n, _Bool black_player) {
    if (l < 0 || l > 9) return -1; // lettera non corretta
    if (n < 0 || n > 9) return -2; // numero non corretto
    if (n%2 != l%2) return -3; // spot del board non utilizzabile
    if (black_player != board->body[l][n]->black) return -4; // spot ha una pedina dell'avversario

    int _return = 0;
    // lato destro
    if (l == 9) _return = 0; else { // se a bordo
        if (l == 8) {
            if (board->body[l][n]->dama) {
                if (board->body[9][n+1] == NULL || board->body[9][n-1] == NULL) _return = 1;
            } else {
                if (black_player) {
                    if (board->body[9][n-1] == NULL) _return = 1;
                } else {if (board->body[9][n+1] == NULL) _return = 1;}
            }
        } else {
            if (black_player || board->body[l][n]->dama) { // se giocatere nero
                if (n>0) {
                    if (board->body[l+1][n-1]==NULL) _return = 1; else { // se posto libero
                        if (board->body[l+1][n-1]->black == board->body[l][n]->black) _return = 0; else {  // se posto occoputa da una tua pedina
                            if (n>1 && board->body[l+2][n-2]==NULL) { // se si puo mangiare
                                if (!board->body[l][n]->dama) {
                                    if (!board->body[l+1][n-1]->dama) _return = 2;
                                } else _return = 2;
                            }
                        }
                    }
                }
            }

            if (!black_player || board->body[l][n]->dama){ // se giocatere bianco o dama
                if (n<9) {
                    if (board->body[l+1][n+1]==NULL) {
                        if (_return < 1) _return = 1;
                    } else { // se posto libero
                        if (board->body[l+1][n+1]->black != board->body[l][n]->black) {  // se posto occoputa da una tua pedina
                            if (n<8 && board->body[l+2][n+2]==NULL) { // se si puo mangiare
                                if (!board->body[l][n]->dama) {
                                    if (!board->body[l+1][n+1]->dama) _return = 2;
                                } else _return = 2;
                            }
                        }
                    }
                }

            }
        }
    }

    int return_ = 0;

    // lasto sinistro
    if (l == 0) return_ = 0; else { // se a bordo
        if (l == 1) {
            if (board->body[l][n]->dama) {
                if (board->body[0][n+1] == NULL || board->body[0][n-1] == NULL) _return = 1;
            } else {
                if (black_player) {
                    if (board->body[0][n-1] == NULL) _return = 1;
                } else {if (board->body[0][n+1] == NULL) _return = 1;}
            }
        } else {  // se a 1 prima di bordo
            if (black_player || board->body[l][n]->dama) { // se giocatere nero o dama
                if (n>0) {
                    if (board->body[l-1][n-1]== NULL) return_ = 1; else { // se posto libero
                        if (board->body[l-1][n-1]->black == board->body[l][n]->black) return_= 0; else {  // se posto occoputa da una tua pedina
                            if (n>1 && board->body[l-2][n-2]== NULL) { // se si puo mangiare
                                if (!board->body[l][n]->dama) {
                                    if (!board->body[l-1][n-1]->dama) return_ = 2;
                                } else return_ = 2;
                            }
                        }
                    }
                }
            }
            if (!black_player || board->body[l][n]->dama){ // se giocatere bianco o dama
                if (n<9) {
                    if (board->body[l-1][n+1]==NULL) {
                        if (return_< 1)return_ = 1;
                    }  else { // se posto libero
                        if (board->body[l-1][n+1]->black != board->body[l][n]->black) {  // se posto occoputa da una tua pedina
                            if (n<8 && board->body[l-2][n+2]==NULL) { // se si puo mangiare
                                if (!board->body[l][n]->dama) {
                                    if (!board->body[l-1][n+1]->dama) return_ = 2;
                                } else return_ = 2;
                            }
                        }
                    }
                }
            }
        }
    }

    if (_return > return_) return _return; else return return_;
}

void move_piece(Board *board, int l, int n, _Bool bot, _Bool fast)  {
    char answer[20];
    char answer2[20];
    char right[] = "right";
    char left[] = "left";
    char up[] = "up";
    char down[] = "down";

    int err = 0;
    int rn = 0;

    _Bool dama = board->body[l][n]->dama;
    _Bool up_right = 0;
    _Bool up_left = 0;
    _Bool down_right = 0;
    _Bool down_left = 0;

    if (board->body[l][n]->black || board->body[l][n]->dama) {
        if (n>0) {
            // right side
            if (l<9) {
                if (board->body[l+1][n-1] == NULL) down_right = 1;
            }

            // left side
            if (l>0) {
                if (board->body[l-1][n-1] == NULL) down_left = 1;
            }
        }
    }

    if (!board->body[l][n]->black || board->body[l][n]->dama) {
        if (n<9) {
            // right side
            if (l<9) {
                if (board->body[l+1][n+1] == NULL) up_right = 1;
            }

            // left side
            if (l>0) {
                if (board->body[l-1][n+1] == NULL) up_left = 1;
            }
        }
    }

    if (up_right + up_left + down_right + down_left == 0) {
        perror("\nERROR, piece selected can not make a move.\n");
        return;
    }

    if (n == 8) {
        if (!dama && !board->body[l][n]->black) board->body[l][n]->dama = 1;
    }

    if (n == 1) {
        if (!dama && board->body[l][n]->black) board->body[l][n]->dama = 1;
    }

    if (up_right + up_left + down_right + down_left > 1) {

            do {
                if (dama) {
                    if (bot) {
                        rn = rand() % 4;
                        do {
                            if (rn == 0 && up_right) {
                                up_left = 0;
                                down_right = 0;
                                down_left = 0;
                            }
                            if (rn == 1 && down_right) {
                                up_left = 0;
                                up_right = 0;
                                down_left = 0;
                            }
                            if (rn == 2 && down_left) {
                                up_left = 0;
                                up_right = 0;
                                down_right = 0;
                            }
                            if (rn == 3 && up_left) {
                                up_right = 0;
                                down_right = 0;
                                down_left = 0;
                            }
                            rn = (rn + 1) % 4;
                        } while (up_right + up_left + down_right + down_left > 1);
                    } else {
                        if (err) {
                        printf("\nPlease only type: ");
                        if ( up_left ) printf("'up left' ");
                        if ( up_right ) printf("'up right' ");
                        if ( down_left ) printf("'down left' ");
                        if ( down_right ) printf("'down right' ");
                        printf(".\nTry again:\n");
                    }
                    err = 0;
                    printf("\nYou can move to: ");
                    if ( up_left ) printf("'up left' ");
                    if ( up_right ) printf("'up right' ");
                    if ( down_left ) printf("'down left' ");
                    if ( down_right ) printf("'down right' ");
                    printf("\nWhere do you wanna go? ");
                    scanf("%s %s", answer, answer2);
                        while ((getchar()) != '\n');
                    if (strcmp(answer, up) && strcmp(answer, down) && strcmp(answer2, right) && strcmp(answer2, left)) {
                        err = 1;
                    } else {
                        if (strcmp(answer, up) && strcmp(answer, down)) err=1; else {
                            if (!strcmp(answer, up)) {
                                if (strcmp(answer2, right) && strcmp(answer2, left)) err = 1; else {
                                    if (!strcmp(answer2, right)) {
                                        up_left = 0;
                                        down_right = 0;
                                        down_left = 0;
                                    }
                                    if (!strcmp(answer2, left)){
                                        up_right = 0;
                                        down_right = 0;
                                        down_left = 0;
                                    }
                                    }

                            } else {
                                if (strcmp(answer2, right) && strcmp(answer2, left)) err = 1; else {
                                    if (!strcmp(answer2, right)) {
                                        up_left = 0;
                                        up_right = 0;
                                        down_left = 0;
                                    }
                                    if (!strcmp(answer2, left)) {
                                        up_right = 0;
                                        down_right = 0;
                                        up_left = 0;
                                    }
                                }
                            }
                        }
                    }
                    }

                } else {
                    err = 0;
                    if (bot) {
                        rn = rand() % 2;
                        if (rn == 1) {
                            up_left = 0;
                            down_left = 0;
                        } else {
                            up_right = 0;
                            down_right = 0;
                        }
                    } else {
                        printf("\nWant to move 'left' or 'right'? ");
                        scanf("%s", answer);
                        while ((getchar()) != '\n');
                        if (strcmp(answer, right) && strcmp(answer, left)) {
                            err = 1;
                            printf("\nPlease only type: 'left' or 'right'. \nTry again:\n");
                        } else {
                            if (!strcmp(answer, right)) {
                                up_left = 0;
                                down_left = 0;
                            } else {
                                up_right = 0;
                                down_right = 0;
                            }
                        }
                    }
            }
        } while (err);
    }

    if (down_right) {
        board->body[l+1][n-1] = board->body[l][n];
        board->body[l][n] = NULL;
        printf("\nMoving from %c%d", number_to_letter(l), n+1);
        printf(" to %c%d ...\n", number_to_letter(l+1), n);
    }
    if (down_left) {
        board->body[l-1][n-1] = board->body[l][n];
        board->body[l][n] = NULL;
        printf("\nMoving from %c%d", number_to_letter(l), n+1);
        printf(" to %c%d ...\n", number_to_letter(l-1), n);
    }
    if (up_right) {
        board->body[l+1][n+1] = board->body[l][n];
        board->body[l][n] = NULL;
        printf("\nMoving from %c%d", number_to_letter(l), n+1);
        printf(" to %c%d ...\n", number_to_letter(l+1), (n+2));
    }
    if (up_left) {
        board->body[l-1][n+1] = board->body[l][n];
        board->body[l][n] = NULL;
        printf("\nMoving from %c%d", number_to_letter(l), n+1);
        printf(" to %c%d ...\n", number_to_letter(l-1), n+2);
    }
    if (!fast) {
        sleep(1);
        if (bot) sleep(2);
    }
}

void eat_piece(Board *board, int l, int n,_Bool bot, _Bool multi_eat, _Bool fast) {
    Piece * eater = board->body[l][n];
    char answer[20];
    char answer2[20];
    char right[] = "right";
    char left[] = "left";
    char up[] = "up";
    char down[] = "down";

    int err = 0;
    int rn = 0;

    _Bool up_right = 0;
    _Bool up_left = 0;
    _Bool down_right = 0;
    _Bool down_left = 0;

    if (board->body[l][n]->black || multi_eat || eater->dama) {

        if (n>1) {
            // right side
            if (l<8) {
                if (board->body[l+2][n-2] == NULL ) {
                    if (board->body[l+1][n-1] != NULL) {
                        if (board->body[l+1][n-1]->black != eater->black) {
                            if (!eater->dama) {
                                if (!board->body[l+1][n-1]->dama) down_right = 1;
                            } else down_right = 1;
                        }
                    }
                }
            }

            // left side
            if (l>1) {
                if (board->body[l-2][n-2] == NULL ) {
                    if (board->body[l-1][n-1] != NULL) {
                        if (board->body[l-1][n-1]->black != eater->black) {
                            if (!eater->dama) {
                                if (!board->body[l-1][n-1]->dama) down_left = 1;
                            } else down_left = 1;
                        }
                    }
                }
            }
        }
    }

    if (!board->body[l][n]->black || multi_eat || eater->dama) {

        if (n<8) {
            // right side
            if (l<8) {
                if (board->body[l+2][n+2] == NULL ) {
                    if (board->body[l+1][n+1] != NULL) {
                        if (board->body[l+1][n+1]->black != eater->black) {
                            if (!eater->dama) {
                                if (!board->body[l+1][n+1]->dama) up_right = 1;
                            } else up_right = 1;
                        }
                    }
                }
            }

            // left side
            if (l>1) {
                if (board->body[l-2][n+2] == NULL ) {
                    if (board->body[l-1][n+1] != NULL) {
                        if (board->body[l-1][n+1]->black != eater->black) {
                            if (!eater->dama) {
                                if (!board->body[l-1][n+1]->dama) up_left = 1;
                            } else up_left = 1;
                        }
                    }
                }
            }
        }
    }

    if (up_right + up_left + down_right + down_left == 0) {
        // if (!multi_eat) perror("\nERROR, piece selected can not eat.\n");
        return;
    }

    if (multi_eat) print_board(board);

    if (up_right + up_left + down_right + down_left > 1) {

            do {
                if (multi_eat || eater->dama) {
                    if (bot) {
                        rn = rand() % 4;
                        do {
                            if (rn == 0 && up_right) {
                                up_left = 0;
                                down_right = 0;
                                down_left = 0;
                            }
                            if (rn == 1 && down_right) {
                                up_left = 0;
                                up_right = 0;
                                down_left = 0;
                            }
                            if (rn == 2 && down_left) {
                                up_left = 0;
                                up_right = 0;
                                down_right = 0;
                            }
                            if (rn == 3 && up_left) {
                                up_right = 0;
                                down_right = 0;
                                down_left = 0;
                            }
                            rn = (rn + 1) % 4;
                        } while (up_right + up_left + down_right + down_left > 1);
                    } else {
                        if (err) {
                        printf("\nPlease only type one of : ");
                        if ( up_left ) printf("'up left' ");
                        if ( up_right ) printf("'up right' ");
                        if ( down_left ) printf("'down left' ");
                        if ( down_right ) printf("'down right' ");
                        printf(".\nTry again:\n");
                    }
                    err = 0;
                    printf("\nYou can eat to the : ");
                    if ( up_left ) printf("'up left' ");
                    if ( up_right ) printf("'up right' ");
                    if ( down_left ) printf("'down left' ");
                    if ( down_right ) printf("'down right' ");
                    printf("\nWhere do ypu wanna go? ");
                    scanf("%s %s", answer, answer2);
                        while ((getchar()) != '\n');
                    if (strcmp(answer, up) && strcmp(answer, down) && strcmp(answer2, right) && strcmp(answer2, left)) {
                        err = 1;
                    } else {
                        if (strcmp(answer, up) && strcmp(answer, down)) err=1; else {
                            if (!strcmp(answer, up)) {
                                if (strcmp(answer2, right) && strcmp(answer2, left)) err = 1; else {
                                    if (!strcmp(answer2, right)) {
                                        up_left = 0;
                                        down_right = 0;
                                        down_left = 0;
                                    }
                                    if (!strcmp(answer2, left)){
                                        up_right = 0;
                                        down_right = 0;
                                        down_left = 0;
                                    }
                                    }

                            } else {
                                if (strcmp(answer2, right) && strcmp(answer2, left)) err = 1; else {
                                    if (!strcmp(answer2, right)) {
                                        up_left = 0;
                                        up_right = 0;
                                        down_left = 0;
                                    }
                                    if (!strcmp(answer2, left)) {
                                        up_right = 0;
                                        down_right = 0;
                                        up_left = 0;
                                    }
                                }
                            }
                        }
                    }
                    }
                } else {
                    if (bot) {
                        rn = rand() % 2;
                        if (rn) {
                            up_left = 0;
                            down_left = 0;
                        } else {
                            up_right = 0;
                            down_right = 0;
                        }
                    } else {

                        err = 0;
                        printf("\nWant to eat to the 'left' or 'right'? ");
                        scanf("%s", answer);
                        while ((getchar()) != '\n');
                        if (strcmp(answer, right) && strcmp(answer, left)) {
                            err = 1;
                            printf("\nPlease only type: 'left' or 'right'. \nTry again:\n");
                        } else {
                            if (!strcmp(answer, right)) {
                                up_left = 0;
                                down_left = 0;
                            } else {
                                up_right = 0;
                                down_right = 0;
                            }
                        }
                    }
                }
        } while (err);
    }

    if (down_right) {
        board->body[l+2][n-2] = board->body[l][n];
        board->body[l][n] = NULL;
        // free(board->body[l+1][n-1]);
        board->body[l+1][n-1] = NULL;
        printf("\nFrom %c%d eating in %c%d ", number_to_letter(l), n+1, number_to_letter(l+1), n);
        printf("going to %c%d ...\n", number_to_letter(l+2), n-1);
        l = l+2;
        n = n-2;
    }
    if (down_left) {
        board->body[l-2][n-2] = board->body[l][n];
        board->body[l][n] = NULL;
        // free(board->body[l-1][n-1]);
        board->body[l-1][n-1] = NULL;
        printf("\nFrom %c%d eating in %c%d ", number_to_letter(l), n+1, number_to_letter(l-1), n);
        printf("going to %c%d ...\n", number_to_letter(l-2), n-1);
        l = l-2;
        n = n-2;
    }
    if (up_right) {
        board->body[l+2][n+2] = board->body[l][n];
        board->body[l][n] = NULL;
        // *board->body[l+1][n+1] = NULL;
        board->body[l+1][n+1] = NULL;
        printf("\nFrom %c%d eating in %c%d ", number_to_letter(l), n+1, number_to_letter(l+1), n+2);
        printf("going to %c%d ...\n", number_to_letter(l+2), n+3);
        l = l+2;
        n = n+2;
    }
    if (up_left) {
        board->body[l-2][n+2] = board->body[l][n];
        board->body[l][n] = NULL;
        // free(board->body[l-1][n+1]);
        board->body[l-1][n+1] = NULL;
        printf("\nFrom %c%d eating in %c%d ", number_to_letter(l), n+1, number_to_letter(l-1), n+2);
        printf("going to %c%d ...\n", number_to_letter(l-2), n+3);
        l = l-2;
        n = n+2;
    }
    if (!fast) {
        sleep(3);
        if (multi_eat) sleep(1);
    }

    if (n == 9) {
        if (!board->body[l][n]->dama && !board->body[l][n]->black) board->body[l][n]->dama = 1;
    }

    if (n == 0) {
        if (!board->body[l][n]->dama && board->body[l][n]->black) board->body[l][n]->dama = 1;
    }

    eat_piece(board, l, n, bot, 1, fast);

}

int check_winner(Board *board, _Bool black_playing) { // 1 white vince, 2 black vince;
    int white = 0;
    int black = 0;
    _Bool white_move = 0;
    _Bool black_move = 0;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (board->body[i][j] != NULL) {
                if (board->body[i][j]->black) {
                    black++;
                    if (!black_move && available_moves(board, i, j, 1) > 0) black_move = 1;
                } else {
                    white++;
                    if (!white_move && available_moves(board, i, j, 0) > 0) white_move = 1;
                }
            }
        }
    }

    if (white == 0) return 2;
    if (black == 0) return 1;
    if (!white_move && black_playing) return 2;
    if (!black_move && !black_playing) return 1;
    return 0;
}


