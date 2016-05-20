#ifndef SNARKCS_CPP_
#define SNARKCS_CPP_

template<typename field_t>
constraint<field_t>::constraint() 
{
}

template<typename field_t>
void constraint<field_t>::clear()
{

  A.clear();
  B.clear();
  C.clear();
}

template<typename field_t>
void constraint<field_t>::reset_constraint(const linear_combination<field_t> a,
					   const linear_combination<field_t> b,
					   const linear_combination<field_t> c) 
{

  A.clear();
  B.clear();
  C.clear();
  A = a;
  B = b;
  C = c;

}

template<typename field_t> 
void constraint_system<field_t>::add_constraint(const constraint<field_t> constr) 
{
  constraints.emplace_back(constr);
}

template<typename field_t>
constraint<field_t>& constraint_system<field_t>::operator[](const size_t i)
{
  return constraints[i];
}

template<typename field_t>
snarkcs<field_t>::snarkcs() {}

template<typename field_t>
void snarkcs<field_t>::add_constraint(const constraint<field_t> constr) 
{
  r1_constraints.add_constraint(constr);
}

template<typename field_t>
void snarkcs<field_t>::add_witness(const field_t w)
{
  witness.push_back(w);
}

template<typename field_t>
void snarkcs<field_t>::print_constraints() const
{

  for(size_t i = 0; i < r1_constraints.constraints.size();i++)
    { 
      std::cout<<r1_constraints.constraints[i]<<std::endl;
    }
}

template<typename field_t>
void snarkcs<field_t>::print_witness() const
{

  for(auto w: witness)
    std::cout<<field_rep(w)<<std::endl;
}

template<typename field_t>
void snarkcs<field_t>::pop_constraint()
{
  r1_constraints.constraints.pop_back();
}

template<typename field_t>
const size_t snarkcs<field_t>::constraint_size() const
{
  return r1_constraints.constraints.size();
}

template<typename field_t>
const size_t snarkcs<field_t>::witness_size() const
{
  return witness.size();
}

#endif
