#include "Walk.hh"
#include <cmath>

Walk::Walk(AllReplica *allRep_, Parameter *par_, std::size_t seed) : allRep(allRep_), par(par_) {
    mt = std::mt19937(seed);
}

void Walk::WalkProcess() {
    auto &replicas = allRep->GetRefReplicas();
    auto deltaTau = par->deltaTau;
    std::normal_distribution<> norm(0, 1);
    for (auto &replica : replicas) {
        for (auto &pos : replica) {
            pos += std::sqrt(deltaTau) * norm(mt);
        }
    }
}