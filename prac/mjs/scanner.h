#ifndef __SCANER__
#define __SCANER__
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <unistd.h>
#include <fcntl.h>

using std::string;
using std::vector;
using std::ostream;

enum  type_of_lex {
	LEX_NULL, /*0*/
	LEX_BEGIN,LEX_END,/*1 2*/
	LEX_FIN, /*3*/
	LEX_SEMICOLON,//4
	LEX_COMMA,//5
	LEX_STR, //6
	LEX_COLON, //7
	LEX_LPAR, LEX_RPAR,//8 9 
	LEX_DEF,//10
	LEX_LESS, LEX_GRT, // 11 12 
	LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, //13 - 16
	LEX_LEQ, LEX_GEQ, //17 18
	LEX_EQ, LEX_NEQ, LEX_EQ2, LEX_NEQ2, //22
	LEX_PLUSD, LEX_INC, LEX_MINUSD, LEX_DEC, //26 
	LEX_MULD, LEX_DIVD, LEX_MOD, LEX_MODD, //30
	LEX_AND,LEX_OR, LEX_DO, LEX_ELSE, LEX_DOT, //35
	LEX_FUNC, LEX_IF, LEX_FALSE, LEX_READ, LEX_TRUE, LEX_VAR, LEX_WHILE,
	LEX_WRITE,LEX_FOR, LEX_IN, LEX_BREAK, LEX_CONT, LEX_RETURN, LEX_NOT,LEX_BOOL,OBJ_ENV,OBJ_RESPONSE,
	LEX_NUM, //53
	LEX_IDENT, //54
	POLIZ_LABEL, //55
	POLIZ_ADDRESS, //56
	POLIZ_GO, //57
	POLIZ_FGO,//58
	LEX_LAR,LEX_RAR,LEX_ARR,
	LEX_NAN
};


class Lex{
	type_of_lex t_lex;
	int v_lex;
	string n_lex;
public:
	Lex ( type_of_lex t = LEX_NULL,  int v = 0){
		t_lex = t; 
		v_lex = v;
	}
	Lex ( type_of_lex t ,const string s){
		t_lex = t; 
		v_lex = 0;
		n_lex = s;
	}
	string get_name() const{
		return n_lex;
	}
	type_of_lex get_type () const{ return t_lex; }
	int get_value () const{ return  v_lex; }
	friend ostream& operator<< (ostream & s, Lex l ){
		s << '(' << l.t_lex << ',' << l.v_lex << ");" ;
		return  s;
	}
};

class Ident{
	string name;
	bool declare;
	type_of_lex type;
	bool assign;
	int value;
public:
	vector <int> elem;
	Ident(){ 
		declare = false; 
		assign = false;
	}
	Ident(const string & n){
		name = n;  
		declare = false; 
		assign = false;
	}
	bool operator==(const string & s) const {return name == s; }
	string get_name() { return name; }
	void put_name(const string & n){ name = n;}
	bool get_declare() { return declare; }
	void put_declare() { declare = true; }
	int put (const string & );
	type_of_lex  get_type() { return type; }
	void put_type(type_of_lex t) { type = t; }
	bool get_assign() { return assign; }
	void put_assign() { assign = true; }
	int get_value() { return value; }
	void put_value(int v) { value = v; }
};
class Scanner{
	FILE *fp; 
	int c;
	int look(const string & buf,  string *  list){
		int i= 1;
		while(list[i] != ""){
			if(buf==list[i])
				return i;
			i++;
		}
		return 0;  
	}
	void gc(){ c = fgetc(fp);}

public:
	static string TW [], TD[];
	static type_of_lex dlms[];
	static type_of_lex table[];
	Scanner (const char* program){
		fp = fopen( program, "r" ); 
	}
	Lex get_lex();
};
#endif
