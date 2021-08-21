#include "Parameter.hh"
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Parameter::Parameter(const char *dir, const char *filename) {
    std::string directory = dir;
    std::string fName = filename;
    path = directory + fName;

    isSetParams.assign(__size, false);
    funcs["nInitReplica"] = &Parameter::_nInitReplica;
    funcs["nDimensions"] = &Parameter::_nDimensions;
    funcs["nBodies"] = &Parameter::_nBodies;
    funcs["initPos"] = &Parameter::_initPos;
    funcs["deltaTau"] = &Parameter::_deltaTau;
    funcs["nRepeat"] = &Parameter::_nRepeat;
}

void Parameter::ReadParameter() {
    std::ifstream ifile(path.data());
    if (!ifile) {
        std::cout << "-- Error : file " << path << " does not exist." << std::endl;
        std::cout << "-- Error No. : " << errno << std::endl;
        assert(ifile);
    }

    std::string buffer;
    while (std::getline(ifile, buffer)) {
        std::istringstream iss(buffer);

        std::string paramName;
        iss >> paramName;
        if (paramName == "" || paramName[0] == '#') {
            continue;
        }
        auto it = funcs.find(paramName);
        auto it_end = funcs.end();
        if (it == it_end) {
            std::cout << "No parameter name : " << it->second << std::endl;
            std::abort();
        }

        (this->*it->second)(iss);

    } // while(getline)
}

void Parameter::_nInitReplica(std::istringstream &iss) {
    iss >> nInitReplica;
    isSetParams[__nInitReplica] = true;
}

void Parameter::_nDimensions(std::istringstream &iss) {
    iss >> nDimensions;
    isSetParams[__nDimensions] = true;
    if (isSetParams[__nBodies]) {
        effDimensions = nDimensions * nBodies;
        isSetParams[__effDimensions] = true;
    }
}

void Parameter::_nBodies(std::istringstream &iss) {
    iss >> nBodies;
    isSetParams[__nBodies] = true;
    if (isSetParams[__nDimensions]) {
        effDimensions = nDimensions * nBodies;
        isSetParams[__effDimensions] = true;
    }
}

void Parameter::_initPos(std::istringstream &iss) {
    if (!isSetParams[__effDimensions]) {
        std::cout << "-- Error : you should set nBodies and nDimensions before initPos" << std::endl;
        assert(isSetParams[__effDimensions]);
    }
    initPos.assign(nBodies, std::vector<double>(nDimensions));
    while (!iss.eof()) {
        std::string temp;
        iss >> temp;
        if (temp[1] != ':') {
            std::cout << "-- Error : you should specify i-th body by i:" << std::endl;
            std::abort();
        }
        temp.erase(1, 1);
        auto iBody = std::atoi(temp.data());
        try {
            for (std::size_t i = 0; i < nDimensions; ++i) {
                auto &r = initPos.at(iBody - 1);
                iss >> r.at(i);
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    }
    isSetParams[__initPos] = true;
}

void Parameter::_deltaTau(std::istringstream &iss) {
    iss >> deltaTau;
    isSetParams[__deltaTau] = true;
}

void Parameter::_nRepeat(std::istringstream &iss) {
    iss >> nRepeat;
    isSetParams[__nRepeat] = true;
}