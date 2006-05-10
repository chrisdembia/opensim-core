#ifndef _BodyIndAcc_h_
#define _BodyIndAcc_h_
// BodyIndAcc.h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	AUTHOR: Frank C. Anderson
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//=============================================================================
// INCLUDES
//=============================================================================
#include <OpenSim/Tools/rdMath.h>
#include <OpenSim/Tools/rdTools.h>
#include <OpenSim/Simulation/Model/Model.h>
#include "suAnalysesDLL.h"
#include "IndAcc.h"


//=============================================================================
//=============================================================================
/**
 * A class for computing induced accelerations of the body segments of a
 * model.
 *
 * @author Frank C. Anderson
 * @version 1.0
 */
namespace OpenSim { 

class SUANALYSES_API BodyIndAcc : public IndAcc 
{
//=============================================================================
// DATA
//=============================================================================
private:
	Storage **_aeBodyStore;

//=============================================================================
// METHODS
//=============================================================================
public:
	BodyIndAcc(Model *aModel);
	BodyIndAcc(Model *aModel,Storage *aStates,Storage *aControls,
		char *aBaseName,char *aDir=NULL,char *aExtension=NULL);
	virtual ~BodyIndAcc();
private:
	void constructDescription();
	void constructColumnLabels();
	void allocateBodyStoragePointers();
	void allocateBodyStorage();
	void deleteBodyStorage();

public:
	//--------------------------------------------------------------------------
	// GET AND SET
	//--------------------------------------------------------------------------
	virtual void setStorageCapacityIncrements(int aIncrement);
	Storage* getBodyStore(int index);

	//--------------------------------------------------------------------------
	// OPERATIONS
	//--------------------------------------------------------------------------
	int computeBodyAccelerations();

	//--------------------------------------------------------------------------
	// UTILITY
	//--------------------------------------------------------------------------
	void sumBodyAccelerationResults();

	//--------------------------------------------------------------------------
	// IO
	//--------------------------------------------------------------------------
	virtual int
		printResults(const char *aBaseName,const char *aDir=NULL,
		double aDT=-1.0,const char *aExtension=".sto");

//=============================================================================
};	// END of class BodyIndAcc

}; //namespace
//=============================================================================
//=============================================================================


#endif // #ifndef __BodyIndAcc_h__
