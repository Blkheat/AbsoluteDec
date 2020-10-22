//...............................................................................................................
// 
//	CEFFootStrength.h														 마인의 각력
//
//...............................................................................................................
#ifndef __CEFFootStrength_H
#define __CEFFootStrength_H

#include "CEffectBase.h"
#include "CTail.h"

class CEFFootStrength : public CEffectBase
{
public:
	 
	         CEFFootStrength();
	virtual ~CEFFootStrength();

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
};

#endif


