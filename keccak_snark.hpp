#ifndef _KECCAK_SNARK_HPP_
#define _KECCAK_SNARK_HPP_
#include "common/common.hpp"
#include "common/utility.hpp"
#include "keccak_header.hpp"

int mod(int x, int n) {

  int r = x;
  if(x >= 0) return x%n;
  else {
    do {
      r += n;
    }while(r < 0);
  }
  return r;
}

namespace snark {

  


  template<typename field_t>
  class keccak_snark {

  private:
    int xorCount;
    int multCount;
    int numofRound;

    snarkcs<field_t> snark_constr_witness;
    
  public:
    
    keccak_snark(int numOfround):
      numofRound(numOfround),
      xorCount(0), multCount(0)
    {
    }
  
    void generate_witness(std::vector<field_t> inputX);
    void generate_r1_constraint();

    int num_of_addition() {
      return xorCount;
    }

    int num_of_mult() {
      return multCount;
    }

    size_t num_constraint() {
      return snark_constr_witness.constraint_size();
    }
 
    std::vector<field_t>
    generate_witness_theta(std::vector<field_t> inputX);

    std::vector<field_t> 
    generate_witness_rho(std::vector<field_t> inputX);

    std::vector<field_t> 
    generate_witness_pi(std::vector<field_t> inputX);

  };

  template<typename field_t> 
  class keccak_chi_snark{
    
    int xorCount;
    int multCount;
  public:

    svar<field_t> Y;
    svar_array<field_t> X, &Z;
    field_t &r;

    snarkcs<field_t> *ptr_snarkcs;

    keccak_chi_snark();

    keccak_chi_snark(const svar_array<field_t> x, const svar<field_t> y, 
		     svar_array<field_t> &res, field_t &Res, snarkcs<field_t> *ptr_scs):
      X(x), Y(y), Z(res), r(Res), 
      xorCount(0), multCount(0)
    {
      ptr_snarkcs = ptr_scs;
    }

    void set_gadget(const svar_array<field_t> x, const svar<field_t> y, 
		    svar_array<field_t> &res, field_t &Res, snarkcs<field_t> *ptr_scs)
    {
      X = x; Y = y; Z = res;
      r = Res;
      ptr_snarkcs = ptr_scs;
    }

    void generate_r1_constraint();
    void generate_witness();


    int num_of_xor() {
      return xorCount;
    }

    int num_of_mult() {
      return multCount;
    }
    
  };

#include "keccak_snark.cpp"

}

#endif
