#include <bits/stdc++.h>

using namespace std;

const int n = 3;

map<char, int> d; // init players 'O':2, 'X':1;

void clear() {
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

/*Checking free fields*/
vector<pair<int, int>> free(char field[n][n]) {
    vector<pair<int, int>> dm;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (field[i][j] == '-') {
                dm.emplace_back(i, j);
            }
        }
    }
    if (dm.size() == 0) {
        dm.emplace_back(-1, -1);
    }
    return dm;
}


pair<bool, int> win(char field[n][n]);

pair<int, int> diagonal_win(char field[n][n], vector<pair<int, int>> diag, int playerX) {
    if (diag.size() == 0) {
        return {-1, -1};
    }
    int x = diag[0].first;
    int y = diag[0].second;
    int ax = -1, ay = -1;
    for (int i = 1; i < diag.size(); i++) {
        field[x][y] = playerX == 1 ? 'X' : 'O';
        auto res = win(field);
        if (res.first) {
            ax = x;
            ay = y;
            field[x][y] = '-';
            break;
        } else {
            field[x][y] = '-';
        }
    }
    return {ax, ay};
}


pair<int, int> line_win(char field[n][n], vector<pair<int, int>> pass, int playerX, int N) {

    int x = pass[0].first;
    int y = pass[0].second;
    int ax = -1, ay = -1;
    for (int i = 1; i < N; i++) {
        field[x][y] = playerX == 1 ? 'X' : 'O';
        auto res = win(field);
        if (res.first) {
            ax = x;
            ay = y;
            field[x][y] = '-';
            break;
        } else {
            field[x][y] = '-';
        }
        x = pass[i].first;
        y = pass[i].second;
    }
    return {ax, ay};
}

int Nx = -1, Ny = -1;

bool if_loseAI(char field[n][n], int playerX) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (field[i][j] == '-') {
                field[i][j] = playerX == 1 ? 'O' : 'X';
                auto pre = win(field);
                if (pre.first && pre.second == (playerX == 1 ? 2 : 1)) {
                    field[i][j] = '-';
                    Nx = i;
                    Ny = j;
                    return true;
                }
                field[i][j] = '-';
            }
        }
    }
    return false;
}


bool p_cnt = false;

void intelligence(char field[n][n], int playerX) {
    srand(time(0));
    int x = -1, y = -1;
    if (playerX == 1 && p_cnt == 0) { // rewrite
        x = rand() % 2;
        y = x == 1 ? 1 : 0;
        field[x][y] = 'X';
        p_cnt = true;
        return;
    }
    int N = free(field).size();
    vector<pair<int, int>> pass(N);
    vector<pair<int, int>> diag;
    for (int i = 0; i < N; i++) {
        pass[i].first = free(field)[i].first;
        pass[i].second = free(field)[i].second;
        if (pass[i].first == 0 && (pass[i].second == 0 or pass[i].second == 2)) {
            diag.emplace_back(pass[i].first, pass[i].second);
        } else if (pass[i].first == 2 && (pass[i].second == 2 or pass[i].second == 0)) {
            diag.emplace_back(pass[i].first, pass[i].second);
        } else if (pass[i].first == 1 && pass[i].second == 1) {
            diag.emplace_back(pass[i].first, pass[i].second);
        }
    }
    auto lw = line_win(field, pass, playerX, N);
    auto dw = diagonal_win(field, diag, playerX);
    if (lw.first != -1 && lw.second != -1) {
        x = lw.first;
        y = lw.second;

    } else if (dw.first != -1 && dw.second != -1) {
        x = dw.first;
        y = dw.second;

    } else if (if_loseAI(field, playerX) && Nx != -1 && Ny != -1) {
        x = Nx;
        y = Ny;

    } else {
        srand(time(0));
        pass.clear();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (field[i][j] == '-') {
                    pass.emplace_back(i, j);
                }
            }
        }
        random_shuffle(pass.begin(), pass.end());
        auto d = pass[rand() % pass.size()];
        x = d.first;
        y = d.second;
    }
    field[x][y] = playerX == 1 ? 'X' : 'O';
}


