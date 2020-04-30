#ifndef __PARSER__
#define __PARSER__
#include "scanner.h"
#include "stack.h"

class Poliz{
	Lex *p;
	int _size;
	int top;
public:
	Poliz(int size = 100){
		p = new Lex[_size = size];
		top = 0;
	}
	~Poliz(){ delete []p; }
	void put_lex(Lex l){
		p[top++] = l;
	}
	void put_lex(Lex l, int place){
		p[place] = l;
	}
	void pop(){
		if(top > 0) top--;
	}
	int get_top(){	return top; }
	int get_size(){ return _size; }
	Lex& operator[](int index){
		if(index > _size) throw "POLIZ: index";
		else if(index > top) throw "POLIZ: undefinet";
		else return p[index];
	}
	void print(){
		for(int i = 0; i < top; i++){
			std::cout << p[i] << std::endl;
		}
	}
};

class Parser{
	Lex cur_lex;
	type_of_lex c_type;
	int c_val;
	Scanner scan;
	Stack < int, 128> st_int;
	Stack < type_of_lex, 128>  st_lex;
	void gl (){ 
		cur_lex = scan.get_lex();
		c_type = cur_lex.get_type();
		c_val = cur_lex.get_value();
	}
	bool is_def(){
		return (c_type == LEX_DEF)||(c_type == LEX_PLUSD)||(c_type == LEX_MULD)||(c_type == LEX_DIVD);
	}
	bool is_expr();
	bool is_un();
	bool is_bin(type_of_lex);
	bool is_olgapar1012();
	void expresion();
	void check_op();
	void check_ident();
	void check_not();
	void dec(type_of_lex);
	void dec2(type_of_lex,int);
	void S();
	void E();
	void E1();
	void T();
	void F();
	void D();
	void SENTENCE();
	void FUNC_DEF();
	void OPERATOR();
	void BLOCK();
	void VAR_DEF();
	void IF();
	void ENV();
	void RESPONSE();
	void TRANSIT_OP();
	void LOOP_OP();
	void EXPR_OP();
	void IDNT(int);
	void READ();
	void WRITE();
public:
	Poliz prog;
	Parser (const char * program):scan(program){}
	void analyze();
};
#endif
