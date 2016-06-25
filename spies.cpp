#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <random>
using namespace std;

#define N 8
#define tc 0

typedef vector<vector<unsigned> > Board;

volatile bool solved = false; //For stopping other threads early.

void attack(Board& board, const vector<unsigned>& pos){
    
    for (unsigned row = 0; row < N; ++row){
        
        unsigned col = pos[row];
        
        //Block out cols
        for (unsigned i = 0; i < N; ++i){
            //increment every rows column equal to this queens col, skip this row.
            if (i == row) continue;
            board[i][col]++;
        }
        
        
        //FIXME Need to check this logic for the loops. Too bored to do so.
        //Block left horizontals
        for(unsigned r = ){
            board[row + (col - x)][x]++;
        }
        //Block right horizontals
        for(int x = col; x < N && row - (col - x) < N; ++x){
            board[row - (col - x)][x]++;
        }
        
    }
    
}

void solve(Board board){
    
    //Set initial positions randomly
    vector<unsigned> pos(N);
    random_device rand;
    uniform_int_distribution<unsigned> dist(0,N);
    for (unsigned& r : pos){
        r = dist(rand);
    }
    
    //Set the attack levels on the board.
    attack(board, pos);
    
    for (auto& r : board){
        
        for (unsigned i : r){
            
            cout << i;
            
        }
        
        cout << endl;
        
    }
    
}


int main(){
    
    vector<vector<unsigned> > board(N, vector<unsigned>(N));
    
    //Start the problem a few times, one will complete first.
    vector<thread> threads(tc);
    for (unsigned i = 0; i < tc; ++i){
        threads[i] = thread(solve, board);
    }
    
    //Run it on main thread too.
    solve(board);
    
    for (thread& t : threads){
        t.join();
    }
    
    return 0;
}
