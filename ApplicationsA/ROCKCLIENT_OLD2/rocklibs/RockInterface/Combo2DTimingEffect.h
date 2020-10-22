#ifndef __COMBO2DTIMINGEFFECT_H__
#define __COMBO2DTIMINGEFFECT_H__
//-----------------------------------------------------------------------------

#include <vector>
#include <list>

using namespace	std;

enum  CTCOMBOSTATE
{
	STATE_COMBO_NONE = 0,
	STATE_COMBO_SUCCESS,
	STATE_COMBO_FAIL,
	STATE_COMBO_TIMEOVER,
};

enum COMKEY
{
	COMZKEY = 0,
	COMXKEY,
	COMCKEY,
};

struct SComEffect
{
	bool  bSuccess;	
	float fCumtime;
	char  comkey;
};

class CCombo2DTimingEffect
{
public:
	long        m_lEtime;

	int			m_nComCount;				//콤보 카운트
	int			m_nRace;					//콤보적용대상 타입 
	long		m_lDestID;					//콤보적용대상 아이디

protected:
	int			m_nMotion;
	
	bool		m_IsActive;					//콤보타이밍 동작활성화 여부
	bool		m_IsRender;					//렌더링 여부	
	
	bool		m_IsTempFlag;				//이펙트 렌더링 닫을때 바로 닫지않기위한 플래그
	bool		m_IsRenderGlow;				//반짝이는 효과
	
	SRect		m_BarBgRect;				//타이밍 외곽

	SRect		m_BarComKeyRect[3];			//타이밍 콤보키 이미지
	SPoint2		m_BarComKeyCenPoint[3];		//타이밍 콤보키 중심점

	SRect		m_BarGaugeRect;				//게이지 외곽
	SRect		m_BarSBgRect;				//성공 타이밍 외곽

	D3DCOLOR	m_Color;					//칼라값
	D3DCOLOR	m_TexColor;					//내부글씨 칼라값

	float		m_fFlashTime;				//성공시-점멸 타임
	float		m_fCumulativeTime;			//누적 타임

	UINT		m_uComboState;

	float		m_fRatio;					//게이지바 비율
	bool		m_IsRenderComboKey;			//콤보키 렌더링 여부

	float		m_finterTime;				//콤보키가 나오는 템포 간격타임
	float		m_fiCumTime;				//콤보키가 나오는 템포 누적타임

	float       m_fGlowRatio;				//반짝이는 시간누적 비율
	
	bool		m_IsComboKeyReg;			//콤보키 등록여부	
	
	char		m_cComboKey;
	bool		m_bEventFlag;				//이벤트 발생플래그
	bool		m_bIsSuccess;

	int			m_nCurrentFrame;
	long		m_lElasetime;
	bool		m_bIsStart;	
	
	float		m_fComDelayTime;			//키성공하고 다음콤보 들어오는것 기다리는 시간

	list  <SComEffect>  m_comEffList;
	
	char		m_cRndKey;
	char		m_cNextRndKey;
	int			m_nNextRNum;
	
	SRect		m_NextRComKeyRect;			//다음 랜덤키 
	SPoint2		m_NextRComKeyCenPoint;		//다음 랜덤키 중심점 

public:
	CCombo2DTimingEffect();
	virtual ~CCombo2DTimingEffect();
	void InitEffect();
	void StartEffect();
	void EndEffect();

	void UpdateProc();
	void RenderProc();

	bool IsRender()		{ return m_IsRender; }
	bool IsActive()		{ return m_IsActive; }	
	void SetActive(bool bFlag)	{m_IsActive = bFlag;}
	void SetComboState(UINT uState);
	UINT GetComboState();
	
	void RenderComboKeyProc(int PosX, int PosY);
	void SetComCount(int nCount, int nRace, long lDestID );
	
	int GetCurtComboType();
	int GetCurtComboTypeSecond();
	void GetNeedSkill( TCHAR * strTemp );
	int GetComboKey( int nCurtComboType );
	int GetComCount()	{ return m_nComCount; }
	char GetNowComboKey();
	
	void LogOff();
};


//-----------------------------------------------------------------------------
#endif	__COMBO2DTIMINGEFFECT_H__