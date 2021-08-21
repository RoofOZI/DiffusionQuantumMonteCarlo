#pragma once

#include "Replica.hh"

class AllReplica {
  public:
    AllReplica(Parameter *par_);
    ~AllReplica() {}

    template <class InputIterator> void InitPosition(int ithBody, InputIterator begin, InputIterator end);
    void Print();
    auto &GetRefReplicas() { return replicas; };

  private:
    Parameter *par;
    std::vector<Replica> replicas;
};

template <class InputIterator> void AllReplica::InitPosition(int ithBody, InputIterator begin, InputIterator end) {
    auto nReplica = par->nInitReplica;
    for (std::size_t i = 0; i < par->nInitReplica; ++i) {
        std::copy(begin, end, replicas[ithBody * nReplica + i].begin() + ithBody * par->nDimensions);
    }
}