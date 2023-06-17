#include<iostream>
#include<fstream>
#include<ctime> 
#include<string>
using namespace std;

const int BOARD_SIZE = 8;

char kingB = 'k', queenB = 'q', rookB = 'r', bishopB = 'b', knightB = 'h', pawnB = 'p';
char kingW = 'K', queenW = 'Q', rookW = 'R', bishopW = 'B', knightW = 'H', pawnW = 'P';
char blank = ' ';
bool castle1 = true;
bool castle2 = true;

bool isCorrectPieceSelected(int player, char piece);
bool isOponentpiece(char board[][BOARD_SIZE], int player, int i, int j);
void printBoard(char board[][BOARD_SIZE]);
void initializeBoard(char board[][BOARD_SIZE]);
char getMove(char board[][BOARD_SIZE], int player, int& i, int& j, int& x, int& y, ofstream& fout);
int switchPlayer(int player);
bool pawnMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
bool rookMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
bool bishopMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
bool knightMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
bool queenMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
bool kingMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);

bool isCheckByPawn(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckByRook(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckByBishop(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckByQueen(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckByKnight(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckByKing(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheck(char board[][BOARD_SIZE], int player);


bool isCheckMateByPawn(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMateByRook(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMateByBishop(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMateByQueen(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMateByKnight(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMateByKing(char board[][BOARD_SIZE], int player, int x, int y);
bool isCheckMate(char board[][BOARD_SIZE], int player);


bool isStaleMate();

bool isCasling(char board[][BOARD_SIZE], int i, int j, int x, int y, int player);
void saveGame();
void loadMoves(char board[][BOARD_SIZE], int& player, int& i, int& j, int& x, int& y, bool flag);

// Helper Function
int oppositPlayer(int player)
{
	return player == 1 ? 2 : 1;
}

void classicalMode(char board[][BOARD_SIZE], int player, int i, int j, int x, int y) {

	ofstream fout("save.txt");
	bool end = false;
	bool valid = false;
	printBoard(board);
	clock_t player1Time = 0;
	clock_t player2Time = 0;

	while (!end)
	{
		clock_t time_req = clock();
		char piece = ' ';
		cout << "Press x to resign" << endl;
		cout << "Press y to handshake" << endl;
		piece = getMove(board, player, i, j, x, y, fout);
		if (piece == 'x' || piece == 'y') {
			if (piece == 'x')
				cout << "Player " << player << " resiged the match." << endl;
			else
				cout << "Player " << player << " handshaked." << endl;
			end = true;
		}
		else {
			if (piece == pawnB || piece == pawnW) { // if pawn is moved
				valid = pawnMovement(board, i, j, x, y, player);
			}
			else if (piece == rookB || piece == rookW) { // If rook is moved
				valid = rookMovement(board, i, j, x, y, player);
			}
			else if (piece == bishopB || piece == bishopW) {
				valid = bishopMovement(board, i, j, x, y, player);
			}
			else if (piece == knightB || piece == knightW) {
				valid = knightMovement(board, i, j, x, y, player);
			}
			else if (piece == queenB || piece == queenW) {
				valid = queenMovement(board, i, j, x, y, player);
			}
			else if (piece == kingB || piece == kingW) {
				valid = kingMovement(board, i, j, x, y, player);
			}
			time_req = clock() - time_req;
			if (player == 1) {
				player1Time += time_req;
			}
			else {
				player2Time += time_req;
			}

			printBoard(board);

			if (valid) {
				if (isCheck(board, player)) {
					cout << "\nCheck";
					if (isCheckMate(board, oppositPlayer(player))) {
						cout << " Mate";
						end = true;
					}
				}
				else if (isCheckMate(board, oppositPlayer(player))) {
					cout << "StaleMate";
					end = true;
				}
				cout << endl;
				player = switchPlayer(player);
				valid = false;
			}
			else if (isCheck(board, oppositPlayer(player))) {
				cout << "\nCheck";
				if (isCheckMate(board, player))
					cout << " Mate";
				cout << endl;
			}
		}
	}
	cout << "Time Taken by Player 1: "
		<< (float)player1Time / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Time Taken by Player 2: "
		<< (float)player2Time / CLOCKS_PER_SEC << " seconds" << endl;

}

void rapidMode(char board[][BOARD_SIZE], int player, int i, int j, int x, int y) {
	ofstream fout("save.txt");
	bool end = false;
	bool valid = false;
	printBoard(board);
	clock_t player1Time = 0;
	clock_t player2Time = 0;
	int moves1 = 0;
	int moves2 = 0;

	while (!end)
	{
		clock_t time_req = clock();
		char piece = ' ';
		cout << "Press x to resign" << endl;
		cout << "Press y to handshake" << endl;
		piece = getMove(board, player, i, j, x, y,fout);
		if (piece == 'x' || piece == 'y') {
			if (piece == 'x')
				cout << "Player " << player << " resiged the match." << endl;
			else
				cout << "Player " << player << " handshaked." << endl;
			end = true;
		}
		else {
			if (piece == pawnB || piece == pawnW) { // if pawn is moved
				valid = pawnMovement(board, i, j, x, y, player);
			}
			else if (piece == rookB || piece == rookW) { // If rook is moved
				valid = rookMovement(board, i, j, x, y, player);
			}
			else if (piece == bishopB || piece == bishopW) {
				valid = bishopMovement(board, i, j, x, y, player);
			}
			else if (piece == knightB || piece == knightW) {
				valid = knightMovement(board, i, j, x, y, player);
			}
			else if (piece == queenB || piece == queenW) {
				valid = queenMovement(board, i, j, x, y, player);
			}
			else if (piece == kingB || piece == kingW) {
				valid = kingMovement(board, i, j, x, y, player);
			}
			time_req = clock() - time_req;
			if (player == 1) {
				player1Time += time_req;
			}
			else {
				player2Time += time_req;
			}

			printBoard(board);

			if (valid) {
				if (player == 1)
					moves1++;
				else
					moves2++;

				if (isCheck(board, player)) {
					cout << "\nCheck";
					if (isCheckMate(board, oppositPlayer(player))) {
						cout << " Mate";
						end = true;
					}
				}
				else if (isCheckMate(board, oppositPlayer(player))) {
					cout << "StaleMate";
					end = true;
				}
				cout << endl;
				player = switchPlayer(player);
				valid = false;
			}
			else if (isCheck(board, oppositPlayer(player))) {
				cout << "\nCheck";
				if (isCheckMate(board, player))
					cout << " Mate";
				cout << endl;
			}
		}
		if (((float)player1Time / CLOCKS_PER_SEC) > 600.00) {
			end = true;
			cout << "Player 1 time is exceeded!" << endl;
		}
		else if (((float)player2Time / CLOCKS_PER_SEC) > 600.00) {
			end = true;
			cout << "Player 2 time is exceeded!" << endl;
		}
	}
	cout << "Time Taken by Player 1: "
		<< (float)player1Time / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Time Taken by Player 2: "
		<< (float)player2Time / CLOCKS_PER_SEC << " seconds" << endl;

}

void blitzMode(char board[][BOARD_SIZE], int player, int i, int j, int x, int y) {

	bool end = false;
	bool valid = false;
	printBoard(board);
	clock_t player1Time = 0;
	clock_t player2Time = 0;
	int moves1 = 0;
	int moves2 = 0;
	ofstream fout("save.txt");

	while (!end)
	{
		clock_t time_req = clock();
		char piece = ' ';
		cout << "Press x to resign" << endl;
		cout << "Press y to handshake" << endl;
		piece = getMove(board, player, i, j, x, y,fout);
		if (piece == 'x' || piece == 'y') {
			if (piece == 'x')
				cout << "Player " << player << " resiged the match." << endl;
			else
				cout << "Player " << player << " handshaked." << endl;
			end = true;
		}
		else {
			if (piece == pawnB || piece == pawnW) { // if pawn is moved
				valid = pawnMovement(board, i, j, x, y, player);
			}
			else if (piece == rookB || piece == rookW) { // If rook is moved
				valid = rookMovement(board, i, j, x, y, player);
			}
			else if (piece == bishopB || piece == bishopW) {
				valid = bishopMovement(board, i, j, x, y, player);
			}
			else if (piece == knightB || piece == knightW) {
				valid = knightMovement(board, i, j, x, y, player);
			}
			else if (piece == queenB || piece == queenW) {
				valid = queenMovement(board, i, j, x, y, player);
			}
			else if (piece == kingB || piece == kingW) {
				valid = kingMovement(board, i, j, x, y, player);
			}
			time_req = clock() - time_req;
			if (player == 1) {
				player1Time += time_req;
			}
			else {
				player2Time += time_req;
			}

			printBoard(board);

			if (valid) {
				if (player == 1)
					moves1++;
				else
					moves2++;

				if (isCheck(board, player)) {
					cout << "\nCheck";
					if (isCheckMate(board, oppositPlayer(player))) {
						cout << " Mate";
						end = true;
					}
				}
				else if (isCheckMate(board, oppositPlayer(player))) {
					cout << "StaleMate";
					end = true;
				}
				cout << endl;
				player = switchPlayer(player);
				valid = false;
			}
			else if (isCheck(board, oppositPlayer(player))) {
				cout << "\nCheck";
				if (isCheckMate(board, player))
					cout << " Mate";
				cout << endl;
			}
		}
		if (((float)player1Time / CLOCKS_PER_SEC) > 300.00) {
			end = true;
			cout << "Player 1 time is exceeded!" << endl;
		}
		else if (((float)player2Time / CLOCKS_PER_SEC) > 300.00) {
			end = true;
			cout << "Player 2 time is exceeded!" << endl;
		}
	}
	cout << "Time Taken by Player 1: "
		<< (float)player1Time / CLOCKS_PER_SEC << " seconds" << endl;
	cout << "Time Taken by Player 2: "
		<< (float)player2Time / CLOCKS_PER_SEC << " seconds" << endl;

}

void lighteningMode(char board[][BOARD_SIZE], int player, int i, int j, int x, int y) {

	bool end = false;
	bool valid = false;
	printBoard(board);
	clock_t player1Time = 0;
	clock_t player2Time = 0;
	int moves1 = 0;
	int moves2 = 0;
	ofstream fout("save.txt");

	while (!end)
	{
		clock_t time_req = clock();
		char piece = ' ';
		piece = getMove(board, player, i, j, x, y,fout);

		if (piece == pawnB || piece == pawnW) { // if pawn is moved
			valid = pawnMovement(board, i, j, x, y, player);
		}
		else if (piece == rookB || piece == rookW) { // If rook is moved
			valid = rookMovement(board, i, j, x, y, player);
		}
		else if (piece == bishopB || piece == bishopW) {
			valid = bishopMovement(board, i, j, x, y, player);
		}
		else if (piece == knightB || piece == knightW) {
			valid = knightMovement(board, i, j, x, y, player);
		}
		else if (piece == queenB || piece == queenW) {
			valid = queenMovement(board, i, j, x, y, player);
		}
		else if (piece == kingB || piece == kingW) {
			valid = kingMovement(board, i, j, x, y, player);
		}
		time_req = clock() - time_req;
		printBoard(board);

		if (valid) {
			if (player == 1)
				moves1++;
			else
				moves2++;

			if (isCheck(board, player)) {
				cout << "\nCheck";
				if (isCheckMate(board, oppositPlayer(player))) {
					cout << " Mate";
					end = true;
				}
			}
			else if (isCheckMate(board, oppositPlayer(player))) {
				cout << "StaleMate";
				end = true;
			}
			cout << endl;
			player = switchPlayer(player);
			valid = false;
		}
		else if (isCheck(board, oppositPlayer(player))) {
			cout << "\nCheck";
			if (isCheckMate(board, player))
				cout << " Mate";
			cout << endl;
		}
		if (((float)time_req / CLOCKS_PER_SEC) > 15.00) {
			end = true;
			if(player == 2)
				cout << "Player 1 time is exceeded!" << endl;
			else
				cout << "Player 2 time is exceeded!" << endl;
		}
	}
}

void menu(char board[][BOARD_SIZE], int player, int i, int j, int x, int y) {
	cout << "Press 1: To load Moves for Evaluation" << endl;
	cout << "Press 2: To load Game" << endl;
	cout << "Press 3: Classic Mode" << endl;
	cout << "Press 4: Rapid Mode" << endl;
	cout << "Press 5: Blitz Mode" << endl;
	cout << "Press 6: Lightening" << endl;
	int op = 0;
	cin >> op;
	system("cls");
	string input = "";
	getline(cin, input, '\n');
	if (op == 1) {
		loadMoves(board, player, i, j, x, y, true);
	}
	else if (op == 2) {
		loadMoves(board, player, i, j, x, y, false);
		classicalMode(board, player, i, j, x, y);
	}
	else if (op == 3) {
		classicalMode(board, player, i, j, x, y);
	}
	else if (op == 4) {
		rapidMode(board, player, i, j, x, y);
	}
	else if (op == 5) {
		blitzMode(board, player, i, j, x, y);
	}
	else if (op == 6) {
		lighteningMode(board, player, i, j, x, y);
	}
	
}

int main()
{
	int i = 0, j = 0, x = 0, y = 0;
	int player = 1; // player 2 is black and player 1 is white
	char board[BOARD_SIZE][BOARD_SIZE] = { blank };
	initializeBoard(board);
	menu(board, player, i, j, x, y);
	system("PAUSE");
	return 0;
}

void initializeBoard(char board[][BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			board[i][j] = blank;

	/*for (int i = 0; i < BOARD_SIZE; i++)
		board[1][i] = pawnB;*/
	for (int i = 0; i < BOARD_SIZE; i++)
		board[6][i] = pawnW;

	//board[0][0] = rookB;
	//board[0][7] = rookB;
	//board[7][0] = rookW;
	//board[7][7] = rookW;

	//board[0][1] = knightB;
	//board[0][6] = knightB;
	//board[7][1] = knightW;
	//board[7][6] = knightW;

	//board[0][2] = bishopB;
	//board[0][5] = bishopB;
	//board[7][2] = bishopW;
	//board[7][5] = bishopW;

	//board[0][4] = kingB;
	//board[7][4] = kingW;

	board[0][3] = queenB;
	//board[7][3] = queenW;

	//
}

bool isCorrectPieceSelected(int player, char piece) {
	if (player == 2) {
		return piece == kingB || piece == queenB || piece == rookB ||
			piece == knightB || piece == pawnB || piece == bishopB;
	}
	else if (player == 1) {
		return piece == kingW || piece == queenW || piece == rookW ||
			piece == knightW || piece == pawnW || piece == bishopW;
	}
	return false;
}

bool isOponentpiece(char board[][BOARD_SIZE], int player, int i, int j) {
	if (player == 1) {
		return board[i][j] == kingB || board[i][j] == queenB || board[i][j] == rookB ||
			board[i][j] == knightB || board[i][j] == pawnB || board[i][j] == bishopB;
	}
	else if (player == 2) {
		return board[i][j] == kingW || board[i][j] == queenW || board[i][j] == rookW ||
			board[i][j] == knightW || board[i][j] == pawnW || board[i][j] == bishopW;
	}
	return false;
}

void printBoard(char board[][BOARD_SIZE]) {
	// system("cls");
	cout << "     a  " << " b  " << " c  " << " d  " << " e  " << " f  " << " g  " << " h  " << endl;
	cout << "   ---------------------------------" << endl;
	for (int i = 0, count = BOARD_SIZE; i < BOARD_SIZE; i++, count--)
	{
		cout << count << "  | ";
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			cout << board[i][j] << " | ";
		}
		cout << " " << count << endl;
		cout << "   ---------------------------------" << endl;
	}
	cout << "     a  " << " b  " << " c  " << " d  " << " e  " << " f  " << " g  " << " h  " << endl;
}

char getMove(char board[][BOARD_SIZE], int player, int& i, int& j, int& x, int& y, ofstream& fout) {
	// assuming he/she will enter the correct input!!
	cout << "Player " << player << " turn: ";
	string input = "";
	getline(cin, input, '\n');
	fout << input << endl;
	if (input == "x" || input == "y")
		return input[0];

	// a7 a5
	if (player == 2) {
		i = 7 - (input[1] - 49);
		j = input[0] - 97;
		x = 7 - (input[3] - 49);
		y = input[2] - 97;
	}
	else {
		// player 1 move
		i = 8 - (input[1] - 48);
		j = input[0] - 97;
		x = 8 - (input[3] - 48);
		y = input[2] - 97;
	}
	return board[i][j];
}

int switchPlayer(int player) {
	if (player == 1)
		return 2;
	return 1;
}

bool pawnMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {

	bool valid = false;
	// Black
	if (player == 2) {
		if (board[i][j] == pawnB) {
			if (j == y) { // If straight movement
				// Only moves if there is no piece in the path and the jump is of one box
				if (i - x == -1 && board[x][y] == blank)
				{
					valid = true;
				}
				// Only moves if there is no piece in the path and the jump is of two boxes
				// and the piece should be at the original position
				else if (i - x == -2 && i == 1 && board[x][y] == blank)
				{
					valid = true;
				}
			}
			// If movement is in cross left or right then 
			// only move when the oponent piece is in path 
			else if ((j - y == 1 || j - y == -1) && isOponentpiece(board, player, x, y)) {
				valid = true;
			}
		}
		if (valid) {
			char current = board[i][j];
			char next = board[x][y];

			board[x][y] = board[i][j];
			board[i][j] = blank;
			if (isCheck(board, 1)) {
				board[i][j] = current;
				board[x][y] = next;
				valid = false;
			}
			else {
				if (x == 7) {
					cout << "Press 1: for queen" << endl;
					cout << "Press 2: for bishop" << endl;
					cout << "Press 3: for rook" << endl;
					cout << "Press 4: for knight" << endl;
					int input = 0;
					cin >> input;
					string temp = "";
					getline(cin, temp, '\n');
					if (input == 1)
						board[x][y] = queenB;
					else if (input == 2)
						board[x][y] = bishopB;
					else if (input == 3)
						board[x][y] = rookB;
					else if (input == 4)
						board[x][y] = knightB;
				}
			}
		}
	}
	else // White
	{
		if (board[i][j] == pawnW) {
			if (j == y) { // If straight movement
				// Only moves if there is no piece in the path and the jump is of one box
				if (i - x == 1 && board[x][y] == blank)  // also no check to king
				{
					valid = true;
				}
				// Only moves if there is no piece in the path and the jump is of two boxes
				// and the piece should be at the original position
				else if (i - x == 2 && i == 6 && board[x][y] == blank) // also no check to king
				{
					valid = true;
				}
			}
			// If movement is in cross left or right then 
			// only move when the oponent piece is in path 
			else if ((j - y == 1 || j - y == -1) && isOponentpiece(board, player, x, y)) {
				valid = true;
			}
		}
		if (valid) {
			char current = board[i][j];
			char next = board[x][y];

			board[x][y] = board[i][j];
			board[i][j] = blank;
			if (isCheck(board, 2)) {
				board[i][j] = current;
				board[x][y] = next;
				valid = false;
			}
			else {
				if (x == 0) {
					cout << "Press 1: for queen" << endl;
					cout << "Press 2: for bishop" << endl;
					cout << "Press 3: for rook" << endl;
					cout << "Press 4: for knight" << endl;
					int input = 0;
					cin >> input;
					string temp = "";
					getline(cin, temp, '\n');
					if (input == 1)
						board[x][y] = queenW;
					else if (input == 2)
						board[x][y] = bishopW;
					else if (input == 3)
						board[x][y] = rookW;
					else if (input == 4)
						board[x][y] = knightW;
				}
			}

		}
	}
	return valid;
}

bool rookMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {
	if (isCorrectPieceSelected(player, board[i][j]))
	{
		if (j == y) // Up/Down 
		{
			if (i > x) // Upward Movement
			{
				for (int v = i - 1; v > x; v--) {
					if (board[v][y] != blank) {
						return false;
					}
				}
			}
			if (i < x) // Downward Movement
			{
				for (int v = i + 1; v < x; v++) {
					if (board[v][y] != blank) {
						return false;
					}
				}
			}
		}
		else if (i == x) // Left/Right 
		{
			if (j > y) // Left Movement
			{
				for (int v = j - 1; v > y; v--) {
					if (board[x][v] != blank) {
						return false;
					}
				}
			}
			if (j < y) // Right Movement
			{
				for (int v = j + 1; v < y; v++) {
					if (board[x][v] != blank) {
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}
		if (isOponentpiece(board, player, x, y) || board[x][y] == blank) {

			char current = board[i][j];
			char next = board[x][y];
			board[x][y] = board[i][j];
			board[i][j] = blank;
			if (isCheck(board, oppositPlayer(player))) {
				board[i][j] = current;
				board[x][y] = next;
				return false;
			}
			return true;
		}
	}
	return false;
}

bool bishopMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {
	if (isCorrectPieceSelected(player, board[i][j]))
	{
		if (y > j && x < i) // Right-Upward
		{
			if ((y - j) == (i - x)) {
				for (int a = i - 1, b = j + 1; a > x; a--, b++) {
					if (board[a][b] != blank)
						return false;
				}
			}
			else
				return false;

		}
		else if (y < j && x < i) // Left-Upward
		{
			if ((j - y) == (i - x)) {
				for (int a = i - 1, b = j - 1; a > x; a--, b--) {
					if (board[a][b] != blank)
						return false;
				}
			}
			else
				return false;

		}
		else if (y > j && x > i) // Right-Downward
		{
			if ((y - j) == (x - i)) {
				for (int a = i + 1, b = j + 1; a < x; a++, b++) {
					if (board[a][b] != blank)
						return false;
				}
			}
			else
				return false;
		}
		else if (y < j && x > i) // Left-Downward
		{
			if ((j - y) == (x - i)) {
				for (int a = i + 1, b = j - 1; a < x; a++, b--) {
					if (board[a][b] != blank)
						return false;
				}
			}
			else
				return false;
		}
		else {
			return false;
		}
		if (isOponentpiece(board, player, x, y) || board[x][y] == blank) {
			char current = board[i][j];
			char next = board[x][y];
			board[x][y] = board[i][j];
			board[i][j] = blank;
			player = player == 1 ? 2 : 1;
			if (isCheck(board, player)) {
				board[i][j] = current;
				board[x][y] = next;
				return false;
			}
			return true;
		}
	}
	return false;
}

bool knightMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {
	if (isCorrectPieceSelected(player, board[i][j]))
	{
		if ((i - 2 == x && j + 1 == y) || (i - 2 == x && j - 1 == y) || (i + 2 == x && j + 1 == y) || (i + 2 == x && j - 1 == y)
			|| (i - 1 == x && j + 2 == y) || (i - 1 == x && j - 2 == y) || (i + 1 == x && j + 2 == y) || (i + 1 == x && j - 2 == y)) {
			if (isOponentpiece(board, player, x, y) || board[x][y] == blank) {
				char current = board[i][j];
				char next = board[x][y];
				board[x][y] = board[i][j];
				board[i][j] = blank;
				if (isCheck(board, oppositPlayer(player))) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
				return true;
			}
		}
	}
	return false;
}

bool queenMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) { // Complete
	return isCorrectPieceSelected(player, board[i][j]) &&
		(rookMovement(board, i, j, x, y, player) || bishopMovement(board, i, j, x, y, player));
}

bool kingMovement(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {
	if (isCorrectPieceSelected(player, board[i][j]) && (abs(i - x) <= 1 && abs(j - y) <= 1)) {
		if (isOponentpiece(board, player, x, y) || board[x][y] == blank) {
			char current = board[i][j];
			char next = board[x][y];
			board[x][y] = board[i][j];
			board[i][j] = blank;
			if (isCheck(board, oppositPlayer(player))) {
				board[i][j] = current;
				board[x][y] = next;
				return false;
			}
			if (player == 1)
				castle1 = false;
			else
				castle2 = false;
			return true;
		}
	}
	if (isCorrectPieceSelected(player, board[i][j])) {
		return isCasling(board, i, j, x, y, player);
	}
	return false;
}

bool isCheckByPawn(char board[][BOARD_SIZE], int player, int x, int y) {

	if (player == 1) {
		if (board[x][y] != pawnW)
			return false;

		if (x != 0) {
			if (y != 0 && y != BOARD_SIZE - 1) {
				if (board[x - 1][y + 1] == kingB || board[x - 1][y - 1] == kingB)
					return true;
			}
			else if (y == 0) {
				if (board[x - 1][y + 1] == kingB)
					return true;
			}
			else if (y == BOARD_SIZE - 1) {
				if (board[x - 1][y - 1] == kingB)
					return true;
			}
		}
	}
	else if (player == 2) {
		if (board[x][y] != pawnB)
			return false;

		if (x != BOARD_SIZE - 1) {
			if (y != 0 && y != BOARD_SIZE - 1) {
				if (board[x + 1][y + 1] == kingB || board[x + 1][y - 1] == kingB)
					return true;
			}
			else if (y == 0) {
				if (board[x + 1][y + 1] == kingB)
					return true;
			}
			else if (y == BOARD_SIZE - 1) {
				if (board[x + 1][y - 1] == kingB)
					return true;
			}
		}
	}
	return false;
}

bool isCheckByRook(char board[][BOARD_SIZE], int player, int x, int y, bool isQueen = false) {

	char blackPiece = rookB;
	char whitePiece = rookW;
	if (isQueen) {
		blackPiece = queenB;
		whitePiece = queenW;
	}

	char king = ' ';
	if (player == 1) {
		king = kingB;
		if (board[x][y] != whitePiece)
			return false;
	}
	else {
		king = kingW;
		if (board[x][y] != blackPiece)
			return false;
	}

	// According to white
	// up
	for (int i = x - 1; i >= 0; i--) {
		if (board[i][y] == king)
			return true;
		else if (board[i][y] != blank)
			break;
	}
	// down
	for (int i = x + 1; i < BOARD_SIZE; i++) {
		if (board[i][y] == king)
			return true;
		else if (board[i][y] != blank)
			break;
	}
	// left
	for (int i = y - 1; i >= 0; i--) {
		if (board[x][i] == king)
			return true;
		else if (board[x][i] != blank)
			break;
	}
	// right
	for (int i = y + 1; i < BOARD_SIZE; i++) {
		if (board[x][i] == king)
			return true;
		else if (board[x][i] != blank)
			break;
	}
	return false;
}

bool isCheckByBishop(char board[][BOARD_SIZE], int player, int x, int y, bool isQueen = false) {

	char blackPiece = bishopB;
	char whitePiece = bishopW;

	if (isQueen) {
		blackPiece = queenB;
		whitePiece = queenW;
	}

	char king = ' ';
	if (player == 1) {
		king = kingB;
		if (board[x][y] != whitePiece)
			return false;
	}
	else {
		king = kingW;
		if (board[x][y] != blackPiece)
			return false;
	}
	// According to white
	// upward-right
	for (int i = x - 1, j = y + 1; i >= 0 && j < BOARD_SIZE; i--, j++) {
		if (board[i][j] == king)
			return true;
		else if (board[i][j] != blank)
			break;
	}
	// upward-left
	for (int i = x - 1, j = y - 1; i >= 0 && j >= 0; i--, j--) {
		if (board[i][j] == king)
			return true;
		else if (board[i][j] != blank)
			break;
	}
	// downward-right
	for (int i = x + 1, j = y + 1; i < BOARD_SIZE && j < BOARD_SIZE; i++, j++) {
		if (board[i][j] == king)
			return true;
		else if (board[i][j] != blank)
			break;
	}
	// downward-left
	for (int i = x + 1, j = y - 1; i < BOARD_SIZE && j >= 0; i++, j--) {
		if (board[i][j] == king)
			return true;
		else if (board[i][j] != blank)
			break;
	}
	return false;
}

bool isCheckByQueen(char board[][BOARD_SIZE], int player, int x, int y) {
	return isCheckByRook(board, player, x, y, true) || isCheckByBishop(board, player, x, y, true);
}

bool isCheckByKnight(char board[][BOARD_SIZE], int player, int x, int y) {
	char king = ' ';
	if (player == 1) {
		king = kingB;
		if (board[x][y] != knightW)
			return false;
	}
	else {
		king = kingW;
		if (board[x][y] != knightB)
			return false;
	}

	int i = x - 2;
	int j = y + 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	i = x - 2;
	j = y - 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	i = x + 2;
	j = y + 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	i = x + 2;
	j = y - 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}
	return false;
}

bool isCheckByKing(char board[][BOARD_SIZE], int player, int x, int y) {
	char king = ' ';
	if (player == 1) {
		king = kingB;
		if (board[x][y] != kingW)
			return false;
	}
	else {
		king = kingW;
		if (board[x][y] != kingB)
			return false;
	}

	// up
	int i = x - 1;
	int j = y;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// up-left
	i = x - 1;
	j = y - 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// up-right
	i = x - 1;
	j = y + 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// left
	i = x;
	j = y - 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// right
	i = x;
	j = y + 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// down
	i = x + 1;
	j = y;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}


	// down-left
	i = x + 1;
	j = y - 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}

	// down-right
	i = x + 1;
	j = y + 1;
	if ((i >= 0 && i < BOARD_SIZE) && (j >= 0 && j < BOARD_SIZE)) {
		if (board[i][j] == king)
			return true;
	}
	return false;
}

bool isCheck(char board[][BOARD_SIZE], int player) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (isCheckByPawn(board, player, i, j) || isCheckByRook(board, player, i, j, false) || isCheckByBishop(board, player, i, j, false)
				|| isCheckByQueen(board, player, i, j) || isCheckByKnight(board, player, i, j) || isCheckByKing(board, player, i, j)) {
				return true;
			}
		}
	}
	return false;
}

bool isStaleMate() {
	return true;
}

void saveGame() {

}

bool isCheckMate(char board[][BOARD_SIZE], int player) {
	bool check1 = true;
	bool check2 = true;
	bool check3 = true;
	bool check4 = true;
	bool check5 = true;
	bool check6 = true;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((board[i][j] == pawnW && player == 1) || (board[i][j] == pawnB && player == 2)) {
				check1 = isCheckMateByPawn(board, player, i, j);
				if (!check1)
					return false;
			}
			if ((board[i][j] == rookW && player == 1) || (board[i][j] == rookB && player == 2)) {
				check2 = isCheckMateByRook(board, player, i, j);
				if (!check2)
					return false;
			}
			if ((board[i][j] == bishopW && player == 1) || (board[i][j] == bishopB && player == 2)) {
				check3 = isCheckMateByBishop(board, player, i, j);
				if (!check3)
					return false;
			}
			if ((board[i][j] == queenW && player == 1) || (board[i][j] == queenB && player == 2)) {
				check4 = isCheckMateByQueen(board, player, i, j);
				if (!check4)
					return false;
			}
			if ((board[i][j] == knightW && player == 1) || (board[i][j] == knightB && player == 2)) {
				check5 = isCheckMateByKnight(board, player, i, j);
				if (!check5)
					return false;
			}
			if ((board[i][j] == kingW && player == 1) || (board[i][j] == kingB && player == 2)) {
				check6 = isCheckMateByKing(board, player, i, j);
				if (!check6)
					return false;
			}
		}
	}
	return true;
}

bool isCheckMateByPawn(char board[][BOARD_SIZE], int player, int i, int j) {

	if (player == 1) {
		if (i != 0) {
			if (j != 0 && j != BOARD_SIZE - 1) {
				int x = i - 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i - 1; //up-left
				y = j - 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i - 1; //up-right
				y = j + 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
			else if (j == 0) {
				int x = i - 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
				x = i - 1; //up-right
				y = j + 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
			else if (j == BOARD_SIZE - 1) {
				int x = i - 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i - 1; //up-left
				y = j - 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
		}
	}
	else if (player == 2) {
		if (i != BOARD_SIZE - 1) {
			if (j != 0 && j != BOARD_SIZE - 1) {
				int x = i + 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i + 1; //up-left
				y = j - 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i + 1; //up-right
				y = j + 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
			else if (j == 0) {
				int x = i + 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
				x = i + 1; //up-right
				y = j + 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
			else if (j == BOARD_SIZE - 1) {
				int x = i + 1; //up
				int y = j;
				char current = board[i][j];
				char next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}

				x = i + 1; //up-left
				y = j - 1;
				current = board[i][j];
				next = board[x][y];
				if (pawnMovement(board, i, j, x, y, player)) {
					board[i][j] = current;
					board[x][y] = next;
					return false;
				}
			}
		}
	}
	return true;
}

bool isCheckMateByRook(char board[][BOARD_SIZE], int player, int i, int j) {

	// According to white
	// up
	for (int x = i - 1, y = j; x >= 0; x--) {
		char current = board[i][j];
		char next = board[x][y];
		if (rookMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// down
	for (int x = i + 1, y = j; x < BOARD_SIZE; x++) {
		char current = board[i][j];
		char next = board[x][y];
		if (rookMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// left
	for (int x = i, y = j - 1; y >= 0; y--) {
		char current = board[i][j];
		char next = board[x][y];
		if (rookMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// right
	for (int x = i, y = j + 1; y < BOARD_SIZE; y++) {
		char current = board[i][j];
		char next = board[x][y];
		if (rookMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	return true;
}

bool isCheckMateByBishop(char board[][BOARD_SIZE], int player, int i, int j) {

	// According to white
	// upward-right
	for (int x = i - 1, y = j + 1; x >= 0 && y < BOARD_SIZE; x--, y++) {
		char current = board[i][j];
		char next = board[x][y];
		if (bishopMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// upward-left
	for (int x = i - 1, y = j - 1; x >= 0 && y >= 0; x--, y--) {
		char current = board[i][j];
		char next = board[x][y];
		if (bishopMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// downward-right
	for (int x = i + 1, y = j + 1; x < BOARD_SIZE && y < BOARD_SIZE; x++, y++) {
		char current = board[i][j];
		char next = board[x][y];
		if (bishopMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	// downward-left
	for (int x = i + 1, y = j - 1; x < BOARD_SIZE && y >= 0; x++, y--) {
		char current = board[i][j];
		char next = board[x][y];
		if (bishopMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	return true;
}

bool isCheckMateByQueen(char board[][BOARD_SIZE], int player, int x, int y) {
	return isCheckMateByRook(board, player, x, y) || isCheckMateByBishop(board, player, x, y);
}

bool isCheckMateByKnight(char board[][BOARD_SIZE], int player, int i, int j) {
	int x = i - 2;
	int y = j + 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i - 2;
	y = j - 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i + 2;
	y = j + 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i + 2;
	y = j - 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i - 1;
	y = j + 2;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i - 1;
	y = j - 2;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i + 1;
	y = j + 2;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	x = i + 1;
	y = j - 2;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (knightMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	return true;
}

bool isCheckMateByKing(char board[][BOARD_SIZE], int player, int i, int j) {
	// up
	int x = i - 1;
	int y = j;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// up-left
	x = i - 1;
	y = j - 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// up-right
	x = i - 1;
	y = j + 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// left
	x = i;
	y = j - 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// right
	x = i;
	y = j + 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// down
	x = i + 1;
	y = j;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}


	// down-left
	x = i + 1;
	y = j - 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}

	// down-right
	x = i + 1;
	y = j + 1;
	if ((x >= 0 && x < BOARD_SIZE) && (y >= 0 && y < BOARD_SIZE)) {
		char current = board[i][j];
		char next = board[x][y];
		if (kingMovement(board, i, j, x, y, player)) {
			board[i][j] = current;
			board[x][y] = next;
			return false;
		}
	}
	return true;
}

bool isCasling(char board[][BOARD_SIZE], int i, int j, int x, int y, int player) {

	if (player == 1 && i == 7 && j == 4 && x == 7 && y == 6 &&
		board[7][4] == kingW && board[7][7] == rookW && board[7][6] == blank && board[7][5] == blank && castle1) {

		board[7][5] = board[7][4];
		board[7][4] = ' ';
		if (!isCheck(board, oppositPlayer(player))) {
			board[7][6] = board[7][5];
			board[7][5] = ' ';
			if (!isCheck(board, oppositPlayer(player))) {
				board[7][5] = board[7][7];
				board[7][7] = ' ';
				return true;
			}
			else {
				board[7][4] = kingW;
				board[7][5] = ' ';
				board[7][6] = ' ';
			}
		}
		else {
			board[7][4] = kingW;
			board[7][5] = ' ';
			board[7][6] = ' ';
		}
	}
	else if (
		player == 1 &&
		i == 7 &&
		j == 4 &&
		x == 7 &&
		y == 2 &&
		board[7][4] == kingW &&
		board[7][0] == rookW &&
		board[7][1] == blank &&
		board[7][2] == blank &&
		board[7][3] == blank &&
		castle1) {

		board[7][3] = board[7][4];
		board[7][4] = ' ';
		if (!isCheck(board, oppositPlayer(player))) {
			board[7][2] = board[7][3];
			board[7][3] = ' ';
			if (!isCheck(board, oppositPlayer(player))) {
				board[7][3] = board[7][0];
				board[7][0] = ' ';
				return true;
			}
			else {
				board[7][4] = kingW;
				board[7][1] = ' ';
				board[7][2] = ' ';
				board[7][3] = ' ';
			}
		}
		else {
			board[7][4] = kingW;
			board[7][1] = ' ';
			board[7][2] = ' ';
			board[7][3] = ' ';
		}
	}
	else if (
		player == 2 &&
		i == 0 &&
		j == 4 &&
		x == 0 &&
		y == 6 &&
		board[0][4] == kingB &&
		board[0][7] == rookB &&
		board[0][6] == blank &&
		board[0][5] == blank &&
		castle2) {

		board[0][5] = board[0][4];
		board[0][4] = ' ';
		if (!isCheck(board, oppositPlayer(player))) {
			board[0][6] = board[0][5];
			board[0][5] = ' ';
			if (!isCheck(board, oppositPlayer(player))) {
				board[0][5] = board[0][7];
				board[0][7] = ' ';
				return true;
			}
			else {
				board[0][4] = kingW;
				board[0][5] = ' ';
				board[0][6] = ' ';
			}
		}
		else {
			board[0][4] = kingW;
			board[0][5] = ' ';
			board[0][6] = ' ';
		}
	}
	else if (
		player == 2 &&
		i == 0 &&
		j == 4 &&
		x == 0 &&
		y == 2 &&
		board[0][4] == kingB &&
		board[0][0] == rookB &&
		board[0][1] == blank &&
		board[0][2] == blank &&
		board[0][3] == blank &&
		castle2) {

		board[0][3] = board[0][4];
		board[0][4] = ' ';
		if (!isCheck(board, oppositPlayer(player))) {
			board[0][2] = board[0][3];
			board[0][3] = ' ';
			if (!isCheck(board, oppositPlayer(player))) {
				board[0][3] = board[0][0];
				board[0][0] = ' ';
				return true;
			}
			else {
				board[0][4] = kingB;
				board[0][1] = ' ';
				board[0][2] = ' ';
				board[0][3] = ' ';
			}
		}
		else {
			board[0][4] = kingB;
			board[0][1] = ' ';
			board[0][2] = ' ';
			board[0][3] = ' ';
		}

		return false;
	}
	return false;
}

void loadMoves(char board[][BOARD_SIZE], int& player, int& i, int& j, int& x, int& y, bool flag) {

	string file = "";

	if (flag) {
		file = "moves.txt";
	}
	else {
		file = "save.txt";
	}
	ifstream fin(file);
	bool valid = false;
	int count = 0;
	// assuming he/she will enter the correct input!!

	if (fin) {
		string input = "";
		while (!fin.eof()) {
			count++;
			getline(fin, input, '\n');
			if (input.length() == 4) {
				// a7 a5
				if (player == 2) {
					i = 7 - (input[1] - 49);
					j = input[0] - 97;
					x = 7 - (input[3] - 49);
					y = input[2] - 97;
				}
				else {
					// player 1 move
					i = 8 - (input[1] - 48);
					j = input[0] - 97;
					x = 8 - (input[3] - 48);
					y = input[2] - 97;
				}
				char piece = board[i][j];

				if (piece == pawnB || piece == pawnW) { // if pawn is moved
					valid = pawnMovement(board, i, j, x, y, player);
				}
				else if (piece == rookB || piece == rookW) { // If rook is moved
					valid = rookMovement(board, i, j, x, y, player);
				}
				else if (piece == bishopB || piece == bishopW) {
					valid = bishopMovement(board, i, j, x, y, player);
				}
				else if (piece == knightB || piece == knightW) {
					valid = knightMovement(board, i, j, x, y, player);
				}
				else if (piece == queenB || piece == queenW) {
					valid = queenMovement(board, i, j, x, y, player);
				}
				else if (piece == kingB || piece == kingW) {
					valid = kingMovement(board, i, j, x, y, player);
				}
				printBoard(board);

				if (valid) {
					if (isCheck(board, player)) {
						cout << "\nCheck";
						if (isCheckMate(board, oppositPlayer(player))) {
							cout << " Mate";
						}
					}
					else if (isCheckMate(board, oppositPlayer(player))) {
						cout << "StaleMate";
					}
					cout << endl;
					player = switchPlayer(player);
					valid = false;
				}
				else if (isCheck(board, oppositPlayer(player))) {
					cout << "\nCheck";
					if (isCheckMate(board, player))
						cout << " Mate";
					cout << endl;
				}
			}
		}
	}
}