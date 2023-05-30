/**
 * @file minesweeper.h
 * @author Ivano Izzo
 */
#pragma once
#include "../include/board.h"

extern int goal, moves, uncovered_cells;
extern bool game_over;

/**
 * @brief places the mines randomly on the board
 */
void place_mines();

/**
 * @brief signals a mine to the cells surrounding it
 * @param row the mine's row
 * @param col the mine's column
 */
void signal_mine(int, int);

/**
 * @brief the game loop - it ends when the game is over or the user quits
 * @return 'q' to quit the game or 'n' to start a new game
 */
int game_loop();

/**
 * @brief plays a move in a cell
 * @param row the cell's row
 * @param col the cell's column
 * @return 1 if the game is over
 *         0 if the game is not over
 */
int play(int, int);

/**
 * @brief flags or unflags an undiscovered cell
 * @param row the cell's row
 * @param col the cell's column
 */
void flag(int, int);

/**
 * @brief discovers the cells surrounding a cell
 * @param row the cell's row
 * @param col the cell's column
 * @return 1 if the game is over
 *         0 if the game is not over
 * @see discoverable
 */
int discover(int, int);

/**
 * @brief prints the game board
 */
void print_board();
