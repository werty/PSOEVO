#include "GGA.h"
#include <utility>
#include <functional>


GGA::GGA(QObject *parent) : QThread(parent)
{


}
/**
 * @brief GGA::GGA
 * @param wielk_pop
 * @param il_bitow
 * @param prawd_mutacji
 * @param prawd_krzyz
 * @param problem
 */
GGA::GGA(unsigned int wielk_pop, unsigned int il_bitow, double prawd_mutacji, double prawd_krzyz,unsigned int ilOsobElit, QVector<QString> nazwyRodzajnikow,QVector<QVector<unsigned int> > funkcjeRodzajnikow,Problem *problem)
{
    this->funkcjeRodzajnikow=funkcjeRodzajnikow;
    this->nazwyRodzajnikow=nazwyRodzajnikow;
    this->num_iter = 0;
    this->F = NULL;
    this->c = 2.0;
    this->wiel_pop = wielk_pop;
    this->il_bitow = il_bitow;
    this->prawd_mutacji = prawd_mutacji;
    this->prawd_krzyz = prawd_krzyz;
    this->problem = problem;
    this->ilOsElitarnych=ilOsobElit;
    this->ilOsNieElit=wielk_pop-ilOsElitarnych;

    if(ilOsElitarnych%2!=0)
    {
        ilOsNieElit+=1;

    }

    this->wiel_pop=ilOsElitarnych+ilOsNieElit;

    for (int i = 0; i < wiel_pop; i++) {
        rodzice.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));
        rodzenstwo.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));
    }

    //    for (int i = 0; i < 2 * wiel_pop; i++) {
    //        rodzice.push_back(new Solution(problem->zmienne.size(), il_bitow, problem->parseryFunkcji.size()));


    //    }

    przydzieloneRodzajniki.resize(wielk_pop);
    rozwPrzydzRodzajnikom.resize(funkcjeRodzajnikow.size());

    rangiSuboptymalnosci.resize(wielk_pop);
    znormRangiSuboptymalnosci.resize(wielk_pop);

    for(unsigned int i=0;i<wielk_pop;i++)
    {
        rangiSuboptymalnosci[i].resize(nazwyRodzajnikow.size());
        znormRangiSuboptymalnosci[i].resize(nazwyRodzajnikow.size());
    }

}

GGA::~GGA()
{

}

void GGA::GOL(QVector<unsigned int> &indSortGOL)
{
    indSortGOL.clear();
    QVector<float> maksWartoscPrzystosowania(problem->funkcje.size(),0);
    //wyznaczamy maksymalne wartosci przystosowania przeskalowanego dla kazdej funckcji kryterialnej
    for(unsigned int i=0;i<rodzice.size();i++)
    {
        for(unsigned int j=0;j<problem->funkcje.size();j++)
        {
            if(rodzice[i]->przystosowaniePrzeskalowane[j]>maksWartoscPrzystosowania[j])
            {
                maksWartoscPrzystosowania[j]=rodzice[i]->przystosowaniePrzeskalowane[j];
            }
        }
    }

    //wyznaczamy gol czyli najmniejsza wartosc ze znormalizowanych wartosci przesk przyst
    float wartoscGOL;

    float tmp;
    indSortGOL.resize(rodzice.size());//indeksy osobnikow posortowanych od najw do najm GOL'a
    for(unsigned int i=0;i<rodzice.size();i++)
    {
        indSortGOL[i]=i;
        wartoscGOL=rodzice[i]->przystosowaniePrzeskalowane[0]/maksWartoscPrzystosowania[0];
        for(unsigned int j=1;j<problem->funkcje.size();j++)
        {
            tmp=rodzice[i]->przystosowaniePrzeskalowane[j]/maksWartoscPrzystosowania[j];
            if(tmp<wartoscGOL)
            {
                wartoscGOL=tmp;
            }
        }
        rodzice[i]->GOL=wartoscGOL;
    }

    using namespace std::placeholders;

    qSort(indSortGOL.begin(),indSortGOL.end(),bind(&GGA::porownajWartGOL,this,_1,_2));


}

