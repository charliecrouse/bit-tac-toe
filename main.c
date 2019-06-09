#include <stdio.h>
#include <stdlib.h>

/*
 * Get the n bits of x starting from position p
 */
unsigned get_bits(unsigned x, unsigned p, unsigned n) {
  // Create a mask of 1 bits of length n starting at position 0
  unsigned mask = ~(~0U << n);

  // Shift the end of the desired range of bits to position 0
  x = x >> (p + 1 - n);

  // Zero out the bits left of the starting position
  x = x & mask;

  return x;
}

/*
 * Set n bits of x from position p to the rightmost n bits of y
 */
void set_bits(unsigned *x, int p, int n, unsigned y) {
  // Create a mask of 1 bits of length n starting at position 0
  unsigned mask = ~(~0U << n);

  // Zero out the bits to set in x
  *x = *x & ~(mask << (p + 1U - n));

  // Shift the bits in y and set them in x
  *x = *x | ((y & mask) << (p + 1U - n));
}

unsigned get_active_bit(unsigned x) { return get_bits(x, 31, 1); }

unsigned get_symbol_bit(unsigned x) { return get_bits(x, 30, 1); }

unsigned get_north_bits(unsigned x) { return get_bits(x, 29, 10); }

unsigned get_northwest_bits(unsigned x) { return get_bits(x, 19, 10); }

unsigned get_west_bits(unsigned x) { return get_bits(x, 9, 10); }

void set_active_bit(unsigned *x) { set_bits(x, 31, 1, 1U); }

void set_symbol_bit(unsigned *x, char symbol) {
  set_bits(x, 30, 1, symbol == 'X' ? 0U : 1U);
}

void set_north_bits(unsigned *x, unsigned y) { set_bits(x, 29, 10, y); }

void set_northwest_bits(unsigned *x, unsigned y) { set_bits(x, 19, 10, y); }

void set_west_bits(unsigned *x, unsigned y) { set_bits(x, 9, 10, y); }

void print_board(unsigned **board, int N) {
  // printf("\e[1;1H\e[2J");
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      char symbol = ' ';
      if (get_active_bit(board[i][j])) {
        symbol = get_symbol_bit(board[i][j]) ? 'O' : 'X';
      }
      printf("%c%s", symbol, j == N - 1 ? "\n" : "|");
    }
    printf("%s", i == N - 1 ? "" : "- - -\n");
  }
}

void init_board(unsigned ***board, const int N) {
  *board = (unsigned **)malloc(sizeof(unsigned *) * N);

  int i, j;
  for (i = 0; i < N; i++) {
    (*board)[i] = (unsigned *)malloc(sizeof(unsigned) * N);
    for (j = 0; j < N; j++) {
      (*board)[i][j] = 0U;
    }
  }
}

int valid_coords(int row, int col, int N) {
  return (row >= 0 && row < N) && (col >= 0 && col < N);
}

int check_winner(unsigned **board, const int N) {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if (!get_active_bit(board[i][j])) continue;

      char symbol = get_symbol_bit(board[i][j]);

      int row, col;

      // set north bits (i - 1, j)
      row = i - 1;
      col = j;
      set_north_bits(&board[i][j],
                     valid_coords(row, col, N) &&
                             get_active_bit(board[row][col]) &&
                             get_symbol_bit(board[row][col]) == symbol
                         ? get_north_bits(board[row][col]) + 1
                         : 0);

      // set northwest bits (i-1, j-1)
      row = i - 1;
      col = j - 1;
      set_northwest_bits(&board[i][j],
                         valid_coords(row, col, N) &&
                                 get_active_bit(board[row][col]) &&
                                 get_symbol_bit(board[row][col]) == symbol
                             ? get_northwest_bits(board[i - 1][j - 1] + 1)
                             : 0);
      // set west bits (i, j-1)
      row = i;
      col = j - 1;
      set_west_bits(&board[i][j],
                    valid_coords(row, col, N) &&
                            get_active_bit(board[row][col]) &&
                            get_symbol_bit(board[row][col]) == symbol
                        ? get_west_bits(board[row][col]) + 1
                        : 0);

      unsigned val = board[i][j];
      if (get_north_bits(val) == 2 || get_northwest_bits(val) == 2 ||
          get_west_bits(val) == 2) {
        return symbol == 'X' ? 1 : 2;
      }
    }
  }

  return 0;
}

void play_game() {
  const int N = 3;
  unsigned **board;
  init_board(&board, N);

  unsigned turn = 0;
  while (turn < N * N) {
    print_board(board, N);

    int row, col;
    printf("Enter the row:\n");
    scanf("%d", &row);
    printf("Enter the col:\n");
    scanf("%d", &col);

    if (!valid_coords(row, col, N)) {
      fprintf(stderr, "\nInvalid square: %d, %d. Try again:\n", row, col);
      continue;
    }

    if (get_active_bit(board[row][col]) & 1) {
      fprintf(stderr, "\nThat square has been taken. Try again:\n");
      continue;
    }

    char symbol = turn & 1 ? '0' : 'X';

    set_active_bit(&board[row][col]);
    set_symbol_bit(&board[row][col], symbol);

    int winner;
    if ((winner = check_winner(board, N))) {
      printf("\nPlayer %d has won!\n", winner);
      exit(0);
    }
    turn += 1U;
  }

  print_board(board, N);
  printf("\nNo winner!");
}

int main(int argc, char **argv) {
  play_game();
  return 0;
}
