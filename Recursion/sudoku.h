/*
 *  sudoku.h
 *  Sudoku
 *  Created by Prof. Ramavarapu Sreenivas 
 *  Inspired by: http://web.eecs.utk.edu/courses/spring2012/cs140/Notes/Sudoku/index.html
*/

#ifndef sudoku
#define sudoku

#include <vector>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
using std::vector;
using namespace std;
class Sudoku 
{ 
	// Private
	int puzzle[9][9];
	
	bool findEmptyPlace(int &row, int &col) 
	{
	// from https://cppsecrets.com/users/96531181051109712199104111112114975750461189964103109971051084699111109/Sudoku-solver-Backtracking.php    
	for (row = 0; row < 9; row++)
		for (col = 0; col < 9; col++)
			if (puzzle[row][col] == 0) 
				return true;
	return false;
	}
	bool check_empty()
	{
		int count = 0;
		for (int i=0; i<9; i++)
		{
			for (int j=0; j<9; j++)
			{
				count = count + puzzle[i][j];
			}
		}
		if(count==0)
			return true;
		else
			return false;
	}
	bool check_duplicates(int arr[9])
	// Checks if sorted array has repeated entries, ignoring 0
		{
			for (int i=0; i<8;i++)
			{
				if(arr[i]==arr[i+1])
				{
					if (arr[i]!=0)
						return false;
				}
			}
			return true;
		}
	bool row_valid(int row)
		{

			int row_vec[9];
			for (int i=0; i<9; i++)
			{
				row_vec[i] = puzzle[row][i];
			}
			sort(row_vec, row_vec+9);
			return check_duplicates(row_vec);
		}
	
	// Private member function that checks if the named column is valid
	bool col_valid(int col)
	{
		int col_vec[9];
			for (int i=0; i<9; i++)
			{
				col_vec[i] = puzzle[i][col];
			}
			sort(col_vec, col_vec+9);
			return check_duplicates(col_vec);
	}
	
	// Private member function that checks if the named 3x3 block is valid
	bool block_valid(int row, int col)
	{
		vector<int>block_arr;
		for (int i = row; i < row+3; i++)
		{
			for (int j = col; j < col+3; j++)
			{
				block_arr.push_back(puzzle[i][j]);
			}
		}
		sort(block_arr.begin(), block_arr.end());
		int arr[9];
		std::copy(block_arr.begin(), block_arr.end(), arr);
		return check_duplicates(arr);
	}
	
public:
	// Public member function that reads the incomplete puzzle
	// we are not doing any checks on the input puzzle -- that is,
	// we are assuming they are indeed valid
	void read_puzzle(int argc, char * const argv[])
	{
	ifstream input_file;

     input_file.open(argv[1]);
     int numbers[81];
     int count = 0;
        // Read the numbers from the file into the array.

    for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			input_file >>puzzle[i][j];
		}
	}

    input_file.close();
	}
	
	// Public member function that prints the puzzle when called
	void print_puzzle()
	{
		std::cout << std::endl << "Board Position" << std::endl;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				// check if we have a legitimate integer between 1 and 9
				if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
				{
					// printing initial value of the puzzle with some formatting
					std::cout << puzzle[i][j] << " ";
				}
				else {
					// printing initial value of the puzzle with some formatting
					std::cout << "X ";
				}
			}
			std::cout << std::endl;
		}
	}



	
	// Public member function that (recursively) implements the brute-force 
	// search for possible solutions to the incomplete Sudoku puzzle
	bool Solve(int row, int col)
	{
		// this part of the code identifies the row and col number of the 
		// first incomplete (i.e. 0) entry in the puzzle.  If the puzzle has
		// no zeros, the variable row will be 9 => the puzzle is done, as 
		// each entry is row-, col- and block-valid...
		
		// use the pseudo code of figure 3 of the description
			if(check_empty())
			{
				cout<<"Puzzle cannot be empty!"<<endl;
				return false;
			}
		// Base case
		if(!findEmptyPlace(row, col))
			{	
				// Found a solution, print it
				print_puzzle();
				// Keep backtracking, do not break recursion
				puzzle[row][col] = 0 ;
				return false;
			}
		for (int k=1; k<=9; k++)
		{
			puzzle[row][col] = k;

			bool valid = col_valid(col) && row_valid(row) && block_valid(row-row%3, col-col%3) && Solve(row, col);
			if (valid)
			{
				return true;
			}
		}
		// print_puzzle();
		puzzle[row][col]=0;
		return false;
	
	}
};

#endif
