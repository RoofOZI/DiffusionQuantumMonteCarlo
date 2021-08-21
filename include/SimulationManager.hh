#pragma once

#include "AllReplica.hh"
#include "Branch.hh"
#include "Parameter.hh"
#include "Walk.hh"
#include <fstream>
#include <iostream>
#include <memory>

template <class PotentialE> class SimulationManager {
  public:
    SimulationManager();
    ~SimulationManager(){};

    void Execution();
    void PrintParameter();
    void Save(const char *filename);
    auto &GetRefEnergies() { return refEs; }

  private:
    std::shared_ptr<Parameter> par;
    std::shared_ptr<AllReplica> allRep;
    std::shared_ptr<Walk> walk;
    std::shared_ptr<Branch<PotentialE>> branch;
    PotentialE potentialE;

    std::vector<double> refEs;
};

template <class PotentialE> SimulationManager<PotentialE>::SimulationManager() {}

template <class PotentialE> void SimulationManager<PotentialE>::Execution() {
    par = std::make_shared<Parameter>("../", "param.txt");
    par->ReadParameter();
    allRep = std::make_shared<AllReplica>(par.get());
    for (std::size_t iBody = 0; iBody < par->nBodies; ++iBody) {
        allRep->InitPosition(iBody, par->initPos[iBody].begin(), par->initPos[iBody].end());
    }

    walk = std::make_shared<Walk>(allRep.get(), par.get(), 0);
    branch = std::make_shared<Branch<PotentialE>>(allRep.get(), par.get(), 1);
    auto nRepeat = par->nRepeat;
    refEs.reserve(nRepeat);
    for (std::size_t iRepeat = 0; iRepeat < nRepeat; ++iRepeat) {
        walk->WalkProcess();
        branch->BranchProcess();
        refEs.emplace_back(branch->GetCurrentRefE());
    }
    // allRep->Print();
}

template <class PotentialE> void SimulationManager<PotentialE>::PrintParameter() {
    std::cout << "====== Parameter List ======\n";
    std::cout << "nInitReplica  : " << par->nInitReplica << "\n";
    std::cout << "nBodies       : " << par->nBodies << "\n";
    std::cout << "nDimensions   : " << par->nDimensions << "\n";
    std::cout << "effDimensions : " << par->effDimensions << "\n";
    std::cout << "initPos --> \n";
    for (std::size_t iBody = 0; iBody < par->nBodies; ++iBody) {
        std::cout << "  Body [" << iBody << "] : {";
        for (auto it = par->initPos[iBody].begin(), end = par->initPos[iBody].end(); it != end; ++it) {
            std::cout << *it;
            if (std::next(it) != end) {
                std::cout << ", ";
            } else {
                std::cout << "}\n";
            }
        }
    }
    std::cout << "deltaTau      : " << par->deltaTau << "\n";
    std::cout << "nRepeat       : " << par->nRepeat << std::endl;
}

template <class PotentialE> void SimulationManager<PotentialE>::Save(const char *filename) {
    std::ofstream ofile(filename, std::ios::out);
    ofile << "====== Parameter List ======\n";
    ofile << "nInitReplica  : " << par->nInitReplica << "\n";
    ofile << "nBodies       : " << par->nBodies << "\n";
    ofile << "nDimensions   : " << par->nDimensions << "\n";
    ofile << "effDimensions : " << par->effDimensions << "\n";
    ofile << "initPos --> \n";
    for (std::size_t iBody = 0; iBody < par->nBodies; ++iBody) {
        ofile << "  Body [" << iBody << "] : {";
        for (auto it = par->initPos[iBody].begin(), end = par->initPos[iBody].end(); it != end; ++it) {
            ofile << *it;
            if (std::next(it) != end) {
                ofile << ", ";
            } else {
                ofile << "}\n";
            }
        }
    }
    ofile << "deltaTau      : " << par->deltaTau << "\n";
    ofile << "nRepeat       : " << par->nRepeat << "\n";

    auto &replicas = allRep->GetRefReplicas();
    ofile << "Replica Num   : " << replicas.size() << "\n\n";

    ofile << "==== Replica Plsitions ==== \n";
    ofile << std::scientific;
    for (auto &&replica : replicas) {
        for (auto it = replica.begin(), end = replica.end(); it != end; ++it) {
            ofile << *it;
            if (std::next(it) == end) {
                ofile << "\n";
            } else {
                ofile << ", ";
            }
        }
    }
    ofile << std::endl;
    ofile.close();
}