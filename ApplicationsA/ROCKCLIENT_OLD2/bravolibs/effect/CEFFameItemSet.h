// EFFameItemSet.h: interface for the CEFFameItemSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFAMEITEMSET_H__3953AA4B_93C4_4EFE_94A6_5A724666A71B__INCLUDED_)
#define AFX_EFFAMEITEMSET_H__3953AA4B_93C4_4EFE_94A6_5A724666A71B__INCLUDED_


#include "CEffectBase.h"
#include <vector>


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEFFameItemSet : public CEffectBase
{
public:
	CEFFameItemSet();
	virtual ~CEFFameItemSet();

	BYTE		m_Level;
	int			m_EldaType;
	float 		m_Theta;
	D3DXCOLOR	m_ColorEnd;
	D3DXCOLOR	m_ColorStart;
	float		m_fSize;
	float		m_fSpeed;
	D3DXVECTOR3 m_vGravity;

	
	float m_NeedParticle;


	DWORD m_OldTime; 
	bool m_bSubtract; 
	bool m_bKeepUp;


	bool	m_bDownTime;
	bool	m_bUpTime;

	DWORD m_OldDownTime; 
	DWORD m_OldUpTime; 


	virtual int		 Render(float dtime);							
	virtual int      Update(float dtime);
	virtual int		 SortRender(float dtime);
	virtual void	 Init(void);

	void SetModeEffect(float fSize , float fSpeed , float fTheta , float ColorS , float ColorE);

	

private:
	
	CParticleSystem					 m_ParticleLink[ 5 ];
	CParticleSystem					 m_ParticleAnother[ 5 ];

	std::vector		<ENCHANT_POINT>	 m_LArm;
	std::vector		<ENCHANT_POINT>	m_RArm;
	std::vector		<ENCHANT_POINT>	m_Lleg;
	std::vector		<ENCHANT_POINT>	m_Rleg;

};



#endif // !defined(AFX_EFFAMEITEMSET_H__3953AA4B_93C4_4EFE_94A6_5A724666A71B__INCLUDED_)
