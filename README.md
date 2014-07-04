OpenSim Core [![Build Status][buildstatus_image]][travisci]
============ 

OpenSim is software that lets users develop models of musculoskeletal
structures and create dynamic simulations of movement.

![Simulation of human running by Sam Hamner (doi:
10.1016/j.jbiomech.2010.06.025)][running_gif]

More information can be found at our websites:

* [OpenSim website](http://opensim.stanford.edu); in particular, the [support
  page](http://opensim.stanford.edu/support/index.html)).
* [SimTK project website](https://simtk.org/home/opensim)

This repository contains the source code for OpenSim's C++ libraries, C++
examples, command-line applications (inverse kinematics, computed muscle
control, etc.), and Java and Python wrapping. This repository does *not*
include source code for the OpenSim GUI.


Simple example
--------------
Let's simulate a simple arm whose elbow is actuated by a muscle:
```cpp
#include <OpenSim/OpenSim.h>
using namespace SimTK;
using namespace OpenSim;
int main() {
    Model model; model.setUseVisualizer(true);
    // Two links, with mass of 1 kg, center of mass at the
    // origin of the body's frame, and moments/products of inertia of zero.
    OpenSim::Body* link1 = new OpenSim::Body("humerus", 1.0, Vec3(0), Inertia(0));
    OpenSim::Body* link2 = new OpenSim::Body("radius", 1.0, Vec3(0), Inertia(0));

    // Joints that connect the bodies together.
    OpenSim::PinJoint* joint1 = new OpenSim::PinJoint("shoulder",
            // Parent body, location in parent, orientation in parent.
            model.getGroundBody(), Vec3(0), Vec3(0),
            // Child body, location in child, orientation in child.
            *link1, Vec3(0, 1, 0), Vec3(0));
    joint1->getCoordinateSet()[0].setName("shoulder_angle");
    OpenSim::PinJoint* joint2 = new OpenSim::PinJoint("elbow",
            *link1, Vec3(0), Vec3(0), *link2, Vec3(0, 1, 0), Vec3(0));
    joint2->getCoordinateSet()[0].setName("elbow_angle");

    // Add an actuator that crosses the elbow, and a joint stop.
    Millard2012EquilibriumMuscle* muscle = new
        Millard2012EquilibriumMuscle("biceps", 200, 0.6, 0.55, 0);
    muscle->addNewPathPoint("point1", *link1, Vec3(0, 0.8, 0));
    muscle->addNewPathPoint("point2", *link2, Vec3(0, 0.7, 0));

    // A controller that specifies the excitation of the biceps muscle.
    PrescribedController* brain = new PrescribedController();
    brain->addActuator(*muscle);
    // Muscle excitation is 0.3 for the first 0.5 seconds, and 1.0 thereafter.
    brain->prescribeControlForActuator("biceps",
            new StepFunction(0.5, 3.0, 0.3, 1.0));

    // Add bodies and joints to the model.
    model.addBody(link1); model.addBody(link2);
    model.addJoint(joint1); model.addJoint(joint2);
    model.addForce(muscle); 
    model.addController(controller);

    // Configure the model.
    State& state = model.initSystem();
    // Fix shoulder joint, flex elbow joint.
    model.updCoordinateSet()[0].setLocked(state, true);
    model.updCoordinateSet()[1].setValue(state, 0.5 * Pi);
    model.equilibrateMuscles(state);

    // Add display geometry.
    model.updMatterSubsystem().setShowDefaultGeometry(true);
    Visualizer& viz = model.updVisualizer().updSimbodyVisualizer();
    viz.setBackgroundColor(Vec3(1, 1, 1));

    // Simulate.
    RungeKuttaMersonIntegrator integrator(model.getSystem());
    Manager manager(model, integrator);
    manager.setInitialTime(0); manager.setFinalTime(10.0);
    manager.integrate(state);
};
```

This code produces the following animation:

![Simulation of an arm actuated by a muscle][simple_example_gif]

---


Dependencies
------------
* Cross-platform build system:
  [CMake](http://www.cmake.org/cmake/resources/software.html) 2.8 or greater
* Compiler.
    * [Microsoft Visual Studio](http://www.visualstudio.com) 2013 (Windows only).
    * [gcc](http://gcc.gnu.org), typically on Linux; or
      [Clang](http://clang.llvm.org) typically on Mac.
* [Simbody](https:/github.com/simbody/simbody), our physics engine.
* API documentation (optional):
  [Doxygen](http://www.stack.nl/~dimitri/doxygen/) 1.8 or laterk


Building from the source code
-----------------------------
Building from source is difficult and we have limited resource to support 
it. Instructions can be found online at:
http://simtk-confluence.stanford.edu:8080/display/OpenSim/Building+OpenSim+from+Source


[travisci]: https://magnum.travis-ci.com/opensim-org/opensim-core
[buildstatus_image]: https://travis-ci.org/opensim-org/opensim-core.png?branch=master
[running_gif]: OpenSim/doc/images/opensim_running.gif
[simple_example_gif]: OpenSim/doc/images/opensim_double_pendulum_muscle.gif
