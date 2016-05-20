#ifndef COMMON_HPP_
#define COMMON_HPP_

#define DEBUG 1

#include <assert.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <stdint.h>
#include <initializer_list>
#include <bitset>           /*used in random bit generation*/


#include <type_traits>      /* used for type checking in tamplate function*/



#define BOLD "\e[1m"
#define CYAN "\e[96m" //light cyan
#define RESET "\033[0m"

#define _DEBUG 1
#define _DEBUG_LOGIC 1
#define _VERBOSE 1


bool getrandbit();


bool getrandbit() {

  static std::bitset<80> state; //Keeps the 80 bit LSFR state
  bool tmp = 0;
  //If state has not been initialized yet
  if (state.none ()) {
    state.set (); //Initialize with all bits set
    //Throw the first 160 bits away
    for (unsigned i = 0; i < 160; ++i) {
      //Update the state
      tmp =  state[0] ^ state[13] ^ state[23]
	^ state[38] ^ state[51] ^ state[62];
      state >>= 1;
      state[79] = tmp;
    }
  }

  bool choice = false;
  do {
    //Update the state
    tmp =  state[0] ^ state[13] ^ state[23]
      ^ state[38] ^ state[51] ^ state[62];
    state >>= 1;
    state[79] = tmp;
    choice = tmp;
    tmp =  state[0] ^ state[13] ^ state[23]
      ^ state[38] ^ state[51] ^ state[62];
    state >>= 1;
    state[79] = tmp;
  } while (! choice);

  return tmp;
}






#endif
