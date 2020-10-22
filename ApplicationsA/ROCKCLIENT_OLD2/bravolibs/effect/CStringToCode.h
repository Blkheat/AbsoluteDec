#ifndef __CStringToCode_H
#define __CStringToCode_H

#include <string.h>
#include <iostream>

class CStringToCode
{
public:
	 
	 CStringToCode() {};
	~CStringToCode();
	 CStringToCode(const char *pStr);
	 
public:

	 char          *GetString(void)				{ return m_pString;  }
	 unsigned long  GetHashCode(void)			{ return m_HashCode; }
	 
	 int			SetString(const char *pStr);

	 CStringToCode &operator=(const CStringToCode &HashString);

	 bool operator<(const CStringToCode &HashString) const;

private:

	unsigned long  MakeHashCode(const char *pStr);

public:

	char			*m_pString;
	unsigned long	 m_HashCode;
	

};

#endif