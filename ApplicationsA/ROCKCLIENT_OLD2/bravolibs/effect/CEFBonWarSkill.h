//................................................................................................................
//
//  ÇØ°ñ Àü»ç¿ë ½ºÅ³
//
//................................................................................................................
#ifndef __CEFBONWARSKILL_H
#define __CEFBONWARSKILL_H

#include "CEffectBase.h"
#include "CTail.h"
#include "Billboard.h"

class CEFBonWarSkill : public CEffectBase
{
public:
	 
	         CEFBonWarSkill();
	virtual ~CEFBonWarSkill();

public:

	virtual void	 Init(void);
	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);
	virtual int		 SetAfterEffect();
	virtual HRESULT	 InvalidateDeviceObject();
#ifdef DIRECT_VERSION_9_MJH
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE9 pDevice);
#else
	virtual HRESULT	 RestoreDeviceObject (LPDIRECT3DDEVICE8 pDevice);
#endif // DIRECT_VERSION_9_MJH

	void			 InitBonSTrust();		///-- Âî¸£±â
	void			 InitBonSWCut();		///-- ±Þ¼Òº£±â
	void			 InitBonSTCut();		///-- Âñ·¯º£±â
	void			 InitBonSTurnCut();		///-- È¸Àüº£±â
	void			 InitBonWX1Cut();		///-- Xº£±â1	
	void			 InitBonWX2Cut();		///-- Xº£±â2
	void			 InitBonBFStorm();		///-- È­¿°ÆøÇ³
	void			 InitBonJCraFen();		///-- ±¤ÆøÀÇ °Ë¼ú
	void			 InitSkelDStorm();		///-- ¾ÏÈæÆøÇ³		
	
	int				 CheckFrameBonSTrust();
	int				 CheckFrameBonSWCut();
	int				 CheckFrameBonSTCut();
	int				 CheckFrameBonSTurnCut();
	int				 CheckFrameBoneWX1Cut();
	int				 CheckFrameBoneWX2Cut();
	int				 CheckFrameBoneBFStorm();
	int				 CheckFrameBonJCraFen();
	int				 CheckFrameSkelDStorm();

	int				 UpdateBonSTrust(float dtime);
	int				 UpdateBonSWCut(float dtime);
	int				 UpdateBonSTCut(float dtime);
	int				 UpdateBonSTurnCut(float dtime);
	int				 UpdateBonWX1Cut(float dtime);
	int				 UpdateBonWX2Cut(float dtime);
	int				 UpdateBonBFStorm(float dtime);
	int				 UpdateBonJCraFen(float dtime);
	int				 UpdateSkelDStorm(float dtime);

	int				 RenderBonSTrust(float dtime);
	int				 RenderBonSWCut(float dtime);
	int				 RenderBonSTCut(float dtime);
	int				 RenderBonSTurnCut(float dtime);
	int				 RenderBonWX1Cut(float dtime);
	int				 RenderBonWX2Cut(float dtime);
	int				 RenderBonBFStorm(float dtime);
	int				 RenderBonJCraFen(float dtime);
	int				 RenderSkelDStorm(float dtime);

	int				 SetAEffectBonSTrust();
	int				 SetAEffectBonSWCut();
	int				 SetAEffectBonSTCut();
	int				 SetAEffectBonSTurnCut();
	int				 SetAEffectBonWX1Cut();
	int				 SetAEffectBonWX2Cut();
	int				 SetAEffectBonBFStorm();
	int				 SetAEffectBonJCraFen();
	int				 SetAEffectSkelDStorm();

	CBillboard		 m_LinkBillboard;		///-- ¸µÅ©ºôº¸µå	
	CTail			 m_SwordTail;			///-- °Ë±â

	int				 m_nPrevFrame;			///-- ÀÌÀüÇÁ·¹ÀÓ
	
	BOOL			 m_ISDamageProcess;		///-- µ¥¹ÌÁö Ã³¸®¿©ºÎ
};

#endif