#include "jsonreader.hpp"



JSONReader::JSONReader(const char* path)
{
	initList(&m_data);

	FILE* fp;
	if ((fp = fopen(path, "r")) == NULL) {
		printf("Can't read json file %s. Aborting.\n", path);
		exit(1);
	}

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	rewind(fp);

	JSONNode* pCurrJSONNode = new JSONNode;

	for (int i = 0; i < fileSize; i++) {
		char c;
		fread(&c, 1, 1, fp);

		bool bNodeCompleted = false;

		if (pCurrJSONNode->iStartValuePos != -1) {
			if ((c == ',' || c == '}') && pCurrJSONNode->iEndValuePos == -1) {
				pCurrJSONNode->iEndValuePos = i - 1;
				if (c == '}') {
					pCurrJSONNode->iEndBlockPos = i - 1;
				}
				bNodeCompleted = true;
				break;
			}
		}

		if (!bNodeCompleted) {
			if (c == '{') {
				pCurrJSONNode = createNode("", NULL);
				pCurrJSONNode->iStartBlockPos = i + 1;
				pCurrJSONNode->iStartKeyPos = i + 1;
			}
			else if (c == '}') {
				pCurrJSONNode->iEndBlockPos = i - 1;
				pCurrJSONNode->iEndValuePos = i - 1;
			}
			else if (c == ':') {
				pCurrJSONNode->iStartValuePos = i + 1;
				pCurrJSONNode->iEndKeyPos = i - 1;
			}
		}
	}

	int iKeySize = pCurrJSONNode->iEndKeyPos - pCurrJSONNode->iStartKeyPos-1;
	int iValueSize = pCurrJSONNode->iEndValuePos - pCurrJSONNode->iStartValuePos-1;

	char* szKey = new char[iKeySize+1];
	char* szValue = new char[iValueSize+1];

	fseek(fp, pCurrJSONNode->iStartKeyPos, SEEK_SET);
	fread(szKey, 1, iKeySize, fp);

	fseek(fp, pCurrJSONNode->iStartValuePos, SEEK_SET);
	fread(szValue, 1, iValueSize, fp);

	szKey[iKeySize] = '\0';
	szValue[iValueSize] = '\0';

	fclose(fp);
}

JSONNode* JSONReader::createNode(const char* szKey, void* pValue) {
	//LLNode* newLLNode = new LLNode();
	JSONNode* newJSONNode = new JSONNode();

	newJSONNode->szKey = new char[strlen(szKey) + 1];
	strcpy(newJSONNode->szKey, szKey);
	newJSONNode->pValue = pValue;

	//newLLNode->pData = (void*)newJSONNode;

	return newJSONNode;
}

JSONReader::~JSONReader()
{
	
}
