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
    // check duong cheo
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

// check all the blank is filled
bool isFillAll(State state) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (state[i][j] == _) {
                return false;
            }
        }
    }
    return true;
}

// check final state
bool isFinalState(State state)
{
    char gameState = checkGameState(state);
    if (gameState != _)
        return true;
    return isFillAll(state);
}

// get score to determine who is wins
int getScoreState(State state, char currentPlayer)
{
    char gameState = checkGameState(state);
    if (gameState == _)
        return 0;
    else
    {
        return gameState == currentPlayer ? 1 : -1; 
    }
}

// get next state
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

// calculate best move
pair<State, int> getScore(State state, bool isComputerMoves)
{
    char player = getCurrentPlayer(state);
    if (isFinalState(state))
    {
        int score = getScoreState(state, player);
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

// human movement
pair<int, int> humanMovement(State &state, char currentPlayer)
{
    cout << "Nhap toa do " << currentPlayer << " :";
    int i, j;
    cin >> i >> j;
    // check toạ độ hợp lệ
    return make_pair(i, j);
}


State turn(State state, pair<int,int> coord, char player) {
    cout << player << " move " << coord.first << " " << coord.second << endl;
    state[coord.first][coord.second] = player;
    return state;
}

// computer movement
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

// check game state and return result
bool isGameOver(State state) {
    int score = getScoreState(state, X);
    if(score != 0) {
        char winner = score > 0 ? X : O;
        cout << winner << " is winner";
        return true;
    }

    if(isFillAll(state)) {
        cout << "Draw";
        return true;
    }

    return false;
}

// check illegal move
bool checkIllegalMove(State state, pair<int, int> move) {

    if(move.first < 0 || move.second < 0 || move.first >= SIZE || move.second >= SIZE) {
        return false;
    }
    if(state[move.first][move.second] != _) {
        return false;
    }
    return true;
}

int main()
{
    State state(SIZE, vector<char>(SIZE, _));

    // state[0][2] = X; state[0][0] = O;
    // state[1][0] = X; state[2][1] = O;
    // state[2][0] = X; state[2][2] = O;

    char human = X;

    draw(state);
    while (true)
    {
        char currentPlayer = getCurrentPlayer(state);

        if(isGameOver(state)) {
            break;
        }

        pair<int, int> nextMove;

        nextMove = currentPlayer == human
                       ? humanMovement(state, currentPlayer)
                       : computerMovement(state);

        if(!checkIllegalMove(state, nextMove)) {
            cout << "Nuoc di khong hop le. Vui long di lai.\n";
            continue;
        }

        state = turn(state, nextMove, currentPlayer);
        draw(state);
    }

    return 0;
}
