#include "parser.h"
extern vector <Ident> TID;
bool lvalue = true;

void Parser::S(){
	gl();
	while(c_type != LEX_FIN){
		SENTENCE();
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
	else if(c_type == LEX_READ) READ();
	else if(c_type == LEX_WRITE) WRITE(); 
	else if((c_type == LEX_BREAK)||(c_type == LEX_CONT)||(c_type == LEX_RETURN)) TRANSIT_OP();
	else if((c_type == LEX_DO)||(c_type == LEX_FOR)||(c_type == LEX_WHILE)) LOOP_OP();
	else if(c_type == LEX_IDENT) IDNT(0);
	else if((c_type == LEX_NUM) || (c_type == LEX_PLUS) || (c_type == LEX_MINUS)) expresion();
	else if(c_type == OBJ_ENV) ENV();
	else if(c_type == OBJ_RESPONSE) RESPONSE();
	else if(c_type == LEX_INC){
		gl();
		if(c_type != LEX_IDENT) throw(cur_lex);
		IDNT(1);
	}else if(c_type == LEX_DEC){
		gl();
		if(c_type != LEX_IDENT) throw(cur_lex);
		IDNT(-1);
	}
	else throw(cur_lex);
}
void Parser::VAR_DEF(){
	st_int.reset();
	do{
		gl();
		if(c_type != LEX_IDENT) throw(cur_lex);
		st_int.push(c_val);
		int prev_val = c_val;
		gl();
		if(c_type == LEX_DEF){
			prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
			gl();
			if(is_expr()){
				expresion();
				prog.put_lex(Lex(LEX_DEF));
			}else if(c_type == LEX_LAR){
				prog.pop();
				int offs = 0;
				gl();
				while(c_type != LEX_RAR){
					prog.put_lex(Lex(POLIZ_ADDRESS,prev_val*2048 + offs));
					expresion();
					prog.put_lex(Lex(LEX_DEF));
					++offs;
					if(c_type == LEX_COMMA) gl();
					else if(c_type != LEX_RAR) throw(cur_lex);
				}
				gl();
			}else if(c_type == OBJ_ENV){
				ENV();
			}
			else throw(cur_lex);
		}dec(LEX_IDENT);
	}
	while(c_type == LEX_COMMA);
	if(c_type != LEX_SEMICOLON) throw(cur_lex);
	gl();
}
void Parser::IF(){
	gl();
	if(c_type != LEX_LPAR) throw(cur_lex);
	gl();
	if(is_expr()) expresion();
	else throw(cur_lex);
	int pl2 = prog.get_free();
	prog.blank();
	prog.put_lex(Lex(POLIZ_FGO));
	if(c_type != LEX_RPAR) throw(cur_lex);
	gl();
	OPERATOR();
	int pl3 = prog.get_free(); 
	prog.blank();
	prog.put_lex(Lex(POLIZ_GO));
	prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
	if(c_type == LEX_ELSE){
		gl();
		OPERATOR();
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
	}
}
void Parser::TRANSIT_OP(){
	if(c_type == LEX_RETURN){
		gl();
		if(is_expr()) expresion();
	}
	else{
		gl();
	}
	if(c_type != LEX_SEMICOLON) throw(cur_lex);
	gl();
}
void Parser::LOOP_OP(){
	int pl0,pl1,pl2;
	switch(c_type){
	case LEX_WHILE:
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		pl0 = prog.get_free();
		if(is_expr())expresion();
		else throw(cur_lex);
		if(c_type != LEX_RPAR) throw(cur_lex);
		gl();
		pl1 = prog.get_free();
		prog.blank();
		prog.put_lex(Lex(POLIZ_FGO)); 
		OPERATOR();
		prog.put_lex(Lex(POLIZ_LABEL, pl0));
		prog.put_lex(Lex(POLIZ_GO));
		prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
	break;
	case LEX_DO:
		pl0 = prog.get_free();
		gl();
		OPERATOR();
		if(c_type != LEX_WHILE) throw(cur_lex);
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		if(is_expr()){
			expresion();
		}
		else throw(cur_lex);
		pl1 = prog.get_free();
		prog.put_lex(Lex(POLIZ_LABEL, pl1+4));
		prog.put_lex(Lex(POLIZ_FGO));
		prog.put_lex(Lex(POLIZ_LABEL, pl0));
		prog.put_lex(Lex(POLIZ_GO));
		if(c_type != LEX_RPAR) throw(cur_lex);
		gl();
		if(c_type != LEX_SEMICOLON) throw(cur_lex);
		gl();
	break;
	case LEX_FOR://poliz...
		gl();
		if(c_type != LEX_LPAR) throw(cur_lex);
		gl();
		if(c_type == LEX_VAR){
			gl();
			if(c_type != LEX_IDENT) throw(cur_lex);
			gl();
			if(c_type != LEX_IN) throw(cur_lex);
			gl();
			if(is_expr()) expresion();
			else throw(cur_lex);
			if(c_type != LEX_RPAR) throw(cur_lex);
			gl();
			OPERATOR();
		}
		else if(is_expr() || (c_type == LEX_SEMICOLON)){
			if(is_expr()){
				expresion();
				if(c_type != LEX_SEMICOLON) throw(cur_lex);
			}
			gl();
			pl0 = prog.get_free();
			if(is_expr()){
				expresion();
				if(c_type != LEX_SEMICOLON) throw(cur_lex);
			}
			else if(c_type!=LEX_SEMICOLON) throw(cur_lex);
			pl1 = prog.get_free();
			gl();
			prog.blank();
			prog.put_lex(Lex(POLIZ_FGO));
			prog.blank();
			prog.put_lex(Lex(POLIZ_GO));
			if(is_expr()){
				expresion();
			}
			prog.put_lex(Lex(POLIZ_LABEL, pl0));
			prog.put_lex(Lex(POLIZ_GO));
			pl2 = prog.get_free();
			if(c_type != LEX_RPAR) throw(cur_lex);
			gl();
			OPERATOR();
			prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()+2),pl1);
			prog.put_lex(Lex(POLIZ_LABEL, pl2),pl1+2);
			prog.put_lex(Lex(POLIZ_LABEL, pl1 + 4));
			prog.put_lex(Lex(POLIZ_GO));
		}
		else throw(cur_lex);
	break;
	default:
	break;
	}
}
bool Parser::is_expr(){
	if((c_type == LEX_NUM)||(c_type == LEX_NOT)||(c_type == LEX_IDENT)||(c_type == LEX_TRUE)||(c_type == LEX_FALSE)||(c_type == LEX_INC)||(c_type == LEX_DEC)||(c_type == LEX_STR)) return true;
	return false;
}
bool Parser::is_un(){
	if((c_type == LEX_INC )||(c_type == LEX_DEC)) return true;
	return false;
}
bool Parser::is_bin(type_of_lex param){
	if((param == LEX_AND)||(param == LEX_DEF)||(param == LEX_DIV)||(param == LEX_DIVD)||
		(param == LEX_EQ)||(param == LEX_EQ2)||(param == LEX_GEQ)||(param == LEX_GRT)|| 
		(param == LEX_LEQ)||(param == LEX_LESS)||(param == LEX_MINUS)||(param == LEX_MINUSD)|| 
		(param == LEX_MOD)||(param == LEX_MODD)||(param == LEX_MUL)||(param == LEX_MULD)|| 
		(param == LEX_NEQ)||(param == LEX_NEQ2)||(param == LEX_PLUSD)||(param == LEX_PLUS)||
		(param == LEX_OR)||(param == LEX_AND)) return true;
	return false;
}
bool Parser::is_olgapar1012(){
	if((c_type == LEX_IDENT)||(c_type == LEX_NUM)||(c_type == LEX_TRUE)||(c_type == LEX_FALSE)||(c_type == LEX_STR))return true;
	return false;
}
void Parser::dec(type_of_lex type){
	int i;
	while(!st_int.is_empty()){
		i = st_int.pop();
		if(TID[i].get_declare()){
			std::cout<<st_int.pop();
			throw "Twice declare";
		}else{
			TID[i].put_declare();
			TID[i].put_type(type);
		}
	}
}

