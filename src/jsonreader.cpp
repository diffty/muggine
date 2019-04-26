#include "jsonreader.hpp"



JSONReader::JSONReader(const char* path)
{
	LinkedList llNodeStack;
	initList(&llNodeStack);

	LinkedList llMapStack;
	initList(&llMapStack);

	FILE* fp;
	if ((fp = fopen(path, "rb")) == NULL) {
		printf("Can't read JSON file %s. Aborting.\n", path);
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	rewind(fp);

	JSONDictItem* pCurrJSONItem = NULL;

	JSONDict* pCurrDict = NULL;

	bool bInQuotes = false;
	
	for (int i = 0; i < fileSize; i++) {
		char c;

		fseek(fp, i, SEEK_SET);
		fread(&c, 1, 1, fp);

		if ((c == ',' || c == '}' || c == ']')) {
			pCurrJSONItem->iEndValuePos = i - 1;

			if (c == '}') {
				pCurrJSONItem->iEndBlockPos = i - 1;

				if (pCurrJSONItem->isValid()) {
					this->fillNode(pCurrJSONItem, fp);
					this->parseNode(pCurrJSONItem);

					pCurrDict->m_pContentDict[std::string(pCurrJSONItem->szKey)] = pCurrJSONItem;
				}

				pCurrJSONItem = pCurrDict->m_pParentItem;

				if (pCurrJSONItem) {
					pCurrJSONItem->pValue = (void*)pCurrDict;
					if (c == ']') {
						pCurrJSONItem->eType = EContentType_LIST;
					}
					else {
						pCurrJSONItem->eType = EContentType_DICT;
					}
				}

				pCurrDict = pCurrDict->m_pParentDict;
			}
			else {
				this->fillNode(pCurrJSONItem, fp);
				this->parseNode(pCurrJSONItem);
				pCurrDict->m_pContentDict[std::string(pCurrJSONItem->szKey)] = pCurrJSONItem;

				pCurrJSONItem = createItem("", NULL);
				pCurrJSONItem->iStartBlockPos = i + 1;
				pCurrJSONItem->iStartKeyPos = i + 1;
			}
		}

		if (!bInQuotes) {
			if (c == '{') {
				JSONDict* pNewDict = new JSONDict();

				if (m_pRootDict == NULL) {
					m_pRootDict = pNewDict;
				}
				else {
					pNewDict->m_pParentDict = pCurrDict;
					pNewDict->m_pParentItem = pCurrJSONItem;
				}

				pCurrDict = pNewDict;

				pCurrJSONItem = createItem("", NULL);
				pCurrJSONItem->iStartBlockPos = i + 1;
				pCurrJSONItem->iStartKeyPos = i + 1;
			}
			else if (c == ':') {
				pCurrJSONItem->iStartValuePos = i + 1;
				pCurrJSONItem->iEndKeyPos = i - 1;
			}
		}

		if (c == '"') {
			bInQuotes = !bInQuotes;
		}
	}

	if (pCurrJSONItem && pCurrJSONItem->iStartValuePos == -1) {
		delete pCurrJSONItem;
	}

	fclose(fp);
}

void JSONReader::fillNode(JSONDictItem* pCurrJSONNode, FILE* fp) {
	int iKeySize = pCurrJSONNode->iEndKeyPos - pCurrJSONNode->iStartKeyPos + 1;
	int iValueSize = pCurrJSONNode->iEndValuePos - pCurrJSONNode->iStartValuePos + 1;

	pCurrJSONNode->szKey = new char[iKeySize + 1];
	pCurrJSONNode->szValue = new char[iValueSize + 1];

	fseek(fp, pCurrJSONNode->iStartKeyPos, SEEK_SET);
	fread(pCurrJSONNode->szKey, 1, iKeySize, fp);

	fseek(fp, pCurrJSONNode->iStartValuePos, SEEK_SET);
	fread(pCurrJSONNode->szValue, 1, iValueSize, fp);

	pCurrJSONNode->szKey[iKeySize] = '\0';
	pCurrJSONNode->szValue[iValueSize] = '\0';
}

void JSONReader::parseNode(JSONDictItem* pJsonNode) {
	char* resultKey = NULL;
	char* resultValue = NULL;

	this->trunctStr(pJsonNode->szKey, &resultKey);
	this->trunctStr(pJsonNode->szValue, &resultValue);

	int iKeySize = strlen(resultKey);

	if (resultKey[0] == '"' && resultKey[iKeySize - 1] == '"') {
		delete pJsonNode->szKey;
		pJsonNode->szKey = new char[iKeySize - 2];
		spliceString(resultKey, 1, iKeySize - 2, pJsonNode->szKey);
	}
	else {
		printf("<!!> Something bad happened in parseNode: key must be a string.\n");
		printf("\tCurrent thing parsed : %s\n", pJsonNode->szKey);
		//exit(1);
	}

	int iValueSize = strlen(resultValue);
	
	if (resultValue[0] == '"' && resultValue[iValueSize - 1] == '"') {
		char* strValueContent = new char[iValueSize - 2];
		spliceString(resultValue, 1, iValueSize - 2, strValueContent);
		pJsonNode->pValue = (void*) strValueContent;
		pJsonNode->eType = EContentType_STR;
	}
	else if (resultValue[0] == '{' && resultValue[iValueSize - 1] == '}') {
		pJsonNode->eType = EContentType_DICT;
	}
	else if (resultValue[0] == '[' && resultValue[iValueSize - 1] == ']') {
		pJsonNode->eType = EContentType_LIST;
	}
	else if (pJsonNode->eType == EContentType_UNDEFINED) {
		int* intValueContent = new int;
		*intValueContent = intFromStr(resultValue);
		pJsonNode->pValue = (void*)intValueContent;
		pJsonNode->eType = EContentType_INT;
	}

	delete resultKey;
	delete resultValue;
}

void JSONReader::trunctStr(char* str, char** result) {
	int keyStartIdx = -1;
	int keyEndIdx = -1;

	int iStrLen = strlen(str);
	int reversedi;

	for (int i = 0; i < iStrLen; i++) {
		reversedi = iStrLen - i - 1;

		if (   str[i] != ' '
			&& str[i] != '\n'
			&& str[i] != '\r'
			&& str[i] != '\t'
			&& keyStartIdx == -1) {

			keyStartIdx = i;
			if (keyStartIdx != -1 && keyEndIdx != -1) {
				break;
			}
		}

		if (   str[reversedi] != ' '
			&& str[reversedi] != '\n'
			&& str[reversedi] != '\r'
			&& str[reversedi] != '\t'
			&& keyEndIdx == -1) {

			keyEndIdx = reversedi;
			if (keyStartIdx != -1 && keyEndIdx != -1) {
				break;
			}
		}
	}

	int iParsedSize = keyEndIdx - keyStartIdx + 1;

	if (*result != NULL) {
		delete *result;
	}
	*result = new char[iParsedSize + 1];

	memcpy(*result, str + keyStartIdx, iParsedSize);
	(*result)[iParsedSize] = '\0';
}

JSONDictItem* JSONReader::createItem(const char* szKey, void* pValue) {
	JSONDictItem* newJSONNode = new JSONDictItem();

	newJSONNode->szKey = new char[strlen(szKey) + 1];
	strcpy(newJSONNode->szKey, szKey);
	newJSONNode->pValue = pValue;

	return newJSONNode;
}

JSONReader::~JSONReader()
{
	
}
