/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#ifndef XSTRING_H
#define XSTRING_H

/**
	@file XdevLXstring.h
	@brief This file contains string manipulation function.

	This functions extends the STL. It provides some additional functions to handle
	strings in a easier way.
*/

#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <cstring>
#include <cstdlib>


#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#ifdef __GNUC__
#ifdef _WIN32
// This is a MinGw 3.4.5 fix because it doesn't support wstringstream
namespace std {
typedef basic_stringstream<wchar_t> wstringstream;
}
#endif
#endif


namespace xstd {

/**
	Special characters [language(ascii - unicode)]:
	deutsch(üöä)/türkisch(ç - ğiış)/spanisch(áéíñóú)
	französisch(àâæèêëîïôùûÿ - œ)/portugisisch(ãõ)
	norwegisch(øå)/estnisch( - čšž)/isländisch(ðÞ)
	italienisch(ìò)/katalanische( - ŀ)/polnisch( - ąćęłńśźż)
	rumänisch( - ăţ)/litauisch( - ėįūų)/maltekische( - ċġħ)
	kroatische( - đ)/ungarische( - őű)/tschechisch(ý - ďěňřťů)
	slowakisch( - ĺľŕ)/lettisch( - āēģīķļņōŗ)

	kyrilisch: А 1040 - а 1072
 */


static std::string _theLowerSpecialCharacters		=  "üöäçáéíñóúàâæèêëîïôùûÿãõøåðÞìòý";
static std::string _theUpperSpecialCharacters		=  "ÜÖÄÇÁÉÍÑÓÚÀÂÆÈÊËÎÏÔÙÛŸÃÕØÅÐþÌÒÝ";

static std::wstring _theLowerSpecialCharactersW = L"üöäçğiışáéíñóúàâæèêëîïôùûÿœãõøåčšžðÞìòŀąćęłńśźżăţėįūųċġħđőűďěňřťůýĺľŕāēģīķļņōŗ";
static std::wstring _theUpperSpecialCharactersW = L"ÜÖÄÇĞİIŞÁÉÍÑÓÚÀÂÆÈÊËÎÏÔÙÛŸŒÃÕØÅČŠŽÐþÌÒĿĄĆĘŁŃŚŹŻĂŢĖĮŪŲĊĠĦĐŐŰĎĚŇŘŤŮÝĹĽŔĀĒĢĪĶĻŅŌŖ";


/**
	@fn template<typename T> inline std::string& to_string(const T& value, std::string& res) 
	@brief Converts a value into a string.
	@param value Value which will be converted into a std::string.
	@param res A valid reference to a string object which will hold the solution.
	@return The converted value as an std::string.

	Here an example how to use it:

	@code
	int v = 23;
	std::string value;

	// Convert to value into string.
	to_string(v, value);

	// As you can see the return value is the converted string itself so you can use
	// the << operator on it.
	std::cout << to_string(v, value) << std::endl;

	@endcode

*/
template<typename T>
inline std::string& to_string(const T& value, std::string& res) {
	std::stringstream out;
	out << value;
	res = out.str();
	return res;
}

/**
	@fn template<> inline std::string& to_string(const std::string& value, std::string& res)
	@brief Template specialization for to_string for that case a string is declared.
	@param value The std::string value that is converted into a std::string :) .
	@param res A valid reference to a string object.	
	@return The converted value as an std::string.
*/
template<>
inline std::string& to_string(const std::string& value, std::string& res) {
	res = value;
	return res;
}


/**
	@fn template<typename T> inline std::wstring& to_string(const T& value, std::wstring& res)
	@brief Converts a value into a wide string.
	@param value Value which will be converted into a std::wstring.
	@param res A valid reference to a wstring object which will hold the solution.	
	@return The converted value as an std::wstring.

	Here an example how to use it:

	@code
	int v = 23;
	std::wstring value;

	// Convert to value into string.
	to_string(v, value);

	// As you can see the return value is the converted string itself so you can use
	// the << operator on it.
	std::wcout << to_string(v, value) << std::endl;

	@endcode

*/
template<typename T>
inline std::wstring& to_string(const T& value, std::wstring& res) {
	std::wstringstream out;
	out << value;
	res = out.str( );
	return res;
}

/**
	@fn template<> inline std::wstring& to_string(const std::wstring& value, std::wstring& res)
	@brief Template specialization for to_string for that case a string is declared.
	@param value The std::wstring value that is converted into a std::wstring :).
	@param res A valid reference to a wstring object.
	@return The converted value as an std::wstring.
*/
template<>
inline std::wstring& to_string(const std::wstring& value, std::wstring& res) {
	res = value;
	return res;
}

/**
	@fn template<typename T> inline T from_string(const std::string& str)
	@brief Converts a string into a value.
	@param str The std::string that is converted into a value.
	@return The converted value.

	Here an example how to use it:

	@code

	std::string value("34.3f");

	// Convert to string into a value.
	int v = from_string<int>(value);

	@endcode

*/
template<typename T>
inline T from_string(const std::string& str) {
	std::stringstream streamIn(str);
	T ret = T();
	streamIn >> ret;
	return ret;
}

/**
	@fn template<typename T> inline T from_string(const std::wstring& str)
	@brief Converts a wide string into a value.
	@param str The std::wstring that is converted into a value.
	@return The converted value.

	Here an example how to use it:

	@code

	std::wstring value("34.3f");

	// Convert to string into a value.
	int v = from_string<int>(value);

	@endcode

*/
template<typename T>
inline T from_string(const std::wstring& str) {
	std::wstringstream streamIn(str);
	T ret = T();
	streamIn >> ret;
	return ret;
}

/**
	@fn emplate<typename T> inline size_t tokenize(const T& str, std::vector<T>& tokens, const T& delimiters = " ")
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
template<typename T>
inline size_t tokenize(const T& str, std::vector<T>& tokens, const T& delimiters = " ") {
	// Skip delimiters at beginning.
	typename T::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	typename T::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (T::npos != pos || T::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn template<typename T> inline size_t tokenize(const T& str, std::list<T>& tokens, const T& delimiters = " ") 
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
template<typename T>
inline size_t tokenize(const T& str, std::list<T>& tokens, const T& delimiters = " ") {
	// Skip delimiters at beginning.
	typename T::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	typename T::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (T::npos != pos || T::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn void tokenize(	const std::string& str, std::vector<std::string>& tokens, const char* delimiters = " ")
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
inline size_t tokenize(const std::string& str, std::vector<std::string>& tokens, const char* delimiters = " ") {
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn void tokenize(	const std::string& str, std::list<std::string>& tokens, const char* delimiters = " ")
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
inline size_t tokenize(const std::string& str, std::list<std::string>& tokens, const char* delimiters = " ") {
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn void tokenize(	const std::wstring& str, std::vector<std::wstring>& tokens, const wchar_t* delimiters = " ")
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
inline size_t tokenize(const std::wstring& str, std::vector<std::wstring>& tokens, const wchar_t* delimiters = L" ") {
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn void tokenize(	const std::wstring& str, std::list<std::wstring>& tokens, const wchar_t* delimiters = " ")
	@brief tokenize a string with delimiters
	@param str The source string that should split.
	@param tokens A vector of strings that will have the split tokens.
	@param delimiters The delimiters that is used to split words. Can be more than one like "\ / ,"
	@return count of tokens
*/
inline size_t tokenize(const std::wstring& str, std::list<std::wstring>& tokens, const wchar_t* delimiters = L" ") {
	// Skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	// Find first "non-delimiter".
	std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos) {
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);
		// Find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens.size();
}

