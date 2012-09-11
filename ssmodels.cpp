#include <iostream>
#include <vector>
#include "ssmodels.h"
#include <cstdio>


using namespace std;


DEFINE_DIFFERENTIAL_EQUATION_SYSTEM(SysClosedLoopYsWithPID)

xdot[0]=x[1];
xdot[1]=x[2];
xdot[2]=x[3];
xdot[3]=x[4];

//xdot[4]=-4*Ki*x[0]+(Ki-4*Kp)*x[1]+(0.5*Ki-4*Kd+Kp-20.0)*x[2]+(Kd+0.5*Kp-22)*x[3]+(0.5*Kd-8)*x[4]+u[0];
//p[0]=Kp
//p[1]=Ki
//p[2]=Kd
xdot[4]=-4*p[1]*x[0]+(p[1]-4*p[0])*x[1]+(0.5*p[1]-4*p[2]+p[0]-20.0)*x[2]+(p[2]+0.5*p[0]-22)*x[3]+(0.5*p[2]-8)*x[4]+u[0];

END_DEFINITION


DEFINE_INPUTS(SysClosedLoopYsWithPID)

//cout<<"t "<<t<<endl;
u[0]=u[0];//10.0 - 0.1*t*t + 2.0*sin(10.0*t);



END_DEFINITION


DEFINE_OUTPUTS(SysClosedLoopYsWithPID)
//p[3]=2;
y[0] =4*p[1]*x[0]-(-4*p[0]+p[1])*x[1]-(p[0]+0.5*p[1]-4*p[2])*x[2]-(0.5*p[0]+p[2])*x[3]-0.5*p[2]*x[4];

END_DEFINITION

