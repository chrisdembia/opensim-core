#ifndef _DecompTargetNoCompPrescribed_h_
#define _DecompTargetNoCompPrescribed_h_
// DecompTargetNoCompPrescribed.h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	AUTHOR: Frank C. Anderson
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//==============================================================================
// INCLUDES
//==============================================================================
#include <math.h>
#include <OpenSim/Tools/rdMath.h>
#include <OpenSim/SQP/rdFSQP.h>
#include <OpenSim/Simulation/Model/Model.h>
#include "DecompHardUTWalkNoCompPrescribed.h"



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/**
 * An optimization target for finding spring forces that will achieve
 * specified acceleration constraints.  This "NoComp" version is a 
 * companion to suDecmpHardUTWalkNoComp and accepts this type of analysis
 * as an input
 */
namespace OpenSim { 

class DecompTargetNoCompPrescribed : public rdOptimizationTarget
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//==============================================================================
// DATA
//==============================================================================
private:
	DecompHardUTWalkNoCompPrescribed *_analysis;

//==============================================================================
// METHODS
//==============================================================================
public:
	//---------------------------------------------------------------------------
	// CONSTRUCTION
	//---------------------------------------------------------------------------
	virtual ~DecompTargetNoCompPrescribed();
	DecompTargetNoCompPrescribed(int aNX,int aNC,DecompHardUTWalkNoCompPrescribed *aAnalysis);

	//---------------------------------------------------------------------------
	// SET AND GET
	//---------------------------------------------------------------------------

	//--------------------------------------------------------------------------
	// REQUIRED OPTIMIZATION TARGET METHODS
	//--------------------------------------------------------------------------
	// PERFORMANCE AND CONSTRAINTS
	int compute(double *x,double *p,double *c);
	int computeGradients(double *dx,double *x,double *dpdx,double *dcdx);
	// PERFORMANCE
	int computePerformance(double *x,double *p);
	int computePerformanceGradient(double *x,double *dpdx);
	// CONSTRAINTS
	int computeConstraint(double *x,int i,double *c);
	int computeConstraintGradient(double *x,int i,double *dcdx);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
};	// END class DecompTargetNoCompPrescribed

}; //namespace
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#endif // #ifndef __DecompTargetNoCompPrescribed_h__
