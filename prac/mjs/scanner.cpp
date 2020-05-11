#include "scanner.h"

vector <Ident> TID;

string Scanner::TW[ ] = {"","do","else","function","if","false","read","true","var","while","write","for","in","break","continue","return","Response", "Environment"};
string Scanner::TD[ ] = {"",";",",",":","(",")","=","<", ">", "+", "-", "*", "/", "<=", ">=","==","!=","===","!==","+=","++","-=","--","*=","/=","%","%=","&&","||","!",".","[","]"};
type_of_lex Scanner::table[] = {LEX_NULL, LEX_DO, LEX_ELSE, LEX_FUNC, LEX_IF, LEX_FALSE, LEX_READ, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,LEX_FOR, LEX_IN, LEX_BREAK, LEX_CONT, LEX_RETURN, OBJ_RESPONSE, OBJ_ENV};
type_of_lex Scanner::dlms[] = {LEX_NULL, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_LPAR, LEX_RPAR, LEX_DEF, LEX_LESS, LEX_GRT, LEX_PLUS, LEX_MINUS, LEX_MUL, LEX_DIV, LEX_LEQ, LEX_GEQ, LEX_EQ, LEX_NEQ, LEX_EQ2, LEX_NEQ2, LEX_PLUSD, LEX_INC, LEX_MINUSD, LEX_DEC, LEX_MULD, LEX_DIVD, LEX_MOD, LEX_MODD,LEX_AND,LEX_OR,LEX_NOT,LEX_DOT,LEX_LAR,LEX_RAR};
int put (const string & buf){
	vector <Ident>::iterator k;
	if((k = find(TID.begin(), TID.end(), buf)) != TID.end())
		return k-TID.begin();
	TID.push_back(Ident(buf));
	return TID.size()-1;
}
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
					else{
						j = put(buf);
						return Lex (LEX_IDENT, j);
					}
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
					buf.push_back(c);
					gc();
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
