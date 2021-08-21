#pragma once

#include "Parameter.hh"
#include <vector>

class Replica {
    using Iterator = typename std::vector<double>::iterator;

  public:
    Replica(){};
    Replica(std::size_t dim);
    Replica(const Replica &rhs);
    Replica(Replica &&rhs);
    ~Replica() {}
    auto &GetPosRef() { return pos; }

    bool IsArive() const { return isArive; }
    void Terminate() { isArive = false; }
    std::size_t GetPosSize() const { return pos.size(); }

    double operator[](std::size_t i) const { return pos[i]; }
    double &operator[](std::size_t i) { return pos[i]; }
    Replica operator=(const Replica &rhs);
    Replica operator=(Replica &&rhs);
    double *data() { return pos.data(); }

    Iterator begin() { return pos.begin(); }
    Iterator end() { return pos.end(); }

  private:
    std::vector<double> pos;
    bool isArive = true;
};