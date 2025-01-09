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
	if(remainder) {
				//1)number
		setTextToMagenta(console);
				printf("%d ", wh);
		setTextToGreen(console);
		//2)plot to a buffer
		writeStringOfBinaries(pToBinaries, string, remainder);
		//3) plot it in a console
		printf("%s \n",string );
	}

}

void plotBinaryBlockOfSymbolsAsString (unsigned char* pToBinaries, int bytesToShow,  HANDLE console ) {
	unsigned char string [1024];
    int wh=0;
	const int symbolsPerString = 8;
	int wholeStrings= bytesToShow / symbolsPerString;

	//a) calculate remainder
	int remainder = bytesToShow % symbolsPerString;
	//b)plot whole strings firstly
		for ( wh=0; wh < wholeStrings; wh++) {
			//1) plot number
			setTextToMagenta(console);
			printf("%02d ", wh);
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
	if (remainder) {
			//1)number
			setTextToMagenta(console);
			printf("%02d ", wh);
			setTextToGreen(console);
	//2)plot to a buffer
			writeStringOfBinaries(pToBinaries, string, remainder);
	//3) plot it in a console
			printf("%s \n",string );
	}

}


void plotToFileBinaryBlockOfSymbolsAsString (FILE** fOutFile,unsigned char* pToBinaries, int bytesToShow ) {
	unsigned char string [2048];
    int wh=0;
	const int symbolsPerString = 8;
	int wholeStrings= bytesToShow / symbolsPerString;

	//a) calculate remainder
	int remainder = bytesToShow % symbolsPerString;
	//b)plot whole strings firstly
		for ( wh=0; wh < wholeStrings; wh++) {
			//1) plot number
			
			fprintf(*fOutFile,"%02d ", wh);
			//2)plot string into buffer
			writeStringOfBinaries(pToBinaries,string,symbolsPerString);
			//3) add pointer string of length
			pToBinaries += symbolsPerString;
			//4)plot a string
			fprintf(*fOutFile, "%s \n", string );
			//5)clear an array 
			memset(string, 0, 1024);
		}
	//c) plot a reminder :
	if (remainder) {
			//1)number
			fprintf(*fOutFile, "%02d ", wh);
	//2)plot to a buffer
			writeStringOfBinaries(pToBinaries, string, remainder);
	//3) plot it in a console
			fprintf(*fOutFile, "%s \n",string );
	}

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

int readAndShowFullFile (FILE** fPointer, HANDLE console,  int numbSymbPerView) {
     unsigned char binBuffer[4096];
	int numberOfChunks, remainder, chunkNumber, fileSize;
	//read size of file
	fseek(*fPointer,0,SEEK_END);
	fileSize = ftell(*fPointer);
	fseek(*fPointer,0,SEEK_SET);
	//amount of chunks of data
	numberOfChunks = fileSize / numbSymbPerView;
	remainder = fileSize % numbSymbPerView;
	printf ("\n \n");
	//show  whole chunks
	for (int x=0; x < numberOfChunks; x++) {
		//plot number of string 
		setTextToMagenta(console);
		printf("Page: %d \n", x);
		//read chunk from HDD
		fread(binBuffer,numbSymbPerView,1,*fPointer);
		//plot a block
		plotBinaryBlockOfSymbolsAsString (binBuffer, numbSymbPerView, console );
	}
	//are there any remainder?
	if (remainder) {
			//plot number of string 
		setTextToMagenta(console);
		printf("Page: L A S T  \n");
		//read chunk from HDD
		fread (binBuffer, remainder, 1, *fPointer);
	    //plot a block
		plotBinaryBlockOfSymbolsAsString (binBuffer, remainder, console );
	}
	return 1;
}


int readAndConvertFullFile (FILE** fPointer, FILE** fDest,  int numbSymbPerView) {
     unsigned char binBuffer[4096];
	int numberOfChunks, remainder, chunkNumber, fileSize;
	//read size of file
	fseek(*fPointer,0,SEEK_END);
	fileSize = ftell(*fPointer);
	fseek(*fPointer,0,SEEK_SET);
	//amount of chunks of data
	numberOfChunks = fileSize / numbSymbPerView;
	remainder = fileSize % numbSymbPerView;
	
	//show  whole chunks
	for (int x=0; x < numberOfChunks; x++) {
		//plot number of string 

		fprintf(*fDest,"Page: %d \n", x);
		//read chunk from HDD
		fread(binBuffer,numbSymbPerView,1,*fPointer);
		//plot a block
	plotToFileBinaryBlockOfSymbolsAsString (fDest, binBuffer, numbSymbPerView );
	}
	//are there any remainder?
	if (remainder) {
			//plot number of string 
		
		fprintf(*fDest, "Page: L A S T  \n");
		//read chunk from HDD
		fread (binBuffer, remainder, 1, *fPointer);
	    //plot a block
		 plotToFileBinaryBlockOfSymbolsAsString (fDest, binBuffer, remainder);
	}
	return 1;
}


int readDisplayDown (FILE** fPointer, HANDLE console, int fileSize, int chunkSize) {
	COORD crd;
	int pageNum; 
	unsigned char binBuffer[1024];
	//a)has the last chunk been acheived?
	//1)read current position
	int currPos = ftell(*fPointer);
	//2)is it the end?
	if (currPos == fileSize) {
		//when end - return code -1
		return -1;
	}
	//3)Is it the last chunk?
	if ((fileSize - currPos) <= chunkSize) {
		//there is the last chuk - less that given chunkSize,
		//so, show the last chunk
		//a) number of page (chunk)
	    pageNum = currPos / chunkSize;
		//b)show it
		crd.X = 3;
		crd.Y = 3;
		SetConsoleCursorPosition(console, crd);
		setTextToMagenta(console);
		printf("Page: %d",pageNum);
		//c)read a last chunk from HDD
		fread (binBuffer, (fileSize - currPos), 1, *fPointer);
		//d) show it 
		plotBinaryBlockOfSymbols(binBuffer, (fileSize - currPos),5,console);
		return 0;
	}
	//4)read non-last chunk
	pageNum =  currPos / chunkSize;
	//b)show it
	crd.X = 3;
	crd.Y = 3;
	SetConsoleCursorPosition(console, crd);
	setTextToMagenta(console);
	printf("Page: %d",pageNum);
	//c)read a last chunk from HDD
	fread (binBuffer, chunkSize, 1, *fPointer);
	//d)show it
	plotBinaryBlockOfSymbols(binBuffer, chunkSize, 5, console);
	return 0;
}

int readDisplayUp (FILE** fPointer, HANDLE console, int fileSize, int chunkSize) {

 	COORD crd;
	int pageNun; 
	unsigned char binBuffer[1024];
    
    //1)read current position
	int currPos = ftell(*fPointer);
	//is it the begin of a file?
	if (currPos == 0) {
		//when begin-exit
		return -1;
	}
	

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






