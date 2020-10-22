#ifndef __CLOGINSHUTTER_H
#define __CLOGINSHUTTER_H

#include "Render.h"

typedef enum LOGIN_SHUTTER_STATE
{
	SHUTTER_DN_STOP     ,
	SHUTTER_UP_PROCESS ,
	SHUTTER_UP_STOP    ,
	SHUTTER_DN_PROCESS  ,
};


class CLoginShutter
{
public:

	 CLoginShutter();
	~CLoginShutter();

public:

	void	Start(LOGIN_SHUTTER_STATE State,DWORD Time);
	void    Update(void);
	void    Reset(void);

private:

	DWORD	m_OldTimer;
	int		m_RangeTime;

	LOGIN_SHUTTER_STATE		m_State;
};

#endif
