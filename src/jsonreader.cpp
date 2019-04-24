#include "jsonreader.hpp"



JSONReader::JSONReader(const char* path)
{
	LinkedList llNodeStack;
	initList(&llNodeStack);

	LinkedList llMapStack;
	initList(&llMapStack);

	FILE* fp;
	if ((fp = fopen(path, "r")) == NULL) {
		printf("Can't read JSON file %s. Aborting.\n", path);
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	rewind(fp);

	JSONNode* pCurrJSONNode = NULL;

	std::map<std::string, JSONNode*>* pmCurrContent = NULL;
	
	for (int i = 0; i < fileSize; i++) {
		char c;

		fread(&c, 1, 1, fp);

		if (pCurrJSONNode != NULL && pCurrJSONNode->iStartValuePos != -1) {
			if ((c == ',' || c == '}') && pCurrJSONNode->iEndValuePos == -1) {
				pCurrJSONNode->iEndValuePos = i - 1;
				if (c == '}') {
					pCurrJSONNode->iEndBlockPos = i - 1;
				}

				this->fillNode(pCurrJSONNode, fp);
				this->parseNode(pCurrJSONNode);

				(*pmCurrContent)[std::string(pCurrJSONNode->szKey)] = pCurrJSONNode;

				pCurrJSONNode = createNode("", NULL);
				pCurrJSONNode->iStartBlockPos = i + 2;
				pCurrJSONNode->iStartKeyPos = i + 2;
			}
		}

		if (c == '{') {
			pCurrJSONNode = createNode("", NULL);
			pCurrJSONNode->iStartBlockPos = i + 1;
			pCurrJSONNode->iStartKeyPos = i + 1;
			addDataToList(&llNodeStack, pCurrJSONNode);
			addDataToList(&llMapStack, pmCurrContent);
			pmCurrContent = new std::map<std::string, JSONNode*>;
			pCurrJSONNode->pValue = (void*)pmCurrContent;
		}
		else if (c == '}') {
			pCurrJSONNode->iEndBlockPos = i - 1;
			pCurrJSONNode->iEndValuePos = i - 1;

			LLNode* pJSONLLNode = popNodeFromList(&llNodeStack);
			LLNode* pMapLLNode = popNodeFromList(&llMapStack);

			removeNodeFromList(&llNodeStack, pJSONLLNode);
			removeNodeFromList(&llMapStack, pMapLLNode);

			pCurrJSONNode = (JSONNode*)pJSONLLNode->pData;
			pmCurrContent = (std::map<std::string, JSONNode*>*) pMapLLNode->pData;
		}
		else if (c == ':') {
			pCurrJSONNode->iStartValuePos = i + 1;
			pCurrJSONNode->iEndKeyPos = i - 1;
		}
	}

	if (pCurrJSONNode->iStartValuePos == -1) {
		delete pCurrJSONNode;
	}

	fclose(fp);

	pm_mContentDict = pmCurrContent;
}

void JSONReader::fillNode(JSONNode* pCurrJSONNode, FILE* fp) {
	int iKeySize = pCurrJSONNode->iEndKeyPos - pCurrJSONNode->iStartKeyPos + 1;
	int iValueSize = pCurrJSONNode->iEndValuePos - pCurrJSONNode->iStartValuePos + 1;

	pCurrJSONNode->szKey = new char[iKeySize + 1];
	pCurrJSONNode->szValue = new char[iValueSize + 1];

	fseek(fp, pCurrJSONNode->iStartKeyPos, SEEK_SET);
	fread(pCurrJSONNode->szKey, 1, iKeySize, fp);

	fseek(fp, pCurrJSONNode->iStartValuePos + 1, SEEK_SET);
	fread(pCurrJSONNode->szValue, 1, iValueSize, fp);

	pCurrJSONNode->szKey[iKeySize] = '\0';
	pCurrJSONNode->szValue[iValueSize] = '\0';
}

void JSONReader::parseNode(JSONNode* pJsonNode) {
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
		exit(1);
	}

	int iValueSize = strlen(resultValue) - 1;
	
	if (resultValue[0] == '"' && resultKey[iValueSize - 1] == '"') {
		char* strValueContent = new char[iValueSize - 2];
		spliceString(resultValue, 1, iValueSize - 2, strValueContent);
		pJsonNode->pValue = (void*) strValueContent;
		pJsonNode->eType = EContentType_STR;
	}
	else if (resultValue[0] == '{' && resultKey[iValueSize - 1] == '}') {
		pJsonNode->eType = EContentType_DICT;
	}
	else if (resultValue[0] == '[' && resultKey[iValueSize - 1] == ']') {
		pJsonNode->eType = EContentType_LIST;
	}
	else {
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

	for (int i = 0; i < iStrLen / 2; i++) {
		reversedi = iStrLen - i - 1;

		if (str[i] != ' '
			&& str[i] != '\n'
			&& str[i] != '\r'
			&& str[i] != '\t'
			&& keyStartIdx == -1) {

			keyStartIdx = i;
			if (keyStartIdx != -1 && keyEndIdx != -1) {
				break;
			}
		}

		if (str[reversedi] != ' '
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

JSONNode* JSONReader::createNode(const char* szKey, void* pValue) {
	JSONNode* newJSONNode = new JSONNode();

	newJSONNode->szKey = new char[strlen(szKey) + 1];
	strcpy(newJSONNode->szKey, szKey);
	newJSONNode->pValue = pValue;

	return newJSONNode;
}

JSONReader::~JSONReader()
{
	
}
