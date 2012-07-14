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
//int Solution::tmp;

Forma::Forma() {
    nsga_ii = NULL;
    wskazniki = new Wskazniki();
    flaga_bledu = false;
    widget.setupUi(this);
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
    plot3d->rosenbrock->parser.SetExpr("log((1 - x)*(1 - x) + 100 * (y - x * x)*(y - x * x)) / 8");
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
    connect(widget. bIterujMOPSO, SIGNAL(clicked()), this, SLOT(sIterujMOPSO()));
    connect(widget.Funkcje, SIGNAL(currentIndexChanged(int)), this, SLOT(Zmiana_funkcji()));
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
    connect(widget.bIterujRazMOPSO, SIGNAL(clicked()), this, SLOT(sIterujRazMOPSO()));

    connect(widget.bInicjalizujNSGA, SIGNAL(clicked()), this, SLOT(sInicjalizujNSGA()));
    connect(widget.bIterujNSGA, SIGNAL(clicked()), this, SLOT(sIterujNSGA()));


    connect(widget.bStopNSGA_II, SIGNAL(clicked()), this, SLOT(sStopNSGA()));
    connect(widget.bStopMOPSO, SIGNAL(clicked()), this, SLOT(sStopMOPSO()));
    

    connect(widget.bInicjalizujGGA, SIGNAL(clicked()), this, SLOT(sInicjalizujGGA()));
    connect(widget.bIterujRazGGA, SIGNAL(clicked()), this, SLOT(sIterujRazGGA()));
    connect(widget.bAttachFunToGender, SIGNAL(clicked()), this, SLOT(sAttachFunToGender()));
    connect(widget.bRemoveFun, SIGNAL(clicked()), this, SLOT(sRemoveFun()));
    connect(widget.bRemoveAllFun, SIGNAL(clicked()), this, SLOT(sRemoveAllFun()));

    plot3d->resize(600, 570);


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

    wykresGOL = new Wykres2d((QWidget*) widget.PlotGOL, 500, 500, "Globalny poziom optymalności", "Pokolenia", "GOL");

    //////////////////////////////////////

    //    qPlot=new QCustomPlot( (QWidget*) widget.qPlotWidget);

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

    plot2d = new QwtPlot(QwtText("Two Curves"), (QWidget*) widget.Plot2D);
    plot2d->setFixedSize(500, 500);
    // add curves
    curve1 = new QwtPlotCurve("Curve 1");
    curve2 = new QwtPlotCurve("Curve 2");


    points_nondom = new QwtPlotCurve("Test Curve");
    points_nondom->setStyle(QwtPlotCurve::NoCurve);
    points_nondom->setData(new CurveData());

    points_nondom->setSymbol(new QwtSymbol(QwtSymbol::XCross,
                                           Qt::NoBrush, QPen(Qt::blue), QSize(4, 4)));

    points_nondom->attach(plot2d);



    points_dom = new QwtPlotCurve("Test Curve");
    points_dom->setStyle(QwtPlotCurve::NoCurve);
    points_dom->setData(new CurveData());

    points_dom->setSymbol(new QwtSymbol(QwtSymbol::XCross,
                                        Qt::NoBrush, QPen(Qt::red), QSize(4, 4)));

    points_dom->attach(plot2d);

    //d_directPainter = new QwtPlotDirectPainter(plot2d);
    // copy the data into the curves
    //curve1->setRawSamples();
    //  curve2->setData(...);

    plot2d->setAxisAutoScale(plot2d->xBottom, true);
    plot2d->setAxisAutoScale(plot2d->yLeft, true);
    //curve1->attach(plot2d);
    //  curve2->attach(plot2d);

    // finally, refresh the plot
    plot2d->replot();


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
    plot3d->rosenbrock->create();

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

void Forma::sStopMOPSO() {
    //mopso->koniec=true;
    mopso->stop();

}

void Forma::sDoneMOPSO() {
    aktualizuj_wykres_przystosowania();

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

    Dodaj_zakresy_zmiennych();

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

            data_dom->append(QPointF(mopso->populacja[i]->fitness[0], mopso->populacja[i]->fitness[1]));

        }
    }

    for (int i = 0; i < mopso->repozytorium.size(); i++) {


        data_nondom->append(QPointF(mopso->repozytorium[i]->fitness[0], mopso->repozytorium[i]->fitness[1]));



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
    wykresGOL->wyczysc();




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
    aktualizuj_wykres_przystosowania();

    // qDebug() << "Generuje kostki\n";
    //  pso->generuj_kostki();




}

