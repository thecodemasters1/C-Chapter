/****************************************************************
 * Summary: This program implements the Game of Life.           *
 *                                                              *
 * Example: game_of_life                                        *
 *          game_of_life world.txt                              *
 ****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WORLD_SIZE  (60)
#define SIZEX       WORLD_SIZE
#define SIZEY       WORLD_SIZE
#define DEAD        (' ')
#define ALIVE       ('*')
#define DELAY       (20)
#define RAND()      DEAD + ( (ALIVE - DEAD) * (rand() & 0x1) )

#define INVALID_ARGS        (-1)
#define INVALID_FORMAT      (-2)
#define FAILED_TO_OPEN      (-3)
#define FAILED_TO_CLOSE     (-4)
#define NOT_ENOUGH_MEMORY   (-5)

void set_window_size();

int start_file(char world[SIZEX][SIZEY], char *file_name);

void start_rand(char world[SIZEX][SIZEY]);

int condition(int gen, int changed);

char check_cell(char world[SIZEX][SIZEY], int i, int j);

int step(char before[SIZEX][SIZEY], char after[SIZEX][SIZEY]);

void print(char world[SIZEX][SIZEY]);

static HANDLE wHnd = NULL;    /* Handle to change window size */

int main(int argc, char *argv[])
{
	int current = 0;
	int gen = 0;
	int changed = 0;
	char world[2][SIZEX][SIZEY];

	/* Check args */
	if (1 == argc) {
		/* No extra args - use random to fill up the world. */
		start_rand(world[0]);
	} else if (2 == argc) {
		/* One extra arg - read board from text file, deal with possible problems. */
		switch(start_file(world[0], argv[1]))
		{
		case INVALID_FORMAT:/* File is in incorrect format. */
			printf("Invalid file format.\n"
				"The file must contain %d lines with %d characters in each row.\n"
				"The characters can only be '%c' for living cells OR '%c' for dead cells.\n",
				SIZEX, SIZEY, ALIVE, DEAD);
			return INVALID_FORMAT;
		case FAILED_TO_OPEN:/* Cannot open file. */
			printf("Could not open \"%s\"", argv[1]);
			return FAILED_TO_OPEN;
		case FAILED_TO_CLOSE:/* Cannot close file. */
			printf("Could not close \"%s\"", argv[1]);
			return FAILED_TO_CLOSE;
		default:/* All is well */
			break;
		}
	} else {
		/* Too many extra args - invalid use. */
		printf("Usage:\n"
			" %s\t" "start with a random board.\n"
			" %s file_name\t" "read %d" "x" "%d board from file.\n",
			argv[0], argv[0], SIZEX, SIZEY);
		return INVALID_ARGS;
	}
	/* Initialize other variables */
	current = 0;
	changed = 1;
	gen = 1;

	/* Make console window big enough */
	set_window_size();

	/* step */
	while (0 != condition(gen, changed)) {
		/* Show the world */
		print(world[current]);
		/* Next step */
		changed = step(world[current], world[!current]);

		current = !current;
		++gen;
		
		Sleep(DELAY);
	}
	/* Show the last world */
	print(world[current]);

	return 0;
}

/****************************************************************
 * Summary: Sets the size of the window to match the size of    *
 *          the world.                                          *
 *                                                              *
 * Parameters: None.                                            *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void set_window_size()
{
	SMALL_RECT windowSize = {0, 0, SIZEX + 1, SIZEY + 1};

	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
}

/****************************************************************
 * Summary: Initializes the world using input from a file.      *
 *                                                              *
 * Parameters: world - Represents the world.                    *
 *             file_name - Represents the input file.           *
 *                                                              *
 * Returns: 0 if successful, can return NOT_ENOUGH_MEMORY,      *
 *          FAILED_TO_OPEN, FAILED_TO_CLOSE, INVALID_FORMAT.    *
 ****************************************************************/
