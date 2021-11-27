// SymbolStuff.h:                                         HDO, 2004-2020
// -------------
// Abstract base class Symbol is an interface for T- and NTSymbol only.
// Objects of derived classes TSymbol and NTSymbol represent terminal and
// non-terminal symbols for use in grammars respectively.
// Class SymbolPool provides a singleton object with factory methods
// for T- and NTSymbols, it is an implementation of the flyweight pattern.
// =====================================================================

#ifndef SymbolStuff_h
#define SymbolStuff_h

#include <iosfwd>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

#include "ObjectCounter.h"


class   SymbolPool;  // singleton providing factory methods for T- and NTSymbols,
                     //   symbol pool has the ownership for these symbols

class   Symbol;      // abstract base class for
class  TSymbol;      // 1. concrete class  TSymbol and
class NTSymbol;      // 2. concrete class NTSymbol


// === class SymbolPool ================================================

class SymbolPool final // no public base class
        /*OC+*/ : private ObjectCounter<SymbolPool> /*+OC*/ {

  friend std::ostream &operator<<(std::ostream &os, const SymbolPool &sp);

  private:

    static SymbolPool *instance; // pointer to the singleton SymbolPool

    std::unordered_map<std::string,  TSymbol *>  tSyMap;
    std::unordered_map<std::string, NTSymbol *> ntSyMap;

    SymbolPool() = default; // for singleton pattern only
    SymbolPool(const SymbolPool *sp) = delete;
    SymbolPool &operator=(const SymbolPool *sp) = delete;

  public:

    static SymbolPool *getInstance(); // returns a pointer to the singleton,
                                      //   to be deleted at the end of program

    ~SymbolPool(); // not virtual because of final class

    // special kind of factory methods for symbols,
    //   return pointers to existing or newly constructed symbols
     TSymbol * tSymbol(const std::string name);
    NTSymbol *ntSymbol(const std::string name);

    // lookup method to retrieve existing symbol,
    //   returns nullptr for unknown name
    Symbol *symbolFor(const std::string &name) const;

}; // SymbolPool

std::ostream &operator<<(std::ostream &os, const SymbolPool &sp);


// === class Symbol ====================================================

class Symbol { // abstract base class, so no object counting necessary

private:

    Symbol(const Symbol *sy) = delete;
    Symbol &operator=(const Symbol *sy) = delete;

  protected:

    Symbol(const std::string &name);
    virtual ~Symbol() = 0;     // abstract to gain an abstract class
                               //   but implementation is necessary (!)

  public:

    const std::string name;    // const as symbols may be elements of sets

    virtual bool isT () const; // is symbol a T  in a grammar
    virtual bool isNT() const; // is symbol a NT in a grammar

    int compare(const Symbol &sy) const;

}; // Symbol

bool isT (const Symbol *sy);   // sy->isT () <==> isT (sy)
bool isNT(const Symbol *sy);   // sy->isNT() <==> isNT(sy)

bool operator< (const Symbol &sy1, const Symbol &sy2);
bool operator> (const Symbol &sy1, const Symbol &sy2);
bool operator==(const Symbol &sy1, const Symbol &sy2);
bool operator!=(const Symbol &sy1, const Symbol &sy2);

struct LessForSymbolPtrs {
  bool operator()(const Symbol *sy1, const Symbol *sy2) const;
}; // LessForSymbolPtrs

struct EqualForSymbolPtrs {
  bool operator()(const Symbol *sy1, const Symbol *sy2) const;
}; // EqualForSymbolPtrs

std::ostream &operator<<(std::ostream &os, const Symbol &sy);


// === class TSymbol ===================================================

class TSymbol: public Symbol
  /*OC+*/ , private ObjectCounter<TSymbol> /*+OC*/ {

  friend class SymbolPool; // for construction and destruction only

  private:

    TSymbol(const TSymbol *tSy) = delete;
    TSymbol &operator=(const TSymbol *tSy) = delete;

  protected:

    TSymbol(const std::string &name);
    virtual ~TSymbol() = default;

}; // TSymbol


// === class NTSymbol ==================================================

class NTSymbol: public Symbol
  /*OC+*/ , private ObjectCounter<NTSymbol> /*+OC*/ {

  friend class SymbolPool; // for construction and destruction only

  private:

    NTSymbol(const NTSymbol *tSy) = delete;
    NTSymbol &operator=(const NTSymbol *tSy) = delete;

  protected:

    NTSymbol(const std::string &name);
    virtual ~NTSymbol() = default;

}; // NTSymbol


#endif

// end of SymbolStuff.h
// =====================================================================
