//...............................................................................................................
// 
//	CEFRockShield.h														 락 실드
//
//...............................................................................................................
#ifndef __CEFROCKSHIELD_H
#define __CEFROCKSHIELD_H

#include "CEffectBase.h"

class CEFRockShield : public CEffectBase
{
public:
	 
	         CEFRockShield();
	virtual ~CEFRockShield();

	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);
	
	virtual void	 Init(void);

private:

	int							m_Type;
	int							m_AddRange[ 3 ];			// 변위
	int							m_Angle[ 3 ];				// 각
	D3DXMATRIX					m_matWorld[ 3 ];			// 위치
	float						m_fHeight[ 3 ];				// 높이 값

	DWORD						m_OldTime;

public:

#ifdef DIRECT_VERSION_9_MJH
	static LPDIRECT3DTEXTURE9	m_pTextureList[ 3 ];
#else
	static LPDIRECT3DTEXTURE8	m_pTextureList[ 3 ];
#endif // DIRECT_VERSION_9_MJH
	static Model			   *m_pModelList[ 3 ];
	
	BOOL						m_IsRender;	 //하드코딩 정말 싫어 .. 

};

#endif