double GGA::RozpFrontParWarFunKryt()
{
    QList<QVector<double> >lista;

    for(unsigned int i=0;i<rodzice.size();i++)
    {
       // qDebug()<<sol->wartFunkcjiKryterialnych;
        if(stopZdominowania[i]==0)
        {
            lista.append(rodzice[i]->wartFunkcjiKryterialnych);
        }

    }

    return wskazniki.Spacing(lista);


}


void GGA::Krzyzuj(Solution * S1, Solution* S2) {
    int maks = il_bitow * problem->zmienne.size();
    int m = losuj(1, maks - 1);

    for (int i = m; i < maks; i++) {


    }
    int indeks = 0;
    bool tmp;
    for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
        for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
            if (indeks >= m) {
                tmp = S1->genotyp[i_zmienna][i_bit];
                S1->genotyp[i_zmienna][i_bit] = S2->genotyp[i_zmienna][i_bit];
                S2->genotyp[i_zmienna][i_bit] = tmp;
            }
            indeks++;

        }
    }
}

void GGA::Mutuj(Solution * S) {
    for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
        for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
            if (losuj(0.0, 1.0) < prawd_mutacji) {
                S->genotyp[i_zmienna][i_bit] = !S->genotyp[i_zmienna][i_bit];

            }
        }
    }

}

void GGA::StworzNowaPopulacje()
{
    QVector<unsigned int > indSortGol;
    GOL(indSortGol);

    for(unsigned int i=0;i<rodzice.size();i++)
    {
        qDebug()<<"GOL["<<i<<"] : "<<rodzice[indSortGol[i]]->GOL<<" ("<<rodzice[indSortGol[i]]->wartFunkcjiKryterialnych[0]<<","<<rodzice[indSortGol[i]]->wartFunkcjiKryterialnych[1]<<")";

    }
    TRACE;
    for (int i = 0; i < ilOsElitarnych; ++i) {
        *(rodzenstwo[i])=*(rodzice[indSortGol[i]]);
    }
TRACE;

    vector<vector<unsigned int> > wysRozw(nazwyRodzajnikow.size());
    for (int i = 0; i < nazwyRodzajnikow.size(); ++i) {
        WybMetSDR(i,20,wysRozw[i]);
    }

    TRACE;

    unsigned int indRodz1,indRodz2,indOsob1,indOsob2;
    for (unsigned int i = 0; i < ilOsNieElit/2; ++i) {
        indRodz1=losuj(0,nazwyRodzajnikow.size()-1);
        indRodz2=indRodz1;
        while(indRodz2==indRodz1){
            indRodz2=losuj(0,nazwyRodzajnikow.size()-1);

        }

        indOsob1=losuj(0,wysRozw[indRodz1].size()-1);
        indOsob2=losuj(0,wysRozw[indRodz2].size()-1);
        *(rodzenstwo[ilOsElitarnych+i*2])=*(rodzice[wysRozw[indRodz1][indOsob1]]);
        *(rodzenstwo[ilOsElitarnych+i*2+1])=*(rodzice[wysRozw[indRodz2][indOsob2]]);
        if (losuj(0.0, 1.0) < prawd_krzyz) {
            Krzyzuj(rodzenstwo[ilOsElitarnych+i*2], rodzenstwo[ilOsElitarnych+i*2+1]);

        }

        Mutuj(rodzenstwo[ilOsElitarnych+i*2]);
        Mutuj(rodzenstwo[ilOsElitarnych+i*2+1]);


    }
TRACE;

    for (int i = 0; i < wiel_pop; ++i) {
        swapWithMove(rodzice[i],rodzenstwo[i]);
    }







    // if (i > 0 && ((i % 2) == 0))//dla parzystego indesku bierzemy 2 ostatnie osob. i wyk. mut. krzyz.
    //  {
    //      if (losuj(0.0, 1.0) < prawd_krzyz) {
    //          Krzyzuj(rodzenstwo[i], rodzenstwo[i - 1]);

    //      }

    //       Mutuj(rodzenstwo[i]);
    //       Mutuj(rodzenstwo[i - 1]);
    //   }


}

