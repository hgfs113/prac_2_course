#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <stack>
#include <unistd.h>
#include <fcntl.h>
#include "interpreter.h"

using namespace std;

int main(){
/*        Lex cur_lex(LEX_NULL,0);
	Scanner scan("test.cpp");
	while(cur_lex.get_type() != LEX_FIN){
		cur_lex = scan.get_lex();
		cout<<cur_lex.get_type()<<","<<cur_lex.get_value()<<endl;
	}*/
	try{
		Interpretator I("cgi-bin/test.cpp");
		I.interpretation();
	}
	catch(char c){
		cout << "unexpected symbol " << c << endl;
		return 1;
	}
	catch(int c){
		cout << "unexpected symbol " << c << endl;
		return 1;
	}
	catch(Lex l){
		cout << "unexpected lexeme" <<l.get_type()<<endl;
		return 1;
	}
	catch(const char * s){
			cout<<s<<endl;
			return 1;
	}
	return 0;
}

