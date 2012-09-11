#include "dsystem.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
/******************************************************************/
/*								  */
/*               	Implementation                            */
/*						                  */
/******************************************************************/

/*----------------------------------------------------------------------*/

using namespace std;
DSystemElement::DSystemElement(void)
{
  data = (DSYSTEM_PRECISION) 0;
  next = NULL;
}

DSystemElement::DSystemElement(DSystemElement& ego)
{
  data = ego.data;
  next = ego.next;
 }

/*----------------------------------------------------------------------*/

DSystemVector::DSystemVector(void)
{
  header = NULL;
  n_elements = 0;
  status = MANUAL;
}

DSystemVector::DSystemVector(int n)
{
  int i;
  header = NULL;
  n_elements = 0;
  for(i = 0; i<n;++i)
    InsertElement(0);
  status = MANUAL;
}


DSystemVector::DSystemVector(DSystemVector& ego)
{
  int i;
  header = NULL;
  n_elements = 0;

  if (ego.n_elements != 0) {
    status = AUTOMATIC;
    for(i=0;i<ego.n_elements;++i)
      InsertElement(ego[i]);
   }
   else {
     header = NULL;
     n_elements = 0;
   }
   
  status = ego.status;
 }


void DSystemVector::InsertElement(DSYSTEM_PRECISION value)
{
  DSystemElement *ptr;
 
  if (n_elements == 0) {
    if ((header = new DSystemElement) == NULL) {
      cerr << "DSystemVector::InsertElement()" << endl;
      cerr << "Memory Allocation Fault" << endl;
      exit(1);
    }

    header->data = value;
    header->next = NULL;
  }
  else {
    for(ptr = header; ptr->next != NULL;ptr = ptr->next);

    if ((ptr->next = new DSystemElement) == NULL) {
      cerr << "DSystemVector::InsertElement()" << endl;
      cerr << "Memory Allocation Fault." << endl;
      exit(1);
    }

    ptr = ptr->next;
    ptr->data = value;
    ptr->next = NULL;
  }
  n_elements++;
}

int DSystemVector::RemoveElement(int n)
{
  int cont;
  DSystemElement *actual,*previous;

  if ((n >= n_elements)||(n < 0))
    return 0;

  previous = NULL;
  actual = header;
  for(cont = 0;cont <= n;++cont) {
    previous = actual;
    actual = actual->next;
  }

  if (previous == NULL) {
    header = header->next;
    delete actual;
  }
  else {
    previous->next = actual->next;
    delete actual;
  }
  n_elements--;

  return 1;
 }

DSYSTEM_PRECISION& DSystemVector::operator[](int n)
{
  DSystemElement *ptr;
  int i;

  if ((n < n_elements)&&(n>=0)) {
    for(ptr = header,i = 0;i<n;ptr = ptr->next,++i);
    return ptr->data;
  }

  if ((status == AUTOMATIC)&&(n>=0)) {
    n -= n_elements-1;
    for(i = 0;i < n;++i)
      InsertElement((DSYSTEM_PRECISION) 1);

    for(ptr = header; ptr->next != NULL;ptr = ptr->next);
    return ptr->data;
  }
  else {
    cerr << "DSystemVector::operator[]" << endl;
    cerr << "Index exceeds vector dimension." << endl;
    exit(1);
  }

  return ptr->data; // Dummy return.
 }

DSystemVector DSystemVector::operator=(DSystemVector& other)
{
  int i;

  for(i = 0;i<other.GetSize();++i)
    (*this)[i] = other[i];

  return *this;
}

DSystemVector DSystemVector::operator=(DSYSTEM_PRECISION value)
{
  int i;

  for(i=0;i<n_elements;++i)
    (*this)[i] = value;

  return *this;
}

DSystemVector::~DSystemVector(void)
{
  DSystemElement *actual,*previous = NULL;

  actual = header;
  while (actual != NULL) {
    for(;actual->next != NULL;previous = actual,actual = actual->next);

    if (actual != header) {
      delete actual;
      previous->next = NULL;
    }
    else {
      delete header;
      header = NULL;
    }

    actual = header;
  }
}

/*----------------------------------------------------------------------*/

