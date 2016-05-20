#include "variable.hpp"
#include "sbox_snark.hpp"
#include "ntl_header.hpp"
#include "lowmc_snark.hpp"
#include <chrono>

void profile_lowmc_snark(int numofsbox, int numofround, std::vector<NTL::GF2> x);


int main() 
{

  std::vector<NTL::GF2> x;

  for(int i = 0; i < LOWMC_BLOCK_SIZE;i++) x.push_back(NTL::random_GF2());

  //profile_lowmc_snark(20, 55, x);

  
  profile_lowmc_snark(196, 16, x);

  return 0;
}


void profile_lowmc_snark(int numofsbox, int numofround, std::vector<NTL::GF2> x) {

  

  std::vector<Lmat> lowmc_mat = snark::generate_random_matrices(numofround);
  Lmat round_const = snark::generate_round_const(numofround);


  auto time_start = std::chrono::high_resolution_clock::now();
  
  snark::lowmc_snark<NTL::GF2> g_lowmc_snark(numofsbox, numofround, lowmc_mat, round_const);
  g_lowmc_snark.generate_r1_constraint();

  auto time_end1 = std::chrono::high_resolution_clock::now();

  g_lowmc_snark.generate_witness(x);

  auto time_end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time_fp_ms_constr = time_end1-time_start;

  std::chrono::duration<double, std::milli> time_fp_ms_witness = time_end-time_end1;
  

  g_lowmc_snark.print_param();
  printf("\n\n");

  printf("time taken to generate constraint = %f milliseconds\n", time_fp_ms_constr.count());
  printf("time taken to generate witness = %f milliseconds\n\n", time_fp_ms_witness.count());


  printf("Total number of additions = %d\n", g_lowmc_snark.num_of_addition());
  printf("Total number of multiplications = %d\n\n", g_lowmc_snark.num_of_mult() );  

  printf("Total number of rank-1 constraints = %lu\n\n", g_lowmc_snark.num_of_constr());

}
