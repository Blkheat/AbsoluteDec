#ifndef __BZONEIN2DEFFECT_H__
#define __BZONEIN2DEFFECT_H__
//-----------------------------------------------------------------------------

#include "Define.h"

enum N2DEffectType
{
	N2DEffect_None = 0,
	N2DEffect_Trans,
	N2DEffect_View,
	N2DEffect_Hide,
};

class CBZoneIn2DEffect
{
protected:
	// ���� ����� �ʿ� ����
	UINT			m_uReadyState;				//���� ���������
	UINT			m_uRedayImg;				//���� ���� ���� �̹���
	SRect			m_sReadyMoveRect;			//���� ���� �̵� ����
	SRect			m_sReadyNomalRect;			//���� ���� �Ϲ� ����
	
	SRect			m_sReadyRenderRect;			//���� ���� ���� ����
	float			m_fMovetime;				//���� �̵� Ÿ��
	float			m_fMCumtime;				//���� �̵� ����Ÿ��
	int				m_nReadyAlpha;				//���� ���İ�

	float			m_fReadyFadetime;			//���� ���̵� ����Ÿ��
	float			m_fRedayFCumtime;			//���� ���̵� ���봩��Ÿ��


	//��ŸƮ ����� �ʿ� ����
	UINT			m_uStartState;				//���� ��ŸƮ�������
	UINT			m_uStartImg;				//���� ��ŸƮ ���� �̹���
	SRect			m_sStartMiniRect;			//��ŸƮ ���� �ּ� ����
	SRect			m_sStartNormalRect;			//��ŸƮ ���� �Ϲ� ����
	
	SRect			m_sStartRenderRect;			//���� ��ŸƮ ���� ����
	SPoint2			m_sStartCenPoint;			//���� ��ŸƮ ���� �߽���
	float			m_fSizetime;				//��ŸƮ ��� Ÿ��
	float			m_fSCumtime;				//��ŸƮ ��� ����Ÿ��
	int				m_nStartAlpha;				//��ŸƮ ���İ�
	
	float			m_fStartViewtime;			//��ŸƮ �� ����Ÿ��
	float			m_fStartVCumtime;			//��ŸƮ �� ���봩��Ÿ��
	
	float			m_fStartFadetime;			//��ŸƮ ���̵� ����Ÿ��	
	float			m_fStartFCumtime;			//��ŸƮ ���̵� ���봩��Ÿ��

	bool		    m_bIsGlow;					//��½�̴� ȿ�� ���뿩��
	float			m_fGlowtime;				//�۷ο� ���� Ÿ��
	float			m_fGCumtime;				//�۷ο� ���� ����Ÿ��

public:
	CBZoneIn2DEffect();
	virtual ~CBZoneIn2DEffect();

	void InitEffect();
	void SetReadyEffect(UINT uEffectType);
	void SetStartEffect(UINT uEffectType);	
		
	void UpdateProc();
	void RenderProc();

protected:
	void InitReadyEffect();
	void InitStartEffect();
	
	void UpdateReadyProc();
	void UpdateReadyTransProc();
	void UpdateReadyViewProc();
	void UpdateReadyHideProc();
	
	void UpdateStartProc();
	void UpdateStartTransProc();
	void UpdateStartViewProc();
	void UpdateStartHideProc();

	void RenderReadyProc();
	void RenderStartProc();

};

#endif	__BZONEIN2DEFFECT_H__