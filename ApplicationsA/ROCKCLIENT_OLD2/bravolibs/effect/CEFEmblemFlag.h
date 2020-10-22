#ifndef __CEFEmblemFlag_H
#define __CEFEmblemFlag_H

#include "CEffectBase.h"
#include "..\\obj\\EffItemManager.h"
#include "CVAniModel.h"

class CEFEmblemFlag : public CEffectBase
{
public:
	
	         CEFEmblemFlag();
	virtual ~CEFEmblemFlag();

public:

	virtual void		Init(void);
	virtual int			CheckFrame(void);
	virtual int			Render(float dtime);
	virtual int			Update(float dtime);

	static  int			InitDevice(void);
	static  int			DeleteDevice(void);
	
	int					RenderShadow(void);
	void				ApplyShaowProj();
	
private:

	SEffItemInfo	    m_EffItemInfo;
	DWORD				m_dTexCode;
	Model			    *m_pModel;
	Model			    *m_pMoveModel;
	CVAniModel			m_VAniModel;
	CVAniModel			m_VAniMoveModel;

	bool				m_bEmblemFlag;

};

#endif

