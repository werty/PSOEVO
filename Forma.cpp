/*
 * File:   Forma.cpp
 * Author: werty
 *
 * Created on 28 kwiecień 2011, 16:42
 */

//#include <qt4/QtGui/qradiobutton.h>
//#include <qt4/QtGui/qbuttongroup.h>
#include <QtGui/QMessageBox>

#include "treemodel.h"
#include "Forma.h"
#include <utility>
#include <QItemEditorFactory>
#include <vector>
//int Solution::tmp;


//TODO: dodac pomoc tooltip
//TODO: wiecej wykresow i wskaznikow

Forma::Forma() {
    widget.setupUi(this);




    setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    isFunctionInitialized=false;
    isGGAInitialized=false;
    isMOPSOInitialized=false;
    isNSGA_IIInitialized=false;

    //    QItemEditorFactory *factory = new QItemEditorFactory;

    //    QItemEditorCreatorBase *colorListCreator =
    //            new QStandardItemEditorCreator<ColorListEditor>();



    //    QItemEditorCreatorBase *stringListCreator =
    //            new QStandardItemEditorCreator<StringListEditor>();

    //    factory->registerEditor(QVariant::Color, colorListCreator);
    //    factory->registerEditor(QVariant::String, stringListCreator);

    //    QItemEditorFactory::setDefaultFactory(factory);





    nsga_ii = NULL;
    wskazniki = new Wskazniki();
    flaga_bledu = false;




    //resize(800, 1000);
    createActions();
    createMenus();



    //    int row = widget.lista_funkcji->rowCount();
    //    widget.lista_funkcji->insertRow(row);
    //    QTableWidgetItem *item0 = new QTableWidgetItem;
    //    QTableWidgetItem *item1 = new QTableWidgetItem;
    //    QTableWidgetItem *item2 = new QTableWidgetItem;
    //    //    // item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //    widget.lista_funkcji->setItem(row, 0, item0);
    //    widget.lista_funkcji->setItem(row, 1, item1);
    //    widget.lista_funkcji->setItem(row, 2, item2);
    //
    //    widget.lista_funkcji->resizeColumnToContents(0);
    widget.lista_funkcji->horizontalHeader()->setStretchLastSection(true);
    widget.lista_funkcji->setColumnWidth(0, 295);
    widget.lista_funkcji->setColumnWidth(1, 40);
    widget.lista_funkcji->setColumnWidth(2, 40);
    Dodaj_funkcje();
    Dodaj_funkcje();

    widget.lista_funkcji->item(0, 0)->setText(QString("(x1+50)^2+x2^2"));
    widget.lista_funkcji->item(1, 0)->setText(QString("(x1-50)^2+x2^2"));

    widget.lista_zmiennych->horizontalHeader()->setStretchLastSection(true);
    widget.lista_zmiennych->setColumnWidth(0, 215);
    widget.lista_zmiennych->setColumnWidth(1, 80);
    widget.lista_zmiennych->setColumnWidth(2, 80);

    widget.lista_ograniczen->horizontalHeader()->setStretchLastSection(true);
    widget.lista_ograniczen->setColumnWidth(0, 165);
    widget.lista_ograniczen->setColumnWidth(1, 45);
    widget.lista_ograniczen->setColumnWidth(2, 165);

    //widget.plot=new Plot();

    plot3d = new Plot();

    plot3d->setParent(widget.Plot3D);
    // plot3d->rosenbrock->parser.SetExpr("log((1 - x)*(1 - x) + 100 * (y - x * x)*(y - x * x)) / 8");
    plot3d->resize(600, 570);






    Rysuj();

    // mu::ParserInt int_parser;

    //   try {

    //#ifdef MUP_EXAMPLE_INT_PARSER
    //        int_parser.SetExpr(sLine);
    //        mu::console() << int_parser.Eval() << "\n";
    //        mu::console() << int_parser.Eval() << "\n";
    //#elif defined MUP_EXAMPLE_COMPLEX_PARSER
    //        cmplx_parser.SetExpr(sLine);
    //        mu::console() << cmplx_parser.Eval() << "\n";
    //#else
    //
    //
    //
    //
    //
    //
    //#endif
    //    } catch (mu::Parser::exception_type &e) {
    //        mu::console() << _T("\nError:\n");
    //        mu::console() << _T("------\n");
    //        mu::console() << _T("Message:     ") << e.GetMsg() << _T("\n");
    //        mu::console() << _T("Expression:  \"") << e.GetExpr() << _T("\"\n");
    //        mu::console() << _T("Token:       \"") << e.GetToken() << _T("\"\n");
    //        mu::console() << _T("Position:    ") << (int) e.GetPos() << _T("\n");
    //        mu::console() << _T("Errc:        ") << std::dec << e.GetCode() << _T("\n");
    //    }




    // Dot dot=Dot(0.007,0.5);

    //  Dot* dot = (Dot*)plot.setPlotStyle(Dot(0.007,false));

    // plot.addEnrichment(Dot(0.007,false));
    //    widget.lista_funkcji= new QTableWidget(0, 4);
    //    widget.lista_funkcji->setHorizontalHeaderLabels(QStringList() << tr("Działanie")
    //                                                   << tr("Priorytet")
    //                                                   << tr("Parametry")
    //                                                   << tr("Kolor")
    //                                                   );
    connect(widget.Rysuj, SIGNAL(clicked()), this, SLOT(Rysuj()));
    connect(widget.bIterujMOPSO, SIGNAL(clicked()), this, SLOT(sIterujMOPSO()));
    //connect(widget.Funkcje, SIGNAL(currentGGAIndexChanged(int)), this, SLOT(Zmiana_funkcji()));
    connect(widget.horizontalScrollBar, SIGNAL(sliderMoved(int)), this, SLOT(Suwak_poziomy()));
    connect(widget.verticalScrollBar, SIGNAL(sliderMoved(int)), this, SLOT(Suwak_pionowy()));
    connect(widget.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(Cube()));

    connect(widget.Wireframe, SIGNAL(clicked()), this, SLOT(Wireframe()));

    connect(widget.usun_funkcje, SIGNAL(clicked()), this, SLOT(Usun_funkcje()));
    connect(widget.dodaj_funkcje, SIGNAL(clicked()), this, SLOT(Dodaj_funkcje()));
    connect(widget.zaladuj_funkcje, SIGNAL(clicked()), this, SLOT(sDodaj_wszystkie_funkcje()));

    connect(widget.usun_ograniczenie, SIGNAL(clicked()), this, SLOT(Usun_ograniczenie()));
    connect(widget.dodaj_ograniczenie, SIGNAL(clicked()), this, SLOT(Dodaj_ograniczenie()));
    connect(widget.zaladuj_ograniczenia, SIGNAL(clicked()), this, SLOT(sDodaj_wszystkie_ograniczenia()));
    connect(widget.zaladuj_zakresy_zmiennych, SIGNAL(clicked()), this, SLOT(sDodaj_zakresy_zmiennych()));


    connect(widget.bInicjalizujMOPSO, SIGNAL(clicked()), this, SLOT(sInicjalizujMOPSO()));
    //  connect(widget.bIterujRazMOPSO, SIGNAL(clicked()), this, SLOT(sIterujRazMOPSO()));

    connect(widget.bInicjalizujNSGA, SIGNAL(clicked()), this, SLOT(sInicjalizujNSGA()));
    connect(widget.bIterujNSGA, SIGNAL(clicked()), this, SLOT(sIterujNSGA()));


    connect(widget.bStopNSGA_II, SIGNAL(clicked()), this, SLOT(sStopNSGA()));
    connect(widget.bStopMOPSO, SIGNAL(clicked()), this, SLOT(sStopMOPSO()));
    

    connect(widget.bInicjalizujGGA, SIGNAL(clicked()), this, SLOT(sInicjalizujGGA()));
    connect(widget.bIterujRazGGA, SIGNAL(clicked()), this, SLOT(sIterujRazGGA()));
    connect(widget.bAttachFunToGender, SIGNAL(clicked()), this, SLOT(sAttachFunToGender()));
    connect(widget.bRemoveFun, SIGNAL(clicked()), this, SLOT(sRemoveFun()));
    connect(widget.bRemoveAllFun, SIGNAL(clicked()), this, SLOT(sRemoveAllFun()));



    //    plotGOL = new QwtPlot(QwtText("GOL"), (QWidget*) widget.PlotGOL);
    //    plotGOL->setFixedSize(500, 500);
    //    plotGOL_data = new QwtPlotCurve("GOL");
    //
    //    plotGOL_data->attach(plotGOL);
    //    plotGOL->setAxisAutoScale(plotGOL->xBottom, true);
    //    plotGOL->setAxisAutoScale(plotGOL->yLeft, true);
    //    plotGOL->setTitle("Globalny poziom optymalności");
    //    plotGOL->setAxisTitle(QwtPlot::xBottom, "Pokolenia");
    //    plotGOL->setAxisTitle(QwtPlot::yLeft, "GOL");

    // wykresGOL = new Wykres2d((QWidget*) widget.PlotGOL, 500, 500, "Globalny poziom optymalności", "Pokolenia", "GOL");

    //////////////////////////////////////

    //    qPlot=new QCustomPlot( (QWidget*) widget.qPlotNSGA_II_PID);

    //    qPlot->setFixedSize(500,500);
    //    qPlot->addGraph();
    //    QPen pen;
    //    pen.setStyle(Qt::DotLine);
    //    pen.setWidth(1);
    //    pen.setColor(QColor(180,180,180));
    //    qPlot->graph(0)->setName("Confidence Band 68%");
    //    qPlot->graph(0)->setPen(pen);
    //    qPlot->graph(0)->setBrush(QBrush(QColor(255,50,30,20)));

    //    qPlot->legend->setVisible(true);
    //    qPlot->legend->setFont(QFont("Helvetica",9));
    //    QVector<double> x0(250), y0(250);
    //    QVector<double> yConfUpper(250), yConfLower(250);
    //    for (int i=0; i<250; ++i)
    //    {
    //        x0[i] = (i/249.0-0.5)*30+0.01; // by adding a small offset we make sure not do divide by zero in next code line
    //        y0[i] = sin(x0[i])/x0[i]; // sinc function
    //        yConfUpper[i] = y0[i]+0.15;
    //        yConfLower[i] = y0[i]-0.15;
    //        x0[i] *= 1000;
    //    }
    //    QVector<double> x1(50), y1(50), y1err(50);
    //    for (int i=0; i<50; ++i)
    //    {
    //        // generate a gaussian distributed random number:
    //        double tmp1 = rand()/(double)RAND_MAX;
    //        double tmp2 = rand()/(double)RAND_MAX;
    //        double r = sqrt(-2*log(tmp1))*cos(2*M_PI*tmp2); // box-muller transform for gaussian distribution
    //        // set y1 to value of y0 plus a random gaussian pertubation:
    //        x1[i] = (i/50.0-0.5)*30+0.25;
    //        y1[i] = sin(x1[i])/x1[i]+r*0.15;
    //        x1[i] *= 1000;
    //        y1err[i] = 0.15;
    //    }
    //    // pass data to graphs and let QCustomPlot determine the axes ranges so the whole thing is visible:
    //    qPlot->graph(0)->setData(x0, yConfUpper);

    //    qPlot->setupFullAxesBox();
    //    qPlot->replot();

    //////////////////////////////////////////

    //    plot2d = new QwtPlot(QwtText("Two Curves"), (QWidget*) widget.Plot2D);
    //    plot2d->setFixedSize(500, 500);
    //    // add curves
    //    curve1 = new QwtPlotCurve("Curve 1");
    //    curve2 = new QwtPlotCurve("Curve 2");


    //    points_nondom = new QwtPlotCurve("Test Curve");
    //    points_nondom->setStyle(QwtPlotCurve::NoCurve);
    //    points_nondom->setData(new CurveData());

    //    points_nondom->setSymbol(new QwtSymbol(QwtSymbol::XCross,
    //                                           Qt::NoBrush, QPen(Qt::blue), QSize(4, 4)));

    //    points_nondom->attach(plot2d);



    //    points_dom = new QwtPlotCurve("Test Curve");
    //    points_dom->setStyle(QwtPlotCurve::NoCurve);
    //    points_dom->setData(new CurveData());

    //    points_dom->setSymbol(new QwtSymbol(QwtSymbol::XCross,
    //                                        Qt::NoBrush, QPen(Qt::red), QSize(4, 4)));

    //    points_dom->attach(plot2d);

    //    //d_directPainter = new QwtPlotDirectPainter(plot2d);
    //    // copy the data into the curves
    //    //curve1->setRawSamples();
    //    //  curve2->setData(...);

    //    plot2d->setAxisAutoScale(plot2d->xBottom, true);
    //    plot2d->setAxisAutoScale(plot2d->yLeft, true);
    //    //curve1->attach(plot2d);
    //    //  curve2->attach(plot2d);

    //    // finally, refresh the plot
    //    plot2d->replot();


    Zaladuj_wzor(widget.Wzor_PSO, "wzor_pso.mml");
    Zaladuj_wzor(widget.Wzor_c1, "wzor_c1.mml");
    Zaladuj_wzor(widget.Wzor_c2, "wzor_c2.mml");
    Zaladuj_wzor(widget.Wzor_w, "wzor_w.mml");
    Zaladuj_wzor(widget.Wzor_czi, "wzor_czi.mml");

    //    widget.czy_maksimum->setChecked(true);

    //  Rysuj();




    //    QTreeWidgetItem *item = new QTreeWidgetItem();

    //    item->setText(0, "zero");
    //    item->setFlags(item->flags() | Qt::ItemIsEditable);
    //    widget.treeWidget->insertTopLevelItem(0, item);

    //    item = new QTreeWidgetItem();

    //    item->setText(0, "jeden");
    //    item->setFlags(item->flags() | Qt::ItemIsEditable);
    //    widget.treeWidget->insertTopLevelItem(0, item);

    //  for (int column = 0; column < model->columnCount(); ++column)
    //     widget.treeView->resizeColumnToContents(column);

    //  QStandardItemModel *stdModel = new QStandardItemModel(0, 1,this);
    //  stdModel->setHorizontalHeaderLabels(headers2);


    //    stdModel->insertRow(0);
    //    stdModel->setData(stdModel->index(0, 0), "subject");
    //    stdModel->insertRow(1);
    //    stdModel->setData(stdModel->index(1, 0), "subject");
    //  widget.funtionTable->ins;

    widget.qPlotNSGA_II_PID->setTitle(tr("Odpowiedzi skokowe niezdominowanych rozwiazan"));
    widget.qPlotNSGA_II_PID->setAutoMargin(false);
    widget.qPlotNSGA_II_PID->setMargin(60,20,35,230);
    //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
    widget.qPlotNSGA_II_PID->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    widget.qPlotNSGA_II_PID->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    widget.qPlotNSGA_II_PID->legend->setFont(legendFont);
    widget.qPlotNSGA_II_PID->legend->setPositionStyle(QCPLegend::psManual);
    widget.qPlotNSGA_II_PID->legend->setPosition(QPoint(50,600));
    widget.qPlotNSGA_II_PID->legend->setBrush(QBrush(QColor(255,255,255,230)));
    widget.qPlotNSGA_II_PID->legend->setSelectable(QCPLegend::spItems);
    widget.qPlotNSGA_II_PID->setInteractions( QCustomPlot::iSelectLegend | QCustomPlot::iSelectTitle);
    widget.qPlotNSGA_II_PID->xAxis->setLabel("t");
    widget.qPlotNSGA_II_PID->yAxis->setLabel("y(t)");
    //QCPLa

  //  widget.qPlotNSGA_II_PID->
   // QCPLayer* layer=new QCPLayer(widget.qPlotNSGA_II_PID,"GOL");
    widget.qPlotNSGA_II_PID->addLayer("GOL");
    ///////////////////////////////////////////////////////////////////////////////////////

    widget.qPlotGGA_PID->setTitle(tr("Odpowiedzi skokowe niezdominowanych rozwiazan"));
    widget.qPlotGGA_PID->setAutoMargin(false);
    widget.qPlotGGA_PID->setMargin(60,20,35,230);
    //  widget.qPlotGGA_PID->setFixedSize(550,550);
    widget.qPlotGGA_PID->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    widget.qPlotGGA_PID->legend->setVisible(true);
    legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    widget.qPlotGGA_PID->legend->setFont(legendFont);
    widget.qPlotGGA_PID->legend->setPositionStyle(QCPLegend::psManual);
    widget.qPlotGGA_PID->legend->setPosition(QPoint(50,600));
    widget.qPlotGGA_PID->legend->setBrush(QBrush(QColor(255,255,255,230)));
    widget.qPlotGGA_PID->legend->setSelectable(QCPLegend::spItems);
    widget.qPlotGGA_PID->setInteractions( QCustomPlot::iSelectLegend | QCustomPlot::iSelectTitle);
    widget.qPlotGGA_PID->xAxis->setLabel("t");
    widget.qPlotGGA_PID->yAxis->setLabel("y(t)");
    widget.qPlotGGA_PID->addLayer("GOL");


    ////////////////////////////////////////////////////////////////////////////////////////


    widget.qPlotMOPSO_PID->setTitle(tr("Odpowiedzi skokowe niezdominowanych rozwiazan"));
    widget.qPlotMOPSO_PID->setAutoMargin(false);
    widget.qPlotMOPSO_PID->setMargin(60,20,35,230);
    //  widget.qPlotMOPSOI_PID->setFixedSize(550,550);
    widget.qPlotMOPSO_PID->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    widget.qPlotMOPSO_PID->legend->setVisible(true);
    legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    widget.qPlotMOPSO_PID->legend->setFont(legendFont);
    widget.qPlotMOPSO_PID->legend->setPositionStyle(QCPLegend::psManual);
    widget.qPlotMOPSO_PID->legend->setPosition(QPoint(50,600));
    widget.qPlotMOPSO_PID->legend->setBrush(QBrush(QColor(255,255,255,230)));
    widget.qPlotMOPSO_PID->legend->setSelectable(QCPLegend::spItems);
    widget.qPlotMOPSO_PID->setInteractions( QCustomPlot::iSelectLegend | QCustomPlot::iSelectTitle);
    widget.qPlotMOPSO_PID->xAxis->setLabel("t");
    widget.qPlotMOPSO_PID->yAxis->setLabel("y(t)");
    widget.qPlotMOPSO_PID->addLayer("GOL");




    //PlotStepResponse(&sys,1.8859,1.0/2.9903,0.9430,20.0,0.01);
    //   PlotStepResponse(&sys,3.1302,1.0/2.7712,0.8978,20.0,0.01);
    // PlotImpulseResponse(&sys,20.0,0.01);
    // PlotStepResponse(&sys,1,1,1,20.0,0.01);

    double maxOvershoot,riseTime,settlingTime;

    QVector<QPair<double,double> > dataXY;

    double time,dt;
    dt=0.01;
    time=40;

    //GetStepResponse(&sys,1.8859,1.0/2.9903,0.9430,time,dt,dataXY);
    //AddPlot(widget.qPlotNSGA_II_PID,dataXY,"QString name",QColor(Qt::red));
    //qDebug()<<"wyk1 "<<dataXY[0].second<<endl;
    //GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
    //qDebug()<<"MaxOvershoot "<<maxOvershoot<<" RiseTime "<<riseTime<<" SettlingTime "<<settlingTime<<endl;

    //GetStepResponse(&sys,3.1302,1.0/2.7712,0.8978,time,dt,dataXY);
    //AddPlot(widget.qPlotNSGA_II_PID,dataXY,"QString name",QColor(Qt::blue));
    //qDebug()<<"wyk1 "<<dataXY[0].second<<endl;
    //GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
    //qDebug()<<"MaxOvershoot "<<maxOvershoot<<" RiseTime "<<riseTime<<" SettlingTime "<<settlingTime<<endl;

    //GetStepResponse(&sys,2.3261,1.0/2.3897,0.5162,time,dt,dataXY);
    //AddPlot(widget.qPlotNSGA_II_PID,dataXY,"QString name",QColor(Qt::green));
    //qDebug()<<"wyk1 "<<dataXY[0].second<<endl;
    //GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
    //qDebug()<<"MaxOvershoot "<<maxOvershoot<<" RiseTime "<<riseTime<<" SettlingTime "<<settlingTime<<endl;

    //GetStepResponse(&sys,4.1484,1.0/2.9842,0.746,time,dt,dataXY);
    //AddPlot(widget.qPlotNSGA_II_PID,dataXY,"QString name",QColor(Qt::black));
    //GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
    //qDebug()<<"MaxOvershoot "<<maxOvershoot<<" RiseTime "<<riseTime<<" SettlingTime "<<settlingTime<<endl;

    dataXY.clear();
    for (float  i= 0;  i< time; i+=dt) {
        dataXY.push_back({i,1.0});
    }
    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"skok jednostkowy",QColor(Qt::gray));
    AddPlot(widget.qPlotGGA_PID,dataXY,"skok jednostkowy",QColor(Qt::gray));
    AddPlot(widget.qPlotMOPSO_PID,dataXY,"skok jednostkowy",QColor(Qt::gray));

    dataXY.clear();
    for (float  i= 0;  i< time; i+=dt) {
        dataXY.push_back({i,1.02});
    }
    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"1.02",QColor(Qt::gray));
    AddPlot(widget.qPlotGGA_PID,dataXY,"1.02",QColor(Qt::gray));
    AddPlot(widget.qPlotMOPSO_PID,dataXY,"1.02",QColor(Qt::gray));

    dataXY.clear();
    for (float  i= 0;  i< time; i+=dt) {
        dataXY.push_back({i,0.98});
    }
    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"0.98",QColor(Qt::gray));
    AddPlot(widget.qPlotGGA_PID,dataXY,"0.98",QColor(Qt::gray));
    AddPlot(widget.qPlotMOPSO_PID,dataXY,"0.98",QColor(Qt::gray));

    //qDebug()<<"wyk1 "<<dataXY[0].second<<endl;




    qDebug()<<"stability for pid 3.1302 1/2.7712 0.8978 "<<CheckStabilityOfMySystem(1,1,1)<<endl;



    problem = new Problem();
    problem->tab_minmax = new bool[3];

    for (int i = 0; i < 3; ++i) {
        problem->funkcje.push_back("pid");
        problem->zmienne.push_back(new Zmienna("pid"));
        problem->tab_minmax[i] = false;
    }
    problem->zmienne[0]->min=0;
    problem->zmienne[0]->max=10;
    problem->zmienne[1]->min=0;
    problem->zmienne[1]->max=6;
    problem->zmienne[2]->min=0;
    problem->zmienne[2]->max=1;
    problem->cos = 666;

    mopso_ssmodel=new MOPSO_for_ssmodel(30,10,10,false,problem);
    mopso_ssmodel->vmax = 100.0;
    qDebug()<<"il osob "<<mopso_ssmodel->populacja.size()<<endl;

    mopso_ssmodel->Inicjalizuj();
    for (int i = 0; i < mopso_ssmodel->populacja.size(); ++i) {
        if(CheckStabilityOfMySystem(mopso_ssmodel->populacja[i]->x[0],mopso_ssmodel->populacja[i]->x[1],mopso_ssmodel->populacja[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,mopso_ssmodel->populacja[i]->x[0],mopso_ssmodel->populacja[i]->x[1],mopso_ssmodel->populacja[i]->x[2],time,dt,dataXY);
            AddPlot(widget.qPlotMOPSO_PID,dataXY,""+QVariant(mopso_ssmodel->populacja[i]->x[0]).toString()+" , "+QVariant(mopso_ssmodel->populacja[i]->x[1]).toString()+" , "+QVariant(mopso_ssmodel->populacja[i]->x[2]).toString(),QColor(Qt::black));

        }
    }




    /////////////////////////////////////////
    gga_ssmodel=new GGA_for_ssmodel(30,32,0.01,0.7,6,{"rodz1","rodz2","rodz3"},{{0},{1},{2}},problem);
    //gga_ssmodel->vmax = 100.0;
    //qDebug()<<"il osob "<<gga_ssmodel->populacja.size()<<endl;
    gga_ssmodel->Inicjalizuj();
    //widget.qPlotNSGA_II_PID->clearGraphs();
    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {
        if(CheckStabilityOfMySystem(gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2],time,dt,dataXY);
            AddPlot(widget.qPlotGGA_PID,dataXY,""+QVariant(gga_ssmodel->rodzice[i]->x[0]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->x[1]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->x[2]).toString(),QColor(Qt::black));

        }
    }



    /////////////////////////////////////////
    nsga_ii_ssmodel=new NSGA_II_for_ssmodel(30,32,0.01,0.7,problem);
    //nsga_ii_ssmodel->vmax = 100.0;
    //qDebug()<<"il osob "<<nsga_ii_ssmodel->populacja.size()<<endl;
    nsga_ii_ssmodel->Inicjalizuj();
    //widget.qPlotNSGA_II_PID->clearGraphs();
    for (int i = 0; i < nsga_ii_ssmodel->rodzice.size(); ++i) {
        if(CheckStabilityOfMySystem(nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2],time,dt,dataXY);
            AddPlot(widget.qPlotNSGA_II_PID,dataXY,""+QVariant(nsga_ii_ssmodel->rodzice[i]->x[0]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[1]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[2]).toString(),QColor(Qt::black));

        }
    }


    //mopso_ssmodel.WyznaczWartFunkcjiKryterialnych();

}