/**
	@fn size_t to_narrow(const std::wstring& pStr, char* pResult)
	@brief Converts a std::wstring into a char.
	@param pStr The std::wstring that shall converted.
	@param pResult An char array. This is filled with the result.
	@remarks This function allocates new memory for pResult.
	@return It returns the number of bytes written into the multibyte output string without the terminating NULL.
*/
inline size_t to_narrow(const std::wstring& pStr, char* pResult ) {
	std::wstring::size_type len = pStr.length();
	if ( len > 0 ) {
		size_t nChars = wcstombs(NULL, (pStr).c_str(), len);
		if ( nChars > 0 ) {
			pResult = (char*)malloc(sizeof(char)*nChars);
			nChars = wcstombs(pResult, pStr.c_str(), len);

			return nChars;
		}
	}

	pResult = NULL;
	return 0;
}

/**
	@fn size_t to_narrow(const wchar_t* pStr, std::string& pResult)
	@brief Converts a wchar_t into a std::string.
	@param pStr The wchar_t that shall converted into.
	@param pResult The provided std::string is filled with the result.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const wchar_t* pStr, std::string& pResult ) {
	if (pStr) {
		size_t len = wcslen(pStr);
		if ( len > 0 ) {
			size_t nChars = wcstombs(NULL, pStr, len);
			if ( nChars > 0 ) {
				pResult.reserve( nChars );
				pResult.resize( nChars );
				nChars = wcstombs(const_cast<char*>(pResult.c_str()), pStr, len);

				return nChars;
			}
		}
	}
	pResult = "";
	return 0;
}

/**
	@fn size_t to_narrow(const std::wstring& pStr, std::string& pResult)
	@brief Converts a std::wstring into a std::string.
	@param pStr The std::wstring that shall converted.
	@param pResult The provided std::string is filled with the result.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const std::wstring& pStr, std::string& pResult ) {
	std::wstring::size_type len = pStr.length();
	if ( len > 0 ) {
		size_t nChars = wcstombs(NULL, pStr.c_str(), len);
		if ( nChars > 0 ) {
			pResult.reserve(nChars);
			pResult.resize(nChars);
			nChars = wcstombs(const_cast<char*>(pResult.c_str()), pStr.c_str(), len);

			return nChars;
		}
	}

	pResult = "";
	return 0;
}

/**
	@fn size_t to_narrow(const std::string& pStr, wchar_t* pResult)
	@brief Converts a std::string into a wchar_t.
	@param pStr The std::string that shall converted.
	@param pResult The provided wchar_t is filled with the result.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const std::string& pStr, wchar_t* pResult ) {
	std::string::size_type len = pStr.length();
	if ( len > 0 ) {
		size_t nChars = mbstowcs(NULL, pStr.c_str(), len);
		if ( nChars > 0 ) {
			pResult = (wchar_t*)malloc(sizeof(wchar_t)*nChars);
			nChars = mbstowcs(pResult, pStr.c_str(), len);

			return nChars;
		}
	}

	pResult = NULL;
	return 0;
}

/**
	@fn to_narrow( const std::string& pStr, std::wstring& pResult)
	@brief Converts a std::string into a std::wstring.
	@param pStr The std::string that shall converted.
	@param pResult The provided std::wstring is filled with the result.
	@remarks This function allocates new memory for pResult.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const std::string& pStr, std::wstring& pResult) {
	std::string::size_type len = pStr.length();
	if ( len > 0 ) {
		size_t nChars = mbstowcs(NULL, pStr.c_str(), len);
		if ( nChars > 0 ) {
			pResult.reserve(nChars);
			pResult.resize( nChars );
			nChars = mbstowcs(const_cast<wchar_t*>(pResult.c_str()), pStr.c_str(), len);

			return nChars;
		}
	}
	pResult = L"";
	return 0;
}

/**
	@fn to_narrow( const std::string& pStr, std::wstring& pResult)
	@brief Converts a std::string into a std::wstring.
	@param pStr The std::string that shall converted.
	@param pResult The provided std::wstring is filled with the result.
	@remarks This function allocates new memory for pResult.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const std::string& pStr, std::string& pResult) {
	pResult = pStr;
	return pResult.length();
}

/**
	@fn to_narrow( const std::string& pStr, std::wstring& pResult)
	@brief Converts a std::string into a std::wstring.
	@param pStr The std::string that shall converted.
	@param pResult The provided std::wstring is filled with the result.
	@remarks This function allocates new memory for pResult.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const std::wstring& pStr, std::wstring& pResult) {
	pResult = pStr;
	return pResult.length();
}

/**
	@fn size_t to_narrow(const char* pStr, std::wstring& pResult)
	@brief Converts a char into a std::wstring.
	@param pStr The char that shall converted.
	@param pResult The provided std::wstring is filled with the result.
	@return It returns the number of bytes written into the multibyte output string, excluding the terminating NULL.
*/
inline size_t to_narrow(const char* pStr, std::wstring& pResult ) {
	if (pStr) {
		size_t len = strlen(pStr);
		if ( len > 0 ) {
			size_t nChars = mbstowcs(NULL, pStr, len);
			if ( nChars > 0 ) {
				pResult.reserve( nChars );
				pResult.resize( nChars );
				nChars = mbstowcs(const_cast<wchar_t*>(pResult.c_str()), pStr, len);

				return nChars;
			}
		}
	}
	pResult = L"";
	return 0;
}

