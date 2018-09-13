#include <iostream>

const int status_incorrect_player = -1;
const int status_space_taken = -2;
const int status_valid_move = 0;
const int status_winner = 1;
const int status_draw = 2;


// Player Definition
typedef int Player;

// Location Structure
struct Location{
  int x,y;
};

// Result Structure
struct Result{
  Player player;
  Location location;
  int status;
};

struct WinPosibility{
  Player player;
  int spaces_taken;
};

// Square NxN board, win with N consecutive.
class TicTacToe {
 public:
  TicTacToe();
  TicTacToe(int size, int num_players);
  ~TicTacToe();

  ///
  /// \brief MakeMove Interface for the game playing system to add a new move to
  /// the game.
  /// \param player   Player making this move.
  /// \param location The selected location on the board.
  /// \return Result of the move, including the new game status such as Win, Invalid, etc.
  ///
  Result MakeMove(Player player, Location location);
  void clearGame();

private:
  /// Create a representation of the game state and any internal structures to help
  /// determine win conditions. The implementation should easily extend to different
  /// board sizes and run time should scale linearly (or better) with N.
  
  int move;
  int board_size;
  int players;
  int min_moves_to_win;
  int diagonal_win_position;
  int **board;
  WinPosibility *posibilities;
  int max_moves;
  void allocBoard();
  int TestWin(Location location);
  void printBoard();
};

// Generic 3x3 constructor for 2 players
TicTacToe::TicTacToe(){
  board_size = 3;
  players = 2;
  move = 1;
  max_moves = 9;
  diagonal_win_position = 2 * board_size;
  // Create the board
  allocBoard();
  // Initialize board to all zeros
  for(int i = 0; i < board_size; i++){
    for(int j = 0; j < board_size; j++){
      board[i][j] = 0;
    }
  }
  min_moves_to_win = 5;
}

// Constructor for boards of different sizes and players
TicTacToe::TicTacToe(int size, int num_players){
  move = 1;
  board_size = size;
  players = num_players;
  max_moves = board_size * board_size;
  diagonal_win_position = 2 * board_size;
  allocBoard();
  // Initialize board to all zeros
  for(int i = 0; i < board_size; i++){
    for(int j = 0; j < board_size; j++){
      board[i][j] = 0;
    }
  }
  min_moves_to_win = (board_size-1) * players + 1;
}

// Destructor
TicTacToe::~TicTacToe(){
  for(int i = 0; i < board_size; i++){
    delete[] board[i];
  }
  delete[] board;
  delete[] posibilities;
}

