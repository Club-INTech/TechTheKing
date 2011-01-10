#ifndef CONTRACTS_H
#define CONTRACTS_H

#ifdef CONTRACTS_ABORT
#include <stdlib.h>
#define REQUIRE(cond,texte) if(!(cond)) abort()
#define ENSURE(cond, texte) if(!(cond)) abort()
#define INVARIANT(cond, texte) if(!(cond)) abort()
#define BEGIN_INVARIANT_BLOCK(className) void _contract_check_invariants() {
											#define END_INVARIANT_BLOCK }
#define CHECK_INVARIANTS _contract_check_invariants
#endif