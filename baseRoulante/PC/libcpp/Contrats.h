
#ifndef CONTRACTS_H

#define CONTRACTS_H

#define CONTRACTS_NO_CHECK

#ifdef CONTRACTS_USE_ABORT
#include <stdlib.h>
#define REQUIRE(cond,texte) if(!(cond)) abort()
#define ENSURE(cond, texte) if(!(cond)) abort()
#define INVARIANT(cond, texte) if(!(cond)) abort()
#define BEGIN_INVARIANT_BLOCK(className) void _contract_check_invariants() {
#define END_INVARIANT_BLOCK }
#define CHECK_INVARIANTS _contract_check_invariants
#endif

#ifdef CONTRACTS_USE_ASSERT
#include <assert.h>
#define REQUIRE(cond, texte) assert(cond)
#define ENSURE(cond, texte) assert(cond)
#define INVARIANT(cond, texte) assert(cond)
#define BEGIN_INVARIANT_BLOCK(className) void _contract_check_invariants() {
#define END_INVARIANT_BLOCK }
#define CHECK_INVARIANTS _contract_check_invariants
#endif

#ifdef CONTRACTS_USE_EXCEPTION

#include <string>
#include <sstream>
#include <stdlib.h>
class contract_violation_exception : public std::exception {
        std::string message;
		std::string m_condition;
public:
        contract_violation_exception(std::string condition, char const * file, int line)
        {
				m_condition=condition;
                std::ostringstream str;
                str << file << " : ligne " << line << " - Violation du contrat : " << condition;
                message = str.str();
        };
        char const * what() const throw() { return message.c_str(); };
		char const * why() const throw() { return m_condition.c_str(); };
        ~contract_violation_exception() throw() {};
};



#define REQUIRE(cond, texte) if(!(cond)) throw contract_violation_exception(texte, __FILE__, __LINE__)
#define ENSURE(cond, texte) if(!(cond)) throw contract_violation_exception(texte, __FILE__, __LINE__)
#define INVARIANT(cond, texte) if(!(cond)) throw contract_violation_exception(texte, __FILE__, __LINE__)
#define BEGIN_INVARIANT_BLOCK(className) void _contract_check_invariants() {
#define END_INVARIANT_BLOCK } \
	void contracts_handle_invariants(char const * file, int line, char const * fonction){\
		try{\
			_contract_check_invariants();\
		}\
		catch(contract_violation_exception contratVioletOlol){\
			cout << file << " : ligne " << line << " dans " << fonction << endl;\
			cout << "Echec de vérification de l'invariant : " << contratVioletOlol.why() << endl << endl;\
			abort();\
		}\
	}
#define CHECK_INVARIANTS contracts_handle_invariants(__FILE__,__LINE__,__FUNCTION__)
#endif

#ifdef CONTRACTS_NO_CHECK
#define REQUIRE(cond, texte)
#define ENSURE(cond, texte)
#define INVARIANT(cond, texte)
#define BEGIN_INVARIANT_BLOCK(className)
#define END_INVARIANT_BLOCK
#define CHECK_INVARIANTS
#endif

#ifdef CONTRACTS_NO_PRECONDITION
#undef REQUIRE
#define REQUIRE(cond, texte)
#endif

#ifdef CONTRACTS_NO_POSTCONDITION
#undef ENSURE
#define ENSURE(cond, texte)
#endif

#ifdef CONTRACTS_NO_INVARIANT
#undef INVARIANT
#undef BEGIN_INVARIANT_BLOCK
#undef END_INVARIANT_BLOCK
#undef CHECK_INVARIANTS
#define CHECK_INVARIANTS()
#define INVARIANT(cond, text)
#define BEGIN_INVARIANT_BLOCK(className)
#define END_INVARIANT_BLOCK
#endif

#endif