#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Konstruktor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Pobieramy wymiary ramki
    width = ui->frame->width();
    height = ui->frame->height();
    // Pobieramy współrzędne (względem głównego okna) lewego, górnego naroznika ramki
    startX = ui->frame->x();
    startY = ui->frame->y();

    // Tworzymy obiekt klasy QImage o wymiarach równych wymiarom ramki
    // Format_RGB32 to 32 bitowe RGB (z kanałem alfa ustawionym na wartość 255)
    img = new QImage(width, height, QImage::Format_RGB32);
    //nadajemy kolor podstawowy
     red=255;
     green=0;
     blue=0;

     numberOfPixels = -1; // ilosc kliknietych pikseli
     highOfBigPixel = 4; // wielkosc kliknietego piksela
     activeRedButton = -1; // czy button czerwony byl juz klikniety // ustawione na -1
     counterOfLines = -1; // licznik krzywych beziera wielokrotnosc 4ki
}
//*****************************************************************************************************


// Destruktor
MainWindow::~MainWindow()
{
    delete ui;
}
//*****************************************************************************************************

// Funkcja ,,odrysowująca'' okno aplikacji. Wywoływana automatycznie podczas uruchamiania aplikacji
// i w innych sytuacjach, w których zachodzi konieczność ponownego narysowania okna.
// Można też wymusić jej wywołanie za pomocą funkcji update()
void MainWindow::paintEvent(QPaintEvent*)
{
    // Obiekt przy pomocy, którego możemy rysować po elementach interfejsu graficznego aplikacji
    QPainter p(this);

    // Metoda drawImage wyświetla zawartość obiektu *img klasy QImage w głównym oknie aplikacji
    // startX i startY to współrzędne (względem głównego okna) lewego górnego narożnika
    // wyświetlanego obiektu
    p.drawImage(startX, startY, *img);
}
//*****************************************************************************************************



void MainWindow::clean_screen()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *wsk;

    // przechodzimy po wszystkich wierszach obrazu
    for(int i=0; i<height; i++)
    {
        // ustawiamy wskaźnik na początku i-tego wiersza
        wsk = img->scanLine(i);

        // przechodzimy po wszystkich piselach i-tego wiersza
        // i nadajemy im wartość (0,0,0) odpowiadającą kolorowi czarnemu
        for(int j=0; j<width; j++)
        {
            wsk[4*j] = 0;   // ustawiamy składową ,,niebieską'' na 0
            wsk[4*j+1] = 0; // ustawiamy składową ,,zielonąą'' na 0
            wsk[4*j+2] = 0; // ustawiamy składową ,,czerwoną'' na 0
        }
    }
}

//*****************************************************************************************************

// zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
int MainWindow::drawPixel(int x, int y, int blue, int green, int red)
{
    unsigned char *wsk;

    // sprawdzamy czy (x,y) leży w granicach rysunku
    if(x>=0 && y>=0 && x<width && y<height)
    {
        wsk = img->scanLine(y);
        wsk[4*x] = blue;
        wsk[4*x+1] = green;
        wsk[4*x+2] = red;
    }
    return(0);
}

