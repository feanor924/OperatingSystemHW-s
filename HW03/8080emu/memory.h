#ifndef H_MEMORY
#define H_MEMORY

#include "memoryBase.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// This is just a simple memory with no virtual addresses. 
// You will write your own memory with base and limit registers.

class memory: public MemoryBase {
public:
        memory(){mem = (uint8_t*) malloc(0x10000);}
		~memory(){ free(mem);}
		virtual uint8_t & at(uint32_t ind){ return mem[ind];}
		virtual uint8_t & physicalAt(uint32_t ind){ return mem[ind];}
    	void setBase(uint16_t base_){ base = base_; }
    	void setLimit(uint16_t limit_) { limit = limit_; }
    	uint16_t getBase(){ return base; }
    	uint16_t getLimit(){ return limit; }
private:
		uint8_t * mem;
		uint32_t base;
		uint32_t limit;
		
};

#endif


