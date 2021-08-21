#pragma once
#include "AllReplica.hh"
#include <random>

class Walk {
  public:
    Walk(AllReplica *allRep, Parameter *par_, std::size_t seed = 0);
    ~Walk() {}

    void WalkProcess();

  private:
    std::mt19937 mt;
    AllReplica *allRep;
    Parameter *par;
};