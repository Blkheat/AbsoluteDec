
//...............................................................................................................
// 
//	CEFIceRain.h														 아이스 레인
//
//...............................................................................................................
#ifndef __CEFIceRain_H
#define __CEFIceRain_H

#include "CEffectBase.h"
#include "CTail.h"
#include "CDropEffect.h"
#include "Billboard.h"
#include "CBillboardLine.h"

class CEFIceRain : public CEffectBase
{
public:
	 
	         CEFIceRain();
	virtual ~CEFIceRain();

	virtual int      Update(float dtime);

	int	Render(float dtime);

public:

	virtual void	 Init(void);

	DWORD			 m_OldTime;

	CDropEffect		 m_EffectDrop;
	BOOL			 m_ISDamageProcess;

	CBillboard		 m_BillboardIce;
	
	CBillboardLine	 m_Tail;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 m_Tex;
#else
	LPDIRECT3DTEXTURE8 m_Tex;
#endif // DIRECT_VERSION_9_MJH
	
	BOOL			 m_ISDisplay;
};

#endif


