#pragma once
#include "AllReplica.hh"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <valarray>

template <class PotentialE> class Branch {
  public:
    Branch(AllReplica *allRep, Parameter *par_, std::size_t seed = 1);
    ~Branch(){};

    void BranchProcess();
    double GetCurrentRefE() { return currentRefE; };

  private:
    std::mt19937 mt;
    PotentialE potentialE;
    AllReplica *allRep = nullptr;
    Parameter *par;
    std::valarray<double> potEs;
    double EvalRefE();
    void RemoveDeadReplica();
    double currentRefE;
};

template <class PotentialE>
Branch<PotentialE>::Branch(AllReplica *allRep_, Parameter *par_, std::size_t seed) : allRep(allRep_), par(par_) {
    mt = std::mt19937(seed);
}

template <class PotentialE> void Branch<PotentialE>::BranchProcess() {
    auto &replicas = allRep->GetRefReplicas();
    auto size = replicas.size();
    auto refE = EvalRefE();
    auto dT = par->deltaTau;
    currentRefE = refE;
    refE += (1 - static_cast<double>(size) / par->nInitReplica);
    std::uniform_real_distribution<> uni(0, 1);
    for (std::size_t iReplica = 0; iReplica < size; ++iReplica) {
        auto W = std::exp((refE - potEs[iReplica]) * dT);
        auto W_INT = std::min(static_cast<int>(W + uni(mt)), 3);
        if (W_INT == 0) {
            replicas[iReplica].Terminate();
        } else if (W_INT == 1) {
            continue;
        } else if (W_INT == 2) {
            replicas.emplace_back(replicas[iReplica]);
        } else if (W_INT == 3) {
            replicas.emplace_back(replicas[iReplica]);
            replicas.emplace_back(replicas[iReplica]);
        } else {
            std::terminate();
            continue;
        }
    } // for <iReplica>
    RemoveDeadReplica();
}

template <class PotentialE> double Branch<PotentialE>::EvalRefE() {
    auto &replicas = allRep->GetRefReplicas();
    auto size = replicas.size();
    potEs.resize(size, 0);
    for (std::size_t i = 0; i < size; ++i) {
        assert(replicas[i].IsArive() == true);
        potEs[i] = potentialE(replicas[i].data());
    }
    return potEs.sum() / size;
}

template <class PotentialE> void Branch<PotentialE>::RemoveDeadReplica() {
    auto &replicas = allRep->GetRefReplicas();
    auto it_rm = std::remove_if(replicas.begin(), replicas.end(), [](Replica &rep) { return rep.IsArive() == false; });
    replicas.erase(it_rm, replicas.end());
}