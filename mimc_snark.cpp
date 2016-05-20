#ifndef MIMC_SNARK_CPP
#define MIMC_SNARK_CPP

void generate_mimc_roundconst(std::vector<NTL::GF2E> &roundconst, 
			      int blocksize, int numround)
{

  //NTL::GF2E::init(modp);
  for(int i = 0;i < numround;i++) {
    NTL::GF2X tmp;
    NTL::GF2E c;

    for(int j = 0;j < blocksize;j++) {
      NTL::SetCoeff(tmp, (long)j, (NTL::GF2)getrandbit());
    }
    NTL::conv(c, tmp); //conv to GF2E from GF2X
    roundconst.emplace_back(c);
  }
}

template<typename field_t>
mimc_perm_snark<field_t>::mimc_perm_snark():
  xorCount(0), multCount(0) 
{
}

template<typename field_t> 
void mimc_perm_snark<field_t>::generate_r1_constraint()
{

  index_t var_index = 1;

  for(int jround = 0;jround < num_round;jround++) {

    svar<field_t> x0(0), x1(var_index), y(var_index+1), z(var_index+2);
    x0.assign_value(roundConst[jround]);
    svar_array<field_t> X({x0,x1}), Y({x0, x1, y}), Z({y,z});

    //f2n_mul_gadget<field_t> gmul(X, Y, Z, &mimc_constr_wit);
    linear_combination<field_t> A(X), B(Y), C(Z);
    constraint<field_t> constr(A, B, C);
    //gmul.generate_r1cs_constraint();
    mimc_constr_wit.add_constraint(constr);
    var_index += 2;
  }

  //mimc_constr_wit.print_constraints();

}


template<typename field_t> 
void mimc_perm_snark<field_t>::generate_witness(field_t xval)
{

  index_t var_index = 1;
  mimc_constr_wit.add_witness(xval);

  for(int jround = 0;jround < num_round;jround++) {

    //svar<field_t> x0(0, roundConst[jround]), x1(var_index, xval);
    //x0.assign_value(round_const[jround]);
    //svar_array<field_t> X({x0,x1});

    field_t temp = xval + roundConst[jround];
    xorCount++;
    //svar<field_t> y(var_index+1);
    field_t yval = temp*temp;
    multCount++;
    mimc_constr_wit.add_witness(yval);

    temp = temp*yval;
    multCount++;
    mimc_constr_wit.add_witness(temp);

    xval = temp;
    var_index += 2;

    // f2n_mul_gadget<field_t> gmul(X, Y, Z, &ptr_sk);
    // gmul.generate_r1cs_constraint();
    // var_index += 2;
  }

}

#endif