void Forma::sIterujRazMOPSO() {


    IterujRazMOPSO();
    aktualizuj_wykres_przystosowania();
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

    mopso->Iteruj();
    // pso->wyswietl_repozytorium();
    double gol = wskazniki->GOL(mopso->repozytorium, problem);
    //      vGol.push_back(QPointF((double)pso->num_iter,gol));
    qDebug() << "GOL = " << gol << "\n";
    wykresGOL->dodajPunkt(QPointF((double) mopso->num_iter, gol));
    wykresGOL->aktualizuj();
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

void Forma::PokazWszystkieFronty()
{
    QVector< QVector<Solution*> > * Fi=gga->FastNonDominatedSort();
    qDebug()<<"ilosc frontow "<<Fi->size()<<"\n";

    widget.qPlotWidget->clearGraphs();

    unsigned int tmp=255/(Fi->size()-1);
    for(unsigned int i=0;i<Fi->size();i++)
    {
        widget.qPlotWidget->addGraph();
        widget.qPlotWidget->graph(i)->setPen(QColor(255, tmp*i, 0, 255));
        widget.qPlotWidget->graph(i)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotWidget->graph(i)->setScatterStyle(QCPGraph::ssPlus);
        widget.qPlotWidget->graph(i)->setScatterSize(4);
        widget.qPlotWidget->graph(i)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));

        for(unsigned int j=0;j<(*Fi)[i].size();j++)
        {
            widget.qPlotWidget->graph(i)->addData((*Fi)[i][j]->wartFunkcjiKryterialnych[0],(*Fi)[i][j]->wartFunkcjiKryterialnych[1]);

        }

    }



}

void Forma::AktualizujWykresGGA()
{


    qDebug()<<"aktualizuje wykres dla GGA";
    widget.qPlotWidget->graph(0)->clearData();
    widget.qPlotWidget->graph(1)->clearData();
    widget.qPlotWidget->graph(2)->clearData();
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
            widget.qPlotWidget->graph(2)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
        }
        else
        {

            if(gga->przydzieloneRodzajniki[i]==0)
            {
                widget.qPlotWidget->graph(0)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
            }
            else
            {
                widget.qPlotWidget->graph(1)->addData(gga->rodzice[i]->wartFunkcjiKryterialnych[0],gga->rodzice[i]->wartFunkcjiKryterialnych[1]);
            }

        }
        qDebug()<<"p["<<i<<"] "<<gga->rodzice[i]->wartFunkcjiKryterialnych[0]<<", "<<gga->rodzice[i]->wartFunkcjiKryterialnych[1];
        qDebug()<<"rodzajnik "<<gga->przydzieloneRodzajniki[i];

    }







    // widget.qPlotWidget->graph(0)->rescaleAxes(true);


    widget.qPlotWidget->xAxis->setRange(0,max(maksx,maksy));
    widget.qPlotWidget->yAxis->setRange(0,max(maksx,maksy));


    widget.qPlotWidget->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    widget.qPlotWidget->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    widget.qPlotWidget->setInteraction(QCustomPlot::iSelectPlottables); //

    // PokazWszystkieFronty();

    widget.qPlotWidget->replot();
}

