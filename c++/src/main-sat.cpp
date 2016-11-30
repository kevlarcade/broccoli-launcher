#include "sat.hpp"
#include "dimacs.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char ** argv) {
  if (argc < 3) {
    cerr << "Usage: sat fichier.dimacs fichier-resultat.output" << endl;
    return -1;
  } else {
    //////////
    // Lecture de l'entrée au format dimacs
    ifstream input(argv[1]);
    dimacs data;
    data.nbVars = 0;
    if (! input.good()) {
      cerr << "Le fichier " << argv[1] << " ne peut être lu" << endl;
      return -2;
    }
    lit_dimacs(input,data);

    //////////
    // Test de satisfiabilité à faire
    set<lit_t> lits;
    etat_t etat;           // TODO: initialiser l'état
    init_etat(etat, data.nbVars);
    bool sat = false;      // TODO: appeler le solveur via chercher(etat, data.cnf)
    sat = cherche(etat, data.cnf);

    //////////
    // Ajout des littéraux vrais dans lits pour écriture dans le fichier de sortie
    if (sat) {
      for(unsigned int i = 0; i < etat.valeurs.size(); ++i)
        lits.insert(var2lit(var(i), etat.valeurs[i] == vrai));
    }

    //////////
    // Ecriture du fichier résultat
    ofstream output(argv[2]);
    if (! output.good()) {
      cerr << "Le fichier " << argv[2] << " ne peut être écrit" << endl;
      return -2;
    }
    ecrit_sortie_sat(output, lits, sat);
    output.close();
    return 0;
  }
}

