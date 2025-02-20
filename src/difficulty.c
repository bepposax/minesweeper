/**
 * @file difficulty.c
 * @author Ivano Izzo
 */
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "difficulty.h"
#include "string-builder.h"
#include "symbols.h"

const int side = 16, mid_col = side + 1;
const char *diffs[] = {"D I F F I C U L T Y",
                       "B E G I N N E R",
                       "I N T E R M E D I A T E",
                       "E X P E R T",
                       "C U S T O M"};
#define NUM_DIFFS (int)sizeof(diffs) / (int)sizeof(diffs[0])
int semilen[NUM_DIFFS];

extern bool is_printable(int height, int width);

/**
 * @brief initializes semilen[] with half the lengths of the strings in diffs[]
 */
static inline void init_semilenghts()
{
    for (int i = 0; i < NUM_DIFFS; ++i)
        semilen[i] = (int)strlen(diffs[i]) / 2;
}

static inline int abs(int x) { return x < 0 ? -x : x; }

/**
 * @brief mvprintw with color
 * @param color the color to use
 * @param row the row to print
 * @param col the column to print
 * @param str the string to print
 * @return the return value of mvprintw
 * @see mvprintw
 */
static int cmvprintw(int color, int row, int col, const char *str, ...);

int select_diff()
{
    int ch, offset;

#ifdef TEST
    return -1;
#endif
    print_diff_menu();
    while ((ch = getch()))
    {
        if (ch == KEY_MOUSE)
        {
            MEVENT event;
            if (getmouse(&event) == OK && (event.bstate & BUTTON1_CLICKED))
            {
                offset = abs(mid_col - event.x);
                switch (event.y)
                {
                case 6:
                    if (offset <= semilen[1])
                        return 1;
                    break;
                case 9:
                    if (offset <= semilen[2])
                        return 2;
                    break;
                case 12:
                    if (offset <= semilen[3])
                        return 3;
                    break;
                case 15:
                    if (offset <= semilen[4])
                        return 4;
                    break;
                }
            }
        }
        else if (ch == 'q' || ch == 'Q')
            break;
        else if (ch == KEY_RESIZE)
            print_diff_menu();
    }
    return 0;
}

int print_diff_menu()
{
    const int board_h = side, board_w = side * 2,
              menu_h = board_h + 4, menu_w = board_w + 3;
    int optns_col = board_w + 6;

    clear();
    if (!is_printable(menu_h, menu_w))
        return 1;

    // corner stats
    cmvprintw(COLOR_GREEN, 0, 1, "goal");
    cmvprintw(COLOR_RED, 0, menu_w - 6, "mines");
    cmvprintw(COLOR_CYAN, menu_h - 1, 1, "moves");
    cmvprintw(COLOR_YELLOW, menu_h - 1, menu_w - 5, "time");
    move(1, 0);
    refresh();

    // board
    strappend(ARC0);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC1 "\n\r");
    for (int i = 0; i < board_h; i++)
    {
        strappend(LINE_V " ");
        for (int j = 0; j < board_w; j += 2)
            strappend(CELL " ");
        strappend(LINE_V "\n\r");
    }
    strappend(ARC2);
    for (int i = 0; i <= board_w; i++)
        strappend(LINE_H);
    strappend(ARC3 "\n\r");
    printf("%s", buffer);
    offset = 0;

    // difficulties
    init_semilenghts();
    attron(A_BOLD);
    mvprintw(3, mid_col - semilen[0], "%s", diffs[0]);
    cmvprintw(COLOR_GREEN, 6, mid_col - semilen[1], diffs[1]);
    cmvprintw(COLOR_YELLOW, 9, mid_col - semilen[2], diffs[2]);
    cmvprintw(COLOR_RED, 12, mid_col - semilen[3], diffs[3]);
    cmvprintw(COLOR_CYAN, 15, mid_col - semilen[4], diffs[4]);
    attroff(A_BOLD);

    // side options
    if (optns_col + 7 < COLS)
    {
        attron(A_UNDERLINE);
        mvprintw(menu_h - 5, optns_col, "n");
        mvprintw(menu_h - 4, optns_col, "r");
        mvprintw(menu_h - 3, optns_col, "q");
        attroff(A_UNDERLINE);
        optns_col++;
        mvprintw(menu_h - 5, optns_col, "ew game");
        mvprintw(menu_h - 4, optns_col, "estart");
        mvprintw(menu_h - 3, optns_col, "uit");
    }

    return 0;
}

static int cmvprintw(int color, int row, int col, const char *str, ...)
{
    va_list args;
    va_start(args, str);
    attron(COLOR_PAIR(color));
    move(row, col);
    int r = vw_printw(stdscr, str, args);
    attroff(COLOR_PAIR(color));
    va_end(args);

    return r;
}