int GetStepResponse(ContinuousDynamicalSystem *sys,float Kp,float Ki,float Kd,float time,float dt,QVector<QPair<float,float>>& data)
{

    data.clear();
    cout << "Hello World!" << endl;
   // SysClosedLoopYsWithPID RC_Circuit;
    cout<<"78\n";
    double out;

    sys->SetTimeStep(dt);
    sys->SetTime(0.0);
    sys->Clear();

    sys->SetStateVar(0,0.0);
    sys->SetStateVar(1,0.0);
    sys->SetStateVar(2,0.0);
    sys->SetStateVar(3,0.0);
    sys->SetStateVar(4,0.0);

    sys->SetParameter(0,Kp);//2.3261);
    sys->SetParameter(1,Ki);//1.0/2.3897);
    sys->SetParameter(2,Kd);//0.5162);

    //// RC_Circuit.SetParameter(0,10.0e3);
    // RC_Circuit.SetParameter(1,47.0e-6);
    cout<<"x[0] x[1] "<<sys->GetStates()[0]<<" "<<sys->GetStates()[0]<<endl;
   do
    {
        out = sys->Output(0);

//        // Pen.Draw(RC_Circuit.GetTime(),out);
        data.push_back({(float)sys->GetTime(),(float)out});
       // cout<<RC_Circuit.GetTime()<<" "<<out<<endl;

    //  getchar();
//
       //cout<<"x[0] x[1] "<<RC_Circuit.GetStates()[0]<<" "<<RC_Circuit.GetStates()[0]<<endl;

       sys->Evolve();

    }while(sys->GetTime() < time );

    // if (!getch()) getch();


//    float dt=0.01;
//    float Kd=0.01;
//    float Kp=0.1;
//    float Ki=0.005;

//    float czas=3;
//    float calka=0;
//    vector<float> y(czas/dt);
//    float e_pop=0;
//    float e_akt=1;
//    y[0]=0;
//    for (int i = 1; i < czas/dt; ++i) {
//        e_akt=(1.0-y[i-1]);
//        calka+=(e_pop+e_akt)/2.0*dt;

//        y[i]=Kp*e_akt+Ki*calka+Kd*(e_akt-e_pop)/dt;
//        cout<<"calka "<<calka<<endl;
//        cout<<"y["<<i*dt<<"] "<<y[i]<<endl;
//        getchar();
//        e_pop=e_akt;

//    }



    return 0;
}
int GetImpulseResponse(ContinuousDynamicalSystem *sys,float Kp,float Ki,float Kd,float time,float dt,QVector<QPair<float,float>>& data)
{

    data.clear();
    cout << "Hello World!" << endl;
   // SysClosedLoopYsWithPID RC_Circuit;
    cout<<"78\n";
    double out;

    sys->SetTimeStep(dt);

    sys->Clear();
    sys->SetTime(0.0);
    sys->SetStateVar(0,0.0);
    sys->SetStateVar(1,0.0);
    sys->SetStateVar(2,0.0);
    sys->SetStateVar(3,0.0);
    sys->SetStateVar(4,0.0);

    sys->SetParameter(0,Kp);//2.3261);
    sys->SetParameter(1,Ki);//1.0/2.3897);
    sys->SetParameter(2,Kd);//0.5162);

    //// RC_Circuit.SetParameter(0,10.0e3);
    // RC_Circuit.SetParameter(1,47.0e-6);
    cout<<"x[0] x[1] "<<sys->GetStates()[0]<<" "<<sys->GetStates()[0]<<endl;
    bool impuls=false;
   do
    {
        if(!impuls)
        {

            sys->SetInput(0,100);
            impuls=true;
        }
        else
        {
            sys->SetInput(0,0);
        }
        sys->Evolve();
        out = sys->Output(0);

//        // Pen.Draw(RC_Circuit.GetTime(),out);
        data.push_back({(float)sys->GetTime(),(float)out});
       // cout<<RC_Circuit.GetTime()<<" "<<out<<endl;

    //  getchar();
//
       //cout<<"x[0] x[1] "<<RC_Circuit.GetStates()[0]<<" "<<RC_Circuit.GetStates()[0]<<endl;



    }while(sys->GetTime() < time );

    // if (!getch()) getch();


//    float dt=0.01;
//    float Kd=0.01;
//    float Kp=0.1;
//    float Ki=0.005;

//    float czas=3;
//    float calka=0;
//    vector<float> y(czas/dt);
//    float e_pop=0;
//    float e_akt=1;
//    y[0]=0;
//    for (int i = 1; i < czas/dt; ++i) {
//        e_akt=(1.0-y[i-1]);
//        calka+=(e_pop+e_akt)/2.0*dt;

//        y[i]=Kp*e_akt+Ki*calka+Kd*(e_akt-e_pop)/dt;
//        cout<<"calka "<<calka<<endl;
//        cout<<"y["<<i*dt<<"] "<<y[i]<<endl;
//        getchar();
//        e_pop=e_akt;

//    }



    return 0;
}


int CheckStabilityOfMySystem(float Kp,float Ki,float Kd)
{

    return CheckStabilityRouthHurwitz({(float)4.0*Ki,(float)-(Ki-4.0*Kp),(float)-(0.5*Ki-4.0*Kd+Kp-20.0),(float)-(Kd+0.5*Kp-22.0),(float)-(0.5*Kd-8.0),(float)1.0});
}

