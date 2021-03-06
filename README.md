# Diffusion Quantum Monte Carlo Simulation

## Brief description
A numerical calculation of quantum mechanical systems based on so-callsed diffusion quantum monte carlo (DMC) is simply implemented by C++. With this code, quantum mechanical systems such as harmonic oscillator, hydrogen atom and hellium atom are successfully calculated. From the principle of DMC, only a ground state of a system can be acquired.

## How to use
You should set parameter configuration with a text file "param.txt" and potential energy as a function of positions of particles.

An example of a parameter file "param.txt" is shown below 
```
nInitReplica 1000
nBodies 1
nDimensions 3
initPos 1: 0 0 0
deltaTau 0.1
nRepeat 1000
# You can write comments with a prefix "#".
```
The left words mean names of the paramters and the right words correspond to their values. You can write comments with prefix #. The meaning of each parameter is as follows.

- `nInitReplica` : The initlal number of replicas, which means virtual particles randomly working in the system.
- `nBodies` : The number of bodies.
- `nDimensions` : The dimension of the system.
- `initPos` : The initial positions of "replicas". You should specify i-th body (physical particle) as `i:` and the positions as a series of values. For example, `1: 0 10 2: 1 -1` means that 1st body is located at `x=0, y=10` and 2nd body is located at `x=1, y=-1`. 
- `deltaTau` : The interval of imaginary time.
- `nRepeat` : This corresponds to how many times you repeat random walking and birth-dead processes.

As for the expression of potential energy, you should give it as a structure which has an `double operator()(const double*)`. For example, clone potential in 3-dimensional space can be expressed as
```c++
struct potentialE {
    double operator()(const double*) {
        return 1. / std::sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    }
};
```

, where `x[0], x[1]` and `x[2]` correspond to positions at x, y and z coordinates.

To make a simulation with these configurations,  use class `SimulationManager` like
```c++
struct PotentialE {
    double operator()(const double*){
        /* implementation */
    }
};

int main(){
    std::shared_ptr<SimulationManager<PotentialE>> simu = std::make_shared<SimulationManager<PotentialE>>;
    simu->Execution();
    return 0;
}
```
In the class `SimulationManager`, some useful methods are prepared. 
- `void PrintParameter()` : Just print out information of input parameters.
- `void Save(const char* filename)` : Output a simulation results as a text file with a name of `filename`. The input parameters and the positions of each "replica" are written.
- `std::vector<double>& GetRefEnergies()` : Return a reference of Reference Energies at the i-th repeat. The reference energies after reaching steady state are the numerical results of ground state energy by this DMC. Usually, the energy of the ground state is evaluated by taking average of the reference energies after reaching a steady state.

## BUILD
```shell
DiffusionMonteCarlo$ mkdir build
DiffusionMonteCarlo$ cd build
build$ cmake -DCMAKE_BUILD_TYPE=Release ..
build$ make
```
And then, `main.exe` is generated.

## Some Result

Also see the URL : 

In the following examples, we chose the input parameters as
- `nInitReplica` = 2000
- `deltaTau` = 0.05
- `nRepeat` = 2000.

As for `nBodies`, `nDimensions` and `initPos`, these parameters are different for each system.

### 1 dimensional harmonic oscillator.

The potential term of 1 dimensional harmonic oscillator is 1/2x^2 (dimensionless). So, the potential can be expressed as

```c++
struct potentialE {
    double operator()(const double*) {
        return 0.5 * x[0] * x[0];
    }
};
```
We took average of the reference energy over `deltaTau` >= 50 region and obtained a numerical value of 0.50085(37), which is good agreement with the theoretical value of 0.5. 

The figure below shows that the position distribution of replicas, which corresponds to the wavefunction of the system. The result of chi-squared fitting with a gaussian function is also shown. It is found that the obtained fit function is almost the same as the theoretical wavefunction,

![f1](https://latex.codecogs.com/gif.latex?f(x)=\pi^{-\frac{1}{4}}\exp{\left(-\frac{x}{2}\right)}).

![aa](https://github.com/RoofOZI/Image/blob/main/harmonic1d_xdist.png)

