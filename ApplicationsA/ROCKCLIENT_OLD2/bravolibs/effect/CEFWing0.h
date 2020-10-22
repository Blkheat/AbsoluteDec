//CEFWing0
#ifndef __CEFWING0_H
#define __CEFWING0_H

#include "CEffectBase.h"
#include "..\\obj\\EffItemManager.h"
#include "CVAniModel.h"

class CEFWing0 : public CEffectBase
{
public:
	
	         CEFWing0();
	virtual ~CEFWing0();

public:

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	
	virtual void	 SetDestroy(BOOL ISDamageAct = FALSE, BOOL ISBasicAttack = FALSE);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	int				 RenderShadow(void);
	void			 ApplyShaowProj();

	int				 m_nItemCode;
	int				 m_nLinkBone;
	
	BOOL			 m_IsHighLight;		///-- 하이라이트 작용여부
	BOOL			 m_IsSemiOpacity;	///-- 반투명 적용여부
	
private:

	SEffItemInfo	 m_EffItemInfo;
	DWORD			 m_dTexCode;			 
	Model			 *m_pModel;
	
	CVAniModel		 m_VAniModel;	
	bool			 m_bEmblemFlag;
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9		 m_Light;
#else
	D3DLIGHT8		 m_Light;
#endif // DIRECT_VERSION_9_MJH
};

#endif

