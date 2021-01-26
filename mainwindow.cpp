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


     r=255;
     g=0;
     b=0;

     ile_punktow =0;
     ktory_punkt = -1;
     grubosc = 4;
     aktywny = -1;
     czy_styczne = 0;
     czy_punkty = 1;
     segment = -1;
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



// Funkcja zamalowująca na czarno wszystkie piksele obrazu *img
void MainWindow::clean()
{
    // deklaracja wskaźnika do poruszania się po pikselach obrazu
    unsigned char *ptr;

    ptr = img->bits();

    int i,j;
    int kolor =0;

    for( i=0; i<height; i++)
    {
            for(j=0; j<width; j++)
            {
                ptr[width*4*j + 4*j] = kolor;   // ustawiamy składową ,,niebieską'' na 0
                ptr[width*4*j + 4*j + 1] = kolor; // ustawiamy składową ,,zielonąą'' na 0
                ptr[width*4*j + 4*j + 2] = kolor; // ustawiamy składową ,,czerwoną'' na 0
            }
    }

 }

//*****************************************************************************************************










int MainWindow::wstaw_piksel(int x0, int y0, int b, int g, int r) {

    unsigned char *wsk;


    if ((x0>=0)&&(y0>=0)&&(x0<width)&&(y0<height))
    {
        wsk = img->bits();
        wsk[width*4*y0 + 4*x0] = b;
        wsk[width*4*y0 + 4*x0+1] = g;
        wsk[width*4*y0 + 4*x0+2] = r;
    }
    return(0);
}


int MainWindow::wstaw_punkt(int x0, int y0, int grubosc, int b, int g, int r)
{
    int i,j;
    for(j=y0-grubosc; j<=y0+grubosc; j++){
        for (i=x0-grubosc; i<=x0+grubosc; i++){
            wstaw_piksel(i,j,b,g,r);
        }
    }
 return(0);
}

void MainWindow::on_styczne_clicked()
{
    if (czy_styczne==0)
    {
        czy_styczne=1;
        rysuj();
    }
    else
    {
        czy_styczne=0;
        rysuj();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
        int X, Y, i;
        //pobieramy pkt klikniecia
        X = event->x() - startX;
        Y = event->y() - startY;

        if(event->button() == Qt::RightButton )
        {
            ktory_punkt++;
            x[ktory_punkt] = X;
            y[ktory_punkt] = Y;
            //segment = ktory_punkt /4;
            if(ktory_punkt % 4 == 3)
            {
                segment++;
                ktory_punkt++;
                x[ktory_punkt] = X;
                y[ktory_punkt] =Y;
            }
            rysuj();
        }

        if ( event->button() == Qt::LeftButton)
        {
            for (i=0; i<=ktory_punkt; i++)
                if (i%4 != 0 || i==0)
                    if (abs(X-x[i])<=grubosc && abs(Y-y[i])<=grubosc)
                    {

                        aktywny=i;
                    }
            if (aktywny != -1)
                rysuj();
        }

}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    int X, Y;
    //pobieramy pkt klikniecia
    X = event->x() - startX;
    Y = event->y() - startY;
    if (aktywny != -1)
    {
        x[aktywny] = X;
        y[aktywny] = Y;
        aktywny = -1;
        if (aktywny % 4 == 3 )
        {
                x[aktywny+1]=X;
                y[aktywny+1]=Y;
        }
        rysuj();
    }
        update();
}

void MainWindow::mouseMoveEvent( QMouseEvent * event )
{
        int X, Y;
        //pobieramy pkt klikniecia
        X = event->x() - startX;
        Y = event->y() - startY;
        if (aktywny != -1)
        {
            x[aktywny] = X;
            y[aktywny] = Y;
            if (aktywny % 4 == 3 )
            {
                 x[aktywny+1]=X;
                 y[aktywny+1]=Y;
            }

            rysuj();
        }
            update();
    }




int MainWindow::rysuj()
{
    clean();
    if (czy_punkty==1){
        for (int i=0; i<=ktory_punkt; i++)
        {
            if (i % 4 !=0 || i==0)
                wstaw_punkt(x[i],y[i],grubosc,0,0,255);
            if (aktywny != -1)
                wstaw_punkt(x[aktywny],y[aktywny],grubosc,0,255,0);
        }

        if (ktory_punkt>=3)
        {
            for (int  i=0; i<=segment; i++)
            {
                draw_bezier(4*i, 4*i, 4*i+1, 4*i+1, 4*i+2, 4*i+2, 4*i+3, 4*i+3);
            }




        }
    }




    update();
    return(0);
}

//rysuje krzywa beziera dla 4 punktow - 3go stopnia
void MainWindow::draw_bezier(int px0, int py0, int px1, int py1,int px2, int py2,int px3, int py3 )
{
    int x_1,y_1;

   // QTextStream(stdout) << x0<<y0 << " \n";

    //rysowanie krzywej Beizera 3go stopnia
    for (float t=0; t<=1; t+=0.001) {
        x_1=qPow((1-t),3)*x[px0]+3*(qPow((1-t),2))*t*x[px1]+3*(1-t)*qPow(t,2)*x[px2]+qPow(t,3)*x[px3];
        y_1=qPow((1-t),3)*y[py0]+3*(qPow((1-t),2))*t*y[py1]+3*(1-t)*qPow(t,2)*y[py2]+qPow(t,3)*y[py3];

        //drawPixel(x_1,y_1);
        wstaw_punkt(x_1,y_1,grubosc,0,255,0);
    }
}


// zamalowuje piksel (x,y) na kolor (red,green,blue), domyślnie na biało
void MainWindow::drawPixel(int x, int y)
{
    unsigned char *wsk;

    // sprawdzamy czy (x,y) leży w granicach rysunku
    if(x>=0 && y>=0 && x<width && y<height)
    {
        wsk = img->scanLine(y);
        wsk[4*x] = b;
        wsk[4*x+1] = g;
        wsk[4*x+2] = r;
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
    clean();

    ktory_punkt=-1;
    segment=-1;


    // Po zmodyfikowaniu obiektu QImage odświeżamy okno aplikacji, aby zobaczyc zmiany
    update();
}

void MainWindow::on_XChanged(int x0)
{
   ui->labelX->setText(QString::number(x0));
}

void MainWindow::on_YChanged(int y0)
{
   ui->labelY->setText(QString::number(y0));
}
void MainWindow::on_X1Changed(int x1)
{
   ui->labelX_2->setText(QString::number(x1));
}

void MainWindow::on_Y1Changed(int y1)
{
   ui->labelY_2->setText(QString::number(y1));
}
void MainWindow::on_X2Changed(int x2)
{
   ui->labelX_3->setText(QString::number(x2));
}

void MainWindow::on_Y2Changed(int y2)
{
   ui->labelY_3->setText(QString::number(y2));
}
void MainWindow::on_X3Changed(int x3)
{
   ui->labelX_4->setText(QString::number(x3));
}

void MainWindow::on_Y3Changed(int y3)
{
   ui->labelY_4->setText(QString::number(y3));
}

void MainWindow::on_radioButton_clicked()
{
    option=1;
}

void MainWindow::on_radioButton_2_clicked()
{
    option=2;
}

void MainWindow::on_radioButton_3_clicked()
{
    option=3;
}

void MainWindow::on_radioButton_4_clicked()
{
    r=255;
    g=0;
    b=0;
}

void MainWindow::on_radioButton_6_clicked()
{
    r=0;
    g=255;
    b=0;
}

void MainWindow::on_radioButton_5_clicked()
{
    r=255;
    g=255;
    b=255;
}






