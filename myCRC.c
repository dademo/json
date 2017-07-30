#include <stdio.h>

#include <stdint.h>	

uint64_t mkMyCRC(void* data, int dataSize);	// double = 8 bytes
//int32_t mkMyCRC(void* data, int dataSize);	// double = 8 bytes
void binShowData(void* data, int dataSize);

/*int main()
{
	uint64_t allCRC[10];
	//char toConvert[] = "Hello World !";
	char toConvert[] = "Coucou, je m'appelle Damien et je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert2[] = "Coucou je m'appelle Damien et je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert3[] = "Coucou, je 'appelle Damien et je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert4[] = "Coucou, je m'appelle damien et je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert5[] = "Coucou, je m'appelle damien et je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert6[] = "Coucou, je m'appelle Damien et e teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert7[] = "Coucou, je m'appelle Damien et Je teste mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert8[] = "Coucou, je m'appelle Damien et je testeuh mes fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert9[] = "Coucou, je m'appelle Damien et je teste les fonctions !\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	char toConvert10[] = "Coucou, je m'appelle Damien et je teste mes fonctions ,\nsohflqsihfisqgbqsgfkggjveqsfligsqflbsdkfbsdfbksd";
	allCRC[0] = mkMyCRC(toConvert, sizeof(toConvert));
	allCRC[1] = mkMyCRC(toConvert2, sizeof(toConvert2));
	allCRC[2] = mkMyCRC(toConvert3, sizeof(toConvert3));
	allCRC[3] = mkMyCRC(toConvert4, sizeof(toConvert4));
	allCRC[4] = mkMyCRC(toConvert5, sizeof(toConvert5));
	allCRC[5] = mkMyCRC(toConvert6, sizeof(toConvert6));
	allCRC[6] = mkMyCRC(toConvert7, sizeof(toConvert7));
	allCRC[7] = mkMyCRC(toConvert8, sizeof(toConvert8));
	allCRC[8] = mkMyCRC(toConvert9, sizeof(toConvert9));
	allCRC[9] = mkMyCRC(toConvert10, sizeof(toConvert10));

	for(int i = 0; i < 10; i++)
	{
		printf ("Result is : %ld\t->", allCRC[i]);
		binShowData(&allCRC[i], sizeof(allCRC[i]));
	}

	//int data = 1011;

	//binShowData(&data, sizeof(int));

	return 0;
}*/


uint64_t mkMyCRC(void* data, int dataSize)	// double = 8 bytes
{
	uint64_t outCRC = 0,
		tempData = 0;

	for(int i = 0; i < dataSize; i++)
	{
		char inData = ( (char) *( ((char*)data) +i) );	// Data which will be read ;; Double cast to avoid error (error: cast from pointer to smaller type 'char' loses information)
		tempData = inData << i%(sizeof(uint64_t)*8);
		outCRC ^= tempData;	// XOR
	}
	return outCRC;
}

void binShowData(void* data, int dataSize)
{
	for(int i = dataSize-1; i >= 0; i--)
	{
		char tmpData = (char) *(( (char*)data )+i);
		for(int j = 7; j >= 0; j--)
		{ printf("%d", (tmpData & (1 << j)) >> j); }
		printf (" ");
	}
	printf("\n");
}
