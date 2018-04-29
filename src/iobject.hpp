#ifndef IOBJECT_HPP
#define IOBJECT_HPP

#include <stdio.h>


class ClassInfo {
private:
	const char* m_szClassTypeName;

public:
	ClassInfo() {
		m_szClassTypeName = "undefined";
	};


	void setClassTypeName(const char* szClassTypeName) {
		m_szClassTypeName = szClassTypeName;
	}

	const char* getClassTypeName() {
		return m_szClassTypeName;
	}
};


class IObject {
protected:
	ClassInfo m_classInfo;

public:
	IObject() {};
	virtual ~IObject() {};

	ClassInfo* getClassType() {
		return &m_classInfo;
	}
};


#endif