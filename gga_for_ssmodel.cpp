#include "gga_for_ssmodel.h"

GGA_for_ssmodel::GGA_for_ssmodel()
{

}

//GGA_for_ssmodel::GGA_for_ssmodel(unsigned int wielk_pop, unsigned int il_bitow, double prawd_mutacji, double prawd_krzyz, unsigned int ilOsobElit, QVector<QString> nazwyRodzajnikow, QVector<QVector<unsigned int> > funkcjeRodzajnikow, Problem *problem)
//{

//}

void GGA_for_ssmodel::WyznaczWartFunkcjiKryterialnych()
{

    QVector<QPair<double,double> > dataXY;

    for (int i = 0; i < rodzice.size(); ++i) {

        if(CheckStabilityOfMySystem(rodzice[i]->x[0],rodzice[i]->x[1],rodzice[i]->x[2])==0)//plotujemy stabilne rozwiazania
        {
        GetStepResponse(&sys,rodzice[i]->x[0],rodzice[i]->x[1],rodzice[i]->x[2],30,0.01,dataXY);

       // GetStepResponseCharacteristics( dataXY,maxOvershoot,riseTime,settlingTime);
        GetStepResponseCharacteristics( dataXY,rodzice[i]->wartFunkcjiKryterialnych[0],rodzice[i]->wartFunkcjiKryterialnych[1],rodzice[i]->wartFunkcjiKryterialnych[2]);

        if(rodzice[i]->wartFunkcjiKryterialnych[2]>20)
        {
            rodzice[i]->wartFunkcjiKryterialnych[2]+=100.0;
            rodzice[i]->wartFunkcjiKryterialnych[1]+=100.0;
            rodzice[i]->wartFunkcjiKryterialnych[0]+=100.0;
        }

        }
        else
        {
            rodzice[i]->wartFunkcjiKryterialnych[0]=10000;
            rodzice[i]->wartFunkcjiKryterialnych[1]=100;
            rodzice[i]->wartFunkcjiKryterialnych[2]=100;
        }
    }
}


void GGA_for_ssmodel::Inicjalizuj()
{
    qsrand(QTime::currentTime().msec());
    qDebug() << "P.size() " <<rodzice.size() << "\n";
    for (int i_pop = 0; i_pop <rodzice.size(); i_pop++) {
        // DINFO;
        for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
            //  DINFO;
            for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
                // DINFO;
                 qDebug()<<"i_pop "<<i_pop<<" i_zmienna "<<i_zmienna<<" i_bit"<<i_bit<<"\n";
                if (((float) qrand() / (float) RAND_MAX) > 0.5) {
                    //   DINFO;
                    rodzice[i_pop]->genotyp[i_zmienna][i_bit] = true;
                } else {
                    // DINFO;
                    rodzice[i_pop]->genotyp[i_zmienna][i_bit] = false;
                }
                //  DINFO;
            }

        }
    }


    TRACE;
    DekodujGenotyp();
    TRACE;
    qDebug()<<__FILE__<<" at "<<__LINE__;

    WyznaczWartFunkcjiKryterialnych();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    TRACE;
    WyznaczPrzystosowanie();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    TRACE;
    PrzeskalujPrzystosowanie();
    qDebug()<<__FILE__<<" at "<<__LINE__;
    WyznaczStopnieZdominowania();

    WyznaczRangi();




    zainicjalizowany=true;

}

