//...............................................................................................................
// 
//	CEffectModel.h														 ÀÌÆåÆ® ¸ðµ¨
//
//...............................................................................................................
#ifndef __CEffectModel_H
#define __CEffectModel_H

#include "CEffectBase.h"

class CEffectModel : public CEffectBase
{
public:
	 
			 CEffectModel();
    virtual ~CEffectModel();

	int		 InitDevice(char *ModelName, unsigned long	HashCode);
	int		 DeleteDevice(void);
	int		 Render(void);	

public:

	BOOL				m_ISModelLoad;
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTex;
#else
	LPDIRECT3DTEXTURE8	m_pTex;
#endif // DIRECT_VERSION_9_MJH
	Model			   *m_pModel;
};

#endif
