#include "sat.hpp"
#include <UnitTest++.h>

TEST(var2lit){
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  CHECK_EQUAL(var2lit(quatre, true), (lit_t)8);
  CHECK_EQUAL(var2lit(quatre, false), (lit_t)9);
  CHECK_EQUAL(var2lit(zero, true), (lit_t)0);
  CHECK_EQUAL(var2lit(zero, false), (lit_t)1);
}

TEST(lit2var){
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  CHECK_EQUAL(lit2var(8).num, quatre.num);
  CHECK_EQUAL(lit2var(9).num, quatre.num);
  CHECK_EQUAL(lit2var(0).num, zero.num);
  CHECK_EQUAL(lit2var(1).num, zero.num);
}

TEST(positif){
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  CHECK_EQUAL(positif(var2lit(quatre, true)), true);
  CHECK_EQUAL(positif(var2lit(quatre, false)), false);
  CHECK_EQUAL(positif(var2lit(zero, true)), true);
  CHECK_EQUAL(positif(var2lit(zero, false)), false);
}

TEST(oppose){
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  CHECK_EQUAL(oppose(var2lit(quatre, true)), var2lit(quatre, false));
  CHECK_EQUAL(oppose(var2lit(quatre, false)), var2lit(quatre, true));
  CHECK_EQUAL(oppose(var2lit(zero, true)), var2lit(zero, false));
  CHECK_EQUAL(oppose(var2lit(zero, false)), var2lit(zero, true));
}

TEST(clause){
  set<lit_t> vide;
  set<lit_t> singleton;
  set<lit_t> couple;
  set<lit_t> quelconque;
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  singleton.insert(var2lit(zero, true));
  couple.insert(var2lit(zero, true));
  couple.insert(var2lit(quatre, false));
  for (int i = 0; i < 42; i++)
  {
    var_t temp;

    temp.num = i;
    quelconque.insert(var2lit(temp, i % 2));
  }
  CHECK(clause(vide).litteraux.empty());
  CHECK_EQUAL(clause(singleton).litteraux.size(), (size_t)1);
  CHECK(clause(singleton).litteraux == singleton);
  CHECK_EQUAL(clause(couple).litteraux.size(), (size_t)2);
  CHECK(clause(couple).litteraux == couple);
  CHECK_EQUAL(clause(quelconque).litteraux.size(), (size_t)42);
  CHECK(clause(quelconque).litteraux == quelconque);
}

TEST(ajouteLitteral){
  set<lit_t> vide;
  cls_t singleton;
  cls_t couple;
  cls_t quelconque;
  var_t quatre;
  var_t zero;

  quatre.num = 4;
  zero.num = 0;
  singleton = clause(vide);
  CHECK_EQUAL(singleton.litteraux.size(), (size_t)0);
  ajouteLitteral(var2lit(zero, true), singleton);
  CHECK_EQUAL(singleton.litteraux.size(), (size_t)1);
  CHECK(singleton.litteraux.find(var2lit(zero, true)) != singleton.litteraux.end());

  couple = clause(vide);
  CHECK_EQUAL(couple.litteraux.size(), (size_t)0);
  ajouteLitteral(var2lit(zero, true), couple);
  ajouteLitteral(var2lit(quatre, false), couple);
  CHECK_EQUAL(couple.litteraux.size(), (size_t)2);
  CHECK(couple.litteraux.find(var2lit(zero, true)) != couple.litteraux.end());
  CHECK(couple.litteraux.find(var2lit(quatre, false)) != couple.litteraux.end());

  quelconque = clause(vide);
  CHECK_EQUAL(quelconque.litteraux.size(), (size_t)0);
  for (int i = 0; i < 42; i++)
  {
    var_t temp;

    temp.num = i;
    ajouteLitteral(var2lit(temp, i % 2), quelconque);
    CHECK_EQUAL(quelconque.litteraux.size(), (size_t)i + 1);
    CHECK(quelconque.litteraux.find(var2lit(temp, i % 2)) != quelconque.litteraux.end());
  }
}

bool operator==(const cls_t &c1, const cls_t &c2)
{
  return c1.litteraux == c2.litteraux;
}