/**
	@fn int equal(const wchar_t* pStr1, const wchar_t* pStr2)
	@brief Compares a wchar_t with a wchar_t.
	@param pStr1 Null-terminated string to compare.
	@param pStr2 Null-terminated string to compare.
	@return It returns true if the strings are equal otherwise false.
*/
inline int equal(const wchar_t* pStr1, const wchar_t* pStr2) {
	return wcscmp(pStr1, pStr2);
}

/**
	@fn int equal(const wchar_t* pStr1, const char* pStr2)
	@brief Compares a wchar_t with a char.
	@param pStr1 Null-terminated string to compare.
	@param pStr2 Null-terminated string to compare.
	@return It returns true if the strings are equal otherwise false.
*/
inline int equal(const wchar_t* pStr1, const char* pStr2) {
	std::wstring narrowStr;

	to_narrow(pStr2, narrowStr);
	return wcscmp(pStr1, narrowStr.c_str());
}

/**
	@fn int equal(const char* pStr1, const wchar_t* pStr2)
	@brief Compares a char with a wchar_t.
	@param pStr1 Null-terminated string to compare.
	@param pStr2 Null-terminated string to compare.
	@return It returns true if the strings are equal otherwise false.
*/
inline int equal(const char* pStr1, const wchar_t* pStr2) {
	std::wstring narrowStr;

	to_narrow(pStr1, narrowStr);
	return wcscmp(pStr2, narrowStr.c_str());
}

