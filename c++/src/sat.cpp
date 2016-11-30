#include "sat.hpp"
#include <stack>
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
  res.litteraux = litteraux;
  return res;
}

void ajouteLitteral(lit_t l, cls_t & cls) {
  cls.litteraux.insert(l);
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

  cnf.clauses.push_back(clause);
  clauseidx = cnf.clauses.size() - 1;

  for (it = clause.litteraux.begin(); it != clause.litteraux.end(); ++it) {
    if (*it + 1 >= cnf.index.size())
      cnf.index.resize(*it + 2);
    cnf.index[*it].push_back(&cnf.clauses[clauseidx]);
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
  vector< cls_t >::const_iterator it;
  bool indet = false;
  val_t v;

  l = oppose(l);

  for (it = cnf.index[l].begin(); it != cnf.index[l].end(); ++it) {
    v = evaluer_clause(etat, *it);
    if (v == faux)
      return faux;
    if (v == indeterminee)
      indet = true;
  }
  if (indet == true)
    return indeterminee;
  return vrai;
}

infos_retour_arriere_t affecte(etat_t & etat, lit_t & lit) {
  var_t i;
  infos_retour_arriere_t res;

  i = lit2var(lit);
  res.litteral_affecte = lit;
  res.variable_precedement_affectee = etat.derniere_affectee;
  if (positif(lit))
    etat.valeurs[i.num] = vrai;
  else
    etat.valeurs[i.num] = faux;
  etat.derniere_affectee = i;

  return res;
}

void retour_arriere(etat_t & etat, lit_t & lit, const infos_retour_arriere_t &
    infos) {
  var_t i;

  i = lit2var(lit);

  lit = infos.litteral_affecte;
  etat.derniere_affectee = infos.variable_precedement_affectee;
  etat.valeurs[i.num] = indeterminee;
}

lit_t choisit_litteral(const etat_t & etat, const cnf_t & cnf) {
  if (etat.derniere_affectee.num + 1 <= (int)etat.valeurs.size())
    return var2lit(var(etat.derniere_affectee.num + 1), true);
  return NOLIT;
}

bool cherche(etat_t & etat, cnf_t & cnf) {
  lit_t lit;
  infos_retour_arriere_t ira;
  val_t res;

  //while ((lit = choisit_litteral(etat, cnf)) != NOLIT) {
  //  ira = affecte(etat, lit);
  //  stack.push([ira, lit, true]); //TODO
  //  res = evaluer_cnf(etat, cnf);
  //  if (res == vrai)
  //    return true;
  //  while (res == faux) {
  //    [ira, lit, b] = stack.pop;
  //    retour_arriere(etat, cnf);
  //    if (b) {
  //      ira = affecte(etat, lit);
  //      stack.push([ira, lit, false]);
  //      break;
  //    }
  //    res = evaluer_cnf(cnf, lit);
  //  }
  //}

  lit = choisit_litteral(etat, cnf);
  if (lit == NOLIT)
    return true;

  ira = affecte(etat, lit); // on ira au lit si l'etat est fatigué

  res = evaluer_cnf(etat, cnf);

  if (res == vrai)
    return cherche(etat, cnf);

  if (res == indeterminee && cherche(etat, cnf))
    return true;

  retour_arriere(etat, lit, ira);
  lit = oppose(lit);
  affecte(etat, lit);

  return cherche(etat, cnf);
}
