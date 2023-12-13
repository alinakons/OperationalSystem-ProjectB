
//made by Alina Poklad and Shantala Gaitonde

char* getFilename(char*);
main()
{
	char line[80];
	char command[10];
	char filename[10];
	char filename1[10];
	char* fileText;
	int bufferIndex;
	char buffer[13312];
	int sectorsRead=0;
	int i = 0;
	int j = 0;
	char directory[512];
	syscall(0,"SHELL>");
	syscall(1,line);
	command[0] = line[0];
	command[1] = line[1];
	command[2] = line[2];
	command[3] = line[3];
	command[4] = line[4];
	command[5] = line[5];
	command[6] = '\0';


	if(command[0] == 't' && command[1] == 'y' && command[2] == 'p' && command[3] == 'e')
	{
		i = 5;
        	j = 0; 
        	while(line[i] != '\n') {
                	filename[j] = line[i];
                	i++;
                	j++;
        	}
        	filename[j] = '\0';

		syscall(3,filename,buffer,&sectorsRead);
		if(sectorsRead > 0) {
			syscall(0,buffer);
		}
		else {
			syscall(0,"File not found\n");
		}
	}
	else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'e' && command[3] == 'c') {
		i = 5;
		j = 0;
		while(line[i] != '\n') {
			filename[j] = line[i];
			i++;
			j++;
		}
		filename[j] = '\0';
		syscall(4,filename);
	}
	else if(command[0] == 'd' && command[1] == 'i' && command[2] == 'r') {
		syscall(2,directory,2);
		for(i=0;i<512;i+=32) {
			if(directory[i] != '\0') {
				filename[0] = directory[i];
				filename[1] = directory[i+1];
				filename[2] = directory[i+2];
				filename[3] = directory[i+3];
				filename[4] = directory[i+4];
				filename[5] = directory[i+5];
				filename[6] = '\0';
				syscall(0,filename);
				syscall(0,"\r\n");
			}
		}
	}
	else if(command[0] == 'd' && command[1] == 'e' && command[2] == 'l') {
		i = 4;
                j = 0;
                while(line[i] != '\r') {
                        filename[j] = line[i];
                        i++;
                        j++;
                }
                filename[j] = '\0';
		syscall(0,filename);
		syscall(7,filename);
	}
	else if(command[0] == 'c' && command[1] == 'o' && command[2] == 'p' && command[3] == 'y') {
		i = 5;
		j = 0;
		while(line[i] != ' ') {
			filename[j] = line[i];
			i++;
			j++;
		}
		filename[j] = '\0';
		i++;
		j=0;
		while(line[i] != '\n') {
			filename1[j] = line[i];
			i++;
			j++;
		}
		filename1[j] = '\0';
		syscall(3,filename,buffer,&sectorsRead);
		syscall(8,buffer,filename1,sectorsRead);
	}
	else if(command[0] == 'c' && command[1] == 'r' && command[2] == 'e' && command[3] == 'a' && command[4] == 't' && command[5] == 'e') {
		i = 7;
		j = 0;
		while(line[i] != '\n') {
			filename[j] = line[i];
			i++;
			j++;
		}
		filename[j] = '\0';
		bufferIndex = 0;
		while(1) {
			i = 0;
			syscall(0,"Enter a line:");
			syscall(1,fileText);
			if(fileText[0] == '\r'){
				break;
			}
			while(fileText[i] != '\n') {
				buffer[bufferIndex] = fileText[i];
				bufferIndex++;
				i++;
			}
			
		}
		syscall(8,buffer,filename,3);
	}
	syscall(5);
        while(1);
}

char* getFilename(char* line)
{
	int i = 5;
	int j = 0;
	char filename[10];
	while(line[i] != '\n') {

		filename[j] = line[i];
		i++;
		j++;
	}
	filename[j] = '\0';
	return filename;
}
