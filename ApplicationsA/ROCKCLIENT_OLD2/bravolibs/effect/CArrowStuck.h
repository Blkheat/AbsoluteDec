//.................................................................................................................
//
// CArrowStuck.h ( 화살 몸에 붙는거 )																By wxywxy
//
//.................................................................................................................
#ifndef __CARROWSTRUCK_H
#define __CARROWSTRUCK_H

#include "CEFArrow.h"

class CArrowStuck : public CEffectBase
{
public:
	
	         CArrowStuck();
	virtual ~CArrowStuck();

public:

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

	virtual void	 Init(void);
	
	void			 SetExtraInfo(float AddRad,D3DXVECTOR3 vAddPos,int LinkBone);

private:

	DWORD			 m_OldTimer;
	DWORD			 m_RangeTime;

	D3DXVECTOR3		 m_vAddPos;
	float			 m_fAddRad;
	D3DXMATRIX		 m_matWorld;
	int				 m_LinkBone;
};


#endif