Forma::~Forma() {
}

void Forma::Zaladuj_wzor(QWidget * parent, QString path) {
    QtMmlWidget* m_mml_widget = new QtMmlWidget(parent);

    //QString path="example.mml";


    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        showWarning("File error: Could not open \""
                    + path
                    + "\": " + file.errorString());
        return;
    }

    QTextStream stream(&file);
    //    stream.setEncoding(QTextStream::UnicodeUTF8);
    QString text = stream.readAll();
    file.close();






    QString error_msg;
    int error_line, error_column;
    bool result = m_mml_widget->setContent(text, &error_msg, &error_line,
                                           &error_column);

    if (!result) {
        showWarning("Parse error: line " + QString::number(error_line)
                    + ", col " + QString::number(error_column)
                    + ": " + error_msg);

    }



}

void Forma::Rysuj() {
    DINFO
            plot3d->rosenbrock->setDomain(
                QVariant(widget.xmin->text()).toFloat(NULL),
                QVariant(widget.xmax->text()).toFloat(NULL),
                QVariant(widget.ymin->text()).toFloat(NULL),
                QVariant(widget.ymax->text()).toFloat(NULL));

    // plot->rosenbrock->setMinZ(widget.fmin->text().toFloat(NULL));
    // plot->rosenbrock->setMaxZ(widget.fmax->text().toFloat(NULL));

    DINFO
            qDebug() << "ok " << widget.Funkcja->text().toAscii().data();
    plot3d->rosenbrock->parser.SetExpr(widget.Funkcja->text().toAscii().data());
    DINFO
            plot3d->setScale(
                QVariant(widget.scale_x->text()).toFloat(NULL),
                QVariant(widget.scale_y->text()).toFloat(NULL),
                QVariant(widget.scale_f->text()).toFloat(NULL));

    plot3d->setRotation(30, 0, 15);
    //    pso->czi = QVariant(widget.czi->text()).toFloat(NULL);
    //    pso->w = QVariant(widget.w->text()).toFloat(NULL);
    //
    //    // pso->maximize = false;
    //
    //    pso->parser.SetExpr(widget.Funkcja->text().toAscii().data());
    //
    //    pso->xmin = QVariant(widget.xmin->text()).toFloat(NULL);
    //    pso->xmax = QVariant(widget.xmax->text()).toFloat(NULL);
    //    pso->ymin = QVariant(widget.ymin->text()).toFloat(NULL);
    //    pso->ymax = QVariant(widget.ymax->text()).toFloat(NULL);
    //
    //    pso->vmax = 100.0;
    //    pso->c_p = 2.8;
    //    pso->c_g = 1.3;
    //
    //    pso->x_width = 10.0;
    //    pso->y_width = 10.0;


    DINFO;

    //    pso->fitness_gbest = pso->populacja[0]->fitness;
    //
    //    pso->gbest_pos[0] = pso->populacja[0]->pbest_pos[0];
    //    pso->gbest_pos[1] = pso->populacja[0]->pbest_pos[1];
    //
    //    plot3d->data_wek.clear();
    //
    //    for (int i = 0; i < pso->populacja.size(); i++) {
    //
    //        // plot->data_wek.push_back(new Point3d(pso->particles_vec[i]->x_pos[0],pso->particles_vec[i]->x_pos[1],pso->particles_vec[i]->fitness));
    //
    //    }
    //
    //
    //
    //
    //
    //



    //plot3d->rosenbrock->create();


    // plot3d->setHull();
    cout<<"hull x.min "<<plot3d->hull().minVertex.x<<endl;
    cout<<"hull x.max "<<plot3d->hull().maxVertex.x<<endl;
    cout<<"hull y.min "<<plot3d->hull().minVertex.y<<endl;
    cout<<"hull y.max "<<plot3d->hull().maxVertex.y<<endl;
    cout<<"hull z.min "<<plot3d->hull().minVertex.z<<endl;
    cout<<"hull z.max "<<plot3d->hull().maxVertex.z<<endl;
    ParallelEpiped hull(Triple(0,0,0),Triple(100,100,100));

    plot3d->createCoordinateSystem(hull);



    for (int i = 0; i < 30; ++i) {
        plot3d->data_wek.push_back(new Point3d(rand()%100,rand()%100,rand()%100));
    }

    for (unsigned i = 0; i != plot3d->coordinates()->axes.size(); ++i) {
        plot3d->coordinates()->axes[i].setMajors(7);
        plot3d->coordinates()->axes[i].setMinors(4);


    }




    plot3d->coordinates()->axes[X1].setLabelString("x-axis");
    plot3d->coordinates()->axes[Y1].setLabelString("y-axis");
    plot3d->coordinates()->axes[Z1].setLabelString("f(x,y)"); // Omega - see http://www.unicode.org/charts/

    //  plot3d->setCoordinateStyle(BOX);

    plot3d->updateData();
    plot3d->updateGL();
}

void Forma::sIterujMOPSO() {



}

void Forma::sStopMOPSO() {
    //mopso->koniec=true;
    mopso->stop();

}

void Forma::sDoneMOPSO() {
    // aktualizuj_wykres_przystosowania();

    // qDebug() << "iteracja " << mopso->num_iter << " z " << mopso->il_iter << "\n";
    //  qWarning("num it %i\n",mopso->num_iter);
    widget.progressBarMOPSO->setValue(mopso->num_iter);


    ///if (mopso->il_iter == mopso->num_iter) {

    //  }

}

void Forma::sFinishedMOPSO()
{
    qWarning("finished\n");
    widget.Debug->append(QString("[MOPSO]:Done ") + QString::number(mopso->num_iter) + QString(" iterations in ") + QString::number((float) time.elapsed() / 1000.0) + QString(" s."));

    
}

void Forma::Resetuj() {

    widget.Debug->clear();
    dane.clear();
    plot3d->data_wek.clear();
    mopso->Inicjalizuj();
    Wykres();
}

void Forma::Zmiana_funkcji() {
    switch (widget.Funkcje->currentIndex()) {
    case 0://Rastrigin
        widget.Funkcja->setText(QString("x*x-10*cos(2*_pi*x)+10+y*y-10*cos(2*_pi*y)"));
        widget.xmin->setText(QString("-6"));
        widget.xmax->setText(QString("6"));
        widget.ymin->setText(QString("-6"));
        widget.ymax->setText(QString("6"));
        widget.fmax->setText(QString("120"));
        break;
    case 1://Alpine
        widget.Funkcja->setText(QString("2.808*2.808-sin(x)*sin(y)*sqrt(x*y)"));
        widget.xmin->setText(QString("0"));
        widget.xmax->setText(QString("10"));
        widget.ymin->setText(QString("0"));
        widget.ymax->setText(QString("10"));
        widget.fmax->setText(QString("15"));
        break;
    case 2://Rosenbrock
        widget.Funkcja->setText(QString("abs((1-x)*(1-x)+100*(y-x*x)*(y-x*x))"));
        widget.xmin->setText(QString("-2"));
        widget.xmax->setText(QString("2"));
        widget.ymin->setText(QString("-2"));
        widget.ymax->setText(QString("2"));
        widget.fmax->setText(QString("4000"));
        break;
    case 3://cos(3*x*y)
        widget.Funkcja->setText(QString("cos(3*x*y)"));
        widget.xmin->setText(QString("-5"));
        widget.xmax->setText(QString("5"));
        widget.ymin->setText(QString("-5"));
        widget.ymax->setText(QString("5"));
        widget.fmax->setText(QString("2"));
        break;



    }
    Rysuj();


}

void Forma::Suwak_pionowy() {

    // qDebug()<<"suwak pionowy";
    plot3d->setRotation(widget.verticalScrollBar->value() - 180 + 30, 0, widget.horizontalScrollBar->value() - 180 + 15);


}

void Forma::Suwak_poziomy() {

    //qDebug()<<"suwak poziomy";
    plot3d->setRotation(widget.verticalScrollBar->value() - 180 + 30, 0, widget.horizontalScrollBar->value() - 180 + 15);



}

void Forma::Cube() {
    double x, y, f;
    x = mopso->xmax - mopso->xmin;
    y = mopso->ymax - mopso->ymin;
    f = QVariant(widget.fmax->text()).toFloat(NULL) - QVariant(widget.fmin->text()).toFloat(NULL);



    if (x >= y && x >= f) {

        plot3d->setScale(1, x / y, x / f);
    } else if (y >= x && y >= f) {

        plot3d->setScale(y / x, 1, y / f);

    } else if (f >= x && f >= y) {

        plot3d->setScale(f / x, f / y, 1);
    }

}

void Forma::Wykres() {
    qDebug() << "wykres\n";

    for (int i = 0; i < mopso->populacja.size(); i++) {

        //  plot3d->data_wek.push_back(new Point3d(pso->populacja[i]->x[0], pso->populacja[i]->x[1], pso->populacja[i]->fitness));

    }




    //    for (float i = -2; i < 2; i += 0.2) {
    //
    //        for (float j = -2; j < 2; j += 0.2) {
    //
    //            plot->rosenbrock->xVal = i;
    //            plot->rosenbrock->yVal = j;
    //            plot->data_wek.push_back(new Point3d(i, j, plot->rosenbrock->parser.Eval()));
    //
    //        }
    //
    //    }

    // plot->rosenbrock->create();
    //   plot3d->updateData();
    //   plot3d->updateGL();




}

void Forma::Wireframe() {
    plot3d->setPlotStyle(WIREFRAME);

    Rysuj();


}

void Forma::Dodaj_funkcje() {
    // widget.lista_funkcji->
    qDebug() << "w dodaj funkcje";
    qDebug() << "row count " << widget.lista_funkcji->rowCount();
    // qDebug()<<"text "<<widget.lista_funkcji->item(0, 0)->text();
    int row = widget.lista_funkcji->rowCount();
    widget.lista_funkcji->insertRow(row);
    QTableWidgetItem *item0 = new QTableWidgetItem;
    //    QTableWidgetItem *item1 = new QTableWidgetItem;
    //    QTableWidgetItem *item2 = new QTableWidgetItem;
    QButtonGroup* grupa = new QButtonGroup();
    QRadioButton* radio1 = new QRadioButton();
    QRadioButton* radio2 = new QRadioButton();
    grupa->addButton(radio1);
    grupa->addButton(radio2);
    radio1->setChecked(true);
    //    // item0->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    widget.lista_funkcji->setItem(row, 0, item0);
    // widget.lista_funkcji->setItem(row, 1, item1);
    //  widget.lista_funkcji->setItem(row, 2, item2);
    widget.lista_funkcji->setCellWidget(row, 1, radio1);
    widget.lista_funkcji->setCellWidget(row, 2, radio2);
    //widget.lista_funkcji->item(row, 0)->setText(QString("lol"));
}

void Forma::Usun_funkcje() {
    qDebug() << "w usun funkcje";
    int i = -1;
    for (short s = 0; s < widget.lista_funkcji->rowCount(); s++) {
        if (widget.lista_funkcji->item(s, 0)->isSelected()) {
            i = s;
        }
    }

    if (i >= 0) widget.lista_funkcji->removeRow(i);


}

void Forma::sDodaj_wszystkie_funkcje() {
    Dodaj_wszystkie_funkcje();
    Inicjalizuj_zmienne();




}

void Forma::Dodaj_wszystkie_funkcje() {
    problem = new Problem();
    problem->tab_minmax = new bool[widget.lista_funkcji->rowCount()];
    problem->cos = 666;
    qDebug() << "ilosc funkcji " << widget.lista_funkcji->rowCount() << "\n";
    for (int i = 0; i < widget.lista_funkcji->rowCount(); i++) {
        qDebug() << widget.lista_funkcji->item(i, 0)->text().toAscii().data() << " " << widget.lista_funkcji->item(i, 0)->text().length() << "\n";
        if (widget.lista_funkcji->item(i, 0)->text().length() == 0) {
            showWarning("Nie podano funkcji !!!");
            break;
        }
        qDebug() << "dodaje funkcje numer " << i;
        qDebug() << widget.lista_funkcji->item(i, 0)->text().toAscii().data();
        problem->funkcje.push_back(widget.lista_funkcji->item(i, 0)->text().toAscii().data());
        problem->parseryFunkcji.push_back(new mu::Parser());
        problem->parseryFunkcji[i]->SetVarFactory(&(problem->CCallback), problem);
        problem->parseryFunkcji[i]->SetExpr(widget.lista_funkcji->item(i, 0)->text().toAscii().data());
        try {

            problem->parseryFunkcji[i]->Eval();

        } catch (mu::Parser::exception_type &e) {
            qWarning() << "Message:  " << e.GetMsg().c_str() << "\n";
            qWarning() << "Formula:  " << e.GetExpr().c_str() << "\n";
            qWarning() << "Token:    " << e.GetToken().c_str() << "\n";
            qWarning() << "Position: " << e.GetPos() << "\n";
            qWarning() << "Errc:     " << e.GetCode() << "\n";

            showWarning("Funkcja " + QString::number(i + 1) + " jest niepoprawna");

            return;
        }


        if (((QRadioButton*) widget.lista_funkcji->cellWidget(i, 1))->isChecked())//oznacza minimalizacje
        {
            problem->tab_minmax[i] = false; //min
            qDebug() << "\ntab_minmax " << i << " = false\n";
        } else {
            problem->tab_minmax[i] = true; //max
            qDebug() << "\ntab_minmax " << i << " = true\n";
        }
        //funkcje[i]->GetUsedVar()
    }





    //sprawdzenie poprawnosci losowania metoda ruletki

    //    int* tab=new int[pso->hiperkostki.size()];
    //    memset(tab,0,pso->hiperkostki.size()*sizeof(int));
    //    int tmp;
    //    for (int i = 0; i < 100; i++) {
    //        tmp=pso->losuj_kostke_ruletka();
    //        tab[tmp]++;
    //        qDebug()<<"h_ind rand= "<<tmp<<"\n";
    //
    //
    //    }
    //
    //    for (int i = 0; i < pso->hiperkostki.size(); i++) {
    //        qDebug()<<"il wyl "<<i<<" rowna sie "<<tab[i]<<"\n";
    //
    //
    //    }


    //    for (int i = 0; i < problem->funkcje.size(); i++) {
    //       qDebug()<<"wartosc funkcji "<<i<<" wynosi "<< problem->funkcje[i]->Eval()<<"\n";
    //
    //
    //
    //    }


    //czysczenie tabel z funkcjami i rodzajnikami
    while(widget.treeWidget->topLevelItemCount())
    {
        delete widget.treeWidget->topLevelItem(0);
    }
    while(widget.functionTable->rowCount())
    {
        widget.functionTable->removeRow(0);
    }

    //dodanie funkcji do tabeli przydzialu funkcji do rodzajnika
    QTreeWidgetItem *item;
    for(unsigned int i=0;i< problem->funkcje.size();i++)
    {
        qDebug()<<"fun "<<i<<" "<<problem->funkcje[i]<<endl;
        widget.functionTable->insertRow(i);
        QTableWidgetItem *newitem = new QTableWidgetItem(problem->funkcje[i]);
        widget.functionTable->setItem(i, 0, newitem);
        mFunction[newitem]=i;

        item = new QTreeWidgetItem();
        item->setText(0, "Rodzajnik "+QVariant(i).toString());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        widget.treeWidget->insertTopLevelItem(i, item);

    }
    isFunctionInitialized=true;
}

void Forma::Dodaj_ograniczenie() {
    // widget.lista_funkcji->
    qDebug() << "w dodaj ograniczenie";
    qDebug() << "row count " << widget.lista_ograniczen->rowCount();
    // qDebug()<<"text "<<widget.lista_funkcji->item(0, 0)->text();
    int row = widget.lista_ograniczen->rowCount();
    widget.lista_ograniczen->insertRow(row);
    QTableWidgetItem *item0 = new QTableWidgetItem();
    QComboBox *item1 = new QComboBox();
    QStringList rodz_ograniczen;
    rodz_ograniczen << "<" << "<=" << "=" << ">=" << ">";
    item1->addItems(rodz_ograniczen);
    item1->setCurrentIndex(2);
    QTableWidgetItem *item2 = new QTableWidgetItem();
    widget.lista_ograniczen->setItem(row, 0, item0);
    widget.lista_ograniczen->setCellWidget(row, 1, item1);
    widget.lista_ograniczen->setItem(row, 2, item2);
    // widget.lista_funkcji->setItem(row, 1, item1);
    //  widget.lista_funkcji->setItem(row, 2, item2);


    //widget.lista_funkcji->item(row, 0)->setText(QString("lol"));


}

void Forma::Usun_ograniczenie() {
    qDebug() << "w usun funkcje";
    int i = -1;
    for (short s = 0; s < widget.lista_ograniczen->rowCount(); s++) {
        if (widget.lista_ograniczen->item(s, 0)->isSelected()) {
            i = s;
        }
    }

    if (i >= 0) widget.lista_ograniczen->removeRow(i);


}

void Forma::sDodaj_zakresy_zmiennych() {

    if(isFunctionInitialized)
    {
        Dodaj_zakresy_zmiennych();
    }
    else
    {
        showWarning(tr("Należy najpierw wczytać funkcje"));
    }

}

void Forma::Dodaj_zakresy_zmiennych() {
    bool ok;
    for (unsigned int ind_zmiennych = 0; ind_zmiennych < problem->zmienne.size(); ind_zmiennych++) {
        problem->zmienne[ind_zmiennych]->min = widget.lista_zmiennych->item(ind_zmiennych, 1)->text().toDouble(&ok);
        if (!ok) {
            showWarning("Blad konwersji minimalnej wartosci dla zmiennej o nazwie " + QString(problem->zmienne[ind_zmiennych]->nazwa.c_str()));
            flaga_bledu = true;
            return;

        }
        problem->zmienne[ind_zmiennych]->max = widget.lista_zmiennych->item(ind_zmiennych, 2)->text().toDouble(&ok);
        if (!ok) {
            showWarning("Blad konwersji maksymalnej wartosci dla zmiennej o nazwie " + QString(problem->zmienne[ind_zmiennych]->nazwa.c_str()));
            flaga_bledu = true;
            return;

        }
        if (problem->zmienne[ind_zmiennych]->min >= problem->zmienne[ind_zmiennych]->max) {
            showWarning("Wartość minimalna nie moze być >= wartosci maksymalnej !!!");
            flaga_bledu = true;
            return;


        }

    }
}

void Forma::sDodaj_wszystkie_ograniczenia() {
    for (int i = 0; i < widget.lista_ograniczen->rowCount(); i++) {
        Parser* parser0 = new mu::Parser();
        parser0->SetExpr(widget.lista_ograniczen->item(i, 0)->text().toAscii().data());
        parser0->SetVarFactory(&(problem->CCallback2), problem);
        try {

            parser0->Eval();

        } catch (mu::Parser::exception_type &e) {
            qWarning() << "Message:  " << e.GetMsg().c_str() << "\n";
            qWarning() << "Formula:  " << e.GetExpr().c_str() << "\n";
            qWarning() << "Token:    " << e.GetToken().c_str() << "\n";
            qWarning() << "Position: " << e.GetPos() << "\n";
            qWarning() << "Errc:     " << e.GetCode() << "\n";

            showWarning("Lewa strona ograniczenia " + QString::number(i + 1) + " jest niepoprawna !!!");
            return;
        }
        Parser* parser1 = new mu::Parser();
        parser1->SetExpr(widget.lista_ograniczen->item(i, 2)->text().toAscii().data());
        parser1->SetVarFactory(&(problem->CCallback2), problem);



        try {

            parser1->Eval();

        } catch (mu::Parser::exception_type &e) {
            qWarning() << "Message:  " << e.GetMsg().c_str() << "\n";
            qWarning() << "Formula:  " << e.GetExpr().c_str() << "\n";
            qWarning() << "Token:    " << e.GetToken().c_str() << "\n";
            qWarning() << "Position: " << e.GetPos() << "\n";
            qWarning() << "Errc:     " << e.GetCode() << "\n";

            showWarning("Prawa strona ograniczenia " + QString::number(i + 1) + " jest niepoprawna !!!");
            return;
        }



        qDebug() << "wybrany typ ograniczenia " << ((QComboBox*) widget.lista_ograniczen->cellWidget(i, 1))->currentIndex() << "\n";



        problem->ograniczenia.push_back(new Ograniczenie(parser0, ((QComboBox*) widget.lista_ograniczen->cellWidget(i, 1))->currentIndex(), parser1));
    }

}

void Forma::aktualizuj_wykres_przystosowania() {

    CurveData *data_dom = static_cast<CurveData *> (points_dom->data()); //zeby moc dodawac punkty na wykresie zdominowane i niezdominowane
    //  data->append(point);
    CurveData *data_nondom = static_cast<CurveData *> (points_nondom->data());
    //  data->append(point);


    data_dom->clear();
    data_nondom->clear();


    for (int i = 0; i < mopso->populacja.size(); i++) {
        if (mopso->populacja[i]->zdominowana) {

            data_dom->append(QPointF(mopso->populacja[i]->wartFunkcjiKryterialnych[0], mopso->populacja[i]->wartFunkcjiKryterialnych[1]));

        }
    }

    for (int i = 0; i < mopso->repozytorium.size(); i++) {


        data_nondom->append(QPointF(mopso->repozytorium[i]->wartFunkcjiKryterialnych[0], mopso->repozytorium[i]->wartFunkcjiKryterialnych[1]));



        // qDebug() << "i " << i << " : " << pso->repozytorium[i]->fitness[0] << " " << pso->repozytorium[i]->fitness[1] << " " << "\n";
    }


    //  qDebug() << "il nondom " << data_nondom->size() << "\n";

    plot2d->replot();


}

