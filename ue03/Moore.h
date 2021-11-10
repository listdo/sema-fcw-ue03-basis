// DFA.h:                                                 HDO, 2006-2019
// -----
// Objects of class DFA represent deterministic finite automata.
//======================================================================

#ifndef Moore_h
#define Moore_h

#include "ObjectCounter.h"
#include "TapeStuff.h"
#include "StateStuff.h"
#include "DFA.h"


class FABuilder;           // forward for friend declaration only

class Moore: public DFA
 /*OC+*/ , private ObjectCounter<Moore> /*+OC*/ {

  friend class FABuilder;  // so ::build... methods can call prot. constr.

  private:

    typedef DFA Base;

  protected: // allows derived classes, e.g., for Mealy and or Moore

    // constructor called by FABuilder::build... methods only
    Moore(const StateSet         &S,  const TapeSymbolSet &V,
        const State            &s1, const StateSet      &F,
        const DDelta           &delta,
        const map<State, char> lambda);

  public:

    const map<State, char> lambda;

    virtual ~Moore() = default;

    virtual void onStateEntered(State s) const;

}; // Moore


#endif

// end of Moore.h
//======================================================================



