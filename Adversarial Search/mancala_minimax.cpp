#include <bits/stdc++.h>
using namespace std;
const long long inf = 1e9 + 7;
int MAX_PLAYER;
int MIN_PLAYER;
int heuristic;
int w1 = 1, w2 = 2, w3 = 3, w4 = 4; // can be randomized
#define pii pair<int, int>
struct State
{
    int id, store[2], player, total_bonus_moves[2], total_captured[2], current_court;
    vector<int> court[2];
    State()
    {
        id = 0;
        for (int i = 0; i < 2; i++)
        {
            store[i] = total_bonus_moves[i] = total_captured[i] = 0;
            court[i] = vector<int>(6, 4);
        }
        player = current_court = 0;
    }
    // value of states according to heuristic
    int get_state_value(int myplayer)
    {
        int stones_in_my_side = 0, stones_in_opponents_side = 0;
        for (int i = 0; i < 6; i++)
        {
            stones_in_my_side += court[myplayer][i];
        }
        for (int i = 0; i < 6; i++)
        {
            stones_in_opponents_side += court[!myplayer][i];
        }
        int stones_in_my_storage = store[myplayer], stones_in_opponents_storage = store[!myplayer];
        if (heuristic == 1)
        {
            return stones_in_my_storage - stones_in_opponents_storage;
        }
        else if (heuristic == 2)
        {
            return w1 * (stones_in_my_storage - stones_in_opponents_storage) + w2 * (stones_in_my_side - stones_in_opponents_side);
        }
        else if (heuristic == 3)
        {
            return w1 * (stones_in_my_storage - stones_in_opponents_storage) + w2 * (stones_in_my_side - stones_in_opponents_side) + w3 * total_bonus_moves[myplayer];
        }
        else if (heuristic == 4)
        {
            return w1 * (stones_in_my_storage - stones_in_opponents_storage) + w2 * (stones_in_my_side - stones_in_opponents_side) + w3 * total_bonus_moves[myplayer] + w4 * total_captured[myplayer];
        }
        return 0;
    }
    void printState()
    {
        cout << "------------------------------------------" << endl;
        cout << "(Store 2)-------";
        cout << "(Player 2)------";
        cout << "(Store 1)" << endl;
        cout << "\t\t\t";
        for (int i = 0; i < 6; i++)
            cout << court[1][i] << " ";
        cout << endl;
        cout << "   " << store[1] << "\t\t\t\t\t\t\t" << store[0] << endl;
        cout << "\t\t\t";
        for (int i = 0; i < 6; i++)
            cout << court[0][i] << " ";
        cout << endl;
        cout << "(Store 2)-------";
        cout << "(Player 1)------";
        cout << "(Store 1)" << endl;
        cout << "------------------------------------------" << endl;
    }
};
vector<State> allStates;
vector<int> moves;
State move(State s)
{
    int current_side = s.player;
    int cur_pos = s.current_court;
    int current_stones = s.court[current_side][cur_pos];
    // take all the stones in current court and start moving
    s.court[current_side][cur_pos] = 0;
    int bonus_move = 0, captured = 0;
    for (int i = 1; i <= current_stones; i++)
    {
        if (current_side == 0)
        {
            cur_pos++;
        }
        else
            cur_pos--;
        // if this is the store of state player put stones otherwise ignore
        if (cur_pos >= 6 || cur_pos < 0)
        {

            if (current_side == s.player)
            {
                s.store[0] += cur_pos >= 6;
                s.store[1] += cur_pos < 0;
            }
            else
            {
                i--;
            }
            current_side = !current_side;
        }
        else
        {
            s.court[current_side][cur_pos]++;
            if (i == current_stones && current_side == s.player && s.court[current_side][cur_pos] == 1)
            {
                captured += s.court[!current_side][cur_pos];
                s.total_captured[s.player] += captured;
                s.court[!current_side][cur_pos] = 0;
                s.store[current_side] += captured;
            }
        }
    }
    int allzero = 1;
    for (int i = 0; i < 6; i++)
    {
        if (s.court[s.player][i])
            allzero = 0;
    }
    if (allzero)
    {
        for (int i = 0; i < 6; i++)
        {
            s.store[!s.player] += s.court[!s.player][i];
            s.court[!s.player][i] = 0;
        }
    }
    if (cur_pos >= 6 && s.player == 0)
    {
        s.total_bonus_moves[s.player] += 1;
    }
    else if (cur_pos < 0 && s.player == 1)
    {
        s.total_bonus_moves[s.player] += 1;
    }
    else
        s.player = !s.player;
    return s;
}
vector<int> getChildList(int id, int player)
{
    vector<int> ret;

    for (int curpos = 0; curpos < 6; curpos++)
    {
        State s = allStates[id];
        if (s.court[player][curpos] == 0)
            continue;
        s.current_court = curpos;
        s.player = player;
        s = move(s);
        int nid = allStates.size();
        s.id = nid;
        ret.push_back(nid);
        allStates.push_back(s);
    }
    return ret;
}
bool gameover(int pos)
{
    int over = 1;
    for (int i = 0; i < allStates[pos].court[0].size(); i++)
    {
        if (allStates[pos].court[0][i])
            over = 0;
    }
    for (int i = 0; i < allStates[pos].court[1].size(); i++)
    {
        if (allStates[pos].court[1][i])
            over = 0;
    }
    return over;
}

int minimax(int pos, int depth, int alpha, int beta, int player, int &current_court)
{
    if (depth == 0 || gameover(pos))
    {
        return allStates[pos].get_state_value(player);
    }
    if (player == MAX_PLAYER)
    {
        int maxeval = -1 * inf, mval = 0;
        int next_court = 0;
        for (int child : getChildList(pos, player))
        {
            int next_court = 0;
            int eval = minimax(child, depth - 1, alpha, beta, allStates[child].player, next_court);
            if (maxeval < eval)
            {
                current_court = allStates[child].current_court;
                maxeval = eval;
            }
            alpha = max(alpha, eval);
            if (beta <= alpha)
                break;
        }

        return maxeval;
    }
    else
    {
        int mineval = inf, mval = 0;
        for (int child : getChildList(pos, player))
        {
            int next_court = 0;
            int eval = minimax(child, depth - 1, alpha, beta, allStates[child].player, next_court);
            if (mineval > eval)
            {
                current_court = allStates[child].current_court;
                mineval = eval;
            }
            beta = min(eval, beta);
            if (beta <= alpha)
                break;
        }
        return mineval;
    }
}
void init()
{
    heuristic = 1;
    MAX_PLAYER = 0;
    MIN_PLAYER = 1;
}
int main()
{
    init();
    State s;
    allStates.push_back(s);
    s.player = 0;
    cout << "Initial Court" << endl;
    s.printState();
    int court = 0;
    while (!gameover(0))
    {
        MAX_PLAYER = s.player;
        MIN_PLAYER = !s.player;
        // set heuristic of a player to know which one is better
        if (s.player == 0)
            heuristic = 1;
        else
            heuristic = 4;
        minimax(0, 5, -inf, inf, s.player, court);
        cout << "Player " << s.player + 1 << " move from court " << court + 1 << endl;
        s.current_court = court;
        s = move(s);
        s.printState();
        allStates.clear();
        allStates.push_back(s);
    }
    return 0;
}