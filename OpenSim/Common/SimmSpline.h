#ifndef OPENSIM_SIMMSPLINE_H_
#define OPENSIM_SIMMSPLINE_H_
/* -------------------------------------------------------------------------- *
 *                           OpenSim:  SimmSpline.h                           *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2012 Stanford University and the Authors                *
 * Author(s): Peter Loan                                                      *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */


// INCLUDES
#include "osimCommonDLL.h"
#include <string>
#include "PropertyArrayDouble.h"
#include "Array.h"
#include "Function.h"


//=============================================================================
//=============================================================================
/**
 * A class implementing a smooth function with a cubic spline as 
 * implemented in SIMM. Use a SIMM Spline if you want to reproduce
 * the behavior of a joint function created in SIMM.
 *
 * This class inherits from Function and so can be used as input to
 * any class requiring a Fuction as input.
 *
 * @author Peter Loan
 * @version 1.0
 */
namespace OpenSim { 

class OSIMCOMMON_API SimmSpline : public Function {
OpenSim_DECLARE_CONCRETE_OBJECT(SimmSpline, Function);

//=============================================================================
// MEMBER VARIABLES
//=============================================================================
protected:
	/** @name Property declarations
	These are the serializable properties associated with a SimmSpline. **/
	/**@{**/
    OpenSim_DECLARE_PROPERTY(x, Array<double>,
            "Array of values for the independent variable (i.e., the spline "
            "knot sequence). This array must be monotonically increasing.")

    OpenSim_DECLARE_PROPERTY(y, Array<double>,
            "Array of values of the dependent variable corresponding to the "
            "x values.")
	/**@}**/

private:
	Array<double> _b;
	Array<double> _c;
	Array<double> _d;

//=============================================================================
// METHODS
//=============================================================================
public:
	//--------------------------------------------------------------------------
	// CONSTRUCTION
	//--------------------------------------------------------------------------
	SimmSpline();
	SimmSpline(int numPoints, const double* x, const double* y,
		const std::string &aName="");

    /**
     * Initialize the spline with X and Y values.
     *
     * @param aN the number of X and Y values
     * @param aXValues the X values
     * @param aYValues the Y values
     */
	virtual void init(Function* aFunction);

private:
    void constructProperties();
	void setEqual(const SimmSpline &aSpline);

	//--------------------------------------------------------------------------
	// SET AND GET
	//--------------------------------------------------------------------------
public:
    /**
     * Number of independent data points (or number of coefficients)
     * used to construct the spline.
     */
	int getSize() const;
	const Array<double>& getX() const;
	const Array<double>& getY() const;
	virtual const double* getXValues() const;
	virtual const double* getYValues() const;
	virtual int getNumberOfPoints() const { return get_x().getSize(); }
	virtual double getX(int aIndex) const;
	virtual double getY(int aIndex) const;
	virtual double getZ(int aIndex) const { return 0.0; }
	virtual void setX(int aIndex, double aValue);
	virtual void setY(int aIndex, double aValue);
	virtual bool deletePoint(int aIndex);
	virtual bool deletePoints(const Array<int>& indices);
	virtual int addPoint(double aX, double aY);

	//--------------------------------------------------------------------------
	// EVALUATION
	//--------------------------------------------------------------------------
    double calcValue(const SimTK::Vector& x) const;
    double calcDerivative(const std::vector<int>& derivComponents, const SimTK::Vector& x) const;
    int getArgumentSize() const;
    int getMaxDerivativeOrder() const;
    SimTK::Function* createSimTKFunction() const;

	virtual void updateFromXMLNode(SimTK::Xml::Element& aNode, int versionNumber=-1);

private:
	void calcCoefficients();
//=============================================================================
};	// END class SimmSpline

}; //namespace
//=============================================================================
//=============================================================================

#endif  // OPENSIM_SIMMSPLINE_H_
