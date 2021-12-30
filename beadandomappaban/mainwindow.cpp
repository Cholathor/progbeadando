#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    size = 11;
    mezok = QVector<QVector<Mezo>>(size, QVector<Mezo>(size, Mezo(0, 0, 0, "")));
    szinek = {
        {"R", QColor(Qt::red)},
        {"G", QColor(Qt::green)},
        {"B", QColor(Qt::blue)},
        {"RG", QColor(Qt::yellow)},
        {"RB", QColor(Qt::darkMagenta)},
        {"BG", QColor(Qt::cyan)},
        {"RGB", QColor(Qt::white)}
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    int field_size = 40;


    for (Termelo t: osszes_termelo)
    {
        mezok[t.y][t.x].termelo_rahelyez(t);
    }
    for (Fogyaszto f: osszes_fogyaszto)
    {
        mezok[f.y][f.x].fogyaszto_rahelyez(f);
    }
    for (Futoszalag f: osszes_futoszalag)
    {
        mezok[f.y][f.x].futoszalag_rahelyez(f);
    }

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size; i++)
        {

            if (mezok[i][j].get_id() == 0) //ures
            {
                QLabel *k = new QLabel();

                ui->gridLayout->addWidget(k, i, j);
                QPixmap m(1, 1);
                m.fill(Qt::gray);
                k->setPixmap(m.scaled(field_size, field_size));
            } else if (mezok[i][j].get_id() < 5) //futoszalag
            {
                draw_arrow(i, j, mezok[i][j].get_image(), field_size, mezok[i][j].belt.szin);
            } else if (mezok[i][j].get_id() == 5) //fogyaszto
            {
                draw_fogyaszto(i, j, mezok[i][j].sink);
            } else if (mezok[i][j].get_id() == 6)
            {
                draw_termelo(i, j, mezok[i][j].source);
            }
        }
    }

}

void MainWindow::draw_termelo(int a, int b, Termelo t)
{
    QLabel *k = new QLabel();
    QString filename = "C:/Users/user/Downloads/progbeadando/futoszalag" + QString::number(t.irany) + ".png";

    ui->gridLayout->addWidget(k, a, b);
    QPixmap m(1, 1);
    m.load(filename);
    QPainter* painter = new QPainter(&m);
    painter->setPen(QPen(szinek[t.termelt_szin], 30, Qt::SolidLine));
    painter->drawEllipse(50, 50, 40, 40);
    painter->end();

    if (t.szin != "")
    {
        QPainter* painter = new QPainter(&m);
        painter->setPen(QPen(szinek[t.szin], 4, Qt::SolidLine));
        painter->fillRect(10, 10, 60, 60, szinek[t.szin]);
        painter->end();
    }

    k->setPixmap(m.scaled(40, 40));
}

void MainWindow::folyamat_1()
{
    for(int i = 0; i < osszes_fogyaszto.size();i++){
        if(osszes_fogyaszto[i].osszes_szin[osszes_fogyaszto[i].szin] > 0)
        {
            osszes_fogyaszto[i].osszes_szin[osszes_fogyaszto[i].szin]--;
        }
        osszes_fogyaszto[i].szin = "";
        mezok[osszes_fogyaszto[i].y][osszes_fogyaszto[i].x].sink = osszes_fogyaszto[i];
    }
}

void MainWindow::folyamat_2()
{
    for(int j =0; j < osszes_futoszalag.size(); j++){
        if(osszes_futoszalag[j].irany == 1){
            mezok[osszes_futoszalag[j].y-1][osszes_futoszalag[j].x].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y-1][osszes_futoszalag[j].x].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
        if(osszes_futoszalag[j].irany == 2){
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x-1].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y-1][osszes_futoszalag[j].x].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
        if(osszes_futoszalag[j].irany == 3){
            mezok[osszes_futoszalag[j].y+1][osszes_futoszalag[j].x].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y-1][osszes_futoszalag[j].x].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
        if(osszes_futoszalag[j].irany == 4){
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x+1].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y-1][osszes_futoszalag[j].x].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
    }
    osszes_futoszalag = QVector<Futoszalag>();
    for(int i = 0; i < mezok.size(); i++)
    {
        for(int j = 0; j < mezok[i].size(); j++)
        {
            if(mezok[i][j].get_id() == 1 || mezok[i][j].get_id() == 2 || mezok[i][j].get_id() == 3 || mezok[i][j].get_id() == 4)
            {
                osszes_futoszalag.push_back(mezok[i][j].belt);
                qDebug() << mezok[i][j].belt.szin;
            }
        }
    }
    osszes_fogyaszto = QVector<Fogyaszto>();
    for(int i = 0; i < mezok.size(); i++)
    {
        for(int j = 0; j < mezok[i].size(); j++)
        {
            if(mezok[i][j].get_id() == 5)
            {
                osszes_fogyaszto.push_back(mezok[i][j].sink);
            }
        }
    }
    termelo_atad();

}