void GGA::Iteruj()
{
     TRACE;
    StworzNowaPopulacje();
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
 TRACE;

     TRACE;

    WyznaczStopnieZdominowania();
    //  WyswietlPrzystosowanie();
    WyznaczRangi();


    //  WyswietlPrzystosowanie();


    for (int i = 0; i < rozwPrzydzRodzajnikom.size(); ++i) {
        qDebug()<<"il os "<<i<<" rodz: "<<rozwPrzydzRodzajnikom[i].size();
    }



}

void GGA::Inicjalizuj()
{
    qsrand(QTime::currentTime().msec());
    qDebug() << "P.size() " <<rodzice.size() << "\n";
    for (int i_pop = 0; i_pop <rodzice.size(); i_pop++) {
        // DINFO;
        for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
            //  DINFO;
            for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
                // DINFO;
                // qDebug()<<"i_pop "<<i_pop<<" i_zmienna "<<i_zmienna<<" i_bit"<<i_bit<<"\n";
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





}

void GGA::DekodujGenotyp()
{
    for (int i_pop = 0; i_pop <rodzice.size(); i_pop++) {
        for (unsigned int i_zmienna = 0; i_zmienna < problem->zmienne.size(); i_zmienna++) {
            //            double wartosc = 0;
            //            for (int i_bit = 0; i_bit < il_bitow; i_bit++) {
            //                if (rodzice[i_pop]->genotyp[i_zmienna][i_bit]) {
            //                    wartosc += tab_poteg[i_bit];
            //                }
            //            }
            // qDebug() << "min " << problem->zmienne[i_zmienna]->min << " max " << problem->zmienne[i_zmienna]->max << " zakres " << zakres << " wartosc " << wartosc << "\n";
            rodzice[i_pop]->x[i_zmienna] = problem->zmienne[i_zmienna]->min + (problem->zmienne[i_zmienna]->max - problem->zmienne[i_zmienna]->min) * rodzice[i_pop]->genotyp[i_zmienna].GetNormalisedValue();
        }
    }

}

void GGA::WyznaczWartFunkcjiKryterialnych()
{
    for (int i = 0; i <rodzice.size(); i++) {

        for (unsigned int x_ind = 0; x_ind < problem->zmienne.size(); x_ind++) {
            problem -> zmienne[x_ind] -> zmienna =rodzice[i] ->x[x_ind];
            qDebug()<<rodzice[i] ->x[x_ind]<<endl;
        }

        for (unsigned int f_ind = 0; f_ind < problem->parseryFunkcji.size(); f_ind++) {
            rodzice[i] -> wartFunkcjiKryterialnych[f_ind] = problem -> parseryFunkcji[f_ind] -> Eval();
        }
    }


}
QVector< QVector<Solution*> > * GGA::FastNonDominatedSort() {

    QVector< QVector<int> > Fi;
    QVector< QVector<int> > S;
    QVector<int> n(rodzice.size(), 0);

    Fi.push_back(QVector<int>());
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        S.push_back(QVector<int>());
        for (int q_pop = 0; q_pop < rodzice.size(); q_pop++) {
            if ((*rodzice[p_pop]) > (*rodzice[q_pop])) {
                S[p_pop].push_back(q_pop);
            } else if ((*rodzice[q_pop]) > (*rodzice[p_pop])) {
                n[p_pop]++;

            }

        }
        // qDebug() << "n[pop]= " << n[p_pop] << "\n";
        if (n[p_pop] == 0) {

            // pokolenie[p_pop]->ranga = 1;
            Fi[0].push_back(p_pop);

        }

    }
    DEBUG("size of Fi= %i\n", Fi.size());
    DEBUG("size of Fi[0] = %i\n",Fi[0].size());

    int i = 0;
    QVector<int> Q;
    while (i < Fi.size() && Fi[i].size() > 0) {
        Q.clear();
        for (int i_p = 0; i_p < Fi[i].size(); i_p++) {
            for (int i_q = 0; i_q < S[Fi[i][i_p]].size(); i_q++) {
                n[ S[Fi[i][i_p]][i_q]]--;
                if (n[ S[Fi[i][i_p]][i_q]] == 0) {
                    rodzice[S[Fi[i][i_p]][i_q]]->ranga = i + 2;
                    Q.push_back(S[Fi[i][i_p]][i_q]);
                }
            }
        }

        i++;
        if (Q.size() > 0) {
            //qDebug()<<"size Q = "<<Q.size()<<"\n";
            Fi.push_back(QVector<int>());
            for (int j = 0; j < Q.size(); j++) {
                Fi[i].push_back(Q[j]);
            }
        }
    }


    //qDebug()<<"Fi size "<<Fi.size()<<"\n";
    QVector< QVector<Solution*> >* F = new QVector< QVector<Solution*> >();

    for (int i = 0; i < Fi.size(); i++) {
        (*F).push_back(QVector<Solution*>());
        for (int j = 0; j < Fi[i].size(); j++) {
            (*F)[i].push_back(new Solution(*((rodzice[Fi[i][j]]))));
        }
    }
    //qDebug()<<"il frontow "<<(F)->size()<<"\n";
    return F;
}
void GGA::WyznaczPrzystosowanie()
{
    double skrajnaWartosc;
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {


        if (problem->tab_minmax[i_fun])//maksymalizacja wiec szukamy minimalnej wartosci
        {

            skrajnaWartosc =rodzice[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {
                if (rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun] < skrajnaWartosc) {
                    skrajnaWartosc =rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {
                rodzice[i_rep]->przystosowanie[i_fun] = skrajnaWartosc -rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun];
            }


        } else//minimalizacja wiec szukamy maksymalnej wartosci
        {
            skrajnaWartosc =rodzice[0]->wartFunkcjiKryterialnych[i_fun];
            for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {
                if (rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun] > skrajnaWartosc) {
                    skrajnaWartosc =rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun];
                }
            }

            for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {
                rodzice[i_rep]->przystosowanie[i_fun] =rodzice[i_rep]->wartFunkcjiKryterialnych[i_fun] - skrajnaWartosc;
            }
        }
    }


}

