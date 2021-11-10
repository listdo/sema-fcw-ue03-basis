// DFA.h:                                                 HDO, 2006-2019
// -----
// Objects of class DFA represent deterministic finite automata.
//======================================================================

#ifndef DFA_h
#define DFA_h

#include "ObjectCounter.h"
#include "TapeStuff.h"
#include "StateStuff.h"
#include "FA.h"


class FABuilder;           // forward for friend declaration only

class DFA: public  FA
 /*OC+*/ , private ObjectCounter<DFA> /*+OC*/ {

  friend class FABuilder;  // so ::build... methods can call prot. constr.

  private:

    typedef FA Base;

  protected: // allows derived classes, e.g., for Mealy and or Moore

    // constructor called by FABuilder::build... methods only
    DFA(const StateSet &S,  const TapeSymbolSet &V,
        const State    &s1, const StateSet      &F,
        const DDelta   &delta);

    virtual StateSet deltaAt(const State &src, TapeSymbol tSy) const;

  public:

    const DDelta delta;    // deterministic transition function

    DFA(const DFA  &dfa) = default;
    DFA(      DFA &&dfa) = default;

    virtual ~DFA() = default;

    virtual void onStateEntered(State s) const;

    virtual bool accepts(const Tape &tape) const; // impl. of abstr. meth.

    DFA *minimalOf() const; // minimization: DFA => minimal DFA

    DFA *renamedOf() const; // equiv. automation with states named 0, 1, ...

}; // DFA


#endif

// end of DFA.h
//======================================================================