Result TicTacToe::MakeMove(Player player, Location location){
  Result result;
  
  result.player = player;
  result.location = location;
  
  // Test for correct player (Assuming 2 players)
  if((move % players) != (player % players)){

    result.status = status_incorrect_player;
    return result;
  }
  
  // Test for open space
  if(board[location.x][location.y] != 0){
    result.status = status_space_taken;
    return result;
  }
  // Fill space if open
  board[location.x][location.y] = player;

  // Fill Win Container and Check for win

  // Fill x direction
  if(posibilities[location.x].player == 0){
    posibilities[location.x].player = player;
    posibilities[location.x].spaces_taken++;
  }
  else if(posibilities[location.x].player == player){
    posibilities[location.x].spaces_taken++;
    if(posibilities[location.x].spaces_taken == board_size){
      std::cout << "Winner" << std::endl;
    }
  } else {
    posibilities[location.x].player = -1; // Player can no longer win here;
    posibilities[location.x].spaces_taken++;
  }

  // Fill y direction
  int y_dir = location.y + board_size;
  if(posibilities[y_dir].player == 0){
    posibilities[y_dir].player = player;
    posibilities[y_dir].spaces_taken++;
  }
  else if(posibilities[y_dir].player == player){
    posibilities[y_dir].spaces_taken++;
    if(posibilities[y_dir].spaces_taken == board_size){
      std::cout << "Winner" << std::endl;
    }
  } else {
    posibilities[y_dir].player = -1; // Player can no longer win here;
    posibilities[y_dir].spaces_taken++;
  }
  
  // Fill TL to BR win
  if(location.x == location.y){
    if(posibilities[diagonal_win_position].player == 0){
      posibilities[diagonal_win_position].player = player;
      posibilities[diagonal_win_position].spaces_taken++;
    }
    else if(posibilities[diagonal_win_position].player == player){
      posibilities[diagonal_win_position].spaces_taken++;
      if(posibilities[diagonal_win_position].spaces_taken == board_size){
        std::cout << "Winner" << std::endl;
      }
    } else {
      posibilities[diagonal_win_position].player = -1; // Player can no longer win here;
      posibilities[diagonal_win_position].spaces_taken++;
    }
  }

  // Fill BL to TR win
  int diagonal_win_position2 = diagonal_win_position + 1;
  if(location.x + location.y == board_size - 1){
    if(posibilities[diagonal_win_position2].player == 0){
      posibilities[diagonal_win_position2].player = player;
      posibilities[diagonal_win_position2].spaces_taken++;
    }
    else if(posibilities[diagonal_win_position2].player == player){
      posibilities[diagonal_win_position2].spaces_taken++;
      if(posibilities[diagonal_win_position2].spaces_taken == board_size){
        std::cout << "Winner" << std::endl;
      }
    } else {
      posibilities[diagonal_win_position2].player = -1; // Player can no longer win here;
      posibilities[diagonal_win_position2].spaces_taken++;
    }
  }

  // 
  
  // Test for result (Must have made 2x board_size -1 before player can win)
  int result_val = 0;
  if(move >= min_moves_to_win){
    result_val = TestWin(location);
  }
  move++;
  
  result.status = result_val;

  //printBoard();
  
  // Return result
  return result;
}

void TicTacToe::clearGame(){
  // Initialize board to all zeros
  for(int i = 0; i < board_size; i++){
    for(int j = 0; j < board_size; j++){
      board[i][j] = 0;
    }
  }
  WinPosibility clear = {0,0};
  for(int i = 0; i<(2*board_size+2); i++){
    posibilities[i] = clear;
  }
  
  move = 1;
}

// Board allocation as NxN matrix of board_size
void TicTacToe::allocBoard(){
  board = new int*[board_size];
  for(int i = 0; i < board_size; i++){
    board[i] = new int[board_size];
  }
  posibilities = new WinPosibility[2*board_size + 2];
}

// Test for a win/tie
int TicTacToe::TestWin(Location location){
  // Test row for win (board size is also equal to the consecutive spaces needed to win)
  for(int i = 0; i < board_size; i++){
    if((board[i][location.y]) != (board[location.x][location.y])){
      
      //std::cout << "Row- " << board[i][location.y] << " != " << board[location.x][location.y] << std::endl;
      i = board_size; // Not a winner
    }
    if(i == (board_size - 1)){
      return status_winner;
    }
  }
  
  // Test column for win (board size is also equal to the consecutive spaces needed to win)
  for(int i = 0; i < board_size; i++){
    if((board[location.x][i]) != (board[location.x][location.y])){
      //std::cout << "Column- " << board[i][location.y] << " != " << board[location.x][location.y] << std::endl;
      i = board_size; // Not a winner
    }
    if(i == (board_size - 1)){
      return status_winner;
    }
  }
  
  // If on top left to bottom right diagonal, test for win
  if(location.x == location.y){
    for(int i = 0; i < board_size; i++){
      if((board[i][i]) != (board[location.x][location.y])){
        i = board_size; // Not a winner
      }
      if(i == (board_size - 1)){
        return status_winner;
      }
    }
  }
  // If on bottom left to top right diagonal, test for win
  if((location.x + location.y) == (board_size - 1)){
    for(int i = 0; i < board_size; i++){
      if((board[i][(board_size - 1) - i]) != (board[location.x][location.y])){
        i = board_size; // Not a winner
      }
      if(i == (board_size - 1)){
        return status_winner;
      }
    }
  }
  
  // If max number of moves has been made, call a draw
  if(move == max_moves){
    return status_draw;
  }
  return 0;
}

