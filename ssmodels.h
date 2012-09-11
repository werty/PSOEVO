#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <vector>
#include "dsystem.h"
#include <QPair>
#include <QVector>
//r(s) sygnal zadany
//e(s) uchyb sterowania
//u(s) sygnal sterujacy
//y(s) sygnal wyjsciowy

//              Ki
//PID(s)= Kp + ---- + Kd*s
//              s

//         (-0.5s+1)(s+4)
// Gp(s)= -----------------
//        s(s+2)(s^2+6s+10)

// r(s)     e(s)          u(s)              y(s)
//----->(+)----->[PID(s)]----->[Gp(s)]------->
//     -|                                |
//      |                                |
//      \_________________1______________/
//
//PID(s)=
//
//transmitancja wyjsciowa
DECLARE_CONTINUOUS_DYNAMICAL_SYSTEM(SysClosedLoopYsWithPID)

  DECLARE_INPUTS;
  DECLARE_OUTPUTS;

END_DECLARATION



// r(s)                                    e(s)
//----->(+)------------------------------------->
//     -|                                |
//      | y(s)          u(s)             |
//      \------[Gc(s)]<-------[PID(s)]---/
//
//PID(s)=
//
//transmitancja uchybowa
DECLARE_CONTINUOUS_DYNAMICAL_SYSTEM(SysClosedLoopEsWithPID)

  DECLARE_INPUTS;
  DECLARE_OUTPUTS;

END_DECLARATION


// r(s)     e(s)                           u(s)
//----->(+)----->[PID(s)]----------------------->
//     -|                                |
//      |    y(s)                        |
//      \-----------[Gc(s)]<-------------/
//
//PID(s)=
//
//transmitancja sygnalu sterujacego
DECLARE_CONTINUOUS_DYNAMICAL_SYSTEM(SysClosedLoopUsWithPID)

  DECLARE_INPUTS;
  DECLARE_OUTPUTS;

END_DECLARATION

int CheckStabilityRouthHurwitz(std::vector<float> poly);
int CheckStabilityOfMySystem(float Kp,float Ki,float Kd);

int GetStepResponse(ContinuousDynamicalSystem*sys,float Kp,float Ki,float Kd,float time,float dt,QVector<QPair<float,float>>& data);
int GetImpulseResponse(ContinuousDynamicalSystem*sys,float Kp,float Ki,float Kd,float time,float dt,QVector<QPair<float,float>>& data);

void GetStepResponseCharacteristics(QVector<QPair<float,float>>& data,float &MaxOvershoot,float &RiseTime,float &SettlingTime);

#endif // EXAMPLE_H