void MainWindow::folyamat_3()
{
    for(int i = 0; i < osszes_futoszalag.size(); i++){
        osszes_futoszalag[i].keveredes();

        mezok[osszes_futoszalag[i].y][osszes_futoszalag[i].x].belt = osszes_futoszalag[i];
    }

}

void MainWindow::folyamat_4(int i)
{
    for (int j = 0; j < osszes_termelo.size(); j++)
    {
        if (i % osszes_termelo[j].sebesseg == 0)
        {
            osszes_termelo[j].szin = osszes_termelo[j].termelt_szin;
        } else osszes_termelo[j].szin = "";
    }

}
void MainWindow::termelo_atad()
{
    for (int i = 0; i < osszes_termelo.size(); i++)
    {
        int x = osszes_termelo[i].x;
        int y = osszes_termelo[i].y;

        if (osszes_termelo[i].irany == 1) y--;
        if (osszes_termelo[i].irany == 2) x--;
        if (osszes_termelo[i].irany == 3) y++;
        if (osszes_termelo[i].irany == 4) x++;

        mezok[y][x].belt.szin_megkap(osszes_termelo[i].szin);

    }

    osszes_futoszalag = QVector<Futoszalag>();
    for(int i = 0; i < mezok.size(); i++)
    {
        for(int j = 0; j < mezok[i].size(); j++)
        {
            if(mezok[i][j].get_id() == 1 || mezok[i][j].get_id() == 2 || mezok[i][j].get_id() == 3 || mezok[i][j].get_id() == 4)
            {
                osszes_futoszalag.push_back(mezok[i][j].belt);
            }
        }
    }
}
void MainWindow::draw_arrow(int a, int b, QString filename, int field_size, QString szin)
{
    QLabel *k = new QLabel();

    ui->gridLayout->addWidget(k, a, b);
    QPixmap m(1, 1);
    m.load(filename);
    if (szin != "")
    {
        QPainter* painter = new QPainter(&m);
        painter->setPen(QPen(szinek[szin], 4, Qt::SolidLine));
        painter->fillRect(10, 10, 60, 60, szinek[szin]);
        painter->end();
    }

    k->setPixmap(m.scaled(field_size, field_size));
}


void MainWindow::draw_fogyaszto(int a, int b, Fogyaszto f)
{
    QGridLayout *g = new QGridLayout();
    int i = 0;
    for (auto szin: f.osszes_szin)
    {
        QLabel *k = new QLabel();
        g->addWidget(k, i, 0);

        QPixmap m (1, 1);
        m.fill(szinek[szin.first]);
        k->setPixmap(m.scaled(szin.second*7, 3));
//        qDebug() << szin.first << " " << szin.second;
        i++;
    }

    ui->gridLayout->removeItem(ui->gridLayout->itemAtPosition(a, b));
    ui->gridLayout->addLayout(g, a, b);

}

void MainWindow::on_pushButton_clicked()
{

   refresh();
}

