/*
 * File:   Forma.h
 * Author: werty
 *
 * Created on 28 kwiecień 2011, 16:42
 */

#ifndef _FORMA_H
#define	_FORMA_H

#include "ui_Forma.h"

#include <QDebug>

//#include "Widget.h"
#include "muParser.h"
#include "muParserInt.h"
#include <vector>
#include "Problem.h"
#include "Wykres2d.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QVariant>
#include "qwt3d/qwt3d_gridplot.h"
#include "qwt3d/qwt3d_function.h"
#include "MOPSO.h"
#include "NSGA_II.h"
#include "GGA.h"
#include "Wskazniki.h"
#include <qtmmlwidget.h>



#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>


#include "uzyteczne_funkcje.h"
#include "struktury.h"

#include "qcustomplot.h"


#include <QStandardItemModel>


using namespace Qwt3D;


class QtMmlWidget;


class QThreadEx : public QThread
{
protected:
    void run() { exec(); }
};


class Rosenbrock : public Function {
public:
    Parser parser;
    double xVal, yVal;

    Rosenbrock(GridPlot& pw)
    : Function(pw) {

        parser.DefineVar("x", &this->xVal);
        parser.DefineVar("y", &this->yVal);

    }

    double operator()(double x, double y) {
        this->xVal = x;
        this->yVal = y;
        return parser.Eval();
        // return log((1 - x)*(1 - x) + 100 * (y - x * x)*(y - x * x)) / 8;
    }
};

class Plot : public GridPlot {
public:

    Plot() {

        // setTitle(QString("$\\frac{1}{x^2+y^2+\\frac{1}{2}}$"));

        rosenbrock = new Rosenbrock(*this);

        rosenbrock->setMesh(50, 50);
        rosenbrock->setDomain(0, 10, 0, 10);
        // rosenbrock->setMinZ(0);
        // rosenbrock->setMaxZ(20);



        //  QFont serifFont("Times", 10);
        // QFont sansFont("Helvetica [Cronyx]", 12);

    }
    Rosenbrock* rosenbrock;
};

class Forma : public QMainWindow {
    Q_OBJECT
public:
    Forma();
    bool flaga_bledu;
    virtual ~Forma();
    void createActions();
    void createMenus();
    bool wczytajProblem(QString nazwa_pliku);
    bool zapiszProblem(QString nazwa_pliku);

    
    QTime time;
   
    QMenu *fileMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
    QActionGroup *alignmentGroup;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *settingsAct;

    QVector<QPointF> dane;
    Problem* problem;
    MOPSO* mopso;
    NSGA_II* nsga_ii;
    GGA* gga;
    
    Plot* plot3d;
    
    Wykres2d* wykresGOL;
    
    QCustomPlot* qPlot;


    QwtPlot *plot2d;//,*plotGOL;
   // QwtPlotCurve *plotGOL_data;
  //  QVector<QPointF>vGol;
    Wskazniki* wskazniki;
    
    QwtPlotCurve *curve1;
    QwtPlotCurve *curve2;

    QwtPlotCurve *points_nondom;
    QwtPlotCurve *points_dom;
   // QwtPlotDirectPainter *d_directPainter;
    void Wykres();
   // double x[600], y[600];
    int size;
    QtMmlWidget *m_mml_widget;
    void Zaladuj_wzor(QWidget * parent, QString path);
    void aktualizuj_wykres_przystosowania();
    void aktualizuj_wykres_przystosowania(QVector< QVector<Solution*> > *F);
    void aktualizuj_wykres_gol();
    void Inicjalizuj_zmienne();
    void Dodaj_wszystkie_funkcje();
    void Dodaj_zakresy_zmiennych();
    void IterujRazMOPSO();
    void Loguj();

    void PokazWszystkieFronty();


    void AktualizujWykresGGA();
    
    // static  value_type* AddVariable(const char_type *a_szName, void *a_pUserData);
    //---------------------------------------------------------------------------
    // Factory function for creating new parser variables
    // This could as well be a function performing database queries.


    QMap<QTableWidgetItem*,int> mFunction;
    QMap<QTreeWidgetItem *,int> mFunctionAttachedToGender;


private slots:

    void newFile();
    void open();
    void save();
    void exit();
    void settings();

    void Rysuj();
    
    void Zmiana_funkcji();
    void Suwak_pionowy();
    void Suwak_poziomy();
    void Cube();
    void Resetuj();
    void Wireframe();
    void Dodaj_funkcje();
    void Usun_funkcje();
    void sDodaj_zakresy_zmiennych();
    void Dodaj_ograniczenie();
    void Usun_ograniczenie();
    void sDodaj_wszystkie_funkcje();
    void sDodaj_wszystkie_ograniczenia();
    
    void sInicjalizujMOPSO();
    void sIterujMOPSO();
    void sIterujRazMOPSO();
    void sStopMOPSO();
    void sDoneMOPSO();
    void sFinishedMOPSO();


    void sInicjalizujNSGA();
    void sIterujNSGA();
    void sIterujRazNSGA();
    void sStopNSGA();
    void sDoneNSGA();
    void sFinishedNSGA();

    void sInicjalizujGGA();
    void sIterujGGA();
    void sIterujRazGGA();
    void sStopGGA();
    void sDoneGGA();
    void sFinishedGGA();

    void sAttachFunToGender();
    void sRemoveFun();
    void sRemoveAllFun();


private:
    Ui::Forma widget;
};

#endif	/* _FORMA_H */
