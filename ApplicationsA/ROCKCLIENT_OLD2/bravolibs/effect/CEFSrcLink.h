//................................................................................................................
//
//  �ҽ��� ����Ʈ �ٴ°� ���� ó��
//
//................................................................................................................
#ifndef __CEFSRCLINK_H
#define __CEFSRCLINK_H

#include "CEffectBase.h"

class CEFSrcLink: public CEffectBase
{
public:
	 
	         CEFSrcLink();
	virtual ~CEFSrcLink();
	
	BOOL	m_IsGiantUnit;

public:

	virtual void	 Init(void);
	int	Update(float dtime);
};


#endif