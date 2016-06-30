#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>
//#include <atomic>
//#include <condition_variable>
//#include <mutex>
#include <cmath>
//#include <cassert>
#include <sstream>

#include "gcd.h"

using namespace std;

unsigned N;
#ifndef PROFILING
#define tc thread::hardware_concurrency()
#else
#define tc 1
#endif
#define maxMoves 50
//#define debug

volatile bool solved = false; //For stopping other threads early.
//atomic_uint games;
//condition_variable cv;

//Board access: [row][column]

void attack(vector<vector<unsigned> >& board, const vector<unsigned>& pos){

	for (auto& row : board){ //Reset all the attack values in the board.
		fill(row.begin(), row.end(), 0);
	}

	for (unsigned row = 0; row < N; ++row){

		const unsigned col = pos[row];

		//Block left horizontals
		if (col <= row){
			for (unsigned y = row - col, x = 0; y < N; ++x, ++y){ //y will grow to N faster so skip checking x
				if (y == row) continue; //Skip ourself, we can't attack ourself
				board[y][x]++;
			}
		} else {
			for (unsigned y = 0, x = col - row; x < N; ++x, ++y){
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
			for (unsigned y = row + (iy - (iy - col)), x = 0; y < N; --y, ++x){ //y will overflow to above n when -1
				if (y == row) continue;
				board[y][x]++;
			}
		}

		//mark all integer intersections of spies lined up
		for (unsigned i = row + 1; i < N; ++i){

			unsigned rise = i - row; //Keep in mind that values increase down the board
			int run = (int)pos[i] - (int)pos[row];

			int x = gcd(rise, abs(run));
			if (x != 1){
				rise /= x;
				run /= x;
			}

			int fillx, filly;

			unsigned startRow, startColumn;
			if (run < 0){
				//Solve for neg row
				fillx = (N - 1 - pos[row]) / -run; //We are now going the other way
				filly = row / rise;

				if (fillx >= filly){
					startRow = row - rise * filly;
					startColumn = pos[row] - run * filly;
				} else {
					startRow = row - rise * fillx;
					startColumn = pos[row] - run * fillx;
				}

			} else {
				//Solve for pos row
				fillx = pos[row] / run;
				filly = row / rise;

				if (fillx >= filly){
					startRow = row - rise * filly;
					startColumn = pos[row] - run * filly;
				} else {
					startRow = row - rise * fillx;
					startColumn = pos[row] - run * fillx;
				}

			}

			for (unsigned r = startRow, c = startColumn; r < N && c < N; r += rise, c += run){
				if (r == row || r == i) continue;
				board[r][c]++;
			}

		}

	}

}

void solve(const int seed){

	thread_local vector<vector<unsigned> > board(N, vector<unsigned>(N));
	thread_local default_random_engine rand(seed);

	while (!solved){
		//Set initial positions randomly
		vector<unsigned> pos(N);
		for (unsigned i = 0; i < N; ++i){
			pos[i] = i;
		}
		random_shuffle(pos.begin(), pos.end());

		vector<unsigned> ipos(N);
		for (unsigned i = 0; i < N; ++i){
			ipos[pos[i]] = i;
		}

		//		games++;
		//		cv.notify_one();

		int moves = maxMoves;
		while (moves > 0 && !solved){

			//Set the attack levels on the board.
			attack(board, pos);

#ifdef debug
			for (unsigned r = 0; r < N; ++r){
				for (unsigned c = 0; c < N; ++c){
					if (pos[r] == c){
						cout << 'X';
					} else {
						if (board[r][c] == 0){
							cout << '-';
						} else {
							cout << board[r][c];
						}
					}
				}
				cout << endl;
			}
			cout << endl;
#endif

			vector<unsigned> conflicted;
			for (unsigned i = 0; i < N; ++i){
				if (board[i][pos[i]] > 0){
					conflicted.push_back(i);
				}
			}

			if (conflicted.size() == 0 && !solved){ //Win and no other thread finished first.

				solved = true;

				for (unsigned i : pos){
					cout << i + 1 << " ";
				}

				cout << endl;

				//Human readable version of the board
#ifdef debug
				for (unsigned p : pos){
					for (unsigned i = 0; i < N; ++i){
						if (i == p){
							cout << '#';
						} else {
							cout << '-';
						}
					}
					cout << endl;
				}
#endif

				//				cv.notify_one();

				return;

			} else {

				uniform_int_distribution<unsigned> pick(0, conflicted.size() - 1);

				unsigned tries = conflicted.size();

				while(tries--){

					unsigned rowA = conflicted[pick(rand)];

					int bestImprovement = 0;
					unsigned rowB = N;

					for (unsigned c : conflicted){
						unsigned col = pos[c];
						if (col == pos[rowA]) continue;

						int change = (int)board[rowA][pos[rowA]] - (int)board[rowA][col]
									+ (int)board[ipos[col]][col] - (int)board[ipos[col]][pos[rowA]];

						if (change > bestImprovement){
							rowB = ipos[col];
							bestImprovement = change;
						}

					}

					if (rowB == N) continue;

					swap(pos[rowA], pos[rowB]); //Switch the values in pos
					swap(ipos[pos[rowA]], ipos[pos[rowB]]); //Switch the indexes in ipos
					break;
				}

				if (tries == 0){
					break; //This initial condition failed try another.
				}

			}

			moves--;

		}

	}

}


int main(int argc, char** argv){

	if (argc < 2){
		cout << "You must supply the N you are solving for." << endl;
		return 0;
	}

	stringstream ss;
	ss << argv[1];
	ss >> N;

	if (N < 4){
		cout << "N must be greater than 4." << endl;
		return 0;
	}

	//Pregenerate all the random seeds
	random_device rand;

	//Start the problem a few times, one will complete first.
	vector<thread> threads(tc);
	for (unsigned i = 0; i < tc; ++i){
		threads[i] = thread(solve, rand());
	}

	//	mutex lck;

	//	while(!solved){
	//		unique_lock<mutex> ulck(lck);
	//		cv.wait(ulck);
	//		cout << "Games: " << games.load() << "\r";
	//	}

	for (thread& t : threads){
		t.join();
	}

	return 0;
}
