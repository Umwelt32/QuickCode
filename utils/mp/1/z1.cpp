#include <iostream>
#include <fstream>

using namespace std;

class K1
{
    public:
        K1(const int &i1=0,const string &s1="BRAK"):p1_(i1),p2_(new string(s1)){;}
        K1(const K1 &k): K1(){*this=k;}
        K1 &operator=(const K1 &k){if(k.p2_!=nullptr){(*(this->p2_))=(*k.p2_);}this->p1_=k.p1_;return *this;}
        friend ostream& operator<<(ostream& os, const K1& obj) {os<<(obj.p1_)<<" "<<(*obj.p2_)<<endl; return os;}
        virtual ~K1(){delete p2_;}
    private:
        int p1_;
        string *p2_;
};

class K2
{
    public:
        K2():tab(new K1[2]){;}
        K2(const K2 &k): K2(){*this=k;}
        virtual ~K2(){delete[] tab;}
        K2 &operator=(const K2 &k) {this->tab[0]=k.tab[0];this->tab[1]=k.tab[1];return *this;}
        K1 &operator[](int idx) {if ((idx<2)&&(idx>=0)){return tab[idx]; }else{return tab[0];}}
        friend ostream& operator<<(ostream& os, const K2& obj) {os<<obj.tab[0]<<obj.tab[1]; return os;}

    private:
        K1 *tab;
};

int main()
{
    const K1 ob1,ob2(2,"koty"),ob3(3,"psy");
    cout << ob1 << ob2 << ob3 << endl;
    K2 *wsk1 = new K2;
    cout<<"**1**\n"<<(*wsk1);
    (*wsk1)[0]=ob2;
    (*wsk1)[1]=ob3;
    cout<<"**2**\n"<<(*wsk1);
    const K2 ob4(*wsk1);
    delete wsk1;
    wsk1=nullptr;
    K2 ob5;
    ob5[0]=K1(1,"kangurek");
    ob5[1]=K1(2,"papugi");
    K2 *tablica[3];
    tablica[0]=new K2(ob4);
    tablica[1]=new K2;
    tablica[2]=new K2(ob5);
    cout<<"**3**\n";
    for (int i=0;i<3;++i)cout<<*tablica[i];
    for (int i=0;i<3;++i) delete tablica[i];
    return 0;
}
