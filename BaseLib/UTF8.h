#ifndef UTF8_H
#define UTF8_H
#include <stringapiset.h>

namespace Util {
	char* ToUTF8(const char* pszIn) {
		int nLenOfUni = 0, nLenOfUTF = 0;
		wchar_t* us = NULL;
		char* pszOut = NULL;

		if ((nLenOfUni = MultiByteToWideChar(CP_ACP, 0, pszIn, strlen(pszIn), NULL, 0)) <= 0) return 0;
		us = new wchar_t[nLenOfUni + 1];
		memset(us, 0x00, sizeof(wchar_t)*(nLenOfUni + 1));

		// ansi ---> unicode
		nLenOfUni = MultiByteToWideChar(CP_ACP, 0, pszIn, strlen(pszIn), us, nLenOfUni);
		if ((nLenOfUTF = WideCharToMultiByte(CP_UTF8, 0, us, nLenOfUni, NULL, 0, NULL, NULL)) <= 0) {
			//free(us);
			delete[] us;
			return 0;
		}
		pszOut = new char[nLenOfUTF + 1];
		memset(pszOut, 0x00, sizeof(char)*(nLenOfUTF + 1));

		// unicode ---> utf8
		nLenOfUTF = WideCharToMultiByte(CP_UTF8, 0, us, nLenOfUni, pszOut, nLenOfUTF, NULL, NULL);
		pszOut[nLenOfUTF] = 0;
		delete[] us;

		return pszOut;
	}

	char* ToAnsi(const char* pszIn) {
		int nLenOfUni = 0, nLenOfANSI = 0;
		wchar_t* us = NULL;
		char* pszOut = NULL;

		if ((nLenOfUni = MultiByteToWideChar(CP_UTF8, 0, pszIn, strlen(pszIn), NULL, 0)) <= 0)
			return 0;

		us = new wchar_t[nLenOfUni + 1];
		memset(us, 0x00, sizeof(wchar_t)*(nLenOfUni + 1));

		// utf8 --> unicode
		nLenOfUni = MultiByteToWideChar(CP_UTF8, 0, pszIn, strlen(pszIn), us, nLenOfUni);
		if ((nLenOfANSI = WideCharToMultiByte(CP_ACP, 0, us, nLenOfUni, NULL, 0, NULL, NULL)) <= 0) {
			//free(us);
			delete[] us;
			return 0;
		}

		pszOut = new char[nLenOfANSI + 1];
		memset(pszOut, 0x00, sizeof(char)*(nLenOfANSI + 1));

		// unicode --> ansi
		nLenOfANSI = WideCharToMultiByte(CP_ACP, 0, us, nLenOfUni, pszOut, nLenOfANSI, NULL, NULL);
		pszOut[nLenOfANSI] = 0;
		delete[] us;

		return pszOut;
	}
}

#endif
