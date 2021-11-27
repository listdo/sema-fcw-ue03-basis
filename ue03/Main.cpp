// Main.cpp:                                              HDO, 2006-2021
// --------
// Simple main program for
// *     deterministic finite automata (DFA) and
// * non-deterministic fintie automata (NFA).
//======================================================================

#include <cstdio>          // for getchar only
#include <iostream>
#include <stdexcept>

using namespace std;

#include "SignalHandling.h"
#include "TapeStuff.h"
#include "StateStuff.h"
#include "MbMatrix.h"
#include "DeltaStuff.h"
#include "FA.h"
#include "DFA.h"
#include "NFA.h"
#include "Moore.h"
#include "FABuilder.h"
#include "GraphVizUtil.h"


// Activation (1) allows simple builds via command line:
// * for GNU   use:  g++      -std=c++17 Main.cpp
// * for Clang use:  clang++  -std=c++17 Main.cpp
// * for M.S.  use:  cl /EHsc /std:c++17 Main.cpp
#if 0
#include "SignalHandling.cpp"
#include "TapeStuff.cpp"
#include "StateStuff.cpp"
#include "MbMatrix.cpp"
#include "DeltaStuff.cpp"
#include "FA.cpp"
#include "DFA.cpp"
#include "NFA.cpp"
#include "Moore.cpp"
#include "FABuilder.cpp"
#include "GraphVizUtil.cpp"
#endif
#include "Grammar.h"
#include "GrammarBuilder.h"

/* FUNCTION DEFINITIONS */
FA* faOf(const Grammar* g);
Grammar* grammarOf(const NFA* nfa);


int main(int argc, char* argv[]) {

	FABuilder* fab = nullptr;
	DFA* dfa = nullptr;
	NFA* nfa = nullptr;
	DFA* dfaOfNfa = nullptr;
	DFA* minDfaOfNfa = nullptr;
	DFA* renMinDfaOfNfa = nullptr;

	Moore* moore = nullptr;

	installSignalHandlers(); // to catch signals, especially SIGSEGV

	cout << "START: Main" << endl;
	cout << endl;

	try {
		SymbolPool* sp = SymbolPool::getInstance();
		Grammar* g = GrammarBuilder(string("G.txt")).buildGrammar();

		// or for short: g2 = GrammarBuilder(string("G.txt")).buildGrammar();
		cout << "grammar from text file:" << endl << *g << endl;

		FA* faOfGrammar = faOf(g);

		cout << "faOfGrammar->accepts(\"aab\") = " << faOfGrammar->accepts("aab") << endl;
		cout << "faOfGrammar->accepts(\"aaacaab\")   = " << faOfGrammar->accepts("aaacaab") << endl;

		vizualizeFA("faOfGrammar", faOfGrammar);

		fab = new FABuilder(
			"-> 0 -> 0 1 | 1 2 \n\
			 () 1 -> 0 1 | 1 2 \n\
			 () 2 -> 0 1 | 1 2    ");

		fab->setLambda({ {"0", ' '},
						 {"1", 'F'},
						 {"2", 'T'} });

		moore = fab->buildMoore();
		cout << moore->accepts("1010") << endl;

		cout << "1. DFA" << endl;
		cout << "------" << endl;
		cout << endl;

		int buildCase = 1;
		switch (buildCase) {
		case 1: // programmatical init. of builder
			fab = new FABuilder(); // example from FS slides p. 47
			fab->setStartState("B").
				addFinalState("R").
				addTransition("B", 'b', "R").
				addTransition("R", 'b', "R").
				addTransition("R", 'z', "R");
			break;
		case 2: // init. builder from text file
			fab = new FABuilder(string("IdDFA.txt"));
			break;
		case 3: // init. builder from C string literal
			fab = new FABuilder(
				"-> B -> b R  \n\
				 () R -> b R | z R");
			break;
		default:
			throw runtime_error("invalid buildCase");
		} // switch

		dfa = fab->buildDFA();
		delete fab;

		cout << "dfa:" << endl << *dfa;
		vizualizeFA("dfa", dfa);

		cout << "dfa->accepts(\"bzb\") = " << dfa->accepts("bzb") << endl;
		cout << "dfa->accepts(\"z\")   = " << dfa->accepts("z") << endl;
		cout << endl;

		cout << "type CR to continue ... ";
		// getchar();
		cout << endl;


		cout << "2. NFA" << endl;
		cout << "------" << endl;
		cout << endl;

		fab = new FABuilder( // Floyd, Beigel: The Language of Machines, p. 255
			"-> 1 -> a 2 | b 1        \n\
           () 2 -> a 2 | b 1 | b 3  \n\
              3 -> a 2 | b 4        \n\
           () 4 -> a 4 | b");
		nfa = fab->buildNFA();
		delete fab;

		auto grammarOfNfa = grammarOf(nfa);
		cout << "Grammar of NFA:" << endl << *grammarOfNfa << endl;
		
		
		// TODO  
		//cout << "nfa:" << endl << *nfa;
		//vizualizeFA("nfa", nfa);

		//cout << "nfa->accepts1(\"abba\") = " << nfa->accepts1("abba") << endl;
		//cout << "nfa->accepts2(\"abba\") = " << nfa->accepts2("abba") << endl;
		//cout << "nfa->accepts3(\"abba\") = " << nfa->accepts3("abba") << endl;
		//cout << "nfa->accepts1(\"bbab\") = " << nfa->accepts1("bbab") << endl;
		//cout << "nfa->accepts2(\"bbab\") = " << nfa->accepts2("bbab") << endl;
		//cout << "nfa->accepts3(\"bbab\") = " << nfa->accepts3("bbab") << endl;
		//cout << endl;

		//cout << "transforming nfa to dfaOfNfa ..." << endl;
		//dfaOfNfa = nfa->dfaOf();
		//cout << "dfaOfNfa:" << endl << *dfaOfNfa;
		//vizualizeFA("dfaOfNfa", dfaOfNfa);

		//cout << "minimizing dfaOfNfa to minDfaOfNfa ..." << endl;
		//minDfaOfNfa = dfaOfNfa->minimalOf();
		//cout << "minDfaOfNfa:" << endl << *minDfaOfNfa;
		//vizualizeFA("minDfaOfNfa", minDfaOfNfa);

		//cout << "renaming minDfaOfNfa to renMinDfaOfNfa ..." << endl;
		//renMinDfaOfNfa = minDfaOfNfa->renamedOf();
		//cout << "renMinDfaOfNfa:" << endl << *renMinDfaOfNfa;
		//vizualizeFA("renMinDfaOfNfa", renMinDfaOfNfa);


	}
	catch (const exception& e) {
		cerr << "EXCEPTION (" << typeid(e).name() << "): " << e.what() << endl;
	} // catch

	  // fab has already been deleted
	delete dfa;
	delete nfa;
	delete dfaOfNfa;
	delete minDfaOfNfa;
	delete renMinDfaOfNfa;

	cout << endl;
	cout << "END Main" << endl;

	return 0;

} // main

