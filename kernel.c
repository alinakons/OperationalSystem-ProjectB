//made by Alina Poklad and Shantala Gaitonde

void printString(char*);
void printChar(char c);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int, int, int, int);


void main() {

char line[80];
char buffer[512];

//printString("Hello World\0");

printString("Enter a line: ");
//readString(line);;
//printString(line);
//readSector(buffer, 30);
//printString(buffer);

makeInterrupt21();
interrupt(0x21,1,line,0,0);
interrupt(0x21,0,line,0,0);
//readSector(buffer, 30);
//printString(buffer);
interrupt(0x21,2,buffer,30,0);
interrupt(0x21, 0,buffer,0,0);
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
        else {
            printString("Error");
        }
}
