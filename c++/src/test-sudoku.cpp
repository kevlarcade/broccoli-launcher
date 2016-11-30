#include "sat.hpp"
#include "sudoku.hpp"
#include <UnitTest++.h>

TEST(genere_probleme_sat) {
  sudoku count_dooku;
  count_dooku.resize(2);
  count_dooku[0].push_back(1);
  count_dooku[0].push_back(0);
  count_dooku[1].push_back(2);
  count_dooku[1].push_back(0);
  std::ostringstream stream;;
  genere_probleme_sat(stream, count_dooku);
  std::string str = stream.str();
  CHECK(str == "1 2 \n-1 -2\n3 4 \n-3 -4\n5 6 \n-5 -6\n7 8 \n-7 -8\n1 3 \n"
      "-1 -3\n5 7 \n-5 -7\n2 4 \n-2 -4\n6 8 \n-6 -8\n1 5 \n-1 -5\n3 7 \n-3 -7\n"
      "2 6 \n-2 -6\n4 8 \n-4 -8\n1\n6\n");
}
