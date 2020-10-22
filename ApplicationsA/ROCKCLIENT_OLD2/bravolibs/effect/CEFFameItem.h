// EFFameItem.h: interface for the CEFFameItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFAMEITEM_H__C67B7C8B_B0F8_4D5D_9D77_48AFB15DE070__INCLUDED_)
#define AFX_EFFAMEITEM_H__C67B7C8B_B0F8_4D5D_9D77_48AFB15DE070__INCLUDED_

#include "CEffectBase.h"
#include "CTail.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEFFameItem : public CEffectBase
{
public:
	CEFFameItem();
	virtual ~CEFFameItem();

	
	virtual void Init(void);
	virtual int      Update(float dtime);
	virtual HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice);
#else
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	int	Render(float dtime);

private:


	CTail	m_Tail;	//¿À¸¥ÂÊ ²ÞÄ¡~¾î±ú 
	CTail	m_Tail2; //¿À¸¥ÂÊ ¼Õ~²ÞÄ¡ 
	CTail	m_Tail3; //¿À¸¥ÂÊ ²ÞÄ¡~¾î±ú 
	CTail	m_Tail4; //¿À¸¥ÂÊ ¼Õ~²ÞÄ¡ 

};

#endif // !defined(AFX_EFFAMEITEM_H__C67B7C8B_B0F8_4D5D_9D77_48AFB15DE070__INCLUDED_)
