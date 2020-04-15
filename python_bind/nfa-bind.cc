// TODO: add header

#include <limits>

#include <vata2/nfa.hh>

using namespace Vata2::Nfa;

// Type of ID of NFAs that are used for referencing from outside
using NfaId = size_t;

// bookkeeping functions
//
extern "C" void nfa_set_debug_level(unsigned lvl);
// returns the number of NFAs that are kept track of
extern "C" size_t nfa_library_size();
// clears all automata in the library
extern "C" void nfa_clear_library();

// constructors, destructors, etc.
extern "C" NfaId nfa_init();
extern "C" void nfa_free(NfaId id_nfa);
extern "C" void nfa_copy(NfaId dst, NfaId src);

// automata handling
extern "C" void nfa_add_initial(NfaId id_nfa, State state);
extern "C" void nfa_remove_initial(NfaId id_nfa, State state);
extern "C" bool nfa_is_initial(NfaId id_nfa, State state);
extern "C" void nfa_add_final(NfaId id_nfa, State state);
extern "C" void nfa_remove_final(NfaId id_nfa, State state);
extern "C" bool nfa_is_final(NfaId id_nfa, State state);
extern "C" void nfa_add_trans(NfaId id_nfa, State src, Symbol symb, State tgt);
extern "C" bool nfa_has_trans(NfaId id_nfa, State src, Symbol symb, State tgt);
extern "C" void nfa_print(NfaId id_nfa);

// language operations
extern "C" void nfa_union(NfaId id_dst, NfaId id_lhs, NfaId id_rhs);
extern "C" bool nfa_test_inclusion(NfaId id_lhs, NfaId id_rhs);

/** Library of NFAs */
std::unordered_map<NfaId, Nfa*> mem;
NfaId cnt = 0;

void nfa_set_debug_level(unsigned verbosity)
{
	Vata2::LOG_VERBOSITY = verbosity;
	DEBUG_PRINT("VATA verbosity: " + std::to_string(Vata2::LOG_VERBOSITY));
}

size_t nfa_library_size()
{
	return mem.size();
}

void nfa_clear_library()
{
	for (auto it = mem.begin(); it != mem.end(); ++it) {
		delete it->second;
		mem.erase(it);
	}
}


NfaId nfa_init()
{
	DEBUG_PRINT("Note that the management of created NFAs is a bit trivial now");
	assert(cnt < std::numeric_limits<int>::max());
	mem[cnt] = new Nfa();
	return cnt++;
}

void nfa_free(NfaId id_nfa)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	delete aut;
	mem.erase(id_nfa);
}

void nfa_copy(NfaId dst, NfaId src)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[src];
	Nfa* cp = mem[dst];

	// TODO: inefficient
	cp->initialstates = aut->initialstates;
	cp->finalstates = aut->finalstates;

	for (auto tr : *aut) {
		cp->add_trans(tr);
	}
}

void nfa_add_initial(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	aut->initialstates.insert(state);
}

void nfa_remove_initial(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	aut->initialstates.erase(state);
}

bool nfa_is_initial(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	return aut->has_initial(state);
}

void nfa_add_final(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	aut->finalstates.insert(state);
}

void nfa_remove_final(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	aut->finalstates.erase(state);
}

bool nfa_is_final(NfaId id_nfa, State state)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];
	return aut->has_final(state);
}

void nfa_add_trans(NfaId id_nfa, State src, Symbol symb, State tgt)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];

	aut->add_trans(src, symb, tgt);
}

bool nfa_has_trans(NfaId id_nfa, State src, Symbol symb, State tgt)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* aut = mem[id_nfa];

	return aut->has_trans(src, symb, tgt);
}

void nfa_print(NfaId id_nfa)
{
	DEBUG_PRINT("Some bound checking here...");

	Nfa* aut = mem[id_nfa];
	DEBUG_PRINT(std::to_string(*aut));
}

bool nfa_test_inclusion(NfaId id_lhs, NfaId id_rhs)
{
	assert(false);
	return true;
}

void nfa_union(NfaId id_dst, NfaId id_lhs, NfaId id_rhs)
{
	DEBUG_PRINT("Some bound checking here...");
	Nfa* lhs = mem[id_lhs];
	Nfa* rhs = mem[id_rhs];
	Nfa* dst = mem[id_dst];
	union_norename(dst, *lhs, *rhs);
}