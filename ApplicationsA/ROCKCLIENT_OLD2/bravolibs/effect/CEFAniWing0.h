//CEFWing0
#ifndef __CEFANIWING0_H
#define __CEFANIWING0_H

#include "CEffectBase.h"
#include "..\\obj\\EffItemManager.h"

class CEFAniWing0 : public CEffectBase
{
public:
	
	         CEFAniWing0();
	virtual ~CEFAniWing0();

public:

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	int				 RenderShadow(void);
	void			 ApplyShaowProj();

	int				 RenderAniWing();

	int				 m_nItemCode;
	int				 m_nLinkBone;
	
	BOOL			 m_IsHighLight;		///-- 하이라이트 작용여부
	BOOL			 m_IsSemiOpacity;	///-- 반투명 적용여부
	BOOL			 m_IsMotionSync;	///-- 모션 싱크여부	
	
private:

	SEffItemInfo	 m_EffItemInfo;
	DWORD			 m_dTexCode;			 
	Model			 *m_pModel;
	Bone			 *m_pBone;

	VTX_BUFFER_INFO  m_VtxBuffer;
	
	long			 m_PreTime;

	int				 m_uFrame;
	
	int				 m_next_frame;
	int				 m_curt_frame;
	int				 m_ani_frame;
	int				 m_end_frame;
	float			 m_middle_time; 
#ifdef DIRECT_VERSION_9_MJH
	D3DLIGHT9		 m_Light;
#else
	D3DLIGHT8		 m_Light;
#endif // DIRECT_VERSION_9_MJH
};

#endif