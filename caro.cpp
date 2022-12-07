// 3x3 tic tac toe
// 1) Ai engine
//     + State representation : vector<vector<char>> (3x3)
//     + State game
//     + checking if game is end
//     + detect winner if game end
//     + render (text screen)
//     + valid moves
//         + detect current player
//         + calculate valid moves
//     + calculate state getScore
//         + final state score
//         + not final state score
//     + caculate best move
// 2) User interface
//     + asking X or O
//     + input move cordinate start from 0
//     + respond if illegal move
//     + respond game result when end
//     + print current game
//     + print machine moves

#include <iostream>
#include <vector>
using namespace std;

#define X 'x'
#define O 'o'
#define _ ' '
#define SIZE 3

// State representation
typedef vector<vector<char> > State;
// cho phép người dùng thêm tên mới cho kiểu có sẵn với cú pháp như sau:
// typedef key_word new_name_use;

void draw(State state)
{
    cout << endl;
    // system("clear");
    for (int i = 0; i < SIZE; i++)
    {
        cout << '|';
        for (int j = 0; j < SIZE; j++)
        {
            cout << state[i][j] << '|';
        }
        cout << endl;
    }
}

// State game
char checkGameState(State state)
{
    // check row
    for (int i = 0; i < SIZE; i++)
    {
        if (state[i][0] != _ && state[i][0] == state[i][1] && state[i][1] == state[i][2])
        {
            return state[i][0];
        }
    }
    // check col
    for (int j = 0; j < SIZE; j++)
    {
        if (state[0][j] != _ && state[0][j] == state[1][j] && state[1][j] == state[2][j])
        {
            return state[0][j];
        }
    }
    // chck duong cheo
    if (state[0][0] != _ && state[0][0] == state[1][1] && state[0][0] == state[2][2])
    {
        return state[0][0];
    }
    if (state[0][2] != _ && state[0][2] == state[1][1] && state[0][2] == state[2][0])
    {
        return state[0][2];
    }
    return _;
}

// checking if game is end
// input: gameState
// output: true if game end, else false
bool checkFinalState(State state)
{
    char winner = checkGameState(state);
    if (winner == _)
    {
        return false;
    }
    // cout << "Người chơi: " << winner << " chiến thắng" << endl;
    return true;
}

// detect current player
char getCurrentPlayer(State state)
{
    int countX = 0, countO = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (state[i][j] == X)
                ++countX;
            else if (state[i][j] == O)
                ++countO;
        }
    }
    if (countX == countO)
        return X;
    return O;
}

bool isFinalState(State state)
{
    char gameState = checkGameState(state);

    if (gameState != _)
        return true;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (state[i][j] == _)
                return false;
        }
    }
    return true;
}

int getScoreFinalState(State state, char currentPlayer)
{
    char gameState = checkGameState(state);
    if (gameState == _)
        return 0;
    else
    {
        return gameState == currentPlayer ? 1 : -1; 
    }
}

vector<State> getNextState(State state, char nextPlayer)
{
    vector<State> nextState;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (state[i][j] == _)
            {
                State ns(state);
                ns[i][j] = nextPlayer;
                nextState.push_back(ns);
            }
        }
    }
    return nextState;
}

int db = 0;
// calculate best move
pair<State, int> getScore(State state, bool isComputerMoves)
{

    int t = db;
    db = 1;
    char player = getCurrentPlayer(state);
    if (isFinalState(state))
    {
        int score = getScoreFinalState(state, player);
        return make_pair(state, isComputerMoves ? score * -1 : score);
    }

    vector<State> nextState = getNextState(state, player);

    int bestScore = isComputerMoves ? 10 : -10;
    State bestState;

    for(int i = 0; i < nextState.size(); i++) {
        if(isComputerMoves) {
            // tim min cua cac nextState
            pair<State, int> current = getScore(nextState[i], false);
            if(current.second < bestScore) {
                bestScore = current.second;
                bestState = nextState[i];
            }

        } else {
            // tim max
            pair<State, int> current = getScore(nextState[i], true);  
            if(current.second > bestScore) {
                bestScore = current.second;
                bestState = nextState[i];
            }
        }
    }

   
    return make_pair(bestState, bestScore);

}

pair<int, int> humanMovement(State &state, char currentPlayer)
{
    cout << "Nhập toạ độ " << currentPlayer << " :";
    int i, j;
    cin >> i >> j;
    // check toạ độ hợp lệ
    return make_pair(i, j);
}

State turn(State state, pair<int,int> coord, char value) {
    state[coord.first][coord.second] = value;
    return state;
}

pair<int, int> computerMovement(State state)
{
    pair<State, int> p = getScore(state, true);
    State newState = p.first;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (newState[i][j] != state[i][j])
                return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

int main()
{
    State state(SIZE, vector<char>(SIZE, _));

    // state[0][2] = X; state[0][0] = O;
    // state[1][0] = X; state[2][1] = O;
    // state[2][0] = X; state[2][2] = O;

    char human = X;

    while (true)
    {
        // gameLoop
        // ve ban co
        draw(state);
        // kiem tra trang thai của game; thắng/thua || tiếp tục
        if (isFinalState(state))
        {
            cout << "End game";
            break;
        }
        char currentPlayer = getCurrentPlayer(state);
        pair<int, int> nextMove;

        nextMove = currentPlayer == human
                       ? humanMovement(state, currentPlayer)
                       : computerMovement(state);

        if(nextMove.first == -1 || nextMove.second == -1) {
            break;
        }

        state = turn(state, nextMove, currentPlayer);
    }

    return 0;
}