void Parser::dec2(type_of_lex type, int val){
	if(TID[val].get_declare()){
		std::cout<<c_type<<" here "<<c_val<<std::endl;
		throw "Twice declare";
	}
	else{
		TID[val].put_declare();
		TID[val].put_type(type);
	}
}
void Parser::EXPR_OP(){
	expresion();
	if(c_type != LEX_SEMICOLON) throw(cur_lex);
	gl();
}


void Parser::expresion(){
	lvalue = true;
	E1();
	if(c_type == LEX_EQ || c_type == LEX_LESS || c_type == LEX_GRT ||
			c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ||
			c_type == LEX_EQ2 || c_type == LEX_NEQ2){
		//st_lex.push(c_type);
		type_of_lex prev_type=c_type;
		gl();
		E1();
		check_op();
		prog.put_lex(Lex(prev_type)); 
	}
}

void Parser::E1(){
	T();
	while(c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR){
		//st_lex.push(c_type);
		type_of_lex prev_type=c_type;
		gl();
		T();
		check_op();
		prog.put_lex(Lex(prev_type)); 
	}
}
void Parser::T(){
	F();
	while(c_type == LEX_MUL || c_type == LEX_DIV || c_type == LEX_AND){
		//st_lex.push(c_type);
		type_of_lex prev_type=c_type;
		gl();
		F();
		check_op();
		prog.put_lex(Lex(prev_type)); 
	}
}
void Parser::F(){
//	std::cout<<"sun sun sun here he comes "<<c_type<<std::endl;
	if(c_type == LEX_IDENT){
		check_ident();
		if(TID[c_val].get_type() != LEX_ARR){
			prog.put_lex(Lex(LEX_IDENT, c_val));
			int p_val = c_val;
			gl();
			if(lvalue && is_def()){
				prog.pop();
				prog.put_lex(Lex(POLIZ_ADDRESS, p_val));
				D();
			}else if(lvalue && (c_type == LEX_DEC || c_type == LEX_INC)){
				prog.pop();
				prog.put_lex(Lex(POLIZ_ADDRESS, p_val));
				prog.put_lex(Lex(LEX_NUM, 1));
				if(c_type == LEX_INC) prog.put_lex(Lex(LEX_PLUSD));
				else prog.put_lex(Lex(LEX_MINUSD));
				gl();
			}
		}else{
			int p_val = c_val;
			gl();
			if(c_type == LEX_LAR){
				gl();
				if(c_type != LEX_NUM) throw(cur_lex);
				prog.put_lex(Lex(LEX_IDENT, c_val));
				p_val = 2048*p_val + c_val;
				gl();
				if(c_type != LEX_RAR) throw(cur_lex);
				gl();
				if(lvalue && is_def()){
					prog.pop();
					prog.put_lex(Lex(POLIZ_ADDRESS, p_val));
					D();
				}else if(lvalue && ((c_type == LEX_INC) || (c_type == LEX_DEC))){
					prog.pop();
					prog.put_lex(Lex(POLIZ_ADDRESS, p_val));
					prog.put_lex(Lex(LEX_NUM, 1));
					if(c_type == LEX_INC) prog.put_lex(Lex(LEX_PLUSD));
					else prog.put_lex(Lex(LEX_MINUSD));
					gl();
				}
			}
		}
	}else if(c_type == LEX_NUM){
		st_lex.push(LEX_NUM);
		prog.put_lex(cur_lex);
		gl();
	}else if(c_type == LEX_STR){
		st_lex.push(LEX_STR);
		prog.put_lex(cur_lex);
		gl();
	}else if(c_type == LEX_TRUE){
		st_lex.push(LEX_BOOL);
		prog.put_lex(Lex(LEX_TRUE, 1));//LEX_BOOL
		gl();
	}else if(c_type == LEX_FALSE){
		st_lex.push(LEX_BOOL);
		prog.put_lex(Lex(LEX_FALSE, 0));
		gl();
	}else if(c_type == LEX_NOT){
		gl();
		F();
		check_not();
	}else if(c_type == LEX_LPAR){
		gl();
		expresion();
		if(c_type == LEX_RPAR){
			gl();
		}
		else throw(cur_lex);
	}else if(c_type == OBJ_ENV){
		lvalue = false;
		ENV();
	}else{return;}
	lvalue=false; 
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
	prog.print();
	if(c_type != LEX_FIN)
		throw(cur_lex);
	std::cout<<"succses!..."<<std::endl;
}
void Parser::check_op(){}
void Parser::check_ident(){
	if(TID[c_val].get_declare()){
		st_lex.push(TID[c_val].get_type());
	}else throw "Not declared";
}
void Parser::check_not(){
	if(st_lex.pop() != LEX_BOOL) throw "Wrong type not";
	else st_lex.push(LEX_BOOL);
}
void Parser::D(){
    if(is_def()){
        type_of_lex p_type = c_type;
        gl();
        expresion();
        prog.put_lex(Lex(p_type));
    }else throw(cur_lex);
}

