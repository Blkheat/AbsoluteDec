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

	int			m_nComCount;				//�޺� ī��Ʈ
	int			m_nRace;					//�޺������� Ÿ�� 
	long		m_lDestID;					//�޺������� ���̵�

protected:
	int			m_nMotion;
	
	bool		m_IsActive;					//�޺�Ÿ�̹� ����Ȱ��ȭ ����
	bool		m_IsRender;					//������ ����	
	
	bool		m_IsTempFlag;				//����Ʈ ������ ������ �ٷ� �����ʱ����� �÷���
	bool		m_IsRenderGlow;				//��¦�̴� ȿ��
	
	SRect		m_BarBgRect;				//Ÿ�̹� �ܰ�

	SRect		m_BarComKeyRect[3];			//Ÿ�̹� �޺�Ű �̹���
	SPoint2		m_BarComKeyCenPoint[3];		//Ÿ�̹� �޺�Ű �߽���

	SRect		m_BarGaugeRect;				//������ �ܰ�
	SRect		m_BarSBgRect;				//���� Ÿ�̹� �ܰ�

	D3DCOLOR	m_Color;					//Į��
	D3DCOLOR	m_TexColor;					//���α۾� Į��

	float		m_fFlashTime;				//������-���� Ÿ��
	float		m_fCumulativeTime;			//���� Ÿ��

	UINT		m_uComboState;

	float		m_fRatio;					//�������� ����
	bool		m_IsRenderComboKey;			//�޺�Ű ������ ����

	float		m_finterTime;				//�޺�Ű�� ������ ���� ����Ÿ��
	float		m_fiCumTime;				//�޺�Ű�� ������ ���� ����Ÿ��

	float       m_fGlowRatio;				//��¦�̴� �ð����� ����
	
	bool		m_IsComboKeyReg;			//�޺�Ű ��Ͽ���	
	
	char		m_cComboKey;
	bool		m_bEventFlag;				//�̺�Ʈ �߻��÷���
	bool		m_bIsSuccess;

	int			m_nCurrentFrame;
	long		m_lElasetime;
	bool		m_bIsStart;	
	
	float		m_fComDelayTime;			//Ű�����ϰ� �����޺� �����°� ��ٸ��� �ð�

	list  <SComEffect>  m_comEffList;
	
	char		m_cRndKey;
	char		m_cNextRndKey;
	int			m_nNextRNum;
	
	SRect		m_NextRComKeyRect;			//���� ����Ű 
	SPoint2		m_NextRComKeyCenPoint;		//���� ����Ű �߽��� 

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