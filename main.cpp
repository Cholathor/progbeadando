#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

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

struct Tile
{
    int x, y, helyezes;
    int g, h, f;

    Tile()
    {
        x = 0;
        y = 0;
        f = 0;
        g = 0;
        h = 0;
    }

    Tile(int xx, int yy)
    {
        Tile();
        x = xx;
        y = yy;
        helyezes = 0;
    }
    void calculate(int celx, int cely, Tile szomszed)
    {
        h = abs(x-celx) + abs(y-cely);
        g = szomszed.g+1;
        f = h + g;
        if (helyezes == 0)
            helyezes = szomszed.helyezes + 1;
    }

    int irany(Tile t)
    {
        if (t.x == x && t.y-1 == y) return 3;
        if (t.x == x && t.y+1 == y) return 1;
        if (t.x-1 == x && t.y == y) return 4;
        if (t.x+1 == x && t.y == y) return 2;
        return 1;
    }

};

bool operator< (const Tile t1, const Tile t2)
{
    if (t1.f < t2.f) return true;
    else if (t1.f == t2.f && t1.helyezes != t2.helyezes) return t1.helyezes > t2.helyezes;
    else if (t1.f == t2.f && t1.x != t2.x) return t1.x < t2.x;
    else if (t1.f == t2.f) return t1.y < t2.y;
    else return false;
}

//bool operator== (const Tile t1, const Tile t2)
//{
//    return (t1.x == t2.x && t1.y == t2.y);
//}



class Kezelo
{
public:
    vector<vector<Mezo>> mezok;

    map<pair<int, int>, Termelo> osszes_termelo;
    map<pair<int, int>, Fogyaszto> osszes_fogyaszto;
    map<pair<int, int>, Futoszalag> osszes_futoszalag;
    vector<vector<Tile>> tiles;

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

    void utkeres(int startx, int starty, int celx, int cely)
    {
        set<Tile> open;
        set<Tile> closed_set;
        vector<Tile> closed_vec;


        Tile start(startx, starty);
        start.helyezes = 0;

        open.insert(start);

        tiles = vector<vector<Tile>>();
        for (int i = 0; i < SIZE; i++)
        {
            vector<Tile> uj;
            for (int j = 0; j < SIZE; j++)
            {
                Tile t(j, i);
                uj.push_back(t);
            }
            tiles.push_back(uj);
        }
        int sorrend = 0;
        while (!(open.begin()->x == celx && open.begin()->y == cely) )
        {
            Tile current = *(open.begin());
            open.erase(open.begin());
            current.helyezes = sorrend;
//            current.calculate();
            closed_set.insert(current);
            closed_vec.push_back(current);

            for (Tile neighbour: szomszedok(current))
            {
                int cost = current.g + 1;
                neighbour.calculate(celx, cely, current);

                if (open.count(neighbour) && cost < neighbour.g)
                {
                    open.erase(open.find(neighbour));
                }

                if (!open.count(neighbour) && !closed_set.count(Tile(neighbour.x, neighbour.y)))
                {
                    open.insert(neighbour);
                }
            }
            sorrend++;
            if (open.begin()->x == celx && open.begin()->y == cely)
            {
                closed_set.insert(*(open.begin()));
                closed_vec.push_back(*(open.begin()));
            }
        }

        for (int i = 0; i < closed_vec.size()-1; i++)
        {
            futoszalag_letrehoz(closed_vec[i].x, closed_vec[i].y, closed_vec[i].irany(closed_vec[i+1]));
        }
    }

    vector<Tile> szomszedok(Tile t)
    {
        vector<Tile> result;

        if (t.x > 0 && valid(Tile(t.x-1, t.y))) result.push_back(tiles[t.y][t.x-1]);
        if (t.x < SIZE-1 && valid(Tile(t.x+1, t.y))) result.push_back(tiles[t.y][t.x+1]);

        if (t.y > 0 && valid(Tile(t.x, t.y-1))) result.push_back(tiles[t.y-1][t.x]);
        if (t.y < SIZE-1 && valid(Tile(t.x, t.y+1))) result.push_back(tiles[t.y+1][t.x]);

        return result;
    }
    bool valid(Tile t)
    {
        if (mezok[t.y][t.x].get_id() == 5 && osszes_fogyaszto.count({t.x, t.y}) && osszes_fogyaszto[{t.x, t.y}].osszes_szin.size() == 0)
        return false;
        if (osszes_futoszalag.count({t.x, t.y})) return false;
        return true;
    }
};



int main()
{



    Kezelo K;

    K.utkeres(2, 3, 8, 3);
    K.megjelenit();

    cout << "----" << endl;

    K.utkeres(2, 2, 8, 2);
    K.megjelenit();

//    K.termelo_letrehoz(4, 5, 1, "R");
////    K.termelo_letrehoz(4, 3, 4, "G");
////    K.termelo_letrehoz(5, 4, 4, "B");
////    K.termelo_letrehoz(6, 7, 1, "R");
//
//    K.fogyaszto_letrehoz(2, 0, "R", 3, "B", 5);
//    K.fogyaszto_letrehoz(7, 7, "R", 3, "B", 5);
//
//    K.futoszalag_letrehoz(4, 5, 1);
//    K.futoszalag_letrehoz(4, 3, 3);
//    K.futoszalag_letrehoz(5, 4, 2);
//    K.futoszalag_letrehoz(6, 7, 4);
//    K.futoszalag_letrehoz(2, 1, 1);
//    K.futoszalag_letrehoz(2, 2, 1);
//    K.futoszalag_letrehoz(2, 3, 1);
//    K.futoszalag_letrehoz(2, 4, 1);
//    K.futoszalag_letrehoz(3, 4, 2);
//    K.futoszalag_letrehoz(4, 4, 2);
//
//
//    K.megjelenit();
//
//    cout << endl;
//    for (int i = 0; i < 10; i++)
//    {
//
//
//
//
//        K.folyamat_1();
//
//        K.folyamat_2();
//
//        K.folyamat_3();
//
//        K.folyamat_4(i);
//
//
//    K.megjelenit();
//    }


    return 0;
}