void Forma::aktualizuj_wykres_przystosowania(QVector< QVector<Solution*> > *F) {
    //qDebug()<<"il frontow "<<F->size()<<"\n";
    CurveData *data_dom = static_cast<CurveData *> (points_dom->data()); //zeby moc dodawac punkty na wykresie zdominowane i niezdominowane
    //  data->append(point);
    CurveData *data_nondom = static_cast<CurveData *> (points_nondom->data());
    //  data->append(point);


    data_dom->clear();
    data_nondom->clear();

    // DINFO;
    // qDebug()<<"il frontow "<<(*F).size()<<"\n";
    //  qDebug()<<"zdominowane ";
    for (int i = 1; i < (*F).size(); i++) {
        //  qDebug()<<"Front "<<i<<"\n";
        for (int j = 0; j < (*F)[i].size(); j++) {

            //  DINFO;
            // qDebug()<<(*F)[i][j]->przystosowaniePrzeskalowane[0]<<" "<<(*F)[i][j]->przystosowaniePrzeskalowane[1]<<"\n";
            // data_dom->append(QPointF((*F)[i][j]->przystosowaniePrzeskalowane[0],(*F)[i][j]->przystosowaniePrzeskalowane[1]));
            data_dom->append(QPointF((*F)[i][j]->wartFunkcjiKryterialnych[0], (*F)[i][j]->wartFunkcjiKryterialnych[1]));
        }


        //if (pso->populacja[i]->zdominowana) {


        // }


    }
    //  DINFO;
    //  qDebug()<<"Niezdominowane\n";
    for (int i = 0; i < (*F)[0].size(); i++) {

        //  qDebug()<<(*F)[0][i]->przystosowaniePrzeskalowane[0]<<" "<<(*F)[0][i]->przystosowaniePrzeskalowane[1]<<"\n";
        data_nondom->append(QPointF((*F)[0][i]->wartFunkcjiKryterialnych[0], (*F)[0][i]->wartFunkcjiKryterialnych[1]));



        // qDebug() << "i " << i << " : " << pso->repozytorium[i]->fitness[0] << " " << pso->repozytorium[i]->fitness[1] << " " << "\n";
    }


    //  qDebug() << "il nondom " << data_nondom->size() << "\n";
    //DINFO;
    plot2d->replot();

}


//void Forma::aktualizuj_wykres_gol()
//{
//      CurveData *data = static_cast<CurveData *> (plotGOL_data->data()); 
//      data->clear();
//      double gol=wskazniki->GOL(pso->repozytorium,problem);
//      vGol.push_back(QPointF((double)pso->num_iter,gol));
//      qDebug()<<"iter "<<pso->num_iter<<" GOL "<<gol<<"\n";
//      for (int i = 0; i < vGol.size(); i++) {
//        
//        data->append(vGol[i]);
//
//    }
//
//      
//    
//      plotGOL->replot();
//    
//}

void Forma::sInicjalizujMOPSO() {



}


void Forma::IterujRazMOPSO() {

    // qDebug()<<"--------------------------generacja "<<i<<"\n";



    //    qDebug() << "Aktualizuje predkosci\n";
    //    pso->aktualizuj_predkosci();
    //
    //    //  pso->wyswietl_populacje();
    //    qDebug() << "Aktualizuje pozycje\n";
    //    pso->aktualizuj_pozycje();
    //
    //    qDebug() << "Aktualizuje wartosci\n";
    //    pso->aktualizuj_wartosci();
    //
    //    if (pso->z_mutacja) {
    //        qDebug() << "Mutuje\n";
    //
    //        pso->mutuj(pso->max_iter, pso->num_iter, 0.5);
    //    }
    //    pso->num_iter++;
    //
    //    qDebug() << "Sprawdzam zdominowanie\n";
    //    pso->sprawdz_zdominowanie();
    //
    //    qDebug() << "Generuje kostki\n";
    //    pso->generuj_kostki();
    //
    //    qDebug() << "Aktualizuje repozytorium\n";
    //    pso->aktualizuj_repozytorium();
    //
    //    qDebug() << "Aktualizuje pbest\n";
    //    pso->aktualizuj_pbest();

    //  pso->wyswietl_populacje();
    //  pso->wyswietl_repozytorium();


    //wykresGOL->dodajPunkt(QPointF((double) mopso->num_iter, gol));
    // wykresGOL->aktualizuj();
    //aktualizuj_wykres_gol();


}

void Forma::createActions() {

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));


    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));


    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));



    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    settingsAct = new QAction(tr("Ustawienia"), this);

    settingsAct->setStatusTip(tr("Exit the application"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));


}

void Forma::createMenus() {

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    //    fileMenu->addAction(openAct);
    //
    fileMenu->addAction(saveAct);
    fileMenu->addAction(openAct);
    //
    //
    fileMenu->addSeparator();
    //
    fileMenu->addAction(exitAct);


    settingsMenu = menuBar()->addMenu(tr("&Settings"));
    settingsMenu->addAction(settingsAct);


}

void Forma::newFile() {
    // infoLabel->setText(tr("Invoked <b>File|New</b>"));


}

void Forma::open() {
    QFileDialog *dialog = new QFileDialog();

    QString str = QDir::currentPath();
    QString filename = dialog->getOpenFileName(this, tr("Open File"), str +
                                               "/zapiane/problem.prob",
                                               tr("Images (*.prob)"));

    qDebug("990\n");
    wczytajProblem(filename);
}

void Forma::save() {
    QFileDialog *dialog = new QFileDialog();


    QString str = QDir::currentPath();
    QString filename = dialog->getSaveFileName(this, tr("Save File"), str +
                                               "/zapiane/problem.prob",
                                               tr("Images (*.prob)"));


    zapiszProblem(filename);

    // graph->save(filename.remove(str + "/"));
    // infoLabel->setText(tr("Invoked <b>File|Save</b>"));
}

void Forma::exit() {


}

void Forma::settings() {


}

bool Forma::wczytajProblem(QString nazwa_pliku) {

    qDebug() << "wczytuje";

    QString nazwa, kolor;

    //  tab_funkcji = new QVector<Jednostka>[4];

    QFile file(nazwa_pliku);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for writing: "
                 << qPrintable(file.errorString()) << endl;
        return false;
    }
    QTextStream in(&file);
    QString co;
    int il;
    double liczba;
    qDebug() << "wczytuje funkcje\n";
    in >> co;
    in >> il;
    for (int i = widget.lista_funkcji->rowCount() - 1; i >= 0; i--)widget.lista_funkcji->removeRow(i);

    for (int i = 0; i < il; i++) {
        qDebug() << i << "\n";
        widget.lista_funkcji->insertRow(i);
        qDebug() << "po insert" << "\n";
        in >> co;
        qDebug() << co << "\n";
        QTableWidgetItem *item0 = new QTableWidgetItem;

        DINFO
                QButtonGroup* grupa = new QButtonGroup();
        QRadioButton* radio1 = new QRadioButton();
        QRadioButton* radio2 = new QRadioButton();
        grupa->addButton(radio1);
        grupa->addButton(radio2);
        radio1->setChecked(true);

        widget.lista_funkcji->setItem(i, 0, item0);
        widget.lista_funkcji->setCellWidget(i, 1, radio1);
        widget.lista_funkcji->setCellWidget(i, 2, radio2);

        widget.lista_funkcji->item(i, 0)->setText(co);
        DINFO

                in >> co;
        DINFO
                qDebug() << co << "\n";
        DINFO
                if (co.compare("min")) {
            ((QRadioButton*) widget.lista_funkcji->cellWidget(i, 1))->setChecked(true);

        } else {
            ((QRadioButton*) widget.lista_funkcji->cellWidget(i, 1))->setChecked(true);

        }

    }

    //funkcje
    qDebug() << "wczytuje zmienne\n";
    in >> co;
    in >> il;
    for (int i = widget.lista_zmiennych->rowCount() - 1; i >= 0; i--)widget.lista_zmiennych->removeRow(i);

    for (int i = 0; i < il; i++) {
        qDebug() << i << "\n";
        widget.lista_zmiennych->insertRow(i);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 0, item0);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 1, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 2, item2);

        in >> co;
        widget.lista_zmiennych->item(i, 0)->setText(co);

        in >> liczba;

        widget.lista_zmiennych->item(i, 1)->setText(QString::number(liczba));

        in >> liczba;

        widget.lista_zmiennych->item(i, 2)->setText(QString::number(liczba));


    }


    //ograniczenia

    qDebug() << "wczytuje ograniczenia\n";

    in >> co;
    in >> il;
    for (int i = widget.lista_ograniczen->rowCount() - 1; i >= 0; i--)widget.lista_ograniczen->removeRow(i);

    for (int i = 0; i < il; i++) {
        qDebug() << i << "\n";
        widget.lista_ograniczen->insertRow(i);


        QTableWidgetItem *item0 = new QTableWidgetItem();
        QComboBox *item1 = new QComboBox();
        QStringList rodz_ograniczen;
        rodz_ograniczen << "<" << "<=" << "=" << ">=" << ">";
        item1->addItems(rodz_ograniczen);

        QTableWidgetItem *item2 = new QTableWidgetItem();
        widget.lista_ograniczen->setItem(i, 0, item0);
        widget.lista_ograniczen->setCellWidget(i, 1, item1);
        widget.lista_ograniczen->setItem(i, 2, item2);
        in >> co;
        widget.lista_ograniczen->item(i, 0)->setText(co);

        in >> liczba;

        ((QComboBox*) widget.lista_ograniczen->cellWidget(i, 1))->setCurrentIndex((int) liczba);

        in >> co;

        widget.lista_ograniczen->item(i, 2)->setText(co);


    }



    // kolor_liczb=kolor;

    //    while (!in.atEnd()) {
    //
    //        in >> nazwa;
    //        qDebug() << nazwa;
    //        in >> prio;
    //        qDebug() << " " << prio;
    //        in >> param;
    //        qDebug() << " " << param ;
    //        in >> kolor;
    //        qDebug() << " " << kolor << "\n";
    //
    //        if (nazwa != QString("")) {
    //           // tab_funkcji[prio - 1].push_back(Jednostka(nazwa, prio, param, QColor(kolor)));
    //
    //        }
    //    }

    file.close();
    Dodaj_wszystkie_funkcje();
    Dodaj_zakresy_zmiennych();
    sDodaj_wszystkie_ograniczenia();



    return true;
}

bool Forma::zapiszProblem(QString nazwa_pliku) {
    QFile file(nazwa_pliku);
    if (!file.open(QIODevice::WriteOnly)) {
        //qDebug() << "Cannot open file for writing: "
        //     << qPrintable(file.errorString()) << endl;
        return false;
    }
    QTextStream out(&file);



    // tab_funkcji[table->item(row, 1)->data(0).toInt(NULL)]

    out << "[Funkcje]\n";
    // widget.lista_funkcji.si
    out << widget.lista_funkcji->rowCount() << "\n";
    for (int i = 0; i < widget.lista_funkcji->rowCount(); i++) {

        out << widget.lista_funkcji->item(i, 0)->text().toAscii().data();
        if (((QRadioButton*) widget.lista_funkcji->cellWidget(i, 1))->isChecked()) {
            out << " " << "min" << "\n";

        } else {
            out << " " << "max" << "\n";

        }
    }

    out << "[Zmienne]\n";
    out << widget.lista_zmiennych->rowCount() << "\n";
    for (int i = 0; i < widget.lista_zmiennych->rowCount(); i++) {

        out << widget.lista_zmiennych->item(i, 0)->text().toAscii().data() << " "
            << widget.lista_zmiennych->item(i, 1)->text().toAscii().data() << " "
            << widget.lista_zmiennych->item(i, 2)->text().toAscii().data() << "\n";






    }

    out << "[Ograniczenia]\n";
    out << widget.lista_ograniczen->rowCount() << "\n";
    for (int i = 0; i < widget.lista_ograniczen->rowCount(); i++) {

        out << widget.lista_ograniczen->item(i, 0)->text().toAscii().data() << " "
            << ((QComboBox*) widget.lista_ograniczen->cellWidget(i, 1))->currentIndex() << " "
            << widget.lista_ograniczen->item(i, 2)->text().toAscii().data() << "\n";

    }

    file.close();


    return true;

}


void Forma::Inicjalizuj_zmienne() {

    //wypelniam tabele ze zmiennymi
    for (int i = widget.lista_zmiennych->rowCount() - 1; i >= 0; i--)widget.lista_zmiennych->removeRow(i);

    for (unsigned int i = 0; i < problem->zmienne.size(); i++) {

        widget.lista_zmiennych->insertRow(i);
        QTableWidgetItem *item0 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 0, item0);
        QTableWidgetItem *item1 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 1, item1);
        QTableWidgetItem *item2 = new QTableWidgetItem;
        widget.lista_zmiennych->setItem(i, 2, item2);
        // widget.lista_funkcji->setItem(row, 1, item1);
        //  widget.lista_funkcji->setItem(row, 2, item2);
        widget.lista_zmiennych->item(i, 0)->setText(problem->zmienne[i]->nazwa.c_str());
        widget.lista_zmiennych->item(i, 1)->setText("-100");
        widget.lista_zmiennych->item(i, 2)->setText("100");
        //widget.lista_funkcji->setCellWidget(row, 1, radio1);
        //widget.lista_funkcji->setCellWidget(row, 2, radio2);


    }

    for (unsigned int i = 0; i < problem->zmienne.size(); i++) {

        problem->zmienne[i]->zmienna = 1.0;

    }

}

void Forma::Loguj() {
    // fprintf();



}

void Forma::AddRowToMOPSOPlotTable()
{
    if(!isFunctionInitialized)
    {
        showWarning("Funkcje nie zostaly wczytane");
        return;
    }


    QVector<QString> patternNames= { "None"     ///< no scatter symbols are drawn (e.g. data only represented with lines, see \ref setLineStyle)
                                     ,"Dot"       ///< a single pixel, \ref setScatterSize has no influence on its size.
                                     ,"Cross"     ///< a cross (x)
                                     ,"Plus"      ///< a plus (+)
                                     ,"Circle"    ///< a circle which is not filled
                                     ,"Disc"      ///< a circle which is filled with the color of the graph's pen (not the brush!)
                                     ,"Square"    ///< a square which is not filled
                                     ,"Star"      ///< a star with eight arms, i.e. a combination of cross and plus
                                     ,"Triangle"  ///< an equilateral triangle which is not filled, standing on baseline
                                     ,"TriangleInverted" ///< an equilateral triangle which is not filled, standing on corner
                                     ,"CrossSquare"      ///< a square which is not filled, with a cross inside
                                     ,"PlusSquare"       ///< a square which is not filled, with a plus inside
                                     ,"CrossCircle"      ///< a circle which is not filled, with a cross inside
                                     ,"PlusCircle"       ///< a circle which is not filled, with a plus inside
                                     ,"Peace"    ///< a circle which is not filled, with one vertical and two downward diagonal lines
                                     ,"Pixmap"    ///< a custom pixmap specified by setScatterPixmap, centered on the data point coordinates. \ref setScatterSize has no influence on its size.
                                   };


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

    QComboBox* Combcolor = new QComboBox;
    Combcolor->setMinimumWidth(40);
    Combcolor->setMaximumWidth(40);
    QPalette pal = Combcolor->palette(); // holds the widget's palette.
    pal.setColor(QPalette::Highlight, Qt::transparent); // construction of palette
    Combcolor->setPalette(pal); //set the comboBox palette

    int size = Combcolor->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size,size);

    int con=0;
    foreach (QString name, colorNms)
    {
        Combcolor->addItem("", QColor(con));//Adding ComboItems
        pixmap.fill(QColor(name));
        Combcolor->setItemData(con, pixmap, Qt::DecorationRole);//Setting color palettes
        con=con+1;

    }


    QComboBox* comboPattern = new QComboBox;

    foreach (QString name, patternNames)
    {
        comboPattern->addItem(name);//Adding ComboItems

    }

    QVector<QString> os1Names,os2Names,dataNames,rodzNames,indexNames;
    os1Names<<"x1"<<"x2";
    os2Names<<"x1"<<"x2";
    dataNames<<"wszystkie"<<"front pareto";



    // for (int i = 0; i < gga->funkcjeRodzajnikow.size(); ++i) {
    //dataNames<<"rozw. przydzielone rodzajnikowi "+QVariant(i+1).toString();
    // }
    rodzNames<<"fun. kryt."<<"param.";




    for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
        indexNames<<"f"+QVariant(i+1).toString();
    }


    QComboBox* comboOs1Rodz = new QComboBox();
    signalMapper = new QSignalMapper(this);


    foreach (QString name, rodzNames)
    {
        comboOs1Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs1Index = new QComboBox();

    foreach (QString name, indexNames)
    {
        comboOs1Index->addItem(name);//Adding ComboItems

    }


    QComboBox* comboOs2Rodz = new QComboBox;

    foreach (QString name, rodzNames)
    {
        comboOs2Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs2Index = new QComboBox;

    foreach (QString name, indexNames)
    {
        comboOs2Index->addItem(name);//Adding ComboItems

    }

    QComboBox* comboData = new QComboBox;



    foreach (QString name, dataNames)
    {
        comboData->addItem(name);//Adding ComboItems

    }




    //nazwa os1 os2 rodz_danych wzor kolor
    unsigned int row=widget.plotTableMOPSO->rowCount();
    widget.plotTableMOPSO->insertRow(row);

    //    QTableWidgetItem *colorItem = new QTableWidgetItem;
    //    colorItem->setData(Qt::DisplayRole, QColor("red"));

    //    QTableWidgetItem *os1Item = new QTableWidgetItem;
    //    os1Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *os2Item = new QTableWidgetItem;
    //    os2Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *daneItem = new QTableWidgetItem;
    //    daneItem->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *wzorItem = new QTableWidgetItem;
    //    wzorItem->setData(Qt::DisplayRole, "cos");

    QLineEdit* nameItem=new QLineEdit("Wykres");




    widget.plotTableMOPSO->setCellWidget(row, 0, nameItem);
    widget.plotTableMOPSO->setCellWidget(row, 1, comboOs1Rodz);
    widget.plotTableMOPSO->setCellWidget(row, 2, comboOs1Index);
    widget.plotTableMOPSO->setCellWidget(row, 3, comboOs2Rodz);
    widget.plotTableMOPSO->setCellWidget(row, 4, comboOs2Index);
    widget.plotTableMOPSO->setCellWidget(row, 5, comboData);
    widget.plotTableMOPSO->setCellWidget(row, 6, comboPattern);
    widget.plotTableMOPSO->setCellWidget(row,7,Combcolor);
    widget.plotTableMOPSO->resizeColumnToContents(0);
    widget.plotTableMOPSO->horizontalHeader()->setStretchLastSection(true);


    QObject* ac=new AuxClass(row,1);
    connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs1Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentMOPSOIndexChanged(QObject *)));

    ac=new AuxClass(row,3);
    connect(comboOs2Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs2Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentMOPSOIndexChanged(QObject *)));



}

void Forma::AddRowToGGAPlotTable()
{

    if(!isFunctionInitialized)
    {
        showWarning("Funkcje nie zostaly wczytane");
        return;
    }


    QVector<QString> patternNames= { "None"     ///< no scatter symbols are drawn (e.g. data only represented with lines, see \ref setLineStyle)
                                     ,"Dot"       ///< a single pixel, \ref setScatterSize has no influence on its size.
                                     ,"Cross"     ///< a cross (x)
                                     ,"Plus"      ///< a plus (+)
                                     ,"Circle"    ///< a circle which is not filled
                                     ,"Disc"      ///< a circle which is filled with the color of the graph's pen (not the brush!)
                                     ,"Square"    ///< a square which is not filled
                                     ,"Star"      ///< a star with eight arms, i.e. a combination of cross and plus
                                     ,"Triangle"  ///< an equilateral triangle which is not filled, standing on baseline
                                     ,"TriangleInverted" ///< an equilateral triangle which is not filled, standing on corner
                                     ,"CrossSquare"      ///< a square which is not filled, with a cross inside
                                     ,"PlusSquare"       ///< a square which is not filled, with a plus inside
                                     ,"CrossCircle"      ///< a circle which is not filled, with a cross inside
                                     ,"PlusCircle"       ///< a circle which is not filled, with a plus inside
                                     ,"Peace"    ///< a circle which is not filled, with one vertical and two downward diagonal lines
                                     ,"Pixmap"    ///< a custom pixmap specified by setScatterPixmap, centered on the data point coordinates. \ref setScatterSize has no influence on its size.
                                   };


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

    QComboBox* Combcolor = new QComboBox;
    Combcolor->setMinimumWidth(40);
    Combcolor->setMaximumWidth(40);
    QPalette pal = Combcolor->palette(); // holds the widget's palette.
    pal.setColor(QPalette::Highlight, Qt::transparent); // construction of palette
    Combcolor->setPalette(pal); //set the comboBox palette

    int size = Combcolor->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size,size);

    int con=0;
    foreach (QString name, colorNms)
    {
        Combcolor->addItem("", QColor(con));//Adding ComboItems
        pixmap.fill(QColor(name));
        Combcolor->setItemData(con, pixmap, Qt::DecorationRole);//Setting color palettes
        con=con+1;

    }


    QComboBox* comboPattern = new QComboBox;

    foreach (QString name, patternNames)
    {
        comboPattern->addItem(name);//Adding ComboItems

    }

    QVector<QString> os1Names,os2Names,dataNames,rodzNames,indexNames;
    os1Names<<"x1"<<"x2";
    os2Names<<"x1"<<"x2";
    dataNames<<"wszystkie"<<"front pareto";



    for (int i = 0; i < gga->funkcjeRodzajnikow.size(); ++i) {
        dataNames<<"rozw. przydzielone rodzajnikowi "+QVariant(i+1).toString();
    }
    rodzNames<<"fun. kryt."<<"param.";




    for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
        indexNames<<"f"+QVariant(i+1).toString();
    }


    QComboBox* comboOs1Rodz = new QComboBox();
    signalMapper = new QSignalMapper(this);


    foreach (QString name, rodzNames)
    {
        comboOs1Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs1Index = new QComboBox();

    foreach (QString name, indexNames)
    {
        comboOs1Index->addItem(name);//Adding ComboItems

    }


    QComboBox* comboOs2Rodz = new QComboBox;

    foreach (QString name, rodzNames)
    {
        comboOs2Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs2Index = new QComboBox;

    foreach (QString name, indexNames)
    {
        comboOs2Index->addItem(name);//Adding ComboItems

    }

    QComboBox* comboData = new QComboBox;



    foreach (QString name, dataNames)
    {
        comboData->addItem(name);//Adding ComboItems

    }




    //nazwa os1 os2 rodz_danych wzor kolor
    unsigned int row=widget.plotTableGGA->rowCount();
    widget.plotTableGGA->insertRow(row);

    //    QTableWidgetItem *colorItem = new QTableWidgetItem;
    //    colorItem->setData(Qt::DisplayRole, QColor("red"));

    //    QTableWidgetItem *os1Item = new QTableWidgetItem;
    //    os1Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *os2Item = new QTableWidgetItem;
    //    os2Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *daneItem = new QTableWidgetItem;
    //    daneItem->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *wzorItem = new QTableWidgetItem;
    //    wzorItem->setData(Qt::DisplayRole, "cos");

    QLineEdit* nameItem=new QLineEdit("Wykres");




    widget.plotTableGGA->setCellWidget(row, 0, nameItem);
    widget.plotTableGGA->setCellWidget(row, 1, comboOs1Rodz);
    widget.plotTableGGA->setCellWidget(row, 2, comboOs1Index);
    widget.plotTableGGA->setCellWidget(row, 3, comboOs2Rodz);
    widget.plotTableGGA->setCellWidget(row, 4, comboOs2Index);
    widget.plotTableGGA->setCellWidget(row, 5, comboData);
    widget.plotTableGGA->setCellWidget(row, 6, comboPattern);
    widget.plotTableGGA->setCellWidget(row,7,Combcolor);
    widget.plotTableGGA->resizeColumnToContents(0);
    widget.plotTableGGA->horizontalHeader()->setStretchLastSection(true);


    QObject* ac=new AuxClass(row,1);
    connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs1Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentGGAIndexChanged(QObject *)));

    ac=new AuxClass(row,3);
    connect(comboOs2Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs2Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentGGAIndexChanged(QObject *)));

}

