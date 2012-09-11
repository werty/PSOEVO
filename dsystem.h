#ifndef DSYSTEM_H
#define DSYSTEM_H

#define DSYSTEM_PRECISION float

/**************************************************************************/
//
// Classes: 1) Continuous Dynamical System,
//          2) Vector.
//
// Author: Leonardo A. B. Torres
// Date: 05/1997
// Universidade Federal de Minas Gerais - Brazil
//
// Linux Version: 0.0.1 - 03/1999
/**************************************************************************/

#include <math.h>
#include <iostream>

/*************************************************************************/
/*                                                                       */
/*                             Declarations                              */
/*                                                                       */
/*************************************************************************/

#define DSYSTEM_TRUE  1
#define DSYSTEM_FALSE 0

class DSystemElement {

public:
  DSYSTEM_PRECISION data;
  DSystemElement *next;

  DSystemElement(void);
  DSystemElement(DSystemElement& ego);
};

/*----------------------------------------------------------------------*/

typedef enum growth {
  AUTOMATIC,
  MANUAL
}growth_type;

class DSystemVector {

private:

   DSystemElement *header;

   int n_elements;
   growth_type status; // The vector grows automatically.

public:

   DSystemVector(void);
   DSystemVector(DSystemVector& ego);
   DSystemVector(int n);

   void SetGrowthMode(growth_type _s) { status = _s; }
   growth_type GetGrowthMode(void)    { return status; }

   void InsertElement(DSYSTEM_PRECISION value);
   int RemoveElement(int n);

   int GetSize(void) { return n_elements; }

   DSYSTEM_PRECISION& operator[](int n);
   DSystemVector operator=(DSystemVector& other);
   DSystemVector operator=(DSYSTEM_PRECISION value);

   ~DSystemVector(void);
};

/*----------------------------------------------------------------*/

class ContinuousDynamicalSystem {

protected:

  DSystemVector  x,       // State Variables.
                 xdot,    // Derivatives.
                 p,       // Parameters.
	         u,       // Inputs.
	         y;       // Outputs.

  DSYSTEM_PRECISION  t,   // Current time.
	             dt;  // Integration interval.

  int          n_pars,    // Number of parameters.
	       n_svars,   // Number of state variables.
   	       n_inputs,  // Number of inputs.
  	       n_outputs; // Number of outputs.

  // Calculate the inputs
  virtual void inputs(void) { }

  // Calculate the outputs
  virtual void outputs(void) { }

  // Differential Equation System description.
  virtual void DifferentialEquationSystem(void) = 0;

  // Class Initialization.
  void Initialize(void);

private:

  DSystemVector  A,B,C,D,AUX, // Runge-Kutta fourth order variables.
	         states;

  // Object Initialization
  char isInitialized;

public:
  void Clear();


  ContinuousDynamicalSystem(void);

  // Inline functions
  void SetTime(DSYSTEM_PRECISION _t) { t = _t; }
  DSYSTEM_PRECISION GetTime(void)    { return t; }

  void SetTimeStep(DSYSTEM_PRECISION time_step)   { dt = time_step; }
  DSYSTEM_PRECISION GetTimeStep(void)             { return dt; }

  void SetStateVar(int n,DSYSTEM_PRECISION value) { x[n] = value; }
  DSYSTEM_PRECISION GetStateVar(int n)            { return x[n]; }

  void SetParameter(int n,DSYSTEM_PRECISION value){ p[n] = value; }
  DSYSTEM_PRECISION GetParameter(int n)           { return p[n]; }

  void SetInput(int n,DSYSTEM_PRECISION value){ u[n] = value; }



  void SetStates(DSystemVector &_x)  { x = _x; }
  DSystemVector& GetStates(void)      { return x; }

  void SetParms(DSystemVector &_p);
  DSystemVector GetParms(void) { return p; }

  DSYSTEM_PRECISION GetDerivative(int n);
  DSystemVector& GetStateSpaceFlow(void);

  DSYSTEM_PRECISION Input(int n);
  DSYSTEM_PRECISION Output(int n);

  DSystemVector GetInputs(void);
  DSystemVector GetOutputs(void);

  // Other functions
  void Evolve(void);
  void Evolve(DSYSTEM_PRECISION final_time,
              DSYSTEM_PRECISION time_step = (DSYSTEM_PRECISION) 0.0);
              
  virtual ~ContinuousDynamicalSystem(void) {}               
 };

/*************************************************************************/
/*                                                                       */
/*                Macro Declarations                                     */
/*                                                                       */
/*************************************************************************/

#define DECLARE_CONTINUOUS_DYNAMICAL_SYSTEM(NAME) \
class NAME : public ContinuousDynamicalSystem {\
private:\
  void DifferentialEquationSystem(void);\
public:\
  NAME(void) { Initialize(); }\

#define END_DECLARATION };

#define DECLARE_INPUTS private: void inputs(void)

#define DECLARE_OUTPUTS private: void outputs(void)

#define DEFINE_DIFFERENTIAL_EQUATION_SYSTEM(NAME)\
void NAME::DifferentialEquationSystem(void)\
{\

#define END_DEFINITION }

#define DEFINE_INPUTS(NAME) void NAME::inputs(void) {

#define DEFINE_OUTPUTS(NAME) void NAME::outputs(void) {


#endif /* End of DSystem.h */



