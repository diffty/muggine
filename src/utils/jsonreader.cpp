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

	JSONItemKeyValue* pCurrJSONItem = NULL;

	JSONItem* pCurrDict = NULL;

	bool bInQuotes = false;
    bool bItemJustLoaded = false;
    
	EContentType eInContent = EContentType_UNDEFINED;
	
	for (int i = 0; i < fileSize; i++) {
		char c;

		fseek(fp, i, SEEK_SET);
		fread(&c, 1, 1, fp);

		if (c == ',' || c == '}' || c == ']') {
			pCurrJSONItem->iEndValuePos = i - 1;
			pCurrJSONItem->iEndBlockPos = i - 1;

			if (c == '}' || c == ']') {
                if (pCurrJSONItem->iStartBlockPos < pCurrJSONItem->iEndBlockPos) {
                    if (pCurrJSONItem->isValid()) {
                        this->fillNode(pCurrJSONItem, fp);
						if (this->parseNode(pCurrJSONItem)) {
							if (pCurrDict->m_eNodeType == EContentType_DICT) {
								pCurrDict->m_contentDict[std::string(pCurrJSONItem->szKey)] = pCurrJSONItem;
							}
							else if (pCurrDict->m_eNodeType == EContentType_LIST) {
								pCurrDict->m_contentList.push_back(pCurrJSONItem);
							}
						}
                    }
                }

				pCurrJSONItem = pCurrDict->m_pParentItem;

				if (pCurrJSONItem) {
					pCurrJSONItem->pValue = (void*)pCurrDict;
				}

				pCurrDict = pCurrDict->m_pParentDict;
			}
			else {
				this->fillNode(pCurrJSONItem, fp);
				if (this->parseNode(pCurrJSONItem)) {
					if (pCurrDict->m_eNodeType == EContentType_DICT) {
						pCurrDict->m_contentDict[std::string(pCurrJSONItem->szKey)] = pCurrJSONItem;
					}
					else if (pCurrDict->m_eNodeType == EContentType_LIST) {
						pCurrDict->m_contentList.push_back(pCurrJSONItem);
					}
				}

				pCurrJSONItem = createItem("", NULL);
				pCurrJSONItem->iStartBlockPos = i + 1;
				pCurrJSONItem->iStartKeyPos = i + 1;
				pCurrJSONItem->m_pParent = pCurrDict;
			}
		}

		if (!bInQuotes) {
			if (c == '{' || c == '[') {
				JSONItem* pNewDict = new JSONItem();

				if (m_pRootDict == NULL) {
					m_pRootDict = pNewDict;
				}

                pNewDict->m_pParentDict = pCurrDict;
                pNewDict->m_pParentItem = pCurrJSONItem;
                
				pCurrDict = pNewDict;

				pCurrJSONItem = createItem("", NULL);
				pCurrJSONItem->iStartBlockPos = i + 1;
				pCurrJSONItem->iStartKeyPos = i + 1;
				pCurrJSONItem->m_pParent = pCurrDict;
                
                if (m_pRootItem == NULL) {
                    m_pRootItem = pCurrJSONItem;
                }

				if (c == '{') {
					pCurrDict->m_eNodeType = EContentType_DICT;
				}
				else if (c == '[') {
					pCurrDict->m_eNodeType = EContentType_LIST;
				}
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

void JSONReader::fillNode(JSONItemKeyValue* pCurrJSONNode, FILE* fp) {
	// It's a list!
	if (pCurrJSONNode->iStartKeyPos != -1
		&& pCurrJSONNode->iStartValuePos == -1
		&& pCurrJSONNode->iEndKeyPos == -1
		&& pCurrJSONNode->iEndValuePos != -1) {

		pCurrJSONNode->iStartValuePos = pCurrJSONNode->iStartKeyPos;
	}
	else { // It's a dict!
		int iKeySize = pCurrJSONNode->iEndKeyPos - pCurrJSONNode->iStartKeyPos + 1;
		pCurrJSONNode->szKey = new char[iKeySize + 1];

		fseek(fp, pCurrJSONNode->iStartKeyPos, SEEK_SET);
		fread(pCurrJSONNode->szKey, 1, iKeySize, fp);

		pCurrJSONNode->szKey[iKeySize] = '\0';
	}

	int iValueSize = pCurrJSONNode->iEndValuePos - pCurrJSONNode->iStartValuePos + 1;

	pCurrJSONNode->szValue = new char[iValueSize + 1];

	fseek(fp, pCurrJSONNode->iStartValuePos, SEEK_SET);
	fread(pCurrJSONNode->szValue, 1, iValueSize, fp);
	pCurrJSONNode->szValue[iValueSize] = '\0';
}

bool JSONReader::parseNode(JSONItemKeyValue* pJsonNode) {
	char* resultKey = NULL;
	char* resultValue = NULL;

	if (pJsonNode->m_pParent->m_eNodeType == EContentType_DICT) {
		if (!this->trunctStr(pJsonNode->szKey, &resultKey)) {
			pJsonNode->szKey = NULL;
		}
		else {
			int iKeySize = strlen(resultKey);

			if (resultKey[0] == '"' && resultKey[iKeySize - 1] == '"') {
				delete pJsonNode->szKey;
				pJsonNode->szKey = new char[iKeySize - 2 + 1];
				spliceString(resultKey, 1, iKeySize - 2, pJsonNode->szKey);
				delete resultKey;
			}
			else {
				printf("<!!> Something bad happened in parseNode: key must be a string.\n");
				printf("Current thing parsed : %s\n", pJsonNode->szKey);
			}
		}
	}

	if (!this->trunctStr(pJsonNode->szValue, &resultValue)) {
		pJsonNode->szValue = NULL;
		return false;
	}

	int iValueSize = strlen(resultValue);
	
	if (resultValue[0] == '"' && resultValue[iValueSize - 1] == '"') {
		char* strValueContent = new char[iValueSize - 2 + 1];
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
		if (strlen(resultValue) > 0) {
			int* intValueContent = new int;
			*intValueContent = atoi(resultValue);
			pJsonNode->pValue = (void*)intValueContent;
			pJsonNode->eType = EContentType_INT;
		}
	}

	delete resultValue;

	return pJsonNode->eType != EContentType_UNDEFINED;
}

bool JSONReader::trunctStr(char* str, char** result) {
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

	if (keyEndIdx != -1 && keyStartIdx != -1) {
		int iParsedSize = keyEndIdx - keyStartIdx + 1;

		if (*result != NULL) {
			delete *result;
		}
		*result = new char[iParsedSize + 1];

		memcpy(*result, str + keyStartIdx, iParsedSize);
		(*result)[iParsedSize] = '\0';
		return true;
	}
	else {
		return false;
	}
}

JSONItemKeyValue* JSONReader::createItem(const char* szKey, void* pValue) {
	JSONItemKeyValue* newJSONNode = new JSONItemKeyValue();

	newJSONNode->szKey = new char[strlen(szKey) + 1];
	strcpy(newJSONNode->szKey, szKey);
	newJSONNode->pValue = pValue;

	return newJSONNode;
}

JSONReader::~JSONReader()
{
	
}
