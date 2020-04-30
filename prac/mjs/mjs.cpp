#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

using namespace std;

int main(int argc, char** argv){
/*        Lex cur_lex(LEX_NULL,0);
	Scanner scan("test.cpp");
	while(cur_lex.get_type() != LEX_FIN){
		cur_lex = scan.get_lex();
		cout<<cur_lex.get_type()<<","<<cur_lex.get_value()<<endl;
	}*/
	Parser par("test.cpp");
	try{
		par.analyze();
	}
	catch(int c){
		cout<<"lex err in "<<c<<endl;
		return 0;
	}
	catch(Lex l){
		cout<<"parsing err in lex "<<l.get_type()<<endl;
		return 0;
	}
	catch(const char * s){
		cout<<s<<endl;
		return 0;
	}
	return 0;
}