bool validator(char field[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (field[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

pair<bool, int> win(char field[n][n]) {
    if (field[0][0] == field[1][1] && field[1][1] == field[2][2] && field[2][2] != '-') {
        return {true, d[field[0][0]]};
    } else if (field[0][2] == field[1][1] && field[1][1] == field[2][0] && field[2][0] != '-') {
        return {true, d[field[1][1]]};
    }
    for (int i = 0; i < n; i++) {
        if (field[i][0] == field[i][1] && field[i][1] == field[i][2] && field[i][2] != '-') {
            return {true, d[field[i][0]]};
        } else if (field[0][i] == field[1][i] && field[1][i] == field[2][i] && field[2][i] != '-') {
            return {true, d[field[0][i]]};
        }
    }
    return {false, -1};
}

void render(char field[n][n]) {
    cout << "  0 1 2\n";
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        for (int j = 0; j < n; j++) {
            cout << field[i][j] << " ";
        }
        cout << "\n";
    }
}

void query(char field[n][n], int playerX) {
    /*doesn't work with non-digit*/
    int x, y;
    cin >> x >> y;
    while (field[x][y] != '-') {
        cout << "This field is taken! Choose another!\n";
        cin >> x >> y;
    }
    field[x][y] = playerX == 1 ? 'X' : 'O';
}

void run(int player) {
    char field[n][n];
    d['X'] = 1;
    d['O'] = 2;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            field[i][j] = '-';
        }
    }
    bool answered = false;

    while (!validator(field) && !win(field).first) {
        if (player == 2) {
            render(field);
            cout << "Player 1 is 'X'! Enter the coordinates X and Y!\n";
            query(field, 1);
            if (validator(field) or win(field).first) {
                clear();
                break;
            }
            clear();
            render(field);
            cout << "Player 2 is 'O'!\n";
            query(field, 2);
            clear();
        } else {
            int playerX;
            if (!answered) {
                cout << "Would you like to play first or second? Enter 1 or 2!\n";
                char get_;
                cin >> get_;
                playerX = get_ - '0';
                if (playerX < 1 or playerX > 2) {
                    while (playerX < 1 or playerX > 2) {
                        cout << "Incorrect player! Enter: 1 or 2\n";
                        cin >> get_;
                        playerX = get_ - '0';
                    }
                }
                answered = true;
            }
            cout << "First coordinate is a string, second - column!\n";
            if (playerX == 1) {
                render(field);
                cout << "Player 1 is YOU! Enter the coordinates X and Y!\n";
                query(field, 1);
                if (validator(field) or win(field).first) {
                    clear();
                    break;
                }
                clear();
                render(field);
                cout << "Player 2 is AI with level easy!\n";
                intelligence(field, 2);
                clear();
            } else {
                render(field);
                cout << "Player 1 is AI with level easy!\n";
                intelligence(field, 1);
                if (validator(field) or win(field).first) {
                    clear();
                    break;
                }
                clear();
                render(field);
                cout << "Player 2 is YOU! Enter the coordinates X and Y!\n";
                query(field, 2);
                clear();
            }
        }
    }
    auto result = win(field);
    if (result.first) {
        cout << "Congratulations! Player " << result.second << " wins!\n";
        cout << "GAME OVER\n";
    } else {
        cout << "Draw!\n";
        cout << "GAME OVER\n";
    }
    render(field);
}

int main() {
    /*
     * doesn't work correctly with non-digit
     */
    cout << "Enter a count of players (1 or 2):\n";
    char get_;
    cin >> get_;
    int players = get_ - '0';
    if (players < 1 or players > 2) {
        while (players < 1 or players > 2) {
            cout << "Incorrect player! Enter: 1 or 2\n";
            cin >> get_;
            players = get_ - '0';
        }
    }
    run(players);
    return 0;
}
