#include "font_bmp.hpp"

#include "../utils/csv_reader.hpp"


FontBitmap::FontBitmap(char* szFileName, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset)
	: m_sprSht(szFileName, iGridWidth, iGridHeight, uLength),
	  m_iSizeOffset(iSizeOffset)

{
        
    size_t strLen = strlen(szFileName);
        
	char* szCsvFileName = new char[strLen+1];
	strcpy(szCsvFileName, szFileName);
        
    szCsvFileName[strLen-3] = 'c';
    szCsvFileName[strLen-2] = 's';
    szCsvFileName[strLen-1] = 'v';
    
	m_pCharSizesCsv = new CSVReader(szCsvFileName);
        
    delete[] szCsvFileName;

	int fieldsCount = m_pCharSizesCsv->countFields();

	char** m_pCharSizesCsvData = new char*[fieldsCount];

	m_pCharSizesTable = new int[fieldsCount];
	
	m_pCharSizesCsv->fillArrayWithDataPtr(&m_pCharSizesCsvData);

	for (int i = 0; i < fieldsCount; i++) {
		m_pCharSizesTable[i] = atoi(m_pCharSizesCsvData[i]) + m_iSizeOffset;
	}
	
	delete[] m_pCharSizesCsvData;
}


FontBitmap::~FontBitmap() {
	delete m_pCharSizesCsv;
	delete m_pCharSizesTable;
}


int FontBitmap::getWidthForChar(char c, int hSize) {
	return m_pCharSizesTable[c - 32];
}

size2d_t FontBitmap::getCharSize() {
	return m_sprSht.getFrameSize();
}

int FontBitmap::getCharHeight() {
	return m_sprSht.getFrameSize().h;
}

void FontBitmap::draw(uint8* pBuffer, wchar_t c, int x, int y, int size, Color* color) {
	m_sprSht.draw(pBuffer, c - 32, x, y, false, true);
}
