#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>

using namespace std;
map<string, string> szinek =
{
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
        szin = "";
        kovetkezo_szinek = "";
    }

    Fogyaszto(int xx, int yy)
    {
        x = xx;
        y = yy;
        id = 5;
        szin = "";
        kovetkezo_szinek = "";
    }
    void szin_bemegy(string szin)
    {
        kovetkezo_szinek += szin;
    }

    void szin_elnyel()
    {
        keveredes();
        if (osszes_szin.count(szin) && osszes_szin[szin] > 0)
        {
            osszes_szin[szin]--;
        }
        szin = "";
    }

    void keveredes()
    {
        string uj = "";

        if(kovetkezo_szinek.find("R") != string::npos) uj += "R";

        if(kovetkezo_szinek.find("G") != string::npos) uj += "G";

        if(kovetkezo_szinek.find("B") != string::npos) uj += "B";

        szin = uj;
        kovetkezo_szinek = "";
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

        if(kovetkezo_szinek.find("R") != string::npos) uj += "R";

        if(kovetkezo_szinek.find("G") != string::npos) uj += "G";

        if(kovetkezo_szinek.find("B") != string::npos) uj += "B";

        szin = uj;
        kovetkezo_szinek = "";
    }

    string kiir()
    {
//        if (szin != "") return szinek[szin];
        if (irany == 1) return "^";
        if (irany == 2) return "<";
        if (irany == 3) return "v";
        if (irany == 4) return ">";
    }

};

class Coord
{
    int x, y;
    int Gcost, Hcost, Fcost;
    pair<int,int> lastCoord;
    bool checked;

    int calcFcost()
    {
        return Gcost + Hcost;
    }

    int calcHcost(pair<int,int>& target)
    {
        int tx = target.first;
        int ty = target.second;
        return abs(this->x - tx) + abs(this->y - ty);
    }

public:
    Coord(int _x, int _y, const pair<int, int>& lastCoord, pair<int,int>& target, const vector<vector<Coord>>& helperGrid)
    {
        this->x = _x;
        this->y = _y;

        this->lastCoord = lastCoord;

        int lx = lastCoord.first;
        int ly = lastCoord.second;

        this->checked = !(_x == -1 || _y == -1);

        if (lx == -1 or ly == -1)
            this->Gcost = 0;
        else
            this->Gcost = helperGrid[ly][lx].Gcost+1;
        this->Hcost = this->calcHcost(target);
        this->Fcost = this->calcFcost();
    }

    pair<int, int> get()
    {
        return {x, y};
    }

    pair<int, int> getLastCoord()
    {
        return this->lastCoord;
    }

    bool isChecked()
    {
        return this->checked;
    }

