void byteToString (unsigned char sym, char* storage);
void writeStringOfBinaries (unsigned char* pToBinData, char* outString, int bytesToConvert);
void convertBinariesIntoBlock (unsigned char* pToBinaries, char* outBuffer, int bytesPerRow, int numOfRows);
void setTextToGreen (HANDLE console);
void setTextToMagenta (HANDLE console);
void setTextToRed (HANDLE console);
void setTextToWhite (HANDLE console);
int openFilePrompt (FILE** fPointer, int* pFileSize, HANDLE console);
void plotBinaryBlockOfSymbols (unsigned char* pToBinaries, int bytesToShow, int startRow, HANDLE console );