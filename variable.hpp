#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_
#include "common.hpp"
#include "utility.hpp"

namespace snark {

  typedef uint32_t index_t;

  template<typename field_t>
  class svar;

  template<typename field_t>
  class svar_array;


  template<typename field_t> 
  class linear_combination;

  

  class variable {

  public:
    index_t index;
    variable(const index_t index = 0):
      index(index)
    {
    }
  };

  template<typename field_t>  
  class linear_term 
  {
    
  public:
    index_t index;
    field_t coef;
    
    linear_term() {};
    linear_term(long unsigned int index, field_t val):
      index(index), coef(val)
    {
    }
    
    linear_term(const linear_term<field_t> &other);
    
    void set(long unsigned int index, field_t val) {
      index = index; 
      coef = val;
    }

    /* this computes A*(B_i*x_i) */
    linear_term<field_t> operator*(const field_t field_coef);

    
    friend std::ostream& operator<< (std::ostream &out, const linear_term &lt) {

      if(std::is_same<field_t, NTL::GF2>::value) {
	if(lt.index != 0) out<<" X_"<<lt.index;
	else
	  out<<" 1";
      } 
      else if(std::is_same<field_t, NTL::GF2E>::value){
	if(lt.index == 0) out<<" const";
	else
	  out<<lt.coef<<" *X_"<<lt.index;
      }
      return out;
    }

  };

  template<typename field_t>
  inline bool operator==(const linear_term<field_t> &lhs, const linear_term<field_t> &rhs) {
      return lhs.index == rhs.index && lhs.coef == rhs.coef ;
  }

  /* linear term X_i with coefficinets 1 or 0 */
  class linTermBinaryCoef: public variable{
    
  public:
    linTermBinaryCoef() {};
    linTermBinaryCoef(const long unsigned int index):
      variable(index)
    {
    }

    linTermBinaryCoef(const linTermBinaryCoef &other) {
      index = other.index;
    }
 
    void reset(long unsigned int index) {
      index = index; 
    }

    friend std::ostream& operator<< (std::ostream &out, const linTermBinaryCoef &lt) {
      out<<" X_"<<lt.index;
      return out;
    }

  };


  /* data structure to represent the rank-1 constraints 
     of the for (A,w)*(B,w) = (C,w)

   */

  template<typename field_t> 
  class linear_combination {

  public:
    std::vector<linear_term<field_t> > lin_terms;
    
    linear_combination();

    linear_combination(const linear_combination<field_t> &other);

    /*constructed by a vector of linear terms*/
    linear_combination(const std::vector<linear_term<field_t> > &lts);
    
    /*constructed from snark variable array */
    linear_combination(const svar_array<field_t> sva);

    linear_combination(const std::initializer_list<svar<field_t> > sva);


    typename std::vector<linear_term<field_t> >::const_iterator begin() const;
    typename std::vector<linear_term<field_t> >::const_iterator end() const;

    void clear() {
      if(!lin_terms.empty()) lin_terms.clear();
    }

    linear_term<field_t>& operator[](const size_t i); 
    
    size_t size()
    {
      return lin_terms.size();
    }
    
    void add_term(const linear_term<field_t> &lterm); 

    void append(const svar_array<field_t> sva);


    linear_combination<field_t> operator+(const linear_combination<field_t> &other_lc);
    
    linear_combination<field_t> operator+(const linear_term<field_t> &lt);
    
    bool operator==(linear_combination<field_t> &other);

    field_t evaluate(std::vector<field_t> &var_assignment);

    friend std::ostream& operator<< (std::ostream &out, const linear_combination<field_t> &lc)
    {
      out<<"( ";
      size_t i = 0;
      for(i = 0; i < lc.lin_terms.size()-1;i++)
	out<<lc.lin_terms[i]<<" + ";
      out<<lc.lin_terms[i]<<" )";
      return out;
    }

  };

  // template<typename field_t>
  // class binary_linear_combination {

  // public:
  //   std::vector<linTermWithBinaryCoef> boolterms;

  //   binary_linear_combination();

  //   binary_linear_combination(const binary_linear_combination<field_t> &other);

  //   binary_linear_combination(std::initializer_list<linTermWithBinaryCoef> bts);

  //   binary_linear_combination(svar_array<field_t> sva);

  //   void add_term(linTermWithBinaryCoef bt);

  //   binary_linear_combination<field_t> operator+(const linTermWithBinaryCoef);

  // };


  /*variable: input to snark e.g x_0, x_1 etc.*/
  template<typename field_t>
  class svar: public variable {

    /*assigned value to the variable*/
    field_t assigned_value;
  public:

    bool is_assigned;
    svar();
    svar(const index_t index = 0);
    svar(const index_t index, const field_t val):
      variable(index), assigned_value(val), is_assigned(true)
    {
    }

    void assign_value(field_t val);
    void set_index(index_t i);

    field_t value();
     
  };


  /*array of variables - svar: input to the snark*/
  template<typename field_t>
  class svar_array {

  public:

    std::vector<svar<field_t> > svars;

    svar_array() {};
    svar_array(const std::vector<svar<field_t> > sva):
      svars(sva)
    {
    }
    svar_array(const std::initializer_list<svar<field_t> > sva):
      svars(sva)
    {
    }

    void copy(const svar_array<field_t> other);

    void add_var(const svar<field_t> sv);

    void add_vars(const std::initializer_list<svar<field_t> > sva)
    {
      svars.insert(svars.end(), sva.begin(), sva.end());
    }

    size_t size()
    {
      return svars.size();
    }

    field_t evaluate() const;

    void pop_back() {
      svars.pop_back();
    }

    void insert_value(field_t val);
 
  };

#include "variable.cpp"

}//mimc_snark

#endif