void Forma::AddRowToNSGA_IIPlotTable()
{
    if(!isFunctionInitialized)
    {
        showWarning("Funkcje nie zostaly wczytane");
        return;
    }


    QVector<QString> patternNames= { "None"     ///< no scatter symbols are drawn (e.g. data only represented with lines, see \ref setLineStyle)
                                     ,"Dot"       ///< a single pixel, \ref setScatterSize has no influence on its size.
                                     ,"Cross"     ///< a cross (x)
                                     ,"Plus"      ///< a plus (+)
                                     ,"Circle"    ///< a circle which is not filled
                                     ,"Disc"      ///< a circle which is filled with the color of the graph's pen (not the brush!)
                                     ,"Square"    ///< a square which is not filled
                                     ,"Star"      ///< a star with eight arms, i.e. a combination of cross and plus
                                     ,"Triangle"  ///< an equilateral triangle which is not filled, standing on baseline
                                     ,"TriangleInverted" ///< an equilateral triangle which is not filled, standing on corner
                                     ,"CrossSquare"      ///< a square which is not filled, with a cross inside
                                     ,"PlusSquare"       ///< a square which is not filled, with a plus inside
                                     ,"CrossCircle"      ///< a circle which is not filled, with a cross inside
                                     ,"PlusCircle"       ///< a circle which is not filled, with a plus inside
                                     ,"Peace"    ///< a circle which is not filled, with one vertical and two downward diagonal lines
                                     ,"Pixmap"    ///< a custom pixmap specified by setScatterPixmap, centered on the data point coordinates. \ref setScatterSize has no influence on its size.
                                   };


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

    QComboBox* Combcolor = new QComboBox;
    Combcolor->setMinimumWidth(40);
    Combcolor->setMaximumWidth(40);
    QPalette pal = Combcolor->palette(); // holds the widget's palette.
    pal.setColor(QPalette::Highlight, Qt::transparent); // construction of palette
    Combcolor->setPalette(pal); //set the comboBox palette

    int size = Combcolor->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size,size);

    int con=0;
    foreach (QString name, colorNms)
    {
        Combcolor->addItem("", QColor(con));//Adding ComboItems
        pixmap.fill(QColor(name));
        Combcolor->setItemData(con, pixmap, Qt::DecorationRole);//Setting color palettes
        con=con+1;

    }


    QComboBox* comboPattern = new QComboBox;

    foreach (QString name, patternNames)
    {
        comboPattern->addItem(name);//Adding ComboItems

    }

    QVector<QString> os1Names,os2Names,dataNames,rodzNames,indexNames;
    os1Names<<"x1"<<"x2";
    os2Names<<"x1"<<"x2";
    dataNames<<"wszystkie"<<"front pareto";


    rodzNames<<"fun. kryt."<<"param.";


    for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
        indexNames<<"f"+QVariant(i+1).toString();
    }


    QComboBox* comboOs1Rodz = new QComboBox();
    signalMapper = new QSignalMapper(this);


    foreach (QString name, rodzNames)
    {
        comboOs1Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs1Index = new QComboBox();

    foreach (QString name, indexNames)
    {
        comboOs1Index->addItem(name);//Adding ComboItems

    }


    QComboBox* comboOs2Rodz = new QComboBox;

    foreach (QString name, rodzNames)
    {
        comboOs2Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs2Index = new QComboBox;

    foreach (QString name, indexNames)
    {
        comboOs2Index->addItem(name);//Adding ComboItems

    }

    QComboBox* comboData = new QComboBox;



    foreach (QString name, dataNames)
    {
        comboData->addItem(name);//Adding ComboItems

    }




    //nazwa os1 os2 rodz_danych wzor kolor
    unsigned int row=widget.plotTableNSGA_II->rowCount();
    widget.plotTableNSGA_II->insertRow(row);

    //    QTableWidgetItem *colorItem = new QTableWidgetItem;
    //    colorItem->setData(Qt::DisplayRole, QColor("red"));

    //    QTableWidgetItem *os1Item = new QTableWidgetItem;
    //    os1Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *os2Item = new QTableWidgetItem;
    //    os2Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *daneItem = new QTableWidgetItem;
    //    daneItem->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *wzorItem = new QTableWidgetItem;
    //    wzorItem->setData(Qt::DisplayRole, "cos");

    QLineEdit* nameItem=new QLineEdit("Wykres");




    widget.plotTableNSGA_II->setCellWidget(row, 0, nameItem);
    widget.plotTableNSGA_II->setCellWidget(row, 1, comboOs1Rodz);
    widget.plotTableNSGA_II->setCellWidget(row, 2, comboOs1Index);
    widget.plotTableNSGA_II->setCellWidget(row, 3, comboOs2Rodz);
    widget.plotTableNSGA_II->setCellWidget(row, 4, comboOs2Index);
    widget.plotTableNSGA_II->setCellWidget(row, 5, comboData);
    widget.plotTableNSGA_II->setCellWidget(row, 6, comboPattern);
    widget.plotTableNSGA_II->setCellWidget(row,7,Combcolor);
    widget.plotTableNSGA_II->resizeColumnToContents(0);
    widget.plotTableNSGA_II->horizontalHeader()->setStretchLastSection(true);


    QObject* ac=new AuxClass(row,1);
    connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs1Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentNSGA_IIIndexChanged(QObject*)));

    ac=new AuxClass(row,3);
    connect(comboOs2Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs2Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentNSGA_IIIndexChanged(QObject *)));

}
void Forma::AddPlot(QCustomPlot *qPlotNSGA_II_PID,QVector<QPair<double,double> > &dataXY,QString name,QColor color)
{
    qPlotNSGA_II_PID->addGraph();
    qPlotNSGA_II_PID->graph(qPlotNSGA_II_PID->graphCount()-1)->setPen(color);
    qPlotNSGA_II_PID->graph(qPlotNSGA_II_PID->graphCount()-1)->setLineStyle(QCPGraph::lsLine);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterStyle(QCPGraph::ssPlus);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterSize(4);
    qPlotNSGA_II_PID->graph(qPlotNSGA_II_PID->graphCount()-1)->setName(name);

    for(unsigned int i=0;i<dataXY.size();i++)
    {
        qPlotNSGA_II_PID->graph(qPlotNSGA_II_PID->graphCount()-1)->addData(dataXY[i].first,dataXY[i].second);

    }

    qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();
    qPlotNSGA_II_PID->rescaleAxes();
    qPlotNSGA_II_PID->replot();


}

void Forma::PlotStepResponse(ContinuousDynamicalSystem *sys,double Kp,double Ki,double Kd, double time, double dt)
{
    QVector<QPair<double,double> > dataXY;
    GetStepResponse(sys,Kp,Ki,Kd,time,dt,dataXY);
    //widget.qPlotNSGA_II_PID->clearGraphs();
    widget.qPlotNSGA_II_PID->addGraph();
    widget.qPlotNSGA_II_PID->graph(0)->setPen(QColor(255, 200, 0, 255));
    widget.qPlotNSGA_II_PID->graph(0)->setLineStyle(QCPGraph::lsLine);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterStyle(QCPGraph::ssPlus);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterSize(4);
    widget.qPlotNSGA_II_PID->graph(0)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));

    for(unsigned int i=0;i<dataXY.size();i++)
    {
        widget.qPlotNSGA_II_PID->graph(0)->addData(dataXY[i].first,dataXY[i].second);

    }

    widget.qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();
    widget.qPlotNSGA_II_PID->rescaleAxes();
    widget.qPlotNSGA_II_PID->replot();
}
void Forma::PlotImpulseResponse(ContinuousDynamicalSystem *sys, double Kp, double Ki, double Kd, double time, double dt)
{
    QVector<QPair<double,double> > dataXY;
    GetImpulseResponse(sys,Kp,Ki,Kd,time,dt,dataXY);
    widget.qPlotNSGA_II_PID->clearGraphs();
    widget.qPlotNSGA_II_PID->addGraph();
    widget.qPlotNSGA_II_PID->graph(0)->setPen(QColor(255, 200, 0, 255));
    widget.qPlotNSGA_II_PID->graph(0)->setLineStyle(QCPGraph::lsLine);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterStyle(QCPGraph::ssPlus);
    // widget.qPlotNSGA_II_PID->graph(0)->setScatterSize(4);
    widget.qPlotNSGA_II_PID->graph(0)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));

    for(unsigned int i=0;i<dataXY.size();i++)
    {
        widget.qPlotNSGA_II_PID->graph(0)->addData(dataXY[i].first,dataXY[i].second);

    }

    widget.qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();
    widget.qPlotNSGA_II_PID->rescaleAxes();
    widget.qPlotNSGA_II_PID->replot();
}

void Forma::PokazWszystkieFronty()
{
    QVector< QVector<Solution*> > * Fi=gga->FastNonDominatedSort();
    qDebug()<<"ilosc frontow "<<Fi->size()<<"\n";
    widget.qPlotNSGA_II_PID->clearGraphs();

    unsigned int tmp=255/(Fi->size()-1);
    for(unsigned int i=0;i<Fi->size();i++)
    {
        widget.qPlotNSGA_II_PID->addGraph();
        widget.qPlotNSGA_II_PID->graph(i)->setPen(QColor(255, tmp*i, 0, 255));
        widget.qPlotNSGA_II_PID->graph(i)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotNSGA_II_PID->graph(i)->setScatterStyle(QCP::ssPlus);
        widget.qPlotNSGA_II_PID->graph(i)->setScatterSize(4);
        widget.qPlotNSGA_II_PID->graph(i)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));

        for(unsigned int j=0;j<(*Fi)[i].size();j++)
        {
            widget.qPlotNSGA_II_PID->graph(i)->addData((*Fi)[i][j]->wartFunkcjiKryterialnych[0],(*Fi)[i][j]->wartFunkcjiKryterialnych[1]);

        }
    }
}

void Forma::AktualizujWykresParamGGA()
{
    qDebug()<<"aktualizuje wykres dla GGA";
    widget.qPlotNSGA_II_PID->graph(0)->clearData();
    widget.qPlotNSGA_II_PID->graph(1)->clearData();
    widget.qPlotNSGA_II_PID->graph(2)->clearData();
    float maksx,maksy;
    maksx=gga->rodzice[0]->x[0];
    maksy=gga->rodzice[0]->x[1];
    for( int i=0;i<gga->rodzice.size();i++)
    {

        if(gga->rodzice[i]->x[0]>maksx)
        {
            maksx=gga->rodzice[i]->x[0];
        }

        if(gga->rodzice[i]->x[1]>maksy)
        {
            maksy=gga->rodzice[i]->x[1];
        }



        if(gga->stopZdominowania[i]==0)
        {
            widget.qPlotNSGA_II_PID->graph(2)->addData(gga->rodzice[i]->x[0],gga->rodzice[i]->x[1]);
        }
        else
        {

            if(gga->przydzieloneRodzajniki[i]==0)
            {
                widget.qPlotNSGA_II_PID->graph(0)->addData(gga->rodzice[i]->x[0],gga->rodzice[i]->x[1]);
            }
            else
            {
                widget.qPlotNSGA_II_PID->graph(1)->addData(gga->rodzice[i]->x[0],gga->rodzice[i]->x[1]);
            }

        }
        qDebug()<<"p["<<i<<"] "<<gga->rodzice[i]->x[0]<<", "<<gga->rodzice[i]->x[1];
        qDebug()<<"rodzajnik "<<gga->przydzieloneRodzajniki[i];

    }







    // widget.qPlotNSGA_II_PID->graph(0)->rescaleAxes(true);


    //widget.qPlotNSGA_II_PID->xAxis->setRange(0,max(maksx,maksy));
    //widget.qPlotNSGA_II_PID->yAxis->setRange(0,max(maksx,maksy));


    widget.qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();

    widget.qPlotNSGA_II_PID->replot();
}

void Forma::changeGGAIndexSlot(int index, int row, int column)
{
    showWarning("trololo");
}

void Forma::currentGGAIndexChanged(QObject * ac)
{

    //showWarning(" "+QVariant(((AuxClass*)ac)->col).toString());

    int col=((AuxClass*)ac)->col+1;
    int row=((AuxClass*)ac)->row;
    //qDebug()<<"row "<<row<<" col "<<col;
    //problem->funkcje.size()
    QComboBox* cb=((QComboBox*) widget.plotTableGGA->cellWidget(row, col));

    //qDebug()<<"1791 "<<cb->count();
    while(cb->count()!=0)
    {
        cb->removeItem(0);
    }
    //cb->clear();
    // qDebug()<<"1793";
    //qDebug()<<"current index "<<cb->currentIndex();
    if(((QComboBox*) widget.plotTableGGA->cellWidget(row, col-1))->currentIndex()==0)
    {

        for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
            cb->addItem("f"+QVariant(i+1).toString());
        }
    }
    else
    {
        for (unsigned int i = 0; i < problem->zmienne.size(); ++i) {
            cb->addItem( QString(problem->zmienne[i]->nazwa.c_str()));
        }

    }



}


void Forma::currentPlot2DIndexChanged(QObject * ac)
{

    //showWarning(" "+QVariant(((AuxClass*)ac)->col).toString());

    int col=((AuxClass*)ac)->col+1;
    int row=((AuxClass*)ac)->row;
    //qDebug()<<"row "<<row<<" col "<<col;
    //problem->funkcje.size()
    QComboBox* cb=((QComboBox*) widget.wykresy2D->cellWidget(row, col));

    //qDebug()<<"1791 "<<cb->count();
    while(cb->count()!=0)
    {
        cb->removeItem(0);
    }
    //cb->clear();
    // qDebug()<<"1793";
    //qDebug()<<"current index "<<cb->currentIndex();
    if(((QComboBox*) widget.wykresy2D->cellWidget(row, col-1))->currentIndex()==0)
    {

        for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
            cb->addItem("f"+QVariant(i+1).toString());
        }
    }
    else
    {
        for (unsigned int i = 0; i < problem->zmienne.size(); ++i) {
            cb->addItem( QString(problem->zmienne[i]->nazwa.c_str()));
        }

    }



}

void Forma::currentPlot3DIndexChanged(QObject * ac)
{

    //showWarning(" "+QVariant(((AuxClass*)ac)->col).toString());

    int col=((AuxClass*)ac)->col+1;
    int row=((AuxClass*)ac)->row;
    //qDebug()<<"row "<<row<<" col "<<col;
    //problem->funkcje.size()
    QComboBox* cb=((QComboBox*) widget.wykresy3D->cellWidget(row, col));

    //qDebug()<<"1791 "<<cb->count();
    while(cb->count()!=0)
    {
        cb->removeItem(0);
    }
    //cb->clear();
    // qDebug()<<"1793";
    //qDebug()<<"current index "<<cb->currentIndex();
    if(((QComboBox*) widget.wykresy3D->cellWidget(row, col-1))->currentIndex()==0)
    {

        for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
            cb->addItem("f"+QVariant(i+1).toString());
        }
    }
    else
    {
        for (unsigned int i = 0; i < problem->zmienne.size(); ++i) {
            cb->addItem( QString(problem->zmienne[i]->nazwa.c_str()));
        }

    }



}
void Forma::currentMOPSOIndexChanged(QObject * ac)
{

    //showWarning(" "+QVariant(((AuxClass*)ac)->col).toString());

    int col=((AuxClass*)ac)->col+1;
    int row=((AuxClass*)ac)->row;
    //qDebug()<<"row "<<row<<" col "<<col;
    //problem->funkcje.size()
    QComboBox* cb=((QComboBox*) widget.plotTableMOPSO->cellWidget(row, col));

    //qDebug()<<"1791 "<<cb->count();
    while(cb->count()!=0)
    {
        cb->removeItem(0);
    }
    //cb->clear();
    // qDebug()<<"1793";
    //qDebug()<<"current index "<<cb->currentIndex();
    if(((QComboBox*) widget.plotTableMOPSO->cellWidget(row, col-1))->currentIndex()==0)
    {

        for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
            cb->addItem("f"+QVariant(i+1).toString());
        }
    }
    else
    {
        for (unsigned int i = 0; i < problem->zmienne.size(); ++i) {
            cb->addItem( QString(problem->zmienne[i]->nazwa.c_str()));
        }

    }



}

void Forma::currentNSGA_IIIndexChanged(QObject * ac)
{

    //showWarning(" "+QVariant(((AuxClass*)ac)->col).toString());

    int col=((AuxClass*)ac)->col+1;
    int row=((AuxClass*)ac)->row;
    //qDebug()<<"row "<<row<<" col "<<col;
    //problem->funkcje.size()
    QComboBox* cb=((QComboBox*) widget.plotTableNSGA_II->cellWidget(row, col));

    //qDebug()<<"1791 "<<cb->count();
    while(cb->count()!=0)
    {
        cb->removeItem(0);
    }
    //cb->clear();
    // qDebug()<<"1793";
    //qDebug()<<"current index "<<cb->currentIndex();
    if(((QComboBox*) widget.plotTableNSGA_II->cellWidget(row, col-1))->currentIndex()==0)
    {

        for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
            cb->addItem("f"+QVariant(i+1).toString());
        }
    }
    else
    {
        for (unsigned int i = 0; i < problem->zmienne.size(); ++i) {
            cb->addItem( QString(problem->zmienne[i]->nazwa.c_str()));
        }

    }



}
void Forma::AktualizujWykresFunKrytGGA()
{


    qDebug()<<"aktualizuje wykres dla GGA";
    widget.qPlotNSGA_II_PID->graph(0)->clearData();
    widget.qPlotNSGA_II_PID->graph(1)->clearData();
    widget.qPlotNSGA_II_PID->graph(2)->clearData();
    float maksx,maksy;
    maksx=gga->rodzice[0]->wartFunkcjiKryterialnych[0];
    maksy=gga->rodzice[0]->wartFunkcjiKryterialnych[1];
    for( int i=0;i<gga->rodzice.size();i++)
    {

        if(gga->rodzice[i]->wartFunkcjiKryterialnych[0]>maksx)
        {
            maksx=gga->rodzice[i]->wartFunkcjiKryterialnych[0];
        }

        if(gga->rodzice[i]->wartFunkcjiKryterialnych[1]>maksy)
        {
            maksy=gga->rodzice[i]->wartFunkcjiKryterialnych[1];
        }



        if(gga->stopZdominowania[i]==0)
        {
            widget.qPlotNSGA_II_PID->graph(2)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
        }
        else
        {

            if(gga->przydzieloneRodzajniki[i]==0)
            {
                widget.qPlotNSGA_II_PID->graph(0)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
            }
            else
            {
                widget.qPlotNSGA_II_PID->graph(1)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
            }

        }
        qDebug()<<"p["<<i<<"] "<<gga->rodzice[i]->wartFunkcjiKryterialnych[0]<<", "<<gga->rodzice[i]->wartFunkcjiKryterialnych[1];
        qDebug()<<"rodzajnik "<<gga->przydzieloneRodzajniki[i];

    }







    // widget.qPlotNSGA_II_PID->graph(0)->rescaleAxes(true);


    widget.qPlotNSGA_II_PID->xAxis->setRange(0,max(maksx,maksy));
    widget.qPlotNSGA_II_PID->yAxis->setRange(0,max(maksx,maksy));


    widget.qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    widget.qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();

    widget.qPlotNSGA_II_PID->replot();
}


void Forma::sInicjalizujNSGA() {


}

void Forma::sIterujNSGA() {
    //    DINFO;
    //
    //
    //    nsga->Iteruj();
    //    qDebug() << "il frontow " << (*nsga->F).size() << "\n";
    //    qDebug() << "size of F = " << nsga->F->size() << "\n";
    //
    //    aktualizuj_wykres_przystosowania(nsga->F);
    //
    //
    qDebug() << "nsga start";
    bool ok;
    unsigned int il_iter = widget.ilIterNSGA_II->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwa ilosc iteracji");
        return;
    }
    widget.Debug->append(QString("il iteracji ") + QString::number(il_iter));
    qDebug() << "il iteracji " << il_iter << "\n";
    nsga_ii->il_iter = il_iter;
    widget.progressBarNSGA_II->setMaximum(il_iter);
    time.start();
    //
    //    QThreadEx nsgaThread;
    //    nsga->moveToThread(&nsgaThread);
    //
    //    (*nsga).connect(&nsgaThread,
    //            SIGNAL(started()),
    //            SLOT(doTheWork()));
    //
    //    nsgaThread.connect(&nsgaThread,
    //                           SIGNAL(finished()),
    //                           SLOT(quit()));
    nsga_ii->start();

    //  nsgaThread.start();
    // nsgaThread.wait(5000);

}

void Forma::sIterujRazNSGA() {
    nsga_ii->Iteruj();
}

void Forma::sDoneNSGA() {

    // aktualizuj_wykres_przystosowania(nsga_ii->F);

    //qDebug() << "iteracja " << nsga_ii->num_iter << " z " << nsga_ii->il_iter << "\n";
    widget.progressBarNSGA_II->setValue(nsga_ii->num_iter);

    //  widget.Debug->append(QString("nsga_ii->num_iter") + QString::number(nsga_ii->num_iter));
    // if (nsga_ii->il_iter == nsga_ii->num_iter) {


    // }

}

void Forma::sFinishedNSGA()
{       qWarning("finished\n");
        widget.Debug->append(QString("[NSGA_II]:Done ") + QString::number(nsga_ii->num_iter) + QString(" iterations in ") + QString::number((float) time.elapsed() / 1000.0) + QString(" s."));

}

void Forma::sInicjalizujGGA()
{



}

void Forma::sIterujGGA()
{
}

void Forma::sIterujRazGGA()
{

}


void Forma::sStopGGA()
{
}

void Forma::sDoneGGA()
{
}

void Forma::sFinishedGGA()
{
}

