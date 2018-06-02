#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "8080emuCPP.h"
#include "gtuos.h"

using namespace std;

/* syscalls */
uint64_t GTUOS::handleCall(const CPU8080 & cpu){

	/* PRINT_B  */
	if ( (int)cpu.state->a == 1){

		cout << (int)cpu.state->b<<endl;
		return 10;
	}
	/* PRINT_MEM */
	else if ( (int)cpu.state->a == 2){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

		cout << (uint8_t)cpu.memory->at(wd)<<endl;

		return 10;
	}
	/* READ_B */
	else if ( (int)cpu.state->a == 3){
		int a;
		cin >> a;
		cpu.state->b=a;
		return 10;
	}
	/* READ_MEM */
	else if ( (int)cpu.state->a == 4){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		uint32_t number;


		cin >> number;



		cpu.memory->at(wd) = (uint8_t)number;
		
		return 10;
	}
	/* PRINT_STR  */
	else if ( (int)cpu.state->a == 5){
	
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0'){
			cout << cpu.memory->at(wd);
			++wd;
		}
		cout <<endl;
		return 100;
	}
	/* READ_STR  */
	else if ( (int)cpu.state->a == 6){
		string str;
		int counter=0;
		
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		
		cin >> str;

		while ( counter != str.size()){
			cpu.memory->at(wd) = str[counter];
			++counter;
			++wd;
		}
		cpu.memory->at(wd)='\0';
		
		return 100;
	}
	else if(  (int)cpu.state->a == 7 ){


		int number;

	   	number = rand() % 250 + 0;


		cpu.state->b=number;

		return 10;
	}

}




/* Writing to file( exe.mem) as hex number */
void GTUOS::writeToFile(const CPU8080 & cpu, int DEBUG){

	FILE *f;
	f = fopen("exe.mem","w");

	int z=0;
	
	if ( DEBUG == 0 || DEBUG == 1 || DEBUG == 2){
		for ( int i=0;i<65535;i=i+16){
			fprintf(f, "%04x", i);
			fprintf(f, "   ");
			for (int j=0;j<16;++j){
				fprintf(f, "%04x", (unsigned)cpu.memory->at(j+z));
				fprintf(f," ");	
			}
			z+=16;
			fprintf(f,"\n");
		}
	}
	fclose(f);
}