int start_file(char world[SIZEX][SIZEY], char *file_name)
{
	const int templ_size = SIZEY + 2;
	int i = 0, j = 0; /* Loop variables */
	char *templ = NULL;
	FILE *fp_input = NULL;

	/* Open file */
	fp_input = fopen(file_name, "r");

	if (NULL == fp_input) {
		return FAILED_TO_OPEN;
	}

	/* Allocate memory */
	templ = (char *)malloc(templ_size);

	if (NULL != templ) {
		/* Read line by line */
		for (i = 0 ; i < SIZEX ; ++i) {
			/* Reset temp line */
			memset(templ, 0, SIZEY + 2);
			if (NULL == fgets(templ, templ_size, fp_input)) { /* in case of file in incorrect format */
				free(templ);
				return INVALID_FORMAT;
			}
			/* Save data in world matrix */
			for (j = 0 ; j < SIZEY ; ++j) {
				if ( (DEAD == templ[j]) || (ALIVE == templ[j]) ) {/* valid character */
					world[i][j] = templ[j];
				} else {/* invalid character */
					free(templ);
					return INVALID_FORMAT;
				}
			}
		}
		
		/* Free memory */
		free(templ);
	} else {
		return NOT_ENOUGH_MEMORY;
	}

	/* Close file */
	if (0 != fclose(fp_input)) {
		return FAILED_TO_CLOSE;
	}

	return 0;
}

/****************************************************************
 * Summary: Initializes the world with random.                  *
 *                                                              *
 * Parameters: world - Represents the world.                    *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void start_rand(char world[SIZEX][SIZEY])
{
	int i = 0, j = 0; /* Loop variables */

	for (i = 0 ; i < SIZEX ; ++i) {
		for (j = 0 ; j < SIZEY ; ++j) {
			world[i][j] = RAND();
		}
	}
}

/****************************************************************
 * Summary: Checks the condition whether to continue or stop.   *
 *                                                              *
 * Parameters: gen - the number of the current generation.      *
 *             changed - 1 if the world changed in the last     *
 *                       step, 0 if it didn't.                  *
 *                                                              *
 * Returns: Non-0 if condition exists, otherwise 0.             *
 ****************************************************************/
int condition(int gen, int changed)
{
	return 0 != changed;
}

/****************************************************************
 * Summary: Calculates the future status of the specified cell. *
 *                                                              *
 * Parameters: world - Represents the world.                    *
 *             row - The row that the cell is at.               *
 *             col - The column that the cell is at.            *
 *                                                              *
 * Returns: The future status of the specified cell - DEAD or   *
 *          ALIVE.                                              *
 ****************************************************************/
char check_cell(char world[SIZEX][SIZEY], int row, int col)
{
	int c_alive = 0;
	int i = 0, j = 0; /* Loop variables */

	/* Count living cells agound this one. */
	for (i = ((row > 0) ? row - 1 : row) ; i <= ((row < SIZEX - 1) ? row + 1 : row) ; ++i) {
		for (j = ((col > 0) ? col - 1 : col) ; j <= ((col < SIZEY - 1) ? col + 1 : col) ; ++j) {
			if ((i != row) || (j != col)) {
				c_alive += (world[i][j] == ALIVE);
			}
		}
	}

	/* A dead cell with exactly three live neighbors becomes a live cell (birth). */
	if ((world[row][col] == DEAD) && (c_alive == 3)) {
		return ALIVE;
	}
	/* A live cell with two or three live neighbors stays alive (survival). */
	if ((world[row][col] == DEAD) && ( (c_alive == 2) || (c_alive == 3) )) {
		return ALIVE;
	}
	/* In all other cases, a cell dies or remains dead (overcrowding or loneliness). */
	return DEAD;
}

/****************************************************************
 * Summary: Sets the second char[][] to the world on the next   *
 *          step using the forst one as a starting point.       *
 *                                                              *
 * Parameters: before - Represents the world before the step.   *
 *             after - Will have its values set to the world on *
 *                     next step.                               *
 *                                                              *
 * Returns: 1 if the world has changed, 0 if not.               *
 ****************************************************************/
int step(char before[SIZEX][SIZEY], char after[SIZEX][SIZEY])
{
	int changed = 0;
	int i = 0, j = 0; /* Loop variables */

	/* Go through the world, the the next one */
	for (i = 0 ; i < SIZEX ; ++i) {
		for (j = 0 ; j < SIZEY ; ++j) {
			after[i][j] = check_cell(before, i, j);
			/* Remember change */
			changed |= (after[i][j] != before[i][j]);
		}
	}

	return changed;
}

/****************************************************************
 * Summary: Prints the world.                                   *
 *                                                              *
 * Parameters: world - Represents the world.                    *
 *                                                              *
 * Returns: void.                                               *
 ****************************************************************/
void print(char world[SIZEX][SIZEY])
{
	int i = 0, j = 0; /* Loop variables */
	COORD posZero = {0, 0};
	
	SetConsoleCursorPosition(wHnd, posZero);
	/*Old solution:
	system("cls");*/
	for (i = 0 ; i < SIZEX ; ++i) {
		for (j = 0 ; j < SIZEY ; ++j) {
			printf("%c", world[i][j]);
		}
		printf("\n");
	}
}