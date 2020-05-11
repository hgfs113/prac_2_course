#ifndef __INTER__
#define __INTER__
#include "parser.h"
#include "cstring"
struct arg{
	type_of_lex type;
	int value;
	string name;
	arg(){
		type = LEX_NULL;
		value = 0;
		name.clear();
	}
	arg(type_of_lex t, int v){
		type = t;
		value = v;
		name = std::to_string(v);
	}
	arg(type_of_lex t, string n){
		type = t;
		value = 0;
		name = n;
	}
	arg(type_of_lex t, int v, string n){
		type = t;
		value = v;
		name = n;
	}
};
class Executer{
	Lex pc_el;
public:
	void execute(Poliz& prog);
};
class Interpretator{
	Parser pars;
	Executer E;
public:
	Interpretator (const char* program ):pars(program) {
	}
	void interpretation ();
};
#endif
