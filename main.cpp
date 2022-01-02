#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;
map<string, string> szinek = {
        {"R", "R"},
        {"G", "G"},
        {"B", "B"},
        {"RG", "S"},
        {"RB", "L"},
        {"BG", "C"},
        {"RGB", "W"}
};
int SIZE = 10;

struct Mezo
{

    int x, y;
    int id;
    string szin;


    Mezo()
    {
        id = 0;
    }

    Mezo(int xx, int yy, string sz, int id_)
    {
        x = xx;
        y = yy;
        szin = sz;
        id = id_;
    }

    int get_id()
    {
        return id;
    }
};

struct Termelo: Mezo
{
    string termelt_szin;
    int sebesseg;

    Termelo()
    {
        id = 6;
    }
    Termelo(int xx, int yy, int sebesseg_, string term_)
    {
        x = xx;
        y = yy;
        sebesseg = sebesseg_;
        termelt_szin = term_;
        szin = term_;
    }

    string kiir()
    {
        return szinek[termelt_szin];
    }
};

struct Fogyaszto:Mezo
{
    map<string, int> osszes_szin;
    string kovetkezo_szinek = "";

    Fogyaszto()
    {
        id = 5;
    }

    Fogyaszto(int xx, int yy)
    {
        x = xx;
        y = yy;
        id = 5;
    }
    void szin_bemegy(string szin)
    {
        for (auto f: osszes_szin)
        {
            if (f.first == szin && f.second > 0)
            {
                osszes_szin[f.first]--;
            }
        }
    }

    void keveredes()
    {
        string uj = "";
        if(kovetkezo_szinek.find("R") != string::npos){ uj += "R";}
        if(kovetkezo_szinek.find("G") != string::npos){ uj += "G";}
        if(kovetkezo_szinek.find("B") != string::npos){ uj += "B";}
        szin = uj;
    }

    void vart_szin_hozzaad(std::pair<string, int> uj_szin)
    {
        osszes_szin.insert(uj_szin);
    }

    string kiir()
    {
        return "F";
    }
};

struct Futoszalag:Mezo
{
    int irany;
    string kovetkezo_szinek;

    Futoszalag()
    {
        irany = 1;
        kovetkezo_szinek = "";
        szin = "";
    }


    Futoszalag(int x_, int y_, int irany_)
    {
        irany = irany_;
        id = irany_;
        x = x_;
        y = y_;
        kovetkezo_szinek = "";
        szin = "";
    }

    void szin_megkap(string s)
    {
        kovetkezo_szinek += s;
    }

    void keveredes()
    {
        string uj = "";
        if(kovetkezo_szinek.find("R") != string::npos){ uj += "R";}
        if(kovetkezo_szinek.find("G") != string::npos){ uj += "G";}
        if(kovetkezo_szinek.find("B") != string::npos){ uj += "B";}
        szin = uj;
        kovetkezo_szinek = "";
    }

    string kiir()
    {
        if (szin != "") return szinek[szin];
        if (irany == 1) return "^";
        if (irany == 2) return "<";
        if (irany == 3) return "v";
        if (irany == 4) return ">";
    }

};


class Kezelo
{
public:
    vector<vector<Mezo>> mezok;

    map<pair<int, int>, Termelo> osszes_termelo;
    map<pair<int, int>, Fogyaszto> osszes_fogyaszto;
    map<pair<int, int>, Futoszalag> osszes_futoszalag;

    Kezelo()
    {
        for (int i = 0; i < SIZE; i++)
        {
            vector<Mezo> uj_sor;
            for (int j = 0; j < SIZE; j++)
            {
                Mezo m(j, i, "", 0);
                uj_sor.push_back(m);
            }
            mezok.push_back(uj_sor);
        }
    }

    void termelo_letrehoz(int x, int y, int sebesseg_, string term_szin)
    {
        Termelo t(x, y, sebesseg_, term_szin);
        mezok[y][x].id = 6;
        mezok[y][x].szin = term_szin;
        osszes_termelo.insert({{x, y}, t});
    }

    void fogyaszto_letrehoz(int x, int y, string szin1, int szin1_, string szin2, int szin2_)
    {
        Fogyaszto f(x, y);
        mezok[y][x].id = 5;
        f.osszes_szin.insert({szin1, szin1_});
        f.osszes_szin.insert({szin2, szin2_});
        osszes_fogyaszto.insert({{x, y}, f});
    }

    void futoszalag_letrehoz(int x, int y, int irany_)
    {
        Futoszalag f(x, y, irany_);

        if (mezok[y][x].id != 6) mezok[y][x].id = irany_;

        osszes_futoszalag.insert({{x, y}, f});
    }