    const bool operator <(const Coord& right) const
    {
        if (this->Fcost != right.Fcost)
            return this->Fcost < right.Fcost;
        if (this->x != right.x)
            return this->x < right.x;
        return this->y != right.y;
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

    void fogyaszto_letrehoz(int x, int y, string szin1, int szin1_, string szin2, int szin2_, string szin3, int szin3_, string szin4, int szin4_, string szin5, int szin5_, string szin6, int szin6_, string szin7, int szin7_)
    {
        Fogyaszto f(x, y);
        mezok[y][x].id = 5;
        f.osszes_szin.insert({szin1, szin1_});
        f.osszes_szin.insert({szin2, szin2_});
        f.osszes_szin.insert({szin3, szin3_});
        f.osszes_szin.insert({szin4, szin4_});
        f.osszes_szin.insert({szin5, szin5_});
        f.osszes_szin.insert({szin6, szin6_});
        f.osszes_szin.insert({szin7, szin7_});
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

                if (m.get_id() > 0 && m.get_id() < 5) cout << osszes_futoszalag[ {m.x, m.y}].kiir(); //futoszalag

                if (m.get_id() == 5) cout << osszes_fogyaszto[ {m.x, m.y}].kiir(); // fogyaszto

                if (m.get_id() == 6)
                {
                    cout << osszes_termelo[ {m.x, m.y}].kiir(); // termelo

                }
            }
            cout << "\n";
        }

//        for (auto k: osszes_fogyaszto)
//        {
//            for (auto f: k.second.osszes_szin)
//            {
//                cout << f.first << " " << f.second << endl;
//            }
//        }

    }

    bool folyamat_1()
    {
        for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
        {
            it->second.szin_elnyel();
        }

        bool minden_nulla=true;

        for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
        {
            for(auto s:it->second.osszes_szin)
            {
                if(s.second!= 0)
                {
                    minden_nulla=false;
                    //cout<<s.first<<" "<<s.second<<endl;
                    break;
                }
            }
        }
        return minden_nulla;
    }

    void folyamat_2()
    {
        for (map<pair<int, int>, Futoszalag>::iterator it = osszes_futoszalag.begin(); it != osszes_futoszalag.end(); it++)
        {
            Futoszalag f = it->second;
            if (f.irany == 1)
            {
                if (osszes_futoszalag.count({f.x, f.y-1})) osszes_futoszalag[ {f.x, f.y-1}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x, f.y-1})) osszes_fogyaszto[ {f.x, f.y-1}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 2)
            {
                if (osszes_futoszalag.count({f.x-1, f.y})) osszes_futoszalag[ {f.x-1, f.y}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x-1, f.y})) osszes_fogyaszto[ {f.x-1, f.y}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 3)
            {
                if (osszes_futoszalag.count({f.x, f.y+1})) osszes_futoszalag[ {f.x, f.y+1}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x, f.y+1})) osszes_fogyaszto[ {f.x, f.y+1}].szin_bemegy(f.szin);

                f.szin = "";
            }
            if (f.irany == 4)
            {
                if (osszes_futoszalag.count({f.x+1, f.y})) osszes_futoszalag[ {f.x+1, f.y}].szin_megkap(f.szin);

                if (osszes_fogyaszto.count({f.x+1, f.y})) osszes_fogyaszto[ {f.x+1, f.y}].szin_bemegy(f.szin);

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
            }
            else t.szin = "";
            osszes_termelo[ {t.x, t.y}] = t;
        }
    }

    void termelo_atad()
    {
        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it != osszes_termelo.end(); it++)
        {
            Termelo m = it->second;
            int x = osszes_termelo[ {m.x, m.y}].x;
            int y = osszes_termelo[ {m.x, m.y}].y;

            osszes_futoszalag[ {m.x, m.y}].szin_megkap(osszes_termelo[ {m.x, m.y}].szin);
        }
    }

    void addNeighbour(int x, int y, Coord lastCoord, pair<int,int> target, set<Coord>& possibleCoords, vector<vector<Coord>>& helperGrid, vector<vector<Mezo>>& grid)
    {

        if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
            return;
        if (helperGrid[y][x].isChecked())
            return;
        if (grid[y][x].get_id() != 0 && !(x == target.first && y == target.second))
            return;

        Coord C(x, y, lastCoord.get(), target, helperGrid); ///fel kell e cserelni
        possibleCoords.insert(C);
        helperGrid[y][x] = C;
    }

