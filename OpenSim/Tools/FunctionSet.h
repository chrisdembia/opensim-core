#ifndef _FunctionSet_h_
#define _FunctionSet_h_
// FunctionSet.cpp
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




// INCLUDES
#include "rdTools.h"
#include "Object.h"
#include "PropertyDbl.h"
#include "Function.h"
#include "Set.h"

template class RDTOOLS_API OpenSim::Set<OpenSim::Function>;

//=============================================================================
//=============================================================================
/**
 * A class for holding a set of functions.
 *
 * @see Function
 * @author Frank C. Anderson
 */
namespace OpenSim { 

class RDTOOLS_API FunctionSet : public Set<Function>
{
//=============================================================================
// DATA
//=============================================================================
protected:
	// PROPERTIES
	/** Minimum value of the x independent variable. */
	PropertyDbl _propMinX;
	/** Maximum value of the x independent variable. */
	PropertyDbl _propMaxX;
	/** Minimum value of the y independent variable. */
	PropertyDbl _propMinY;
	/** Maximum value of the y independent variable. */
	PropertyDbl _propMaxY;
	/** Minimum value of the z independent variable. */
	PropertyDbl _propMinZ;
	/** Maximum value of the z independent variable. */
	PropertyDbl _propMaxZ;

	// REFERENCES
	double &_minX;
	double &_maxX;
	double &_minY;
	double &_maxY;
	double &_minZ;
	double &_maxZ;

//=============================================================================
// METHODS
//=============================================================================
public:
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
	FunctionSet();
	FunctionSet(const std::string &aFileName);
	virtual ~FunctionSet();
private:
	void setNull();
	void setupProperties();

	//--------------------------------------------------------------------------
	// SET AND GET
	//--------------------------------------------------------------------------
public:
	void setMinX(double aMinX);
	double getMinX();
	void setMaxX(double aMaxX);
	double getMaxX();
	void setMinY(double aMinY);
	double getMinY();
	void setMaxY(double aMaxY);
	double getMaxY();
	void setMinZ(double aMinZ);
	double getMinZ();
	void setMaxZ(double aMaxZ);
	double getMaxZ();

	//--------------------------------------------------------------------------
	// EVALUATION
	//--------------------------------------------------------------------------
	virtual void updateSetBoundingBox();
	virtual void updateBoundingBox();
	virtual void updateBoundingBox(int aIndex);
	virtual double
		evaluate(int aIndex,int aDerivOrder,
		double aX=0.0,double aY=0.0,double aZ=0.0) const;
	virtual void
		evaluate(Array<double> &rValues,int aDerivOrder,
		double aX=0.0,double aY=0.0,double aZ=0.0) const;

//=============================================================================
};	// END class FunctionSet

}; //namespace
//=============================================================================
//=============================================================================

#endif  // __FunctionSet_h__
