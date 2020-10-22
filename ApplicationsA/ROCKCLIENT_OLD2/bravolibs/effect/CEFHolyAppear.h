#ifndef __CEFHolyAppear_H
#define __CEFHolyAppear_H

#include "CEffectBase.h"
#include "CEffectModel.h"

typedef enum e_HolyAppear_State
{

	HOLYAPPEAR_START = 0 ,
	HOLYAPPEAR_RUN	     ,
	HOLYAPPEAR_WAIT      

}HOLYAPPEAR_STATE;

class CEFHolyAppear : public CEffectBase
{
public:
	 
	         CEFHolyAppear();
	virtual ~CEFHolyAppear();

	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual void	 Init(void);

public:

	static  int		 InitDevice(void);
	static  int		 DeleteDevice(void);

private:
	
	CParticleSystem					  m_ParticleLink;

	D3DXVECTOR3						 *m_pProcessList;
	D3DXVECTOR3						  m_vStart;

	DWORD							 m_OldTime;

	HOLYAPPEAR_STATE				 m_State;

private:

	void	DeleteAllMemory(void);

private:

	static CEffectModel				 m_BaseModel;
	static D3DXVECTOR3 			    *m_pVertexList;
	static UINT						 m_VertexCount;
};

#endif
