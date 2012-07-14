/* 
 * File:   Wykres2d.h
 * Author: marcin
 *
 * Created on 6 wrzesień 2011, 22:52
 */

#ifndef WYKRES2D_H
#define	WYKRES2D_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_canvas.h>
#include <qwt_symbol.h>
#include <qwt_plot_directpainter.h>
#include "qwt3d/qwt3d_gridplot.h"
#include "qwt3d/qwt3d_function.h"


class CurveData : public QwtArraySeriesData<QPointF> {
public:

    CurveData() {
    }

    virtual QRectF boundingRect() const {
        if (d_boundingRect.width() < 0.0)
            d_boundingRect = qwtBoundingRect(*this);

        return d_boundingRect;
    }

    inline void append(const QPointF &point) {
        d_samples += point;
    }

    void clear() {
        d_samples.clear();
        d_samples.squeeze();
        d_boundingRect = QRectF(0.0, 0.0, -1.0, -1.0);
    }
};


class Wykres2d {
public:
    QwtPlot *plot2d;
    QwtPlotCurve *plot2dCurve;
    QVector<QPointF>punkty;
    Wykres2d();
    Wykres2d(QWidget *p,int szer,int wys,QString tytul,QString x_napis,QString y_napis);
    Wykres2d(const Wykres2d& orig);
    void dodajPunkt(QPointF p);
    void aktualizuj();
    void wyczysc();
    virtual ~Wykres2d();
private:

};

#endif	/* WYKRES2D_H */

