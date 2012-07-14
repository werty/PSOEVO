#ifndef GENOTYP_H
#define GENOTYP_H

#include <QVector>
#include <QString>
#include <iostream>
#include <QtCore/qmath.h>


class Chromosome
{
public:
    bool isChanged;//czy nalezy wyznaczyc wartosc fenotypu
    unsigned long value;
    QVector<bool> bits;
    const bool& operator[] (unsigned int)const;
    bool& operator[] (unsigned int);
    void Resize(unsigned int size);
    QString GetString();
    unsigned long GetValue();
    double GetNormalisedValue();//zwraca znormalizowana wartosc z przedzialu 0-1
    Chromosome();
    Chromosome(unsigned int numOfBits,bool inState);
};

#endif // GENOTYP_H