void Forma::sAttachFunToGender()
{
    qDebug()<<widget.treeWidget->currentItem()->parent()<<endl;
    qDebug()<<widget.treeWidget->currentIndex()<<endl;
    qDebug()<< widget.functionTable->currentRow()<<endl;
    qDebug()<<"widget.functionTable->currentItem(); "<<widget.functionTable->currentItem()<<endl;
    if(widget.treeWidget->currentItem()->parent()==0 && widget.functionTable->currentRow()>=0)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        qDebug()<<"address "<<item<<endl;
        item->setText(0, problem->funkcje[mFunction[widget.functionTable->item(widget.functionTable->currentRow(),0)]]);
        mFunctionAttachedToGender[item]=mFunction[widget.functionTable->item(widget.functionTable->currentRow(),0)];
        qDebug()<<"mFunction["<<widget.functionTable->currentRow()<<"] "<<mFunction[widget.functionTable->item(widget.functionTable->currentRow(),0)]<<endl;
        mFunction.remove(widget.functionTable->item(widget.functionTable->currentRow(),0));
        //   widget.treeWidget->insertTopLevelItem(0, item);

        widget.treeWidget->currentItem()->addChild(item);
        widget.treeWidget->currentItem()->setExpanded(true);
        widget.functionTable->removeRow(widget.functionTable->currentRow());
        widget.functionTable->clearSelection();
        widget.functionTable->setCurrentCell(-1,-1);

    }

}

void Forma::sRemoveFun()
{
    if(widget.treeWidget->currentItem()->parent()!=0 )
    {
        QTableWidgetItem *item = new QTableWidgetItem(problem->funkcje[mFunctionAttachedToGender[widget.treeWidget->currentItem()]]);
        mFunction[item]=mFunctionAttachedToGender[widget.treeWidget->currentItem()];
        mFunctionAttachedToGender.remove(  widget.treeWidget->currentItem());
        widget.treeWidget->currentItem()->parent()->removeChild( widget.treeWidget->currentItem());
        widget.functionTable->insertRow( widget.functionTable->rowCount());
        widget.functionTable->setItem(widget.functionTable->rowCount()-1,0,item);
    }
}

void Forma::sRemoveAllFun()
{
    qDebug()<<"top level item "<<widget.treeWidget->topLevelItemCount()<<"\n";
    for(int i=0;i<widget.treeWidget->topLevelItemCount();++i)
    {
        qDebug()<<"num item "<<widget.treeWidget->topLevelItem(i)->childCount()<<"\n";
        for(unsigned int j=0;j<widget.treeWidget->topLevelItem(i)->childCount();j++)
        {

            QTableWidgetItem *item = new QTableWidgetItem(problem->funkcje[mFunctionAttachedToGender[widget.treeWidget->topLevelItem(i)->child(j)]]);
            mFunction[item]=mFunctionAttachedToGender[widget.treeWidget->topLevelItem(i)->child(j)];
            mFunctionAttachedToGender.remove(widget.treeWidget->topLevelItem(i)->child(j));
            widget.treeWidget->topLevelItem(i)->child(j)->parent()->removeChild(widget.treeWidget->topLevelItem(i)->child(j));
            widget.functionTable->insertRow( widget.functionTable->rowCount());
            widget.functionTable->setItem(widget.functionTable->rowCount()-1,0,item);

            j=j-1;

        }
    }
}

void Forma::sStopNSGA() {
    widget.Debug->append(QString("Stop"));
    nsga_ii->stop();
}

void Forma::AddGGADataToGraph(QCPGraph* graph,QVector<AxisSetting>* as,Solution* s,float& a,float& b)
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



void Forma::AddMOPSODataToGraph(QCPGraph* graph,QVector<AxisSetting>* as,Particle* s,float& a,float& b)
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

