#pragma once

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class Parameter {
    using funcPtr = void (Parameter::*)(std::istringstream &);
    using vec2D = std::vector<std::vector<double>>;
    enum { __nInitReplica, __nDimensions, __nBodies, __effDimensions, __initPos, __deltaTau, __nRepeat, __size };

  public:
    Parameter(const char *dir, const char *filename);
    ~Parameter(){};

    void ReadParameter();

    std::size_t nInitReplica;
    std::size_t nDimensions;
    std::size_t nBodies;
    std::size_t effDimensions;
    vec2D initPos;
    double deltaTau;
    std::size_t nRepeat;

  private:
    std::string path;
    std::unordered_map<std::string, funcPtr> funcs;
    std::vector<bool> isSetParams;

    void _nInitReplica(std::istringstream &iss);
    void _nDimensions(std::istringstream &iss);
    void _nBodies(std::istringstream &iss);
    void _initPos(std::istringstream &iss);
    void _deltaTau(std::istringstream &iss);
    void _nRepeat(std::istringstream &iss);
};