void Parser::READ(){
	gl();
	if(c_type != LEX_LPAR) throw(cur_lex);
	gl();
	if(c_type == LEX_IDENT){
		prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
		prog.put_lex(Lex(LEX_READ));
	}
	else throw(cur_lex);
	gl();
	if(c_type != LEX_RPAR) throw(cur_lex);
	gl();
}

void Parser::WRITE(){
	prog.put_lex(Lex(LEX_WRITE));
	gl();
	if(c_type != LEX_LPAR) throw(cur_lex);
	gl();
	int p_val = c_val;
	if(c_type == LEX_IDENT || c_type == LEX_NUM || c_type == LEX_STR || c_type == LEX_BOOL){
		prog.put_lex(cur_lex);
	}else throw(cur_lex);
	gl();
	if(c_type == LEX_LAR){
		prog.pop();
		prog.put_lex(Lex(LEX_ARR, p_val));
		gl();
		if(c_type != LEX_NUM) throw(cur_lex);
		prog.put_lex(cur_lex);
		gl();
		if(c_type != LEX_RAR) throw(cur_lex);
		gl();
	}
	if(c_type != LEX_RPAR) throw(cur_lex);
	gl();
}
void Parser::ENV(){
	gl();
	if(c_type != LEX_DOT) throw(cur_lex);
	gl();
	//
	gl();
}
void Parser::RESPONSE(){
	gl();
	if(c_type != LEX_DOT) throw(cur_lex);
	gl();
	if(c_type != LEX_WRITE) throw(cur_lex);
	WRITE();
}
void Parser::IDNT(int flag){
	check_ident();
	if(TID[c_val].get_type() == LEX_ARR){
		int prev_val = c_val;
		gl();
		if(c_type == LEX_LAR){
			gl();
			if(c_type != LEX_NUM) throw(cur_lex);
			prev_val = 2048*prev_val + c_val;
			if(flag != 0){
				prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
				prog.put_lex(Lex(LEX_NUM, 1));
				if(flag == -1) prog.put_lex(Lex(LEX_MINUSD));
				else if(flag == 1) prog.put_lex(Lex(LEX_PLUSD));
			}
			gl();
			if(c_type != LEX_RAR) throw(cur_lex);
			gl();
			if(is_def()){
				prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
				D();
			}else if(c_type == LEX_INC){
				prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
				prog.put_lex(Lex(LEX_NUM, 1));
				prog.put_lex(Lex(LEX_PLUSD));
				gl();
			}else if(c_type == LEX_DEC){
				prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
				prog.put_lex(Lex(LEX_NUM, 1));
				prog.put_lex(Lex(LEX_MINUSD));
				gl();
			}else expresion();
		}else if(c_type == LEX_DEF){
			gl();
		}else expresion();
	}else{
		int prev_val = c_val;
		if(flag != 0){
			prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
			prog.put_lex(Lex(LEX_NUM, 1));
			if(flag == -1) prog.put_lex(Lex(LEX_MINUSD));
			else if(flag == 1) prog.put_lex(Lex(LEX_PLUSD));
		}
		gl();
		if(is_def()){
			prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
			D();
		}else if(c_type == LEX_INC){
			prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
			prog.put_lex(Lex(LEX_NUM, 1));
			prog.put_lex(Lex(LEX_PLUSD));
			gl();
		}else if(c_type == LEX_DEC){
			prog.put_lex(Lex(POLIZ_ADDRESS, prev_val));
			prog.put_lex(Lex(LEX_NUM, 1));
			prog.put_lex(Lex(LEX_MINUSD));
			gl();
		}else expresion();
	}
}
