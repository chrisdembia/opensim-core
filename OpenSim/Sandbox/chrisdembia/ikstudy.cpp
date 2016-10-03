
class TrajectoryGenerator : public Object {
public:
    virtual StatesTrajectory generate() const {
    }
private:
    Model m_model;
};


class InverseKinematics : public Component
{
    OpenSim_DECLARE_OUTPUT("model_markers", MapVec3);
    OpenSim_DECLARE_OUTPUT("coordinates", State);
    State solve(const State& s) const {
        
    }
};

class Simulation : public TimeAdvancer {
};

class Posthoc : public TimeAdvancer {
};

class InverseDynamics : public Component {
};


class Simulation : public TrajectoryGenerator {
};

class InverseKinematics : public TrajectoryGenerator {
};

int main() {
    Model model("subject01.osim");

    Simulation sim;

    const auto& traj = sim.generate();
}


// 1. multi-outputs
// 2. drivers (time advancers versus state generators).


class X : Object {
    void run() {
        for (s : states) {
            model.realizeReport(s);
        }
    }
};
