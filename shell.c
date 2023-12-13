
//made by Alina Poklad and Shantala Gaitonde

char* getFilename(char*);
main()
{
	char line[80];
	char command[5];
	char filename[10];
	char buffer[13312];
	int sectorsRead=0;
	int i = 0;
	int j = 0;
	syscall(0,"SHELL>");
	syscall(1,line);

	command[0] = line[0];
	command[1] = line[1];
	command[2] = line[2];
	command[3] = line[3];
	command[4] = '\0';


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
