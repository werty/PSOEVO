/*
 * File:   main.cpp
 * Author: marcin
 *
 * Created on 22 kwiecień 2011, 13:33
 */

#include <QtGui/QApplication>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <locale>
#include <limits>
#include <ios>
#include <iomanip>
#include <bitset>




#include "Forma.h"

//-----------------------------------------------------------------
//              simpleplot.cpp
//
//      A simple example which shows how to use SurfacePlot
//-----------------------------------------------------------------

#include <math.h>
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>
//#include "buildnumber.h"
void myMessageOutput(QtMsgType type, const char *msg) {
    switch (type) {
        case QtDebugMsg:


            fprintf(stderr, "Debug: %s", msg);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s\n", msg);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", msg);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", msg);
            abort();
    }

}

union Fenotyp{
    unsigned short liczba;
    bool bity[16];

    Fenotyp(){
        liczba=0;
        bity[0]=true;
    }

};

void SF(Fenotyp &fen)
{
    for(unsigned int i=0;i<16;i++)
    {
        if(fen.bity[i])
        {
            cout<<"1";
        }
        else
        {
            cout<<"0";
        }

    }

}



int main(int argc, char **argv) {

    /////testy/////////////



//    QVector<unsigned int> tab1(5),tab2(10);

//    for (int i = 0; i < 5; ++i) {
//        tab1[i]=i;
//    }

//    QVector<unsigned int>::iterator it;

//    for (auto it = tab1.begin(); it !=tab1.end()-1; ++it) {
//        qDebug()<<*it;
//    }


//    qDebug()<<tab1;


//    for (int i = 0; i < 10; ++i) {
//        tab2[i]=i*i;
//    }

//    swapWithMove(tab1,tab2);



    ////////////////////

#ifdef QT_NO_DEBUG_OUTPUT
    fprintf(stderr,"%s\n",__DATE__);
    fprintf(stderr,"%s\n",__TIME__);
#endif

    qDebug()<<"debug";
   // qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    if (!QGLFormat::hasOpenGL()) {
        qWarning("This system has no OpenGL support. Exiting.");
        return -1;
    }
    Forma okno;
    okno.show();

//    Genotype genotyp(6,true);
//    genotyp[0]=true;
//    cout<<"genotyp "<<qPrintable(genotyp.GetString())<<" = "<<genotyp.GetValue()<<"\n";

//    genotyp[1]=true;
//    cout<<"genotyp "<<qPrintable(genotyp.GetString())<<" = "<<genotyp.GetValue()<<"\n";



//    genotyp[2]=true;
//    cout<<"genotyp "<<qPrintable(genotyp.GetString())<<" = "<<genotyp.GetValue()<<"\n";



   return app.exec();
}