/**
	@fn int equal(const char* pStr1, const char* pStr2)
	@brief Compares a char with a char.
	@param pStr1 Null-terminated string to compare.
	@param pStr2 Null-terminated string to compare.
	@return It returns true if the strings are equal otherwise false.
*/
inline int equal(const char* pStr1, const char* pStr2) {
	return strcmp(pStr1, pStr2);
}

/**
	@fn template<typename N> inline bool	is_int(const N& str)
	@brief Test str if it represents an integer value.
*/
template<typename N>
inline bool	is_int(const N& str) {
	if (str.empty())
		return false;

	typename N::const_iterator i = str.begin();
	typename N::const_iterator iLast = str.end();

	for (; i!=iLast; ++i) {
		if ( ((*i) == '-') && (i!=str.begin()))
			return false;

		if ( (((*i) < 48) || ((*i) > 57)) && ((*i) != '-'))
			return false;
	}

	return true;
}

/**
	@fn template<typename F> inline bool is_float(const F& str)
	@brief Test str if it represents an float value.
*/
template<typename F>
inline bool is_float(const F& str) {
	if (str.empty())
		return false;

	bool dot = false;
	typename F::const_iterator i = str.begin();
	typename F::const_iterator iLast = str.end();

	for (; i!=iLast; ++i) {
		if ( ((*i) == '-') && (i!=str.begin()))
			return false;

		if ((*i) == '.') {
			if (dot == false)
				dot = true;
			else
				return false;
		}

		if ( (((*i) < 48) || ((*i) > 57)) && ((*i) != '-') && ((*i) != '.')) {
			return false;
		}
	}

	return true;
}

