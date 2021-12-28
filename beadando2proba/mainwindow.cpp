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
        mezok[t.y][t.x].szin = t.szin;
    }
    for (Fogyaszto f: osszes_fogyaszto)
    {
        mezok[f.y][f.x].szin = f.szin;
    }
    for (Futoszalag f: osszes_futoszalag)
    {
        mezok[f.y][f.x].szin = f.szin;
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
    QString filename = "C:/Datafiles/progbeadandokepek/futoszalag" + QString::number(t.irany) + ".png";

    ui->gridLayout->addWidget(k, a, b);
    QPixmap m(1, 1);
    m.load(filename);
    QPainter* painter = new QPainter(&m);
    painter->setPen(QPen(szinek[t.szin], 30, Qt::SolidLine));
    painter->drawEllipse(10, 10, 40, 40);
    painter->end();

    k->setPixmap(m.scaled(40, 40));
}

void MainWindow::draw_arrow(int a, int b, QString filename, int field_size, QString szin)
{
    QLabel *k = new QLabel();

    ui->gridLayout->addWidget(k, a, b);
    QPixmap m(1, 1);
    m.load(filename);
    QPainter* painter = new QPainter(&m);
    painter->setPen(QPen(szinek[szin], 4, Qt::SolidLine));
    painter->fillRect(10, 10, 60, 60, szinek[szin]);
    painter->end();

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
        k->setPixmap(m.scaled(szin.second, 3));

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

    QString filename = "C:/Datafiles/progbeadandokepek/futoszalag" + QString::number(uj_id) + ".png";
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

//fuggveny
//{
//    for termelo : termelok
//    {
//        if (termelo.sebesseg % hanyadik_kor = 0)
//        {
//                      az iranynak megfelelo futoszalag kap adagot
//        }
//    }
//}

void MainWindow::termelo_atad()
{
    for (int i = 0; i < osszes_termelo.size(); i++)
    {
        if (osszes_termelo[i].sebesseg % i == 0)
        {
            int x = osszes_termelo[i].x;
            int y = osszes_termelo[i].y;

            if (osszes_termelo[i].irany == 1) y-1;
            if (osszes_termelo[i].irany == 2) x-1;
            if (osszes_termelo[i].irany == 3) y+1;
            if (osszes_termelo[i].irany == 4) x+1;

            mezok[y][x].belt.szin_megkap(osszes_termelo[i].szin);
        }
    }
}






void MainWindow::on_pushButton_4_clicked()
{
    Mezo m = Mezo(6, 7, 6, "");

    m.source = Termelo(1, 2, "R", 7, 6);
    mezok[6][7] = m;
    refresh();
}

