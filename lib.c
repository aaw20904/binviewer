

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
 