/// Convert value of the string to a bool.
/**
	@fn template<typename B> inline bool make_bool(const B& str, const bool pDefault = false)
	@param str String string which contains the expression.
	@param pDefault If the conversion fails it will return the pDefault value.	
*/
template<typename B>
inline bool make_bool	(const B& str, const bool pDefault = false) {
	if (!str.empty()) {
		if (equal(str.c_str(), "0") == 0)
			return false;
		if (equal(str.c_str(), "1")== 0)
			return true;
		if (equal(str.c_str(), "false")== 0)
			return false;
		if (equal(str.c_str(), "true")== 0)
			return true;
	}
	return pDefault;
}

/// Trys to convert value of the string to a char.
/**
	@fn template<typename T> inline T convert(const std::string& str, const T pDefault)
	@param str String string which contains the expression.
	@param pDefault If the conversion fails it will return the pDefault value.
*/
template<typename T>
inline T		convert	(const std::string& str, const T pDefault) {
	if (!str.empty()) {
		std::stringstream s(str.c_str());
		T i;
		s >> i;
		return i;
	}
	return pDefault;
}

/// Trys to convert value of the string to a char.
template<typename T>
inline T		convert	(const std::wstring& str, const T			pDefault) {
	if (!str.empty()) {
		std::wstringstream s(str.c_str());
		T i;

		s >> i;

		return i;
	}

	return pDefault;
}

/// Erases all characters in the string that equals pChar.
template<typename E>
inline size_t erase(E& str, char pChar) {
	size_t count = 0;

	if (!str.empty()) {
		typename E::size_type br;

		while ((br = str.find(pChar)) != E::npos) {
			++count;
			str.erase(br, 1);
		}
	}

	return count;
}

/// Erases all substrings in the string that equals pChar.
template<typename E>
inline size_t erase(E& str, const std::string& pChar) {
	size_t count = 0;

	if (!str.empty()) {
		E searchFor;

		if (to_narrow(pChar, searchFor) > 0) {
			typename E::size_type br(0);

			while ((br = str.find(searchFor, br)) != std::string::npos) {
				++count;
				str.erase(br, pChar.length());
			}
		}
	}

	return count;
}

template<typename E>
inline size_t erase(E& str, const std::wstring& pChar) {
	size_t count = 0;

	if (!str.empty()) {
		E searchFor;

		if (to_narrow(pChar, searchFor) > 0) {
			typename E::size_type br(0);

			while ((br = str.find(searchFor, br)) != std::string::npos) {
				++count;
				str.erase(br, pChar.length());
			}
		}
	}

	return count;
}

/// Replaces all characters in the string that equals pSearch with pNew.
template<typename DEST, typename SEARCH, typename NEW_STR>
inline size_t replace(DEST& str, SEARCH pSearch, NEW_STR pNew) {
	size_t count(0);
	typename DEST::size_type pos(0);
	DEST search(pSearch);

	while ( (pos = str.find(pSearch, pos)) != DEST::npos) {
		str.replace( pos, search.length(), pNew );
		++count;
	}

	return count;
}

template<typename DEST>
inline size_t replace(DEST& str, char pSearch, char pNew) {
	size_t count = 0;

	if (!str.empty()) {
		typename DEST::iterator k = str.begin();
		typename DEST::iterator kLast = str.end();

		for (; k!=kLast; ++k) {
			if ((*k)==pSearch) {
				++count;
				(*k) = pNew;
			}
		}
	}

	return count;
}

