#ifndef LOWMC_SNARK_CPP
#define LOWMC_SNARK_CPP


Lmat generate_random_matrix() 
{
  Lmat rMat;
  std::bitset<LOWMC_BLOCK_SIZE> temp;
  temp.reset();

  for(int i = 0; i < LOWMC_BLOCK_SIZE;i++) {
    for(int j = 0;j < LOWMC_BLOCK_SIZE;j++) {
      temp[j] = getrandbit();
    }
    rMat.emplace_back(temp);
  }
  return rMat;
}

std::vector<Lmat> generate_random_matrices(int numofround) 
{
  std::vector<Lmat> vec_lmat;

  for(int iround = 0; iround < numofround;iround++) { 
    Lmat temp = generate_random_matrix();
    vec_lmat.emplace_back(temp);
  }
  return vec_lmat; 
}

Lmat generate_round_const(int numofround) 
{
  Lmat rMat;
  std::bitset<LOWMC_BLOCK_SIZE> temp;
  temp.reset();

  for(int i = 0; i < numofround;i++) {
    for(int j = 0;j < LOWMC_BLOCK_SIZE;j++) {
      temp[j] = getrandbit();
    }
    rMat.emplace_back(temp);
  }
  return rMat;
}


template<typename field_t>
std::vector<field_t> 
lowmc_snark<field_t>::generate_witness_from_linMat(Lmat roundMat, 
						   std::bitset<LOWMC_BLOCK_SIZE> roundconst, 
						   std::vector<field_t> x)
{

  std::vector<field_t> lin_out;    

  field_t temp;
  for(int iRow = 0;iRow < LOWMC_BLOCK_SIZE;iRow++) {
    temp = 0;
    for(int jCol = 0;jCol < LOWMC_BLOCK_SIZE;jCol++) {
      if(roundMat[iRow][jCol]) {
	temp +=  x[jCol];
	xorCount++;
      }
    }
    if(roundconst[iRow]) temp += ONE<field_t>();
    lin_out.emplace_back(temp);
  }

  return lin_out;
}


template<typename field_t>
void lowmc_snark<field_t>::generate_r1_constraint()
{

  index_t variable_index = 0; 
  unsigned int variableCount = (unsigned int)LOWMC_BLOCK_SIZE;
  index_t inputIndex = 1;

  int sboxbits = 3*numofSbox;
  int i;

  variable_index = (index_t)LOWMC_BLOCK_SIZE + 1;
  for(int jround = 0;jround < numofRound;jround++) {

    for(i = 0;i < sboxbits;i += 3) {
      
      svar<field_t> a(inputIndex), b(inputIndex+1), c(inputIndex+2);
      svar<field_t> sbOutput1(variable_index), sbOutput2(variable_index+1), 
	sbOutput3(variable_index+2);

      lowmc_sbox_snark<field_t> sb_snark(a, b, c, sbOutput1, sbOutput2, sbOutput3, 
					 &snark_constr_witness);


      sb_snark.generate_r1_constraint();
      
      inputIndex += 3;
      variable_index +=3;
      variableCount += 3;
    } 

    for(int j = i;j < LOWMC_BLOCK_SIZE;j++) { 

      inputIndex++;
      variable_index++;
      variableCount++;
    }
    
  }


}

template<typename field_t>
void lowmc_snark<field_t>::generate_witness(std::vector<field_t> x)
{
  
  index_t variable_index = 0; 
  unsigned int variableCount = (unsigned int) LOWMC_BLOCK_SIZE;
  index_t inputIndex = 1;

  int sboxbits = 3*numofSbox;
  std::vector<field_t> next_round_input(x), round_input;

  int i;

  for(i = 0;i < LOWMC_BLOCK_SIZE;i++) 
    snark_constr_witness.add_witness(x[i]);

  i = 0;
  for(int jround = 0;jround < numofRound;jround++) {

    variable_index = (index_t)variableCount + 1; /* 0 index is for constant 
						    in linear combination*/

    round_input = next_round_input;
   
    for(i = 0;i < sboxbits;i += 3) {
      
      svar<field_t> a(inputIndex, round_input[inputIndex-1]), 
	b(inputIndex+1, round_input[inputIndex]), 
	c(inputIndex+2, round_input[inputIndex+1]);

      svar<field_t> sbOutput1(variable_index), sbOutput2(variable_index+1), 
	sbOutput3(variable_index+2);

      lowmc_sbox_snark<field_t> sb_snark(a, b, c, sbOutput1, sbOutput2, sbOutput3, 
					 &snark_constr_witness);

      sb_snark.generate_witness();
     
      xorCount += sb_snark.num_of_xor();
      mulCount += sb_snark.num_of_mult();

      next_round_input[i]   = sbOutput1.value();
      next_round_input[i+1] = sbOutput2.value();
      next_round_input[i+3] = sbOutput3.value();

      inputIndex += 3;
      variable_index +=3;
      variableCount += 3;
    }
// #ifdef CHECK_LOGIC
//     assert(mulCount == ((jround+1)*3*NUMOF_SBOX)); 
// #endif
    for(int j = i;j < LOWMC_BLOCK_SIZE;j++) {
      next_round_input[j] = round_input[j];
      variable_index++;
      inputIndex++;
      variableCount++;
    } 

    assert(lowmcMat.size() == numofRound);
    assert(roundConst.size() == numofRound);
    
    if(!lowmcMat.empty()) 
      next_round_input = generate_witness_from_linMat(lowmcMat[jround], roundConst[jround], 
						      next_round_input);
     
  }

}

#endif
