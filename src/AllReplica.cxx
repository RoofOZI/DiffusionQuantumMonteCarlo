#include "AllReplica.hh"
#include <iostream>

AllReplica::AllReplica(Parameter *par_) : par(par_) {
    auto nBody = par->nBodies;
    auto nReplica = par->nInitReplica;
    replicas.assign(nBody * nReplica, Replica(par->effDimensions));
    replicas.reserve(nBody * nReplica * 2);
}

void AllReplica::Print() {
    int iRep = 0;
    for (auto &&replica : replicas) {
        auto it = replica.begin();
        auto end = replica.end();
        std::cout << iRep << " : {";
        for (; it != end; ++it) {
            std::cout << *it;
            if (std::next(it) == end) {
                std::cout << "}\n";
            } else {
                std::cout << ", ";
            }
        }
        ++iRep;
    }
    std::cout << std::endl;
}