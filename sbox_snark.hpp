#ifndef SBOX_SNARK_HPP
#define SBOX_SNARK_HPP
#include "r1_constraint.hpp"

namespace snark {

  template<typename field_t> 
  class lowmc_sbox_snark {

  private:

    svar_array<field_t> x1, y1, z1;
    svar_array<field_t> x2, y2, z2;
    svar_array<field_t> x3, y3, z3;

    int xorCount;
    int multCount;
  public:

    svar<field_t> A, B, C;
    svar<field_t> &result1, &result2, &result3;

    snarkcs<field_t> *ptr_snarkcs;

    lowmc_sbox_snark();

    lowmc_sbox_snark(const svar<field_t> a , const svar<field_t> b, const svar<field_t> c, 
		     svar<field_t> &res1, svar<field_t> &res2, svar<field_t> &res3,
		     snarkcs<field_t> *ptr_scs):
      A(a), B(b), C(c), result1(res1), result2(res2), result3(res3),
      xorCount(0), multCount(0)
    {
      ptr_snarkcs = ptr_scs;
    }

    void set_gadget(const svar<field_t> a , const svar<field_t> b, const svar<field_t> c, 
		    svar<field_t> res1, svar<field_t> res2, svar<field_t> res3,
		    snarkcs<field_t> *ptr_scs)
    {
      A = a;
      B = b;
      C = c;
      ptr_snarkcs = ptr_scs;
      result1 = res1;
      result2 = res2;
      result3 = res3;
    }

    //~lowmc_sbox_snark();

    void generate_r1_constraint();
    void generate_witness();

    int num_of_xor() {
      return xorCount;
    }
    int num_of_mult() {
      return multCount;
    }

  };

#include "sbox_snark.cpp"

}


#endif
