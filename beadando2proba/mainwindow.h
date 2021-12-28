#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIcon>
#include <QLabel>
#include <map>
#include <set>
#include <QGridLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class Futoszalag
{
public:

    int irany;
    QString szin;
    int x, y;
    Futoszalag()
    {
        irany = 0;
    }

    Futoszalag(int i, int x_, int y_, QString s)
    {
        irany = i;
        x = x_;
        y = y_;
        szin = s;
    }

    void szin_megkap(QString s)
    {
        szin = s;
    }
};

class Termelo
{
public:

    int sebesseg;
    int x, y;
    int irany;
    QString szin;

    Termelo()
    {
        irany = 0;
    }
    Termelo(int s, int i, QString sz, int xx, int yy)
    {
        sebesseg = s;
        irany = i;
        szin = sz;
        int x = xx;
        int y = yy;
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
    int id;
public:
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

private:
    Ui::MainWindow *ui;
    QVector<QVector<Mezo>> mezok;
    QVector<Termelo> osszes_termelo;
    QVector<Fogyaszto> osszes_fogyaszto;
    QVector<Futoszalag> osszes_futoszalag;

    int size;
    std::map<QString, QColor> szinek;

    void shortest_path(int x1, int y1, int x2, int y2);
    void refresh();
    void draw_arrow(int a, int b, QString filename, int field_size, QString szin);
    void draw_fogyaszto(int a, int b, Fogyaszto f);
    void termelo_atad();
    void draw_termelo(int a, int b, Termelo t);

};
#endif // MAINWINDOW_H
