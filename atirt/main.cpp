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
    std::string szin;


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
    }

    string kiir()
    {
        return szinek[szin];
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

    string kiir()
    {
        return "F";
    }
};

struct Futoszalag:Mezo
{
    int irany;
    string kovetkezo_szinek = "";

    Futoszalag()
    {
        irany = 1;
    }

    Futoszalag(int x_, int y_, int irany_)
    {
        irany = irany_;
        id = irany_;
        x = x_;
        y = y_;
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
    }

    string kiir()
    {
        if (irany == 1) return "^";
        if (irany == 2) return "<";
        if (irany == 3) return "v";
        if (irany == 4) return ">";
    }

};


class Kezelo
{
    vector<vector<Mezo>> mezok;

    map<pair<int, int>, Termelo> osszes_termelo;
    map<pair<int, int>, Fogyaszto> osszes_fogyaszto;
    map<pair<int, int>, Futoszalag> osszes_futoszalag;
public:

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

    void termelo_hozzaad(int x, int y, int sebesseg_, string term_szin)
    {
        Termelo t(x, y, sebesseg_, term_szin);
        mezok[y][x].id = 6;
        mezok[y][x].szin = term_szin;
        osszes_termelo.insert({{x, y}, t});
    }

    void fogyaszto_hozzaad(int x, int y)
    {
        Fogyaszto f(x, y);
        mezok[y][x].id = 5;
        osszes_fogyaszto.insert({{x, y}, f});
    }

    void futoszalag_hozzaad(int x, int y, int irany_)
    {
        Futoszalag f(x, y, irany_);
        mezok[y][x].id = irany_;
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

                if (m.get_id() == 6) cout << osszes_termelo[{m.x, m.y}].kiir(); // termelo

            }
            cout << "\n";
        }
    }
};

int main()
{

    Kezelo K;
    K.futoszalag_hozzaad(3, 3, 4);
    K.fogyaszto_hozzaad(4, 5);
    K.megjelenit();
    return 0;
}
