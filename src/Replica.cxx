#include "Replica.hh"
#include <algorithm>
#include <iostream>
#include <utility>

Replica::Replica(std::size_t size) : pos(size), isArive(true) {}

Replica::Replica(const Replica &rhs) {
    pos.assign(rhs.GetPosSize(), 0);
    for (int i = 0, n = pos.size(); i < n; ++i) {
        pos[i] = rhs[i];
    }
    isArive = rhs.IsArive();
}

Replica::Replica(Replica &&rhs) {
    auto &ref = rhs.GetPosRef();
    pos = std::move(ref);
    isArive = true;
}

Replica Replica::operator=(const Replica &rhs) {
    pos.assign(rhs.GetPosSize(), 0);
    for (int i = 0, n = pos.size(); i < n; ++i) {
        pos[i] = rhs[i];
    }
    isArive = rhs.IsArive();
    return *this;
}

Replica Replica::operator=(Replica &&rhs) {
    auto &ref = rhs.GetPosRef();
    pos = std::move(ref);
    isArive = true;
    return *this;
}
