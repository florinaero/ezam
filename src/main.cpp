#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <utility>

using namespace std;

class Maze{
	
	int m_size = 4;

public:
	template <typename T, size_t X, size_t Y>
	void build(T (&graph)[X][Y], int x, int y){
		if(graph[x][y]==0){
			// Mark as visited
			graph[x][y] = 1;
			build(graph,1,1);
		}
	}

	// Generate coord of next block
	void generate(int &x, int&y){
		// abs(x-y)=1 
		// Select x-1 or x+1 
		random_device rd;
		mt19937 rng(rd());		
		
		// Define available options based on current position
		vector<pair<int,int>> neighbors;
		if(x>0){
			// Add west
			neighbors.emplace_back(x-1,y);
		}
		if(x<m_size){
			// Add est
			neighbors.emplace_back(x+1,y);
		}
		if(y>0){
			// Add north		
			neighbors.emplace_back(x,y-1);
		}
		if(y<m_size){
			// Add south		
			neighbors.emplace_back(x,y+1);
		}

		uniform_int_distribution<mt19937::result_type> direction(0,neighbors.size()-1);
		auto coord = neighbors.at(direction(rng));
		
		x = coord.first;
		y = coord.second;
		cout << "x=" << x << " y=" << y << endl;
	}
};

int main (){

	cout << "Maze generator: Randomized depth-first search." << endl;

	const int size = 5;

	int x=0;
	int y=0;

	Maze maze;
	for (int i=0;i<10;i++){
		maze.generate(x,y);
	}

	return 0;
}