inline void _upper(char& pChar) {
	if ((pChar >= 'a') && (pChar <= 'z')) {
		pChar = (char)(pChar-32);
	} else {
		size_t pos = _theLowerSpecialCharacters.find(pChar);

		if (pos != std::string::npos) {
			pChar = _theUpperSpecialCharacters.at(pos);
		}
	}
}

inline void _upper(wchar_t& pChar) {
	if ((pChar >= 'a') && (pChar <= 'z')) {
		pChar = (char)(pChar-32);
	} else
		if ((pChar >= 0x0430/*'а'*/) && (pChar <= 0x044F/*'я'*/)) {
			pChar = (char)(pChar-32);
		} else {
			size_t pos = _theLowerSpecialCharactersW.find(pChar);

			if (pos != std::string::npos) {
				pChar = _theUpperSpecialCharactersW.at(pos);
			}
		}
}

inline void _lower(char& pChar) {
	if ((pChar >= 'A') && (pChar <= 'Z')) {
		pChar = (char)(pChar+32);
	} else {
		size_t pos = _theUpperSpecialCharacters.find(pChar);

		if (pos != std::string::npos) {
			pChar = _theLowerSpecialCharacters.at(pos);
		}
	}
}

inline void _lower(wchar_t& pChar) {
	if ((pChar >= 'A') && (pChar <= 'Z')) {
		pChar = (char)(pChar+32);
	} else
		if ((pChar >= 0x0410/*'А'*/) && (pChar <= 0x044F/*'Я'*/)) {
			pChar = (char)(pChar+32);
		} else {
			size_t pos = _theUpperSpecialCharactersW.find(pChar);

			if (pos != std::string::npos) {
				pChar = _theLowerSpecialCharactersW.at(pos);
			}
		}
}

/// Makes all characters in the string Lower-case
template<typename TR>
inline TR& upper(TR& str) {
	typename TR::iterator it = str.begin();
	typename TR::iterator itLast = str.end();

	for (; it!=itLast; ++it) {
		_upper((*it));
	}
	return str;
}

/// Makes all characters in the string Upper-case
template<typename TR>
inline TR& lower(TR& str) {
	typename TR::iterator it = str.begin();
	typename TR::iterator itLast = str.end();

	for (; it!=itLast; ++it) {
		_lower((*it));
	}
	return str;
}

/// Makes all characters in the string Upper-case
template<typename TR>
inline const TR& upper(TR& str, const TR& pStringToUpper) {
	typename TR::size_type pos(0);
	TR helper = str;
	TR helperToUpper = pStringToUpper;
	typename TR::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToUpper);

	while ( (pos = helper.find(helperToUpper, pos)) != std::string::npos) {
		len = helperToUpper.length();
		for (i=0; i<len; i++) {
			_upper(str.at(pos+i));
		}
		++pos;
	}
	return pStringToUpper;
}

inline const char* upper(std::string& str, const char* pStringToUpper) {
	std::string::size_type pos(0);
	std::string helper = str;
	std::string helperToUpper = pStringToUpper;
	std::string::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToUpper);

	while ( (pos = helper.find(helperToUpper, pos)) != std::string::npos) {
		len = helperToUpper.length();
		for (i=0; i<len; i++) {
			_upper(str.at(pos+i));
		}
		++pos;
	}
	return pStringToUpper;
}

inline const wchar_t* upper(std::wstring& str, const wchar_t* pStringToUpper) {
	std::wstring::size_type pos(0);
	std::wstring helper = str;
	std::wstring helperToUpper = pStringToUpper;
	std::wstring::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToUpper);

	while ( (pos = helper.find(helperToUpper, pos)) != std::string::npos) {
		len = helperToUpper.length();
		for (i=0; i<len; i++) {
			_upper(str.at(pos+i));
		}
		++pos;
	}
	return pStringToUpper;
}

