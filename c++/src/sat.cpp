#include "sat.hpp"
#include <iostream>
#include <cassert>

//////////////////////////////////////////////////////////////////////
// Variables
var_t var(int num) {
  var_t res;
  res.num = num;
  return res;
}

//////////////////////////////////////////////////////////////////////
// Littéraux


lit_t var2lit(var_t v, bool positif) {
  return (2 * v.num) + 1 - positif;
}

var_t lit2var(lit_t l) {
  var_t v;
  v.num = l / 2;
  return v;
}

bool positif(lit_t l) {
  return !(l & 1);
}

lit_t oppose(lit_t l) {
  return l ^ 1; // xor 1 pour changer le dernier bit
}

//////////////////////////////////////////////////////////////////////
// Clauses
cls_t clause(const set<lit_t> & litteraux) {
  cls_t res;
  set< lit_t >::const_iterator it;

  res.est_vrai = false;
  res.nb_indet = 0;
  for (it = litteraux.begin(); it != litteraux.end(); ++it)
    ajouteLitteral(*it, res);
  return res;
}

void ajouteLitteral(lit_t l, cls_t & cls) {
  cls.litteraux.insert(l);
  cls.est_vrai = false;
  cls.nb_indet++;
}

//////////////////////////////////////////////////////////////////////
// Formules conjonctives
cnf_t cnf(const vector<cls_t> & clauses) {
  vector< cls_t >::const_iterator it;
  cnf_t res;

  for (it = clauses.begin(); it != clauses.end(); ++it)
    ajouteClause(*it, res);
  return res;
}

void ajouteClause(const cls_t & clause, cnf_t & cnf) {
  set< lit_t >::const_iterator it;
  int clauseidx;
  size_t minTaille;

  cnf.clauses.push_back(clause); // ENLARGE YOUR PENIS!
  clauseidx = cnf.clauses.size() - 1;

  for (it = clause.litteraux.begin(); it != clause.litteraux.end(); ++it) {
    minTaille = *it + 1;
    if (positif(*it))
      minTaille++;
    if (minTaille > cnf.index.size())
      cnf.index.resize(minTaille);
    cnf.index[*it].push_back(clauseidx);
  }
}

void init_etat(etat_t& etat, int nb_vars) {
  vector< val_t >::iterator it;

  etat.valeurs.resize(nb_vars);
  for (it = etat.valeurs.begin(); it != etat.valeurs.end(); ++it) {
    *it = indeterminee;
  }

  etat.derniere_affectee.num = -1;
}

val_t opposeval(val_t v) {
  if (v == vrai)
    return faux;
  else if (v == faux)
    return vrai;
  return indeterminee;
}

val_t valeur(const etat_t & etat, lit_t lit) {
  var_t v = lit2var(lit);

  if (positif(lit))
    return etat.valeurs[v.num];
  return opposeval(etat.valeurs[v.num]);
}

val_t evaluer_clause(const etat_t & etat, const cls_t & cls) {
  set< lit_t >::const_iterator it;
  bool indet = false;
  val_t v;
  for (it = cls.litteraux.begin(); it != cls.litteraux.end(); ++it) {
    v = valeur(etat, *it);
    if (v == vrai)
      return vrai;
    if (v == indeterminee)
      indet = true;
  }
  if (indet == true)
    return indeterminee;
  return faux;
}

val_t evaluer_cnf(const etat_t & etat, const cnf_t & cnf, lit_t l) {
  vector< int >::const_iterator it;
  val_t v;

  l = oppose(l);

  for (it = cnf.index[l].begin(); it != cnf.index[l].end(); ++it) {
    v = evaluer_clause(etat, cnf.clauses[*it]);
    if (v == faux)
      return faux;
  }
  return indeterminee;
}

