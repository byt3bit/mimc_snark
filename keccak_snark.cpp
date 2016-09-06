#ifndef _KECCAK_SNARK_CPP_
#define _KECCAK_SNARK_CPP_

template<typename field_t> 
std::vector<field_t> 
keccak_snark<field_t>::generate_witness_theta(std::vector<field_t> inputX) {

  std::vector<field_t> temp;
  
  temp = inputX;

  for(int ix = 0;ix < DIMX;ix++) {
    for(int iy = 0;iy < DIMY;iy++) {
      field_t tempvar;

      for(int iz = 0;iz < DIMZ;iz++) {
	tempvar = (temp[(mod(ix-1, 5)*5+0)*64 + iz] + temp[(mod(ix-1, 5)*5+1)*64 + iz] + 
		   temp[(mod(ix-1, 5)*5+2)*64 + iz] + temp[(mod(ix-1, 5)*5+3)*64 + iz] + 
		   temp[(mod(ix-1, 5)*5+4)*64 + iz]);
	tempvar += (temp[(mod(ix-1, 5)*5+0)*64 + mod(iz-1, DIMZ)] + 
		    temp[(mod(ix-1, 5)*5+1)*64 + mod(iz-1, DIMZ)] + 
		    temp[(mod(ix-1, 5)*5+2)*64 + mod(iz-1, DIMZ)] + 
		    temp[(mod(ix-1, 5)*5+3)*64 + mod(iz-1, DIMZ)] + 
		    temp[(mod(ix-1, 5)*5+4)*64 + mod(iz-1, DIMZ)]);
	temp[(ix*DIMY + iy)*DIMZ + iz] = inputX[(ix*DIMY + iy)*DIMZ + iz] + tempvar; 
      }
      xorCount += 10;
    }
  }
  return temp;

}


template<typename field_t> 
std::vector<field_t> 
keccak_snark<field_t>::generate_witness_rho(std::vector<field_t> inputX) {

  std::vector<field_t> temp;
  temp = inputX;

  int ix = 1;
  int iy = 0;

  for(int icount = 0;icount < 25;icount++) {
    for(int t = 0;t < 23;t++) {
      int t1 = ((t+1)*(t+2))/2;
      for(int iz = 0;iz < DIMZ;iz++) {

	temp[(ix*DIMY + iy)*DIMZ + iz] = inputX[(ix*DIMY + iy)*DIMZ + mod(iz - t1, DIMZ)];
	
	int i = ix;
	ix = iy;
	iy = mod(2*i + 3*iy, 5);
      }
    }
  }

  return temp;
}


template<typename field_t> 
std::vector<field_t> 
keccak_snark<field_t>::generate_witness_pi(std::vector<field_t> inputX) {

  std::vector<field_t> temp;
  temp = inputX;

  for(int ix = 0;ix < DIMX;ix++) {
    for(int iy = 0;iy < DIMY;iy++) {
      for(int iz = 0;iz < DIMZ;iz++) {
	int i = mod(ix + 3*iy, 5);
	temp[(ix*DIMY + iy)*DIMZ + iz] = inputX[(i*DIMY + ix)*DIMZ + iz];

      }
    }
  }

  return temp;

}

