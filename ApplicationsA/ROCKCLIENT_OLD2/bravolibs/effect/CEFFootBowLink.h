//...............................................................................................................
// 
//	CEFFootBowLink.h														 화살 발에 붙는거 ( 지속 )
//
//...............................................................................................................
#ifndef __CEFFOOTBOWLINK_H
#define __CEFFOOTBOWLINK_H

#include "CEffectBase.h"

class CEFFootBowLink : public CEffectBase
{
public:
	 
	         CEFFootBowLink();
	virtual ~CEFFootBowLink();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

	void			 SetExtraInfo(float AddRad,D3DXVECTOR3 vAddPos,int LinkBone);

private:

	D3DXVECTOR3		 m_vAddPos;
	float			 m_fAddRad;
	D3DXMATRIX		 m_matWorld;
	int				 m_LinkBone;
};

#endif


