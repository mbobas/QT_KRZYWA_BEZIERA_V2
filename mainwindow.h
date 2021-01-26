#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include<QDebug>
#include <stdio.h>
#include <stdlib.h>

namespace Ui {
class MainWindow;
}

// Deklaracja głównej klasy naszego programu
// Jest ona podklasą klasy QMainWindow
// standardowej klasy Qt reprezentującej główne okno aplikacji
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// Sloty to specjalne funkcje (metody) wywoływane
// po wystąpieniu pewnych zdarzeń np. po kliknięciu przycisku
private slots:
    void on_exitButton_clicked();
    void on_cleanButton_clicked();

    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent( QMouseEvent * event );

    void on_XChanged(int arg1);
    void on_YChanged(int arg1);
    void on_X1Changed(int arg1);
    void on_Y1Changed(int arg1);
    void on_X2Changed(int arg1);
    void on_Y2Changed(int arg1);
    void on_X3Changed(int arg1);
    void on_Y3Changed(int arg1);

    //bool check(int x0, int y0, int x1, int y1);

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_6_clicked();

    void on_radioButton_5_clicked();


    void on_styczne_clicked();

private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    QImage *img; 
    int width, height, startX, startY,x0, y0, x1, y1,r,g,b,option;

    int x[100];  // wspolrzedne wszsytkic wsp x kwadratow kliknietych
    int y[100];  // wspolrzedne wszytkich wsp y, kwadratow kliknietych


    int ile_punktow;
    int ktory_punkt;
    int grubosc;
    int aktywny;
    int czy_styczne;
    int czy_punkty;
    int segment;


    void clean();
    void drawPixel(int x, int y);
    void draw_section(int x0, int y0, int x1, int y1);
    void draw_bezier(int px0, int py0, int px1, int py1,int px2, int py2,int px3, int py3);
    //void draw_bezier();
    void drawBigPixel(int x,int y);
    void drawBigPixel_or_move(int x,int y);
    int wstaw_piksel(int x0, int y0, int b, int g, int r);
    int wstaw_punkt(int x0, int y0, int grubosc, int b, int g, int r);
    int rysuj();
};

#endif // MAINWINDOW_H
