#include <iostream>

using namespace std;

class K1
{
    public:
        K1():p1(new string[2]){;}
        K1(const string &s1,const string &s2=""): K1(){p1[0]=s1;p1[1]=s2;}
        K1(const K1 &k): K1(){*this=k;}
        K1 &operator=(const K1 &k){if(k.p1!=nullptr){this->p1[0]=k.p1[0];this->p1[1]=k.p1[1];};return *this;}
        string &operator[](int idx) {if ((idx<2)&&(idx>=0)){return p1[idx]; }else{return p1[0];}}
        friend ostream& operator<<(ostream& os, const K1& obj) {os<<obj.p1[0]<<" "<<obj.p1[1]; return os;}
        virtual ~K1(){delete[] p1;}

    private:
        string *p1;
};

class K2
{
    public:
        K2(){;}
        K2(const string &s1,const string &s2=""): dane(s1,s2){;}
        virtual ~K2(){;}
        string &operator[](int idx) {return dane[idx];}
        friend ostream& operator<<(ostream& os, const K2& obj) {os<<obj.dane; return os;}
    private:
        K1 dane;
};

int main()
{
    K2 o1,o2;
    const K2* wsk1 = new K2("Ala","Ola");
    const K2 o3(*wsk1);
    cout<<"o2 : "<<o2<<endl;
    cout<<"o3 : "<<o3<<endl;
    cout<<"*******"<<endl;
    delete wsk1;
    wsk1 = 0;
    wsk1 = new K2(o3);
    o2 = *wsk1;
    cout<<"o1 : "<<o1<<endl;
    cout<<"o2 : "<<o2<<endl;
    cout<<"*******"<<endl;
    delete wsk1;
    wsk1 = 0;

    o1 = K2("Ewa","Iza");
    cout<<o1[0]<<", "<<o1[1]<<endl;
    o1[1]="Jan";
    cout<<o1[0]<<", "<<o1[1]<<endl;

    return 0;
}