template<typename field_t> 
void keccak_snark<field_t>::generate_witness(std::vector<field_t> inputX) {

  std::vector<field_t> next_round_input, round_input;

  unsigned int variableCount = (unsigned int) KECCAK_BLOCK_SIZE;
  index_t inputIndex = 0;

  for(int i = 0;i < KECCAK_BLOCK_SIZE;i++) 
    snark_constr_witness.add_witness(inputX[i]);


  round_input = generate_witness_theta(inputX);
  round_input = generate_witness_rho(round_input);
  round_input = generate_witness_pi(round_input);

  next_round_input = round_input;

  index_t variable_index = (index_t) KECCAK_BLOCK_SIZE + 1;

  for(int iround = 0;iround < numofRound;iround++) {


    for(int ix = 0;ix < DIMX;ix++) {
      for(int iy = 0;iy < DIMY;iy++) {
  	for(int iz = 0;iz < DIMZ;iz++) {

  	  index_t inputIndex1 = (index_t) (mod(ix+1,5)*DIMY + iy)*DIMZ + iz + 1 ;
  	  index_t inputIndex2 = (index_t) (mod(ix+2,5)*DIMY + iy)*DIMZ + iz + 1 ;
  	  index_t i = (index_t) (ix*DIMY + iy)*DIMZ + iz + 1;
  	  // inputIndex1 += inputIndex;
  	  // inputIndex2 += inputIndex;
  	  // i += inputIndex;


  	  svar_array<field_t> x1, z1;
  	  svar<field_t> y1(inputIndex2+inputIndex, round_input[inputIndex1-1]);
  	  svar<field_t> a(inputIndex1+inputIndex, round_input[inputIndex2-1]), a0(0, NTL::conv<NTL::GF2>(1));
  	  x1.add_vars({a, a0});
	  
  	  svar<field_t> temp(variable_index);

  	  z1.add_var(temp);
	  
  	  temp.set_index(i+inputIndex);
  	  temp.assign_value(round_input[i-1]);
  	  z1.add_var(temp);

	  field_t tempv;
  	  keccak_chi_snark<field_t> chi_snark(x1, y1, z1, tempv, &snark_constr_witness);
  	  chi_snark.generate_witness();

	  // for(auto sv: z1.svars) {
	  //   if(!sv.is_assigned) std::cout<<"Error\n";
	  // }

  	  // field_t tempv;
  	  // for(auto sv: z1.svars) {
  	  //   if(sv.index == variable_index) {
  	  //     tempv = sv.assigned_value;
  	  //     break;
  	  //   } 
  	  // }
  	  //next_round_input[i] = z1.evaluate();
	  next_round_input[i] = tempv;

  	  xorCount += chi_snark.num_of_xor();
  	  multCount += chi_snark.num_of_mult();

  	  variable_index++;
  	  variableCount++;
  	}
      }
    }

    inputIndex = variableCount;    
    variable_index += ((index_t) KECCAK_BLOCK_SIZE);

    
    round_input = generate_witness_theta(next_round_input);
    round_input = generate_witness_rho(round_input);
    round_input = generate_witness_pi(round_input);

  }

}


template<typename field_t>
void keccak_snark<field_t>::generate_r1_constraint() {

  unsigned int variableCount = (unsigned int) KECCAK_BLOCK_SIZE;
  index_t inputIndex = 0;


  index_t variable_index = 0;
  variable_index += (index_t) KECCAK_BLOCK_SIZE + 1; 

  for(int iround = 0;iround < numofRound;iround++) {

    for(int ix = 0;ix < DIMX;ix++) {
      for(int iy = 0;iy < DIMY;iy++) {
	for(int iz = 0;iz < DIMZ;iz++) {

	  index_t inputIndex1 = (index_t) (mod(ix+1, 5)*DIMY + iy)*DIMZ + iz + 1 ;
	  index_t inputIndex2 = (index_t) (mod(ix+2, 5)*DIMY + iy)*DIMZ + iz + 1 ;
	  index_t i = (index_t) (ix*DIMY + iy)*DIMZ + iz + 1;
	  inputIndex1 += inputIndex;
	  inputIndex2 += inputIndex;
	  i += inputIndex;


	  svar_array<field_t> x1, z1;
	  svar<field_t> y1(inputIndex2);
	  svar<field_t> a(inputIndex1), a0(0, NTL::conv<NTL::GF2>(1));
	  x1.add_vars({a, a0});
	  
	  svar<field_t> temp(variable_index);

	  z1.add_var(temp);
	  
	  temp.set_index(i);
	  z1.add_var(temp);

	  field_t tempv;
	  keccak_chi_snark<field_t> chi_snark(x1, y1, z1, tempv, &snark_constr_witness);
	  chi_snark.generate_r1_constraint();

	  variable_index++;
	  variableCount++;
	}
      }
    }
    inputIndex = variableCount;
    
    variable_index += (index_t) KECCAK_BLOCK_SIZE;
  }

  //snark_constr_witness.print_constraints();

}


template<typename field_t>
void keccak_chi_snark<field_t>::generate_r1_constraint() {

  svar_array<field_t> temp({Y});

  linear_combination<field_t> a(X), b(temp), c(Z);
  constraint<field_t> constr(a, b, c);

  ptr_snarkcs->add_constraint(constr);

}

template<typename field_t> 
void keccak_chi_snark<field_t>::generate_witness() {

  field_t temp, temp1;
  temp = X.evaluate()*Y.value();

  for(auto sv: X.svars) {
    if(!sv.is_assigned) std::cout<<"Error\n";
  } 
  multCount++;

  for(auto sv: Z.svars) {
    if(sv.is_assigned) 
      temp += sv.value();
  }

  r = temp;

  for(auto sv: Z.svars) {
    if(!sv.is_assigned) {
      //std::cout<<"assigned value\n";
      sv.assign_value(temp);
    }
  }

  ptr_snarkcs->add_witness(temp);

}









#endif
