#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stack>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
enum  type_of_lex {
	LEX_NULL, /*0*/
	LEX_BEGIN,LEX_END,/* ...*/
	LEX_FIN, /*19*/
	LEX_SEMICOLON, LEX_COMMA,LEX_STR, LEX_COLON, LEX_LPAR, LEX_RPAR, LEX_DEF,
	LEX_LESS, LEX_GRT, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_LEQ, LEX_GEQ, 
	LEX_EQ, LEX_NEQ, LEX_EQ2, LEX_NEQ2, LEX_PLUSD, LEX_INC, LEX_MINUSD, LEX_DEC, 
	LEX_MULD, LEX_DIVD, LEX_MOD, LEX_MODD,LEX_AND,LEX_OR, LEX_DO, LEX_ELSE, 
	LEX_FUNC, LEX_IF, LEX_FALSE, LEX_READ, LEX_TRUE, LEX_VAR, LEX_WHILE,
	LEX_WRITE,LEX_FOR, LEX_IN, LEX_BREAK, LEX_CONT, LEX_RETURN,
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
	Lex ( type_of_lex t ,const string ){
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

string Scanner::TW[ ] = {"","do","else","function","if","false","read","true","var","while","write","for","in","break","continue","return"};
string Scanner::TD[ ] = {"",";",",",":","(",")","=","<", ">", "+", "-", "*", "/", "<=", ">=","==","!=","===","!==","+=","++","-=","--","*=","/=","%","%=","&&","||"};
type_of_lex Scanner::table[] = {LEX_NULL, LEX_DO, LEX_ELSE, LEX_FUNC, LEX_IF, LEX_FALSE, LEX_READ, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,LEX_FOR, LEX_IN, LEX_BREAK, LEX_CONT, LEX_RETURN};
type_of_lex Scanner::dlms[] = {LEX_NULL, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAR, LEX_RPAR, LEX_DEF, LEX_LESS, LEX_GRT, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_LEQ, LEX_GEQ, LEX_EQ, LEX_NEQ, LEX_EQ2, LEX_NEQ2, LEX_PLUSD, LEX_INC, LEX_MINUSD, LEX_DEC, LEX_MULD, LEX_DIVD, LEX_MOD, LEX_MODD,LEX_AND,LEX_OR};
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
				if(isdigit(c)){CS = NUMB; d = 0;}
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
				if(c=='{'){return Lex(LEX_BEGIN,0);}
				else
				if(c=='}'){return Lex(LEX_END,0);}
				else
				if(c=='#'){CS = COM1; gc();}
				else
				if(c==-1){CS = FIN;}
				else
				if((c=='.')||(c=='[')||(c==']')||(c==',')||(c=='(')||(c==')')||(c==':')||(c==';')){buf.push_back(c);j=look(buf, TD); return Lex(dlms[j], j);}
				else{CS = ERR; }
				break;
			case IDENT:
				if(isalpha(c) || isdigit(c) || (c=='_')){
					buf.push_back(c);
					gc();
				}
				else{
					fseek(fp,-1,SEEK_CUR);
					int k = look(buf, TW);
					if(k>0) return Lex(table[k],k);
					else return Lex(LEX_IDENT, buf);
				}
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
					fseek(fp,-1,SEEK_CUR);
					return Lex(LEX_NUM, d);
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
					fseek(fp,-1,SEEK_CUR);
					return Lex(LEX_NUM, d);
				}
			break;
			case E:
				s=0;
				if(isdigit(c)){
					CS = E_PL;
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
					fseek(fp,-1,SEEK_CUR);
					return Lex(LEX_NUM, d);
				}
			break;
			case E_MIN:
				if(isdigit(c)){
					s=10*s + (c - '0');
					gc();
				}
				else{
					for(int i = 0; i < s; i++) d/= 10;
					fseek(fp,-1,SEEK_CUR);
					return Lex(LEX_NUM, d);
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
					return Lex(LEX_STR, buf);
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
					return Lex(LEX_STR, buf);
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
					return Lex(dlms[j], j);
				}
				else{
					j = look(buf, TD);
					fseek(fp,-1,SEEK_CUR);
					return Lex(dlms[j], j);
				}
			break;
			case MINUS:
				if((c=='-') || (c=='=')){
					buf.push_back(c);
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}
				else{
					j = look(buf, TD);
					fseek(fp,-1,SEEK_CUR);
					return Lex(dlms[j], j);
				}
			break;
			case SLASH:
				if(c == '='){
					buf.push_back(c);
					j = look(buf, TD);
					return Lex(dlms[j], j);
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
					fseek(fp,-1,SEEK_CUR);
					return Lex(dlms[j], j);
				}
			break;
			case COM1:
				if(c == '\n'){
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
					return Lex(dlms[j], j);
				}
				else{
					j = look(buf, TD);
					fseek(fp,-1,SEEK_CUR);
					return Lex(dlms[j], j);
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
					fseek(fp,-1,SEEK_CUR);
					return Lex(dlms[j], j);
				}
			break;
			case EQU2:
			if(c=='='){
				buf.push_back(c);
				j = look(buf, TD);
				return Lex(dlms[j], j);
			}
			else{
				j = look(buf, TD);
				fseek(fp,-1,SEEK_CUR);
				return Lex(dlms[j], j);
			}
			break;
			case AMP:
				if(c == '&'){
					buf.push_back(c);
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}else{
					CS = ERR; 
				}
			break;
			case STICK:
				if(c == '|'){
					buf.push_back(c);
					j = look(buf, TD);
					return Lex(dlms[j], j);
				}else{
					CS = ERR;
				}
			break;
			case ERR:
				throw(c);
			break;
			case FIN:
				return Lex(LEX_FIN,0);
			break;
		}
	}
}



