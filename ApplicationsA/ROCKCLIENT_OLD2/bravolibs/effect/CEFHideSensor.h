//................................................................................................................
//
//  ����
//
//................................................................................................................
#ifndef __CEFHideSensor_H
#define __CEFHideSensor_H

#include "CEffectBase.h"
#include "CFloatSequence.h"

class CEFHideSensor: public CEffectBase
{
public:
	 
	         CEFHideSensor();
	virtual ~CEFHideSensor();

public:

	virtual void	 Init(void);
			int		 Update(float dtime);
	BOOL			DistanceIn(D3DXMATRIX &SrcPos ,D3DXMATRIX &DestPos);
	BOOL			MyPartyHideSensor();	//����Ƽ���� ����߳� ? 
	BOOL			MyUseHideSensor();	//���� ����߳� ? 





public:

	BOOL			m_ISFade;
	CFloatSequence	m_AlphaSequence;
	DWORD			m_OldTimer;
};


#endif