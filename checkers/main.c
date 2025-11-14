#include "dama.h"

int main(void) {
    Player player1;
    Player player2;
    Player * pplaying_player;
    Board board;

    _Bool err ;
    _Bool spot_allowed ;
    _Bool play = 1;
    _Bool start = 1;
    _Bool bot = 0;
    _Bool one_p_game = 0;
    _Bool fast = 0; // SET TO 1 TO HAVE THE PIECES MOVE SUPER FAST, GOOD FOR TESTING THE CODE LOGIC

    char chosen[20] = "..";
    char play_mode[20];
    char quit[] = "quit\0";

    int l_bot = 0;
    int n_bot = 0;
    int randm = 0;
    int letter;
    int number;
    int count = 0;
    int move = 0;
    int need_to_move[20][2];
    int can_move[20][2];
    int cm_count = 0;
    int ntm_count = 0;
    int winner = 0;
    srand(time(NULL));

    init_player(&player1, 0);
    init_player(&player2, 1);
    init_board(&board, &player1, &player2);

    pplaying_player = &player1; // scegli il player che inizia qui

    while (play) {

        if (!start) {
            // invertire giocatori :
            if (pplaying_player->black) {
                pplaying_player = &player1;
            } else {
                pplaying_player = &player2;
            }
        }

        if (start) { // selezione modalità gioco
            do {
                printf("    WELCOME TO CHECKERS!!!\n\n");
                printf("Would you like to play in 2 player mode, 1 player mode or bot vs bot?\n");
                printf("Insert '1p', '2p' or 'bot' mode : ");
                scanf("%s", play_mode);
                while ((getchar()) != '\n'); // pulisce il buffer per lo scanf()
            } while (strcmp(play_mode, "1p\0") && strcmp(play_mode, "2p\0") && strcmp(play_mode, "bot\0"));
            printf("\n\n");
            if (!strcmp(play_mode, "1p\0")) one_p_game = 1;
            if (!strcmp(play_mode, "bot\0")) bot = 1;
            start = 0;
        }

        if(one_p_game && pplaying_player == &player2) bot = 1;

        // controllo se pezzi possono mangiare
        for (int i = 0; i < 20; i++) {
            need_to_move[i][0] = -1;
            need_to_move[i][1] = -1;
        }
        ntm_count = 0;
        for (int l = 0; l < 10; l++) {
            for (int n = 0; n < 10; n++) {
                if (board.body[l][n] != NULL && board.body[l][n]->black == pplaying_player->black) {
                    if (available_moves(&board, l, n, pplaying_player->black) > 1) {
                        need_to_move[ntm_count][0] = l;
                        need_to_move[ntm_count][1] = n;
                        ntm_count++;
                    }
                }
            }
        }

        print_board(&board);
        printf("\n\nNow playing -> player %d ", (pplaying_player == &player2) + 1);

        if (ntm_count == 1) { // se solo una pedina può mangiare
            if (!bot) {
                printf("\nThere is only one spot that can eat, so you have to move that one\n");
                printf("Eating automatically...\n");
                sleep(1);
            }
            letter = need_to_move[0][0];
            number = need_to_move[0][1];
            eat_piece(&board, letter, number, bot, 0, fast);
            } else {
                // reset della variabile choosen
                chosen[0] = '.';
                chosen[1] = '.';
                chosen[2] = '\0';
                count = 0;
                do {
                    letter = 0;
                    number = 0;
                    err = 0;
                    if (!(count % 2) && count > 0) print_board(&board);

                    if (ntm_count > 1 ) {
                        if (bot) {
                            randm = rand() % ntm_count;
                            l_bot = need_to_move[randm][0];
                            n_bot = need_to_move[randm][1];
                        } else {
                            printf("\nThe following spots can eat, so you need to choose one of them:\n");
                            for (int i = 0; i < ntm_count; i++) {
                                printf("'%c%d' ", number_to_letter(need_to_move[i][0]), need_to_move[i][1] + 1);
                            }
                        }
                    } else {
                        cm_count = 0;
                        for (int l = 0; l < 10; l++) {
                            for (int n = 0; n < 10; n++) {
                                if (board.body[l][n] != NULL) {
                                    if (board.body[l][n]->black == pplaying_player->black) {
                                        if (available_moves(&board, l, n, pplaying_player->black) == 1) {
                                            can_move[cm_count][0] = l;
                                            can_move[cm_count][1] = n;
                                            cm_count++;
                                        }
                                    }
                                }

                            }
                        }
                        if (bot && cm_count>0) {
                            randm = (rand() % cm_count);
                            l_bot = can_move[randm][0];
                            n_bot = can_move[randm][1];
                        }
                    }

                    if (!bot) {
                        printf("\n\n(insert 'quit' to stop playing)\n");
                        printf("Insert the spot of the piece you'd like to move (letter-number; es.a1) --> ");
                        scanf("%s" , chosen);
                        while ((getchar()) != '\n');

                        if (!strcmp(chosen, quit)) {
                            play = 0;
                            break;
                        }

                        if (strlen(chosen)!=2) {
                            err = 1;
                            count++;
                            printf("Spot inserted not valid, please insert in the following way:\n");
                            printf("ln (l--> 1 letter from 'a' to 'b', n--> number 1 to 10) ES. b2;\nTry again:");
                            continue;
                        }
                        letter = tolower(letter_to_number(chosen[0]));
                        if (letter < 0) {
                            printf("Letter is wrong, it must be from 'a' to 'l'.\nTry again:");
                            err = 1;
                            count++;
                            continue;
                        }
                        number = (atoi(&chosen[1])) -1;
                        spot_allowed = 0;
                        if (ntm_count) { // se una pedina può mangiare, controllo se ha selezionato una di quelle
                            for (int i = 0; i < ntm_count; i++) {
                                if (need_to_move[i][0] == letter && need_to_move[i][1] == number) spot_allowed = 1;
                            }
                            if (!spot_allowed) {
                                err = 1;
                                continue;
                            }
                        }
                    }

                    if (bot) {
                        move = available_moves(&board, l_bot, n_bot, pplaying_player->black);
                    } else move = available_moves(&board, letter, number, pplaying_player->black);

                    if (move < 1) { // controllo errore con spot
                        err = 1;
                        count++;
                        if (move==0) {
                            printf("Piece can not move.\nChoose an other spot:");
                        }
                        if (move==-1) {
                            printf("Letter is wrong, it must be from 'a' to 'l'.\nTry again:");
                        }
                        if (move==-2) {
                            printf("Number is wrong, it must be from 1 to 10.\nTry again:");
                        }
                        if (move==-3) {
                            printf("Spot chosen is empty.\nChoose an other spot:");
                        }
                        if (move==-4) {
                            printf("Spot is taken by an adversary piece.\nChoose an other spot:");
                        }
                    } else {
                    if (move==1) {
                        if (!bot) {
                            move_piece(&board, letter, number, bot, fast);
                        } else {
                            move_piece(&board, l_bot, n_bot, bot, fast);
                        }

                    } else {
                        if (bot) eat_piece(&board, l_bot, n_bot, bot, 0, fast); else
                        eat_piece(&board, letter, number, bot, 0, fast);
                    }
                }
            } while (err);

        }
        winner = check_winner(&board, pplaying_player->black);
        if (winner) {
            print_board(&board);
            if (winner == 1) {
                printf("\n\n    GAME OVER!!!\n  Player 1 won! \n\nSee you next time :)\n");
            } else {
                printf("\n\n    GAME OVER!!!\n  Player 2 won! \n\nSee you next time :)\n");
            }
            play = 0;
        } else {
            if (!play) printf("\n\n    GAME OVER!!!\n\nSee you next time :)\n");
        }

        if(one_p_game && pplaying_player == &player2) bot = 0;
    }

    return 0;
}
