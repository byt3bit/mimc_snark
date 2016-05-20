#ifndef SBOX_SNARK_CPP_
#define SBOX_SNARK_CPP_


template<typename field_t>
lowmc_sbox_snark<field_t>::lowmc_sbox_snark():
  xorCount(0), multCount(0)
{
}

template<typename field_t>
void lowmc_sbox_snark<field_t>::generate_r1_constraint() {

  x1.add_var(B);
  y1.add_var(C);
  z1.add_vars({A, result1});


  linear_combination<field_t> a(x1), b(y1), c(z1);
  constraint<field_t> constr(a, b, c);
  ptr_snarkcs->add_constraint(constr);
  
  x2.add_var(A);
  svar<field_t> temp(0, NTL::conv<NTL::GF2>(1));
  y2.add_vars({C, temp});
  z2.add_vars({B, result2});

  a.clear();
  b.clear();
  c.clear();
  a.append(x2);
  b.append(y2);
  c.append(z2);

  constr.reset_constraint(a, b, c);
  ptr_snarkcs->add_constraint(constr);

  x3.add_var(A);
  y3.add_vars({B, temp});
  z3.add_vars({B, C, result3});

  a.clear();
  b.clear();
  c.clear();
  a.append(x3);
  b.append(y3);
  c.append(z3);

  constr.reset_constraint(a, b, c);
  ptr_snarkcs->add_constraint(constr);
}

template<typename field_t>
void lowmc_sbox_snark<field_t>::generate_witness()
{

  field_t temp;

  temp = B.value()*C.value();
  multCount++;
  temp = temp + A.value();
  xorCount++;

  result1.assign_value(temp);
  ptr_snarkcs->add_witness(temp);


  temp = A.value()*(C.value()+1);
  temp += B.value();
  xorCount += 2;
  multCount++;
  result2.assign_value(temp);
  ptr_snarkcs->add_witness(temp);

  temp = A.value()*(B.value()+1);
  temp += B.value();
  temp += C.value();
  xorCount += 3;
  multCount++;
  result3.assign_value(temp);
  ptr_snarkcs->add_witness(temp);

}

#endif