TEST(cnf){
  set<lit_t> vide;
  cls_t c_vide;
  cls_t singleton;
  cls_t couple;
  cls_t quelconque;
  var_t quatre;
  var_t zero;
  vector<cls_t> v_vide;
  vector<cls_t> v_singleton;
  vector<cls_t> v_couple;
  vector<cls_t> v_quelconque;

  quatre.num = 4;
  zero.num = 0;
  singleton = clause(vide);
  ajouteLitteral(var2lit(zero, true), singleton);

  couple = clause(vide);
  ajouteLitteral(var2lit(zero, true), couple);
  ajouteLitteral(var2lit(quatre, false), couple);

  quelconque = clause(vide);
  for (int i = 0; i < 42; i++)
  {
    var_t temp;

    temp.num = i;
    ajouteLitteral(var2lit(temp, i % 2), quelconque);
  }

  CHECK(cnf(v_vide).clauses.empty());
  v_singleton.push_back(c_vide); // c'est sous vide
  CHECK_EQUAL(cnf(v_singleton).clauses.size(), (size_t)1);
  CHECK(cnf(v_singleton).clauses == v_singleton);

  v_singleton.clear();
  v_singleton.push_back(singleton);
  CHECK_EQUAL(cnf(v_singleton).clauses.size(), (size_t)1);
  CHECK(cnf(v_singleton).clauses == v_singleton);

  v_singleton.clear();
  v_singleton.push_back(couple);
  CHECK_EQUAL(cnf(v_singleton).clauses.size(), (size_t)1);
  CHECK(cnf(v_singleton).clauses == v_singleton);

  v_singleton.clear();
  v_singleton.push_back(quelconque);
  CHECK_EQUAL(cnf(v_singleton).clauses.size(), (size_t)1);
  CHECK(cnf(v_singleton).clauses == v_singleton);

  v_couple.push_back(c_vide);
  v_couple.push_back(singleton);
  CHECK_EQUAL(cnf(v_couple).clauses.size(), (size_t)2);
  CHECK(cnf(v_couple).clauses == v_couple);

  v_couple.clear();
  v_couple.push_back(couple);
  v_couple.push_back(quelconque);
  CHECK_EQUAL(cnf(v_couple).clauses.size(), (size_t)2);
  CHECK(cnf(v_couple).clauses == v_couple);

  for (int n_cls = 0; n_cls < 42; n_cls++)
  {
    cls_t tmp;
    for (int n_lit = 0; n_lit < 42; n_lit++)
    {
      var_t v;

      v.num = n_lit;
      ajouteLitteral(var2lit(v, n_lit % 2), tmp);
    }
    v_quelconque.push_back(tmp);
  }
  CHECK_EQUAL(cnf(v_quelconque).clauses.size(), (size_t)42);
  CHECK(cnf(v_quelconque).clauses == v_quelconque);
}

bool vectorHas(const vector<cls_t> &v, const cls_t e){
  vector<cls_t>::const_iterator it;

  for (it = v.begin(); it != v.end(); ++it){
    if (*it == e)
      return true;
  }

  return false;
}

TEST(ajouteClause){
  set<lit_t> vide;
  cls_t c_vide;
  cls_t singleton;
  cls_t couple;
  cls_t quelconque;
  var_t quatre;
  var_t zero;
  vector<cls_t> v_vide;
  cnf_t cnf_vide;
  cnf_t cnf_singleton;
  cnf_t cnf_couple;
  cnf_t cnf_quelconque;

  quatre.num = 4;
  zero.num = 0;
  singleton = clause(vide);
  ajouteLitteral(var2lit(zero, true), singleton);

  couple = clause(vide);
  ajouteLitteral(var2lit(zero, true), couple);
  ajouteLitteral(var2lit(quatre, false), couple);

  quelconque = clause(vide);
  for (int i = 0; i < 42; i++)
  {
    var_t temp;

    temp.num = i;
    ajouteLitteral(var2lit(temp, i % 2), quelconque);
  }

  CHECK(cnf(v_vide).clauses.empty());
  cnf_singleton = cnf(v_vide);
  ajouteClause(c_vide, cnf_singleton);
  CHECK_EQUAL(cnf_singleton.clauses.size(), (size_t)1);
  CHECK(vectorHas(cnf_singleton.clauses, c_vide));

  cnf_singleton = cnf(v_vide);
  ajouteClause(singleton, cnf_singleton);
  CHECK_EQUAL(cnf_singleton.clauses.size(), (size_t)1);
  CHECK(vectorHas(cnf_singleton.clauses, singleton));

  cnf_singleton = cnf(v_vide);
  ajouteClause(couple, cnf_singleton);
  CHECK_EQUAL(cnf_singleton.clauses.size(), (size_t)1);
  CHECK(vectorHas(cnf_singleton.clauses, couple));

  cnf_singleton = cnf(v_vide);
  ajouteClause(quelconque, cnf_singleton);
  CHECK_EQUAL(cnf_singleton.clauses.size(), (size_t)1);
  CHECK(vectorHas(cnf_singleton.clauses, quelconque));

  cnf_couple = cnf(v_vide);
  ajouteClause(c_vide, cnf_couple);
  ajouteClause(singleton, cnf_couple);
  CHECK_EQUAL(cnf_couple.clauses.size(), (size_t)2);
  CHECK(vectorHas(cnf_couple.clauses, c_vide));
  CHECK(vectorHas(cnf_couple.clauses, singleton));

  cnf_couple = cnf(v_vide);
  ajouteClause(couple, cnf_couple);
  ajouteClause(quelconque, cnf_couple);
  CHECK_EQUAL(cnf_couple.clauses.size(), (size_t)2);
  CHECK(vectorHas(cnf_couple.clauses, couple));
  CHECK(vectorHas(cnf_couple.clauses, quelconque));

  cnf_quelconque = cnf(v_vide);
  for (int n_cls = 0; n_cls < 42; n_cls++)
  {
    cls_t tmp;
    for (int n_lit = 0; n_lit < 42; n_lit++)
    {
      var_t v;

      v.num = n_lit;
      ajouteLitteral(var2lit(v, n_lit % 2), tmp);
    }
    ajouteClause(tmp, cnf_quelconque);
    CHECK(vectorHas(cnf_quelconque.clauses, tmp));
  }
  CHECK_EQUAL(cnf_quelconque.clauses.size(), (size_t)42);
}
