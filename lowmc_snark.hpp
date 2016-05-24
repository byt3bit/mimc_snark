#ifndef LOWMC_SNARK_HPP
#define LOWMC_SNARK_HPP
#include "common/common.hpp"
#include "common/utility.hpp"

#include "lowmc_header.hpp"
/*
  XOR constraints: Not a proper bilinear gate in the r1_constraint
                   xor constraints are not stored  
*/


namespace snark {

  /*
    generates random square matrix of over F_2
    size of matrix is LOWMC_BLOCK_SIZE
  */
  Lmat generate_random_matrix();

  /*
    generates random round constants
    returns a matrix of size NUM_OF_ROUND X LOWMC_BLOCK_SIZE
  */
  Lmat generate_round_const(int numofround);

  std::vector<Lmat> generate_random_matrices(int numofround);



  /*
    lowmc implementation in snark setting
  */
  template<typename field_t>
  class lowmc_snark {

  private:

    int xorCount;
    int mulCount;

    int numofSbox;
    int numofRound;
    snarkcs<field_t> snark_constr_witness;

    std::vector<Lmat> lowmcMat;
    Lmat roundConst;

    std::vector<field_t> generate_witness_from_linMat(Lmat roundMat, 
						      std::bitset<LOWMC_BLOCK_SIZE> roundconst, 
						      std::vector<field_t> x);

  public:

    lowmc_snark(int numofsbox, int numberofround, std::vector<Lmat> lowmc_mat, 
		Lmat round_const):
      numofSbox(numofsbox), numofRound(numberofround), lowmcMat(lowmc_mat), 
      roundConst(round_const),  
      xorCount(0), mulCount(0)
    {
    }

    lowmc_snark(int numofsbox, int numberofround, Lmat round_const):
      numofSbox(numofsbox), numofRound(numberofround), roundConst(round_const), 
      xorCount(0), mulCount(0)
    {
    }

    void generate_r1_constraint();
    
    void generate_witness(std::vector<field_t> x);

    int num_of_addition() { 
      return xorCount;
    }

    int num_of_mult() {
      return mulCount;
    }

    size_t num_of_constr() {
      return snark_constr_witness.constraint_size();
    }

    void print_param() {
      printf("Number of rounds         =  %3d\n", numofRound);
      printf("Number of sbox per round =  %3d\n", numofSbox);

    }

  };

#include "lowmc_snark.cpp"

}

#endif