/// Makes all characters in the string Upper-case
template<typename TR>
inline TR& lower(TR& str, const TR& pStringToLower) {
	typename TR::size_type pos(0);
	TR helper = str;
	TR helperToLower = pStringToLower;
	typename TR::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToLower);

	while ( (pos = helper.find(helperToLower, pos)) != std::string::npos) {
		len = helperToLower.length();
		for (i=0; i<len; i++) {
			_lower(str.at(pos+i));
		}
		++pos;
	}
	return pStringToLower;
}

inline const char* lower(std::string& str, const char* pStringToLower) {
	std::string::size_type pos(0);
	std::string helper = str;
	std::string helperToLower = pStringToLower;
	std::string::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToLower);

	while ( (pos = helper.find(helperToLower, pos)) != std::string::npos) {
		len = helperToLower.length();
		for (i=0; i<len; i++) {
			_lower(str.at(pos+i));
		}
		++pos;
	}
	return pStringToLower;
}

inline const wchar_t* lower(std::wstring& str, const wchar_t* pStringToLower) {
	std::wstring::size_type pos(0);
	std::wstring helper = str;
	std::wstring helperToLower = pStringToLower;
	std::wstring::size_type i(0);
	size_t len;

	lower(helper);
	lower(helperToLower);

	while ( (pos = helper.find(helperToLower, pos)) != std::string::npos) {
		len = helperToLower.length();
		for (i=0; i<len; i++) {
			_lower(str.at(pos+i));
		}
		++pos;
	}

	return pStringToLower;
}

/// Trims the string from the right
template<typename TR>
inline void trim_right(TR& str) {
	if (!str.empty()) {
		size_t start = 0;
		typename TR::reverse_iterator i = str.rbegin();
		typename TR::reverse_iterator iLast = str.rend();

		do {
			if (((*i) == '\t') || ((*i) == ' ')) {
				++i;
				++start;
			} else {
				break;
			}
		} while (i!=iLast);

		if (start>0)
			str.erase(str.length()-start, start);

	}
}

/// Trims the string from the left
template<typename TL>
inline void trim_left(TL& str) {
	if (!str.empty()) {
		size_t start = 0;
		typename TL::iterator i = str.begin();
		typename TL::iterator iLast = str.end();

		do {
			if (((*i) == '\t') || ((*i) == ' ')) {
				++i;
				++start;
			} else {
				break;
			}
		} while (i!=iLast);

		if (start>0)
			str.erase(0, start);
	}
}

/// Trimes the string from left and right
template<typename T>
inline T& trim(T& str) {
	if (!str.empty()) {
		trim_right(str);
		trim_left(str);
	}
	return str;
}

/// Return true if the str is at the end of the string. Otherwise it is false
template<typename END>
inline bool is_end(const END& str, const std::string& pFind) {
	if (str.length() < pFind.length())
		return false;

	END endOfString(str.substr(str.length() - pFind.length(), pFind.length()));

	return (equal(endOfString.c_str(), pFind.c_str()) == 0);
}
template<typename END>
inline bool is_end(const END& str, const std::wstring& pFind) {
	if (str.length() < pFind.length())
		return false;

	END endOfString(str.substr(str.length() - pFind.length(), pFind.length()));

	return (equal(endOfString.c_str(), pFind.c_str()) == 0);
}

/// Return true if the str is at the begining of the string. Otherwise it is false
template<typename BEGIN>
inline bool is_begin(const BEGIN& str, const std::string& pFind) {
	if (str.length() < pFind.length())
		return false;

	BEGIN beginofstring(str.substr(0,pFind.length()));

	return (equal(beginofstring.c_str(), pFind.c_str()) == 0);
}

template<typename BEGIN>
inline bool is_begin(const BEGIN& str, const std::wstring& pFind) {
	if (str.length() < pFind.length())
		return false;

	BEGIN beginofstring(str.substr(0,pFind.length()));

	return (equal(beginofstring.c_str(), pFind.c_str()) == 0);
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

} // End of namespace xstd


/**
	Because of compatibility of older versions, we need the following functions.
	They all map to the above functions and nothing more...
 */


