#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
#include <QPainter>
#include <QTime>
#include <set>
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
void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(250);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
//        qDebug() << f.osszes_szin["R"] << " piros";
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
                draw_arrow(j, i, mezok[i][j].get_image(), field_size, mezok[i][j].belt.szin);
            } else if (mezok[i][j].get_id() == 5) //fogyaszto
            {
                draw_fogyaszto(j, i, mezok[i][j].sink);
//                qDebug() << mezok[i][j].sink.osszes_szin["R"] << "askdaslkm";
            } else if (mezok[i][j].get_id() == 6)
            {
                draw_termelo(j, i, mezok[i][j].source);
            }
        }
    }

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
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x-1].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
        if(osszes_futoszalag[j].irany == 3){
            mezok[osszes_futoszalag[j].y+1][osszes_futoszalag[j].x].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y+1][osszes_futoszalag[j].x].sink.szin_bemegy(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x].belt.szin = "";
        }
        if(osszes_futoszalag[j].irany == 4){
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x+1].belt.szin_megkap(osszes_futoszalag[j].szin);
            mezok[osszes_futoszalag[j].y][osszes_futoszalag[j].x+1].sink.szin_bemegy(osszes_futoszalag[j].szin);
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

void MainWindow::draw_termelo(int a, int b, Termelo t)
{
    QLabel *k = new QLabel();
    QString filename = "C:/prog2projektek/beadandomappaban/futoszalag" + QString::number(t.irany) + ".png";

    ui->gridLayout->addWidget(k, b, a);
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

void MainWindow::draw_arrow(int a, int b, QString filename, int field_size, QString szin)
{
    QLabel *k = new QLabel();

    ui->gridLayout->addWidget(k, b, a);

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
        i++;
        qDebug() << szin.second << " " << szin.first << " " << a << " " << b ;
    }
//    ui->gridLayout->removeItem(ui->gridLayout->itemAtPosition(b, a));
    ui->gridLayout->addLayout(g, b, a);
    qDebug() << ui->gridLayout->rowCount() << " " << ui->gridLayout->columnCount() << " sor, oszlop";

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

//    refresh();
//    delay();
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
//    termelo_letrehoz(4, 3, 4, "B", 3);
//    termelo_letrehoz(5, 4, 4, "G", 2);
    futoszalag_letrehoz(4, 5, 1);
    futoszalag_letrehoz(4, 3, 3);
    futoszalag_letrehoz(5, 4, 2);
    fogyaszto_letrehoz(2, 0, "R", 3, "B", 5);
    fogyaszto_letrehoz(7, 7, "R", 3, "B", 5);
    termelo_letrehoz(6, 7, 1, "R", 4);
    futoszalag_letrehoz(6, 7, 4);
    futoszalag_letrehoz(2, 1, 1);
    futoszalag_letrehoz(2, 2, 1);
    futoszalag_letrehoz(2, 3, 1);
    futoszalag_letrehoz(2, 4, 1);
    futoszalag_letrehoz(3, 4, 2);
    futoszalag_letrehoz(4, 4, 2);
//    termelo_letrehoz(1, 1, 2, "R", 4);
//    futoszalag_letrehoz(1, 1, 4);

//    termelo_letrehoz(3, 1, 1, "B", 3);
//    futoszalag_letrehoz(3, 1, 3);

//    termelo_letrehoz(5, 1, 2, "G", 2);
//    futoszalag_letrehoz(5, 1, 2);

//    termelo_letrehoz(7, 1, 1, "R", 3);
//    futoszalag_letrehoz(7, 1, 3);

//    termelo_letrehoz(9, 1, 2, "B", 3);
//    futoszalag_letrehoz(9, 1, 3);

//    fogyaszto_letrehoz(3, 7, "RGB", 5, "RB", 4);
//    osszes_fogyaszto[0].vart_szin_hozzaad({"R", 4});
//    futoszalag_letrehoz(2, 1, 3);
//    futoszalag_letrehoz(2, 2, 3);
//    futoszalag_letrehoz(2, 3, 3);
//    futoszalag_letrehoz(2, 4, 3);
//    futoszalag_letrehoz(2, 5, 3);
//    futoszalag_letrehoz(2, 6, 3);
//    futoszalag_letrehoz(2, 7, 4);
//    futoszalag_letrehoz(3, 2, 2);
//    futoszalag_letrehoz(4, 2, 2);
//    futoszalag_letrehoz(3, 1, 3);
//    futoszalag_letrehoz(3, 6, 3);
//    futoszalag_letrehoz(4, 6, 2);
//    futoszalag_letrehoz(5, 6, 2);
//    futoszalag_letrehoz(6, 6, 2);
//    futoszalag_letrehoz(7, 6, 2);
//    futoszalag_letrehoz(7, 5, 3);
//    futoszalag_letrehoz(7, 4, 3);
//    futoszalag_letrehoz(7, 3, 3);
//    futoszalag_letrehoz(7, 2, 3);
//    futoszalag_letrehoz(8, 2, 2);
//    futoszalag_letrehoz(9, 2, 2);

    for (int i = 0; i < 30; i++)
    {
            folyamat_1();
            folyamat_2();
            folyamat_3();
            folyamat_4(i);
            refresh();
            delay();
            qDebug() << osszes_fogyaszto[0].osszes_szin["R"];
    }

}

