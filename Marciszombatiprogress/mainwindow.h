#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIcon>
#include <QLabel>
#include <map>
#include <set>
#include <QGridLayout>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class Futoszalag
{
public:

    int irany;
    QString szin;
    QString kovetkezo_szinek;
    int x, y;
    Futoszalag()
    {
        irany = 0;
        kovetkezo_szinek = "";
    }

    Futoszalag(int i, int x_, int y_, QString s)
    {
        irany = i;
        x = x_;
        y = y_;
        szin = s;
    }

    void szin_megkap(QString s2)
    {       
        kovetkezo_szinek += s2;
    }
    void keveredes()
    {
        QString uj = "";
        if(kovetkezo_szinek.contains("R")){ uj += "R";}
        if(kovetkezo_szinek.contains("G")){ uj += "G";}
        if(kovetkezo_szinek.contains("B")){ uj += "B";}
        kovetkezo_szinek = "";
        szin = uj;
//        qDebug() << kovetkezo_szinek.size();
    }
};

class Termelo
{
public:

    int sebesseg;
    int x, y;
    int irany;
    QString szin;
    QString termelt_szin;

    Termelo()
    {
        irany = 0;
    }
    Termelo(int s, int i, QString sz, int xx, int yy)
    {
        sebesseg = s;
        irany = i;
        termelt_szin = sz;
        x = xx;
        y = yy;
    }
};

class Fogyaszto
{
public:
    std::map<QString, int> osszes_szin;

    int x,y;
    QString szin;
    Fogyaszto()
    {
        osszes_szin = std::map<QString, int>();
        szin = "";
    }

    Fogyaszto(std::map<QString, int> osszes, int xx, int yy)
    {
        osszes_szin = osszes;
        x = xx;
        y = yy;
    }

    void vart_szin_hozzaad(std::pair<QString, int> uj_szin)
    {
        osszes_szin.insert(uj_szin);
    }

    void szin_bemegy(QString szin)
    {
        for (auto f: osszes_szin)
        {
            if (f.first == szin && f.second > 0)
            {
                osszes_szin[f.first]--;
            }
        }
    }

};

class Mezo
{
    int x, y;
    QString kep;
public:
    int id;
    QString szin;
    Fogyaszto sink;
    Termelo source;
    Futoszalag belt;


    Mezo(int x_, int y_, int id_, QString kep_)
    {
        x = x_;
        y = y_;
        id = id_;
        kep = kep_;
        sink = Fogyaszto();
    }
    Mezo()
    {
        id = 0;
        szin = "";
    }

    QString get_image()
    {
        return kep;
    }

    int get_id()
    {
        return id;
    }

    void clicked(QString kep_, int id_)
    {
        id = id_;
        kep = kep_;
    }
    void termelo_rahelyez(Termelo t)
    {
        id = 6;
        source = t;
        szin = t.szin;
    }
    void fogyaszto_rahelyez(Fogyaszto f)
    {
        id = 5;
        sink = f;
        szin = f.szin;
    }
    void futoszalag_rahelyez(Futoszalag f)
    {
        belt = f;
        szin = f.szin;
        QString filename =  "C:/prog2projektek/beadandomappaban/futoszalag" + QString::number(f.irany) + ".png";
        clicked(filename, f.irany);
//        id = f.irany;
    }

};


class Tile
{
public:

    int x, y, F, G, H, helyezes;

    Tile()
    {
        x = 0;
        y = 0;
        F = 0;
        G = 0;
        H = 0;
    }

    Tile(int xx, int yy)
    {
        x = xx;
        y = yy;
        helyezes = 0;
    }

    void calculate(int a, int b, Tile t, int i)
    {
        H = abs(a-x)+abs(b-y);
        G = t.G+1;
        F = G + H;
        if (helyezes == 0)
        helyezes = i;
    }


    int irany(int a, int b)
    {
        if (a == x && b-1 == y) return 3;
        if (a == x && b+1 == y) return 1;
        if (a-1 == x && b == y) return 4;
        if (a+1 == x && b == y) return 2;
        return 1;
    }



};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QVector<Mezo>> mezok;
    QVector<Termelo> osszes_termelo;
    QVector<Fogyaszto> osszes_fogyaszto;
    QVector<Futoszalag> osszes_futoszalag;
    QString fajlnev = "C:/prog2projektek/beadandomappaban/futoszalag";

    std::set<Tile> open_list;
    std::set<Tile> closed_set;
    QVector<Tile> closed_list;
    QVector<QVector<Tile>> tiles;

    int size;
    std::map<QString, QColor> szinek;

    void shortest_path(int x1, int y1, int x2, int y2);
    void refresh();
    void draw_arrow(int a, int b, QString filename, int field_size, QString szin);
    void draw_fogyaszto(int a, int b, Fogyaszto f);
    void termelo_atad();
    void draw_termelo(int a, int b, Termelo t);
    void folyamat_1();
    void folyamat_2();
    void folyamat_3();
    void folyamat_4(int i);
    void proba();
    void termelo_letrehoz(int x, int y, int seb, QString szin, int irany);
    void fogyaszto_letrehoz(int x, int y, QString szin1, int szin1_, QString szin2, int szin2_);
    void futoszalag_letrehoz(int x, int y, int irany);
    void clear_grid();
    void Astar(int x, int y, int a, int b);
    bool mehet(int x, int y);
};
#endif // MAINWINDOW_H
