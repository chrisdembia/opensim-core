#ifndef OPENSIM_PROPERTYARRAYDOUBLE_H_
#define OPENSIM_PROPERTYARRAYDOUBLE_H_

#include "osimCommonDLL.h"
#include "Property.h"
#include "Array.h"

// This header is for use with the new property system, in property macros like:
// OpenSim_DECLARE_PROPERTY(<name>, Array<double>, ...)
// This is in contrast to the PropertyDblArray.h, which is for the old property
// system.

namespace OpenSim {

/** TypeHelper specialization for OpenSim::Array<double>. */
template<> struct Property<Array<double> >::TypeHelper  {
    static const bool IsObjectType = false;
    static SimpleProperty<Array<double> >*
        create(const std::string& name, bool isOne)
        { return new SimpleProperty<Array<double> >(name, isOne); }
    static std::string getTypeName() {return "Array<double>";}
    static bool isEqual(const Array<double>& a, 
                                       const Array<double>& b)
    { return a==b; }
};

}

#endif