void GGA::WyznaczStopnieZdominowania()
{
    stopZdominowania.resize(rodzice.size());
    stopZdominowania.fill(0);
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        for (int q_pop = 0; q_pop < rodzice.size(); q_pop++) {

            if (rodzice[q_pop]->Dominate(*rodzice[p_pop])) {
                stopZdominowania[p_pop]++;
            }
        }
    }


}

void GGA::WybMetSDR(unsigned int numRodzajnika, unsigned int ilRozw, vector<unsigned int> &wysRozwiazania)
{

    double sumaFi=0;
    vector<float> eTab(rozwPrzydzRodzajnikom[numRodzajnika].size());

    for(unsigned int i=0;i<rozwPrzydzRodzajnikom[numRodzajnika].size();i++)
    {
        sumaFi+=rodzice[rozwPrzydzRodzajnikom[numRodzajnika][i]]->fi;
    }


    for(unsigned int i=0;i<rozwPrzydzRodzajnikom[numRodzajnika].size();i++)
    {
        eTab[i]=rodzice[rozwPrzydzRodzajnikom[numRodzajnika][i]]->fi/sumaFi*(float)ilRozw;
    }

    wysRozwiazania.clear();
    int tmp;
    for(unsigned int i=0;i<rozwPrzydzRodzajnikom[numRodzajnika].size();i++)
    {

        tmp=floor(eTab[i]);
        if(tmp>0)
        {
            for(unsigned int j=0;j<tmp;j++)
            {
                wysRozwiazania.push_back(rozwPrzydzRodzajnikom[numRodzajnika][i]);

            }
        }
    }

    unsigned int ilBrakRozw=ilRozw-wysRozwiazania.size();

    //przygotowanie do ruletki

    vector<float> pTab(rozwPrzydzRodzajnikom[numRodzajnika].size());


    pTab.back()=eTab.back()-(float)floor(eTab.back());

    for(int i=rozwPrzydzRodzajnikom[numRodzajnika].size()-2;i>=0;i--)
    {
        pTab[i]+=pTab[i+1]+eTab[i]-floor(eTab[i]);
    }


    for(unsigned int i=1;i<rozwPrzydzRodzajnikom[numRodzajnika].size();i++)
    {
        pTab[i]=pTab[i]/pTab[0];
    }
    pTab[0]=1;

    float p;
    for(unsigned int i=0;i<ilBrakRozw;i++)
    {
        p= losuj(0.0,1.0);

        for( int j=rozwPrzydzRodzajnikom[numRodzajnika].size()-1;j>=0;j--)
        {
            if(p<=pTab[j])
            {
                wysRozwiazania.push_back(rozwPrzydzRodzajnikom[numRodzajnika][j]);
                break;
            }
        }

    }




}
bool GGA::porownajWartGOL(int ind1, int ind2)
{

    if(rodzice[ind1]->GOL>=rodzice[ind2]->GOL)
    {
        return true;
    }

    return false;

}
bool GGA::porownajZnormRangiSubopt(int a, int b, int indRodzajnika)
{

    if(znormRangiSuboptymalnosci[a][indRodzajnika]>=znormRangiSuboptymalnosci[b][indRodzajnika])
    {
        return true;
    }

    return false;

}

