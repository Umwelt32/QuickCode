#include <iostream>
#include <fstream>

using namespace std;

class K1
{
    public:
        K1(const string &s1=""):p1(new string(s1)){;}
        K1(const K1 &k): K1(){*this=k;}
        K1 &operator=(const K1 &k){if(k.p1!=nullptr){(*this->p1)=(*k.p1);};return *this;}
        const string &get(void)const {return *p1;}
        friend ostream& operator<<(ostream& os, const K1& obj) {os<<(*obj.p1); return os;}
        friend istream& operator>>(istream& is, K1& obj) {is>>(*obj.p1); return is;}
        virtual ~K1(){delete p1;}
    private:
        string *p1;
};

class K2
{
    public:
        K2():txt{K1(""),K1("")},w1(0){;}
        K2(const string &s1,const string &s2="",const int &w=0): txt{K1(s1), K1(s2)},w1(w){;}
        K2(const K2 &k): K2(){*this=k;}
        virtual ~K2(){;}
        K2 &operator+(const K2     &k) {this->w1+=k.w1;return *this;}
        K2 &operator+(const int    &w) {this->w1+=w;return *this;}
        K2 &operator+(const string &s) {this->txt[1]=s;return *this;}
        K2 &operator=(const K2     &k) {this->txt[0]=k.txt[0];this->txt[1]=k.txt[1];this->w1=k.w1;return *this;}
        K2 &operator+=(const int   &w) {return ((*this)+w);}
        friend ostream& operator<<(ostream& os, const K2& obj) {os<<obj.txt[0]<<" "<<obj.txt[1]<<" "<<obj.w1; return os;}
        friend istream& operator>>(istream& is, K2& obj)  {is>>(obj.txt[0]);is>>(obj.txt[1]);is>>(obj.w1); return is;}
        friend K2 operator+(const string &s,K2 &c) {return K2(c.txt[0].get(),s,c.w1);}
    private:
        K1 txt[2];
        int w1;
};

int main()
{
    K2 ob1, ob2;
    const K2 *wsk1 = new K2("Ala", "Koala", 15);
    cout << *wsk1 << endl;
    const K2 ob3(*wsk1);
    delete wsk1;
    wsk1=0;
    const K2 *wsk2 =new K2(ob3);
    cout << *wsk2 << endl;
    ob2 = *wsk2;
    cout << ob1 << endl << *wsk2 << endl;
    delete wsk2;
    wsk2 = 0;
    cout << ob2 << endl;
    cout << (ob2+10) << endl;
    cout<<"***** 3 ******"<<endl;
    K2 tab[4];
    ifstream p("data.txt");
    for (int i=0; ((i<4)&&(p));++i)
    {
        p>>tab[i];
    }
    p.close();
    for (int i=0; i<4;++i)
    {
        tab[i]+=1;
        cout<<tab[i]<<endl;
    }
    cout<<"***** 4 ******"<<endl;
    tab[1]= tab[1] + " Kowalska";
    tab[2]=" Bocian" + tab[3];
    for (int i=0; i<4;++i)
    {
        cout<<tab[i]<<endl;
    }
    cout<<"***** 5 ******"<<endl;
    return 0;
}
