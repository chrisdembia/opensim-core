#ifndef _Analysis_h_
#define _Analysis_h_
// Analysis.h
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
* Copyright (c) 2005, Stanford University. All rights reserved. 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions
* are met: 
*  - Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer. 
*  - Redistributions in binary form must reproduce the above copyright 
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the distribution. 
*  - Neither the name of the Stanford University nor the names of its 
*    contributors may be used to endorse or promote products derived 
*    from this software without specific prior written permission. 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE. 
*/

/* Note: This code was originally developed by Realistic Dynamics Inc. 
 * Author: Frank C. Anderson 
 */

//============================================================================

#include "IntegCallback.h"
#include <OpenSim/Tools/PropertyBool.h>
#include <OpenSim/Tools/PropertyStr.h>
#include <OpenSim/Tools/PropertyInt.h>
#include <OpenSim/Tools/ArrayPtrs.h>

namespace OpenSim { 

class XMLDocument;


//=============================================================================
//=============================================================================
/**
 * An abstract class for specifying the interface for an analysis
 * plugin.
 *
 * @author Frank C. Anderson
 * @version 1.0
 */
class RDSIMULATION_API Analysis: public IntegCallback
{
public:
	//enum { DESCRIP_LENGTH=8192 };

//=============================================================================
// DATA
//=============================================================================
private:
	/** Whether or not to write output of angles in degrees. */
	PropertyBool _inDegreesProp;
	bool &_inDegrees;

	// WORK ARRAYS
	/** Column labels. */
	std::string _labels;

protected:
	ArrayPtrs<Storage> _storageList;
//=============================================================================
// METHODS
//=============================================================================
private:
	void setNull();
	void setupProperties();
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
public:
	Analysis(Model *aModel=0);
	Analysis(const std::string &aFileName);
	Analysis(DOMElement *aElement);
	virtual ~Analysis();
	Analysis(const Analysis &aObject);
	virtual Object* copy() const;
	virtual Object* copy(DOMElement *aElement) const;

	//--------------------------------------------------------------------------
	// OPERATORS
	//--------------------------------------------------------------------------
#ifndef SWIG
	Analysis& operator=(const Analysis &aAnalysis);
#endif

	//--------------------------------------------------------------------------
	// GET AND SET
	//--------------------------------------------------------------------------
	// MODEL
	virtual void setModel(Model *aModel);
	// DEGREES/RADIANS
	void setInDegrees(bool aTrueFalse);
	bool getInDegrees() const;
	// COLUMN LABLES
	void setColumnLabels(const char *aLabels);
	const char* getColumnLabels() const;
	// STORAGE INTERVAL
	void setStorageInterval(int aInterval);
	int getStorageInterval() const;

	virtual ArrayPtrs<Storage>& getStorageList();
	//--------------------------------------------------------------------------
	// ANALYSIS
	//--------------------------------------------------------------------------
	virtual bool
		proceed(int aStep=0);

	//--------------------------------------------------------------------------
	// RESULTS
	//--------------------------------------------------------------------------
	virtual int
		printResults(const char *aBaseName,const char *aDir=NULL,
		double aDT=-1.0,const char *aExtension=".sto");

//=============================================================================
};	// END of class Analysis

}; //namespace
//=============================================================================
//=============================================================================

#endif // __Analysis_h__


