//...............................................................................................................
// 
//	CEFBunchStrength.h														 마인의 권술
//
//...............................................................................................................
#ifndef __CEFBunchStrength_H
#define __CEFBunchStrength_H

#include "CEffectBase.h"
#include "CTail.h"

class CEFBunchStrength : public CEffectBase
{
public:
	 
	         CEFBunchStrength();
	virtual ~CEFBunchStrength();

	virtual int      Update(float dtime);
	virtual HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice);
#else
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	int	Render(float dtime);

public:

	virtual void	 Init(void);

private:

	CTail	m_Tail;
	CTail	m_Tail2;
};

#endif