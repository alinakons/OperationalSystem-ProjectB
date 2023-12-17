

//made by Alina Poklad and Shantala Gaitonde

void printString(char*);
void printChar(char c);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int, int, int, int);
void readFile(char*, char*, int*);
void executeProgram(char* name);
void terminateProgram();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char* , char* , int);
void handleTimerInterrupt(int,int);
void killProcess(int);
void waitOnProcess(int);

int processActive[8];
int processStackPointer[8];
int currentProcess;
int processWaitingOn[8];


void main() {

//char line[80];
//char buffer[512];

//printString("Hello World\0");
//printString("Project C");
//printString("Enter a line: ");
//readString(line);;
//printString(line);
//readSector(buffer, 30);
//printString(buffer);

//makeInterrupt21();
//interrupt(0x21,1,line,0,0);
//interrupt(0x21,0,line,0,0);
//readSector(buffer, 30);
//printString(buffer);
//interrupt(0x21,2,buffer,30,0);
//interrupt(0x21, 0,buffer,0,0);

char buffer[13312];
int sectorsRead,i,dataseg;
//printString("here");

/*interrupt(0x21, 3, "messag", buffer, &sectorsRead);
if (sectorsRead>0)
	interrupt(0x21, 0, buffer, 0, 0);
else
	interrupt(0x21, 0, "messag not found\r\n", 0, 0);*/

//interrupt(0x21, 4, "tstpr1", 0, 0);
//printChar('b');
//interrupt(0x21,8,"this is a test message","testmg",3);
//printChar('a');
//dataseg = setKernelDataSegment();
for(i=0; i<8; i++) {
	processActive[i] = 0;
	processStackPointer[i] = 0xff00;
}
currentProcess = -1;
//restoreDataSegment(dataseg);
makeInterrupt21();
printString("\r\nStarting shell.\r\n");
interrupt(0x21,4,"shell",0,0);
makeTimerInterrupt();

while(1); 
}

void printString(char* chars)
{
	while(*chars != 0x0)
	{
		printChar(*chars);
		*chars++;
	}
}

void printChar(char c)
{
	char al = c;
	char ah = 0xe;
	int ax = ah * 256 + al;
	interrupt(0x10, ax, 0, 0, 0);
}

void  readString(char* line)
{
int i=0;
char let="";
//char linefeed=0xa;
//char end=0x0;
while(let != 0xd)
	{
	let=interrupt(0x16,0,0,0,0);
	if(let==0x8)
	 {
	  if(i!=0)
	  {
	    printChar(let);
  	    i--;
	    printChar("");
	    printChar(let);
	  }
	 }
	else
	 {
 	  *(line + i)=let;
	  printChar(let);	
          //interrupt(0x10, 0xe*256+let,0,0,0);
	  i++;
	 }
	}
     // *(line + i)=0xa;
     // interrupt(0x10,0xa,0,0,0);
     // *(line + i +1)=0x0;
     // interrupt(0x10,0x0,0,0,0);
    *(line +i)=0xa;
    *(line+i+1)=0x0;
    printChar(0xd);
    printChar(0xa);
}
void readSector(char* buffer, int sector) 
{

        int AX = 2*256+1;
        int BX = buffer;
        int CX = 0*256+(sector+1);
        int DX = 0*256+0x80;
        interrupt(0x13, AX, BX, CX, DX);

}

void handleInterrupt21(int ax, int bx, int cx, int dx) 
{
        if(ax==0) 
	{
          printString(bx);
        }
        else if(ax==1) 
        {
          readString(bx);
        }
        else if(ax==2) {
          readSector(bx, cx);
        }
	else if(ax==3){
	  readFile(bx, cx, dx);
	}
	else if(ax==4) {
	 executeProgram(bx);
	}
	else if(ax==5) {
	terminateProgram();
	}
	else if(ax==6) {
	writeSector(bx,cx);
	}
	else if(ax==7) {
		deleteFile(bx);
	}
	else if(ax==8) {
		writeFile(bx,cx,dx);
	}
	else if(ax==9) {
		killProcess(bx);
	}
	else if(ax==10) {
		waitOnProcess(bx);
	}
        else {
         printString("Error");
        }
}

void readFile (char* filename , char* buffer, int* numOfSectors)
{
     int fileEntry=0;
     int fileFound = 0;
     int i=0;
     char directory[512];
     readSector(directory,2);
 for ( fileEntry=0; fileEntry<512; fileEntry+=32)
  	{
 	if ( filename[0]==directory[fileEntry] && filename[1]==directory[fileEntry+1]
 && filename[2]==directory[fileEntry+2] && filename[3]==directory[fileEntry+3] 
 && filename[4]==directory[fileEntry+4] && filename[5]==directory[fileEntry+5]
 )
{
	*numOfSectors = 0;
	fileFound = 1;
 	  for(i=6 ; i<32; i++)
		{
		if(directory[fileEntry+i]!=0 )
		{
		readSector(buffer + (*numOfSectors) * 512, directory[fileEntry+i]);
		//buffer=buffer+512;
		(*numOfSectors)++;
		}
		else
		{
		break;
		}
      	  }
		break;
	}
   }
	if(!fileFound) {
		*numOfSectors = 0;
		return;
	}
}

