default: ../sat ../sudoku2dimacs ../res2sudoku

test: ../run-test
	(cd ..; ./run-test)

# parser: parser.tab.cpp parser.tab.hpp lex.yy.cpp

SUFFIXES += .d
#SRC := $(sort $(wildcard *.cpp) parser.tab.cpp lex.yy.cpp)
SRC := $(sort $(wildcard *.cpp))
MAINS := main-sat.o main-run-test.o main-sudoku2dimacs.o main-res2sudoku.o
TESTS := $(sort $(wildcard test-*.cpp))
TEST_OBJECTS := $(TESTS:%.cpp=%.o)
ALLOBJECTS := $(SRC:%.cpp=%.o)
OBJECTS := $(filter-out $(MAINS) $(TEST_OBJECTS),$(ALLOBJECTS))
LDDEPS := -lm
TEST_LDDEPS := $(LDDEPS) -L../unittest-cpp -lUnitTest++
CXXFLAGS := -Wall -Wextra -std=c++0x -I../unittest-cpp/src -ggdb3
EXECUTABLES := ../sat ../sudoku2dimacs ../res2sudoku ../run-test
UNITTEST := ../unittest-cpp/libUnitTest++.a
$(UNITTEST):
	make -C ../unittest-cpp

clean:
	rm -f *.d *.o 
	rm -f *~ 
	rm -f $(EXECUTABLES)
	make -C ../unittest-cpp clean

help:
	sed -n -e 's/^#HELP\(.*\)$$/\1/p' < Makefile

#HELP
#HELP pour afficher l'aide du Makefile
#HELP make help
#HELP
#HELP pour compiler le projet
#HELP make 
#HELP
#HELP pour lancer les tests
#HELP make test
#HELP
#HELP pour nettoyer les fichiers compilés et temporaires
#HELP make clean
#HELP


../run-test: $(OBJECTS) $(TEST_OBJECTS) main-run-test.o $(UNITTEST)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(TEST_OBJECTS) main-run-test.o $(TEST_LDDEPS) -o ../run-test

$(filter-out ../run-test,$(EXECUTABLES)): ../%:  $(OBJECTS) main-%.o
	$(CXX) $(CXXFLAGS) $(OBJECTS) main-$*.o $(LDDEPS) -o $@

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MM $*.cpp -MF $*.d

ifneq ($(subst parser,,$(MAKECMDGOALS)),clean) 
include $(ALLOBJECTS:%.o=%.d)
endif

.PHONY: default test parser

