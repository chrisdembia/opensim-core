#ifndef OPENSIM_DELAY_H_
#define OPENSIM_DELAY_H_
/* -------------------------------------------------------------------------- *
 *                             OpenSim: Delay.h                               *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2015 Stanford University and the Authors                *
 * Author(s): Chris Dembia                                                    *
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

#include "osimCommonDLL.h"
#include "Component.h"

namespace OpenSim {

/** This operator delays its input by a given time delay. The value of the
 * output at time t is the value the input had at time t-delay.
 * A Delay can be used to model the delay in neural reflex circuits.
 *
 * For times prior to the start of a simulation this Component behaves as
 * though the input value had been constant at its initial value.
 *
 * @tparam T The type of the quantity to be delayed. Common choices are a
 * SimTK::Real (see ScalarDelay) or a SimTK::Vector (see VectorDelay).
 *
 * @see ScalarDelay, VectorDelay
 *
 *
 * This Component has a single input named "input" and a single output named
 * "output". The typical way to use this Component is as a subcomponent in
 * another component. Here is a basic example in which a controller specifies
 * the control signal for a single actuator using the delayed value of a
 * coordinate:
 * @code
 * using namespace OpenSim;
 * using namespace SimTK;
 * class MyController : Controller {
 * OpenSim_DECLARE_CONCRETE_OBJECT(MyController, Controller);
 * public:
 *     OpenSim_DECLARE_PROPERTY(delay, double, "Duration of delay (seconds).");
 *     MyController() { constructProperties(); }
 *     void computeControls(const State& s, Vector &controls) const override {
 *         double q = _coordDelay.getValue(s);
 *         // Delayed negative feedback control law.
 *         double u = -10 * q;
 *         getModel().getActuatorSet()[0].addInControls(Vector(1, u), controls);
 *     }
 * private:
 *     void constructProperties() {
 *          constructProperty_delay(0.01); // 10 milliseconds
 *     }
 *     void extendFinalizeFromProperties() override {
 *         _coordDelay.set_delay(get_delay());
 *         addComponent(&_coordDelay);
 *     }
 *     void extendConnectToModel(Model& model) override {
 *         Super::extendConnectToModel(model);
 *         const auto& coord = model.getCoordinateSet()[0];
 *         // Set the input for the Delay component.
 *         _coordDelay.getInput("input").connect(coord.getOutput("value"));
 *     }
 *
 *     ScalarDelay _coordDelay;
 * };
 * @endcode
 *
 * This class is implemented via a SimTK::Measure_<T>::Delay.
 *
 */
template<typename T>
class OSIMCOMMON_API Delay : public Component {
OpenSim_DECLARE_CONCRETE_OBJECT_T(Delay, T, Component);
public:

    /** @name Property declarations
    These are the serializable properties associated with a Delay. **/
    /**@{**/
    OpenSim_DECLARE_PROPERTY(delay, double,
        "The duration (nonnegative, seconds) by which the output is delayed.");
    /**@}**/

    Delay();

    // Convenience constructor that sets the delay property.
    explicit Delay(double delay);

    /// Get the delayed value (the input's value at time t-delay).
    T getValue(const SimTK::State& s) const;

private:

    void constructProperties() override;
    void constructInputs() override;
    void constructOutputs() override;

    void extendFinalizeFromProperties() override;
    void extendAddToSystem(SimTK::MultibodySystem& system) const override;

    SimTK::MeasureIndex _delayMeasureIndex;
};

template<class T>
Delay<T>::Delay() {
    constructInfrastructure();
}

template<class T>
Delay<T>::Delay(double delay) : Delay() {
    set_delay(delay);
}

template<class T>
void Delay<T>::constructProperties() {
    constructProperty_delay(0.0);
}

template<class T>
void Delay<T>::constructInputs() {
    // TODO the requiredAt Stage should be the same as this Delay's output dependsOn stage.
    constructInput<T>("input", SimTK::Stage::Acceleration);
}

template<class T>
void Delay<T>::constructOutputs() {
    // TODO the depensdOn stage should be the dependsOn stage of the output
    // that is wired to this Delay's input.
    constructOutput<T>("output", &Delay::getValue, SimTK::Stage::Acceleration);
}

template<class T>
T Delay<T>::getValue(const SimTK::State& s) const {
    const auto& subsys = getSystem().getDefaultSubsystem();
    const auto& measure = subsys.getMeasure(_delayMeasureIndex);
    return SimTK::Measure_<T>::Delay::getAs(measure).getValue(s);
}

template<class T>
void Delay<T>::extendFinalizeFromProperties() {
    Super::extendFinalizeFromProperties();
    SimTK_VALUECHECK_NONNEG_ALWAYS(get_delay(),
            "delay", "Delay::extendFinalizeFromProperties()");
}

template<class T>
void Delay<T>::extendAddToSystem(SimTK::MultibodySystem& system) const {
    Super::extendAddToSystem(system);
    auto& sub = system.updDefaultSubsystem();
    const auto& input = *static_cast<const Input<T>*>(&getInput("input"));
    typename SimTK::Measure_<T>::Delay delayMeasure(sub,
                                                    InputMeasure<T>(sub, input),
                                                    get_delay());
    const_cast<Delay*>(this)->_delayMeasureIndex =
        delayMeasure.getSubsystemMeasureIndex();
}

typedef Delay<SimTK::Real> ScalarDelay;
typedef Delay<SimTK::Vector> VectorDelay;

} // namespace OpenSim

#endif // OPENSIM_DELAY_H_
