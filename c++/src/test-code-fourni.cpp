#include "dimacs.hpp"
#include "sudoku.hpp"
#include <UnitTest++.h>
#include <iostream>
#include <sstream>
#include <fstream>

// Tests DIMACS
TEST(lecture_dimacs_sample) {
  ifstream sample("../exemples/sat/sample.dimacs");
  dimacs data;
  data.nbVars = 0;
  lit_dimacs(sample, data);
  CHECK(data.nbVars == 2);
  CHECK(data.cnf.clauses.size() == 2);
  set<lit_t> lits1 = {0,3};
  set<lit_t> lits2 = {1};
  CHECK( (data.cnf.clauses[0].litteraux == lits1 && data.cnf.clauses[1].litteraux == lits2)
	 || (data.cnf.clauses[0].litteraux == lits2 && data.cnf.clauses[1].litteraux == lits1) );
}

TEST(lecture_output_sample) {
  ifstream sample("../exemples/sat/sample.output");
  set<lit_t> lits = lit_sortie_sat(sample);
  CHECK(lits.count(1) == 1);
  CHECK(lits.count(3) == 1);
}


TEST(lecture_output_unsat_sample) {
  ifstream sample("../exemples/sat/sample-unsat.output");
  set<lit_t> lits = lit_sortie_sat(sample);
  CHECK(lits.size() == 0);
}

// Test grille Sudoku
TEST(lecture_grille) {
  ifstream grille("../exemples/sudoku/grille-test1.txt");
  CHECK(grille.good());
  sudoku_t sudoku;
  lit_sudoku(grille, sudoku);
  CHECK_EQUAL(sudoku.size() ,  9u);
  for(unsigned int i = 0; i < sudoku.size(); ++i) {
    CHECK_EQUAL(sudoku[i].size() , 9u);
  }
  CHECK_EQUAL(sudoku[0] ,  vector<int>({7,0,0,2,0,0,0,8,5}));
  CHECK_EQUAL(sudoku[1] ,  vector<int>({0,2,5,0,0,0,0,0,0}));
  CHECK_EQUAL(sudoku[2] ,  vector<int>({9,0,0,0,0,0,0,3,0}));
  CHECK_EQUAL(sudoku[3] ,  vector<int>({0,3,0,0,1,0,2,0,0}));
  CHECK_EQUAL(sudoku[4] ,  vector<int>({0,0,0,0,6,0,0,7,1}));
  CHECK_EQUAL(sudoku[5] ,  vector<int>({4,6,0,0,0,0,0,9,0}));
  CHECK_EQUAL(sudoku[6] ,  vector<int>({0,0,0,0,0,4,3,0,0}));
  CHECK_EQUAL(sudoku[7] ,  vector<int>({0,0,0,1,0,0,0,0,8}));
  CHECK_EQUAL(sudoku[8] ,  vector<int>({0,0,2,7,0,3,0,6,0}));
}

TEST(ecriture_grille) {
  ifstream grille("../exemples/sudoku/grille-test1.txt");
  CHECK(grille.good());
  sudoku_t sudoku;
  lit_sudoku(grille, sudoku);
  ofstream grilleb_w("../exemples/sudoku/grille-test1-b.txt");
  ecrit_sudoku(grilleb_w, sudoku);
  grilleb_w.close();
  ifstream grilleb_r("../exemples/sudoku/grille-test1-b.txt");
  sudoku_t sudoku2;
  lit_sudoku(grilleb_r, sudoku2);
  CHECK_EQUAL(sudoku.size(), sudoku2.size());
  for(unsigned int i = 0; i < sudoku.size(); ++i) {
    CHECK_EQUAL(sudoku[i], sudoku2[i]);
  }
}
