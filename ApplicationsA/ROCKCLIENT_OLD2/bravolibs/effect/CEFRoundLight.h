//...............................................................................................................
// 
//	CEFRoundLight.h														 ���� ����Ʈ
//
//...............................................................................................................
#ifndef __CEFRoundLight_H
#define __CEFRoundLight_H

#include "CEffectBase.h"
#include "Billboard.h"
#include "CBillboardLine.h"

class CEFRoundLight : public CEffectBase
{
public:
	 
	         CEFRoundLight();
	virtual ~CEFRoundLight();

public:

	virtual void	 Init(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);	

	D3DXVECTOR3		 m_vStartPos[36];
	D3DXVECTOR3		 m_vEndPos[36];	

	float			 m_fRadius;
		
	DWORD			 m_InitTime;		 //�ʱ�ȭ�ð�	
	DWORD			 m_PrevTime;		
	DWORD			 m_RunningTime;		 //��ü���۽ð�	

	BOOL			 m_ISDisplay;
	BOOL			 m_ISDamageProcess;

	CBillboard		 m_BillboardLight[36];
	CBillboardLine   m_Tail;
};

#endif