void executeProgram(char* name)
{	
	char buffer[13312];
	int i,j;
	int sectorsRead,segment,dataseg;
	readFile(name,buffer,&sectorsRead);
	if(sectorsRead<=0) {
		printString("File not found");
		return;
	}
	dataseg = setKernelDataSegment();
	for(i=0; i<8; i++) {
		if(processActive[i] == 0) {
			break;
	
		}
	} 
	restoreDataSegment(dataseg);
	segment = (i + 2) * 0x1000;
	for(j=0; j<sectorsRead * 512; j++) {
		//printChar(buffer[j]);
		putInMemory(segment,j,buffer[j]);
	}

	initializeProgram(segment);
	dataseg = setKernelDataSegment();
	processActive[i] = 1;
	processStackPointer[i] = 0xff00;
	restoreDataSegment(dataseg);
	
 
}

void terminateProgram()
{
	int dataseg,i;
	dataseg = setKernelDataSegment();
	processActive[currentProcess] = 0;
	for(i=0;i<8;i++) {
                if(processWaitingOn[i] == currentProcess) {
                        processWaitingOn[i] = -1;
                        processActive[i] = 1;
                }
        }

	restoreDataSegment(dataseg);
	while(1);
}

void writeSector(char* buffer, int sector)
{
	int AX = 3*256+1;
        int BX = buffer;
        int CX = 0*256+(sector+1);
        int DX = 0*256+0x80;
        interrupt(0x13, AX, BX, CX, DX);

}

void deleteFile(char* filename)
{
	char directory[512];
	char map[512];
	int i,fileEntry;
	readSector(directory,2);
	readSector(map,1);
	for(fileEntry=0;fileEntry<512;fileEntry+=32) {
		if (filename[0]==directory[fileEntry] && filename[1]==directory[fileEntry+1] && filename[2]==directory[fileEntry+2] 
&& filename[3]==directory[fileEntry+3] && filename[4]==directory[fileEntry+4] && filename[5]==directory[fileEntry+5]) {
		printChar('H');
		directory[fileEntry+0] = '\0';
		for(i=6 ; i<32; i++) {
			if(directory[fileEntry+i] !=0) {
				map[directory[fileEntry+i]] = 0;
				directory[fileEntry+i] = 0;
			} 
			
		}

		}

	}
	writeSector(directory,2);
	writeSector(map,1);
}

void writeFile(char* buffer, char* filename, int numberOfSectors)
{
	char directory[512];
        char map[512];
	int fileEntry,i,dirIndex,j;
	char* tempBuffer;
	int bufferIndex = 0;
	
	readSector(directory,2);
	readSector(map,3);
	//printChar('e');
	 for(fileEntry=0;fileEntry<512;fileEntry+=32) {
		if(directory[fileEntry] == '\0') {
			for(i=0;i<6;i++) {
				directory[fileEntry+i] = '\0';
			}
			for(i=0;i<6;i++) {
				if(filename[i] == '\0') 
					break;
				directory[fileEntry+i] = filename[i];
			}
			dirIndex = fileEntry + 6;
			for(i=3;i<256;i++) {
	
        			if(numberOfSectors == 0){
					break;}
				if(map[i] == 0) {
					map[i] == 0xFF;
					directory[dirIndex] = i;
					dirIndex++;
					numberOfSectors--;
					for(j=0;j<512;j++) {
						tempBuffer[j] = buffer[bufferIndex+j];
					}
					bufferIndex+=512;
					writeSector(tempBuffer,i);
				}

			}
			if(numberOfSectors!=0) {
				return;
			}
			for(i=dirIndex;i<32;i++){
				directory[dirIndex] = '\0';
			}
			writeSector(directory,2);
			writeSector(map,3);
			return;
		}
	}
}
void handleTimerInterrupt(int segment, int sp) {
	//printChar('T');
	//printChar('I');
	//printChar('C');
	int dataseg,i;
	
	dataseg = setKernelDataSegment();
	for(i=0; i<8; i++)
        {
                putInMemory(0xb800,60*2+i*4,i+0x30);
                if(processActive[i]==1)
                        putInMemory(0xb800,60*2+i*4+1,0x20);
                else
                        putInMemory(0xb800,60*2+i*4+1,0);
        }
	if(currentProcess != -1) {
		processStackPointer[currentProcess] = sp;
	}
	while(1) {
		currentProcess++;
		if(currentProcess >= 8) 
			currentProcess = 0;
		if(processActive[currentProcess] == 1)
			break;
	}
	segment = (currentProcess + 2) * 0x1000;
	sp = processStackPointer[currentProcess];
	restoreDataSegment(dataseg);

	returnFromTimer(segment, sp);
}
void killProcess(int processId) {
	int dataseg, i;
	dataseg = setKernelDataSegment();
	processActive[processId] = 0;
	for(i=0;i<8;i++) {
		if(processWaitingOn[i] == processId) {
			processWaitingOn[i] = -1;
			processActive[i] = 1;
		}
	}
	restoreDataSegment(dataseg);
}

void waitOnProcess(int processId) {
	int dataseg;
	dataseg = setKernelDataSegment();
	processActive[currentProcess] = 2;
	processWaitingOn[currentProcess] = processId;
	restoreDataSegment(dataseg);
}
