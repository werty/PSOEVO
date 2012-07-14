/* 
 * File:   struktury.h
 * Author: werty
 *
 * Created on 3 sierpień 2011, 01:37
 */

#ifndef STRUKTURY_H
#define	STRUKTURY_H
#include <QDebug>
#include <QVector>
#include "Particle.h"
#include "Solution.h"

struct Hiperkostka {
    QVector<int> czasteczki;
    QVector<int> polozenie;




};

bool lessThenVectorInt(const QVector<int> &v1, const QVector<int> &v2);

bool lessThenHiperkostka(const Hiperkostka & h1, const Hiperkostka & h2);

bool equalVectorInt(const QVector<int> &v1, const QVector<int> &v2);

bool equalHiperkostka(const Hiperkostka & h1, const Hiperkostka & h2);

void joinHiperkostki(QVector<Hiperkostka> &h);

bool  lessThenSolution(const Solution * s1, const Solution * s2);

bool CrowdedComparisonOperator(const Solution * s1, const Solution * s2);

#endif	/* STRUKTURY_H */

