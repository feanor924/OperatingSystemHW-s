#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"


class GTUOS{
	
	public:
		void setCycle(int cycle) { this->cycle = cycle;}
		int getCycle() { 	return cycle; }
		uint64_t handleCall(CPU8080 & cpu,int debug_);
		void writeToFile(CPU8080 & cpu,int DEBUG);
		void incrementCycle(int cycle);
		void roundRobin(CPU8080 &cpu);
		void debug(int debug);
		void initCurrent(CPU8080 & cpu,char fileName[25]);
		int listDir(char* path, int recursive);

	private:
		int cycle;
};

#endif