void Forma::UpdateAllPlots()
{
    for(unsigned int i=0;i<plotSettingsGGA.size();i++)
    {
        QCustomPlot* qp;
        QVector<AxisSetting>* as;
        qp=plotSettingsGGA[i].qPlot;
        TRACE;
        qp->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        qp->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qp->setInteraction(QCustomPlot::iSelectPlottables); //
        float a,b,minX,maxX,minY,maxY;
        minX=numeric_limits<float>::max();
        maxX=numeric_limits<float>::min();
        minY=numeric_limits<float>::max();
        maxY=numeric_limits<float>::min();
        for (int j = 0; j <  plotSettingsGGA[i].graphSettings.size(); ++j) {
            qp->graph(j)->clearData();

            as=&plotSettingsGGA[i].graphSettings[j].axisSettings;
            if(plotSettingsGGA[i].graphSettings[j].all)
            {
                for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                    AddDataTo2DGraph(qp->graph(j),as,gga->rodzice[iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;
                }
            }
            else if(plotSettingsGGA[i].graphSettings[j].pareto)//tylko front pareto
            {
                for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                    if(gga->stopZdominowania[iSol]==0)
                    {
                        AddDataTo2DGraph(qp->graph(j),as,gga->rodzice[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                    }
                }
            }
            else// wysw osob przypisane do rodzajnika
            {
                for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                    if(gga->przydzieloneRodzajniki[iSol]==plotSettingsGGA[i].graphSettings[i].indexOfGender)
                    {
                        AddDataTo2DGraph(qp->graph(i),as,gga->rodzice[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                    }
                }

            }

        }

        qp->xAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->yAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->replot();
    }


    for(unsigned int i=0;i<plotSettingsMOPSO.size();i++)
    {
        QCustomPlot* qp;
        QVector<AxisSetting>* as;

        qp=plotSettingsMOPSO[i].qPlot;
        TRACE;
        qp->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        qp->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qp->setInteraction(QCustomPlot::iSelectPlottables); //

        float a,b,minX,maxX,minY,maxY;
        minX=numeric_limits<float>::max();
        maxX=numeric_limits<float>::min();
        minY=numeric_limits<float>::max();
        maxY=numeric_limits<float>::min();
        for (int j = 0; j <  plotSettingsMOPSO[i].graphSettings.size(); ++j) {
            qp->graph(j)->clearData();

            as=&plotSettingsMOPSO[i].graphSettings[j].axisSettings;
            if(plotSettingsMOPSO[i].graphSettings[j].all)
            {
                for (int iSol = 0; iSol < mopso->populacja.size(); ++iSol) {

                    AddDataTo2DGraph(qp->graph(j),as,mopso->populacja[iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;
                }

                for (int iSol = 0; iSol < mopso->repozytorium.size(); ++iSol) {
                    // if(!mopso->repozytorium[iSol]->zdominowana)
                    //{
                    AddDataTo2DGraph(qp->graph(j),as,mopso->repozytorium[iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;
                    // }
                }
            }
            else if(plotSettingsMOPSO[i].graphSettings[j].pareto)//tylko front pareto
            {
                for (int iSol = 0; iSol < mopso->repozytorium.size(); ++iSol) {
                    // if(!mopso->repozytorium[iSol]->zdominowana)
                    //{
                    AddDataTo2DGraph(qp->graph(j),as,mopso->repozytorium[iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;
                    // }
                }
            }


        }

        qp->xAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->yAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->replot();
    }

    for(unsigned int i=0;i<plotSettingsNSGA_II.size();i++)
    {
        QCustomPlot* qp;
        QVector<AxisSetting>* as;

        qp=plotSettingsNSGA_II[i].qPlot;
        TRACE;
        qp->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        qp->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qp->setInteraction(QCustomPlot::iSelectPlottables); //

        float a,b,minX,maxX,minY,maxY;
        minX=numeric_limits<float>::max();
        maxX=numeric_limits<float>::min();
        minY=numeric_limits<float>::max();
        maxY=numeric_limits<float>::min();
        for (int j = 0; j <  plotSettingsNSGA_II[i].graphSettings.size(); ++j) {
            qp->graph(j)->clearData();

            as=&plotSettingsNSGA_II[i].graphSettings[j].axisSettings;
            if(plotSettingsNSGA_II[i].graphSettings[j].all)
            {
                for (int iSol = 0; iSol < nsga_ii->rodzice.size(); ++iSol) {

                    AddDataTo2DGraph(qp->graph(j),as,nsga_ii->rodzice[iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;
                }
            }
            else if(plotSettingsNSGA_II[i].graphSettings[j].pareto)//tylko front pareto
            {
                for (int iSol = 0; iSol < (*nsga_ii->F)[0].size(); ++iSol) {

                    AddDataTo2DGraph(qp->graph(j),as,(*nsga_ii->F)[0][iSol],a,b);
                    if(a<minX)minX=a;
                    if(b<minY)minY=b;
                    if(a>maxX)maxX=a;
                    if(b>maxY)maxY=b;

                }
            }


        }

        qp->xAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->yAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->replot();
    }


}

void Forma::Update2DPlots()
{
    for(unsigned int i=0;i<plot2DSettings.size();i++)
    {
        QCustomPlot* qp;
        QVector<AxisSetting>* as;
        qp=plot2DSettings[i].qPlot;
        TRACE;
        qp->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        qp->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qp->setInteraction(QCustomPlot::iSelectPlottables); //
        float a,b,minX,maxX,minY,maxY;
        minX=numeric_limits<float>::max();
        maxX=numeric_limits<float>::min();
        minY=numeric_limits<float>::max();
        maxY=numeric_limits<float>::min();
        for (int j = 0; j <  plot2DSettings[i].graphSettings.size(); ++j) {
            qp->graph(j)->clearData();

            as=&plot2DSettings[i].graphSettings[j].axisSettings;

            if(plot2DSettings[i].graphSettings[j].algorithm==2)//for GGA
            {
                if(plot2DSettings[i].graphSettings[j].all)
                {
                    for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                        AddGGADataToGraph(qp->graph(j),as,gga->rodzice[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                    }
                }
                else if(plot2DSettings[i].graphSettings[j].pareto)//tylko front pareto
                {
                    for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                        if(gga->stopZdominowania[iSol]==0)
                        {
                            AddGGADataToGraph(qp->graph(j),as,gga->rodzice[iSol],a,b);
                            if(a<minX)minX=a;
                            if(b<minY)minY=b;
                            if(a>maxX)maxX=a;
                            if(b>maxY)maxY=b;
                        }
                    }
                }
                else// wysw osob przypisane do rodzajnika
                {
                    for (int iSol = 0; iSol < gga->rodzice.size(); ++iSol) {
                        if(gga->przydzieloneRodzajniki[iSol]==plot2DSettings[i].graphSettings[i].indexOfGender)
                        {
                            AddGGADataToGraph(qp->graph(i),as,gga->rodzice[iSol],a,b);
                            if(a<minX)minX=a;
                            if(b<minY)minY=b;
                            if(a>maxX)maxX=a;
                            if(b>maxY)maxY=b;
                        }
                    }

                }
            }


            if(plot2DSettings[i].graphSettings[j].algorithm==0)//for MOPSO
            {
                if(plot2DSettings[i].graphSettings[j].all)
                {
                    for (int iSol = 0; iSol < mopso->populacja.size(); ++iSol) {

                        AddDataTo2DGraph(qp->graph(j),as,mopso->populacja[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                    }

                    for (int iSol = 0; iSol < mopso->repozytorium.size(); ++iSol) {
                        // if(!mopso->repozytorium[iSol]->zdominowana)
                        //{
                        AddDataTo2DGraph(qp->graph(j),as,mopso->repozytorium[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                        // }
                    }
                }
                else if(plot2DSettings[i].graphSettings[j].pareto)//tylko front pareto
                {
                    for (int iSol = 0; iSol < mopso->repozytorium.size(); ++iSol) {
                        // if(!mopso->repozytorium[iSol]->zdominowana)
                        //{
                        AddDataTo2DGraph(qp->graph(j),as,mopso->repozytorium[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                        // }
                    }
                }

            }

            if(plot2DSettings[i].graphSettings[j].algorithm==1)//for NSGA_II
            {
                if(plot2DSettings[i].graphSettings[j].all)
                {
                    for (int iSol = 0; iSol < nsga_ii->rodzice.size(); ++iSol) {

                        AddDataTo2DGraph(qp->graph(j),as,nsga_ii->rodzice[iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;
                    }
                }
                else if(plot2DSettings[i].graphSettings[j].pareto)//tylko front pareto
                {
                    for (int iSol = 0; iSol < (*nsga_ii->F)[0].size(); ++iSol) {

                        AddDataTo2DGraph(qp->graph(j),as,(*nsga_ii->F)[0][iSol],a,b);
                        if(a<minX)minX=a;
                        if(b<minY)minY=b;
                        if(a>maxX)maxX=a;
                        if(b>maxY)maxY=b;

                    }
                }

            }

        }

        qp->xAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->yAxis->setRange(min(minX,minY),max(maxX,maxY));
        qp->replot();
    }


}

void Forma::Update3DPlots()
{

}

void Forma::on_stworzWykresyGGA_clicked()
{
    //widget.Wykresy->clear();

    if(widget.plotTableGGA->rowCount()==0)
    {
        showWarning(tr("Należy dodać przynajmniej 1 wykres"));
        return;
    }

    QWidget* w=new QWidget;
    QCustomPlot* qPlot=new QCustomPlot(w);
    qPlot->setFixedWidth(550);
    qPlot->setFixedHeight(770);
    qPlot->setAutoMargin(false);
    qPlot->setMargin(50,10,10,230);


    //plotsSettingsGGA.clear();
    plotSettingsGGA.push_back(Plot2DSetting());
    plotSettingsGGA.back().qPlot=qPlot;
    plotSettingsGGA.back().tabIndex=widget.Wykresy->count()-1;
    widget.Wykresy->addTab(w,"GGA");


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";


    TRACE;

    int tmp;

    for (int i = 0; i < widget.plotTableGGA->rowCount(); ++i) {
        plotSettingsGGA.back().graphSettings.push_back(GraphSetting());
        plotSettingsGGA.back().graphSettings.back().axisSettings.resize(2);
        plotSettingsGGA.back().graphSettings.back().name=((QLineEdit*) widget.plotTableGGA->cellWidget(i, 0))->text();

        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 1))->currentIndex();

        if(tmp==0)
        {
            plotSettingsGGA.back().graphSettings.back().axisSettings[0].isParam=false;
        }
        else
        {
            plotSettingsGGA.back().graphSettings.back().axisSettings[0].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 2))->currentIndex();

        plotSettingsGGA.back().graphSettings.back().axisSettings[0].index=tmp;

        /////

        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 3))->currentIndex();

        if(tmp==0)
        {
            plotSettingsGGA.back().graphSettings.back().axisSettings[1].isParam=false;
        }
        else
        {
            plotSettingsGGA.back().graphSettings.back().axisSettings[1].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 4))->currentIndex();

        plotSettingsGGA.back().graphSettings.back().axisSettings[1].index=tmp;

        //////
        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 5))->currentIndex();

        if(tmp<2)
        {
            if(tmp==0)
            {
                plotSettingsGGA.back().graphSettings.back().all=true;
            }
            else
            {
                plotSettingsGGA.back().graphSettings.back().pareto=true;
            }
        }
        else
        {
            tmp=tmp-2;
            plotSettingsGGA.back().graphSettings.back().indexOfGender=tmp;


        }

        QString str=((QComboBox*) widget.plotTableGGA->cellWidget(i, 6))->currentText();
        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 6))->currentIndex();
        plotSettingsGGA.back().graphSettings.back().patternName=str;
        plotSettingsGGA.back().graphSettings.back().patternIndex=tmp;

        tmp=((QComboBox*) widget.plotTableGGA->cellWidget(i, 7))->currentIndex();

        qDebug()<<"tmp "<<tmp;

        plotSettingsGGA.back().graphSettings.back().color=colorNms[tmp];

    }


    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

    plotSettingsGGA.back().qPlot->setTitle(tr("Rozwiązania w przestrzeni kryterialnej"));
    plotSettingsGGA.back().qPlot->setAutoMargin(false);
    plotSettingsGGA.back().qPlot->setMargin(50,10,10,230);
    //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
    plotSettingsGGA.back().qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    plotSettingsGGA.back().qPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    plotSettingsGGA.back().qPlot->legend->setFont(legendFont);
    plotSettingsGGA.back().qPlot->legend->setPositionStyle(QCPLegend::psManual);
    plotSettingsGGA.back().qPlot->legend->setPosition(QPoint(50,560));
    plotSettingsGGA.back().qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));




    for (int i = 0; i <  plotSettingsGGA.back().graphSettings.size(); ++i) {
        plotSettingsGGA.back().qPlot->addGraph();
        plotSettingsGGA.back().qPlot->graph(i)->setPen(QColor(plotSettingsGGA.back().graphSettings[i].color));
        plotSettingsGGA.back().qPlot->graph(i)->setLineStyle(QCPGraph::lsNone);
        plotSettingsGGA.back().qPlot->graph(i)->setScatterStyle((QCP::ScatterStyle)plotSettingsGGA.back().graphSettings[i].patternIndex);//QCPGraph::ssCircle);
        plotSettingsGGA.back().qPlot->graph(i)->setScatterSize(7);
        plotSettingsGGA.back().qPlot->graph(i)->setName(plotSettingsGGA.back().graphSettings[i].name);
        plotSettingsGGA.back().qPlot->graph(i)->addToLegend();
    }





    while(widget.plotTableGGA->rowCount()>0)
    {
        widget.plotTableGGA->removeRow(0);
    }



}

void Forma::on_pushButtonDodajWskazniki_clicked()
{

    if(widget.checkBoxGOL->isChecked() && plotSettingsGOL.tabIndex==-1 )
    {
        QWidget* w=new QWidget;
        QCustomPlot* qPlot=new QCustomPlot(w);
        qPlot->setFixedWidth(550);
        qPlot->setFixedHeight(770);
        qPlot->setAutoMargin(false);
        qPlot->setMargin(50,10,30,230);
        qPlot->setTitleColor(QColor("black"));
        qPlot->setTitle("GOL");
        qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        qPlot->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        qPlot->legend->setFont(legendFont);
        qPlot->legend->setPositionStyle(QCPLegend::psManual);
        qPlot->legend->setPosition(QPoint(50,560));
        qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        plotSettingsGOL.qPlot=qPlot;
        plotSettingsGOL.tabIndex=widget.Wykresy->count()-1;
        widget.Wykresy->addTab(w,"GOL");
    }

    if(widget.checkBoxSpacing->isChecked() && plotSettingsSpacing.tabIndex==-1 )
    {
        QWidget* w=new QWidget;
        QCustomPlot* qPlot=new QCustomPlot(w);
        qPlot->setFixedWidth(550);
        qPlot->setFixedHeight(770);
        qPlot->setAutoMargin(false);
        qPlot->setMargin(50,10,30,230);
        qPlot->setTitleColor(QColor("black"));
        qPlot->setTitle("Spacing");
        qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        qPlot->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        qPlot->legend->setFont(legendFont);
        qPlot->legend->setPositionStyle(QCPLegend::psManual);
        qPlot->legend->setPosition(QPoint(50,560));
        qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        plotSettingsSpacing.qPlot=qPlot;
        plotSettingsSpacing.tabIndex=widget.Wykresy->count()-1;
        widget.Wykresy->addTab(w,"Spacing");
    }

    if(widget.checkBoxD_EC->isChecked() && plotSettingsD_EC.tabIndex==-1 )
    {
        QWidget* w=new QWidget;
        QCustomPlot* qPlot=new QCustomPlot(w);
        qPlot->setFixedWidth(550);
        qPlot->setFixedHeight(770);
        qPlot->setAutoMargin(false);
        qPlot->setMargin(50,10,30,230);
        qPlot->setTitleColor(QColor("black"));
        qPlot->setTitle("D_EC");
        qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        qPlot->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        qPlot->legend->setFont(legendFont);
        qPlot->legend->setPositionStyle(QCPLegend::psManual);
        qPlot->legend->setPosition(QPoint(50,560));
        qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        plotSettingsD_EC.qPlot=qPlot;
        plotSettingsD_EC.tabIndex=widget.Wykresy->count()-1;
        widget.Wykresy->addTab(w,"D_EC");
    }

    if(widget.checkBoxD_EC_GOL->isChecked() && plotSettingsD_EC_GOL.tabIndex==-1 )
    {
        QWidget* w=new QWidget;
        QCustomPlot* qPlot=new QCustomPlot(w);
        qPlot->setFixedWidth(550);
        qPlot->setFixedHeight(770);
        qPlot->setAutoMargin(false);
        qPlot->setMargin(50,10,30,230);
        qPlot->setTitleColor(QColor("black"));
        qPlot->setTitle("D_EC_GOL");
        qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        qPlot->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        qPlot->legend->setFont(legendFont);
        qPlot->legend->setPositionStyle(QCPLegend::psManual);
        qPlot->legend->setPosition(QPoint(50,560));
        qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        plotSettingsD_EC_GOL.qPlot=qPlot;
        plotSettingsD_EC_GOL.tabIndex=widget.Wykresy->count()-1;
        widget.Wykresy->addTab(w,"D_EC_GOL");
    }

    if(widget.checkBoxD_EP_GOL->isChecked() && plotSettingsD_EP_GOL.tabIndex==-1 )
    {
        QWidget* w=new QWidget;
        QCustomPlot* qPlot=new QCustomPlot(w);
        qPlot->setFixedWidth(550);
        qPlot->setFixedHeight(770);
        qPlot->setAutoMargin(false);
        qPlot->setMargin(50,10,30,230);
        qPlot->setTitleColor(QColor("black"));
        qPlot->setTitle("D_EP_GOL");
        qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        qPlot->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        qPlot->legend->setFont(legendFont);
        qPlot->legend->setPositionStyle(QCPLegend::psManual);
        qPlot->legend->setPosition(QPoint(50,560));
        qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
        plotSettingsD_EP_GOL.qPlot=qPlot;
        plotSettingsD_EP_GOL.tabIndex=widget.Wykresy->count()-1;
        widget.Wykresy->addTab(w,"D_EP_GOL");
    }

}
void Forma::showWarning(const QString &msg) {

    //    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
    //    QTextCodec::setCodecForTr(QTextCodec::codecForName( "UTF-8" ) );
    // setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    QMessageBox::warning(this, "Warning", msg,
                         QMessageBox::Ok,
                         QMessageBox::NoButton,
                         QMessageBox::NoButton);
}
void Forma::on_bDodajWykres_clicked()
{
    if(!isGGAInitialized)
    {
        showWarning(tr("GGA nie został zainicjalizowany"));
        return;
    }
    //    for (int i = 0; i < widget.plotTableGGA->rowCount(); ++i) {
    //        qDebug()<<((QLineEdit*) widget.plotTableGGA->cellWidget(i, 0))->text()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 1))->currentText()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 5))->currentIndex();
    //    }
    qDebug()<<"okooo";
    AddRowToGGAPlotTable();

    //dddd
}

void Forma::on_checkBoxMOPSO_toggled(bool checked)
{

}

void Forma::on_checkBoxNSGAII_toggled(bool checked)
{

}

void Forma::on_checkBoxGGA_toggled(bool checked)
{

}

void Forma::on_checkBoxGGA_clicked()
{
    if(!isGGAInitialized)
    {
        widget.checkBoxGGA->setChecked(false);
        showWarning("GGA musi zostać najpierw zainicjalizowany");

    }
}

void Forma::on_checkBoxMOPSO_clicked()
{
    if(!isMOPSOInitialized)
    {
        widget.checkBoxMOPSO->setChecked(false);
        showWarning("MOPSO musi zostać najpierw zainicjalizowany");

    }
}

void Forma::on_checkBoxNSGAII_clicked()
{
    if(!isNSGA_IIInitialized)
    {
        widget.checkBoxNSGAII->setChecked(false);
        showWarning("NSGA_II musi zostać najpierw zainicjalizowany");

    }
}

void Forma::AddIndicatorGraphToQCPlot(QCustomPlot *qp, unsigned int index)
{

    unsigned int i;
    if(index==0)
    {
        qp->addGraph();
        i=qp->graphCount()-1;
        // qp->graph(i)->setPen(QColor("red"));
        QPen blueDotPen;
        blueDotPen.setColor(QColor("red"));
        blueDotPen.setStyle(Qt::DotLine);
        blueDotPen.setWidthF(3);
        qp->graph(i)->setPen(blueDotPen);
        //qp->graph(i)->setScatterStyle(QCPGraph::ssNone);//QCPGraph::ssCircle);
        // qp->graph(i)->setScatterSize(7);
        qp->graph(i)->setName("MOPSO");
        qp->graph(i)->addToLegend();
    }
    else if(index==1)
    {

        qp->addGraph();
        i=qp->graphCount()-1;
        QPen blueDotPen;
        blueDotPen.setColor(QColor("green"));
        blueDotPen.setStyle(Qt::DashDotLine);
        blueDotPen.setWidthF(3);
        qp->graph(i)->setPen(blueDotPen);
        //qp->graph(i)->setPen(QColor("green"));
        //qp->graph(i)->setLineStyle(QCPGraph::lsLine);
        //qp->graph(i)->setScatterStyle(QCPGraph::ssNone);//QCPGraph::ssCircle);
        // qp->graph(i)->setScatterSize(7);
        qp->graph(i)->setName("NSGA_II");
        qp->graph(i)->addToLegend();
    }
    else if(index==2)
    {
        qp->addGraph();
        i=qp->graphCount()-1;
        QPen blueDotPen;
        blueDotPen.setColor(QColor("blue"));
        blueDotPen.setStyle(Qt::SolidLine);
        blueDotPen.setWidthF(3);
        qp->graph(i)->setPen(blueDotPen);
        //qp->graph(i)->setPen(QColor("blue"));
        // qp->graph(i)->setLineStyle(QCPGraph::lsLine);
        // qp->graph(i)->setScatterStyle(QCPGraph::ssNone);//QCPGraph::ssCircle);
        // qp->graph(i)->setScatterSize(7);
        qp->graph(i)->setName("GGA");
        qp->graph(i)->addToLegend();
    }
}

void Forma::UpdateIndicatorsGraphs()
{
    double val;

    for (int i = 0; i < plotSettingsSpacing.graphSettings.size(); ++i) {
        if(plotSettingsSpacing.graphSettings[i].algorithm==0)//MOPSO
        {
            val=wskazniki->Spacing2(mopso->repozytorium,0,1 );
            qDebug()<<"mopso Spacing "<<val;
            // plotSettingsSpacing.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsSpacing.graphSettings[i].algorithm==1)//NSGA_II
        {
            val=wskazniki->Spacing2((*nsga_ii->F)[0],0,1);
            qDebug()<<"nsga_ii Spacing "<<val;
            //plotSettingsSpacing.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsSpacing.graphSettings[i].algorithm==2)//GGA
        {
            QVector<Solution*> niezdom;
            for (int j = 0; j < gga->stopZdominowania.size(); ++j) {
                if(gga->stopZdominowania[j]==0)
                {
                    niezdom.push_back(gga->rodzice[j]);
                }
            }
            val=wskazniki->Spacing2(niezdom,0,1 );
            qDebug()<<"gga Spacing "<<val;
            // plotSettingsSpacing.qPlot->graph(i)->addData(numOfIterations,val);
        }



    }

    //    plotSettingsSpacing.qPlot->xAxis->setRange(0,numOfIterations);
    //    plotSettingsSpacing.qPlot->yAxis->setRange(0,1);
    //    plotSettingsSpacing.qPlot->replot();

    ////////////////////////////////////////////////////////////////////


    for (int i = 0; i < plotSettingsGOL.graphSettings.size(); ++i) {
        if(plotSettingsGOL.graphSettings[i].algorithm==0)//MOPSO
        {
            wskazniki->GOL(mopso->repozytorium,mopso->indSortGOL );
            val=mopso->repozytorium[mopso->indSortGOL[0]]->GOL;
            qDebug()<<"mopso GOL "<<val;
            plotSettingsGOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsGOL.graphSettings[i].algorithm==1)//NSGA_II
        {
            wskazniki->GOL((*nsga_ii->F)[0],nsga_ii->indSortGOL );
            val=(*nsga_ii->F)[0][nsga_ii->indSortGOL[0]]->GOL;
            qDebug()<<"nsga_ii GOL "<<val;
            plotSettingsGOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsGOL.graphSettings[i].algorithm==2)//GGA
        {
            wskazniki->GOL(gga->rodzice,gga->indSortGOL );
            val=gga->rodzice[gga->indSortGOL[0]]->GOL;
            qDebug()<<"gga GOL "<<val;
            plotSettingsGOL.qPlot->graph(i)->addData(numOfIterations,val);
        }



    }

    if(plotSettingsGOL.graphSettings.size()>0)
    {
        plotSettingsGOL.qPlot->xAxis->setRange(0,numOfIterations);
        plotSettingsGOL.qPlot->yAxis->setRange(0,1);
        plotSettingsGOL.qPlot->replot();
    }

    ////////////////////////////////////////////////////////////////////
    for (int i = 0; i < plotSettingsD_EC.graphSettings.size(); ++i) {
        if(plotSettingsD_EC.graphSettings[i].algorithm==0)//MOPSO
        {

            val=wskazniki->D_EC(mopso->populacja );
            qDebug()<<"mopso D_EC "<<val;
            plotSettingsD_EC.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EC.graphSettings[i].algorithm==1)//NSGA_II
        {

            val=wskazniki->D_EC(nsga_ii->pokolenie);
            qDebug()<<"nsga_ii D_EC "<<val;
            plotSettingsD_EC.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EC.graphSettings[i].algorithm==2)//GGA
        {

            val=wskazniki->D_EC(gga->rodzice );
            qDebug()<<"gga D_EC "<<val;
            plotSettingsD_EC.qPlot->graph(i)->addData(numOfIterations,val);
        }


        plotSettingsD_EC.qPlot->graph(i)->rescaleAxes(true);

    }

    if(plotSettingsD_EC.graphSettings.size()>0)
    {
        plotSettingsD_EC.qPlot->xAxis->setRange(0,numOfIterations);

        // plotSettingsD_EC.qPlot->yAxis->setRange(0,1);
        plotSettingsD_EC.qPlot->replot();
    }

    ////////////////////////////////////////////////////////////////////
    for (int i = 0; i < plotSettingsD_EC_GOL.graphSettings.size(); ++i) {
        if(plotSettingsD_EC_GOL.graphSettings[i].algorithm==0)//MOPSO
        {

            wskazniki->GOL(mopso->populacja,mopso->indSortGOL );

            val=wskazniki->D_EC_GOL(mopso->populacja,mopso->populacja[mopso->indSortGOL[0]] );
            qDebug()<<"mopso D_EC_GOL "<<val;
            plotSettingsD_EC_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EC_GOL.graphSettings[i].algorithm==1)//NSGA_II
        {

            wskazniki->GOL(nsga_ii->pokolenie,nsga_ii->indSortGOL );

            val=wskazniki->D_EC_GOL(nsga_ii->pokolenie,nsga_ii->pokolenie[nsga_ii->indSortGOL[0]] );
            qDebug()<<"nsga_ii D_EC_GOL "<<val;
            plotSettingsD_EC_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EC_GOL.graphSettings[i].algorithm==2)//GGA
        {
            wskazniki->GOL(gga->rodzice,gga->indSortGOL );

            val=wskazniki->D_EC_GOL(gga->rodzice,gga->rodzice[gga->indSortGOL[0]] );

            qDebug()<<"gga D_EC_GOL "<<val;
            plotSettingsD_EC_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }


        plotSettingsD_EC_GOL.qPlot->graph(i)->rescaleAxes(true);

    }

    if(plotSettingsD_EC_GOL.graphSettings.size()>0)
    {
        plotSettingsD_EC_GOL.qPlot->xAxis->setRange(0,numOfIterations);

        // plotSettingsD_EC_GOL.qPlot->yAxis->setRange(0,1);
        plotSettingsD_EC_GOL.qPlot->replot();
    }

    ////////////////////////////////////////////////////////////////////
    for (int i = 0; i < plotSettingsD_EP_GOL.graphSettings.size(); ++i) {
        if(plotSettingsD_EP_GOL.graphSettings[i].algorithm==0)//MOPSO
        {

            wskazniki->GOL(mopso->populacja,mopso->indSortGOL );

            val=wskazniki->D_EP_GOL(mopso->populacja,mopso->populacja[mopso->indSortGOL[0]] );
            qDebug()<<"mopso D_EP_GOL "<<val;
            plotSettingsD_EP_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EP_GOL.graphSettings[i].algorithm==1)//NSGA_II
        {

            wskazniki->GOL(nsga_ii->pokolenie,nsga_ii->indSortGOL );

            val=wskazniki->D_EP_GOL(nsga_ii->pokolenie,nsga_ii->pokolenie[nsga_ii->indSortGOL[0]] );
            qDebug()<<"nsga_ii D_EP_GOL "<<val;
            plotSettingsD_EP_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }
        else if(plotSettingsD_EP_GOL.graphSettings[i].algorithm==2)//GGA
        {
            wskazniki->GOL(gga->rodzice,gga->indSortGOL );

            val=wskazniki->D_EP_GOL(gga->rodzice,gga->rodzice[gga->indSortGOL[0]] );

            qDebug()<<"gga D_EP_GOL "<<val;
            plotSettingsD_EP_GOL.qPlot->graph(i)->addData(numOfIterations,val);
        }


        plotSettingsD_EP_GOL.qPlot->graph(i)->rescaleAxes(true);

    }

    if(plotSettingsD_EP_GOL.graphSettings.size()>0)
    {
        plotSettingsD_EP_GOL.qPlot->xAxis->setRange(0,numOfIterations);

        // plotSettingsD_EP_GOL.qPlot->yAxis->setRange(0,1);
        plotSettingsD_EP_GOL.qPlot->replot();
    }


}


void Forma::on_bInicjalizujSymulacja_clicked()
{
    numOfIterations=0;


    if(widget.checkBoxD_EC->isChecked() )
    {
        plotSettingsD_EC.qPlot->clearGraphs();
    }


    if(widget.checkBoxD_EC_GOL->isChecked() )
    {
        plotSettingsD_EC_GOL.qPlot->clearGraphs();
    }

    if(widget.checkBoxD_EP_GOL->isChecked() )
    {

        plotSettingsD_EP_GOL.qPlot->clearGraphs();
    }

    if(widget.checkBoxGOL->isChecked() )
    {
        plotSettingsGOL.qPlot->clearGraphs();
    }


    if(widget.checkBoxSpacing->isChecked() )
    {
        plotSettingsSpacing.qPlot->clearGraphs();
    }




    if(widget.checkBoxMOPSO->isChecked())
    {
        if(widget.checkBoxD_EC->isChecked() )
        {
            plotSettingsD_EC.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC.graphSettings.back().algorithm=0;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC.qPlot,0);
        }

        if(widget.checkBoxD_EC_GOL->isChecked() )
        {
            // plotSettingsD_EC_GOL.algorithms.push_back(0);
            plotSettingsD_EC_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC_GOL.graphSettings.back().algorithm=0;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC_GOL.qPlot,0);
        }

        if(widget.checkBoxD_EP_GOL->isChecked() )
        {
            // plotSettingsD_EP_GOL.algorithms.push_back(0);
            plotSettingsD_EP_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EP_GOL.graphSettings.back().algorithm=0;
            AddIndicatorGraphToQCPlot(plotSettingsD_EP_GOL.qPlot,0);
        }

        if(widget.checkBoxGOL->isChecked())
        {
            // plotSettingsGOL.algorithms.push_back(0);
            plotSettingsGOL.graphSettings.push_back(GraphSetting());
            plotSettingsGOL.graphSettings.back().algorithm=0;
            AddIndicatorGraphToQCPlot(plotSettingsGOL.qPlot,0);
        }

        if(widget.checkBoxSpacing->isChecked() )
        {
            // plotSettingsSpacing.algorithms.push_back(0);
            plotSettingsSpacing.graphSettings.push_back(GraphSetting());
            plotSettingsSpacing.graphSettings.back().algorithm=0;
            AddIndicatorGraphToQCPlot(plotSettingsSpacing.qPlot,0);
        }
    }


    if(widget.checkBoxNSGAII->isChecked())
    {
        if(widget.checkBoxD_EC->isChecked() )
        {
            //plotSettingsD_EC.algorithms.push_back(1);
            plotSettingsD_EC.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC.graphSettings.back().algorithm=1;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC.qPlot,1);
        }

        if(widget.checkBoxD_EC_GOL->isChecked() )
        {
            //plotSettingsD_EC_GOL.algorithms.push_back(1);
            plotSettingsD_EC_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC_GOL.graphSettings.back().algorithm=1;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC_GOL.qPlot,1);
        }

        if(widget.checkBoxD_EP_GOL->isChecked() )
        {
            // plotSettingsD_EP_GOL.algorithms.push_back(1);
            plotSettingsD_EP_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EP_GOL.graphSettings.back().algorithm=1;
            AddIndicatorGraphToQCPlot(plotSettingsD_EP_GOL.qPlot,1);
        }

        if(widget.checkBoxGOL->isChecked() )
        {
            //  plotSettingsGOL.algorithms.push_back(1);
            plotSettingsGOL.graphSettings.push_back(GraphSetting());
            plotSettingsGOL.graphSettings.back().algorithm=1;
            AddIndicatorGraphToQCPlot(plotSettingsGOL.qPlot,1);
        }

        if(widget.checkBoxSpacing->isChecked() )
        {
            //plotSettingsSpacing.algorithms.push_back(1);
            plotSettingsSpacing.graphSettings.push_back(GraphSetting());
            plotSettingsSpacing.graphSettings.back().algorithm=1;
            AddIndicatorGraphToQCPlot(plotSettingsSpacing.qPlot,1);
        }

    }


    if(widget.checkBoxGGA->isChecked())
    { if(widget.checkBoxD_EC->isChecked() )
        {
            //plotSettingsD_EC.algorithms.push_back(2);
            plotSettingsD_EC.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC.graphSettings.back().algorithm=2;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC.qPlot,2);
        }

        if(widget.checkBoxD_EC_GOL->isChecked() )
        {
            //plotSettingsD_EC_GOL.algorithms.push_back(2);
            plotSettingsD_EC_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EC_GOL.graphSettings.back().algorithm=2;
            AddIndicatorGraphToQCPlot(plotSettingsD_EC_GOL.qPlot,2);
        }

        if(widget.checkBoxD_EP_GOL->isChecked() )
        {
            //plotSettingsD_EP_GOL.algorithms.push_back(2);
            plotSettingsD_EP_GOL.graphSettings.push_back(GraphSetting());
            plotSettingsD_EP_GOL.graphSettings.back().algorithm=2;
            AddIndicatorGraphToQCPlot(plotSettingsD_EP_GOL.qPlot,2);
        }
        if(widget.checkBoxGOL->isChecked() )
        {
            //plotSettingsGOL.algorithms.push_back(2);
            plotSettingsGOL.graphSettings.push_back(GraphSetting());
            plotSettingsGOL.graphSettings.back().algorithm=2;
            AddIndicatorGraphToQCPlot(plotSettingsGOL.qPlot,2);
        }
        if(widget.checkBoxSpacing->isChecked() )
        {
            //plotSettingsSpacing.algorithms.push_back(2);
            plotSettingsSpacing.graphSettings.push_back(GraphSetting());
            plotSettingsSpacing.graphSettings.back().algorithm=2;
            AddIndicatorGraphToQCPlot(plotSettingsSpacing.qPlot,2);
        }
    }

    TRACE;
    if(plotSettingsD_EC.graphSettings.size()>0)
    {
        plotSettingsD_EC.qPlot->replot();

    }
    if(plotSettingsD_EC_GOL.graphSettings.size()>0)
    {
        plotSettingsD_EC_GOL.qPlot->replot();

    }
    if(plotSettingsD_EP_GOL.graphSettings.size()>0)
    {
        plotSettingsD_EP_GOL.qPlot->replot();

    }
    if(plotSettingsGOL.graphSettings.size()>0)
    {
        plotSettingsGOL.qPlot->replot();

    }
    if(plotSettingsSpacing.graphSettings.size()>0)
    {
        plotSettingsSpacing.qPlot->replot();

    }
    TRACE;


    if(isMOPSOInitialized)
    {
        mopso->Inicjalizuj();
    }

    if(isNSGA_IIInitialized)
    {
        nsga_ii->Inicjalizuj();
    }

    if(isGGAInitialized)
    {
        gga->Inicjalizuj();
    }

    Update2DPlots();
}

void Forma::on_bIterujRazSymulacja_clicked()
{
    if(widget.checkBoxMOPSO->isChecked())
    {
        mopso->Iteruj();
    }

    if(widget.checkBoxNSGAII->isChecked())
    {
        nsga_ii->Iteruj();
    }

    if(widget.checkBoxGGA->isChecked())
    {
        gga->Iteruj();
    }

    numOfIterations++;
    UpdateIndicatorsGraphs();
    Update2DPlots();
}

void Forma::on_bDodajWykresMOPSO_clicked()
{
    if(!isMOPSOInitialized)
    {
        showWarning(tr("MOPSO nie został zainicjalizowany"));
        return;
    }
    //    for (int i = 0; i < widget.plotTableGGA->rowCount(); ++i) {
    //        qDebug()<<((QLineEdit*) widget.plotTableGGA->cellWidget(i, 0))->text()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 1))->currentText()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 5))->currentIndex();
    //    }
    qDebug()<<"ok";
    AddRowToMOPSOPlotTable();
}

void Forma::on_bDodajWykresNSGA_II_clicked()
{
    if(!isNSGA_IIInitialized)
    {
        showWarning(tr("NSGA_II nie został zainicjalizowany"));
        return;
    }
    //    for (int i = 0; i < widget.plotTableGGA->rowCount(); ++i) {
    //        qDebug()<<((QLineEdit*) widget.plotTableGGA->cellWidget(i, 0))->text()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 1))->currentText()<<" "<<
    //                  ((QComboBox*) widget.plotTableGGA->cellWidget(i, 5))->currentIndex();
    //    }
    qDebug()<<"ok";
    AddRowToNSGA_IIPlotTable();
}

void Forma::on_stworzWykresyMOPSO_clicked()
{
    //widget.Wykresy->clear();

    if(widget.plotTableMOPSO->rowCount()==0)
    {
        showWarning(tr("Należy dodać przynajmniej 1 wykres"));
        return;
    }

    QWidget* w=new QWidget;
    QCustomPlot* qPlot=new QCustomPlot(w);
    qPlot->setFixedWidth(550);
    qPlot->setFixedHeight(770);
    qPlot->setAutoMargin(false);
    qPlot->setMargin(50,10,10,230);


    //plotsSettingsMOPSO.clear();
    plotSettingsMOPSO.push_back(Plot2DSetting());
    plotSettingsMOPSO.back().qPlot=qPlot;
    plotSettingsMOPSO.back().tabIndex=widget.Wykresy->count()-1;
    widget.Wykresy->addTab(w,"MOPSO");


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";


    TRACE;

    int tmp;

    for (int i = 0; i < widget.plotTableMOPSO->rowCount(); ++i) {
        plotSettingsMOPSO.back().graphSettings.push_back(GraphSetting());
        plotSettingsMOPSO.back().graphSettings.back().axisSettings.resize(2);
        plotSettingsMOPSO.back().graphSettings.back().name=((QLineEdit*) widget.plotTableMOPSO->cellWidget(i, 0))->text();

        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 1))->currentIndex();

        if(tmp==0)
        {
            plotSettingsMOPSO.back().graphSettings.back().axisSettings[0].isParam=false;
        }
        else
        {
            plotSettingsMOPSO.back().graphSettings.back().axisSettings[0].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 2))->currentIndex();

        plotSettingsMOPSO.back().graphSettings.back().axisSettings[0].index=tmp;

        /////

        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 3))->currentIndex();

        if(tmp==0)
        {
            plotSettingsMOPSO.back().graphSettings.back().axisSettings[1].isParam=false;
        }
        else
        {
            plotSettingsMOPSO.back().graphSettings.back().axisSettings[1].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 4))->currentIndex();

        plotSettingsMOPSO.back().graphSettings.back().axisSettings[1].index=tmp;

        //////
        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 5))->currentIndex();

        if(tmp<2)
        {
            if(tmp==0)
            {
                plotSettingsMOPSO.back().graphSettings.back().all=true;
            }
            else
            {
                plotSettingsMOPSO.back().graphSettings.back().pareto=true;
            }
        }
        else
        {
            //tmp=tmp-2;
            // plotSettingsMOPSO.back().graphSettings.back().indexOfGender=tmp;


        }

        QString str=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 6))->currentText();
        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 6))->currentIndex();
        plotSettingsMOPSO.back().graphSettings.back().patternName=str;
        plotSettingsMOPSO.back().graphSettings.back().patternIndex=tmp;

        tmp=((QComboBox*) widget.plotTableMOPSO->cellWidget(i, 7))->currentIndex();

        qDebug()<<"tmp "<<tmp;

        plotSettingsMOPSO.back().graphSettings.back().color=colorNms[tmp];

    }


    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

    plotSettingsMOPSO.back().qPlot->setTitle(tr("Rozwiązania w przestrzeni kryterialnej"));
    plotSettingsMOPSO.back().qPlot->setAutoMargin(false);
    plotSettingsMOPSO.back().qPlot->setMargin(50,10,10,230);
    //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
    plotSettingsMOPSO.back().qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    plotSettingsMOPSO.back().qPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    plotSettingsMOPSO.back().qPlot->legend->setFont(legendFont);
    plotSettingsMOPSO.back().qPlot->legend->setPositionStyle(QCPLegend::psManual);
    plotSettingsMOPSO.back().qPlot->legend->setPosition(QPoint(50,560));
    plotSettingsMOPSO.back().qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));




    for (int i = 0; i <  plotSettingsMOPSO.back().graphSettings.size(); ++i) {
        plotSettingsMOPSO.back().qPlot->addGraph();
        plotSettingsMOPSO.back().qPlot->graph(i)->setPen(QColor(plotSettingsMOPSO.back().graphSettings[i].color));
        plotSettingsMOPSO.back().qPlot->graph(i)->setLineStyle(QCPGraph::lsNone);
        plotSettingsMOPSO.back().qPlot->graph(i)->setScatterStyle((QCP::ScatterStyle)plotSettingsMOPSO.back().graphSettings[i].patternIndex);//QCPGraph::ssCircle);
        plotSettingsMOPSO.back().qPlot->graph(i)->setScatterSize(7);
        plotSettingsMOPSO.back().qPlot->graph(i)->setName(plotSettingsMOPSO.back().graphSettings[i].name);
        plotSettingsMOPSO.back().qPlot->graph(i)->addToLegend();
    }





    while(widget.plotTableMOPSO->rowCount()>0)
    {
        widget.plotTableMOPSO->removeRow(0);
    }


}