#ifdef XSTRING_VERSION
#if XSTRING_VERSION > 2
#ifndef XSTRING_DO_NOT_USE_COMPATIBILITY_MODE
#define XSTRING_DO_NOT_USE_COMPATIBILITY_MODE 1
#endif
#endif
#endif

#ifndef XSTRING_DO_NOT_USE_COMPATIBILITY_MODE
inline size_t ToNarrow(const std::wstring& pStr, char* pResult ) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const wchar_t* pStr, std::string& pResult ) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const std::wstring& pStr, std::string& pResult ) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const std::string& pStr, wchar_t* pResult ) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const std::string& pStr, std::wstring& pResult) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const std::string& pStr, std::string& pResult) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const std::wstring& pStr, std::wstring& pResult) {
	return xstd::to_narrow(pStr, pResult);
}

inline size_t ToNarrow(const char* pStr, std::wstring& pResult ) {
	return xstd::to_narrow(pStr, pResult);
}

inline int Equal(const wchar_t* pStr1, const wchar_t* pStr2) {
	return xstd::equal(pStr1, pStr2);
}

inline int Equal(const wchar_t* pStr1, const char* pStr2) {
	return xstd::equal(pStr1, pStr2);
}

inline int Equal(const char* pStr1, const wchar_t* pStr2) {
	return xstd::equal(pStr1, pStr2);
}

inline int Equal(const char* pStr1, const char* pStr2) {
	return xstd::equal(pStr1, pStr2);
}

template<typename F> inline bool IsFloat(const F& str) {
	return xstd::is_float(str);
}

template<typename B> inline bool MakeBool(const B& str, const bool pDefault = false) {
	return xstd::make_bool(str, pDefault);
}

template<typename T> inline T	Convert(const std::string& str, const T	pDefault) {
	return xstd::convert(str, pDefault);
}

template<typename T> inline T	Convert	(const std::wstring& str, const T	pDefault) {
	return xstd::convert(str, pDefault);
}

template<typename E> inline size_t Erase(E& str, char pChar) {
	return xstd::erase(str, pChar);
}

template<typename E> inline size_t Erase(E& str, const std::string& pChar) {
	return xstd::erase(str, pChar);
}

template<typename E> inline size_t Erase(E& str, const std::wstring& pChar) {
	return xstd::erase(str, pChar);
}

template<typename DEST> inline size_t ReplaceChar(DEST& str, char pSearch, char pNew) {
	return xstd::replace(str, pSearch, pNew);
}

template<typename DEST, typename SEARCH, typename NEW_STR> inline size_t Replace(DEST& str, SEARCH pSearch, NEW_STR pNew) {
	return xstd::replace(str, pSearch, pNew);
}

template<typename TR>	inline void MakeUpper(TR& str) {
	xstd::upper(str);
}

template<typename TR> inline void MakeUpper(TR& str, const TR& pStringToUpper) {
	xstd::upper(str, pStringToUpper);
}

template<typename TR> inline void MakeLower(TR& str) {
	xstd::lower(str);
}

template<typename TR> inline void MakeLower(TR& str, const TR& pStringToLower) {
	xstd::lower(str, pStringToLower);
}

template<typename TR> inline void TrimRight(TR& str) {
	xstd::trim_right(str);
}

template<typename TL> inline void TrimLeft(TL& str) {
	xstd::trim_left(str);
}

template<typename TRIM> inline void Trim(TRIM& str) {
	xstd::trim(str);
}

template<typename END> inline bool IsEnd(const END& str, const std::string& pFind) {
	return xstd::is_end(str, pFind);
}

template<typename END> inline bool IsEnd(const END& str, const std::wstring& pFind) {
	return xstd::is_end(str, pFind);
}

template<typename BEGIN> inline bool IsBegin(const BEGIN& str, const std::string& pFind) {
	return xstd::is_begin(str, pFind);
}

template<typename BEGIN> inline bool IsBegin(const BEGIN& str, const std::wstring& pFind) {
	return xstd::is_begin(str, pFind);
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


#endif
