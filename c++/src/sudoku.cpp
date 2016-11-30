#include "sudoku.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>

/**
   Renvoie vrai si la ligne ne contient aucun chiffre ou commence par #
*/
bool ligne_vide(const string & s) {
  if (s.length() > 0 && *(s.cbegin()) == '#') {
    return true;
  }
  for(auto it = s.cbegin(); it != s.cend(); ++it) {
    if (*it >= '0' && *it <= '9') {
      return false;
    }
  }
  return true;
}

bool lit_ligne_sudoku(const string & ligne, vector<int> & ligne_grille) {
  bool lu_entiers = true;
  // bool lit_entier = false;


  string buf = "";
  for(auto it = ligne.cbegin(); it != ligne.cend(); ++it) {
    char c = *it;
    if (c == '#') {
      break;
    } else if ('0' <= c && c <= '9') {
      buf += c;
      // lit_entier = true;
    } else {
      if (buf.size() > 0) {
        ligne_grille.push_back(stoi(buf));
        lu_entiers = true;
        // lit_entier = false;
        buf = "";
      }
    }
  }
  if (buf.size() > 0) {
    ligne_grille.push_back(stoi(buf));
  }
  return lu_entiers;
}

void lit_sudoku(istream& input, sudoku_t & sudoku) {
  string ligne;
  while (input.good()) {
    getline(input,ligne);
    if (!ligne_vide(ligne)) {
      vector<int> s_line;
      if (lit_ligne_sudoku(ligne, s_line)) {
        sudoku.push_back(s_line);
      }
    }
  }
}


ostream& operator<<(ostream& out, const vector<int>& ligne_grille) {
  out << " { ";
  bool not_first = false;
  for(auto it = ligne_grille.cbegin(); it != ligne_grille.cend(); ++it) {
    out << *it; 
    if (not_first) {
      out << ",";
      not_first = true;
    }
    out << " ";
  }
  out << "} ";
  return out;
}


void ecrit_sudoku(ostream& output, const sudoku_t & sudoku) {
  for(auto it_l = sudoku.cbegin(); it_l != sudoku.cend(); ++it_l) {
    for(auto it_c = it_l->cbegin(); it_c != it_l->cend(); ++it_c) {
      output << *it_c << " ";
    }
    output << endl << endl;
  }
}

void genere_probleme_sat(ostream& output, const sudoku_t& sudoku) {
  int i, j, v, n2, n, k, l;
  vector<lit_t> lit;

  n2 = sudoku.size();
  n = (int) sqrt(n2);

  for (i = 0; i < n2; i++) {
    for (j = 0; j < n2; j++) {
      lit.clear();
      for (v = 1; v <= n2; v++) {
        lit.push_back(l_case_v(n2, i, j, v));
      }
      exactement_1_parmi_n(output, lit);
    }
  }

  for (v = 1; v <= n2; v++) {
    for (i = 0; i < n2; i++) {
      lit.clear();
      for (j = 0; j < n2; j++) {
        lit.push_back(l_case_v(n2, i, j, v));
      }
      exactement_1_parmi_n(output, lit);
    }
  }

  for (v = 1; v <= n2; v++) {
    for (j = 0; j < n2; j++) {
      lit.clear();
      for (i = 0; i < n2; i++) {
        lit.push_back(l_case_v(n2, i, j, v));
      }
      exactement_1_parmi_n(output, lit);
    }
  }

  for (v = 1; v <= n2; v++) {
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        lit.clear();
        for (k = 0; k < n; k++) {
          for (l = 0; l < n; l++) {
            lit.push_back(l_case_v(n2, i * n + k, j * n + l, v));
          }
        }
        //exactement_1_parmi_n(output, lit);
      }
    }
  }

  for (i = 0; i < n2; i++) {
    for (j = 0; j < n2; j++) {
      if (sudoku[i][j] != 0) {
        output << lit2var(l_case_v(n2, i, j, sudoku[i][j])).num << endl;
      }
    }
  }
}

lit_t l_case_v(int n2, int i, int j, int v) {
  var_t var;

  var.num = n2 * (n2 * i + j) + v;
  return var2lit(var, true);
}

void v_case_l(int n2, lit_t l, int &i, int &j, int &v) {
  int num = lit2var(l).num;

  v = (num % n2) + 1;
  num /= n2;
  i = num / n2;
  j = num % n2;
}

void exactement_1_parmi_n(ostream& output, const vector<lit_t>& lits) {
  vector<lit_t>::const_iterator it, it2;

  for (it = lits.begin(); it != lits.end(); ++it) {
    if (!positif(*it))
      output << "-";
    output << lit2var(*it).num << " ";
  }
  output << endl;

  for (it = lits.begin(); it != lits.end(); ++it) {
    for (it2 = it + 1; it2 != lits.end(); ++it2) {
      if (positif(*it))
        output << "-";
      output << lit2var(*it).num << " ";

      if (positif(*it2))
        output << "-";
      output << lit2var(*it2).num << endl;
    }
  }
}
