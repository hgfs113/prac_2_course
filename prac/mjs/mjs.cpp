#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
enum  type_of_lex {
	LEX_NULL, /*0*/
	LEX_AND, LEX_BEGIN,/* ...*/ LEX_WRITE, /*18*/ 
	LEX_FIN, /*19*/
	LEX_SEMICOLON, LEX_COMMA,LEX_STR, /*...*/ LEX_GEQ, /*35*/
	LEX_NUM, /*36*/
	LEX_IDENT, /*37*/
	POLIZ_LABEL, /*38*/
	POLIZ_ADDRESS, /*39*/
	POLIZ_GO, /*40*/
	POLIZ_FGO/*41*/
};


class Lex{
	type_of_lex t_lex;
	int v_lex;
public:
	Lex ( type_of_lex t = LEX_NULL,  int v = 0){
		t_lex = t; 
		v_lex = v;
	}
	Lex ( type_of_lex t = LEX_NULL,const string = ""){
		t_lex = t; 
		v_lex = 0;
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
	bool get_declare() { return declare; }
	void put_declare() { declare = true; }
	type_of_lex  get_type() { return type; }
	void put_type(type_of_lex t) { type = t; }
	bool get_assign() { return assign; }
	void put_assign() { assign = true; }
	int get_value() { return value; }
	void put_value(int v) { value = v; }
};

vector <Ident> TID;
int put (const string & buf){
	vector <Ident>::iterator k;

	if((k = find(TID.begin(), TID.end(), buf)) != TID.end())
		return k-TID.begin();

	TID.push_back(Ident(buf));
	return TID.size()-1;
}

class Scanner{
	FILE *fp; 
	int c;
	int look(const string & buf,  string *  list){
		int i= 0;
		while(list[i]!=""){
			if(buf==list[i])
				return i;
			i++;
		}
		return 0;  
	}
	void gc(){ c = fgetc(fp);}
public:
	static string TW [], TD[];
	Scanner (const char* program){
		fp = fopen( program, "r" ); 
	}
	Lex get_lex();
};

string Scanner::TW[ ] = { "","and","begin","bool","do","else","end","if","false","int","not","or","program","read","then","true","var","while","write" };
string Scanner::TD[ ] = {"",";",",",":",":=","(",")","=","<", ">", "+", "-", "*", "/", "<=", ">="};

type_of_lex dlms[25];

Lex Scanner::get_lex(){
	enum state{ H, IDENT, NUMB, FLOAT, E, E_MIN,E_PL,ERR,STR1, STR2, AMP, STICK, PLUS, MINUS, SLASH, COM1, COM2, COM3, ALE, EQU1, EQU2, ECR1, ECR, FIN};
	state CS = H;
	string buf;
	int j, n, s;
	double d;
	while(1){
		switch(CS){
			case H: 
				gc();
				if(c==' ' || c=='\n' || c=='\r' || c=='\t' ); 
				else
				if(isalpha(c)){CS = IDENT;}
				else
				if(isdigit(c)){CS = NUMB;d = 0;}
				else
				if(c=='\''){CS =  STR1; gc();} 
				else
				if(c=='\"'){CS =  STR2; gc();} 
				else
				if(c=='+'){CS =  PLUS; buf.push_back(c);gc();}
				else
				if(c=='-'){CS =  MINUS; buf.push_back(c);gc();}
				else
				if(c=='&'){CS =  AMP; buf.push_back(c);gc();}
				else
				if(c=='|'){CS =  STICK; buf.push_back(c);gc();}
				else
				if(c=='/'){CS =  SLASH; buf.push_back(c);gc();}
				else
				if((c=='<')||(c=='>')||(c=='+')||(c=='%')||(c=='*')){CS =  ALE; buf.push_back(c);gc();}
				else
				if((c=='=')||(c=='!')){CS =  EQU1; buf.push_back(c);gc();}
				else
				if(c=='{'){Lex(LEX_BEGIN,0); gc();}
				else
				if(c=='}'){Lex(LEX_FIN,0); gc();}
				else
				if(c==-1){CS = FIN;}
				else
				if((c=='.')||(c=='[')||(c==']')||(c==',')||(c=='(')||(c==')')||(c==':')||(c==';')){buf.push_back(c);gc();j=look(buf, TD); Lex(dlms[j], j);}
				else{CS = ERR; }
				break;
			case IDENT:
				if(isalpha(c) || isdigit(c) || (c=='_')){
					buf.push_back(c);
					gc();
				}
				else Lex(LEX_IDENT, buf);
				CS = H;
			break;
			case NUMB:
				if(isdigit(c)){
					d = d*10 + (c-'0');
					gc();
				}
				else if((c=='e')||(c=='E')){
					gc();
					CS = E;
				}
				else if(c=='.'){
					gc();
					n = 10;
					CS = FLOAT;
				}
				else{
					Lex(LEX_NUM, d);
					CS = H;
				}
			break;
			case FLOAT:
				if(isdigit(c)){
					d = d*10 + (c-'0')/n;
					n*=10;
					gc();
				}
				else if((c=='e')||(c=='E')){
					gc();
					CS = E;
				}
				else{
					Lex(LEX_NUM, d);
					CS = H;
				}
			break;
			case E:
				s=0;
				if(isdigit(c)){
					CS = E_PL;
					s=c - '0';
				}
				else if(c == '-'){
					gc();
					if(isdigit(c))
						CS = E_MIN;
					else{
						CS = ERR;
					}
				}
				else{
					CS = ERR; 
				}
			break;
			case E_PL:
				if(isdigit(c)){
					s=10*s + (c - '0');
					gc();
				}
				else{
					for(int i = 0; i < s; i++) d*= 10;
					Lex(LEX_NUM, d);
					CS = H;
				}
			break;
			case E_MIN:
				if(isdigit(c)){
					s=10*s + (c - '0');
					gc();
				}
				else{
					for(int i = 0; i < s; i++) d/= 10;
					Lex(LEX_NUM, d);
					CS = H;
				}
			break;
			case STR1:
				if(c == '\\'){
					gc();
					CS = ECR;
				}
				else if(c == -1){
					CS = ERR;			
				}
				else if(c == '\''){
					gc();
					Lex(LEX_STR, buf);
					CS = H;
				}
				else{
					gc();
					buf.push_back(c);
				}
				
			break;
			case STR2:
				if(c == '\\'){
					gc();
					CS = ECR1;
				}
				else if(c == -1){
					CS = ERR;			
				}
				else if(c == '\"'){
					gc();
					Lex(LEX_STR, buf);
					CS = H;
				}
				else{
					gc();
					buf.push_back(c);
				}
				
			break;
			case ECR:
				if(c == '\\'){
					gc();
					buf.push_back('\\');
					CS = STR1;
				}
				else if(c == -1){
					CS = ERR;			
				}
				else if(c == '\''){
					gc();
					buf.push_back('\'');
					CS = STR1;
				}
				else{
					CS = ERR;	
				}
			break;
			case ECR1:
				if(c == '\\'){
					gc();
					buf.push_back('\\');
					CS = STR2;
				}
				else if(c == -1){
					CS = ERR;			
				}
				else if(c == '\"'){
					gc();
					buf.push_back('\"');
					CS = STR2;
				}
				else{
					CS = ERR;
				}
			break;
			case PLUS:
				if((c=='+') || (c=='=')){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}
				else{
					j = look(buf, TD);
					Lex(dlms[j], j);
					CS = H;
				}
			break;
			case MINUS:
				if((c=='-') || (c=='=')){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}
				else{
					j = look(buf, TD);
					Lex(dlms[j], j);
					CS = H;
				}
			break;
			case SLASH:
				if(c == '='){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}
				else if(c == '/'){
					buf.pop_back();
					gc();
					CS = COM1;
				}
				else if(c == '*'){
					buf.pop_back();
					gc();
					CS = COM2;
				}
				else{
					j = look(buf, TD);
					Lex(dlms[j], j);
					CS = H;
				}
			break;
			case COM1:
				if(c == '\n'){
					gc();
					CS = H;
				}
				else{
					gc();
				}
			break;
			case COM2:
				if(c == '*'){
					gc();
					CS = COM3;
				}
				else if(c == -1){
					CS = ERR; 
				}
				else{
					gc();
				}
			break;
			case COM3:
				if(c == '/'){
					gc();
					CS = H;
				}
				else{
					CS = COM2;
				}
			break;
			case ALE:
				if(c=='='){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}
				else{
					j = look(buf, TD);
					Lex(dlms[j], j);
					CS = H;
				}
			break;
			case EQU1:
				if(c=='='){
					buf.push_back(c);
					gc();
					CS = EQU2;
				}
				else{
					j = look(buf, TD);
					Lex(dlms[j], j);
					CS = H;
				}
			break;
			case EQU2:
			if(c=='='){
				buf.push_back(c);
				j = look(buf, TD);
				Lex(dlms[j], j);
				gc();
				CS = H;
			}
			else{
				j = look(buf, TD);
				Lex(dlms[j], j);
				CS = H;
			}
			break;
			case AMP:
				if(c == '&'){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}else{
					CS = ERR; 
				}
			break;
			case STICK:
				if(c == '|'){
					buf.push_back(c);
					j = look(buf, TD);
					Lex(dlms[j], j);
					gc();
					CS = H;
				}else{
					CS = ERR;
				}
			break;
			case ERR:
				throw(c);
			break;
			case FIN:
				std::cout<<"Succses!"<<'\n';
				exit(0);
			break;
		}
	}
}

int main(int argc, char** argv){
	if(argc < 2){
		std::cerr<<"i need file\n";
		exit(0);
	}
	Scanner COMP(argv[1]);
	try{
		COMP.get_lex();
	}
	catch(int c){
		printf("err = %c\n",c);
		exit(0);
	}
	return 0;
}

