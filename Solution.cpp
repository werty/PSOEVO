/*
 * File:   Solution.cpp
 * Author: marcin
 * 
 * Created on 9 wrzesień 2011, 23:27
 */

#include "Solution.h"

Solution::Solution() {
}
Solution::Solution(int il_zmiennych,int il_bitow, int il_funkcji)
{
    this->il_zmiennych  = il_zmiennych;
    this->il_funkcji    = il_funkcji;
    this->il_bitow=il_bitow;
    
    genotyp.resize(il_zmiennych);
    for (int i = 0; i < il_zmiennych; i++) {
        genotyp[i].Resize(il_bitow);
    }
    fenotyp.resize(il_zmiennych);
    wartFunkcjiKryterialnych.resize(il_funkcji);
    przystosowanie.resize(il_funkcji);
    przystosowaniePrzeskalowane.resize(il_funkcji);
   
    
    
}
Solution::Solution(const Solution& orig) {
    //qDebug()<<"bla\n";
    this->il_zmiennych  = orig.il_zmiennych;
    this->il_funkcji    = orig.il_funkcji;
    this->il_bitow=orig.il_bitow;
    this->ranga=orig.ranga;
    this->zatloczenie=orig.zatloczenie;
    
    genotyp.resize(orig.il_zmiennych);
    for (int i = 0; i < orig.il_zmiennych; i++) {
        genotyp[i].Resize(il_bitow);
    }
    fenotyp.resize(orig.il_zmiennych);
    wartFunkcjiKryterialnych.resize(il_funkcji);
    przystosowanie.resize(orig.il_funkcji);
    przystosowaniePrzeskalowane.resize(orig.il_funkcji);
    
    
    for (int i_zmienna = 0; i_zmienna < orig.il_zmiennych; i_zmienna++) {
        fenotyp[i_zmienna]=orig.fenotyp[i_zmienna];
        for (int i_bit = 0; i_bit < orig.il_bitow; i_bit++) {
            genotyp[i_zmienna][i_bit]=orig.genotyp[i_zmienna][i_bit];
        }
    }

    for (int i_funkcja = 0; i_funkcja < orig.il_funkcji; i_funkcja++) {
        wartFunkcjiKryterialnych[i_funkcja]=orig.wartFunkcjiKryterialnych[i_funkcja];
        przystosowanie[i_funkcja]=orig.przystosowanie[i_funkcja];
        przystosowaniePrzeskalowane[i_funkcja]=orig.przystosowaniePrzeskalowane[i_funkcja];
       // qDebug()<<"f = "<<przystosowaniePrzeskalowane[i_funkcja]<<"\n";
    }

    
}
Solution & Solution::operator = (const Solution & orig)
{
    for (int i_zmienna = 0; i_zmienna < orig.il_zmiennych; i_zmienna++) {
        fenotyp[i_zmienna]=orig.fenotyp[i_zmienna];
        for (int i_bit = 0; i_bit < orig.il_bitow; i_bit++) {
            genotyp[i_zmienna][i_bit]=orig.genotyp[i_zmienna][i_bit];
        }
    }

    for (int i_funkcja = 0; i_funkcja < orig.il_funkcji; i_funkcja++) {
        wartFunkcjiKryterialnych[i_funkcja]=orig.wartFunkcjiKryterialnych[i_funkcja];
        przystosowanie[i_funkcja]=orig.przystosowanie[i_funkcja];
        przystosowaniePrzeskalowane[i_funkcja]=orig.przystosowaniePrzeskalowane[i_funkcja];
    }
    
    return *this;
}
Solution::~Solution() {



}
bool Solution::Dominate(const Solution & b)
{
    for (int i = 0; i < il_funkcji; i++) {
       // qDebug()<<"? "<<b.przystosowaniePrzeskalowane[i]<<">="<<this->przystosowaniePrzeskalowane[i]<<"\n";
           if (b.przystosowaniePrzeskalowane[i] >= this->przystosowaniePrzeskalowane[i]){
              // qDebug()<<"false\n";
               return false;
           }
    }
       // qDebug()<<"true\n";
   return true;

}
bool Solution::Dominate(const Solution & b,QVector<unsigned int> vChosenFunctions)
{
    for (int i = 0; i < vChosenFunctions.size(); i++) {
       // qDebug()<<"? "<<b.przystosowaniePrzeskalowane[i]<<">="<<this->przystosowaniePrzeskalowane[i]<<"\n";
           if (b.przystosowaniePrzeskalowane[vChosenFunctions[i]] >= this->przystosowaniePrzeskalowane[vChosenFunctions[i]]){
              // qDebug()<<"false\n";
               return false;
           }
    }
       // qDebug()<<"true\n";
   return true;

}

bool Solution::operator > (const Solution & b)
{
     for (int i = 0; i < il_funkcji; i++) {
        // qDebug()<<"? "<<b.przystosowaniePrzeskalowane[i]<<">="<<this->przystosowaniePrzeskalowane[i]<<"\n";
            if (b.przystosowaniePrzeskalowane[i] >= this->przystosowaniePrzeskalowane[i]){ 
               // qDebug()<<"false\n";
                return false;
            }   
     }
        // qDebug()<<"true\n";
    return true; 
}
