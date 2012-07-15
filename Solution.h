/* 
 * File:   Solution.h
 * Author: marcin
 *
 * Created on 9 wrzesień 2011, 23:27
 */

#ifndef SOLUTION_H
#define	SOLUTION_H
#include <QDebug>
#include <QVector>
#include "Chromosome.h"
#include "uzyteczne_funkcje.h"
class Solution {
public:
    static int tmp;
    int il_zmiennych;
    int il_funkcji;
    int il_bitow;
    double zatloczenie;
    int ranga;
    double GOL;
    double fi;//dla GGA znormalizowany stopien suboptymalnosci
    QVector<Chromosome> genotyp;
    QVector<double> x;//fenotyp
    QVector<double> wartFunkcjiKryterialnych;
    QVector<double> przystosowanie;
    QVector<double> przystosowaniePrzeskalowane;
   
    Solution();
    Solution(int il_zmiennych,int il_bitow, int il_funkcji);
    Solution(const Solution& orig);
    Solution & operator = (const Solution & b);
    bool Dominate(const Solution & b);
    bool Dominate(const Solution & b,QVector<unsigned int> vChosenFunctions);
    bool operator > (const Solution & b);//operator zdominowania

    virtual ~Solution();
private:
    
};

#endif	/* SOLUTION_H */

