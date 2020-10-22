//...............................................................................................................
// 
//	CEFArrow.h														 화살 모델 로드
//
//...............................................................................................................
#ifndef __CEFARROW_H
#define __CEFARROW_H

#include "CEffectBase.h"

class CEFArrow : public CEffectBase
{
public:
	 
			 CEFArrow() {};
    virtual ~CEFArrow() {};

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	static  int		 Render(int Num);	

public:

	static BOOL ISLoad;
	static Model			   *m_pArrowModel[3];
#ifdef DIRECT_VERSION_9_MJH
	static LPDIRECT3DTEXTURE9	m_pTexArrow;
	static LPDIRECT3DTEXTURE9   m_pTexLine;
#else
	static LPDIRECT3DTEXTURE8	m_pTexArrow;
	static LPDIRECT3DTEXTURE8   m_pTexLine;
#endif // DIRECT_VERSION_9_MJH
};

#endif
