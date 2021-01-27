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

    void on_radioButton_clicked();



private:
    // ui to wskaźnik za pomocą którego mamy dostęp
    // do elementów interfejsu użytkownika
    Ui::MainWindow *ui;

    QImage *img; 
    int option =1;
    int width, height, startX, startY;
    int x0, y0;
    int red,green,blue;
    int numberOfPixels;
    int highOfBigPixel;
    int activeRedButton;

    int counterOfLines;

    int x[100];  // wspolrzedne wszsytkic wsp x kwadratow kliknietych
    int y[100];  // wspolrzedne wszytkich wsp y, kwadratow kliknietych

    void clean_screen();
    int  drawPixel(int x0, int y0, int blue, int green, int red);
    void drawBigPixel(int x,int y);
    void draw_bezier(int px0, int py0, int px1, int py1,int px2, int py2,int px3, int py3);
    int refresh_and_draw_all();
};

#endif // MAINWINDOW_H