infos_retour_arriere_t affecte(etat_t & etat, lit_t & lit, cnf_t &cnf) {
  var_t i;
  infos_retour_arriere_t res;
  vector< int >::const_iterator it;
  lit_t olit;

  std::cout << "#var ";
  if (!positif(lit))
    cout << "-";
  cout << lit2var(lit).num << std::endl;
  i = lit2var(lit);
  res.litteral_affecte = lit;
  res.variable_precedement_affectee = etat.derniere_affectee;
  if (positif(lit))
    etat.valeurs[i.num] = vrai;
  else
    etat.valeurs[i.num] = faux;
  etat.derniere_affectee = i;

  for (it = cnf.index[lit].begin(); it != cnf.index[lit].end(); ++it) {
    cls_t &cls = cnf.clauses[*it];
    cls.nb_indet--;
    if (cls.nb_indet == 0)
      cls.est_vrai = true;
  }

  olit = oppose(lit);

  for (it = cnf.index[olit].begin(); it != cnf.index[olit].end(); ++it) {
    cls_t &cls = cnf.clauses[*it];
    cls.nb_indet--;
  }

  return res;
}

void retour_arriere(etat_t & etat, lit_t & lit, const infos_retour_arriere_t &
    infos, cnf_t &cnf) {
  var_t i;
  lit_t olit;
  vector< int >::const_iterator it;

  i = lit2var(lit);

  lit = infos.litteral_affecte;
  etat.derniere_affectee = infos.variable_precedement_affectee;
  etat.valeurs[i.num] = indeterminee;

  for (it = cnf.index[lit].begin(); it != cnf.index[lit].end(); ++it) {
    cls_t &cls = cnf.clauses[*it];
    cls.nb_indet++;
    if (evaluer_clause(etat, cls) == indeterminee)
      cls.est_vrai = false;
  }

  olit = oppose(lit);

  for (it = cnf.index[olit].begin(); it != cnf.index[olit].end(); ++it) {
    cls_t &cls = cnf.clauses[*it];
    cls.nb_indet++;
    if (evaluer_clause(etat, cls) == indeterminee)
      cls.est_vrai = false;
  }
}

lit_t choisit_litteral(const etat_t & etat, const cnf_t & jean) {
  vector< cls_t >::const_iterator it;
  set< lit_t >::const_iterator wars;
  vector< val_t >::const_iterator it2;
  cout << "valll " << valeur(etat, 9) <<endl;

  for (it = jean.clauses.begin(); it != jean.clauses.end(); ++it) { // Jean-Clauses Begin
    if (it->est_vrai == false && it->nb_indet == 1) {
      for (wars = it->litteraux.begin(); wars != it->litteraux.end(); ++wars) {
        if (valeur(etat, *wars) == faux)
          cout << "-";
        if (valeur(etat, *wars) == indeterminee)
          cout << "/";
        cout << lit2var(*wars).num << " ";
        if (valeur(etat, *wars) == indeterminee) // star wars
        {
          cout << "jedi" << endl;
          return *wars;                          // le retour du jedi
        }
      }
      cout << endl;
    }
  }

  for (it2 = etat.valeurs.begin(); it2 != etat.valeurs.end(); ++it2) {
    if (*it2 == indeterminee)
    {
      cout << "vendredi" << endl;
      return var2lit(var(*it2), true);
    }
  }

  cout << "shabbat" << endl;
  return NOLIT;
}

bool cherche(etat_t & etat, cnf_t & cnf) {
  lit_t lit;
  infos_retour_arriere_t ira;
  val_t res;

  lit = choisit_litteral(etat, cnf);
  if (lit == NOLIT)
    return true;

  ira = affecte(etat, lit, cnf); // on ira au lit si l'etat est fatigué

  res = evaluer_cnf(etat, cnf, lit);

  if (res == vrai)
    return cherche(etat, cnf);

  if (res == indeterminee && cherche(etat, cnf))
    return true;

  retour_arriere(etat, lit, ira, cnf);
  lit = oppose(lit);
  affecte(etat, lit, cnf);

  return cherche(etat, cnf);
}
