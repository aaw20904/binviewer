#include <conio.h>
#include <windows.h>
#include <stdio.h>
//author : Andrii Androsowych
void byteToString (unsigned char sym, char* storage) {
	unsigned const char mask = 0x80;
  //processing first whole byte
	for (int cnt=0; cnt < 8; cnt++) {
		*storage = (sym & mask) ? '1' : '0';
		sym <<= 1;
		storage++;
	}
	//add extra space
	*storage = ' ';
}


void writeStringOfBinaries (unsigned char* pToBinData, char* outString, int bytesToConvert) {
	for (int cnt=0; cnt < bytesToConvert; cnt++ ) {
		//a)write a byte inerpretation + one space (9bytes)
		byteToString(*pToBinData, outString);
		//b) increase  string buffer pointer
		outString = outString + 9;
		//c) increase pointer to a symbol
		pToBinData++; 
	} 
}


void convertBinariesIntoBlock (unsigned char* pToBinaries, char* outBuffer, int bytesPerRow, int numOfRows) {

	for (int rows=0; rows < numOfRows;rows++ ) {
		//a)write whole row
		writeStringOfBinaries(pToBinaries, outBuffer, bytesPerRow);
		//b) increase byte pointer
		pToBinaries +=  bytesPerRow;
		//c)increase string pointer
		outBuffer +=  bytesPerRow * 9;
		//d) add new string symbol
		*outBuffer = '\n';
		//e)move pointer up
		outBuffer++;
	}
	
}

void plotBinaryBlockOfSymbols (unsigned char* pToBinaries, int bytesToShow, int startRow, HANDLE console ) {
	unsigned char string [1024];
    int wh=0;
	const int symbolsPerString = 6;
	int wholeStrings= bytesToShow / symbolsPerString;
	//move to start position
	COORD crd = {Y:startRow, X:0};
	SetConsoleCursorPosition(console, crd );
	//a) calculate remainder
	int remainder = bytesToShow % symbolsPerString;
	//b)plot whole strings firstly
		for ( wh=0; wh < wholeStrings; wh++) {
			//1) plot number
			setTextToMagenta(console);
			printf("%d ", wh);
			setTextToGreen(console);
			//2)plot string into buffer
			writeStringOfBinaries(pToBinaries,string,symbolsPerString);
			//3) add pointer string of length
			pToBinaries += symbolsPerString;
			//4)plot a string
			printf("%s \n",string );
			//5)clear an array 
			memset(string, 0, 1024);
		}
	//c) plot a reminder :
	//1)number
	setTextToMagenta(console);
			printf("%d ", wh);
	setTextToGreen(console);
	//2)plot to a buffer
	writeStringOfBinaries(pToBinaries, string, remainder);
	//3) plot it in a console
	printf("%s \n",string );
}


int openFilePrompt(FILE** fPointer, int* pFileSize, HANDLE console){
	//move a cursor into a specific position:
	char filename [32];
	COORD crd;
	crd.X = 1;
	crd.Y = 1;
	SetConsoleCursorPosition (console, crd);
	setTextToMagenta (console);
	puts ("Enter filename:");
	setTextToWhite (console);
	//a)read filename
	gets (filename);
	*fPointer = fopen(filename,"rb");
	if(*fPointer==NULL){
		//when file not found:
		SetConsoleCursorPosition (console, crd);
		setTextToRed (console);
		puts("************File not found!************ ");
		setTextToWhite (console);
		return -1;
	}
	//b)read file size
	fseek(*fPointer,0,SEEK_END);
	*pFileSize = ftell(*fPointer);
	fseek(*fPointer,0,SEEK_END);
	SetConsoleCursorPosition (console, crd);
	setTextToGreen (console);
	printf(" %s successfully opened! %d B",filename, *pFileSize );
	setTextToWhite (console);
	return 0;
}

void readAndShowChunk (FILE* fPointer, int chunkSize, int fileLength) {
  //a) calculate number of chunk
  int numOfChunk, currPos;
  currPos = ftell(fPointer);
  numOfChunk = fileLength / chunkSize;	
}


void setTextToGreen(HANDLE console) {
	 SetConsoleTextAttribute(console, 0x0002);
}
 
void setTextToMagenta(HANDLE console) {
	 SetConsoleTextAttribute(console, 0x0005);
}

void setTextToRed(HANDLE console) {
	 SetConsoleTextAttribute(console, 0x0004);
}

void setTextToWhite(HANDLE console) {
	 SetConsoleTextAttribute(console, 0x0007);
}






