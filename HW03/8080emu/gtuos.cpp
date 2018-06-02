#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"

using namespace std;

typedef struct threadTable {
	
	char 		nameOfThread[25];
	int         threadId;
	int 		startingTime;
	int 		cycleOfThread;
	int			stateOfThread;
	State8080 	savedRegister;
	State8080   stateRegister;
	memory 		mem;
	int 		stackSpace [100];
	
} threadTable;

typedef struct fileTable {
	
	int 		fileHandleID;
	char 		fileName[25];
	int 		fileSize;
	string 		creationTime;
	string 		lastModification;
	string 		lastAccessTime;
	int 		pointerOfFile;
	
} fileTable;

fileTable disk[4096];
int blockCounter=0;
threadTable thread[20];
threadTable scheduleQueue[20];
int yieldThread = 0;
threadTable currentThread;
int threadCounter = 0;
int contextSwitchCycle = 0;
int nonBlockedThread = 0;
int debugmode=0;


/* syscalls */
uint64_t GTUOS::handleCall(CPU8080 & cpu, int debug_){
	debugmode = debug_;

	/* PRINT_B  */
	if ( (int)cpu.state->a == 1){

		cout << (int)cpu.state->b<<endl;

		setCycle(getCycle()+10);
		
		incrementCycle(10);

		contextSwitchCycle += 10;

		
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}

	
		return 10;
	}
	/* PRINT_MEM */
	else if ( (int)cpu.state->a == 2){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

		cout << (uint8_t)cpu.memory->at(wd)<<endl;

		setCycle(getCycle()+10);
		
		incrementCycle(10);

		contextSwitchCycle += 10;
		
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}

		return 10;
	}
	/* READ_B */
	else if ( (int)cpu.state->a == 3){
		int a;
		cin >> a;
		cpu.state->b=a;


		setCycle(getCycle()+10);
		
		incrementCycle(10);

		contextSwitchCycle += 10;
		
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}


		return 10;
	}
	/* READ_MEM */
	else if ( (int)cpu.state->a == 4){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		uint32_t number;


		cin >> number;

		cpu.memory->at(wd) = (uint8_t)number;


		setCycle(getCycle()+10);
		
		incrementCycle(10);

		contextSwitchCycle += 10;
		
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}
		
		return 10;
	}
	else if ( (int)cpu.state->a == 20) {
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

		cpu.memory->at(wd) = (int)cpu.state->l;

		cout << (int)cpu.state->l<<endl;

	}
	/* PRINT_STR  */
	else if ( (int)cpu.state->a == 5){
	
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0'){
			cout << cpu.memory->at(wd);
			++wd;
		}
		cout <<endl;


		setCycle(getCycle()+100);
		
		incrementCycle(100);

		contextSwitchCycle += 100;
		
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}


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

		setCycle(getCycle()+10);
		
		incrementCycle(10);

		contextSwitchCycle += 10;
		
		if ( contextSwitchCycle > 10 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}



		return 10;
	}
	else if ( (int)cpu.state->a == 8){
		setCycle(getCycle()+50);
		
		incrementCycle(50);

		// round robin icin cycle arttırılır
		contextSwitchCycle += 50;

		// round robin olacak mı check eder
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}



	}
	else if ( (int)cpu.state->a == 9 ){

		setCycle(getCycle()+40);
		
		incrementCycle(40);

		// round robin icin cycle arttırılır
		contextSwitchCycle += 40;

		// round robin olacak mı check eder
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}


		for (int i = 0; i < nonBlockedThread; ++i){
			if ( thread[i].threadId == (int)cpu.state->b ){
				// 2 means blocked
				thread[i].stateOfThread = 2;
			}
		}

	}
	else if ( (int)cpu.state->a == 10 ){
	
		strcpy(scheduleQueue[yieldThread].nameOfThread,thread[threadCounter].nameOfThread);
		scheduleQueue[yieldThread].startingTime = thread[threadCounter].startingTime;
		scheduleQueue[yieldThread].cycleOfThread = thread[threadCounter].cycleOfThread;
		scheduleQueue[yieldThread].stateOfThread = thread[threadCounter].stateOfThread;
		scheduleQueue[yieldThread].threadId = thread[threadCounter].threadId;
		yieldThread++;

		setCycle(getCycle()+40);
		
		incrementCycle(40);

		// round robin icin cycle arttırılır
		contextSwitchCycle += 40;

		// round robin olacak mı check eder
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}
		

	}
	else if ( (int)cpu.state->a == 11 ){
		



		++threadCounter;
		nonBlockedThread = 0;

		for ( int k = 0; k < threadCounter; ++k){
			if ( thread[k].stateOfThread == 1)
				++nonBlockedThread;
		}

		if ( nonBlockedThread > 2 ){
			cpu.state->b = 1;
		}
		else{
			cpu.state->b = 0;
		}

		
		thread[threadCounter-1].startingTime = getCycle();

		thread[threadCounter-1].cycleOfThread = 0;

		/* 0 means running*/
		thread[threadCounter-1].stateOfThread = 0;

		thread[threadCounter-1].threadId = threadCounter + 1;

		thread[threadCounter-1].stateRegister.b = 0;


		setCycle(getCycle()+80);
		
		incrementCycle(80);

		// round robin icin cycle arttırılır
		contextSwitchCycle += 80;

		// round robin olacak mı check eder
		if ( contextSwitchCycle > 100 && debug_ != 0){
			roundRobin(cpu);
			if ( debug_ == 3)
				debug(3);
			contextSwitchCycle = 0;
		}


		thread[threadCounter-1].mem.setBase(0x0000);

		CPU8080 theCPU(&thread[threadCounter-1].mem);

		GTUOS	theOS;

		
			
		char adressOfFunction[35];
		int cc1=0;
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0' ){
			adressOfFunction[cc1++] = cpu.memory->at(wd);
			wd++;

		}
		adressOfFunction[cc1-1] = '\0';


		theCPU.ReadFileIntoMemoryAt(adressOfFunction,0x0000);	
	 	theOS.setCycle(0);
	 	theOS.initCurrent(theCPU,adressOfFunction);
		do{

			theCPU.Emulate8080p(0);
		
			if(theCPU.isSystemCall()){
				theOS.handleCall(theCPU,debug_);
			}


		}while ( !theCPU.isHalted() );


		thread[threadCounter-1].stateOfThread = 0;

		State8080 *state1 = (State8080*)malloc(sizeof(State8080));

		State8080 *savedRegister_ = (State8080*)malloc(sizeof(State8080));

		thread[threadCounter-1].savedRegister = *savedRegister_;

	}

	else if ( (int)cpu.state->a == 12 ){


		char fileName[35];
		int cc1=0;
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0' ){
			fileName[cc1++] = cpu.memory->at(wd);
			wd++;

		}
		fileName[cc1] = '\0';


		
		int fd2 = creat(fileName, O_CREAT);  
		if (fd2 == -1) {
		    close(fd2);
		    cpu.state->b=0;
		}
		else{


			strcpy(disk[blockCounter].fileName,fileName);
			disk[blockCounter].fileSize = 0;
			
			time_t t = time(0);   // get time now
    		tm* now = localtime(&t);
    		
    		string s;
    		char hour[10];
    		char minutes[10];
    		char sec[10];
    		string space = " ";
    		sprintf(hour,"%d",now->tm_hour);
    		sprintf(minutes,"%d",now->tm_min);
    		sprintf(sec,"%d",now->tm_sec);
    		

    		s.append(hour);
    		s.append(space);
    		s.append(minutes);
    		s.append(space);
    		s.append(sec);

    		disk[blockCounter].creationTime = s;
			disk[blockCounter].lastAccessTime = "";
			disk[blockCounter].lastModification = "";
			disk[blockCounter].fileHandleID = -1; // not opened yet, just created
			disk[blockCounter].pointerOfFile = 0;
			disk[blockCounter].fileSize = 0;
			disk[blockCounter].fileHandleID = fd2;
			blockCounter++;



			cpu.state->b=1;
		}


    	

	}
	else if ( (int)cpu.state->a == 13 ){

		if ( close(cpu.state->b) == -1){
			cpu.state->b=0;
		}
		else{
			cpu.state->b=1;
		} 


	}
	else if ( (int)cpu.state->a == 14 ){


		char fileName[35];
		int cc1=0;
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0' ){
			fileName[cc1++] = cpu.memory->at(wd);
			wd++;

		}
		fileName[cc1] = '\0';

		if ( blockCounter == 0){

			time_t t = time(0);   // get time now
    		tm* now = localtime(&t);
    		
    		string s;
    		char hour[10];
    		char minutes[10];
    		char sec[10];
    		string space = " ";
    		sprintf(hour,"%d",now->tm_hour);
    		sprintf(minutes,"%d",now->tm_min);
    		sprintf(sec,"%d",now->tm_sec);
    		

    		s.append(hour);
    		s.append(space);
    		s.append(minutes);
    		s.append(space);
    		s.append(sec);

    		disk[0].creationTime = "";
			disk[0].lastAccessTime = s;
			disk[0].lastModification = s; 
			disk[0].pointerOfFile = 0;
			disk[0].fileHandleID = 3;
			strcpy(disk[blockCounter].fileName,fileName);
			cpu.state->b=3;
			++blockCounter;

		}
	
		for ( int i = 0; i < blockCounter;++i){

			if ( strcmp(disk[i].fileName,fileName) == 0){	

				int fd2 = chmod(disk[i].fileName, 0777);

				if ( fd2 == -1){
					cpu.state->b=0;
					break;
				}


				time_t t = time(0);   // get time now
	    		tm* now = localtime(&t);
	    		
	    		string s;
	    		char hour[10];
	    		char minutes[10];
	    		char sec[10];
	    		string space = " ";
	    		sprintf(hour,"%d",now->tm_hour);
	    		sprintf(minutes,"%d",now->tm_min);
	    		sprintf(sec,"%d",now->tm_sec);
	    		

	    		s.append(hour);
	    		s.append(space);
	    		s.append(minutes);
	    		s.append(space);
	    		s.append(sec);

	    		disk[i].creationTime = "";
				disk[i].lastAccessTime = s;
				disk[i].lastModification = s; 
				disk[i].pointerOfFile = 0;
				disk[i].fileHandleID = 3;
				cpu.state->b=fd2;

			}
		}
		

	}
	else if ( (int)cpu.state->a == 15 ){


		int numberOfBytesToRead = cpu.state->d+1;

		int fd = cpu.state->e;
		int numberOfBytes = 0;


		for ( int i = 0; i < blockCounter;++i){
			if ( disk[i].fileHandleID == fd){

				disk[i].pointerOfFile = numberOfBytesToRead;
				cpu.state->b=numberOfBytesToRead;

				FILE *fp;

				fp=fopen(disk[i].fileName,"r");
				int number_;

				for (int j = 0; j < numberOfBytesToRead;++j){
					fscanf (fp, "%d",&number_);
					cout << number_<<endl;
				}

				fclose(fp);
				disk[i].fileSize += numberOfBytesToRead*sizeof(int);

				time_t t = time(0);   // get time now
	    		tm* now = localtime(&t);
	    		
	    		string s;
	    		char hour[10];
	    		char minutes[10];
	    		char sec[10];
	    		string space = " ";
	    		sprintf(hour,"%d",now->tm_hour);
	    		sprintf(minutes,"%d",now->tm_min);
	    		sprintf(sec,"%d",now->tm_sec);
	    		

	    		s.append(hour);
	    		s.append(space);
	    		s.append(minutes);
	    		s.append(space);
	    		s.append(sec);

	    		disk[i].creationTime = "";
				disk[i].lastAccessTime = s;
				disk[i].lastModification = s;
			}
		}

	}
	else if ( (int)cpu.state->a == 16 ){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

		int number_ = (int)cpu.memory->at(wd);
		
		int fd = cpu.state->d;
		int numberOfBytes = 0;

		for ( int i = 0; i < blockCounter;++i){
			if ( disk[i].fileHandleID == fd){
				FILE *fp;

				fp=fopen(disk[i].fileName,"a");

				fprintf (fp, "%d\n",number_);

				fclose(fp);
				
				disk[i].pointerOfFile = 1;
				cpu.state->b=1;

				disk[i].fileSize += 1*sizeof(int);

				time_t t = time(0);   // get time now
	    		tm* now = localtime(&t);
	    		
	    		string s;
	    		char hour[10];
	    		char minutes[10];
	    		char sec[10];
	    		string space = " ";
	    		sprintf(hour,"%d",now->tm_hour);
	    		sprintf(minutes,"%d",now->tm_min);
	    		sprintf(sec,"%d",now->tm_sec);
	    		

	    		s.append(hour);
	    		s.append(space);
	    		s.append(minutes);
	    		s.append(space);
	    		s.append(sec);

	    		disk[i].creationTime = "";
				disk[i].lastAccessTime = s;
				disk[i].lastModification = s;
			}
		}
	
		

	}
	else if ( (int)cpu.state->a == 17 ){

		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;

		int number_ = (int)cpu.memory->at(wd);

		int fd = cpu.state->d;

		for ( int i = 0; i < blockCounter;++i){
			if ( disk[i].fileHandleID == fd){
				
				FILE *fp;

   				fp = fopen(disk[i].fileName,"a");


				if ( fseek(fp,number_, SEEK_SET ) == -1 ){
					cpu.state->b=0;

				}
				else{
					fclose(fp);
					cpu.state->b=1;
					disk[i].pointerOfFile = number_;

					time_t t = time(0);   // get time now
		    		tm* now = localtime(&t);
		    		
		    		string s;
		    		char hour[10];
		    		char minutes[10];
		    		char sec[10];
		    		string space = " ";
		    		sprintf(hour,"%d",now->tm_hour);
		    		sprintf(minutes,"%d",now->tm_min);
		    		sprintf(sec,"%d",now->tm_sec);
		    		

		    		s.append(hour);
		    		s.append(space);
		    		s.append(minutes);
		    		s.append(space);
		    		s.append(sec);

		    		disk[i].creationTime = "";
					disk[i].lastAccessTime = s;
					disk[i].lastModification = s;
				}

			}

		}

	}
	else if ( (int)cpu.state->a == 18 ){

		char fileName[35];
		int cc1=0;
		uint16_t wd = ((uint16_t)cpu.state->b << 8) | cpu.state->c;
		while ( cpu.memory->at(wd) != '\0' ){
			fileName[cc1++] = cpu.memory->at(wd);
			wd++;

		}
		fileName[cc1-1] = '\0';

		cout << fileName<<endl;

		listDir(fileName,1);


	}


}