/* FUNCTION IMPLEMENTATIONS */
FA* faOf(const Grammar* g)
{
	FABuilder*			fab = new FABuilder();
	const auto			NTs = g->vNt;
	const auto			startSymbol = g->root;
	const string 		FIN_STATE = "FIN";

	fab->setStartState(startSymbol->name);
	fab->addFinalState(FIN_STATE);

	for (auto r : g->rules)
	{
		for (auto sequence : r.second)
		{
			if (sequence->hasTerminalsOnly())
			{
				if (!sequence->isEpsilon())
				{
					// Add transition to final state
					char t = sequence->at(0)->name.at(0);
					fab->addTransition(r.first->name, t, FIN_STATE);
				}
				else {
					// if there is an epsilon in the grammar it has to be in the sentence symbol
					if (r.first->compare(*startSymbol) == 0)
					{
						fab->addFinalState(startSymbol->name);
					}
					else {
						throw runtime_error("There is something wrong with the grammar because there was an epsilon in a rule that is not the sentence symbol");
					}
				}
			}
			else {
				// Add transition from current NT to next NT
				char t = sequence->at(0)->name.at(0);
				string Nt = sequence->at(1)->name;

				fab->addTransition(r.first->name, t, Nt);
			}
		}
	}

	auto nfa = fab->buildNFA();
	delete fab;

	return nfa;
}


Grammar* grammarOf(const NFA* nfa)
{
	const auto		symbolPool = SymbolPool::getInstance();
	const auto		rootSymbol = symbolPool->ntSymbol(nfa->s1);
	const auto		gb = new GrammarBuilder(rootSymbol);
	SequenceSet*	ss;

	for (auto states : nfa->S)
	{
		ss = new SequenceSet();
		Sequence* s;

		for (TapeSymbol Ts : nfa->V)
		{
			string terminal;
			terminal.push_back(Ts);

			for (auto dest : nfa->allDestsFor(states, Ts))
			{
				s = new Sequence();
				s->append(symbolPool->tSymbol(terminal));
				s->append(symbolPool->ntSymbol(dest));

				ss->insert(s);
			}
		}

		for(auto s : *ss)
			gb->addRule(symbolPool->ntSymbol(states), s);
	}

	auto grammar = gb->buildGrammar();
	delete gb;

	return grammar;
}


// end of Main.cpp
//======================================================================
