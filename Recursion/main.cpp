/* This is a main() routine that lets you solve sudoku puzzles on standard input.
   It will read puzzles on standard input, and then let you:

     - Either solve the puzzles or not.
     - Print the puzzle (solved or not).
         - You can print on the screen, or
         - You can print commands for the convert program to make Sudoku.jpg
 */

#include <iostream>
#include <cstdlib>
#include "sdku.hpp"
using namespace std;

/* Sometimes it's convenient to have a helper procedure to handle errors
   on the command line.  We could, of course, have used try/catch, but
   the usage() command makes for cleaner code, in my opinion. */

void usage(const string &s)
{
  cerr << "usage: sudoku solve(yes|no) output-type(screen|convert) - puzzles on stdin\n";
  if (s != "") cerr << s << endl;
  exit(1);
}
  
int main(int argc, char **argv)
{
  string solve;         // The first command line argument -- yes or no for whether to solve.
  string output;        // The second command line argument - "screen" or "convert"
  Sudoku sud;           // The puzzle.  
  string r;             // The return value from Read_From_Stdin().

  /* Parse the command line. */

  if (argc != 3) usage("");
  solve = argv[1];
  output = argv[2];
  if (solve != "yes" && solve != "no") usage("bad solve");
  if (output != "screen" && output != "convert") usage("bad output");

  if (output == "screen") cout << "-------------------" << endl;

  while (1) {

    /* Read the puzzle and handle EOF/errors */

    r = sud.Read_From_Stdin();
    if (r != "") {
      if (r == "EOF") return 0;
      cout << r << endl;
      return 1;
    }

    /* Solve the puzzle if desired. **/

    if (solve == "yes") {
      if (!sud.Solve()) {
        printf("Cannot solve puzzle\n");
      }
    }
  
    /* Print the puzzle. */

    if (output == "screen") {
      sud.Print_Screen();
      cout << "-------------------" << endl;
    } else {
      sud.Print_Convert();
    }

    /* Clear the puzzle and try again. (Clearing is unnecessary, but may as well test it.) */

    sud.Clear();
  }
}