int GTUOS::listDir(char* path, int recursive)
{
    struct dirent *direntp = NULL;
    DIR *dirp = NULL;
    size_t path_len;

    path_len = strlen(path);  
   
    dirp = opendir(path);
    if (dirp == NULL)
        return -1;

    while ((direntp = readdir(dirp)) != NULL)
    {
        
        struct stat fstat;
        char name[_POSIX_PATH_MAX + 1];

        
        if ((path_len + strlen(direntp->d_name) + 1) > _POSIX_PATH_MAX)
            continue;

        strcpy(name, path);
        if (name[path_len - 1] != '/')
            strcat(name, "/");
        strcat(name, direntp->d_name);

       
        if ((strcmp(direntp->d_name, ".") == 0) || (strcmp(direntp->d_name, "..") == 0))
            continue;

      
        if (stat(name, &fstat) < 0)
            continue;
        if (S_ISDIR(fstat.st_mode)){
        	char buff[20]; 
        	struct tm * timeinfo;

        	timeinfo = localtime (&(fstat.st_mtime)); 
			strftime(buff, 20, "%b %d %H:%M", timeinfo); 


        	printf("%s 	",direntp->d_name);
            printf( (S_ISDIR(fstat.st_mode)) ? " d" : "-");
		    printf( (fstat.st_mode & S_IRUSR) ? "r" : "-");
		    printf( (fstat.st_mode & S_IWUSR) ? "w" : "-");
		    printf( (fstat.st_mode & S_IXUSR) ? "x" : "-");
		    printf( (fstat.st_mode & S_IRGRP) ? "r" : "-");
		    printf( (fstat.st_mode & S_IWGRP) ? "w" : "-");
		    printf( (fstat.st_mode & S_IXGRP) ? "x" : "-");
		    printf( (fstat.st_mode & S_IROTH) ? "r" : "-");
		    printf( (fstat.st_mode & S_IWOTH) ? "w" : "-");
		    printf( (fstat.st_mode & S_IXOTH) ? "x" : "-");
			printf("	%s",buff);
		    printf("\n");
            if (recursive)
                listDir(name, 1);
        }
    }

    closedir(dirp);
    return 0;
}


