#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#ifndef DAMA_H
#define DAMA_H

typedef struct {
    _Bool black; // 1 for black and 0 for white
    _Bool dama;
} Piece;

typedef struct  {
    Piece * body[10][10];
} Board;

typedef struct {
    _Bool black; // 1 for black and 0 for white
    Piece pieces[20];
} Player;

void init_piece(Piece * piece, _Bool black) ;

void init_player(Player * player, _Bool _black) ;

void init_board(Board * board, Player * white_player, Player * black_player) ;

void print_board(Board * board) ;

int letter_to_number(char letter) ;

char number_to_letter(int number) ;

int available_moves(Board * board, int l, int n, _Bool black_player);

void move_piece(Board *board, int l, int n, _Bool bot, _Bool fast);

void eat_piece(Board *board, int l, int n,_Bool bot, _Bool multi_eat, _Bool fast);

int check_winner(Board *board, _Bool black_playing);

#endif //DAMA_H
