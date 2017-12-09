#include <stdlib.h>
#include "common_tools.hpp"


int intFromStr(char* szStr) {
	int i = 0;
	int iStrLen = strlen(szStr);
	int iDigitNum = 0;
	int iRes = 0;

	char c;

	while ((c = szStr[i]) != '\0') {
		iDigitNum = (iStrLen - 1) - i;
		iRes += atoi(&c) * pow(10, iDigitNum);

		i++;
	}

	return iRes;
}