class Parser{
	Lex cur_lex;
	type_of_lex c_type;
	int c_val;
	Scanner scan;
	stack < int > st_int;
	stack < type_of_lex>  st_lex;
	void gl (){ 
		cur_lex = scan.get_lex();
		c_type = cur_lex.get_type();
		c_val = cur_lex.get_value();
	}
	bool is_expr();
	void S();
	void SENTENCE();
	void FUNC_DEF();
	void OPERATOR();
	void BLOCK();
	void VAR_DEF();
	void IF();
	void TRANSIT_OP();
	void LOOP_OP();
	void EXPR_OP();
public:
	vector <Lex> poliz;
	Parser (const char * program):scan(program){}
	void analyze();
};
void Parser::S(){
	gl();
	if(c_type != LEX_FIN){
		SENTENCE();
		S();
	}
}
void Parser::SENTENCE(){
	if(c_type == LEX_FUNC) FUNC_DEF();
	else OPERATOR();
}
void Parser::FUNC_DEF(){
	gl();
	if(c_type != LEX_IDENT) throw(cur_lex);
	gl();
	if(c_type != LEX_LPAR) throw(cur_lex);
	gl();
	while(c_type == LEX_IDENT){
		gl();
		if(c_type == LEX_RPAR) break;
		if(c_type !=LEX_COMMA) throw(cur_lex);
		gl();
		if(c_type != LEX_IDENT) throw(cur_lex);
	}
	if(c_type != LEX_RPAR) throw(cur_lex);
	gl();
	BLOCK();
}
void Parser::OPERATOR(){
	//OPERATOR ->LOOP_OP | EXPR_OP
	if(c_type == LEX_VAR) VAR_DEF();
	else if(c_type == LEX_SEMICOLON) gl();
	else if(c_type == LEX_BEGIN) BLOCK();
	else if(c_type == LEX_IF) IF(); 
	else if((c_type == LEX_BREAK)||(c_type == LEX_CONT)||(c_type == LEX_RETURN)) TRANSIT_OP();
	else if((c_type == LEX_DO)||(c_type == LEX_FOR)||(c_type == LEX_WHILE)) LOOP_OP();
	else if(is_expr()) EXPR_OP();
	else throw(cur_lex);
}
void Parser::VAR_DEF(){
	do{
		gl();
		if(c_type != LEX_IDENT) throw(cur_lex);
		gl();
		if(c_type == LEX_DEF){
			gl();
			if(is_expr()) EXPR_OP();
			else throw(cur_lex);
		}
	}
	while(c_type == LEX_COMMA);
	if(c_type != LEX_SEMICOLON) throw(cur_lex);
	gl();
}
void Parser::IF(){
	gl();
	if(c_type != LEX_LPAR) throw(cur_lex);
	gl();
	if(is_expr()) EXPR_OP();
	else throw(cur_lex);
	if(c_type != LEX_RPAR) throw(cur_lex);
	gl();
	OPERATOR();
	if(c_type == LEX_ELSE){
		gl();
		OPERATOR();
	}
}
void Parser::TRANSIT_OP(){
	if(c_type == LEX_RETURN){
		gl();
		if(is_expr()) EXPR_OP();
	}
	else{
		gl();
	}
	if(c_type != LEX_SEMICOLON) throw(cur_lex);
	gl();
}
void Parser::LOOP_OP(){
	switch(c_type){
	case LEX_WHILE:
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		if(is_expr())EXPR_OP();
		else throw(cur_lex);
		if(c_type != LEX_RPAR) throw(cur_lex);
		gl();
		OPERATOR();
	break;
	case LEX_DO:
		gl();
		OPERATOR();
		if(c_type != LEX_WHILE) throw(cur_lex);
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		if(is_expr())EXPR_OP();
		else throw(cur_lex);
		if(c_type != LEX_RPAR) throw(cur_lex);
		gl();
		if(c_type != LEX_SEMICOLON) throw(cur_lex);
		gl();
	break;
	case LEX_FOR:
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		if(c_type == LEX_VAR){
			gl();
			if(c_type != LEX_IDENT) throw(cur_lex);
			gl();
			if(c_type != LEX_IN) throw(cur_lex);
			gl();
			if(is_expr()) EXPR_OP();
			else throw(cur_lex);
			if(c_type != LEX_RPAR) throw(cur_lex);
			gl();
			OPERATOR();
		}
		else if(is_expr() || (c_type == LEX_SEMICOLON)){
			if(is_expr()){
				EXPR_OP();
				if(c_type != LEX_SEMICOLON) throw(cur_lex);
			}
			gl();
			if(is_expr()){
				EXPR_OP();
				if(c_type != LEX_SEMICOLON) throw(cur_lex);
			}
			else if(c_type!=LEX_SEMICOLON) throw(cur_lex);
			gl();
			if(is_expr()){
				EXPR_OP();
			}
			if(c_type != LEX_RPAR) throw(cur_lex);
			gl();
			OPERATOR();
		}
		else throw(cur_lex);
	break;
	default:
	break;
	}
}
bool Parser::is_expr(){
	if((c_type == LEX_NUM)||(c_type == LEX_IDENT)||(c_type == LEX_TRUE)||(c_type == LEX_FALSE)||(c_type == LEX_INC)||(c_type == LEX_DEC)||(c_type == LEX_STR)) return true;
	return false;
}
void Parser::EXPR_OP(){
	gl();
}
void Parser::BLOCK(){
	if(c_type != LEX_BEGIN) throw(cur_lex);
	gl();
	while((c_type != LEX_END) && (c_type != LEX_FIN)){
		OPERATOR();
	}
	if(c_type != LEX_END) throw(cur_lex);
	gl();
}
void Parser::analyze(){
	S();
	if(c_type != LEX_FIN)
		throw(cur_lex);
	cout<<"Mr George Harrison, Mr Ringo Starr..."<<endl;
}
int main(int argc, char** argv){
/*	Lex cur_lex(LEX_NULL,0);
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
/*	if(argc < 2){
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
	*/
	
	return 0;
}

