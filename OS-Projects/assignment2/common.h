// Height and width of the playing area.
#define GRID_SIZE 5

// Pathname of personal AFS home directory.
#define HOME_DIR "/afs/unity.ncsu.edu/users/c/cwfields"

// Key for accessing shared memory
#define KEY HOME_DIR, 0

// Struct type used to hold the game state
typedef struct GameStateStruct {
  /** Representation of the current board */
  char currentBoard[GRID_SIZE][GRID_SIZE];

  /** Representation of board before last move */
  char previousBoard[GRID_SIZE][GRID_SIZE];

  /** Flag representing whether or not the current state can undo */
  bool canUndo;
} GameState;