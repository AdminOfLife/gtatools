/*
 * CString.h
 *
 *  Created on: 04.12.2011
 *      Author: alemariusnexus
 */

#ifndef CSTRING_H_
#define CSTRING_H_

#include <boost/shared_array.hpp>
#include <cstring>
#include "strutil.h"

using boost::shared_array;



class CString {
public:
	static CString from(char* s)
			{ CString c; c.cstr = shared_array<char>(s); return c; }

public:
	CString(const char* cstr, size_t len)
	{
		if (cstr) {
			this->cstr = shared_array<char>(new char[len+1]);
			this->cstr[len] = '\0';
			strncpy(this->cstr.get(), cstr, len);
		} else {
			this->cstr = shared_array<char>(NULL);
		}
	}
	CString(const char* cstr)
	{
		if (cstr) {
			this->cstr = shared_array<char>(new char[strlen(cstr)+1]);
			strcpy(this->cstr.get(), cstr);
		} else {
			this->cstr = shared_array<char>(NULL);
		}
	}
	CString(const CString& other) : cstr(other.cstr) {}
	CString() : cstr(NULL) {}
	size_t length() const { return strlen(cstr.get()); }
	CString& lower() { ensureUniqueness(); strtolower(cstr.get(), cstr.get()); return *this; }
	CString& upper() { ensureUniqueness(); strtoupper(cstr.get(), cstr.get()); return *this; }
	const char* get() const { return cstr.get(); }

	bool operator<(const CString& other) const { return strcmp(cstr.get(), other.cstr.get()) < 0; }
	bool operator>(const CString& other) const { return strcmp(cstr.get(), other.cstr.get()) > 0; }
	bool operator<=(const CString& other) const { return !(*this > other); }
	bool operator>=(const CString& other) const { return !(*this < other); }
	bool operator==(const CString& other) const { return strcmp(cstr.get(), other.cstr.get()) == 0; }
	bool operator!=(const CString& other) const { return !(*this == other); }
	char& operator[](size_t idx) { return cstr[idx]; }
	const char& operator[](size_t idx) const { return cstr[idx]; }

private:
	void ensureUniqueness() { if (!cstr.unique()) copy(); }
	void copy();

private:
	shared_array<char> cstr;
};

#endif /* CSTRING_H_ */