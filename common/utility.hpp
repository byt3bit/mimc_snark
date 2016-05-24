#ifndef UTIL_HPP_
#define UTIL_HPP_
#include "ntl_header.hpp"
#include "common.hpp"
#include <typeinfo>
#include <sstream>


#define BLOCK_MSGE(s) std::clog<<s

namespace snark {

  template<typename field_t>
  field_t ONE()
  {
    if(std::is_same<field_t, NTL::GF2E>::value || std::is_same<field_t, NTL::ZZ>::value ||
       std::is_same<field_t, NTL::GF2>::value)
    {
      return NTL::conv<field_t>((int)1);
    }
  }

  // template<typename field_t>
  // std::string field_rep(field_t a)
  // {
  //   NTL::GF2E temp;
  //   std::stringstream elbuf;
  //   if(std::is_same<field_t, NTL::GF2E>::value)
  //     {
  // 	if(a == 1) 
  // 	  {
  // 	    elbuf<<"1";
  // 	    return elbuf.str();
  // 	  }
  // 	NTL::GF2X b = NTL::rep(a);
	
  // 	long int i;
  // 	for(i = 0; i < NTL::deg(b);i++) 
  // 	  if(NTL::coeff(b,i) == 1) elbuf<<"a^"<<i<<" + ";
  // 	if(NTL::coeff(b,i) == 1) elbuf<<"a^"<<i;
  //     }
  //   else{
  //     std::cout<<"Field type is not NTL\n";
  //   }
  //   return elbuf.str();
  // }

}

#endif
