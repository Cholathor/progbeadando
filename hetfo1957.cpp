#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <algorithm>
#include <ctime>

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

class Coord {
    int x, y;
    int Gcost, Hcost, Fcost;
    pair<int,int> lastCoord;
    bool checked;

    int calcFcost() {
        return Gcost + Hcost;
    }

    int calcHcost(Fogyaszto& target) {
        int tx = target.x;
        int ty = target.y;
        return abs(this->x - tx) + abs(this->y - ty);
    }

public:
    Coord(int _x, int _y, const pair<int, int>& lastCoord, Fogyaszto& target, const vector<vector<Coord>>& helperGrid) {
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

    pair<int, int> get() {
        return {x, y};
    }

    pair<int, int> getLastCoord() {
        return this->lastCoord;
    }

    bool isChecked() {
        return this->checked;
    }

    const bool operator <(const Coord& right) const {
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

    void fogyaszto_letrehoz(int x, int y, string szin1, int szin1_, string szin2, int szin2_, string szin3, int szin3_)
    {
        Fogyaszto f(x, y);
        mezok[y][x].id = 5;
        f.osszes_szin.insert({szin1, szin1_});
        f.osszes_szin.insert({szin2, szin2_});
        f.osszes_szin.insert({szin3, szin3_});
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

    void addNeighbour(int x, int y, Coord lastCoord, Fogyaszto target, set<Coord>& possibleCoords, vector<vector<Coord>>& helperGrid, vector<vector<Mezo>>& grid) {

    if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
        return;
    if (helperGrid[y][x].isChecked())
        return;
    if (grid[y][x].get_id() != 0 && !(x == target.x && y == target.y))
        return;


    Coord C(x, y, lastCoord.get(), target, helperGrid); ///fel kell e cserelni
    possibleCoords.insert(C);
    helperGrid[y][x] = C;
}

/// astar
vector<pair<int, int>> aStar(vector<vector<Mezo>>& grid, Termelo source, Fogyaszto target) {
    vector<pair<int, int>> res;

    int sx = source.x, sy = source.y;
    int tx = target.x, ty = target.y;

    Coord emptyCoord(-1, -1, {-1, -1}, target, {});
    vector<Coord> row(SIZE, emptyCoord);
    vector<vector<Coord>> helperGrid(SIZE,row);

//    char object = grid[ty][tx];
//    grid[ty][tx] = '.';

    set<Coord> possibleCoords;
    Coord actualCoord(sx, sy, {-1, -1}, target, helperGrid);
    possibleCoords.insert(actualCoord);
    helperGrid[sy][sx] = actualCoord;

    pair<int, int> c = actualCoord.get();
    int x = c.first;
    int y = c.second;

    while (!(x == tx && y == ty)) {
        if (possibleCoords.empty())
            return res;

//    cout << possibleCoords.size() << " possiblecoords" << endl;
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


//    grid[ty][tx] = object;

    do{
        res.push_back(actualCoord.get());
        c = actualCoord.getLastCoord();
        x = c.first;
        y = c.second;
        actualCoord = helperGrid[y][x];
    }while(!(x == sx && y == sy));

    res.push_back({source.x, source.y});

    reverse(res.begin(), res.end());

    return res;
}

Futoszalag conveyorBeltPart(const vector<pair<int, int>>& path, const int idx) {
    pair<int, int> next = path[idx+1];
    pair<int, int> actual = path[idx];

    int nx = next.first, ny = next.second;
    int ax = actual.first, ay = actual.second;

    Kezelo k;

    if (nx == ax) {
        if (ay < ny)
            return Futoszalag(ax, ay, 3);
          //  return 'V';
//          k.futoszalag_letrehoz(ax, ay, 3);
        //return '^';
//        k.futoszalag_letrehoz(ax, ay, 1);
        return Futoszalag(ax, ay, 1);
    }

    if (ax < nx)
        return Futoszalag(ax, ay, 4);
        //return '>';
//        k.futoszalag_letrehoz(ax, ay, 4);
    //return '<';
//    k.futoszalag_letrehoz(ax, ay, 2);

//   return k.mezok[ay][ax];
    return Futoszalag(ax, ay, 2);
}


/* Lerakja a fut??szalagot a t??rk??pre
 * grid: a t??rk??p
 * path: az ??tvonal a forr??st??l a fogyaszt??ig (a forr??s ??s a fogyaszt?? is benne van)
 */
void addPathToGrid(vector<vector<Mezo>>& grid, const vector<pair<int,int>>& path) {
    for (int i = 1; i < (int)path.size()-1; i++) {
        pair<int, int> P = path[i];
        int x = P.first;
        int y = P.second;
        osszes_futoszalag[{x, y}] = conveyorBeltPart(path, i);
        mezok[y][x].id = conveyorBeltPart(path, i).irany;
    }
}


    void kereses(){
        /// Minden termelo sebessege 1 eset, nincs szinkeveres
        bool minden_sebesseg_1 = true;
        bool nincs_keveres = true;
         for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it!=osszes_termelo.end(); it++){
            if (it->second.sebesseg != 1)
                minden_sebesseg_1 = false;
         }

         map<string, vector<Termelo>> termelo_map;
         vector<pair<Termelo, Fogyaszto>> parok;


         for (map<pair<int, int>, Termelo>::iterator it = osszes_termelo.begin(); it!=osszes_termelo.end(); it++)
            termelo_map[it->second.termelt_szin].push_back(it->second);
        for (auto szin :  termelo_map){
            if (szin.first.length() > 1)
                nincs_keveres =  false;
        }
         if (minden_sebesseg_1 && nincs_keveres){
             for (map<pair<int, int>, Fogyaszto>::iterator it = osszes_fogyaszto.begin(); it!=osszes_fogyaszto.end(); it++){
                     for(auto szin:termelo_map){
                if (it->second.osszes_szin.find(szin.first) != it->second.osszes_szin.end()){
                             int min_tav=2000;
                             int index = 0;
                    for(int j=0;j<szin.second.size();j++){
                        int tavolsag = abs(it->second.x - szin.second[j].x) + abs(it->second.y - szin.second[j].y);
                            if (tavolsag< min_tav){
                                min_tav = tavolsag;
                                index = j;
                            }
                    }
                    parok.push_back({szin.second[index],it->second});
                    }
                }
            }
           for (int i = 0; i<parok.size(); i++){
             ///   path(parok[i].first, parok[i].second);
                // a path lenne egy a* kereses kirajzolasa
                // megj. tekintsuk falnak a mar letett futoszalagokat
                vector<pair<int, int>> path = aStar(mezok, parok[i].first, parok[i].second);
                addPathToGrid(mezok, path);

                if (path.size() == 0)
                    cout << "Nem lehet ??sszek??tni a forr??st ??s a fogyaszt??t. ????'(\n"<<endl;


           }


    }
    /// minden sebesseg 1 de van keveres
//    else if(minden_sebesseg_1 && nincs_keveres=false){
//
//    }
}


};

int main()
{

    Kezelo K;

    K.termelo_letrehoz(4, 1, 1, "R");
    K.termelo_letrehoz(4, 3, 1, "G");
    K.termelo_letrehoz(4, 5, 1, "B");
//    K.termelo_letrehoz(4, 3, 4, "G");
//    K.termelo_letrehoz(5, 4, 4, "B");
//    K.termelo_letrehoz(6, 7, 1, "R");

    K.fogyaszto_letrehoz(8, 3, "R", 4, "B", 4, "G", 3);

//    K.aStar(K.mezok, K.osszes_termelo[{4, 1}], K.osszes_fogyaszto[{8, 3}]).size();

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
    K.kereses();
    K.megjelenit();

    cout << endl;
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