double dist(int y1, int x1, int y2, int x2)
{
    return ((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}

void MainWindow::shortest_path(int y1, int x1, int y2, int x2)
{
    if (y1 == y2 && x1 == x2) return;
    int uj_x, uj_y;

    double distance = mezok.size()*mezok.size()*mezok.size()*mezok.size();
    int uj_id;

    if (dist(y1-1, x1, y2, x2) < distance)
    {
        distance = dist(y1-1, x1, y2, x2);
        uj_y = y1-1;
        uj_x = x1;
        uj_id = 1;
    }
    if (dist(y1+1, x1, y2, x2) < distance)
    {
        distance = dist(y1+1, x1, y2, x2);
        uj_y = y1+1;
        uj_x = x1;
        uj_id = 3;
    }
    if (dist(y1, x1-1, y2, x2) < distance)
    {
        distance = dist(y1, x1-1, y2, x2);
        uj_y = y1;
        uj_x = x1-1;
        uj_id = 2;
    }
    if (dist(y1, x1+1, y2, x2) < distance)
    {
        distance = dist(y1, x1+1, y2, x2);
        uj_y = y1;
        uj_x = x1+1;
        uj_id = 4;
    }

    QString filename = "C:/Users/user/Downloads/progbeadando/futoszalag" + QString::number(uj_id) + ".png";
    mezok[y1][x1].clicked(filename, uj_id);
//    mezok[y1][x1].belt.szin = "RGB";
    if (uj_x == x2 && uj_y == y2) return;
    shortest_path(uj_y, uj_x, y2, x2);
}

void MainWindow::on_pushButton_2_clicked()
{
    shortest_path(9, 3, 3, 7);

    refresh();
}

void MainWindow::on_pushButton_3_clicked()
{
    Mezo m = Mezo(10, 10, 5, "");

    m.sink.x = 10;
    m.sink.y = 10;
    m.sink.vart_szin_hozzaad({"R", 10});
    m.sink.vart_szin_hozzaad({"RG", 15});
    m.sink.vart_szin_hozzaad({"B", 30});
    m.sink.vart_szin_hozzaad({"RGB", 20});

    mezok[10][10] = m;
    refresh();
}

void MainWindow::on_pushButton_4_clicked()
{
    Mezo m = Mezo(6, 7, 6, "");

    m.source = Termelo(1, 2, "R", 7, 6);
    mezok[6][7] = m;
    refresh();
}



void MainWindow::termelo_letrehoz(int x, int y, int seb, QString szin, int irany)
{
    Termelo t(seb, irany, szin, x, y);

    mezok[y][x].termelo_rahelyez(t);
    osszes_termelo.push_back(t);

 }

void MainWindow::fogyaszto_letrehoz(int x, int y, QString szin1, int szin1_, QString szin2, int szin2_)
{
    Fogyaszto f;
    f.osszes_szin.insert({szin1, szin1_});
    f.osszes_szin.insert({szin2, szin2_});
    f.x = x;
    f.y = y;

    mezok[y][x].fogyaszto_rahelyez(f);
    osszes_fogyaszto.push_back(f);
}

void MainWindow::futoszalag_letrehoz(int x, int y, int irany)
{
    Futoszalag f(irany, x, y, "");

    mezok[y][x].futoszalag_rahelyez(f);
    osszes_futoszalag.push_back(f);
}

void MainWindow::clear_grid()
{
    for (int i = 0; i < ui->gridLayout->rowCount(); i++)
    {
        for (int j = 0; j < ui->gridLayout->columnCount(); j++)
        {

        }
    }
}

void MainWindow::proba()
{
    termelo_letrehoz(4, 5, 4, "R", 1);
    termelo_letrehoz(4, 3, 4, "B", 3);
    termelo_letrehoz(5, 4, 4, "G", 2);
    fogyaszto_letrehoz(2, 0, "R", 3, "B", 5);
    futoszalag_letrehoz(2, 1, 1);
    futoszalag_letrehoz(2, 2, 1);
    futoszalag_letrehoz(2, 3, 1);
    futoszalag_letrehoz(2, 4, 1);
    futoszalag_letrehoz(3, 4, 2);
    futoszalag_letrehoz(4, 4, 2);


    for (int i = 0; i < 10; i++)
    {
            folyamat_1();
            folyamat_2();
            folyamat_3();
            folyamat_4(i);
    }

    refresh();
}

void MainWindow::on_pushButton_5_clicked()
{
    proba();
}



void MainWindow::on_pushButton_6_clicked()
{
    std::map<QString, QVector<Termelo>> termelo_map;
    QVector<std::pair<Termelo, Fogyaszto>> parok;
    for(int i=0; i<osszes_termelo.size(); i++){
        termelo_map[osszes_termelo[i].termelt_szin].push_back(osszes_termelo[i]);
    }
    for(int i=0; i<osszes_fogyaszto.size();i++){

        for(auto szin:termelo_map){

            if(osszes_fogyaszto[i].osszes_szin.find(szin.first)!=osszes_fogyaszto[i].osszes_szin.end()){
                int min_tav=2000;
               int index = 0;
                for(int j=0;j<szin.second.size();j++){
                    int tavolsag = abs(osszes_fogyaszto[i].x - szin.second[j].x) + abs(osszes_fogyaszto[i].y - szin.second[j].y);
                    if (tavolsag< min_tav){
                        min_tav = tavolsag;
                        index = j;
                    }

                }
                parok.push_back({szin.second[index],osszes_fogyaszto[i]});
            }

        }
    }

}