void Forma::on_bIterujRazGGA_clicked()
{
    qDebug()<<"iteruj raz GGA\n";
    gga->Iteruj();
    AktualizujWykresParamGGA();
    qDebug()<<"spacing "<<gga->RozpFrontParWarFunKryt();

    wskazniki->GOL(gga->rodzice,gga->indSortGOL );
    double gol=gga->rodzice[gga->indSortGOL[0]]->GOL;
    //      vGol.push_back(QPointF((double)pso->num_iter,gol));
    qDebug()<<"D_EC_GOL "<<wskazniki->D_EC_GOL(gga->rodzice,gga->rodzice[gga->indSortGOL[0]]);
    qDebug() << "GOL = " << gol << "\n";
    qDebug()<<"D_EC "<<wskazniki->D_EC(gga->rodzice);


    UpdateAllPlots();
}

void Forma::on_bIterujRazMOPSO_clicked()
{
    qDebug()<<"iteruj raz MOPSO\n";
    mopso->Iteruj();
    TRACE;
    //AktualizujWykresParamGGA();
    // qDebug()<<"spacing "<<gga->RozpFrontParWarFunKryt();

    //   wskazniki->GOL(gga->rodzice,gga->indSortGOL );
    //   double gol=gga->rodzice[gga->indSortGOL[0]]->GOL;
    //      vGol.push_back(QPointF((double)pso->num_iter,gol));
    //  qDebug()<<"D_EC_GOL "<<wskazniki->D_EC_GOL(gga->rodzice,gga->rodzice[gga->indSortGOL[0]]);
    // qDebug() << "GOL = " << gol << "\n";
    // qDebug()<<"D_EC "<<wskazniki->D_EC(gga->rodzice);


    UpdateAllPlots();
}

void Forma::on_stworzWykresyNSGA_II_clicked()
{

    //widget.Wykresy->clear();

    if(widget.plotTableNSGA_II->rowCount()==0)
    {
        showWarning(tr("Należy dodać przynajmniej 1 wykres"));
        return;
    }

    QWidget* w=new QWidget;
    QCustomPlot* qPlot=new QCustomPlot(w);
    qPlot->setFixedWidth(550);
    qPlot->setFixedHeight(770);
    qPlot->setAutoMargin(false);
    qPlot->setMargin(50,10,10,230);


    //plotsSettingsNSGA_II.clear();
    plotSettingsNSGA_II.push_back(Plot2DSetting());
    plotSettingsNSGA_II.back().qPlot=qPlot;
    plotSettingsNSGA_II.back().tabIndex=widget.Wykresy->count()-1;
    widget.Wykresy->addTab(w,"NSGA II");


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";


    TRACE;

    int tmp;

    for (int i = 0; i < widget.plotTableNSGA_II->rowCount(); ++i) {
        plotSettingsNSGA_II.back().graphSettings.push_back(GraphSetting());
        plotSettingsNSGA_II.back().graphSettings.back().axisSettings.resize(2);
        plotSettingsNSGA_II.back().graphSettings.back().name=((QLineEdit*) widget.plotTableNSGA_II->cellWidget(i, 0))->text();

        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 1))->currentIndex();

        if(tmp==0)
        {
            plotSettingsNSGA_II.back().graphSettings.back().axisSettings[0].isParam=false;
        }
        else
        {
            plotSettingsNSGA_II.back().graphSettings.back().axisSettings[0].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 2))->currentIndex();

        plotSettingsNSGA_II.back().graphSettings.back().axisSettings[0].index=tmp;

        /////

        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 3))->currentIndex();

        if(tmp==0)
        {
            plotSettingsNSGA_II.back().graphSettings.back().axisSettings[1].isParam=false;
        }
        else
        {
            plotSettingsNSGA_II.back().graphSettings.back().axisSettings[1].isParam=true;
        }

        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 4))->currentIndex();

        plotSettingsNSGA_II.back().graphSettings.back().axisSettings[1].index=tmp;

        //////
        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 5))->currentIndex();

        if(tmp<2)
        {
            if(tmp==0)
            {
                plotSettingsNSGA_II.back().graphSettings.back().all=true;
            }
            else
            {
                plotSettingsNSGA_II.back().graphSettings.back().pareto=true;
            }
        }
        else
        {
            //tmp=tmp-2;
            // plotSettingsNSGA_II.back().graphSettings.back().indexOfGender=tmp;


        }

        QString str=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 6))->currentText();
        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 6))->currentIndex();
        plotSettingsNSGA_II.back().graphSettings.back().patternName=str;
        plotSettingsNSGA_II.back().graphSettings.back().patternIndex=tmp;

        tmp=((QComboBox*) widget.plotTableNSGA_II->cellWidget(i, 7))->currentIndex();

        qDebug()<<"tmp "<<tmp;

        plotSettingsNSGA_II.back().graphSettings.back().color=colorNms[tmp];

    }


    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

    plotSettingsNSGA_II.back().qPlot->setTitle(tr("Rozwiązania w przestrzeni kryterialnej"));
    plotSettingsNSGA_II.back().qPlot->setAutoMargin(false);
    plotSettingsNSGA_II.back().qPlot->setMargin(50,10,10,230);
    //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
    plotSettingsNSGA_II.back().qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    plotSettingsNSGA_II.back().qPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    plotSettingsNSGA_II.back().qPlot->legend->setFont(legendFont);
    plotSettingsNSGA_II.back().qPlot->legend->setPositionStyle(QCPLegend::psManual);
    plotSettingsNSGA_II.back().qPlot->legend->setPosition(QPoint(50,560));
    plotSettingsNSGA_II.back().qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));




    for (int i = 0; i <  plotSettingsNSGA_II.back().graphSettings.size(); ++i) {
        plotSettingsNSGA_II.back().qPlot->addGraph();
        plotSettingsNSGA_II.back().qPlot->graph(i)->setPen(QColor(plotSettingsNSGA_II.back().graphSettings[i].color));
        plotSettingsNSGA_II.back().qPlot->graph(i)->setLineStyle(QCPGraph::lsNone);
        plotSettingsNSGA_II.back().qPlot->graph(i)->setScatterStyle((QCP::ScatterStyle)plotSettingsNSGA_II.back().graphSettings[i].patternIndex);//QCPGraph::ssCircle);
        plotSettingsNSGA_II.back().qPlot->graph(i)->setScatterSize(7);
        plotSettingsNSGA_II.back().qPlot->graph(i)->setName(plotSettingsNSGA_II.back().graphSettings[i].name);
        plotSettingsNSGA_II.back().qPlot->graph(i)->addToLegend();
    }


    int i=plotSettingsNSGA_II.back().graphSettings.size();
    plotSettingsNSGA_II.back().qPlot->addGraph();
    plotSettingsNSGA_II.back().qPlot->graph(i)->setPen(QColor("black"));
    plotSettingsNSGA_II.back().qPlot->graph(i)->setLineStyle(QCPGraph::lsLine);
    // plotSettingsNSGA_II.back().qPlot->graph(i)->setScatterStyle((QCPGraph::ScatterStyle)plotSettingsNSGA_II.back().graphSettings[i].patternIndex);//QCPGraph::ssCircle);
    //  plotSettingsNSGA_II.back().qPlot->graph(i)->setScatterSize(7);
    plotSettingsNSGA_II.back().qPlot->graph(i)->setName("true Pareto front");
    plotSettingsNSGA_II.back().qPlot->graph(i)->addToLegend();


    for (float  x = 0; x <=1.0; x+=0.01) {
        plotSettingsNSGA_II.back().qPlot->graph(i)->addData(x,1.0-qSqrt(x));
    }

    while(widget.plotTableNSGA_II->rowCount()>0)
    {
        widget.plotTableNSGA_II->removeRow(0);
    }

}

void Forma::on_bIterujNSGA_2_clicked()
{
    nsga_ii->Iteruj();
    UpdateAllPlots();
}

void Forma::on_bInicjalizujNSGA_clicked()
{
    qDebug()<<"3821";


    if(!isFunctionInitialized)
    {
        showWarning(tr("Problem musi zostać najpierw załadowany"));
        return;
    }

    if (nsga_ii != NULL) {
        qDebug() << "delete nsga\n";
        delete nsga_ii;
    }
    bool ok;
    unsigned int populacja = widget.populacjaNSGA_II->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy rozmiar populacji");
        return;
    }
    unsigned int il_bitow = widget.ilBitowNSGA_II->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy ilosc bitow");
        return;
    }
    float prawd_krzyz = widget.prawdKrzyzNSGA_II->text().toFloat(&ok);
    if (!ok || prawd_krzyz < 0 || prawd_krzyz > 1.0) {
        showWarning("Niewlasciwa wartość prawd. krzyzowania");
        return;
    }
    float prawd_mut = widget.prawdMutNSGA_II->text().toFloat(&ok);
    if (!ok || prawd_mut < 0.0 || prawd_mut > 1.0) {
        showWarning("Niewlasciwa wartość prawd. mutacji");
        return;
    }
    nsga_ii = new NSGA_II(populacja, il_bitow, prawd_mut, prawd_krzyz, problem);
    connect(nsga_ii, SIGNAL(done()), this, SLOT(sDoneNSGA())); //,Qt::QueuedConnection);
    connect(nsga_ii, SIGNAL(finished()), this, SLOT(sFinishedNSGA()),Qt::QueuedConnection);

    DINFO;
    nsga_ii->Inicjalizuj();
    DINFO;

    //  nsga->Inicjalizuj(&nsga->rodzenstwo);

    DINFO;

    nsga_ii->zainicjalizowany=true;
    isNSGA_IIInitialized=true;
    qDebug()<<"3866";
    UpdateAllPlots();
}

void Forma::on_bInicjalizujMOPSO_clicked()
{

    if(!isFunctionInitialized)
    {
        showWarning(tr("Problem musi zostać najpierw załadowany"));
        return;
    }

    bool z_mutacja;
    bool ok;
    unsigned int populacja = widget.populacjaMOPSO->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy rozmiar populacji");
        return;
    }
    unsigned int wiel_rep = widget.wiel_repozytMOPSO->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy rozmiar repozytorium");
        return;
    }
    unsigned int il_hiper = widget.il_hiperMOPSO->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwa ilosc hiperkostek");
        return;
    }
    //wykresGOL->wyczysc();




    if (widget.z_mutacja->isChecked()) {
        z_mutacja = true;

    } else {
        z_mutacja = false;

    }

    mopso = new MOPSO(populacja, wiel_rep, il_hiper, z_mutacja, problem);

    connect(mopso, SIGNAL(done()), this, SLOT(sDoneMOPSO()));
    connect(mopso, SIGNAL(finished()), this, SLOT(sFinishedMOPSO()),Qt::QueuedConnection);
    mopso->vmax = 100.0;

    mopso->Inicjalizuj();




    //aktualizuj_wykres_przystosowania();

    // d_directPainter->drawSeries(points_dom,
    //      data_dom->size() - 1, data_dom->size() - 1);
    //d_directPainter->drawSeries(points_nondom,
    //    data_nondom->size() - 1, data_nondom->size() - 1);
    //wyswietlamy populacje na wykresie funkcji celu



    //  pso->wyswietl_repozytorium();
    //   pso->wyswietl_populacje();

    //  pso->Iteruj();
    // aktualizuj_wykres_przystosowania();

    // qDebug() << "Generuje kostki\n";
    //  pso->generuj_kostki();

    mopso->zainicjalizowany=true;


    isMOPSOInitialized=true;

    UpdateAllPlots();

}

void Forma::on_bInicjalizujGGA_clicked()
{
    ////////

    if(!isFunctionInitialized)
    {
        showWarning(tr("Problem musi zostać najpierw załadowany"));
        return;
    }


    //    widget.qPlotNSGA_II_PID->addGraph();
    //    widget.qPlotNSGA_II_PID->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    //    widget.qPlotNSGA_II_PID->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    //    widget.qPlotNSGA_II_PID->addGraph();
    //    widget.qPlotNSGA_II_PID->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    //    // generate some points of data (y0 for first, y1 for second graph):
    //    QVector<double> x(250), y0(250), y1(250);
    //    for (int i=0; i<250; ++i)
    //    {
    //      x[i] = i;
    //      y0[i] = exp(-i/150.0)*cos(i/10.0); // exponentially decaying cosine
    //      y1[i] = exp(-i/150.0);             // exponential envelope
    //    }
    //    // configure right and top axis to show ticks but no labels (could've also just called widget.qPlotNSGA_II_PID->setupFullAxesBox):
    //    widget.qPlotNSGA_II_PID->xAxis2->setVisible(true);
    //    widget.qPlotNSGA_II_PID->xAxis2->setTickLabels(false);
    //    widget.qPlotNSGA_II_PID->yAxis2->setVisible(true);
    //    widget.qPlotNSGA_II_PID->yAxis2->setTickLabels(false);
    //    // make left and bottom axes always transfer their ranges to right and top axes:
    //    connect(widget.qPlotNSGA_II_PID->xAxis, SIGNAL(rangeChanged(QCPRange)), widget.qPlotNSGA_II_PID->xAxis2, SLOT(setRange(QCPRange)));
    //    connect(widget.qPlotNSGA_II_PID->yAxis, SIGNAL(rangeChanged(QCPRange)), widget.qPlotNSGA_II_PID->yAxis2, SLOT(setRange(QCPRange)));
    //    // pass data points to graphs:
    //    widget.qPlotNSGA_II_PID->graph(0)->setData(x, y0);
    //    widget.qPlotNSGA_II_PID->graph(1)->setData(x, y1);
    //    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    //    widget.qPlotNSGA_II_PID->graph(0)->rescaleAxes();
    //    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    //    widget.qPlotNSGA_II_PID->graph(1)->rescaleAxes(true);
    //    // Note: we could have also just called widget.qPlotNSGA_II_PID->rescaleAxes(); instead
    //    // make range moveable by mouse interaction (click and drag):
    //    widget.qPlotNSGA_II_PID->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    //    widget.qPlotNSGA_II_PID->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    //    widget.qPlotNSGA_II_PID->setInteraction(QCustomPlot::iSelectPlottables); //



    qDebug()<<"wykres"<<endl;
    //widget.qPlotNSGA_II_PID

    // setup for graph 4: key axis right, value axis top
    // will contain parabolically distributed data points with some random perturbance

    if(widget.qPlotNSGA_II_PID->graphCount()==0)
    {

        QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

        widget.qPlotNSGA_II_PID->setTitle(tr("Rozwiązania w przestrzeni kryterialnej"));
        widget.qPlotNSGA_II_PID->setAutoMargin(false);
        widget.qPlotNSGA_II_PID->setMargin(50,10,10,230);
        //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
        widget.qPlotNSGA_II_PID->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        widget.qPlotNSGA_II_PID->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        widget.qPlotNSGA_II_PID->legend->setFont(legendFont);
        widget.qPlotNSGA_II_PID->legend->setPositionStyle(QCPLegend::psManual);
        widget.qPlotNSGA_II_PID->legend->setPosition(QPoint(50,560));
        widget.qPlotNSGA_II_PID->legend->setBrush(QBrush(QColor(255,255,255,230)));


        QPen pen1(Qt::green);
        QPen pen2(Qt::red);

        widget.qPlotNSGA_II_PID->addGraph();
        widget.qPlotNSGA_II_PID->graph(0)->setPen(QColor(255, 0, 0, 255));
        widget.qPlotNSGA_II_PID->graph(0)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotNSGA_II_PID->graph(0)->setScatterStyle(QCP::ssCircle);
        widget.qPlotNSGA_II_PID->graph(0)->setScatterSize(7);
        widget.qPlotNSGA_II_PID->graph(0)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));


        widget.qPlotNSGA_II_PID->addGraph();
        widget.qPlotNSGA_II_PID->graph(1)->setPen(QColor(0, 0, 255, 255));
        widget.qPlotNSGA_II_PID->graph(1)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotNSGA_II_PID->graph(1)->setScatterStyle(QCP::ssPlus);
        widget.qPlotNSGA_II_PID->graph(1)->setScatterSize(7);
        widget.qPlotNSGA_II_PID->graph(1)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 2"));

        widget.qPlotNSGA_II_PID->addGraph();
        widget.qPlotNSGA_II_PID->graph(2)->setPen(QColor(0, 100, 0, 255));
        widget.qPlotNSGA_II_PID->graph(2)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotNSGA_II_PID->graph(2)->setScatterStyle(QCP::ssPlusCircle);
        widget.qPlotNSGA_II_PID->graph(2)->setScatterSize(7);
        widget.qPlotNSGA_II_PID->graph(2)->setName(tr("Front Pareto"));



        widget.qPlotNSGA_II_PID->graph(0)->addToLegend();
        widget.qPlotNSGA_II_PID->graph(1)->addToLegend();
    }
    ///////

    bool ok;
    unsigned int populacja = widget.populacjaGGA->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy rozmiar populacji");
        return;
    }
    unsigned int il_bitow = widget.ilBitowGGA->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwy ilosc bitow");
        return;
    }
    float prawd_krzyz = widget.prawdKrzyzGGA->text().toFloat(&ok);
    if (!ok || prawd_krzyz < 0 || prawd_krzyz > 1.0) {
        showWarning("Niewlasciwa wartość prawd. krzyzowania");
        return;
    }
    float prawd_mut = widget.prawdMutGGA->text().toFloat(&ok);
    if (!ok || prawd_mut < 0.0 || prawd_mut > 1.0) {
        showWarning("Niewlasciwa wartość prawd. mutacji");
        return;
    }


    unsigned int ilOsobElit = widget.ilOsobElitGGA->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwa ilosc osobnikow elitarnych");
        return;
    }


    //wczytujemy przydzial funkcji do rodzajnikow
    QVector<QString> nazwyRodzajnikow;
    QVector<QVector<unsigned int> > funkcjeRodzajnikow;


    if(widget.functionTable->rowCount()!=0)
    {
        showWarning("Nie wszystkie funkcje zostaly przydzielone do rodzajnikow!!!");

        return;
    }


    TRACE;
    qDebug()<<"top level item "<<widget.treeWidget->topLevelItemCount()<<"\n";
    for(int i=0;i<widget.treeWidget->topLevelItemCount();++i)
    {
        qDebug()<<"num item "<<widget.treeWidget->topLevelItem(i)->childCount()<<"\n";
        qDebug()<<"name "<<widget.treeWidget->topLevelItem(i)->text(0)<<endl;

        if(widget.treeWidget->topLevelItem(i)->childCount()>0)
        {
            nazwyRodzajnikow.push_back(widget.treeWidget->topLevelItem(i)->text(0));
            funkcjeRodzajnikow.push_back(QVector<unsigned int>());
        }
        for(unsigned int j=0;j<widget.treeWidget->topLevelItem(i)->childCount();j++)
        {

            qDebug()<<"address "<<widget.treeWidget->topLevelItem(i)->child(j)<<endl;
            funkcjeRodzajnikow.back().push_back(mFunctionAttachedToGender[widget.treeWidget->topLevelItem(i)->child(j)]);

            qDebug()<<"rodz "<<funkcjeRodzajnikow.size()-1<<" = "<<mFunctionAttachedToGender[widget.treeWidget->topLevelItem(i)->child(j)]<<endl;
        }
    }
    TRACE;

    gga = new GGA(populacja, il_bitow, prawd_mut, prawd_krzyz,ilOsobElit,nazwyRodzajnikow,funkcjeRodzajnikow, problem);
    connect(gga, SIGNAL(done()), this, SLOT(sDoneGGA())); //,Qt::QueuedConnection);
    connect(gga, SIGNAL(finished()), this, SLOT(sFinishedGGA()),Qt::QueuedConnection);

    DINFO;
    gga->Inicjalizuj();


    DINFO;


    //widget.qPlotNSGA_II_PID->graph(0)->addData(4,4);
    //  widget.qPlotNSGA_II_PID->graph(0)->addData(200,333);


    TRACE;

    qDebug()<<"il rodzajnikow "<<gga->rozwPrzydzRodzajnikom.size()<<endl;
    for(unsigned int i=0;i<gga->rozwPrzydzRodzajnikom.size();i++)
    {
        qDebug()<<"rodzjnik["<<i<<"] : "<<gga->rozwPrzydzRodzajnikom[i].size()<<endl;

    }


    TRACE;
    gga->DekodujGenotyp();
    TRACE;
    qDebug()<<__FILE__<<" at "<<__LINE__;

    gga->WyznaczWartFunkcjiKryterialnych();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    TRACE;
    gga-> WyznaczPrzystosowanie();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    TRACE;
    gga->PrzeskalujPrzystosowanie();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    gga->WyznaczStopnieZdominowania();

    gga->WyznaczRangi();

    TRACE;
    AktualizujWykresParamGGA();
    TRACE;



    gga->zainicjalizowany=true;

    isGGAInitialized=true;


    qDebug()<<"spacing "<<gga->RozpFrontParWarFunKryt();
    UpdateAllPlots();

}

void Forma::on_bDodajWykres2D_clicked()
{
    AddRowTo2DPlotsTable();
}


