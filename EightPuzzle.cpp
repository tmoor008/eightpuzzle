#include <iostream>
#include <vector>
#include <utility> 
#include <queue>
#include <functional>
#include <string>
#include <unordered_map>
#include <cstdlib>

using namespace std;

class Node
{
	public:
		int cost; //how much to each state, f(n)
		int depth; //how much to get to n g(n)
		int heuristic; //cost of heurisitic h(n)
		int alg; //which heursitic to use
		int sz; //width of puzzle, defaults to 3 for 8-puzzle currently
		vector<vector<int>> state; //current puzzle state
		pair<int, int> index; //where the zero is located
		
		Node(vector<vector<int>> state1, pair<int, int> index1, int cost1, int depth1, int heuristic1, int alg, int sz1)
		{
			state = state1;
			index = index1;
			cost = cost1;
			depth = depth1;
			sz = sz1;
			heuristic = 0;
			
			int area = sz * sz;
			
			if (alg == 2) //misplaced tiles
			{
				int val = 0;
				for (int i = 0; i < sz; ++i)
				{
					for (int j = 0; j < sz; ++j)
					{
						++val;
						
						if (state.at(i).at(j) != val && val < area)
						{
							++heuristic;
						}
					}
				}
			}
			
			else if (alg == 3) //manhattan distance
			{
				int temp = 0;
				int checkVal = 1;
				
				//iterates through the puzzle
				for (int i = 0; i < sz; ++i)
				{
					for (int j = 0; j < sz; ++j)
					{
						if (checkVal == area) //if the checking the last pos, val shud be 0
						{
							checkVal = 0;
						}
						
						if (state.at(i).at(j) != checkVal && checkVal < area) //if val is not in correct position
						{
							//see what the value at that position is
							if(state.at(i).at(j) == 1)
							{
								temp = abs((int)(0 - i)) + abs((int)(0 - j)); //calculate distance from actual position to desired position
								heuristic = heuristic + temp; //increment heuristic by the distance
							}
							
							else if(state.at(i).at(j) == 2)
							{
								temp = abs((int)(0 - i)) + abs((int)(1 - j));
								heuristic = heuristic + temp;
							}
							
							else if(state.at(i).at(j) == 3)
							{
								temp = abs((int)(0 - i)) + abs((int)(2 - j));
								heuristic = heuristic + temp;
							}
					
							else if(state.at(i).at(j) == 4)
							{
								temp = abs((int)(1 - i)) + abs((int)(0 - j));
								heuristic = heuristic + temp;
							}
							
							else if(state.at(i).at(j) == 5)
							{
								temp = abs((int)(1 - i)) + abs((int)(1 - j));
								heuristic = heuristic + temp;
							}
							
							else if(state.at(i).at(j) == 6)
							{
								temp = abs((int)(1 - i)) + abs((int)(2 - j));
								heuristic = heuristic + temp;
							}
							
							else if(state.at(i).at(j) == 7)
							{
								temp = abs((int)(2 - i)) + abs((int)(0 - j));
								heuristic = heuristic + temp;
							}
							
							else if(state.at(i).at(j) == 8)
							{
								temp = abs((int)(2 - i)) + abs((int)(1 - j));
								heuristic = heuristic + temp;
							}
							else
							{
								temp = 0;
							}
						}
						++checkVal;
					}
				}
			}
			
			else //uniform cost defaults
			{
				heuristic = 0;
			}
			
			cost = depth + heuristic; //calcualte f(n) = g(n) + h(n)
		}
		
		void output() //simple output of puzzle
		{
			for (int i = 0; i < sz; ++i)
			{
				for (int j = 0; j < sz; ++j)
				{
					cout << state.at(i).at(j) << " ";
				}
				cout << endl;
			}
		}
};

struct Comparator : public std::binary_function<Node, Node, bool>
{
	//moves least cost node to top of frontier
	bool operator()(const Node lhs, const Node rhs) const
	{
		return lhs.cost > rhs.cost;
	}
};

string convertToString(Node node)
{
	//converts the state in the node to a string
	string str;
	for (int i = 0; i < node.sz; ++i)
	{
		for (int j = 0; j < node.sz; ++j)
		{
			str = str + to_string(node.state.at(i).at(j));
		}
	}
	
	return str;
}

