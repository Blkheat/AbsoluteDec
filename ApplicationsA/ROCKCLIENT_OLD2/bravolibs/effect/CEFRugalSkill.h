//...............................................................................................................
// 
//	CEFRugalSkill.h														 질주
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

	//행동변화 체크
	void UpdateWolfShout();
	void UpdateWolfCurse();
	void UpdateWolfTree();
	void UpdateWolfDestory();
	void UpdateWolfNail(); 
	void UpdateSoltarRecall();// 솔다트 소환
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
	void CheckFrmWolfCurse();// 저주의 외침
	void CheckFrmWolfTree();// 환목 주술
	void CheckFrmWolfDestory();// 파멸의 일격
	void CheckFrmWolfNail(); 
	void CheckFrmSoltarCyclon();
	void CheckFrmTempestDril();
	void CheckFrmTempestHurricane();

	void RenderWolfNail();// 부패의 손톱

	void SideEffect();
	

private:

	std::deque <D3DXVECTOR3> m_FootList;
	std::deque <SAImageData> m_AImageList;
	float m_Scale; 
	float m_Alpha;
	DWORD m_OldTime;
	DWORD m_NowScaleTime;
	
	D3DXCOLOR		m_SideEffectColor;
	CBillboard		 m_LinkBillboard;		///-- 링크빌보드	
	CTail			 m_SwordTail;			///-- 검기
	int				 m_nPrevFrame;			///-- 이전프레임
	bool			m_EffectRun;
};

#endif


