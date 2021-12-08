#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
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

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size; i++)
        {
            if (mezok[i][j].get_id() == 0)
            {
                QLabel *k = new QLabel();

                ui->gridLayout->addWidget(k, i, j);
                QPixmap m(1, 1);
                m.fill(Qt::gray);
                k->setPixmap(m.scaled(field_size, field_size));
            } else if (mezok[i][j].get_id() < 5)
            {
                draw_arrow(i, j, mezok[i][j].get_image(), field_size);
            } else if (mezok[i][j].get_id() == 5)
            {
                draw_fogyaszto(i, j, mezok[i][j].sink);
            }
        }
    }
}
void MainWindow::draw_arrow(int a, int b, QString filename, int field_size)
{
    QLabel *k = new QLabel();

    ui->gridLayout->addWidget(k, a, b);
    QPixmap m(1, 1);
    m.load(filename);
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


    ui->gridLayout->addLayout(g, a, b);

    qDebug() << g->rowCount();
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
//    Mezo m = Mezo(10, 10, 5, "");
//    m.sink.vart_szin_hozzaad({"R", 10});
//    m.sink.vart_szin_hozzaad({"RG", 15});
//    m.sink.vart_szin_hozzaad({"B", 30});
//    mezok[10][10] = m;
//    refresh();
    qDebug() << ui->gridLayout->rowCount();
    clear_grid();
    qDebug() << ui->gridLayout->rowCount();
}

void MainWindow::clear_grid()
{
    ui->gridLayout = new QGridLayout();
}

