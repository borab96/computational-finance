//
//  main.cpp
//  Deng_Renren_HW3
//
//  Created by kevin on 9/16/17.
//  Copyright © 2017 RenrenRenren Deng. All rights reserved.
//  Reference by C++ written by Prof. R.S. Sreenivas


//


#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include "lp_lib.h"

#define N 730
#define ENCODE (81 * (i - 1)) + (9 * (j - 1)) + k

using namespace std;

// Global variables for the problem 9*9*9 = 729
// See my handout for further explanations
lprec* lp;
double solution[N-1];



void block_check(int permute[4])
{
    for (int k = 1; k <= 9; k++)
    {
        double row[N] = { 0 };

        for (int i = permute[0]; i <= permute[1]; i++)
        {
            for (int j = permute[2]; j <= permute[3]; j++)
            {
                row[ENCODE] = 1;
            }
        }
        add_constraint(lp, row, EQ, 1);
    }
}
    

// This sets the generic ILP for the Sudoku Puzzle
// It does not use any of the pre-determined board-positions,
// that part is done after the input file is read.
void set_sudoku_ilp()
{
    // setting the problem up: 9*9*9 = 729 variables
    lp = make_lp(0, 729);

    // This keeps the message reporting of lp_solve to a minimum
    set_verbose(lp, 3);

    {
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                // constraint that says each (i,j) entry in the 9x9 table
                // should have one number in it
                // creating a row with 1's at the right spot (the first
                // entry has to be a zero; this is idiosynchratic of lpsolve)

                double row[730];
                for (int k = 0; k < 730; k++)
                    row[k] = 0;

                for (int k = 1; k <= 9; k++)
                    row[(81 * (i - 1)) + (9 * (j - 1)) + k] = 1;
                // adding the constraint
                add_constraint(lp, row, EQ, 1);
            }
        }
    }
    // Row constraints
    {
        for (int i = 1; i <= 9; i++)
        {
            for (int k = 1; k <= 9; k++)
            {
                double row[N] = { 0 };
                for (int j = 1; j <= 9; j++)
                    row[ENCODE] = 1;
                add_constraint(lp, row, EQ, 1);
            }
        }
    }
    // Col constraints
    {
        for (int j = 1; j <= 9; j++)
        {
            for (int k = 1; k <= 9; k++)
            {
                double row[N] = { 0 };
                for (int i = 1; i <= 9; i++)
                    row[ENCODE] = 1;

                // adding the constraint
                add_constraint(lp, row, EQ, 1);
            }
        }
    }
    // block constraints
    {
        int perms[9][4] = {{1,3,1,3}, {4,6,1,3}, {7,9,1,3},
                           {1,3,4,6}, {1,3,7,9}, {4,6,4,6},
                           {4,6,7,9}, {7,9,4,6}, {7,9,7,9}};

        for (int l = 0; l < 9; l++)
            block_check(perms[l]);
    }
    

    // upper-bound each variable by 1
    for (int i = 1; i <= 729; i++)
    {
        double row[730];
        for (int j = 0; j < 730; j++)
            row[j] = 0;
        row[i] = 1;
        add_constraint(lp, row, LE, 1);
    }

    // it does not matter what the objective function (why?)
    // I am minimizing the sum of all variables.
    {
        double row[730];
        for (int i = 1; i < 730; i++)
            row[i] = 1;
        row[0] = 0;

        set_obj_fn(lp, row);
    }

    // set the variables to be integers
    for (int i = 1; i <= 729; i++)
        set_int(lp, i, TRUE);
}

// This subroutine reads the incomplete board-information from the
// input file and sets the appropriate constraints before the ILP
// is solved.
void read_input_data(char* argv[])
{
    // reading the input filename from commandline
    ifstream input_filename(argv[1]);

    if (input_filename.is_open()) {

        cout << "Input File Name: " << argv[1] << endl;
        cout << endl << "Initial Board Position" << endl;
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                int value_just_read;
                input_filename >> value_just_read;

                // check if we have a legitimate integer between 1 and 9
                if ((value_just_read >= 1) && (value_just_read <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    cout << value_just_read << " ";

                    double row[730];
                    for (int k = 0; k < 730; k++)
                    {
                        row[k] = 0;
                    }
                    row[(81 * (i - 1)) + (9 * (j - 1)) + value_just_read] = 1;

                    add_constraint(lp, row, EQ, 1);

                    // add appropriate constraints that bind the value of the
                    // appropriate variables based on the incomplete information
                    // that was read from the input file
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    cout << "X ";
                }

            }
            cout << endl;
        }
    }
    else {
        cout << "Input file missing" << endl;
        exit(0);
    }
}

// The ILP formulation is solved using the API for Lpsolve
// Pay attention to how the solution is interpretted...
void solve_the_puzzle()
{
    int ret;
    int sol_num = 0;
    // solve the lp
    ret = solve(lp);


    // Check if you had a solution
    // (see online guide for the codes at http://lpsolve.sourceforge.net/5.0/ )
    while (ret == 0)
    {
        sol_num++;
        // get the optimal assignment
        get_variables(lp, solution);

        double row[N] = {0};

        // print the solution
        cout << endl << "Solution #" << sol_num <<endl;
        
        {
            for (int i = 1; i <= 9; i++)
            {
                for (int j = 1; j <= 9; j++)
                {
                    for (int k = 1; k <= 9; k++)
                    {
                        if (solution[ENCODE-1] == 1)
                        {
                            cout << k << " ";
                            row[ENCODE] = 1; // Consider complete solution to be a constraint
                        }
                    }
                }
                cout << endl;
            }
        }
        add_constraint(lp, row, LE, 80);
        ret =  solve(lp);
    }
    if (sol_num==0)
    {
        cout << "Check the input file... looks like there is no solution" << endl;
    }
    delete_lp(lp);
}

int main(int argc, char* argv[])
{
    // formulate the non-input related part of the puzzle
    set_sudoku_ilp();

    // read the incomplete input information, and set appropriate constraints
    read_input_data(argv);

    // solve the puzzle and print the solution
    solve_the_puzzle();

    return(0);
}
