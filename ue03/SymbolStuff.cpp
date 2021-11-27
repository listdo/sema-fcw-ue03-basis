// SymbolStuff.cpp:                                       HDO, 2004-2020
// ---------------
// Abstract base class Symbol is an interface for T- and NTSymbol only.
// Objects of the derived classes TSymbol and NTSymbol represent terminal
// and nonterminal symbols for use in (different) grammars respectively.
// Class SymbolStuff provides a garbage collecting singleton
// that has factory methods for T- and NTSymbols.
// =====================================================================

#include <iostream>
#include <stdexcept>

using namespace std;

#include "SymbolStuff.h"


static inline void checkForEmptyString(const string &name) {
  if (name.length() == 0)
    throw invalid_argument("invalid empty string for symbol name");
} // checkForEmptyString


// === implementation of class SymbolPool ==============================

SymbolPool *SymbolPool::instance = nullptr;

SymbolPool *SymbolPool::getInstance() {
  if (instance == nullptr)
    instance = new SymbolPool();
  return instance;
} // SymbolPool::getInstance

SymbolPool::~SymbolPool() {
  for (auto e: tSyMap) {
    delete e.second;
  } // for
  for (auto e: ntSyMap) {
    delete e.second;
  } // for
  instance = nullptr;
} // SymbolPool::~SymbolPool


TSymbol *SymbolPool::tSymbol(const string name) {
  checkForEmptyString(name);
  TSymbol *tSy = tSyMap[name];
  if (tSy == nullptr) {
    if (ntSyMap.find(name) != ntSyMap.end())
      cout << "WARNING: new terminal " << name <<
              " aliases old nonterminal" << endl;
    tSy = new TSymbol(name);
    tSyMap[name] = tSy;
  } // if
  return tSy;
} // SymbolPool::tSymbol

NTSymbol *SymbolPool::ntSymbol(const string name) {
  checkForEmptyString(name);
  NTSymbol *ntSy = ntSyMap[name];
  if (ntSy == nullptr) {
    if (tSyMap.find(name) != tSyMap.end())
      cout << "WARNING: new nonterminal " << name <<
              " aliases old terminal" << endl;
    ntSy = new NTSymbol(name);
    ntSyMap[name] = ntSy;
  } // if
  return ntSy;
} // SymbolPool::ntSymbol

Symbol *SymbolPool::symbolFor(const std::string &name) const {
  checkForEmptyString(name);
  auto tSyMapIt = tSyMap.find(name);
  if (tSyMapIt != tSyMap.end())
    return tSyMapIt->second;  // tSymbol
  auto ntSyMapIt = ntSyMap.find(name);
  if (ntSyMapIt != ntSyMap.end())
    return ntSyMapIt->second; // ntSymbol
  return nullptr;
} // SymbolPool::symbolFo


std::ostream &operator<<(std::ostream &os, const SymbolPool &sp) {
  os << "symbol pool: " <<
        sp. tSyMap.size() << " terminals and " <<
        sp.ntSyMap.size() << " nonterminals" << endl;
#if (1) // with contents
  bool first = true;
  os << "  terminals    = { ";
  for (auto e: sp.tSyMap) {
    if (!first)
      os << ", ";
    os << *e.second;
    first = false;
  } // for
  os << " }" << endl;
  first = true;
  os << "  nonterminals = { ";
  for (auto e: sp.ntSyMap) {
    if (!first)
      os << ", ";
    os << *e.second;
    first = false;
  } // for
  os << " }" << endl;
#endif
  return os;
} // operator<<


// === implementation of class Symbol ==================================

Symbol::Symbol(const string &name)
: name(name) {
  // nothing to do
} // Symbol::Symbol

Symbol::~Symbol() { // implementation of abstract destructor
  // nothing to do
} // Symbol::~Symbol


bool Symbol::isT() const {
  return typeid(*this) == typeid( TSymbol);
} // Symbol::isT