/// astar
    vector<pair<int, int>> aStar(vector<vector<Mezo>>& grid, pair<int,int> source, pair<int,int> target)
    {
        vector<pair<int, int>> res;

        int sx = source.first, sy = source.second;
        int tx = target.first, ty = target.second;

        Coord emptyCoord(-1, -1, {-1, -1}, target, {});
        vector<Coord> row(SIZE, emptyCoord);
        vector<vector<Coord>> helperGrid(SIZE,row);

        set<Coord> possibleCoords;
        Coord actualCoord(sx, sy, {-1, -1}, target, helperGrid);
        possibleCoords.insert(actualCoord);
        helperGrid[sy][sx] = actualCoord;

        pair<int, int> c = actualCoord.get();
        int x = c.first;
        int y = c.second;

        while (!(x == tx && y == ty))
        {
            if (possibleCoords.empty())
                return res;

            set<Coord>::iterator it = possibleCoords.begin();
            actualCoord = *it;
            possibleCoords.erase(it);

            c = actualCoord.get();
            x = c.first;
            y = c.second;


            addNeighbour(x+1, y, actualCoord, target, possibleCoords, helperGrid, grid);
            addNeighbour(x-1, y, actualCoord, target, possibleCoords, helperGrid, grid);
            addNeighbour(x, y+1, actualCoord, target, possibleCoords, helperGrid, grid);
            addNeighbour(x, y-1, actualCoord, target, possibleCoords, helperGrid, grid);
        }

        do
        {
            res.push_back(actualCoord.get());
            c = actualCoord.getLastCoord();
            x = c.first;
            y = c.second;
            actualCoord = helperGrid[y][x];
        }
        while(!(x == sx && y == sy));

        res.push_back({source.first, source.second});

        reverse(res.begin(), res.end());

        return res;
    }

    Futoszalag conveyorBeltPart(const vector<pair<int, int>>& path, const int idx)
    {
        pair<int, int> next = path[idx+1];
        pair<int, int> actual = path[idx];

        int nx = next.first, ny = next.second;
        int ax = actual.first, ay = actual.second;

        Kezelo k;

        if (nx == ax)
        {
            if (ay < ny)
                return Futoszalag(ax, ay, 3);
            return Futoszalag(ax, ay, 1);
        }

        if (ax < nx)
            return Futoszalag(ax, ay, 4);

        return Futoszalag(ax, ay, 2);
    }



    void addPathToGrid(vector<vector<Mezo>>& grid, const vector<pair<int,int>>& path)
    {
        for (int i = 0; i < (int)path.size()-1; i++)
        {
            pair<int, int> P = path[i];
            int x = P.first;
            int y = P.second;
            osszes_futoszalag[ {x, y}] = conveyorBeltPart(path, i);
            mezok[y][x].id = conveyorBeltPart(path, i).irany;
        }
    }

    bool check()
    {
        bool lehetseges=true;
        map<string, vector<Termelo>> termelo_map;
        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it!=osszes_termelo.end(); it++)
            termelo_map[it->second.termelt_szin].push_back(it->second);

        map<char, int> minden_szin;
        for(map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
        {
            Fogyaszto f=it->second;
            map<string,int> szinek = f.osszes_szin;
            for(auto s: szinek)
            {
                if(s.second!=0){
                for(int i=0; i<s.first.length();i++){
                    char ujszin=s.first[i];
                    minden_szin[ujszin]++;
                }
                }
            }
        }
        for(auto a:minden_szin)
        {
            string ures="";
            ures+=a.first;
            if(termelo_map[ures].size()<a.second)
            {
                lehetseges=false;
            }

        }

        return lehetseges;
    }



    void kereses()
    {
        /// Minden termelo sebessege 1 eset, nincs szinkeveres
        bool minden_sebesseg_1 = true;
        bool nincs_keveres = true;
        bool lehetseges=true;

        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it!=osszes_termelo.end(); it++)
        {
            if (it->second.sebesseg != 1)
                minden_sebesseg_1 = false;
        }

        map<string, vector<Termelo>> termelo_map;

        for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it!=osszes_termelo.end(); it++)
            termelo_map[it->second.termelt_szin].push_back(it->second);
        for(map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
        {
            Fogyaszto g = it->second;
            map<string,int> szinek = g.osszes_szin;
            vector<string> milyen_szinek;
            for(auto a : szinek)
            {
                if(a.second !=0)
                {
                    milyen_szinek.push_back(a.first);
                }
            }
            for(int i=0; i< milyen_szinek.size(); i++)
            {
                if(milyen_szinek[i].length() > 1)
                {
                    nincs_keveres = false;
                }
            }
        }
        if (minden_sebesseg_1 && nincs_keveres)
        {
            vector<pair<Termelo, Fogyaszto>> parok;
            for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
            {
                for(auto ossz : it->second.osszes_szin){
                    if(termelo_map[ossz.first].size()==1 && it->second.osszes_szin[ossz.first]!=0){
                        parok.push_back({termelo_map[ossz.first][0], it->second});
                    }
                }
            }

            for (int i=0; i<parok.size(); i++)
            {
                    pair<int,int> s({parok[i].first.x, parok[i].first.y});
                    pair<int,int> t({parok[i].second.x, parok[i].second.y});
                    vector<pair<int, int>> path = aStar(mezok, s, t);
                     if (path.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                            //lehetseges=false;
                        }
                    addPathToGrid(mezok, path);

            }

            vector<pair<Termelo, Fogyaszto>> parok2;

            for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
            {
                bool volt_mar=false;
                for (int i=0; i<parok.size(); i++)
                {
                if(parok[i].second.x == it->second.x && parok[i].second.y == it->second.y)
                    volt_mar=true;
                }

                if(volt_mar)
                    continue;

               for (auto s: it->second.osszes_szin)
                {
                    if(it->second.osszes_szin[s.first]!=0)
                    {
                        int min_ut=2000;
                        Termelo min_t;
                        for(auto t:termelo_map[s.first])
                        {
                            vector<pair<int, int>> path = aStar(mezok, {t.x, t.y}, {it->second.x, it->second.y});
                            if(path.size()<min_ut)
                            {
                                min_ut=path.size();
                                min_t=t;
                            }
                        }
                        parok2.push_back({min_t,it->second});
                        break;
                    }
                }

            }

            for(int i=0; i<parok2.size(); i++)
            {


                    pair<int,int> s({parok2[i].first.x, parok2[i].first.y});
                    pair<int,int> t({parok2[i].second.x, parok2[i].second.y});
                    vector<pair<int, int>> path = aStar(mezok, s, t);
                       if (path.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                        }
                    addPathToGrid(mezok, path);
            }
        }
        /// minden sebesseg 1 de van keveres
        else if(minden_sebesseg_1 && !nincs_keveres)
        {
            vector<char>nagy_keveres;
            for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
            {
                Fogyaszto g = it->second;
                map<string,int> szinek = g.osszes_szin;
                vector<string> milyen_szinek;
                for(auto a : szinek)
                {
                    if(a.second !=0)
                        milyen_szinek.push_back(a.first);
                }

                for(int i = 0; i < milyen_szinek.size(); i++)
                {
                    vector<pair<Termelo, Fogyaszto>> aktualis_keveres;
                    vector<char> keverendo;

                    for(int j= 0; j < milyen_szinek[i].length(); j++)
                    {

                        string s  = milyen_szinek[i];
                        if(s.length()>1)
                        {
                            char s2 = s[j];
                            keverendo.push_back(s2);
                        }
                    }
                    for(int m=0; m<keverendo.size();m++){
                        nagy_keveres.push_back(keverendo[m]);
                    }
                    for(auto szin:termelo_map)
                    {
                        //cout<<szin.first<<endl;
                        char temp = szin.first[0];

                        if(find(keverendo.begin(), keverendo.end(), temp) != keverendo.end())
                        {
                            int min_tav=2000;
                            int index = 0;
                            for(int j=0; j<szin.second.size(); j++)
                            {

                                int tavolsag = abs(it->second.x - szin.second[j].x) + abs(it->second.y - szin.second[j].y);
                                if (tavolsag < min_tav)
                                {
                                    min_tav = tavolsag;
                                    index = j;
                                }
                            }
                            aktualis_keveres.push_back({szin.second[index],it->second});
                        }
                    }

                    int min_tavolsag=20000;
                    int tav;
                    int idx = 0;
                    for (int k = 0; k<aktualis_keveres.size(); k++)
                    {
                        if(aktualis_keveres[k].first.x == aktualis_keveres[k].second.x || aktualis_keveres[k].first.y == aktualis_keveres[k].second.y)
                        {
                            idx = k;
                            break;

                        }
                        else
                        {
                            tav = abs(aktualis_keveres[k].first.x-aktualis_keveres[k].second.x)+ abs(aktualis_keveres[k].first.y-aktualis_keveres[k].second.y);
                            if(tav < min_tavolsag)
                            {
                                min_tavolsag = tav;
                                idx = k;
                            }
                        }
                    }
                    pair<int,int> s({aktualis_keveres[idx].first.x, aktualis_keveres[idx].first.y});
                    pair<int,int> t({aktualis_keveres[idx].second.x, aktualis_keveres[idx].second.y});
                    vector<pair<int, int>> path = aStar(mezok, s, t);
                       if (path.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                        }
                    addPathToGrid(mezok, path);

                    vector<pair<pair<int,int>,pair<int,int>>> parok;
                    pair<int,int> p;

                    for(int l = 0; l < aktualis_keveres.size(); l++ )
                    {
                        if(l != idx)
                        {
                            int minimum_tavolsag = 20000;
                            int tavolsag;
                            int index;
                            for(int j = 1; j < path.size(); j++)
                            {
                                tavolsag = abs(aktualis_keveres[l].first.x-path[j].first) + abs(aktualis_keveres[l].first.y-path[j].second);
                                if(tavolsag < minimum_tavolsag)
                                {
                                    minimum_tavolsag = tavolsag;
                                    index = j;
                                    p.first = path[j].first;
                                    p.second = path[j].second;
                                }

                            }
                            tavolsag = abs(aktualis_keveres[l].second.x-aktualis_keveres[l].first.x) + abs(aktualis_keveres[l].second.y-aktualis_keveres[l].first.y);

                            if(tavolsag < minimum_tavolsag)
                            {
                                minimum_tavolsag = tavolsag;
                                p.first = aktualis_keveres[l].second.x;
                                p.second = aktualis_keveres[l].second.y;
                            }
                            parok.push_back({{aktualis_keveres[l].first.x,aktualis_keveres[l].first.y},p});


                        }
                    }
                    for(int k = 0; k < parok.size(); k++)
                    {
                        vector<pair<int, int>> path1 = aStar(mezok, parok[k].first, parok[k].second);
                           if (path1.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                        }
                        addPathToGrid(mezok, path1);
                    }
                }
            }
        }

        /// kulonbozo frekvenciaju, keveres nelkul
        else if(!minden_sebesseg_1 && nincs_keveres){
                            vector<pair<Termelo, Fogyaszto>> parok;
            for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
            {
                for(auto ossz : it->second.osszes_szin){
                    if(termelo_map[ossz.first].size()==1 && it->second.osszes_szin[ossz.first]!=0){
                        parok.push_back({termelo_map[ossz.first][0], it->second});
                    }
                }
            }

            for (int i=0; i<parok.size(); i++)
            {
                    pair<int,int> s({parok[i].first.x, parok[i].first.y});
                    pair<int,int> t({parok[i].second.x, parok[i].second.y});
                    vector<pair<int, int>> path = aStar(mezok, s, t);
                     if (path.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                            //lehetseges=false;
                        }
                    addPathToGrid(mezok, path);

            }

            vector<pair<Termelo, Fogyaszto>> parok2;


            for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++)
            {
                bool volt_mar=false;
                for (int i=0; i<parok.size(); i++)
                {
                if(parok[i].second.x == it->second.x && parok[i].second.y == it->second.y)
                    volt_mar=true;
                }

                if(volt_mar)
                    continue;

               for (auto s: it->second.osszes_szin)
                {
                    if(it->second.osszes_szin[s.first]!=0)
                    {
                        int min_ut=2000;
                        Termelo min_t;
                        for(auto t:termelo_map[s.first])
                        {
                            bool volt_mar2=false;
                            for (int i=0; i<parok2.size(); i++)
                            {
                            if(parok2[i].first.x == t.x && parok2[i].first.y == t.y)
                                volt_mar2=true;
                            }

                            if(volt_mar2)
                                continue;
                            vector<pair<int, int>> path = aStar(mezok, {t.x, t.y}, {it->second.x, it->second.y});
                            if(path.size()<min_ut)
                            {
                                min_ut=path.size();
                                min_t=t;
                            }

                        }

                        parok2.push_back({min_t,it->second});
                        break;
                    }
                }

            }

            for(int i=0; i<parok2.size(); i++)
            {


                    pair<int,int> s({parok2[i].first.x, parok2[i].first.y});
                    pair<int,int> t({parok2[i].second.x, parok2[i].second.y});
                    vector<pair<int, int>> path = aStar(mezok, s, t);
                       if (path.size() == 0)
                        {
                            cout << "Nem lehet osszekotni a forrast es a fogyasztot.\n";
                            exit(5);
                        }
                    addPathToGrid(mezok, path);
            }


        }
    }

    void beolvas(string fajlnev)
    {
        ifstream input(fajlnev);

        if(!input.is_open())
        {
            cerr << "Hibas fajlmegnyitas\n";
            exit(100);
        }
        string sor;
        while(getline(input, sor))
        {
            stringstream ss(sor);

            if(sor[0] == 't')
            {
                char kuka;
                ss >> kuka >> kuka;
                int x, y, sebesseg;
                ss >> x >> kuka >> y >> kuka >> sebesseg;
                string szin;
                ss >> kuka >> szin;
                termelo_letrehoz(x, y, sebesseg, szin);
            }
            else if (sor[0] == 'f')
            {
                char kuka;
                int x,y,r,b,g,rb,rg,bg,rgb;
                ss >> kuka >> kuka >> x >> kuka >> y >> kuka >> r >> kuka >> b >> kuka >> g >> kuka >> rb >> kuka >> rg >> kuka >> bg >> kuka >> rgb;
                fogyaszto_letrehoz(x, y, "R", r, "B", b, "G", g, "RB", rb, "RG", rg,"BG", bg, "RGB", rgb);
            }

        }
    }

    void kiir(int ido, string fajlnev)
    {
        ofstream output(fajlnev);

        for (int i = 0; i < mezok.size(); i++)
        {
            for (int j = 0; j < mezok[i].size(); j++)
            {
                Mezo m = mezok[i][j];

                if (m.get_id() == 0) output << ".";

                if (m.get_id() > 0 && m.get_id() < 5) output << osszes_futoszalag[ {m.x, m.y}].kiir(); //futoszalag

                if (m.get_id() == 5) output << osszes_fogyaszto[ {m.x, m.y}].kiir(); // fogyaszto

                if (m.get_id() == 6)
                {
                    output << osszes_termelo[ {m.x, m.y}].kiir(); // termelo

                }
            }
            output << "\n";
        }

        output << endl << endl;
        output << ido << " a megtoltes ideje" << endl;
        output << osszes_futoszalag.size() << " darab futoszalag szukseges" << endl;
        output.close();
    }
};

int main()
{
    Kezelo K;
    K.beolvas("frekis.txt");

    if (K.check())
    K.kereses();
    else exit(5);

//    K.megjelenit();
    bool nincs_vege=true;
    int i=0;

    while(nincs_vege && K.check())
    {
        nincs_vege=!K.folyamat_1();

        K.folyamat_2();

        K.folyamat_3();

        K.folyamat_4(i);
        i++;

    }
    i--;

    K.kiir(i, "valasz.txt");
    return 0;
}
