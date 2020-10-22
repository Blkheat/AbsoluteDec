//...............................................................................................................
// 
//	CEFBizEyeBeam.h														 
//
//...............................................................................................................
#ifndef __CEFBizEyeBeam_H
#define __CEFBizEyeBeam_H

#include "CEffectBase.h"
#include "CBillboardLine.h"

class CEFBizEyeBeam : public CEffectBase
{
public:
	 
	         CEFBizEyeBeam();
	virtual ~CEFBizEyeBeam();

public:

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int      SetAfterEffect(void);

	virtual void	 Init(void);

private:

	DWORD	    m_RangeTime0;
	DWORD	    m_RangeTime1;
	DWORD		m_OldTimer;

	D3DXVECTOR3 m_vStartPos;
	D3DXVECTOR3 m_vDestPos;
	D3DXVECTOR3 m_vPos[ 2 ];

	CBillboardLine m_BillLine;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 m_pTexture;
#else
	LPDIRECT3DTEXTURE8 m_pTexture;
#endif // DIRECT_VERSION_9_MJH
};

#endif


