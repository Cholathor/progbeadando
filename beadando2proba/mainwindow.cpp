#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtMath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mezok = QVector<QVector<Mezo>>(10, QVector<Mezo>(10, Mezo(0, 0, 0, "")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);

    int size = 50;
    ui->tableWidget->setIconSize(QSize(size, size));

    for (int i = 0; i < mezok.size(); i++)
    {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setRowHeight(ui->tableWidget->rowCount()-1, size);
    }
    for (int i = 0; i < mezok[0].size(); i++)
    {

        ui->tableWidget->insertColumn(ui->tableWidget->columnCount());
        ui->tableWidget->setColumnWidth(ui->tableWidget->columnCount()-1, size);
    }
    for (int j = 0; j < mezok.size(); j++)
    {
        for (int i = 0; i < mezok[j].size(); i++)
        {
            if (mezok[i][j].get_id() != 0)
            {

                QIcon icon(mezok[i][j].get_image());
                QTableWidgetItem *icon_item = new QTableWidgetItem;

                icon_item->setIcon(icon);
                ui->tableWidget->setItem(i, j, icon_item);
            }
        }
    }
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->verticalHeader()->setVisible(false);
}

void MainWindow::on_pushButton_clicked()
{
   refresh();
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    int a = ui->spinBox->value();
    QString filename = "C:/Datafiles/progbeadandokepek/futoszalag" + QString::number(a) + ".png";
    mezok[row][column].clicked(filename, a);
    refresh();
}


void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    qDebug() << mezok[row][column].get_id();
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
//    int a = ui->spinBox->value();
    QString filename = "C:/Datafiles/progbeadandokepek/futoszalag" + QString::number(uj_id) + ".png";
    mezok[y1][x1].clicked(filename, uj_id);
    if (uj_x == x2 && uj_y == y2) return;
    shortest_path(uj_y, uj_x, y2, x2);
}

void MainWindow::on_pushButton_2_clicked()
{
//    shortest_path(1, 1, 7, 7);
    shortest_path(9, 3, 3, 7);

    refresh();
}

