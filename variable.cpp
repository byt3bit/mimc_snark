#ifndef VARIABLE_CPP_
#define VARIABLE_CPP_

template<typename field_t>
linear_term<field_t>::linear_term(const linear_term<field_t> &other) 
{

  this->coef = other.coef;
  this->index = other.index;
}

template<typename field_t>
linear_term<field_t>& linear_combination<field_t>::operator[](const size_t i)
{
  if(i > lin_terms.size()) {
    std::cout<<"Error: index out of bound\n";
    return lin_terms[0];
  }
  return lin_terms[i];
}


template<typename field_t>
linear_term<field_t> linear_term<field_t>::operator*(const field_t field_coef) 
{
  return linear_term<field_t>(this->index, field_coef * this->coef);
}


// linTermWithBinaryCoef::linTermWithBinaryCoef(const linTermWithBinaryCoef &other) 
// {
//   index = other.index;
// }

// bool operator==(const linTermWithBinaryCoef &lhs, const linTermWithBinaryCoef &rhs)
// {
//     return lhs.index == rhs.index;
// }





template<typename field_t>
linear_combination<field_t>::linear_combination() {}

template<typename field_t>
linear_combination<field_t>::linear_combination(const std::vector<linear_term<field_t> > &lts)
{
  for(auto t: lts) 
    lin_terms.add_term(t);
}

template<typename field_t>
linear_combination<field_t>::linear_combination(const linear_combination<field_t> &other) {
  
  for(auto t: other.lin_terms) {
    (this->lin_terms).emplace_back(t);
  }
}

template<typename field_t>
linear_combination<field_t>::linear_combination(const svar_array<field_t> sva) 
{

  for(auto sv: sva.svars) {

    if(sv.index == 0) {
      if(sv.is_assigned) { 
	linear_term<field_t> lt(sv.index, sv.value());
	(this->lin_terms).emplace_back(lt);
      }
      else {
	linear_term<field_t> lt(sv.index, ONE<field_t>());
	(this->lin_terms).emplace_back(lt);
      }
    }
    else {
      linear_term<field_t> lt(sv.index, ONE<field_t>());   
      (this->lin_terms).emplace_back(lt);
    }
  }

}


template<typename field_t>
linear_combination<field_t> linear_combination<field_t>::operator+(const linear_combination<field_t> &other_lc)
{

  linear_combination<field_t> result;

  auto *it = this->lin_terms.begin();
  auto *it1 = other_lc.lin_terms.begin();

  while(it != this->lin_terms.end() && it1 != other_lc.lin_terms.end()) {

    if(it->index < it1->index) {
      result.lin_terms.emplace_back(*it);
      ++it;
    }
    else if(it->index > it1->index) {
      result.lin_terms.emplace_back(*it1);
      ++it1;
    }
    else {
      result.lin_terms.emplace_back(linear_term<field_t>(it->index, it->coef+it1->coef));
      ++it;
      ++it1;
    }
  }
  
  while(it != lin_terms.end() ) {
      result.add_term(*it);
      it++;
  }

  while(it1 != lin_terms.end() ) {
      result.add_term(*it1);
      it1++;
  }
  return result;
}


template<typename field_t>
void linear_combination<field_t>::add_term(const linear_term<field_t> &lterm) 
{
  this->lin_terms.emplace_back(lterm);
}
 

/**newly added for the lowmc function */

template<typename field_t>
void linear_combination<field_t>::append(const svar_array<field_t> sva)
{

  for(auto sv: sva.svars) {
    
    if(sv.index == 0) {
      if(sv.is_assigned) { 
	linear_term<field_t> lt(sv.index, sv.value());
	(this->lin_terms).emplace_back(lt);
      }
      else {
	linear_term<field_t> lt(sv.index, ONE<field_t>());
	(this->lin_terms).emplace_back(lt);
      }
    }
    else {
      linear_term<field_t> lt(sv.index, ONE<field_t>());   
      (this->lin_terms).emplace_back(lt);
    }
  }

}


template<typename field_t>
typename std::vector<linear_term<field_t> >::const_iterator linear_combination<field_t>::begin() const
{
  return lin_terms.begin();
}

template<typename field_t>
typename std::vector<linear_term<field_t> >::const_iterator linear_combination<field_t>::end() const
{
  return lin_terms.end();
}



template<typename field_t>
using variable_assign = std::vector<field_t>;

template<typename field_t>
field_t linear_combination<field_t>::evaluate(std::vector<field_t> &var_assignment) 
{    
  
  field_t sum;
  for(auto t : lin_terms)     
    sum += (t.index == 0 ? ONE<field_t>() : t.coef)*var_assignment[t.index-1]; 

  return sum;
}


// template<typename field_t>
// binary_linear_combination<field_t>::binary_linear_combination() {}

// template<typename field_t>
// binary_linear_combination<field_t>::binary_linear_combination(const binary_linear_combination<field_t> 
// 							      &other) 
// {
//   for(auto bt:other.boolterms)
//     (this->boolterms).emplace_back(bt);
// }

// template<typename field_t>
// binary_linear_combination<field_t>::binary_linear_combination(std::initializer_list<linTermWithBinaryCoef> bts):
//   boolterms(bts)
// {
// }

// template<typename field_t>
// void binary_linear_combination<field_t>::add_term(linTermWithBinaryCoef bt)
// {
//   boolterms.emplace_back(bt);
// }





template<typename field_t>
svar<field_t>::svar():
  variable(0), is_assigned(false)
{
}

template<typename field_t>
svar<field_t>::svar(const index_t index):
  variable(index), is_assigned(false)
{
}


template<typename field_t>
void svar<field_t>::assign_value(field_t val)
{

  if(is_assigned) {
    printf("Warning: Overwriting the value of variable x[%u]\n", index);
  }
  assigned_value = val;
  is_assigned = true;
}

template<typename field_t>
void svar<field_t>::set_index(index_t i)
{
  index = i;
}

template<typename field_t>
field_t svar<field_t>::value() 
{
  if(is_assigned) return assigned_value;
  else 
    {
      std::cout<<"Error: No value is assigned to the variable"<<std::endl;
      return assigned_value;
    }
}

template<typename field_t>
void svar_array<field_t>::copy(const svar_array<field_t> other)
{
  this->svars.clear();
  this->svars = other.svars;
}



template<typename field_t>
field_t svar_array<field_t>::evaluate() const
{

  field_t sum;
  for(auto sv: svars)
    {
      if(sv.is_assigned)
	sum += sv.value();
      else
	std::cout<<"Error:accessing unassigned value\n";
    }
  return sum;
}

template<typename field_t>
void svar_array<field_t>::add_var(const svar<field_t> sv)
{
  svars.emplace_back(sv);
}

template<typename field_t>
void svar_array<field_t>::insert_value(field_t val)
{
  if(svars.size() == 1) 
    {
      assert(svars[0].is_assigned == false);
      svars[0].assign_value(val);
      svars[0].is_assigned = true;
    }
  else 
    {
      /*insert the value at the last index position*/
      /*make sure that this is the unknown variable while generating constraint*/
      assert(svars[svars.size()-1].is_assigned == false);
      svars[svars.size()-1].assign_value(val);
      svars[svars.size()-1].is_assigned = true;
    }

}

#endif