void GTUOS::debug(int debug){



	for ( int i = 0; i < threadCounter;++i){
		cout << "thread id: " << thread[i].threadId << "  ";
		cout << "startingTime: " << thread[i].startingTime << "  ";
		cout << "cycleOfThread: " << thread[i].cycleOfThread << "  ";
		if ( thread[i].stateOfThread == 0 )
			cout << "stateOfThread: " << "running" << endl;
		else
			cout << "stateOfThread: " << "blocked" << endl;
	}

}

void GTUOS::initCurrent(CPU8080 & cpu,char fileName[25]){

	int stack[100];
	memory mem;
	mem.setBase(0x0000);

	strcpy(currentThread.nameOfThread,fileName);
	currentThread.startingTime = 0;
	currentThread.cycleOfThread = 0;
	currentThread.stateOfThread = 1;
	currentThread.threadId = 1;
	currentThread.mem = mem;

}



void GTUOS::roundRobin(CPU8080 &cpu){


			
	for ( int j=0; j<threadCounter;++j){
		if (thread[j].stateOfThread == 0 ){
			
			
			if ( debugmode == 2 ){
				cout << "Thread switched " << thread[j].threadId << " -> " << 
				thread[j+1].threadId<<endl;
				cout << "Total Cycles for the blocked thread: " << thread[j+1].cycleOfThread<<endl; 
			}
				
		}
	}
		

	
}



void GTUOS::incrementCycle(int cycle){


	for ( int i = 0; i<threadCounter;++i){
		thread[i].cycleOfThread+=cycle;
	}

}


/* Writing to file( exe.mem) as hex number */
void GTUOS::writeToFile(CPU8080 & cpu, int DEBUG){

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
