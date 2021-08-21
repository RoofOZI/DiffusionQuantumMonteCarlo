#include "SimulationManager.hh"
#include <iostream>
#include <memory>

struct PotentialE {
    double operator()(const double *x) { return 0.5 * (x[0] * x[0] + x[1] * x[1] + x[2] * x[2]); }
};

int main() {
    std::shared_ptr<SimulationManager<PotentialE>> simu = std::make_shared<SimulationManager<PotentialE>>();
    simu->Execution();
    simu->PrintParameter();
    simu->Save("output.txt");
    return 0;
}