void GGA::WyznaczRangi()
{
    DINFO;

    //wyznaczamy stopnie zdominowania
    stopZdominowaniaSubopt.resize(rodzice.size());
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {

        stopZdominowaniaSubopt[p_pop].resize(nazwyRodzajnikow.size());
        stopZdominowaniaSubopt[p_pop].fill(0);

        for( int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
        {
            for (int q_pop = 0; q_pop < rodzice.size(); q_pop++) {

                if (rodzice[q_pop]->Dominate(*rodzice[p_pop],funkcjeRodzajnikow[iGender])) {
                    stopZdominowaniaSubopt[p_pop][iGender]++;
                }
            }
        }
    }




    //wyznaczmy maksymalne stopnie zdominowania dla kazdego rodzajnika
    QVector<unsigned int> maksZdominowania(nazwyRodzajnikow.size(),0);

    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        for( int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
        {
            if(stopZdominowaniaSubopt[p_pop][iGender]>maksZdominowania[iGender])
            {
                maksZdominowania[iGender]=stopZdominowaniaSubopt[p_pop][iGender];
            }

        }
    }


    DINFO;
    //wyznaczamy rangi dla kazdego rodzajnika oraz maks rangi ktore posluza do znormalizowania rang
    QVector<unsigned int> maksRangi(nazwyRodzajnikow.size(),0);
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        for( int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
        {
            rangiSuboptymalnosci[p_pop][iGender]= maksZdominowania[iGender]-stopZdominowaniaSubopt[p_pop][iGender];
            if(rangiSuboptymalnosci[p_pop][iGender]>maksRangi[iGender])
            {
                maksRangi[iGender]=rangiSuboptymalnosci[p_pop][iGender];
            }
        }
    }

    DINFO;
    //normalizujemy rangi dla kazdego rodzajnika
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        for(int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
        {
            znormRangiSuboptymalnosci[p_pop][iGender]=(float)rangiSuboptymalnosci[p_pop][iGender]/(float)maksRangi[iGender];
        }
    }
    DINFO;

    //przydzielamy kazdemu rozwiazaniu maksymalna znormalizowana range suboptymalnosci
    float max_val;
    unsigned int max_val_ind;

    for(int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
    {
        rozwPrzydzRodzajnikom[iGender].clear();
    }
    DINFO;
    for (int p_pop = 0; p_pop < rodzice.size(); p_pop++) {
        max_val=0;
        max_val_ind=0;
        // DINFO;
        qDebug()<<"osobnik["<<p_pop<<"] znorm rangi : ";
        for(int iGender=0;iGender<nazwyRodzajnikow.size();iGender++)
        {
            qDebug()<<znormRangiSuboptymalnosci[p_pop][iGender]<<" ";
            if(znormRangiSuboptymalnosci[p_pop][iGender]>max_val)
            {
                max_val=znormRangiSuboptymalnosci[p_pop][iGender];
                max_val_ind=iGender;
            }
        }
        // DINFO;
        qDebug()<<endl;


        rodzice[p_pop]->fi=max_val;

        przydzieloneRodzajniki[p_pop]=max_val_ind;
        rozwPrzydzRodzajnikom[max_val_ind].push_back(p_pop);

        // DINFO;
    }

    using namespace std::placeholders;
    //sortujemy populacje subkryterialne pod wzgledem znormRangiSuboptymalnosci
    qDebug()<<"przed sortowaniem";
    for(unsigned int i=0;i<nazwyRodzajnikow.size();i++)
    {
        qSort(rozwPrzydzRodzajnikom[i].begin(),rozwPrzydzRodzajnikom[i].end(),bind(bind(&GGA::porownajZnormRangiSubopt,this,_1,_2,_3),_1,_2,i));

        qDebug()<<"po sortowaniu rodzajnika "<<i;
        for(unsigned int j=0;j<rozwPrzydzRodzajnikom[i].size();j++)
        {
            qDebug()<<"znormRO["<<i<<"]["<<j<<"] : "<<znormRangiSuboptymalnosci[rozwPrzydzRodzajnikom[i][j]][i];

        }
    }





    DINFO;




}

void GGA::PrzeskalujPrzystosowanie()
{
    double min, max, srednia, a, b;
    for (unsigned int i_fun = 0; i_fun < problem->parseryFunkcji.size(); i_fun++) {
        srednia = 0;
        min =rodzice[0]->przystosowanie[i_fun];
        max = min;
        for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {
            if (rodzice[i_rep]->przystosowanie[i_fun] < min) {
                min =rodzice[i_rep]->przystosowanie[i_fun];
            }
            if (rodzice[i_rep]->przystosowanie[i_fun] > max) {
                max =rodzice[i_rep]->przystosowanie[i_fun];
            }

            srednia +=rodzice[i_rep]->przystosowanie[i_fun];
        }
        //  qDebug() << "suma " << srednia << "\n";
        srednia = srednia / (double)rodzice.size();
        //  qDebug() << "srednia " << srednia << "\n";

        if (min > ((c * srednia - max) / (c - 1))) {
            a = srednia * (c - 1) / (max - srednia);
            b = -srednia * (a + 1.0);
        } else {
            a = srednia / (srednia - min);
            b = -a*min;
        }

        // qDebug() << "a " << a << " b " << b << "\n";
        for (int i_rep = 0; i_rep <rodzice.size(); i_rep++) {

            rodzice[i_rep]->przystosowaniePrzeskalowane[i_fun] = a *rodzice[i_rep]->przystosowanie[i_fun] + b;
        }



    }


}

void GGA::WyswietlPrzystosowanie()
{
    qDebug() << "PrzystosowaniePrzeskalowane" << "\n";
    for (int i = 0; i <rodzice.size(); i++) {
        qDebug() << i << " : ";
        for (unsigned int j = 0; j < problem->parseryFunkcji.size(); j++) {
            qDebug() <<rodzice[i]->przystosowanie[j] << " ";
        }
        qDebug() << "\n";
    }
}

void GGA::WyswietlPrzystosowaniePrzeskalowane(QVector<Solution *> &P)
{
}

void GGA::run()
{
}


void GGA::doTheWork()
{



}


void GGA::stop()
{
}

void GGA::setNum_iter(int num_iter)
{
}

int GGA::getNum_iter() const
{
}
