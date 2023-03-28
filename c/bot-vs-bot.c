#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct Position {
  int x;
  int y;
} Position;

typedef enum Mark {
  EMPTY = 0,
  PLAYER_X = 1,
  PLAYER_O = -1
} Mark;

static const int WIN_COMBOS[8][3] = {
  // Rows
  {0, 1, 2},
  {3, 4, 5},
  {6, 7, 8},

  // Columns
  {0, 3, 6},
  {1, 4, 7},
  {2, 5, 8},

  // Diagonals
  {0, 4, 8},
  {2, 4, 6}
};

Position coordsFromIndex(int idx) {
  int row = idx / 3;
  int col = row == 0 ? idx : (idx - (row * 3));

  // printf("Calculating coordinates from index %d\n", idx);
  // printf("  Result: row %d, col %d\n", row, col);

  return (Position){.x = row, .y = col};
}

void getFreePositions(
  int board[3][3],
  Position **positions,
  size_t *positionCount) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == EMPTY) {
        (*positionCount)++;
        // printf("Address of array (%d) before realloc: %p\n", (int)*positionCount, *positions);
        *positions =
          realloc(*positions, (*positionCount) * sizeof(Position));
        // printf("Address of array (%d) after realloc: %p\n", (int)*positionCount, *positions);
        Position *pos = malloc(sizeof(Position));
        pos->x = i;
        pos->y = j;
        (*positions)[(*positionCount) - 1] = *pos;
        free(pos);
      }
    }
  }
}

char cfi(Mark player) {
  if (player == 0) {
    return ' ';
  }

  return player == 1 ? 'X' : 'O';
}

void printBoard(int board[3][3]) {
  printf("     1   2   3  \n");
  printf("   +---+---+---+\n");
  printf(" 1 | %c | %c | %c |\n", cfi(board[0][0]), cfi(board[0][1]), cfi(board[0][2]));
  printf("   +---+---+---+\n");
  printf(" 2 | %c | %c | %c |\n", cfi(board[1][0]), cfi(board[1][1]), cfi(board[1][2]));
  printf("   +---+---+---+\n");
  printf(" 3 | %c | %c | %c |\n", cfi(board[2][0]), cfi(board[2][1]), cfi(board[2][2]));
  printf("   +---+---+---+\n\n");
}

Mark calcScore(int board[3][3]) {
  for (int i = 0; i < 8; i++) {
    const int *combo = WIN_COMBOS[i];
    const Position c1 = coordsFromIndex(combo[0]);
    const Position c2 = coordsFromIndex(combo[1]);
    const Position c3 = coordsFromIndex(combo[2]);

    if (board[c1.x][c1.y] != EMPTY
      && board[c1.x][c1.y] == board[c2.x][c2.y]
      && board[c2.x][c2.y] == board[c3.x][c3.y]
    ) {
      return board[c1.x][c1.y];
    }
  }

  return 0;
}

int minimax(int board[3][3], int maximizingPlayer) {
  Mark winner = calcScore(board);

  if (winner != 0) {
    return winner * maximizingPlayer;
  }

  Position *freePositions = NULL;
  size_t freePosCount = 0;

  getFreePositions(board, &freePositions, &freePosCount);

  int move = -1;
  int score = -2;

  for (int i = 0; i < freePosCount; i++) {
    board[freePositions[i].x][freePositions[i].y] = maximizingPlayer;
    int val = -minimax(board, maximizingPlayer * -1);
    board[freePositions[i].x][freePositions[i].y] = EMPTY;

    if (val > score) {
      score = val;
      move = i;
    }
  }

  free(freePositions);

  if (move == -1) {
    return 0;
  }

  return score;
}

Position findBestMove(int board[3][3], Mark player) {
  Position *freePositions = NULL;
  size_t freePosCount = 0;
  Position bestPosition = {-1, -1};
  int score = -2;

  getFreePositions(board, &freePositions, &freePosCount);

  for (int i = 0; i < freePosCount; i++) {
    board[freePositions[i].x][freePositions[i].y] = player;

    int moveScore = -minimax(board, player * -1);

    board[freePositions[i].x][freePositions[i].y] = EMPTY;

    if (moveScore > score) {
      score = moveScore;
      bestPosition.x = freePositions[i].x;
      bestPosition.y = freePositions[i].y;
    }
  }

  free(freePositions);

  return bestPosition;
}

int main() {
  int board[3][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
  };

  printf("Welcome to Tic Tac Toe - bot VS bot!\n\n");

  // Give X random starting position
  srand(time(NULL));
  int startIdx = rand() % 10;
  Position startPos = coordsFromIndex(startIdx);
  board[startPos.x][startPos.y] = PLAYER_X;

  int movesLeft = 8;
  Mark nextPlayer = -1;  // 1 = X, -1 = O

  printBoard(board);

  do {
    Position bestPos = findBestMove(board, nextPlayer);
    board[bestPos.x][bestPos.y] = nextPlayer;
    nextPlayer = nextPlayer * -1;

    printf("\"%c\" chose position %d - %d\n\n", cfi(nextPlayer * -1), bestPos.x + 1, bestPos.y + 1);

    printBoard(board);

    sleep(1);

    --movesLeft;
  } while (movesLeft);

  Mark winner = calcScore(board);

  if (winner == 0) {
    printf("Draw! Duh.\n");
  } else {
    printf("\"%c\" won!\n", cfi(winner));
  }

  return 0;
}
