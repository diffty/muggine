#include "csv_reader.hpp"



CSVReader::CSVReader(char* szFilename) {
	initList(&m_llFields);
	initList(&m_llData);

	m_iNbFields = 0;

	m_szFilename = szFilename;

	readFile();
}


CSVReader::~CSVReader() {
	destroyData();
	clearList(&m_llFields);
	clearList(&m_llData);
}

void CSVReader::destroyData() {
	LLNode* pCurrNode = m_llData.pHead;

	while (pCurrNode != NULL) {
		delete ((char*)pCurrNode->pData);
		pCurrNode = pCurrNode->pNext;
	}
}

void CSVReader::readFile() {
	FILE* pFile = fopen(m_szFilename, "r");
	long lFileSize;
	char* buffer;

	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	buffer = new char[lFileSize+1];

	size_t sizeRead = fread(buffer, sizeof(char), lFileSize, pFile);

	buffer[sizeRead] = '\0';

	fclose(pFile);

	decodeBuffer(buffer);

	delete buffer;
}

void CSVReader::decodeBuffer(char* szBuffer) {
	long lBufferSize = strlen(szBuffer);

	char c;

	int iCellSize = 0;
	int iFileCur = 0;
	int iNbRows = 1;
	
	while ((c = szBuffer[iFileCur]) != '\0') {
		if (c == ';' || c == '\n') {
			char* szCurrCellContent = new char[iCellSize+1];

			spliceString(szBuffer, iFileCur - iCellSize, iFileCur-1, szCurrCellContent);
			convertReturnCharacters(szCurrCellContent);
			addDataToList(&m_llData, szCurrCellContent);

			iCellSize = 0;

			if (c == '\n') {
				if (iNbRows == 1) {
					m_iNbFields = countNodesInList(&m_llData);
				}

				iNbRows++;
			}
		}
		else {
			iCellSize++;
		}
		
		iFileCur++;
	}

	m_iNbDataRows = iNbRows - 2;

	if (m_iNbFields == 0) {
		m_iNbFields = countNodesInList(&m_llData);
	}
}

char* CSVReader::getData(char* szFieldName, int iRowNum) {
	int iFieldId = getFieldId(szFieldName);

	if (iFieldId == -1) {
		printf("<!> Field name not found...\n");
		return NULL;
	}

	LLNode* pNode = getNodeInList(&m_llData, (m_iNbFields * (1 + iRowNum)) + iFieldId);

	return (char*) pNode->pData;
}

int CSVReader::getFieldId(char* szFieldName) {
	LLNode* pCurrNode = m_llData.pHead;

	for (int i = 0; i < m_iNbFields; i++) {
		if (strcmp(szFieldName, (char*) pCurrNode->pData) == 0) {
			return i;
		}

		pCurrNode = pCurrNode->pNext;
	}

	return -1;
}

void CSVReader::printContent() {
	LLNode* pCurrNode = m_llData.pHead;

	int i = 0;

	while (pCurrNode != NULL) {
		char* currStr = (char*)pCurrNode->pData;


		if (i % (m_iNbFields) == m_iNbFields - 1) {
			printf("%s", currStr);
		}
		else {
			printf("%s, ", currStr);
		}

		i++;

		if (i > 0 && i % m_iNbFields == 0) {
			printf("\n");
		}

		pCurrNode = pCurrNode->pNext;
	}
}

int CSVReader::countDataRows() {
	return m_iNbDataRows;
}

int CSVReader::countFields() {
	return m_iNbFields;
}

void CSVReader::fillArrayWithData(char*** pArrayToFill) {
	LLNode* pCurrNode = getNodeInList(&m_llData, m_iNbFields) ;

	int i = 0;

	while (pCurrNode != NULL) {
		strcpy((*pArrayToFill)[i], (char*) pCurrNode->pData);

		pCurrNode = pCurrNode->pNext;
		i++;
	}
}

void CSVReader::fillArrayWithDataPtr(char*** pArrayToFill) {
	LLNode* pCurrNode = getNodeInList(&m_llData, m_iNbFields);

	int i = 0;

	while (pCurrNode != NULL) {
		(*pArrayToFill)[i] = ((char*) pCurrNode->pData);

		pCurrNode = pCurrNode->pNext;
		i++;
	}
}

void CSVReader::spliceString(char* szSrcString, int iStartPos, int iEndPos, char* szDstString) {
	for (int i = iStartPos; i <= iEndPos; i++) {
		szDstString[i - iStartPos] = szSrcString[i];
	}

	szDstString[iEndPos - iStartPos + 1] = '\0';
}

void CSVReader::convertReturnCharacters(char* szSrcString) {
	char c;
	int i = 0;
	int iTmp = 0;

	char* tempConvertedStr = new char[strlen(szSrcString)+1];

	while ((c = szSrcString[i]) != '\0') {
		if (c == '\\' && (szSrcString[i + 1] == 'n' || szSrcString[i + 1] == 'N')) {
			tempConvertedStr[iTmp] = '\n';
			i += 2;
			iTmp++;
		}
		else {
			tempConvertedStr[iTmp] = c;
			i++;
			iTmp++;
		}
	}

	tempConvertedStr[iTmp] = '\0';

	strcpy(szSrcString, tempConvertedStr);

	delete[] tempConvertedStr;
}