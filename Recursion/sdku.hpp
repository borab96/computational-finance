/* This class lets you store, print and solve Sudoku problems. */

#include <vector>

class Sudoku {

  // There is no nead for a constructor, destructor, copy constructor or assignment overload.

  public:
    void Clear();                        // Clear the current puzzle
    std::string Read_From_Stdin();       // Read a puzzle from standard input.  Return "" on 
                                         // success, "EOF" on EOF, or an error string on failure.
    void Print_Screen() const;           // Print the puzzle to the screen
    void Print_Convert() const;          // Print commands for convert to make Sudoku.jpg
    bool Solve();                        // Solve the puzzle - returns false if unsolvable

  // These are helper methods for both reading in the puzzle, and solving the puzzle,
  // plus a vector of strings to store the puzzle. 

  protected:
    bool Is_Row_Valid(int r) const;
    bool Is_Col_Valid(int c) const;
    bool Is_Panel_Valid(int sr, int sc) const;
    bool Recursive_Solve(int r, int c);

    std::vector <std::string> Grid;
};

