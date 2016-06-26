#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <random>
using namespace std;

#define N 8
#define tc 0
#define maxMoves 25

typedef vector<vector<unsigned> > Board;

volatile bool solved = false; //For stopping other threads early.

//Board access: [row][column]

inline unsigned gcd( int x, int y ){
    if( x < y ) swap( x, y );

    while( y > 0 ){
        const int f = x % y;
        x = y;
        y = f;
    }
    return x;
}  

void attack(Board& board, const vector<unsigned>& pos){
    
    for (unsigned row = 0; row < N; ++row){
        
        const unsigned col = pos[row];
        
        //Block out cols
        for (unsigned i = 0; i < N; ++i){
            //increment every rows column equal to this queens col, skip this row.
            if (i == row) continue;
            board[i][col]++;
        }
        
        
        //Block left horizontals
        if (col <= row){
            for (unsigned y = row - col, x = 0; y < N; ++x, ++y){ //y will grow to N faster so skip checking x
                if (y == row) continue; //Skip ourself, we can't attack ourself
                board[y][x]++;
            }
        } else {
            for (unsigned y = 0; x = col - row; x < N; ++x, ++y){
                if (y == row) continue; //Skip ourself, we can't attack ourself
                board[y][x]++;
            }
        }
        
        //Block right horizontals
        const unsigned iy = N - row - 1;
        if (iy <= col){
            for (unsigned y = N - 1, x = col - iy; x < N; --y, ++x){
                if (y == row) continue;
                board[y][x]++;
            }
        } else {
            for (unsigned y = row - iy, x = 0; y >= 0; --y; ++x){
                if (y == row) continue;
                board[y][x]++;
            }
        }
        
        //TODO mark all integer intersections of spies lined up
        
        
    }
    
}

void solve(Board board){
    
    while (!solved){
        //Set initial positions randomly
        vector<unsigned> pos(N);
        random_device rand;
        uniform_int_distribution<unsigned> dist(0,N);
        for (unsigned& r : pos){
            r = dist(rand);
        }
        
        const int moves = maxMoves;
        while (maxMoves > 0 && !solved){
        
            //Set the attack levels on the board.
            attack(board, pos);

            vector<unsigned> conflicted;
            for (unsigned i = 0; i < N; ++i){
                if (board[i][pos[i]] > 0){
                    conflicted.push_back(i);
                }
            }

            if (conflicted == 0 && !solved){ //Win and no other thread finished first.
                
                solved = true;
            
                for (auto& r : board){
                    
                    for (unsigned i : r){
                        
                        cout << i;
                        
                    }
                    
                    cout << endl;
                    
                }
            
            } else {
                
                //Randomly choose a queen to move to its best position and continue.
                
                
            }
        
        }
        
        
        
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