ContinuousDynamicalSystem::ContinuousDynamicalSystem(void)
{
   x.SetGrowthMode(AUTOMATIC);
   xdot.SetGrowthMode(AUTOMATIC);
   p.SetGrowthMode(AUTOMATIC);
   u.SetGrowthMode(AUTOMATIC);
   y.SetGrowthMode(AUTOMATIC);
   A.SetGrowthMode(AUTOMATIC);
   B.SetGrowthMode(AUTOMATIC);
   C.SetGrowthMode(AUTOMATIC);
   D.SetGrowthMode(AUTOMATIC);
   AUX.SetGrowthMode(AUTOMATIC);
   states.SetGrowthMode(AUTOMATIC);

   n_svars = n_pars = 0;
   n_inputs = n_outputs = 0;
   t  = (DSYSTEM_PRECISION) 0.0;
   dt = (DSYSTEM_PRECISION) 0.001;

   isInitialized = DSYSTEM_FALSE;
 }

void ContinuousDynamicalSystem::Clear()
{
    A[n_svars-1] = (DSYSTEM_PRECISION) 0;
    B[n_svars-1] = (DSYSTEM_PRECISION) 0;
    C[n_svars-1] = (DSYSTEM_PRECISION) 0;
    D[n_svars-1] = (DSYSTEM_PRECISION) 0;
    AUX[n_svars-1] = (DSYSTEM_PRECISION) 0;
    states[n_svars-1] = (DSYSTEM_PRECISION) 0;

    for (int i = 0; i < x.GetSize(); ++i) {
        x[i]=0;
    }

//    for (int i = 0; i < p.GetSize(); ++i) {
//        p[i]=0;
//    }
    for (int i = 0; i < xdot.GetSize(); ++i) {
        xdot[i]=0;
    }
//    for (int i = 0; i < u.GetSize(); ++i) {
//        u[i]=0;
//    }
    for (int i = 0; i < y.GetSize(); ++i) {
        y[i]=0;
    }
    for (int i = 0; i < A.GetSize(); ++i) {
        A[i]=0;
    }
    for (int i = 0; i < B.GetSize(); ++i) {
        B[i]=0;
    }
    for (int i = 0; i < C.GetSize(); ++i) {
        C[i]=0;
    }
    for (int i = 0; i < D.GetSize(); ++i) {
        D[i]=0;
    }
    for (int i = 0; i < AUX.GetSize(); ++i) {
        AUX[i]=0;
    }
    for (int i = 0; i < states.GetSize(); ++i) {
        states[i]=0;
    }

}

void ContinuousDynamicalSystem::Initialize(void)
{
    //cout<<"Initialize()\n";
    //cout<<"666 x[0] "<<x[0]<<endl;
    //cout<<"x.GetSize() "<<x.GetSize()<<endl;
    for (int i = 0; i < 5; ++i) {
        x[i]=0;
    }
  DifferentialEquationSystem();
  //cout<<"666 x[0] "<<x[0]<<endl;
  for (int i = 0; i < x.GetSize(); ++i) {
      x[i]=0;
  }
  x.SetGrowthMode(MANUAL);
  xdot.SetGrowthMode(MANUAL);
  p.SetGrowthMode(MANUAL);

  n_svars   = x.GetSize();
  n_pars    = p.GetSize();

  if (n_svars != xdot.GetSize()) {
    cerr<<"n_vars "<<n_svars<<" xdot "<<xdot.GetSize();
    cerr << "ContinuousDynamicalSystem::Initialize()\n" << endl;
    cerr << "The number of Derivatives must be equal to the number" << endl;
    cerr << "of State Variables." << endl;
    exit(1);
  }


  A[n_svars-1] = (DSYSTEM_PRECISION) 0;
  B[n_svars-1] = (DSYSTEM_PRECISION) 0;
  C[n_svars-1] = (DSYSTEM_PRECISION) 0;
  D[n_svars-1] = (DSYSTEM_PRECISION) 0;
  AUX[n_svars-1] = (DSYSTEM_PRECISION) 0;
  states[n_svars-1] = (DSYSTEM_PRECISION) 0;

  A.SetGrowthMode(MANUAL);
  B.SetGrowthMode(MANUAL);
  C.SetGrowthMode(MANUAL);
  D.SetGrowthMode(MANUAL);
  AUX.SetGrowthMode(MANUAL);
  states.SetGrowthMode(MANUAL);

  inputs();
  outputs();

  u.SetGrowthMode(MANUAL);
  y.SetGrowthMode(MANUAL);

  n_outputs = y.GetSize();
  n_inputs  = u.GetSize();


  for (int i = 0; i < x.GetSize(); ++i) {
      x[i]=0;
  }
  isInitialized = DSYSTEM_TRUE;
}

