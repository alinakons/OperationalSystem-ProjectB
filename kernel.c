
//made by Alina Poklad and Shantala Gaitonde

void printString(char*);
void printChar(char c);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int, int, int, int);
void readFile(char*, char*, int*);
void executeProgram(char* name);
void terminateProgram();

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
int sectorsRead;
//printString("here");
makeInterrupt21();
/*interrupt(0x21, 3, "messag", buffer, &sectorsRead);
if (sectorsRead>0)
	interrupt(0x21, 0, buffer, 0, 0);
else
	interrupt(0x21, 0, "messag not found\r\n", 0, 0);*/

//interrupt(0x21, 4, "tstpr1", 0, 0);
interrupt(0x21, 4, "shell", 0, 0);
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
	 executeProgram((char*)bx);
	}
	else if(ax==5) {
	terminateProgram();
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
{	int i = 0;
	char buffer[13312]; 
	int numSectors;
	int segment = 0x2000;
	int address;
	readFile(name,buffer,&numSectors);
	//printChar(char *(numSectors));
	for(i=0;i<numSectors * 512;i++) {
		putInMemory(0x2000,i,buffer[i]);
	}
	launchProgram(segment);
}

void terminateProgram()
{
	char shellname[6]; 
	shellname[0] = 's';
	shellname[1] = 'h';
	shellname[2] = 'e';
	shellname[3] = 'l';
	shellname[4] = 'l';
	shellname[5] = '\0';

	executeProgram(shellname);
}
