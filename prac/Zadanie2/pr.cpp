#include <iostream>
#include <cstring>
#define MAX 20

class student{
    int _year,_code;
    char *_name;
    static int num;
public:
    student(int a = 0,int b = 0,char *c = nullptr):_year(a),_code(b){
        num++;
        if(c != nullptr){
        _name = new char[strlen(c)+1];
        for (int i = 0;i<(int)strlen(c);i++){_name[i]=c[i];}
        _name[strlen(c)]='\0';
        }
        else {_name = nullptr;}
    }
    int gety()const{return _year;}
    int getcode()const{return _code;}
    char * getname()const{return _name;}
    student(student & s):student(s.gety(),s.getcode(),s.getname()){}
    static int Nums(){return num;}
    ~student(){
        num--;
        delete [] _name;
    }
    student & operator=(const student & s){
        if(_name != nullptr) delete[] _name;
        _year = s.gety();
        _code = s.getcode();
        if(s.getname() != nullptr){
        _name = new char[strlen(s.getname())+1];
        for (int i = 0;i<(int)strlen(s.getname());i++){_name[i]=s.getname()[i];}
        _name[strlen(s.getname())]='\0';
        }
        else {
            _name = nullptr;
        }
        return *this;
    }
    friend std::ostream & operator<<(std::ostream & s,const student & a);
};

class _class{
    student *a;
    int _size;
    static int classes;
public:
    _class():_size(0){a=nullptr;classes++;}
    virtual int getnumber()const=0;
    student & operator[](int index)const{return a[index];}
    ~_class(){if(a != nullptr) delete []a;classes--;}
    friend std::ostream & operator<<(std::ostream & ,const _class & );
    static int Nums(){return classes;}
    _class & operator=(const _class & cl){
        if(a) delete [] a;
        a = new student[cl._size];
        for (int i = 0;i < cl._size; i++){a[i] = cl.a[i];}
        _size=cl._size;
        return *this;
    }
    _class(const _class & cl){
    	_size=0;
    	a=nullptr;
    	*this = cl;
    	classes++;
    }
    int getsize()const{return _size;}
    void addst(student st){
        student* newArray = new student[_size + 1];
        for(int i = 0; i < _size; i++){newArray[i]=a[i];}
        newArray[_size]=st;
        delete [] a;
        a = newArray;
        _size++;
    }
};

int student::num = 0;
int _class::classes = 0;
std::ostream & operator<<(std::ostream & s,const _class & cl){
     for (int i = 0;i<cl._size;i++){
        s<<"num "<<i<<":\t";
        s<<cl[i];
     }
     return s;
 }
std::ostream & operator<<(std::ostream & s,const student & st){
    s<<"code="<<st._code<<'\t';
    s<<"year="<<st._year<<'\t';
    if(st._name != nullptr){std::cout<<"name="<<st._name<<'\n';}
    else {s<<"name=empty"<<'\n';}
    return s;
}
class _class1:public _class{
public:
	int getnumber()const{return 0;}
};
int main()
{    
    char s[4];
    s[0]='a';
    s[1]='b';
    s[2]='c';
    s[3]='\0';
    student st(1,2,s),s1;
    _class1 cl;
    cl.addst(st);
    _class1 cl2(cl);
    cl2.addst(s1);
    std::cout <<cl <<'\n';
    std::cout <<cl2<<'\n';
    std::cout <<student::Nums() <<'\n';
    std::cout <<_class::Nums() <<'\n';
    return 0;
}
