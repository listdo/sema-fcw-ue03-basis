// DFA.cpp:                                               HDO, 2006-2019
// -------
// Objects of class DFA represent deterministic finite automata.
//======================================================================

#include <cmath>
#include <cstring>

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

#include "TapeStuff.h"
#include "StateStuff.h"
#include "MbMatrix.h"
#include "Moore.h"
#include "FABuilder.h"


// --- implementation of class Moore ---

Moore::Moore(const StateSet &S,  const TapeSymbolSet &V,
         const State    &s1, const StateSet      &F,
         const DDelta   &delta,
         const map<State, char> lambda)
: DFA(S, V, s1, F, delta), lambda(lambda) {
} // Moore::Moore


void Moore::onStateEntered(State s) const {
   cout << lambda.at(s);
} // Moore::onStateEntered


// end of Moore.cpp
//======================================================================