string convertToStringV(vector<vector<int>> state, int sz)
{
	//converts just the state to a string
	string str;
	for (int i = 0; i < sz; ++i)
	{
		for (int j = 0; j < sz; ++j)
		{
			str = str + to_string(state.at(i).at(j));
		}
	}
	
	return str;
}

bool isGoalState(Node child)
{
	//checks state within the node against the goal state
	unordered_map<string, bool> goal;
	goal["123456780"] = true;
	
	string chk = convertToString(child);
	
	if(goal[chk] == true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//these four check if the blank space can move in their corresponding direction
bool Right(Node child)
{
	if (child.index.second < (child.sz - 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Left(Node child)
{
	if (child.index.second > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Up(Node child)
{
	if (child.index.first > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Down(Node child)
{
	if (child.index.first < (child.sz - 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}


vector<vector<int>> genState(vector<vector<int>> newState, pair<int, int> swap_pos, pair<int, int> og_pos)
{
	//generates a new puzzle by swapping the blank space with the corresponding swap position
	int tmp = newState.at(og_pos.first).at(og_pos.second);
	newState.at(og_pos.first).at(og_pos.second) = newState.at(swap_pos.first).at(swap_pos.second);
	newState.at(swap_pos.first).at(swap_pos.second) = tmp;
	
	return newState;
}

bool checkUnsolvable(Node node)
{
	//checks if the number of inversions is odd and returns true or false
	vector<int> v;
	int inversions = 0;
	int area = node.sz * node.sz;
	
	for (int i = 0; i < node.sz; ++i)
	{
		for (int j = 0; j < node.sz; ++j)
		{
			v.push_back(node.state.at(i).at(j));
		}
	}

	
	for (int k = 0; k < area; ++k)
	{
		for (int l = k + 1; l < area; ++l)
		{
			if (v.at(k) > v.at(l) && v.at(k) != 0 && v.at(l) != 0)
			{
				++inversions;
			}
		}
	}
	
	if ((inversions % 2) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

void Graph_Search(Node start, int alg)
{
	priority_queue<Node, vector<Node>, Comparator> frontier;
	unordered_map<string, bool> explored;
	pair<int, int> swap_pos;
	unsigned maxNodes = 0;
	bool begin = true;
	int size = start.sz;
	
	if (checkUnsolvable(start)) //checks the number of inversions in the puzzle to see if it is unsolvable or not
	{
		cout << "This puzzle has no solution." << endl;
		return;
	}
	
	frontier.push(start);
	
	while(!frontier.empty())
	{
		if (frontier.empty())
		{
			cout << "This puzzle has no solution. Failure." << endl;
			return;
		}
		
		//sets temp node and pops it off the frontier
		Node temp = frontier.top();
		frontier.pop();
		
		if (temp.depth > 31)
		{
			cout << "This puzzle has no solution. Failure." << endl;
			return;
		}
		
		if (isGoalState(temp)) //checks if the current state is the goal state
		{
			cout << endl << "Goal!!" << endl;
			
			int m = 0; //finds values in explored that were seen but not visited
			for (auto it : explored)
			{
				if (it.second == false)
				{
					m++;
				}
			}
			
			cout << "To solve this problem the search algorithm expanded a total of " << explored.size() - m << " nodes." << endl;
			cout << "The maximum number of nodes in the queue at any one time was " << maxNodes << "." << endl;
			cout << "The depth of the goal node was " << temp.depth << "." << endl;
			return;
		}
		
		//sets output based on whether on not it is the initial state
		if (begin == false)
		{
			cout << endl << "The best state to expand with a g(n) = " << temp.depth << " and h(n) = " << temp.heuristic << " is..." << endl;
			temp.output();
		}
		else
		{
			cout << endl << "Expanding initial state..." << endl;
			temp.output();
			begin = false;
		}
		
		explored[convertToString(temp)] = true; // sets current node to visited
		
		//creates temp state
		vector<vector<int>> tmp;
		int x = temp.index.first;
		int y = temp.index.second;
		
		if (Right(temp)) //checks if the blank space can swap right
		{
			//y + 1
			swap_pos.first = x; //determines pos the blank will swap with
			swap_pos.second = y + 1;
			tmp = genState(temp.state, swap_pos, temp.index); //generates the resulting puzzle after swap
			
			if(explored.find(convertToStringV(tmp, size)) == explored.end()) //sees if the puzzle has been seen
			{
				Node n = Node(tmp, swap_pos, 0, temp.depth + 1, 0, alg, size); //creates new node with new puzzle
				frontier.push(n); //pushes new node into the frontier
				explored[convertToString(n)] = false; //sets that node to seen but NOT yet visited
			}
			
		}
		
		//the next three do the same, just for the other 3 possible directions
		if (Left(temp))
		{
			//y - 1
			swap_pos.first = x;
			swap_pos.second = y - 1;
			tmp = genState(temp.state, swap_pos, temp.index);
			
			if(explored.find(convertToStringV(tmp, size)) == explored.end())
			{
				Node n = Node(tmp, swap_pos, 0, temp.depth + 1, 0, alg, size);
				frontier.push(n);
				explored[convertToString(n)] = false;
			}
			
		}
		
		if (Up(temp))
		{
			//x - 1
			swap_pos.first = x - 1;
			swap_pos.second = y;
			tmp = genState(temp.state, swap_pos, temp.index);
			
			if(explored.find(convertToStringV(tmp, size)) == explored.end())
			{
				Node n = Node(tmp, swap_pos, 0, temp.depth + 1, 0, alg, size);
				frontier.push(n);
				explored[convertToString(n)] = false;
			}
			
		}
		
		if (Down(temp))
		{
			//x + 1
			swap_pos.first = x + 1;
			swap_pos.second = y;
			tmp = genState(temp.state, swap_pos, temp.index);
			
			if(explored.find(convertToStringV(tmp, size)) == explored.end())
			{
				Node n = Node(tmp, swap_pos, 0, temp.depth + 1, 0, alg, size);
				frontier.push(n);
				explored[convertToString(n)] = false;
			}
			
		}
		
		//updates max number of nodes in the frontier at any one time
		if (maxNodes < frontier.size())
		{
			maxNodes = frontier.size();
		}
		
		
	}
	
}

int main()
{

	int one, two, three, four, five, six, seven, eight, nine;
	
	cout << "Welcome to 861144845 8-Puzzle Solver!" << endl;
	cout << "Enter width of puzzle ex: 8-puzzle - 3, 15-puzzle - 4, etc. (defaults to 3 currently for 8-puzzle)" << endl;
	int sz;
	cin >> sz;
	sz = 3; //defaults to 3 right now because only solves 8-puzzle
	cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle." << endl;
	int input;
	cin >> input;
	if (input == 2)
	{
		cout << "Enter your puzzle, use a zero to represent a blank." << endl;
		cout << "Enter the first row, use spaces between numbers. Then press enter." << endl;
		cin >> one;
		cin >> two;
		cin >> three;
		cout << "Enter the second row, use spaces between numbers. Then press enter." << endl;
		cin >> four;
		cin >> five;
		cin >> six;
		cout << "Enter the third row, use spaces between numbers. Then press enter." << endl;
		cin >> seven;
		cin >> eight;
		cin >> nine;
	}
	
	else
	{
		one = 2;
		two = 8;
		three = 3;
		four = 4;
		five = 1;
		six = 5;
		seven = 7;
		eight = 0;
		nine = 6;
	}

	//initializes initial state
	vector<vector<int>> v;
	vector<int> temp;
	pair<int, int> index0;
	
	temp.push_back(one);
	temp.push_back(two);
	temp.push_back(three);
	v.push_back(temp);
	temp.clear();
	temp.push_back(four);
	temp.push_back(five);
	temp.push_back(six);
	v.push_back(temp);
	temp.clear();
	temp.push_back(seven);
	temp.push_back(eight);
	temp.push_back(nine);
	v.push_back(temp);
	
	cout << endl << "The puzzle you entered: " << endl;
	
	for (int i = 0; i < sz; ++i)
	{
		for (int j = 0; j < sz; ++j)
		{
			cout << v.at(i).at(j) << " ";
			if (v.at(i).at(j) == 0)
			{
				index0.first = i;
				index0.second = j;
			}
		}
		cout << endl;
	}
	
	int choice;
	cout << endl << "Enter your choice of algorithm: " << endl;
	cout << "1: Uniform Cost Search" << endl;
	cout << "2: A* with the Misplaced Tile Heuristic" << endl;
	cout << "3: A* with the Manhattan Distance Heuristic" << endl;

	cin >> choice;

	Graph_Search(Node(v, index0, 0, 0, 0, choice, sz), choice);

}