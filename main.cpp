#include "variable.hpp"
#include "sbox_snark.hpp"
#include "ntl_header.hpp"
#include "lowmc_snark.hpp"
#include "mimc_snark.hpp"
#include <chrono>

void profile_lowmc_snark(int numofsbox, int numofround, std::vector<NTL::GF2> x, 
			 std::vector<Lmat> lowmc_mat, Lmat round_const);

void profile_mimc_snark(int field_size);

int main() 
{

  std::vector<NTL::GF2> x;

  for(int i = 0; i < LOWMC_BLOCK_SIZE;i++) x.push_back(NTL::random_GF2());

  std::vector<Lmat> lowmc_mat = snark::generate_random_matrices(16);

  Lmat round_const = snark::generate_round_const(16);

  //profile_lowmc_snark(20, 55, x, lowmc_mat, round_const);

  profile_lowmc_snark(196, 16, x, lowmc_mat, round_const);

  profile_mimc_snark(1025);

  return 0;
}


void profile_lowmc_snark(int numofsbox, int numofround, std::vector<NTL::GF2> x, 
			 std::vector<Lmat> lowmc_mat, Lmat round_const) {


  auto time_start = std::chrono::high_resolution_clock::now();
  
  snark::lowmc_snark<NTL::GF2> g_lowmc_snark(numofsbox, numofround, lowmc_mat, round_const);
  g_lowmc_snark.generate_r1_constraint();

  auto time_end1 = std::chrono::high_resolution_clock::now();

  g_lowmc_snark.generate_witness(x);

  auto time_end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time_fp_ms_constr = time_end1-time_start;

  std::chrono::duration<double, std::milli> time_fp_ms_witness = time_end-time_end1;
  

  printf("LowMC profile \n\n");
  g_lowmc_snark.print_param();
  printf("\n\n");

  printf("time taken to generate constraint = %f milliseconds\n", time_fp_ms_constr.count());
  printf("time taken to generate witness = %f milliseconds\n\n", time_fp_ms_witness.count());


  printf("Total number of additions = %d\n", g_lowmc_snark.num_of_addition());
  printf("Total number of multiplications = %d\n\n", g_lowmc_snark.num_of_mult() );  

  printf("Total number of rank-1 constraints = %lu\n\n", g_lowmc_snark.num_of_constr());

}


void profile_mimc_snark(int field_size) {

  NTL::GF2X modp = NTL::BuildIrred_GF2X(field_size);
  NTL::GF2E::init(modp);

  std::vector<NTL::GF2E> round_const;
  int nround = (int) (field_size/log2(3.0));

  NTL::GF2E z = NTL::random_GF2E();  /*random input*/
  //printf("rounds = %d\n", nround);

  snark::generate_mimc_roundconst(round_const, field_size, nround);
  //std::cout<<"size = "<<round_const.size()<<std::endl;
  assert(round_const.size() == nround);

  auto time_start = std::chrono::high_resolution_clock::now();

  snark::mimc_perm_snark<NTL::GF2E> g_mimcp_snark(field_size, round_const);
  g_mimcp_snark.generate_r1_constraint();
  auto time_end1 = std::chrono::high_resolution_clock::now();
  g_mimcp_snark.generate_witness(z);

  auto time_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> time_fp_ms_constr = time_end1-time_start;
  std::chrono::duration<double, std::milli> time_fp_ms_wit = time_end-time_end1;

  printf("MiMC profile: \n\n");

  printf("time taken to generate constraint = %f milliseconds\n", time_fp_ms_constr.count());
  printf("time taken to generate witness = %f milliseconds\n", time_fp_ms_wit.count());

  printf("# additions = %u\n", g_mimcp_snark.num_of_addition());
  printf("# multiplications = %u\n\n", g_mimcp_snark.num_of_mult());

  printf("Total number of rank-1 constraints = %lu\n\n", g_mimcp_snark.num_of_constr());

}