void Forma::AddRowTo2DPlotsTable()
{
    if(!isFunctionInitialized)
    {
        showWarning("Funkcje nie zostaly wczytane");
        return;
    }


    QVector<QString> patternNames= { "None"     ///< no scatter symbols are drawn (e.g. data only represented with lines, see \ref setLineStyle)
                                     ,"Dot"       ///< a single pixel, \ref setScatterSize has no influence on its size.
                                     ,"Cross"     ///< a cross (x)
                                     ,"Plus"      ///< a plus (+)
                                     ,"Circle"    ///< a circle which is not filled
                                     ,"Disc"      ///< a circle which is filled with the color of the graph's pen (not the brush!)
                                     ,"Square"    ///< a square which is not filled
                                     ,"Star"      ///< a star with eight arms, i.e. a combination of cross and plus
                                     ,"Triangle"  ///< an equilateral triangle which is not filled, standing on baseline
                                     ,"TriangleInverted" ///< an equilateral triangle which is not filled, standing on corner
                                     ,"CrossSquare"      ///< a square which is not filled, with a cross inside
                                     ,"PlusSquare"       ///< a square which is not filled, with a plus inside
                                     ,"CrossCircle"      ///< a circle which is not filled, with a cross inside
                                     ,"PlusCircle"       ///< a circle which is not filled, with a plus inside
                                     ,"Peace"    ///< a circle which is not filled, with one vertical and two downward diagonal lines
                                     ,"Pixmap"    ///< a custom pixmap specified by setScatterPixmap, centered on the data point coordinates. \ref setScatterSize has no influence on its size.
                                   };


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";

    QComboBox* Combcolor = new QComboBox;
    Combcolor->setMinimumWidth(40);
    Combcolor->setMaximumWidth(40);
    QPalette pal = Combcolor->palette(); // holds the widget's palette.
    pal.setColor(QPalette::Highlight, Qt::transparent); // construction of palette
    Combcolor->setPalette(pal); //set the comboBox palette

    int size = Combcolor->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size,size);

    int con=0;
    foreach (QString name, colorNms)
    {
        Combcolor->addItem("", QColor(con));//Adding ComboItems
        pixmap.fill(QColor(name));
        Combcolor->setItemData(con, pixmap, Qt::DecorationRole);//Setting color palettes
        con=con+1;

    }


    QComboBox* comboPattern = new QComboBox;

    foreach (QString name, patternNames)
    {
        comboPattern->addItem(name);//Adding ComboItems

    }

    comboPattern->setCurrentIndex(3);


    QVector<QString> algorytmNames,os1Names,os2Names,dataNames,rodzNames,indexNames;
    os1Names<<"x1"<<"x2";
    os2Names<<"x1"<<"x2";
    dataNames<<"wszystkie"<<"front pareto";



    // for (int i = 0; i < gga->funkcjeRodzajnikow.size(); ++i) {
    //dataNames<<"rozw. przydzielone rodzajnikowi "+QVariant(i+1).toString();
    // }
    rodzNames<<"fun. kryt."<<"param.";
    algorytmNames<<"MOPSO"<<"NSGA-II"<<"GGA";




    for (unsigned int i = 0; i < problem->funkcje.size(); ++i) {
        indexNames<<"f"+QVariant(i+1).toString();
    }

    QComboBox* comboAlgorytm = new QComboBox();



    foreach (QString name, algorytmNames)
    {
        comboAlgorytm->addItem(name);//Adding ComboItems

    }


    QComboBox* comboOs1Rodz = new QComboBox();
    signalMapper = new QSignalMapper(this);


    foreach (QString name, rodzNames)
    {
        comboOs1Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs1Index = new QComboBox();

    foreach (QString name, indexNames)
    {
        comboOs1Index->addItem(name);//Adding ComboItems

    }


    QComboBox* comboOs2Rodz = new QComboBox;

    foreach (QString name, rodzNames)
    {
        comboOs2Rodz->addItem(name);//Adding ComboItems

    }

    QComboBox* comboOs2Index = new QComboBox;

    foreach (QString name, indexNames)
    {
        comboOs2Index->addItem(name);//Adding ComboItems

    }

    QComboBox* comboData = new QComboBox;



    foreach (QString name, dataNames)
    {
        comboData->addItem(name);//Adding ComboItems

    }




    //nazwa os1 os2 rodz_danych wzor kolor
    unsigned int row=widget.plotTableMOPSO->rowCount();
    widget.wykresy2D->insertRow(row);

    //    QTableWidgetItem *colorItem = new QTableWidgetItem;
    //    colorItem->setData(Qt::DisplayRole, QColor("red"));

    //    QTableWidgetItem *os1Item = new QTableWidgetItem;
    //    os1Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *os2Item = new QTableWidgetItem;
    //    os2Item->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *daneItem = new QTableWidgetItem;
    //    daneItem->setData(Qt::DisplayRole, "cos");

    //    QTableWidgetItem *wzorItem = new QTableWidgetItem;
    //    wzorItem->setData(Qt::DisplayRole, "cos");

    QLineEdit* nameItem=new QLineEdit("Wykres");



    widget.wykresy2D->setCellWidget(row, 0, comboAlgorytm);
    widget.wykresy2D->setCellWidget(row, 1, nameItem);
    widget.wykresy2D->setCellWidget(row, 2, comboOs1Rodz);
    widget.wykresy2D->setCellWidget(row, 3, comboOs1Index);
    widget.wykresy2D->setCellWidget(row, 4, comboOs2Rodz);
    widget.wykresy2D->setCellWidget(row, 5, comboOs2Index);
    widget.wykresy2D->setCellWidget(row, 6, comboData);
    widget.wykresy2D->setCellWidget(row, 7, comboPattern);
    widget.wykresy2D->setCellWidget(row,8,Combcolor);
    widget.wykresy2D->resizeColumnToContents(0);
    widget.wykresy2D->horizontalHeader()->setStretchLastSection(true);


    QObject* ac=new AuxClass(row,2);
    connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs1Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentPlot2DIndexChanged(QObject *)));

    ac=new AuxClass(row,4);
    connect(comboOs2Rodz,SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));


    signalMapper->setMapping(comboOs2Rodz,ac );
    // connect(comboOs1Rodz,SIGNAL(currentIndexChanged(int)), this, SLOT());
    connect(signalMapper, SIGNAL(mapped(QObject *)),
            this, SLOT(currentPlot2DIndexChanged(QObject *)));
}


void Forma::AddRowTo3DPlotsTable()
{

}

void Forma::on_bStworzOkno2D_clicked()
{
    //widget.Wykresy->clear();

    if(widget.wykresy2D->rowCount()==0)
    {
        showWarning(tr("Należy dodać przynajmniej 1 wykres"));
        return;
    }

    QWidget* w=new QWidget;
    QCustomPlot* qPlot=new QCustomPlot(w);
    qPlot->setFixedWidth(550);
    qPlot->setFixedHeight(770);
    qPlot->setAutoMargin(false);
    qPlot->setMargin(50,10,10,230);


    //plotsSettingsMOPSO.clear();
    plot2DSettings.push_back(Plot2DSetting());
    plot2DSettings.back().qPlot=qPlot;
    plot2DSettings.back().tabIndex=widget.Wykresy->count()-1;



    widget.Wykresy->addTab(w,"MOPSO");


    QStringList colorNms;
    colorNms<<"darkGreen"<<"green"<<"gray"<<"red"<<"white"<<"blue"<<"cyan"<<"darkMagenta"<<"yellow"<<"darkRed"<<"black"<<"magenta";


    TRACE;


    plot2DSettings.back().windowCaption=widget.nazwaOkna2D->text();
    plot2DSettings.back().xAxisCaption=widget.podpisOsiX2D->text();
    plot2DSettings.back().yAxisCaption=widget.podpisOsiY2D->text();

    int tmp;

    for (int i = 0; i < widget.wykresy2D->rowCount(); ++i) {

        plot2DSettings.back().graphSettings.push_back(GraphSetting());
        plot2DSettings.back().graphSettings.back().algorithm=((QComboBox*) widget.wykresy2D->cellWidget(i, 0))->currentIndex();
        plot2DSettings.back().graphSettings.back().axisSettings.resize(2);
        plot2DSettings.back().graphSettings.back().name=((QLineEdit*) widget.wykresy2D->cellWidget(i, 1))->text();

        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 2))->currentIndex();

        if(tmp==0)
        {
            plot2DSettings.back().graphSettings.back().axisSettings[0].isParam=false;
        }
        else
        {
            plot2DSettings.back().graphSettings.back().axisSettings[0].isParam=true;
        }

        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 3))->currentIndex();

        plot2DSettings.back().graphSettings.back().axisSettings[0].index=tmp;

        /////

        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 4))->currentIndex();

        if(tmp==0)
        {
            plot2DSettings.back().graphSettings.back().axisSettings[1].isParam=false;
        }
        else
        {
            plot2DSettings.back().graphSettings.back().axisSettings[1].isParam=true;
        }

        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 5))->currentIndex();

        plot2DSettings.back().graphSettings.back().axisSettings[1].index=tmp;

        //////
        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 6))->currentIndex();

        if(tmp<2)
        {
            if(tmp==0)
            {
                plot2DSettings.back().graphSettings.back().all=true;
            }
            else
            {
                plot2DSettings.back().graphSettings.back().pareto=true;
            }
        }
        else
        {
            //tmp=tmp-2;
            // plot2DSettings.back().graphSettings.back().indexOfGender=tmp;


        }

        QString str=((QComboBox*) widget.wykresy2D->cellWidget(i, 7))->currentText();
        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 7))->currentIndex();
        plot2DSettings.back().graphSettings.back().patternName=str;
        plot2DSettings.back().graphSettings.back().patternIndex=tmp;

        tmp=((QComboBox*) widget.wykresy2D->cellWidget(i, 8))->currentIndex();

        qDebug()<<"tmp "<<tmp;

        plot2DSettings.back().graphSettings.back().color=colorNms[tmp];

    }


    QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

    plot2DSettings.back().qPlot->setTitle(plot2DSettings.back().windowCaption);
    plot2DSettings.back().qPlot->setAutoMargin(false);
    plot2DSettings.back().qPlot->setMargin(50,10,10,230);
    //  widget.qPlotNSGA_II_PID->setFixedSize(550,550);
    plot2DSettings.back().qPlot->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
    plot2DSettings.back().qPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    plot2DSettings.back().qPlot->legend->setFont(legendFont);
    plot2DSettings.back().qPlot->legend->setPositionStyle(QCPLegend::psManual);
    plot2DSettings.back().qPlot->legend->setPosition(QPoint(50,590));
    plot2DSettings.back().qPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    plot2DSettings.back().qPlot->xAxis->setLabel( plot2DSettings.back().xAxisCaption);
    plot2DSettings.back().qPlot->yAxis->setLabel( plot2DSettings.back().yAxisCaption);



    for (int i = 0; i <  plot2DSettings.back().graphSettings.size(); ++i) {
        plot2DSettings.back().qPlot->addGraph();
        plot2DSettings.back().qPlot->graph(i)->setPen(QColor(plot2DSettings.back().graphSettings[i].color));
        plot2DSettings.back().qPlot->graph(i)->setLineStyle(QCPGraph::lsNone);
        plot2DSettings.back().qPlot->graph(i)->setScatterStyle((QCP::ScatterStyle)plot2DSettings.back().graphSettings[i].patternIndex);//QCPGraph::ssCircle);
        plot2DSettings.back().qPlot->graph(i)->setScatterSize(7);
        plot2DSettings.back().qPlot->graph(i)->setName(plot2DSettings.back().graphSettings[i].name);
        plot2DSettings.back().qPlot->graph(i)->addToLegend();
    }





    while(widget.wykresy2D->rowCount()>0)
    {
        widget.wykresy2D->removeRow(0);
    }

}

void Forma::on_bIterujSymulacja_clicked()
{

}

void Forma::on_qPlotWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug()<<"menu";
}

void Forma::on_bIterujMOPSO_clicked()
{
    // IterujRazMOPSO();
    // aktualizuj_wykres_przystosowania();
    // qDebug()<<"mopso D_EC "<<wskazniki->D_EC(mopso->repozytorium);
    qDebug() << "nsga start";



    bool ok;
    unsigned int il_iter = widget.ilIterMOPSO->text().toInt(&ok, 10);
    if (!ok) {
        showWarning("Niewlasciwa ilosc iteracji");
        return;
    }
    widget.Debug->append(QString("il iteracji ") + QString::number(il_iter));
    qDebug() << "il iteracji " << il_iter << "\n";
    mopso->il_iter = il_iter;
    widget.progressBarMOPSO->setMaximum(il_iter);
    mopso->koniec=false;
    time.start();
    //
    //    QThreadEx nsgaThread;
    //    nsga->moveToThread(&nsgaThread);
    //
    //    (*nsga).connect(&nsgaThread,
    //            SIGNAL(started()),
    //            SLOT(doTheWork()));
    //
    //    nsgaThread.connect(&nsgaThread,
    //                           SIGNAL(finished()),
    //                           SLOT(quit()));
    mopso->start();
}

void Forma::on_pushButton_clicked()
{

    float time=40.0;
    float dt=0.01;
    QVector<QPair<double,double> > dataXY;

    mopso_ssmodel->Iteruj();
    //widget.qPlotMOPSO_PID->clearGraphs();
    while(widget.qPlotMOPSO_PID->graphCount()>3)
    {
       widget.qPlotMOPSO_PID->removeGraph(widget.qPlotMOPSO_PID->graphCount()-1);
    }

    wskazniki->GOL(mopso_ssmodel->repozytorium,mopso_ssmodel->indSortGOL );
    double val=mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->GOL;

    unsigned int tmp;
    tmp=widget.qPlotMOPSO_PID->graphCount();
    if(CheckStabilityOfMySystem(mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[0],mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[1],mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[2])==0)//plotujemy stabilne rozwiazania
    {
    GetStepResponse(&sys,mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[0],mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[1],mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->x[2],time,dt,dataXY);

    AddPlot(widget.qPlotMOPSO_PID,dataXY,""+QVariant(mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[mopso_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::red));

    widget.qPlotMOPSO_PID->graph(tmp)->setLayer("GOL");
    }


    for (int i = 0; i < mopso_ssmodel->repozytorium.size(); ++i) {
        if(CheckStabilityOfMySystem(mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2],time,dt,dataXY);

            if(mopso_ssmodel->indSortGOL[0]==i)
            {

            }
            else
            {
                AddPlot(widget.qPlotMOPSO_PID,dataXY,""+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::black));
            }
            TRACE;
        }
    }


//widget.qPlotMOPSO_PID->replot();

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.0} );
//    }
//    AddPlot(widget.qPlotMOPSO_PID,dataXY,"skok",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.02});
//    }
//    AddPlot(widget.qPlotMOPSO_PID,dataXY,"1.02",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,0.98});
//    }
//    AddPlot(widget.qPlotMOPSO_PID,dataXY,"0.98",QColor(Qt::gray));

    TRACE;
    // ,riseTime,settlingTime
    for (int i = 0; i < mopso_ssmodel->repozytorium.size(); ++i) {
        if(CheckStabilityOfMySystem(mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2],time,dt,dataXY);

            qDebug()<<"osobnik z repozytorium numer "<<i;
            qDebug()<<"Kp "+QVariant(mopso_ssmodel->repozytorium[i]->x[0]).toString()+" ,Ki "+QVariant(mopso_ssmodel->repozytorium[i]->x[1]).toString()+" ,Kd "+QVariant(mopso_ssmodel->repozytorium[i]->x[2]).toString();
            qDebug()<<"maxOvershoot "+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[0]).toString()+" ,riseTime "+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[1]).toString()+" ,settlingTime "+QVariant(mopso_ssmodel->repozytorium[i]->wartFunkcjiKryterialnych[2]).toString();


        }
    }

    qDebug()<<"mopso GOL["<<mopso_ssmodel->indSortGOL[0]<<"]= "<<val;

    for (int i = 0; i < mopso_ssmodel->repozytorium.size(); ++i) {
        qDebug()<<"GOL of rep["<<i<<"]= "<<mopso_ssmodel->repozytorium[i]->GOL<<endl;
    }


}

void Forma::on_pushButton_2_clicked()
{
    float time=40.0;
    float dt=0.01;
    QVector<QPair<double,double> > dataXY;

    gga_ssmodel->Iteruj();
   // widget.qPlotGGA_PID->clearGraphs();
    while(widget.qPlotGGA_PID->graphCount()>3)
    {
       widget.qPlotGGA_PID->removeGraph(widget.qPlotGGA_PID->graphCount()-1);
    }

    wskazniki->GOL(gga_ssmodel->rodzice,gga_ssmodel->indSortGOL );
    double val=gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->GOL;

    unsigned int tmp;
    tmp=widget.qPlotGGA_PID->graphCount();

    if(CheckStabilityOfMySystem(gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[0],gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[1],gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[2])==0)//plotujemy stabilne rozwiazania
    {
    GetStepResponse(&sys,gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[0],gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[1],gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->x[2],time,dt,dataXY);

    AddPlot(widget.qPlotGGA_PID,dataXY,""+QVariant(gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(gga_ssmodel->rodzice[gga_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::red));

    widget.qPlotGGA_PID->graph(tmp)->setLayer("GOL");
    }


    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {

        if(gga_ssmodel->stopZdominowania[i]==0)
        {
            if(CheckStabilityOfMySystem(gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
            {
                GetStepResponse(&sys,gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2],time,dt,dataXY);

                if(gga_ssmodel->indSortGOL[0]==i)
                {
                    //AddPlot(widget.qPlotGGA_PID,dataXY,""+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::red));

                }
                else
                {
                    AddPlot(widget.qPlotGGA_PID,dataXY,""+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::black));
                }
                TRACE;
            }
        }
    }
//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.0} );
//    }
//    AddPlot(widget.qPlotGGA_PID,dataXY,"skok",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.02});
//    }
//    AddPlot(widget.qPlotGGA_PID,dataXY,"1.02",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,0.98});
//    }
//    AddPlot(widget.qPlotGGA_PID,dataXY,"0.98",QColor(Qt::gray));

    TRACE;
    // ,riseTime,settlingTime
    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {
        if(CheckStabilityOfMySystem(gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2],time,dt,dataXY);

            qDebug()<<"osobnik z rodzice numer "<<i;
            qDebug()<<"Kp "+QVariant(gga_ssmodel->rodzice[i]->x[0]).toString()+" ,Ki "+QVariant(gga_ssmodel->rodzice[i]->x[1]).toString()+" ,Kd "+QVariant(gga_ssmodel->rodzice[i]->x[2]).toString();
            qDebug()<<"maxOvershoot "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" ,riseTime "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" ,settlingTime "+QVariant(gga_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString();


        }
    }

    qDebug()<<"gga GOL["<<gga_ssmodel->indSortGOL[0]<<"]= "<<val;

    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {
        qDebug()<<"GOL of rep["<<i<<"]= "<<gga_ssmodel->rodzice[i]->GOL<<endl;
    }

    unsigned int  il_niezdominowanych=0;
    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {
        if(gga_ssmodel->stopZdominowania[i]==0){
            il_niezdominowanych++;
        }
    }

    qDebug()<<"il niezdominowanych osob. "<<il_niezdominowanych<<endl;
    for (int i = 0; i < gga_ssmodel->rozwPrzydzRodzajnikom.size(); ++i) {
        qDebug()<<"rodzajnik["<<i<<"] il. rozw. : "<< gga_ssmodel->rozwPrzydzRodzajnikom[i].size()<<endl;
    }
}

void Forma::on_pushButton_3_clicked()
{
    float time=40.0;
    float dt=0.01;
    QVector<QPair<double,double> > dataXY;

    nsga_ii_ssmodel->Iteruj();
   // widget.qPlotNSGA_II_PID->clearGraphs();
    while(widget.qPlotNSGA_II_PID->graphCount()>3)
    {
       widget.qPlotNSGA_II_PID->removeGraph(widget.qPlotNSGA_II_PID->graphCount()-1);
    }


    wskazniki->GOL(nsga_ii_ssmodel->rodzice,nsga_ii_ssmodel->indSortGOL );
    double val=nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->GOL;

    unsigned int tmp;
    tmp=widget.qPlotNSGA_II_PID->graphCount();

    if(CheckStabilityOfMySystem(nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[0],nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[1],nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[2])==0)//plotujemy stabilne rozwiazania
    {
    GetStepResponse(&sys,nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[0],nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[1],nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->x[2],time,dt,dataXY);

    AddPlot(widget.qPlotNSGA_II_PID,dataXY,""+QVariant(nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[nsga_ii_ssmodel->indSortGOL[0]]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::red));

    widget.qPlotNSGA_II_PID->graph(tmp)->setLayer("GOL");
    }

    for (int i = 0; i < nsga_ii_ssmodel->F[0].size(); ++i) {


        if(CheckStabilityOfMySystem(nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2],time,dt,dataXY);

            if(nsga_ii_ssmodel->indSortGOL[0]==i)
            {
                //AddPlot(widget.qPlotNSGA_II_PID,dataXY,""+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::red));

            }
            else
            {
                AddPlot(widget.qPlotNSGA_II_PID,dataXY,""+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString(),QColor(Qt::black));
            }
            TRACE;
        }

    }

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.0} );
//    }
//    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"skok",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,1.02});
//    }
//    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"1.02",QColor(Qt::gray));

//    dataXY.clear();
//    for (float  i= 0;  i< time; i+=dt) {
//        dataXY.push_back({i,0.98});
//    }
//    AddPlot(widget.qPlotNSGA_II_PID,dataXY,"0.98",QColor(Qt::gray));

    TRACE;
    // ,riseTime,settlingTime
    for (int i = 0; i < nsga_ii_ssmodel->F[0].size(); ++i) {
        if(CheckStabilityOfMySystem(nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2],time,dt,dataXY);

            qDebug()<<"osobnik z rodzice numer "<<i;
            qDebug()<<"Kp "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[0]).toString()+" ,Ki "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[1]).toString()+" ,Kd "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[2]).toString();
            qDebug()<<"maxOvershoot "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[0]).toString()+" ,riseTime "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[1]).toString()+" ,settlingTime "+QVariant(nsga_ii_ssmodel->rodzice[i]->wartFunkcjiKryterialnych[2]).toString();


        }
    }

    qDebug()<<"nsga_ii GOL["<<nsga_ii_ssmodel->indSortGOL[0]<<"]= "<<val;

    for (int i = 0; i < nsga_ii_ssmodel->rodzice.size(); ++i) {
        qDebug()<<"GOL of rep["<<i<<"]= "<<nsga_ii_ssmodel->rodzice[i]->GOL<<endl;
    }
}

void Forma::on_bDodajWykres3D_clicked()
{

}

void Forma::on_bStworzOkno3D_clicked()
{

}

void Forma::on_bInicjalizujGGA_PID_clicked()
{
    QVector<QPair<double,double> > dataXY;

    gga_ssmodel->Inicjalizuj();
    while(widget.qPlotGGA_PID->graphCount()>3)
    {
       widget.qPlotGGA_PID->removeGraph(widget.qPlotGGA_PID->graphCount()-1);
    }
    for (int i = 0; i < gga_ssmodel->rodzice.size(); ++i) {
        if(CheckStabilityOfMySystem(gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,gga_ssmodel->rodzice[i]->x[0],gga_ssmodel->rodzice[i]->x[1],gga_ssmodel->rodzice[i]->x[2],40.0,0.01,dataXY);
            AddPlot(widget.qPlotGGA_PID,dataXY,""+QVariant(gga_ssmodel->rodzice[i]->x[0]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->x[1]).toString()+" , "+QVariant(gga_ssmodel->rodzice[i]->x[2]).toString(),QColor(Qt::black));

        }
    }
}

void Forma::on_bInicjalizujNSGA_II_PID_clicked()
{
    QVector<QPair<double,double> > dataXY;

    nsga_ii_ssmodel->Inicjalizuj();
    while(widget.qPlotNSGA_II_PID->graphCount()>3)
    {
       widget.qPlotNSGA_II_PID->removeGraph(widget.qPlotNSGA_II_PID->graphCount()-1);
    }
    for (int i = 0; i < nsga_ii_ssmodel->rodzice.size(); ++i) {
        if(CheckStabilityOfMySystem(nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,nsga_ii_ssmodel->rodzice[i]->x[0],nsga_ii_ssmodel->rodzice[i]->x[1],nsga_ii_ssmodel->rodzice[i]->x[2],40,0.01,dataXY);
            AddPlot(widget.qPlotNSGA_II_PID,dataXY,""+QVariant(nsga_ii_ssmodel->rodzice[i]->x[0]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[1]).toString()+" , "+QVariant(nsga_ii_ssmodel->rodzice[i]->x[2]).toString(),QColor(Qt::black));

        }
    }
}

void Forma::on_bInicjalizujMOPSO_PID_clicked()
{
    QVector<QPair<double,double> > dataXY;

    mopso_ssmodel->Inicjalizuj();
    while(widget.qPlotMOPSO_PID->graphCount()>3)
    {
       widget.qPlotMOPSO_PID->removeGraph(widget.qPlotMOPSO_PID->graphCount()-1);
    }
    for (int i = 0; i < mopso_ssmodel->repozytorium.size(); ++i) {
        if(CheckStabilityOfMySystem(mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
            GetStepResponse(&sys,mopso_ssmodel->repozytorium[i]->x[0],mopso_ssmodel->repozytorium[i]->x[1],mopso_ssmodel->repozytorium[i]->x[2],40.0,0.01,dataXY);
            AddPlot(widget.qPlotMOPSO_PID,dataXY,""+QVariant(mopso_ssmodel->repozytorium[i]->x[0]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[i]->x[1]).toString()+" , "+QVariant(mopso_ssmodel->repozytorium[i]->x[2]).toString(),QColor(Qt::black));

        }
    }
    widget.qPlotMOPSO_PID->replot();
    widget.qPlotMOPSO_PID->rescaleAxes();

}
