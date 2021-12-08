#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIcon>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class Mezo
{
    int x, y;
    QString kep;
    int id;
public:

    Mezo(int x_, int y_, int id_, QString kep_)
    {
        x = x_;
        y = y_;
        id = id_;
        kep = kep_;
    }
    Mezo()
    {
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

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QVector<Mezo>> mezok;

    void shortest_path(int x1, int y1, int x2, int y2);
    void refresh();
};
#endif // MAINWINDOW_H
