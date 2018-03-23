#include "font.hpp"
#include "csv_reader.hpp"


Font::Font(char* szFileName, int iGridWidth, int iGridHeight, uint uLength, int iSizeOffset) : SpriteSheet(szFileName, iGridWidth, iGridHeight, uLength) {

	m_iSizeOffset = iSizeOffset;
        
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


Font::~Font() {
	delete m_pCharSizesCsv;
	delete m_pCharSizesTable;
}


int Font::getSizeForChar(char c) {
	return m_pCharSizesTable[c - 32];
}
