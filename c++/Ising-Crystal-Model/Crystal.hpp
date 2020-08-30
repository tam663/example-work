//
//  Crystal.hpp
//  debug_pracice
//
//  Created by Tom McIver on 08/04/2020.
//  Copyright © 2020 Tom McIver. All rights reserved.
//

#ifndef Crystal_h
#define Crystal_h

#include <vector>
#include <random>

class Crystal{
    
 public:
 
  // Initialise crystal representation. The int gives the crstal dimension size, and it is used to
  // initialise the vecotr 'lattice' member variable. The two float refer to the interaction energy
  // and chemical potential variables of the model, and are assigned to 'J' and 'mu'.
  Crystal(int, float, float, float, float, int);
    
  ~Crystal();
    
  // Prints the squence of +1 and -1s onto the console, which represent the spin state of the
  // lattice.
  void PrintLattice();
    
  // Save the current state of the lattice spin states, i.e. the sequence of +/-1s, to a .csv file,
  // into a file specified by a given string.
  void SaveLatticeCSV(std::string);
   
  // Save the current state of the lattice spin states, i.e. the sequence of +/-1s, to a .pbm file,
  // into a file specified by a given string.
  void SaveLatticePBM(std::string);
    
  // This updates checks a randomly selected point in the lattice, and flips the spin if the metropolis
  // condition is satisfied. The tuple of three floats returned gives the change in energy, change in
  // squared energy and the change in magnetisation which occurs upon checking the position-- i.e.
  // (0.0, 0.0, 0.0) if the spin isn't flipped, or the corresponding values if it is flipped. These
  // values can the be added to running count of the total magnetisation, energy and squared energy
  // of the lattice.
  void UpdatePosition(float, float, float);

  // Reset the lattice spins to a random +/-1.0 state, and the magnetisation and energy variables
  void Reset(float);
    
  // Simmulate the lattice from a random state under some conditions specified by the given floats, that
  // refer to the temperatue and beta. This function is used before sampling begins in a given loop of the Monte
  // Carlo simmulation, to avoid recording non-equilibrium results.
  void InitialiseState(float, float);

  // Return the ratio of the number of accepted spin flips to the number of monte carlo metroplis steps carried
  // out in total.
  float ReturnAcceptanceRatio();

  void CurrentLatticeValues(float);
    
  void ObserveDependentVariables(float, float);
  void ResetDependentVariables();
    
  float AverageAbsMagnetisation();
  float AverageMagnetisation();
  float AverageSquaredMagnetisation();
  float AverageEnergy();
  float AverageSquaredEnergy();

  float AverageAbsMagnetisationPerAtom();
  float AverageMagnetisationPerAtom();
  float AverageSquaredMagnetisationPerAtom();
  float AverageEnergyPerAtom();
  float AverageSquaredEnergyPerAtom();
    
  float HeatCapacity(float);
  float ErrorMagentisation();
  float ErrorSquaredMagnetisation();
  float ErrorEnergy();
  float ErrorSquaredEnergy();
  float ErrorHeatCapacity(float);
  std::tuple<float, float, float, float, float, float, float, float, float, float> ReturnDependentVariables(float);
  float ReturnAverageMagnetisation();
  float ReturnAverageMagnetisationPerAtom();
  std::vector<float>  ReturnCumulativeMagnetisationVector();
    
    
 private:

  // Function that returns that sum of the energy of each spin position in the lattice. This uses the
  // function EnergyPosition() to get the energy of each point.
  float TotalLatticeEnergy(float);
    
  // Get the current energy of the spin at the point specified by two given ints. The floats given
  // refer to the temperature, beta and applied magnetic field, which are required for this calculation.
  // This function is used in the member function
  // TotalLatticeEnergy().
  float EnergyPosition(int, int, float);

  // Calculate the energy required to flip the spin at the position specified by the two given ints. The
  // floats refer to the temperature, beta and applied magnetic field, which are required for this
  // calculation. The energy and squared energy change that would result at the lattice point if the
  // spin were to be flipped is returned.
  float FlipEnergyPosition(int, int, float, float, float);

  // Count the number of spin up atoms in lattice.
  float CountSpinsUp();

  // Count number of spin down atoms in lattice.
  float CountSpinsDown();

  // Returns the difference between the number of spin up and spin down atoms in the lattice.
  float NetMagnetisation();
    
    
  std::vector<std::vector<float> > lattice;
  float J;
  float mu;
  int size;
  int N;
  std::random_device rd;
  std::mt19937 generator;
  std::uniform_real_distribution<> distribution;
  float accepted;
  float total;
  std::vector<float> CumulativeMagnetisation, CumulativeSquaredMagnetisation, CumulativeLatticeEnergy, CumulativeSquaredLatticeEnergy;
  int NumMeasurements;
  float Magnetisation, LatticeEnergy, SquaredLatticeEnergy;
    
};


#endif /* Crystal_h */