void TicTacToe::printBoard(){
  for(int i = 0; i < board_size; i++){
    for(int j = 0; j < board_size; j++){
      std::cout << " " << board[j][i] << " | "; 
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int main(int argc, char** argv) {
  std::cout << "Starting test!" << std::endl;

  Result result;
  Player player = 1;
  Location location;

  int num_players = 2;
  int size = 3;
  if(argc > 1){
    for(int i = 1; i<argc; i++){
      if(argv[i][0] == '-'){
        switch (argv[i][1]){
          case 'P':
          case 'p':
            num_players = atoi(&argv[i][3]);
            break;
          case 'B':
          case 'b':
            size = atoi(&argv[i][3]);
            break;
          default:
            break;
        }
      }
    }
  }
  if(num_players >= size){
    std::cout << "Error - Too many players, board size must be greater than the number of players." << std::endl;
    return -1;
  }
  
  TicTacToe game(size, num_players); // Start a game with board size = size
  
  
 // Add some useful test cases.
  // result = game.MakeMove(Player, Location)
  // Check result is correct.
  
  // test for a valid move
  player = 1;
  location.x = 1;
  location.y = 1;
  result = game.MakeMove(player, location);
  if(result.status != status_valid_move){
    std::cout << "Error - Single move not valid" << std::endl;
    return -1;
  }
  
  
  // test for an invalid move - space taken
  player = 2;
  // location is the same
  result = game.MakeMove(player, location);
  if(result.status != status_space_taken){
    std::cout << "Error - Invalid, location taken, move accepted" << std::endl;
    return -2;
  }
  
  // test for invalid move - wrong player
  player = 1;
  location.x = 0;
  location.y = 0;
  result = game.MakeMove(player, location);
  if(result.status != status_incorrect_player){
    std::cout << "Error - Wrong player move accepted" << std::endl;
    return -3;
  }
  
  game.clearGame();
  
  // test for a row win
  player = 1;
  location.x = 1;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 0;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 0;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 2;
  location.y = 2;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 2;
  location.y = 1;
  result = game.MakeMove(player, location);
  
  if(result.status != status_winner){
    std::cout << result.status << std::endl;
    std::cout << "Error - Row winner not detected" << std::endl;
    return -4;
  }
  
  
  
  // test for a column win
  
  game.clearGame();
  
  player = 1;
  location.x = 1;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 0;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 1;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 2;
  location.y = 2;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 1;
  location.y = 2;
  result = game.MakeMove(player, location);
  
  if(result.status != status_winner){
    std::cout << "Error - Column winner not detected" << std::endl;
    return -5;
  }
  
  
  // test for a diagonal win
  
  game.clearGame();

  player = 1;
  location.x = 1;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 0;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 0;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 1;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 2;
  location.y = 2;
  result = game.MakeMove(player, location);
  
  if(result.status != status_winner){
    std::cout << "Error - Diagonal 1 winner not detected" << std::endl;
    return -6;
  }
  
  
  // test for other diagonal win
  
  game.clearGame();
  
  player = 1;
  location.x = 1;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 0;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 2;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 1;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 0;
  location.y = 2;
  result = game.MakeMove(player, location);
  
  if(result.status != status_winner){
    std::cout << "Error - Diagonal 2 winner not detected" << std::endl;
    return -7;
  }
  
  
  // test for draw
  
  game.clearGame();
  
  player = 1;
  location.x = 1;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 0;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 0;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 2;
  location.y = 1;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 1;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 1;
  location.y = 2;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 0;
  location.y = 2;
  game.MakeMove(player, location);
  
  player = 2;
  location.x = 2;
  location.y = 0;
  game.MakeMove(player, location);
  
  player = 1;
  location.x = 2;
  location.y = 2;
  result = game.MakeMove(player, location);
  
  if(result.status != status_draw){
    std::cout << "Error - draw not detected" << std::endl;
    return -8;
  }
  std::cout << "Test PASS" << std::endl;
 return 0;
}