/* 
 * File:   Wykres2d.cpp
 * Author: marcin
 * 
 * Created on 6 wrzesień 2011, 22:52
 */

#include <qvector.h>

#include "Wykres2d.h"

Wykres2d::Wykres2d() {
}

Wykres2d::Wykres2d(QWidget *p,int szer,int wys,QString tytul,QString x_napis,QString y_napis)
{
    plot2d = new QwtPlot(QwtText("GOL"), (QWidget*) p);
    plot2d->setFixedSize(szer, wys);
    plot2dCurve= new QwtPlotCurve("GOL");
    //plotGOL_data->setStyle(QwtPlotCurve::NoCurve);
   // plotGOL_data->setData(new CurveData());
    plot2dCurve->attach(plot2d);
    plot2d->setAxisAutoScale(plot2d->xBottom, true);
    plot2d->setAxisAutoScale(plot2d->yLeft, true);
    plot2d->setTitle(tytul);
    plot2d->setAxisTitle(QwtPlot::xBottom, x_napis);
    plot2d->setAxisTitle(QwtPlot::yLeft, y_napis);
    
}

Wykres2d::Wykres2d(const Wykres2d& orig) {
}

Wykres2d::~Wykres2d() {
}
void Wykres2d::dodajPunkt(QPointF p)
{
    punkty.push_back(p);
    
    
}
void Wykres2d::aktualizuj()
{
    CurveData *data = static_cast<CurveData *> (plot2dCurve->data()); 
    data->clear();
    for (int i = 0; i < punkty.size(); i++) {
        
        data->append(punkty[i]);

    }

      
    
      plot2d->replot();
    
    
}

void Wykres2d::wyczysc()
{
    CurveData *data = static_cast<CurveData *> (plot2dCurve->data()); 
    data->clear();
    punkty.clear();
     plot2d->replot();
    
}