#include "sudoku.hpp"
#include "dimacs.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

int main(int argc, char ** argv) {
  if (argc < 4) {
    cerr << "Usage: res2sudoku fichier-probleme-sudoku.txt fichier-modele-sat.output fichier-sudoku-resolu.txt" << endl;
    return -1;
  } else {
    ifstream input_sudoku(argv[1]);
    if (! input_sudoku.good()) {
      cerr << "Le fichier " << argv[1] << " ne peut être lu" << endl;
      return -2;
    }
    sudoku_t sudoku;
    lit_sudoku(input_sudoku,sudoku);

    ifstream input_res_sat(argv[2]);
    if (! input_res_sat.good()) {
      cerr << "Le fichier " << argv[1] << " ne peut être lu" << endl;
      return -2;
    }
    set<lit_t> lits= lit_sortie_sat(input_res_sat);

    ofstream output(argv[3]);
    if (! output.good()) {
      cerr << "Le fichier " << argv[2] << " ne peut être écrit" << endl;
      return -2;
    }
    sudoku_t sudoku_resolu = sudoku;
    int n2 = sudoku.size();
    for (auto it = lits.begin(); it != lits.end(); ++it) {
      int i, j, v;
      if (!positif(*it))
        continue;
      v_case_l(n2, *it, i, j, v);
      sudoku_resolu[i][j] = v;
    }
    ecrit_sudoku(output, sudoku_resolu);
    output.close();
    return 0;
  }
}
