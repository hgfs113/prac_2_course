#include <iostream>
#include <cstring>

using namespace std;

class msf{
    int _len;
    char * _str;
public:
    msf(){
        _len = 0;
        _str = nullptr;
    }
    int getlen() const { return _len; }
    char* getstr() const { return _str; }
    msf(const int l, const char * s){
        _len = l;
        _str = new char[_len];
        for(int j = 0; j < _len; j++){ _str[j] = s[j];}
    }
    msf(const char * s) : msf(strlen(s)+1,s){}
    msf(const msf & s) : msf(s.getlen(), s.getstr()) { }
    void operator= (const msf & s){
        if(_len != 0) delete [] _str;
        _len = s.getlen();
        _str = new char[_len];
        for(int j = 0; j < _len; j++){ _str[j] = s.getstr()[j];}}
    void print() const {if(_len)cout << _str <<endl; }
    bool operator>(const msf & b)const {
        int i=0;
        while((i < _len) && (i<b.getlen())){
            if(_str[i] != b.getstr()[i]){return _str[i] > b.getstr()[i];}
            i++;
        }
        if((i-b.getlen()) && (!(i-_len))) return 1;
        return 0;}
    bool operator<(const msf & b)const {
        int i=0;
        while((i < _len) && (i<b.getlen())){
            if(_str[i] != b.getstr()[i]){ return _str[i] < b.getstr()[i];}
            i++;
        }
        if((i-b.getlen()) && !(i-_len)) return 1;
        return 0;}
    bool operator==(const msf & b) const{
        if(_len != b.getlen()) return 0;
        int i=0;
        while(i < _len){
            if(_str[i] != b.getstr()[i]){ return 0;}
            i++;
        }
        return 1;}
    bool operator!=(const msf & b) const{return !operator==(b);}
    int msftoi() const {
        int rez = 0;
        for (int i = 0; i < _len-1; i++){
            rez *=10;
            rez += _str[i] -'0';
        }
        return rez;}
    double msftod() const{
        double rez = 0,dr = 0;
        int j = 0;
        bool pnt = 0;
        for (int i = 0; i < _len-1; i++){
            if(_str[i] == '.'){ pnt = 1;}
            else if(pnt){
                j++;
                dr *=10;
                dr +=_str[i]-'0';
            }
            else{
                rez *=10;
                rez += _str[i] -'0';
            }
        }
        while(j--){dr/=10;}
        return rez+dr;}
    float msftof() const { return (float)msftod();}
    ~msf(){
        if(_len != 0)
            delete [] _str;}
};

msf operator+ (const msf & a, const msf & b){
    int len = a.getlen() + b.getlen()-1;
    char * tmp = new char[len];
    for (int i = 0; i < len; i++){
        if(i < a.getlen()-1) { tmp[i]=a.getstr()[i]; }
        else { tmp[i] = b.getstr()[i-a.getlen()+1]; }
    }
    return msf(len, tmp);}
int printf(const char*,msf a){ a.print(); return 0;}

int main(){
    int k = -1;
    msf A,B,C;
    char s[20];
    while(k){
        cout << "0 \t- Exit(Vihod)\n1 \t- Input string A\n2 \t- Input string B\n3 \t- Input string C\n4 \t- A == B\n5 \t- A > B\n6 \t- C = A + B\n7 \t- print A\n8 \t- print B\n9 \t- print C\n10 \t- print A to int\n11 \t- print A to double\n10 i 11 konechno, ochen' useful fumctions..." << endl;
        scanf("%d",&k);
        switch (k) {
            case 0: break;
            case 1: {cout <<"input:";cin>>s; A = msf(s);cout<< "A:";A.print();};break;
            case 2: cout <<"input:";cin>>s; B = msf(s);cout<< "B:";B.print();break;
            case 3: cout <<"input:";cin>>s; C = msf(s);cout<< "C:";C.print();break;
            case 4: cout << (A == B)<<endl;break;
            case 5: cout << (A > B)<<endl;break;
            case 6: {C = A + B; C.print();};break;
            case 7: A.print();break;
            case 8: B.print();break;
            case 9: C.print();break;
            case 10: cout<<A.msftoi()<<endl;break;
            case 11: cout<<A.msftod()<<endl;break;
        }
        cout <<"-/-/-/-/-/-/-/-/-"<<endl;
    }
    cout << "have a niec day"<< endl;
    return 0;
}
