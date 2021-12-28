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
class Fogyaszto
{
public:
    std::map<QString, int> osszes_szin;

    Fogyaszto(std::map<QString, int> osszes)
    {
        osszes_szin = osszes;
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

    Fogyaszto()
    {
        osszes_szin = std::map<QString, int>();
    }
};

class Mezo
{
    int x, y;
    QString kep;
    int id;
public:
    Fogyaszto sink;

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
        sink = Fogyaszto();
        id = 0;
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

private:
    Ui::MainWindow *ui;
    QVector<QVector<Mezo>> mezok;
    int size;
    std::map<QString, QColor> szinek;

    void shortest_path(int x1, int y1, int x2, int y2);
    void refresh();
    void draw_arrow(int a, int b, QString filename, int field_size);
    void draw_fogyaszto(int a, int b, Fogyaszto f);

};
#endif // MAINWINDOW_H