bool Symbol::isNT() const {
  return typeid(*this) == typeid(NTSymbol);
} // Symbol::isT

int Symbol::compare(const Symbol &sy) const {
  return this->name.compare(sy.name);
} // Symbol::compare

bool isT(const Symbol *sy) {
   return sy->isT();
} // isT

bool isNT(const Symbol *sy) {
   return sy->isNT();
} // isNT


bool operator<(const Symbol &sy1, const Symbol &sy2) {
  return sy1.name < sy2.name;
} // operator<

bool operator>(const Symbol &sy1, const Symbol &sy2) {
  return sy1.name > sy2.name;
} // operator>

bool operator==(const Symbol &sy1, const Symbol &sy2) {
  return sy1.name == sy2.name;
} // operator==

bool operator!=(const Symbol &sy1, const Symbol &sy2) {
  return sy1.name != sy2.name;
} // operator!=


bool LessForSymbolPtrs::operator()(const Symbol* sy1,
                                   const Symbol* sy2) const {
  return sy1->name < sy2->name;
} // LessForSymbolPtrs::operator()

bool EqualForSymbolPtrs::operator()(const Symbol* sy1,
                                    const Symbol* sy2) const {
  return sy1->name == sy2->name;
} // EqualForSymbolPtrs::operator()


ostream &operator<<(ostream &os, const Symbol &sy) {
  os << sy.name;
  return os;
} // operator<<


// === implementation of class TSymbol =================================

TSymbol::TSymbol(const string &name)
 : Symbol(name) {
  // nothing to do
} // TSymbol::TSymbol


// === implementation of class NTSymbol ================================

NTSymbol::NTSymbol(const string &name)
 : Symbol(name) {
  // nothing to do
} // NTSymbol::NTSymbol


// === test ============================================================

#if 0

#include <cstdio>
#include <typeinfo>

#ifdef TEST
#error previously included cpp file already defines a main function for testing
#endif
#define TEST

int main(int argc, char *argv[]) {
try {

  cout << "START: SymbolStuff test" << endl;
  cout << endl;

  SymbolPool *sp = SymbolPool::getInstance();
  cout << *sp << endl;

  Symbol  * sy;

  TSymbol *tSy1, *tSy2;

  tSy1 = sp->tSymbol("a");
  tSy2 = sp->tSymbol("b");

  sy = tSy1;

  cout << "tSy1               : " << tSy1       << endl;
  cout << "tSy2               : " << tSy2       << endl;
  cout << "tSy1->name         : " << tSy1->name << endl;
  cout << "tSy2->name         : " << tSy2->name << endl;
  cout << "tSy1->compare(*tSy2): " << tSy1->compare(*tSy2) << endl;
  cout << " tSy1 <  tSy2      : " << ( tSy1 <  tSy2) << endl;
  cout << "*tSy1 < *tSy2      : " << (*tSy1 < *tSy2) << endl;
  cout << "type of tSy1 is T  : " << tSy1->isT() << endl;
  cout << "type of tSy2 is NT : " << isNT(tSy2)  << endl;

  NTSymbol *ntSy1;

  ntSy1 = sp->ntSymbol("NT");

  sy   = ntSy1;
  cout << "ntSy1: " << *ntSy1         << endl;
  cout << "kind:  " <<  ntSy1->isNT() << endl;

  TSymbol  *tSy3  = sp-> tSymbol("NT"); // alias
  NTSymbol *ntSy2 = sp->ntSymbol("a");  // alias

  cout << endl;
  cout << *sp << endl;
  delete sp;

  cout << endl;
  cout << "END" << endl;

} catch(const exception &e) {
  cerr <<  "ERROR (" << typeid(e).name() << "): " << e.what() << endl;
} // catch

  // cout << "type CR to continue ...";
  // getchar();

  return 0;
} // main

#endif


// end of SymbolStuff.cpp
//======================================================================
