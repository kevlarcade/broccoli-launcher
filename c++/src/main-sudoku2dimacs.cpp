#include "sudoku.hpp"
#include <iostream>
#include <fstream>
int main(int argc, char ** argv) {
  if (argc < 3) {
    cerr << "Usage: sudoku2dimacs fichier-sudoku.txt fichier.dimacs" << endl;
    return -1;
  } else {
    ifstream input(argv[1]);
    if (! input.good()) {
      cerr << "Le fichier " << argv[1] << " ne peut être lu" << endl;
      return -2;
    }
    sudoku_t sudoku;
    lit_sudoku(input,sudoku);

    ofstream output(argv[2]);
    if (! output.good()) {
      cerr << "Le fichier " << argv[2] << " ne peut être écrit" << endl;
      return -2;      
    }
    genere_probleme_sat(output, sudoku);
    output.close();
    return 0;
  }
}