void Forma::sInicjalizujNSGA() {
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

    aktualizuj_wykres_przystosowania(nsga_ii->F);

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
    ////////

    //    widget.qPlotWidget->addGraph();
    //    widget.qPlotWidget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
    //    widget.qPlotWidget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
    //    widget.qPlotWidget->addGraph();
    //    widget.qPlotWidget->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
    //    // generate some points of data (y0 for first, y1 for second graph):
    //    QVector<double> x(250), y0(250), y1(250);
    //    for (int i=0; i<250; ++i)
    //    {
    //      x[i] = i;
    //      y0[i] = exp(-i/150.0)*cos(i/10.0); // exponentially decaying cosine
    //      y1[i] = exp(-i/150.0);             // exponential envelope
    //    }
    //    // configure right and top axis to show ticks but no labels (could've also just called widget.qPlotWidget->setupFullAxesBox):
    //    widget.qPlotWidget->xAxis2->setVisible(true);
    //    widget.qPlotWidget->xAxis2->setTickLabels(false);
    //    widget.qPlotWidget->yAxis2->setVisible(true);
    //    widget.qPlotWidget->yAxis2->setTickLabels(false);
    //    // make left and bottom axes always transfer their ranges to right and top axes:
    //    connect(widget.qPlotWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), widget.qPlotWidget->xAxis2, SLOT(setRange(QCPRange)));
    //    connect(widget.qPlotWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), widget.qPlotWidget->yAxis2, SLOT(setRange(QCPRange)));
    //    // pass data points to graphs:
    //    widget.qPlotWidget->graph(0)->setData(x, y0);
    //    widget.qPlotWidget->graph(1)->setData(x, y1);
    //    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    //    widget.qPlotWidget->graph(0)->rescaleAxes();
    //    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    //    widget.qPlotWidget->graph(1)->rescaleAxes(true);
    //    // Note: we could have also just called widget.qPlotWidget->rescaleAxes(); instead
    //    // make range moveable by mouse interaction (click and drag):
    //    widget.qPlotWidget->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    //    widget.qPlotWidget->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    //    widget.qPlotWidget->setInteraction(QCustomPlot::iSelectPlottables); //



    qDebug()<<"wykres"<<endl;
    //widget.qPlotWidget

    // setup for graph 4: key axis right, value axis top
    // will contain parabolically distributed data points with some random perturbance

    if(widget.qPlotWidget->graphCount()==0)
    {

        QTextCodec::setCodecForTr (QTextCodec::codecForName ("UTF-8"));

        widget.qPlotWidget->setTitle(tr("Wykres prezentujący rozwiązania"));
        widget.qPlotWidget->setAutoMargin(false);
        widget.qPlotWidget->setMargin(50,10,10,100);
        //  widget.qPlotWidget->setFixedSize(550,550);
        widget.qPlotWidget->setLocale(QLocale(QLocale::Polish, QLocale::Poland));
        widget.qPlotWidget->legend->setVisible(true);
        QFont legendFont = font();  // start out with MainWindow's font..
        legendFont.setPointSize(9); // and make a bit smaller for legend
        widget.qPlotWidget->legend->setFont(legendFont);
        widget.qPlotWidget->legend->setPositionStyle(QCPLegend::psManual);
        widget.qPlotWidget->legend->setPosition(QPoint(50,560));
        widget.qPlotWidget->legend->setBrush(QBrush(QColor(255,255,255,230)));


        QPen pen1(Qt::green);
        QPen pen2(Qt::red);

        widget.qPlotWidget->addGraph();
        widget.qPlotWidget->graph(0)->setPen(QColor(255, 0, 0, 255));
        widget.qPlotWidget->graph(0)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotWidget->graph(0)->setScatterStyle(QCPGraph::ssPlus);
        widget.qPlotWidget->graph(0)->setScatterSize(4);
        widget.qPlotWidget->graph(0)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 1"));


        widget.qPlotWidget->addGraph();
        widget.qPlotWidget->graph(1)->setPen(QColor(0, 0, 255, 255));
        widget.qPlotWidget->graph(1)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotWidget->graph(1)->setScatterStyle(QCPGraph::ssPlus);
        widget.qPlotWidget->graph(1)->setScatterSize(4);
        widget.qPlotWidget->graph(1)->setName(tr("Rozwiązania przyporządkowane rodzajnikowi 2"));

        widget.qPlotWidget->addGraph();
        widget.qPlotWidget->graph(2)->setPen(QColor(0, 100, 0, 255));
        widget.qPlotWidget->graph(2)->setLineStyle(QCPGraph::lsNone);
        widget.qPlotWidget->graph(2)->setScatterStyle(QCPGraph::ssPlus);
        widget.qPlotWidget->graph(2)->setScatterSize(4);
        widget.qPlotWidget->graph(2)->setName(tr("Front Pareto"));



        widget.qPlotWidget->graph(0)->addToLegend();
        widget.qPlotWidget->graph(1)->addToLegend();
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


    //widget.qPlotWidget->graph(0)->addData(4,4);
    //  widget.qPlotWidget->graph(0)->addData(200,333);


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
    AktualizujWykresGGA();
    TRACE;





   qDebug()<<"spacing "<<gga->RozpFrontParWarFunKryt();

}

void Forma::sIterujGGA()
{
}

void Forma::sIterujRazGGA()
{
    qDebug()<<"iteruj raz GGA\n";
    gga->Iteruj();
    AktualizujWykresGGA();
    qDebug()<<"spacing "<<gga->RozpFrontParWarFunKryt();


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
