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
#include "mopso_for_ssmodel.h"
#include "NSGA_II.h"
#include "nsga_ii_for_ssmodel.h"
#include "GGA.h"
#include "gga_for_ssmodel.h"
#include "Wskazniki.h"
#include <qtmmlwidget.h>

#include "colorlisteditor.h"
#include "stringlisteditor.h"


#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>


#include "uzyteczne_funkcje.h"
#include "struktury.h"

#include "qcustomplot.h"
#include "ssmodels.h"

#include <QColor>
#include <QVector>
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


enum RodzDanych{
    wszystkie,
    front_pareto,

};

struct AxisSetting{
   bool isParam;//parametry czy wart funkcji kryt
   unsigned int index;//numer funkcji czy parametru
  // unsigned int indexOfVariable;

};
struct GraphSetting{
    QVector<AxisSetting> axisSettings;
    bool all;//wszystkie osobniki
    bool pareto;//czy tylko front pareto
    int indexOfGender;//czy moze ktorys osob przyp ktoremus z rodzajnikow
    QString color;
    QString name;
    QString patternName;
    unsigned int algorithm;//0- MOPSO, 1-NSGA-II, 2-GGA
    unsigned int patternIndex;
};

class Plot2DSetting{
public:
    QString windowCaption,xAxisCaption,yAxisCaption;
    int tabIndex;//index zakladki na ktorej znajduje sie wykres
    QCustomPlot* qPlot;
    QVector<GraphSetting> graphSettings;
    Plot2DSetting()
    {
        tabIndex=-1;
        qPlot=nullptr;
    }


};

class AuxClass : public QObject{
public:
    AuxClass(){

    };
    AuxClass(int row,int col){
        this->col=col;
        this->row=row;
    };
    ~AuxClass()
    {

    };

    int col,row;


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
    bool isFunctionInitialized;//czy funkcje zostaly wczytane
    bool isGGAInitialized;
    bool isNSGA_IIInitialized;
    bool isMOPSOInitialized;

    QVector<Plot2DSetting> plotSettingsGGA;
    QVector<Plot2DSetting> plotSettingsMOPSO;
    QVector<Plot2DSetting> plotSettingsNSGA_II;
    QVector<Plot2DSetting> plot2DSettings;
   // QVector<Plot3DSetting> plot3DSettings;

    Plot2DSetting plotSettingsGOL;
    Plot2DSetting plotSettingsSpacing;
    Plot2DSetting plotSettingsD_EC;
    Plot2DSetting plotSettingsD_EC_GOL;
    Plot2DSetting plotSettingsD_EP_GOL;
    void AddIndicatorGraphToQCPlot(QCustomPlot* qp,unsigned int index);
    void UpdateIndicatorsGraphs();

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
    
   // Wykres2d* wykresGOL;
    
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
    void AddRowToMOPSOPlotTable();
    void AddRowTo2DPlotsTable();
    void AddRowTo3DPlotsTable();
    void AddRowToGGAPlotTable();
    void AddRowToNSGA_IIPlotTable();
    void PlotImpulseResponse(ContinuousDynamicalSystem *sys,double Kp,double Ki,double Kd, double time, double dt);
    void PlotStepResponse(ContinuousDynamicalSystem*sys,double Kp,double Ki,double Kd, double time, double dt);

    void AddPlot(QCustomPlot *qPlotWidget,QVector<QPair<double,double> > &dataXY,QString name,QColor color);

    void PokazWszystkieFronty();

    void AddGGADataToGraph(QCPGraph* graph,QVector<AxisSetting>* as,Solution* s,float& a,float& b);
    void UpdateAllPlots();
    void Update2DPlots();
    void Update3DPlots();


    unsigned int numOfIterations;

    
    // static  value_type* AddVariable(const char_type *a_szName, void *a_pUserData);
    //---------------------------------------------------------------------------
    // Factory function for creating new parser variables
    // This could as well be a function performing database queries.


    QMap<QTableWidgetItem*,int> mFunction;
    QMap<QTreeWidgetItem *,int> mFunctionAttachedToGender;

    SysClosedLoopYsWithPID sys;

    MOPSO_for_ssmodel* mopso_ssmodel;
    GGA_for_ssmodel * gga_ssmodel;
    NSGA_II_for_ssmodel *nsga_ii_ssmodel;

    void AktualizujWykresFunKrytGGA();
    void AktualizujWykresParamGGA();

    void showWarning(const QString &msg);
    void AddMOPSODataToGraph(QCPGraph *graph, QVector<AxisSetting> *as, Particle *s, float &a, float &b);


    template<typename T>
    void AddDataTo2DGraph(QCPGraph *graph, QVector<AxisSetting> *as, T *s, float &a, float &b);
public slots:
     void changeGGAIndexSlot(int index,int row,int column);
     void currentGGAIndexChanged(QObject *ac);
     void currentMOPSOIndexChanged(QObject *ac);
     void currentPlot2DIndexChanged(QObject *ac);
     void currentPlot3DIndexChanged(QObject *ac);
     void currentNSGA_IIIndexChanged(QObject *ac);
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

    void on_stworzWykresyGGA_clicked();

    void on_pushButtonDodajWskazniki_clicked();

    void on_bDodajWykres_clicked();

    void on_checkBoxMOPSO_toggled(bool checked);

    void on_checkBoxNSGAII_toggled(bool checked);

    void on_checkBoxGGA_toggled(bool checked);

    void on_checkBoxGGA_clicked();

    void on_checkBoxMOPSO_clicked();

    void on_checkBoxNSGAII_clicked();



    void on_bInicjalizujSymulacja_clicked();

    void on_bIterujRazSymulacja_clicked();

    void on_bDodajWykresMOPSO_clicked();

    void on_bDodajWykresNSGA_II_clicked();

    void on_stworzWykresyMOPSO_clicked();

    void on_bIterujRazGGA_clicked();

    void on_bIterujRazMOPSO_clicked();

    void on_stworzWykresyNSGA_II_clicked();

    void on_bIterujNSGA_2_clicked();

    void on_bInicjalizujNSGA_clicked();

    void on_bInicjalizujMOPSO_clicked();

    void on_bInicjalizujGGA_clicked();

    void on_bDodajWykres2D_clicked();

    void on_bStworzOkno2D_clicked();

    void on_bIterujSymulacja_clicked();

    void on_qPlotWidget_customContextMenuRequested(const QPoint &pos);

    void on_bIterujMOPSO_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_bDodajWykres3D_clicked();

    void on_bStworzOkno3D_clicked();

private:
    QSignalMapper *signalMapper;
    Ui::Forma widget;
};


template <typename T>
void Forma::AddDataTo2DGraph(QCPGraph* graph, QVector<AxisSetting>* as, T *s, float& a, float& b)
{
    TRACE;

    if((*as)[0].isParam)
    {
        a=s->x[(*as)[0].index];
    }
    else
    {
        a=s->wartFunkcjiKryterialnych[(*as)[0].index];
    }
    if((*as)[1].isParam)
    {
        b=s->x[(*as)[1].index];
    }
    else
    {
        b=s->wartFunkcjiKryterialnych[(*as)[1].index];
    }
    qDebug()<<"a "<<a<<" b "<<b;
    graph->addData(a,b);
    TRACE;
}



#endif	/* _FORMA_H */
