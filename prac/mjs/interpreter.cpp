#include "interpreter.h"
using namespace std;
extern vector <Ident> TID;

void Interpretator::interpretation(){
	pars.analyze ();
	E.execute(pars.prog);
}
char * itoa(int i){
	char * str =new char[11];
	sprintf(str,"%d",i);
	return str;
}
int _stoi(string s){
	int res = 0;
	for(int i = 0; i < s.length(); i++){
		if(isdigit(s[i])){
			res *= 10;
			res += s[i] - '0';
		}else return res;
	}
	return res;
}
void Executer::execute(Poliz& prog){
	Stack <arg, 100> args;
	arg a, b;
	int i,j,index = 0, size = prog.get_free();
	while(index < size){
		pc_el = prog[index];
		switch(pc_el.get_type()){
			case LEX_NUM:case LEX_BOOL:case POLIZ_LABEL:case POLIZ_ADDRESS:case OBJ_ENV:
				args.push(arg(pc_el.get_type(),pc_el.get_value()));
				break;
			case LEX_TRUE:case LEX_FALSE:
				args.push(arg(LEX_BOOL,pc_el.get_value()));
				break;
			case LEX_IDENT:
				i = pc_el.get_value();
				if(TID[i].get_assign()){
					if(TID[i].get_type() == LEX_STR){
						args.push(arg(LEX_STR, TID[i].get_name()));
					}
					else{
						args.push(arg(TID[i].get_type(), TID[i].get_value()));
					}
				}
				else throw("POLIZ: indefinite identifier\n");
				break;
			case LEX_STR:
				args.push(arg(LEX_STR, pc_el.get_name()));
				break;
			case LEX_NOT:
				a = args.pop();
				a.value = !a.value;
				args.push(a);
				break;
			case LEX_OR:
				a = args.pop();
				b = args.pop();
				a.value = a.value || b.value;
				args.push(a);
				break;
			case LEX_AND:
				a = args.pop();
				b = args.pop();
				a.value = a.value && b.value;
				args.push(a);
				break;
			case POLIZ_GO:
				index = args.pop().value - 1;
				break;
			case POLIZ_FGO:
				i = args.pop().value;
				if(!args.pop().value)
					index = i - 1;
				break;
			case LEX_WRITE:
				pc_el = prog[++index];
				if(pc_el.get_type() == LEX_IDENT){
					i = pc_el.get_value();
					if(TID[i].get_type() == LEX_ARR){
					
					}
					else{
						switch(TID[i].get_type()){
							case LEX_NUM:
								cout << TID[i].get_value();
								break;
							case LEX_STR:
								cout << TID[i].get_name();
								break;
							case LEX_BOOL:
								cout << TID[i].get_value();
								break;
							case LEX_NULL:
								cout << "null";
								break;
							case LEX_NAN:
								cout << "NaN";
								break;
							default: throw("Error in write\n");
						}
					}
				}
				else if(pc_el.get_type() == LEX_NUM){
					std::cout<<pc_el.get_value()<<std::endl;
				}
				else if(pc_el.get_type() == LEX_BOOL){
					if(pc_el.get_value())
						std::cout<<"true"<<std::endl;
					else
						std::cout<<"false"<<std::endl;
				}
				else if(pc_el.get_type() == LEX_STR){
					std::cout<<pc_el.get_name();
				}
				cout<<endl;
				break;
			case LEX_READ:
				pc_el = prog[++index];
				int k;
				if(true){
					scanf("%d",&k);
				}
				else{
					char j[20];
				rep:
					cin>>j;
					if(!strcmp(j,"true"))
						k = 1;
					else if(!strcmp(j,"false"))
						k = 0;
					else{
						goto rep;
					}
				}
				std::cout<<pc_el.get_value()<<std::endl;
				TID[pc_el.get_value()].put_value(k);
				TID[pc_el.get_value()].put_assign();
				break;
			case LEX_EQ:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name==a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name == a.name));
						}else args.push(arg(LEX_BOOL, b.value == a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value == a.value));
					}
				break;
			case LEX_LESS:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name<a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name < a.name));
						}else args.push(arg(LEX_BOOL, b.value < a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value < a.value));
					}
				break;
			case LEX_GRT:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name > a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name > a.name));
						}else args.push(arg(LEX_BOOL, b.value > a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value > a.value));
					}
				break;
			case LEX_LEQ:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name <= a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name <= a.name));
						}else args.push(arg(LEX_BOOL, b.value <= a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value <= a.value));
					}
				break;
			case LEX_GEQ:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name >= a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name >= a.name));
						}else args.push(arg(LEX_BOOL, b.value >= a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value >= a.value));
					}
				break;
			case LEX_NEQ:
					a = args.pop();
					b = args.pop();
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name != a.name));
					}
					else if(b.type == LEX_IDENT){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name != a.name));
						}else args.push(arg(LEX_BOOL, b.value != a.value));
					}
					else{
						args.push(arg(LEX_BOOL, b.value != a.value));
					}
				break;
			case LEX_PLUS:
				a = args.pop();
				b = args.pop();
				if(b.type == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, a.value + b.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, a.value + b.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(b.value) + a.name));
				}else if(b.type == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value + a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, b.value || a.value));
					else if(a.type == LEX_STR){
						if(b.value) args.push(arg(LEX_STR, "true" + a.name));
						else args.push(arg(LEX_STR, "false" + a.name));
					}
				}else if(b.type == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, b.name + to_string(a.value)));
					else if(a.type == LEX_BOOL){
						if(a.value) args.push(arg(LEX_NUM, b.value + "true"));
						else args.push(arg(LEX_NUM, b.value + "false"));
					}else if(a.type == LEX_STR) args.push(arg(LEX_STR, b.name + a.name));
				}
				break;
			case LEX_MUL:
				a = args.pop();
				b = args.pop();
				if(b.type == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, a.value * b.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, a.value * b.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(b.value * _stoi(a.name))));
				}else if(b.type == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value * a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, b.value && a.value));
					else if(a.type == LEX_STR){
						if(b.value) args.push(arg(LEX_STR, a.name));
						else args.push(arg(LEX_STR, ""));
					}
				}else if(b.type == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(b.name) * a.value)));
					else if(a.type == LEX_BOOL){
						if(a.value) args.push(arg(LEX_STR, b.name));
						else args.push(arg(LEX_STR, ""));
					}else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(_stoi(b.name)*_stoi(a.name))));
				}
				break;
			case LEX_MINUS:
				a = args.pop();
				b = args.pop();
				if(b.type == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value - a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, b.value - a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(b.value - _stoi(a.name))));
				}else if(b.type == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value - a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, b.value - a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, a.name));
				}else if(b.type == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(b.name) - a.value)));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_STR, b.name));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(_stoi(b.name)-_stoi(a.name))));
				}
				break;
			case LEX_DIV:
				a = args.pop();
				b = args.pop();
				if(((a.type == LEX_STR) && _stoi(a.name) == 0) || ((a.type != LEX_STR) && (a.value == 0))){
					args.push(arg(LEX_NAN, 0));
					break;
				}
				if(b.type == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value / a.value));
					else if(a.type == LEX_BOOL) args.push(b);
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(b.value / _stoi(a.name))));
				}else if(b.type == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, b.value / a.value));
					else if(a.type == LEX_BOOL) args.push(b);
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, a.name));
				}else if(b.type == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(b.name) / a.value)));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_STR, b.name));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(_stoi(b.name) / _stoi(a.name))));
				}
				break;
			case LEX_EQ2:
				a = args.pop();
				b = args.pop();
				if(a.type != b.type){
					args.push(arg(LEX_BOOL, 0));
				}else{
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name == a.name));
					}else if(b.type == POLIZ_ADDRESS){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name == a.name));
						}else args.push(arg(LEX_BOOL, b.value == a.value));
					}else args.push(arg(LEX_BOOL, b.value == a.value));
				}
				break;
			case LEX_NEQ2:
				a = args.pop();
				b = args.pop();
				if(a.type != b.type){
					if(b.type == LEX_STR){
						args.push(arg(LEX_BOOL, b.name != a.name));
					}else if(b.type == POLIZ_ADDRESS){
						if(TID[b.value].get_type() == LEX_STR){
							args.push(arg(LEX_BOOL, b.name != a.name));
						}else args.push(arg(LEX_BOOL, b.value != a.value));
					}else args.push(arg(LEX_BOOL, b.value != a.value));
				}
				break;
			case LEX_DEF:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(a.type == LEX_NUM){
					TID[j].put_value(a.value);
					TID[j].put_type(LEX_NUM);
				}else if(a.type == LEX_BOOL){
					TID[j].put_value(a.value);
					TID[j].put_type(LEX_BOOL);
				}else if(a.type == LEX_STR){
					TID[j].put_name(a.name);
					TID[j].put_type(LEX_STR);
				}else if(a.type == LEX_NAN){
					TID[j].put_type(LEX_NAN);
				}
				TID[j].put_assign();
				break;
			case LEX_PLUSD:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(TID[j].get_type() == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() + a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, TID[j].get_value() + a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(TID[j].get_value()) + a.name));
				}else if(TID[j].get_type() == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() + a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, TID[j].get_value() || a.value));
					else if(a.type == LEX_STR){
						if(b.value) args.push(arg(LEX_STR, "true" + a.name));
						else args.push(arg(LEX_STR, "false" + a.name));
					}
				}else if(TID[j].get_type() == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, TID[j].get_name() + to_string(a.value)));
					else if(a.type == LEX_BOOL){
						if(a.value) args.push(arg(LEX_NUM, TID[j].get_value() + "true"));
						else args.push(arg(LEX_NUM, TID[j].get_value() + "false"));
					}else if(a.type == LEX_STR) args.push(arg(LEX_STR, TID[j].get_name() + a.name));
				}
				a = args.pop();
				TID[j].put_type(a.type);
				TID[j].put_value(a.value);
				TID[j].put_name(a.name);
				TID[j].put_assign();
				break;
			case LEX_MINUSD:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(TID[j].get_type() == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() - a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, TID[j].get_value() - a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(TID[j].get_value() - _stoi(a.name))));
				}else if(TID[j].get_type() == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() - a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, TID[j].get_value() - a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, a.name));
				}else if(TID[j].get_type() == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(TID[j].get_name()) - a.value)));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_STR, TID[j].get_name()));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR,to_string(_stoi(TID[j].get_name())-_stoi(a.name))));
				}
				a = args.pop();
				TID[j].put_type(a.type);
				TID[j].put_value(a.value);
				TID[j].put_name(a.name);
				TID[j].put_assign();
				break;
			case LEX_MULD:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(TID[j].get_type() == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() * a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, TID[j].get_value() * a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(TID[j].get_value() * _stoi(a.name))));
				}else if(TID[j].get_type() == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() * a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, TID[j].get_value() && a.value));
					else if(a.type == LEX_STR){
						if(b.value) args.push(arg(LEX_STR, a.name));
						else args.push(arg(LEX_STR, ""));
					}
				}else if(TID[j].get_type() == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(TID[j].get_name()) * a.value)));
					else if(a.type == LEX_BOOL){
						if(a.value) args.push(arg(LEX_STR, TID[j].get_name()));
						else args.push(arg(LEX_STR, ""));
					}else if(a.type == LEX_STR) args.push(arg(LEX_STR,to_string(_stoi(TID[j].get_name()) * _stoi(a.name))));
				}
				a = args.pop();
				TID[j].put_type(a.type);
				TID[j].put_value(a.value);
				TID[j].put_name(a.name);
				TID[j].put_assign();
				break;
			case LEX_DIVD:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(((a.type == LEX_STR) && (_stoi(a.name) == 0)) || ((a.type != LEX_STR)&&(a.value == 0))){
					args.push(arg(LEX_NAN, 0));
					break;
				}
				if(TID[j].get_type() == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() / a.value));
					else if(a.type == LEX_BOOL) args.push(b);
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(TID[j].get_value()/_stoi(a.name))));
				}else if(TID[j].get_type() == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() / a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, TID[j].get_value() / a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, a.name));
				}else if(TID[j].get_type() == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(TID[j].get_name())/a.value)));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_STR,  TID[j].get_name()));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR,to_string(_stoi(TID[j].get_name())/_stoi(a.name))));
				}
				a = args.pop();
				TID[j].put_type(a.type);
				TID[j].put_value(a.value);
				TID[j].put_name(a.name);
				TID[j].put_assign();
				break;
			case LEX_MODD:
				a = args.pop();
				b = args.pop();
				j = b.value;
				if(((a.type == LEX_STR) && (_stoi(a.name) == 0)) || ((a.type != LEX_STR)&&(a.value == 0))){
					args.push(arg(LEX_NAN, 0));
					break;
				}
				if(TID[j].get_type() == LEX_NUM){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() % a.value));
					else if(a.type == LEX_BOOL) args.push(b);
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, to_string(TID[j].get_value() % _stoi(a.name))));
				}else if(TID[j].get_type() == LEX_BOOL){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, TID[j].get_value() % a.value));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_BOOL, TID[j].get_value() % a.value));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR, a.name));
				}else if(TID[j].get_type() == LEX_STR){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(_stoi(TID[j].get_name())%a.value)));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_STR,  TID[j].get_name()));
					else if(a.type == LEX_STR) args.push(arg(LEX_STR,to_string(_stoi(TID[j].get_name()) % _stoi(a.name))));
				}
				a = args.pop();
				TID[j].put_type(a.type);
				TID[j].put_value(a.value);
				TID[j].put_name(a.name);
				TID[j].put_assign();
				break;
			case LEX_DOT:
				pc_el = prog[++index];
				a = args.pop();
				if(a.type == OBJ_ENV){
					char *s = getenv(pc_el.get_name().c_str());
					if(s == nullptr) args.push(arg(LEX_STR,"NULL"));
					else args.push(arg(LEX_STR, s));
				}
				else if(pc_el.get_name() == "toString"){
					if(a.type == LEX_NUM) args.push(arg(LEX_STR, to_string(a.value)));
					else if(a.type == LEX_BOOL){
						if(a.value) args.push(arg(LEX_STR, "true"));
						else args.push(arg(LEX_STR, "false"));
					}else args.push(a);
				}else if(pc_el.get_name() == "MAX_VALUE"){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, 2147483647));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, 1));
					else throw("Incorrect property");
				}else if(pc_el.get_name() == "MIN_VALUE"){
					if(a.type == LEX_NUM) args.push(arg(LEX_NUM, -2147483648));
					else if(a.type == LEX_BOOL) args.push(arg(LEX_NUM, 0));
					else throw("Incorrect property");
				}else if(pc_el.get_name() == "length"){
					if(a.type == LEX_ARR) args.push(arg(LEX_NUM, TID[a.value].elem.size()));
					else throw("Incorrect property");
				}
				break;
			default:
				cout<<pc_el.get_type(); 
				throw("unexpected poliz\n");
		}
		++index; 
	}
}
