//................................................................................................................
//
//  �ذ� ����� ��ų
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

	void			 InitBonSTrust();		///-- ���
	void			 InitBonSWCut();		///-- �޼Һ���
	void			 InitBonSTCut();		///-- �񷯺���
	void			 InitBonSTurnCut();		///-- ȸ������
	void			 InitBonWX1Cut();		///-- X����1	
	void			 InitBonWX2Cut();		///-- X����2
	void			 InitBonBFStorm();		///-- ȭ����ǳ
	void			 InitBonJCraFen();		///-- ������ �˼�
	void			 InitSkelDStorm();		///-- ������ǳ		
	
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

	CBillboard		 m_LinkBillboard;		///-- ��ũ������	
	CTail			 m_SwordTail;			///-- �˱�

	int				 m_nPrevFrame;			///-- ����������
	
	BOOL			 m_ISDamageProcess;		///-- ������ ó������
};

#endif