void MainWindow::on_pushButton_5_clicked()
{
    proba();

////    fogyaszto_letrehoz(3, 7, "R", 1, "G", 2);
//////    osszes_fogyaszto[0].osszes_szin = std::map<QString, int>();
////    refresh();
//    Astar(2, 7, 5, 2);
//    Astar(2, 5, 8, 9);
//    Astar(4, 3, 3, 6);

//    qDebug() << osszes_futoszalag.size();

//    futoszalag_letrehoz(3, 7, 1);
//    qDebug() << osszes_futoszalag.size();
//    qDebug() << osszes_futoszalag[0].x;
//    refresh();
    qDebug() <<"finished";
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
bool operator< (const Tile t1, const Tile t2)
{
    if (t1.F < t2.F) return true;
    else if (t1.F == t2.F && t1.helyezes != t2.helyezes) return t1.helyezes > t2.helyezes;
    else if (t1.helyezes == t2.helyezes) return t1.x > t2.x;
    else return false;
}
bool operator== (const Tile t1, const Tile t2)
{
    if (t1.x == t2.x && t1.x == t2.x && t1.F == t2.F && t1.G == t2.G && t1.H == t2.H && t1.helyezes == t2.helyezes) return true;
    else return false;
}

void MainWindow::Astar(int x, int y, int a, int b)
{

    closed_list = QVector<Tile>();
    open_list = std::set<Tile>();
    closed_set = std::set<Tile>();

    Tile t(x, y);
    tiles = QVector<QVector<Tile>>();
    for (int i = 0; i < size; i++)
    {
        QVector<Tile> uj;
        for (int j = 0; j < size; j++)
        {
            uj.push_back(Tile(j, i));
        }
        tiles.push_back(uj);
    }
    bool siker = false;

    Tile h;
    h.G = -1;
    t.calculate(a, b, h, 0);

    tiles[y][x] = t;

    open_list.insert(t);

    if (mehet(t.x, t.y-1))
    {
        tiles[t.y-1][t.x].calculate(a, b, t, 0);
        open_list.insert(tiles[t.y-1][t.x]);
    }
    if (mehet(t.x, t.y+1))
    {
        tiles[t.y+1][t.x].calculate(a, b, t, 0);
        open_list.insert(tiles[t.y+1][t.x]);
    }
    if (mehet(t.x-1, t.y))
    {
        tiles[t.y][t.x-1].calculate(a, b, t, 0);
        open_list.insert(tiles[t.y][t.x-1]);
    }
    if (mehet(t.x+1, t.y))
    {
        tiles[t.y][t.x+1].calculate(a, b, t, 0);
        open_list.insert(tiles[t.y][t.x+1]);
    }
    int sorrend = 1;
    while(!siker)
    {

            Tile vizsgalt = *(open_list.begin());
            open_list.erase(open_list.begin());
            closed_list.push_back(vizsgalt);
            if (mehet(vizsgalt.x, vizsgalt.y-1))
            {
                tiles[vizsgalt.y-1][vizsgalt.x].calculate(a, b, vizsgalt, sorrend);
                open_list.insert(tiles[vizsgalt.y-1][vizsgalt.x]);
            }
            if (mehet(vizsgalt.x, vizsgalt.y+1))
            {
                tiles[vizsgalt.y+1][vizsgalt.x].calculate(a, b, vizsgalt, sorrend);
                open_list.insert(tiles[vizsgalt.y+1][vizsgalt.x]);
            }
            if (mehet(vizsgalt.x-1, vizsgalt.y))
            {
                tiles[vizsgalt.y][vizsgalt.x-1].calculate(a, b, vizsgalt, sorrend);
                open_list.insert(tiles[vizsgalt.y][vizsgalt.x-1]);
            }
            if (mehet(vizsgalt.x+1, vizsgalt.y))
            {
                tiles[vizsgalt.y][vizsgalt.x+1].calculate(a, b, vizsgalt, sorrend);
//                if (tiles[vizsgalt.y][vizsgalt.x+1])
                open_list.insert(tiles[vizsgalt.y][vizsgalt.x+1]);
            }



//            Tile uj = *(open_list.begin());
//            futoszalag_letrehoz(uj.x, uj.y, 3);
//            refresh();
//            delay();
//            qDebug() << "uj" << uj.x << uj.y;
//            qDebug() << uj.x << " " << uj.y;

//            if (!closed_set.count(uj))
//            {
//                closed_set.insert(uj);
//                closed_list.push_back(uj);
//            }

//            if (uj.x == a && uj.y == b)
//            {
//                siker = true;
//            }
//            open_list.erase(open_list.begin());

            sorrend++;
//            qDebug() << closed_list.size();
//            qDebug() << "open:";
//            for (auto k: open_list)
//            {
//                qDebug() << k.x << " " << k.y << " " << k.F << " " << k.G << " " << k.H << " " << k.helyezes;
//                draw_arrow(k.x, k.y, "C:/prog2projektek/beadandomappaban/futoszalag1.png", 40, "R");
//            }
//            for (auto k: closed_list)
//            {

//                qDebug() << k.x << " " << k.y << " " << k.F << " " << k.G << " " << k.H << " " << k.helyezes;
//                draw_arrow(k.x, k.y, "C:/prog2projektek/beadandomappaban/futoszalag1.png", 40, "R");
//            }

//            qDebug() << "end";



    }
    futoszalag_letrehoz(x, y, t.irany(closed_list[0].x, closed_list[0].y));
    for (int i = 0; i < closed_list.size()-1; i++)
    {
        futoszalag_letrehoz(closed_list[i].x, closed_list[i].y, closed_list[i].irany(closed_list[i+1].x, closed_list[i+1].y));

        refresh();
        delay();
        qDebug() << closed_list[i].x << " " << closed_list[i].y;
    }
    refresh();
}

bool MainWindow::mehet(int x, int y)
{
    if (x < 0 || x >= size || y < 0 || y >= size) return false;
    if (mezok[y][x].get_id() == 5 && mezok[y][x].sink.osszes_szin.size() == 0) return false;
    if (closed_set.find(tiles[y][x]) != closed_set.end()) return false;
    if (mezok[y][x].get_id() == 1 || mezok[y][x].get_id() == 2 || mezok[y][x].get_id() == 3 || mezok[y][x].get_id() == 4) return false;

    return true;
}