void MainWindow::drawBigPixel(int x,int y)
{
    x=x-5; // poczatek kwadratu x
    y=y-5; // // poczatek kwadratu y

    //rysowanie kwadradu a=5 pikseli
    for(int i=0; i<=10; i++){
        for (int j=0; j<=10; j++) {
            drawPixel(x+i,y+j, 0,0,255);
        }
    }
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
        int pointX, pointY, i;
        //pobieramy pkt X Y klikniecia
        pointX = event->x() - startX;
        pointY = event->y() - startY;

        if(event->button() == Qt::RightButton )
        {
            numberOfPixels++;  // licznik pikseli w tablicach x i y
            x[numberOfPixels] = pointX; // zapisanie wspolrzednej x do tablicy
            y[numberOfPixels] = pointY; // zapisanei y do tablicy
             //QTextStream(stdout) << "1) zapisano punkt w tablicy " << pointX <<pointY << " \n";
            // jesli liczba pikseli osiagnie wielokrotnosc 4 to zwiekszamy licznik lini
            if(numberOfPixels % 4 == 3)
            {
                counterOfLines++; // licznik lini
                numberOfPixels++; // licznik pikseli
                x[numberOfPixels] = pointX; // piksel poczatku nowej krzywej beziera
                y[numberOfPixels] = pointY; //
                 //QTextStream(stdout) << "2) zapisano punkt w tablicy " << pointX <<pointY << " \n";
            }
            refresh_and_draw_all();
        }

        //sprawdzenie czy punkt jest czerwonym buttonem
        if ( event->button() == Qt::LeftButton)
        {
            for (i=0; i<=numberOfPixels; i++)
                if (i%4 != 0 || i==0)
                    if (abs(pointX-x[i])<=highOfBigPixel && abs(pointY-y[i])<=highOfBigPixel)
                        // wartosc bezwzgledna abs - odleglosc <= highOfBigPixel
                    {
                        //zapamietanie, ktory element w tablicy jest czerwonym buttonem

                        activeRedButton=i;
                        //QTextStream(stdout) <<"i: " << i << " - 3) aktywny - zapamietano wsp buttona " << x[i] <<y[i] << " \n";
                    }

            if (activeRedButton != -1)
                refresh_and_draw_all();
        }

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
     int pointX, pointY;
    //pobieramy pkt puszczenia buttona
    pointX = event->x() - startX;
    pointY = event->y() - startY;


    if (activeRedButton != -1) // jesli kliknieto czerwony button
    {
        //podmien wspolrzedne redbuttona
        x[activeRedButton] = pointX;
        y[activeRedButton] = pointY;

        activeRedButton = -1;

        //jesli redbutton jest 4tym punktem na krzywej beziera to podmien tez +1 / doubel punktu
        if (activeRedButton % 4 == 3 )
        {
                x[activeRedButton+1]=pointX;
                y[activeRedButton+1]=pointY;
        }
        refresh_and_draw_all();
    }
        update();
}

void MainWindow::mouseMoveEvent( QMouseEvent * event )
{
        int pointX, pointY;
        //pobieramy pkt klikniecia
        pointX = event->x() - startX;
        pointY = event->y() - startY;

        //jesli redbutton jest aktywny
        if (activeRedButton != -1)
        {
            //pobieraj wsp aktywnego
            x[activeRedButton] = pointX;
            y[activeRedButton] = pointY;

            if (activeRedButton % 4 == 3 )
            {
                //pobieraj wsp. aktywnego + 1 // dla co 4 punktu
                 x[activeRedButton+1]=pointX;
                 y[activeRedButton+1]=pointY;
            }

            refresh_and_draw_all();
        }
            update();
    }

int MainWindow::refresh_and_draw_all()
{
    clean_screen();
    if (option==1){
        //przejdz po tablicy x, y
        for (int i=0; i<=numberOfPixels; i++)
        {
            // rysuj czerwone punkty 0,1,2,3 bez 4

            if (i % 4 !=0 || i==0){
                drawBigPixel(x[i],y[i]);
            }

        }

        if (numberOfPixels>=3)
        {
            for (int  i=0; i<=counterOfLines; i++)
            {
                draw_bezier(4*i, 4*i, 4*i+1, 4*i+1, 4*i+2, 4*i+2, 4*i+3, 4*i+3);
            }
        }
    }
    update();
    return(0);
}

//refresh_and_draw_alle krzywa beziera dla 4 punktow - 3go stopnia
void MainWindow::draw_bezier(int px0, int py0, int px1, int py1,int px2, int py2,int px3, int py3 )
{
    int x_1,y_1;

    //rysowanie krzywej Beizera 3go stopnia
    for (float t=0; t<=1; t+=0.001) {
        x_1=qPow((1-t),3)*x[px0]+3*(qPow((1-t),2))*t*x[px1]+3*(1-t)*qPow(t,2)*x[px2]+qPow(t,3)*x[px3];
        y_1=qPow((1-t),3)*y[py0]+3*(qPow((1-t),2))*t*y[py1]+3*(1-t)*qPow(t,2)*y[py2]+qPow(t,3)*y[py3];

        drawPixel(x_1,y_1,122,160,30);
    }
}

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaźnik na obiekt aplikacji
    // metoda quit() powoduje zakończenie działania aplikacji z kodem wyjścia 0 (sukces)
    qApp->quit();
}
//*****************************************************************************************************

// Metoda wywoływana po nacisnięciu przycisku exitButton (,,Wyjście'')
void MainWindow::on_cleanButton_clicked()
{
    clean_screen();

    numberOfPixels=-1;
    counterOfLines=-1;

    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}


void MainWindow::on_radioButton_clicked()
{
    option=1;
}





