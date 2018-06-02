#include <iostream>
#include <string>
#include "memory.h"

using namespace std;

// virtual adressi döndürür
uint8_t & memory::at(uint32_t ind){

		return mem[ind];


}

// fiziksel adressi döndürür
uint8_t & memory::physicalAt(uint32_t ind){

	
		return mem[ind];


}