//
//dla W(s)=3+8s+s^2+2s^3=0
//poly = {2,1,8,3}
//zwraca ilosc biegunow w prawej polkuli
//0 oznacza stabilnosc
int CheckStabilityRouthHurwitz(vector<float> poly)
{
    float e=0.000001;
    vector<float> aux_poly;
    int size=poly.size();
    size=(size+1)/2+1;
    //cout<<"size "<<size<<endl;

    vector<vector<float> > tab(poly.size(),vector<float>(size,0));
    int index=0;
    for (int  i= poly.size()-1;  i>= 0; i-=2) {
        tab[tab.size()-1][index++]=poly[i];
    }

    index=0;
    for (int  i= poly.size()-2;  i>= 0; i-=2) {
        tab[tab.size()-2][index++]=poly[i];
    }

    //cout<<"tab:\n";

    bool same_zera;
    int il_elem;
    for (int wiersz = poly.size()-3; wiersz >=0; wiersz--) {
        same_zera=true;
        il_elem=0;
        for (int kolumna = 0; kolumna < size; ++kolumna) {
            if(tab[wiersz+1][kolumna+1]!=0 || tab[wiersz+2][kolumna+1]!=0)
            {
                //cout<<"tab[wiersz+2][0] "<<tab[wiersz+2][0]<<" "<<tab[wiersz+1][kolumna+1]<<" "<<tab[wiersz+1][0]<<" "<<tab[wiersz+2][kolumna+1]<<endl;
                tab[wiersz][kolumna]=(tab[wiersz+2][0]*tab[wiersz+1][kolumna+1]-tab[wiersz+1][0]*tab[wiersz+2][kolumna+1])/(-tab[wiersz+1][0]);
                if(tab[wiersz][kolumna]!=0)
                {
                   same_zera=false;
                }
               il_elem++;
            }
            else
            {
                break;
            }
        }

        if(same_zera)//podstawiamy pochodna wielomianu
        {
            //cout<<"same zera\n";
            getchar();
            aux_poly.clear();
            cout<<"aux poly :";
            for (int i = poly.size()-wiersz; i  >=0; i-=2) {
                aux_poly.push_back(i);
                cout<<aux_poly.back()<<" ";
            }
            cout<<endl;

            for (int kolumna = 0; kolumna < aux_poly.size(); ++kolumna) {
                tab[wiersz][kolumna]=aux_poly[kolumna]*tab[wiersz+1][kolumna];
            }

        }
        else
        {
            if(tab[wiersz][0]==0)//sprawdzamy 1 element
            {
                tab[wiersz][0]=e;
            }

        }

    }



    //for (int i = tab.size()-1; i >= 0; i--) {
        //cout<<"s^"<<i<<" | ";
       // for (int j = 0; j < tab[i].size(); ++j) {
            //cout<<tab[i][j]<<" ";
        //}
        //cout<<endl;
   // }
    float ost=tab[0][0];
    int il_zmian_znaku=0;
    for (int wiersz = 1; wiersz < tab.size(); ++wiersz) {
        if(ost*tab[wiersz][0]<0)
        {
            il_zmian_znaku++;
        }
        ost=tab[wiersz][0];
    }
    return il_zmian_znaku;
}



void GetStepResponseCharacteristics(QVector<QPair<float,float>>& data,float &MaxOvershoot,float &RiseTime,float &SettlingTime)
{
    //zakladamy ze skok o amplitudzie 1 wiec wiec mierzymy risetime od 0.1 do 0.9 , czas ustalania dla widelek 0.98-1.02 czyli +-2 %
    float rise_time_begin,rise_time_end;//10% 90%

    for (int i = 0; i < data.size(); ++i) {
        if(data[i].second>=0.1)
        {
            rise_time_begin=data[i].first;
            break;
        }
    }

    for (int i = 0; i < data.size(); ++i) {
        if(data[i].second>=0.9)
        {
            rise_time_end=data[i].first;
            break;
        }
    }


    RiseTime=rise_time_end-rise_time_begin;


    MaxOvershoot=0;
    for (int i = 0; i < data.size(); ++i) {
        if(data[i].second>MaxOvershoot)
        {
            MaxOvershoot=data[i].second;
        }
    }

    if(MaxOvershoot>1.0)
    {
        MaxOvershoot=(MaxOvershoot-1)*100;
    }
    else
    {
        MaxOvershoot=0;
    }


    //settling time

//    float settling_time_min,settling_time_max;
//    settling_time_min=data.back().first;
//    settling_time_max=data.back().first;

    for (int i = data.size()-1; i >0; i--) {
        if(data[i].second<0.98 || data[i].second>1.02)
        {
            SettlingTime=data[i].first;
            break;
        }
    }





}




