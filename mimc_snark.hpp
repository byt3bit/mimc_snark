#ifndef MIMC_SNARK_HPP
#define MIMC_SNARK_HPP
#include "common/common.hpp"
#include "r1_constraint.hpp"
#include "variable.hpp"

namespace snark{

  void generate_mimc_roundconst(std::vector<NTL::GF2E> &roundconst, int blocksize, int numround);

  template<typename field_t>
  class mimc_perm_snark {

  private:

    unsigned int xorCount;
    unsigned int multCount;

    int num_round;
    unsigned long int blockSize;
    std::vector<field_t> roundConst;
    //field_t key;          /*for hashing key = 0*/
    snarkcs<field_t> mimc_constr_wit;

  public:
   
    mimc_perm_snark();
    mimc_perm_snark(unsigned long int block_size, std::vector<field_t> round_const):
      blockSize(block_size), roundConst(round_const), 
      xorCount(0), multCount(0)
    {
      num_round = (int) (block_size/log2(3.0));
    }
    void set_gadget(unsigned long int block_size, std::vector<field_t> round_const)
    {
      blockSize = block_size;
      roundConst = round_const;
      num_round = (int) (block_size/log2(3.0));

      xorCount = 0;
      multCount = 0;
    }
    void generate_r1_constraint();
    void generate_witness(field_t input);
    
    unsigned int num_of_addition() {
      return xorCount;
    }
    unsigned int num_of_mult() {
      return multCount;
    }

    size_t num_of_constr() {
      return mimc_constr_wit.constraint_size();
    }

    bool is_satisfied(field_t inputX);

  };

#include "mimc_snark.cpp" 
}

#endif
