//@autor : Mu'awiyah Namadi
// ceo@namady.com

#include <stdio.h>

//function introduction
int parseInt(const char*);


//program starts
int main(int argc, char** argv)
{
	//take input from commandline args
	char* string = argv[1];
	int number = parseInt(string);
	
	printf("Converted to a number %i\n", number);

	return 0;
}

//function definition
int parseInt(const char *string) {
	int output = 0;
	while(*string)
	{
		output = (output << 3) + (output << 1) + (*string - '0');
		string++;
	}

	return output;
}