    void megjelenit()
    {
        for (int i = 0; i < mezok.size(); i++)
        {
            for (int j = 0; j < mezok[i].size(); j++)
            {
                Mezo m = mezok[i][j];

                if (m.get_id() == 0) cout << ".";

                if (m.get_id() > 0 && m.get_id() < 5) cout << osszes_futoszalag[{m.x, m.y}].kiir();//futoszalag

                if (m.get_id() == 5) cout << osszes_fogyaszto[{m.x, m.y}].kiir(); // fogyaszto

                if (m.get_id() == 6)
                {
                    cout << osszes_termelo[{m.x, m.y}].kiir(); // termelo

                }
            }
            cout << "\n";
        }

        for (auto k: osszes_fogyaszto)
        {
            for (auto f: k.second.osszes_szin)
            {
                cout << f.first << " " << f.second << endl;
            }
        }

    }

    void folyamat_1()
    {
        for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
        {
            it->second.szin_bemegy(it->second.szin);
        }

    }

    void folyamat_2()
    {


        for (map<pair<int, int>, Futoszalag>::iterator it = osszes_futoszalag.begin(); it != osszes_futoszalag.end(); it++)
        {
            Futoszalag f = it->second;
            if (f.irany == 1)
            {
                if (osszes_futoszalag.count({f.x, f.y-1})) osszes_futoszalag[{f.x, f.y-1}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x, f.y-1})) osszes_fogyaszto[{f.x, f.y-1}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 2)
            {
                if (osszes_futoszalag.count({f.x-1, f.y})) osszes_futoszalag[{f.x-1, f.y}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x-1, f.y})) osszes_fogyaszto[{f.x-1, f.y}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 3)
            {
                if (osszes_futoszalag.count({f.x, f.y+1})) osszes_futoszalag[{f.x, f.y+1}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x, f.y+1})) osszes_fogyaszto[{f.x, f.y+1}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 4)
            {
                if (osszes_futoszalag.count({f.x+1, f.y})) osszes_futoszalag[{f.x+1, f.y}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x+1, f.y})) osszes_fogyaszto[{f.x+1, f.y}].szin_bemegy(f.szin);

                f.szin == "";
            }

            osszes_futoszalag.insert({{f.x, f.y}, f});

        }

        termelo_atad();



    }

    void folyamat_3()
    {
        for (map<pair<int, int>, Futoszalag>::iterator it = osszes_futoszalag.begin(); it != osszes_futoszalag.end(); it++)
        {
            it->second.keveredes();
        }
    }

    void folyamat_4(int hanyadik_kor)
    {
        cout << "hanyadik_kor: " << hanyadik_kor << endl;

        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it != osszes_termelo.end(); it++)
        {
            Termelo t = it->second;

            if (hanyadik_kor % t.sebesseg == 0)
            {
                t.szin = t.termelt_szin;
            } else t.szin = "";
            osszes_termelo[{t.x, t.y}] = t;
        }
    }

    void termelo_atad()
    {
        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it != osszes_termelo.end(); it++)
        {
            Termelo m = it->second;
            int x = osszes_termelo[{m.x, m.y}].x;
            int y = osszes_termelo[{m.x, m.y}].y;


            osszes_futoszalag[{m.x, m.y}].szin_megkap(osszes_termelo[{m.x, m.y}].szin);

        }
    }
};

int main()
{

    Kezelo K;


    K.termelo_letrehoz(4, 5, 1, "R");
//    K.termelo_letrehoz(4, 3, 4, "G");
//    K.termelo_letrehoz(5, 4, 4, "B");
//    K.termelo_letrehoz(6, 7, 1, "R");

    K.fogyaszto_letrehoz(2, 0, "R", 3, "B", 5);
    K.fogyaszto_letrehoz(7, 7, "R", 3, "B", 5);

    K.futoszalag_letrehoz(4, 5, 1);
    K.futoszalag_letrehoz(4, 3, 3);
    K.futoszalag_letrehoz(5, 4, 2);
    K.futoszalag_letrehoz(6, 7, 4);
    K.futoszalag_letrehoz(2, 1, 1);
    K.futoszalag_letrehoz(2, 2, 1);
    K.futoszalag_letrehoz(2, 3, 1);
    K.futoszalag_letrehoz(2, 4, 1);
    K.futoszalag_letrehoz(3, 4, 2);
    K.futoszalag_letrehoz(4, 4, 2);


    K.megjelenit();

    cout << endl;
    for (int i = 0; i < 10; i++)
    {




        K.folyamat_1();

        K.folyamat_2();

        K.folyamat_3();

        K.folyamat_4(i);


    K.megjelenit();
    }


    return 0;
}