void ContinuousDynamicalSystem::Evolve(DSYSTEM_PRECISION final_time,
                                       DSYSTEM_PRECISION time_step)
{
  DSYSTEM_PRECISION h;
//cout<<"666 x[0] "<<x[0]<<endl;
  if (time_step == (DSYSTEM_PRECISION) 0.0)
    h = dt;
  else
    h = time_step;

  if ((isInitialized)&&(h != (DSYSTEM_PRECISION) 0.0)) {
    register int n;

    while (t < final_time) {

      /* Calculo dos parametros A */
      for(n=0;n<n_svars;n++) {
	states[n] = x[n];
	A[n] = xdot[n];
      }

      for(n=0;n<n_svars;n++)
	AUX[n] = x[n] + h/2.0*A[n];

      /* Calculo dos parametros B */
      for(n=0;n<n_svars;n++)
	x[n] = AUX[n];

     // cout<<"666 x[0] "<<x[0]<<endl;
     // cout<<"666 x[1] "<<x[1]<<endl;
      if (n_inputs) inputs();
      DifferentialEquationSystem();

      for(n=0;n<n_svars;n++) {
	B[n] = xdot[n];
	AUX[n] = states[n] + h/2.0*B[n];
      }

      /* Calculo dos parametros C */
      for(n=0;n<n_svars;n++)
	x[n] = AUX[n];

      if (n_inputs) inputs();
      DifferentialEquationSystem();

      for(n=0;n<n_svars;n++) {
	C[n] = xdot[n];
	AUX[n] = states[n] + h*C[n];
      }

      /* Calculo dos parametros D */
      for(n=0;n<n_svars;n++)
	x[n] = AUX[n];

      if (n_inputs) inputs();
      DifferentialEquationSystem();
      
      for(n=0;n<n_svars;n++)
	D[n] = xdot[n];

      /* Calculo do resultado */
      for(n=0;n<n_svars;n++)
	x[n] = states[n] + h/6.0 * ( A[n] + 2.0*B[n] + 2.0*C[n] + D[n] );

      t += h;

      if (n_inputs) inputs();
      DifferentialEquationSystem();
      
    }
    
    if (n_inputs) inputs();
    if (n_outputs) outputs();
  }
}

void ContinuousDynamicalSystem::Evolve(void)
{
  if (isInitialized)
    Evolve(t+dt);
}

DSYSTEM_PRECISION ContinuousDynamicalSystem::Input(int n)
{
  return u[n];
}

DSYSTEM_PRECISION ContinuousDynamicalSystem::Output(int n)
{
  return y[n];
}

void ContinuousDynamicalSystem::SetParms(DSystemVector &_p)
{
  p.SetGrowthMode(AUTOMATIC);
  p = _p;
  p.SetGrowthMode(MANUAL);
}

DSystemVector ContinuousDynamicalSystem::GetInputs(void)
{
  if (n_inputs == 0) {
    cerr << "Systems Inputs not declared and/or initialized." << endl;
    exit(1);
   }

  return u;
}

DSystemVector ContinuousDynamicalSystem::GetOutputs(void)
{
  if (n_outputs == 0) {
    cerr << "Systems Outputs not declared and/or initialized." << endl;
    exit(1);
  }

  return y;
 }

DSYSTEM_PRECISION ContinuousDynamicalSystem::GetDerivative(int n)
{
  return xdot[n];
}

DSystemVector& ContinuousDynamicalSystem::GetStateSpaceFlow(void)
{
  return xdot;
}
