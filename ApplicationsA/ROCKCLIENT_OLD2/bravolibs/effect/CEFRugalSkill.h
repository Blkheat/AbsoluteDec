//...............................................................................................................
// 
//	CEFRugalSkill.h														 ����
//
//...............................................................................................................
#ifndef __CEFRugalSkill_H
#define __CEFRugalSkill_H

#include "CEffectBase.h"
#include "Billboard.h"

class CEFRugalSkill : public CEffectBase
{
public:
	 
    CEFRugalSkill();
	virtual ~CEFRugalSkill();


public:
	virtual int      Update(float dtime);
	virtual int		 CheckFrame(void);
	virtual int		 Render(float dtime);							
	virtual int      SetAfterEffect(void);
	virtual void	 Init(void);


public:
	void InitWolfCurse();

	//�ൿ��ȭ üũ
	void UpdateWolfShout();
	void UpdateWolfCurse();
	void UpdateWolfTree();
	void UpdateWolfDestory();
	void UpdateWolfNail(); 
	void UpdateSoltarRecall();// �ִ�Ʈ ��ȯ
	void UpdateSoltarCyclon();
	void UpdateTempestDril();
	void UpdateTempestHurricane();

	
	void SetAfterEffetWolfShout();
	void SetAfterEffetCurse();
	void SetAfterEffetTree();
	void SetAfterEffetDestory();
	void SetAfterEffetNail(); 
	void SetAfterEffetSoltarCyclon();
	void SetAfterEffetTempestDril();
	void SetAfterEffetTempestHurricane();

	void CheckFrmWolfShout();
	void CheckFrmWolfCurse();// ������ ��ħ
	void CheckFrmWolfTree();// ȯ�� �ּ�
	void CheckFrmWolfDestory();// �ĸ��� �ϰ�
	void CheckFrmWolfNail(); 
	void CheckFrmSoltarCyclon();
	void CheckFrmTempestDril();
	void CheckFrmTempestHurricane();

	void RenderWolfNail();// ������ ����

	void SideEffect();
	

private:

	std::deque <D3DXVECTOR3> m_FootList;
	std::deque <SAImageData> m_AImageList;
	float m_Scale; 
	float m_Alpha;
	DWORD m_OldTime;
	DWORD m_NowScaleTime;
	
	D3DXCOLOR		m_SideEffectColor;
	CBillboard		 m_LinkBillboard;		///-- ��ũ������	
	CTail			 m_SwordTail;			///-- �˱�
	int				 m_nPrevFrame;			///-- ����������
	bool			m_EffectRun;
};

#endif


