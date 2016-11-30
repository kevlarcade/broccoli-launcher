#ifndef Sudoku
#define Sudoku
#include <iostream>
#include <vector>
#include "sat.hpp"

using namespace std;

typedef vector< vector<int> > sudoku_t;

ostream& operator<<(ostream& out, const vector<int>& ligne_grille); 

void lit_sudoku(istream& input, sudoku_t & sudoku);

void ecrit_sudoku(ostream& output, const sudoku_t & sudoku);

/**
   Génère la formule conjonctive représentant le problème de Sudoku passé en argument.
   La formule peut être écrite directement sur le flux de sortie output.
 */
void genere_probleme_sat(ostream& output, const sudoku_t& sudoku);

/**
   Renvoie le littéral positif de la variable codant le fait que la case (i,j) prend la valeur v.
   L'entier n2 est égal nombre de lignes (et donc de colonnes) de la grille.
*/
lit_t l_case_v(int n2, int i, int j, int v);

void v_case_l(int n2, lit_t l, int &i, int &j, int &v);

/**
   Ecrit les clauses correspondant à la contrainte "1 vrai parmi n" au format dimacs sur le flux de sortie. lits contient les n littéraux // // sur lesquels appliquer la contrainte.
*/
void exactement_1_parmi_n(ostream& output, const vector<lit_t>& lits);
#endif
