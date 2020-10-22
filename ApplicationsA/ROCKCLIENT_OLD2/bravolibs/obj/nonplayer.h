

#ifndef _NONPLAYER_H_
#define _NONPLAYER_H_

#include	"Character.H"
#include	"..\\..\\RockLibs\\RockInterface\\Define.h"
#include	"CClassView.h"		// ī�޶� ��ŷ ���� By wxywxy

const WORD	c_ObjectNPCCode_Min( 5300 );
const WORD	c_ObjectNPCCode_Max( 5499 );

const WORD  c_NotMoveNPCCode_Min( 3800 );
const WORD  c_NotMoveNPCCode_Max( 3801 );

class NonPlayer : public Character
{
public:
	EVENT_DATA_INFO		theAttack_Event;	// ���� �޼��� ó�� �̺�Ʈ
	
	bool				theIsAttack;
	long				theAttackDelayTime;
	
	bool				m_OneTimeJump;
	int					m_nNpcCode;

protected:		
	void		AniProcessMove( bool middle );			// �̵� �̺�Ʈó��	
	bool		FindDestTile( EVENT_DATA_INFO& data, bool bNpc = false );		// �̵����� Ÿ������ �˻�
	bool		GetNextTile();							// ����Ÿ�� �˾Ƴ���
	bool		FindPath( short now_x, short now_y, int count ); // ���� ��ã��
	bool		FindPath( short now_x, short now_y, EVENT_DATA_INFO& data, int count );
	int			GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data );
	bool		FindNpcTile( int _nCurtX, int _nCurtY, int _nRange, EVENT_DATA_INFO& data );
	void		CheckTargetTile( EVENT_DATA_INFO& data );

	// ���� -> Ŭ���̾�Ʈ �޼���
	bool		SM__MOVE__BROADCAST( EVENT_DATA_INFO& data );
	bool		SM__MOVE( EVENT_DATA_INFO& data );				// �̵�
	bool		SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc );				// �̵�
	bool		SM__ATTACK__NPC( EVENT_DATA_INFO& data );		// ����
	bool		SM__UNDERATTACK( EVENT_DATA_INFO& data );		// �´� ��� ó��
	bool		SM__STATE__DIE( EVENT_DATA_INFO& data );		// �´� ��� ó��
	bool		SM__NPC__RESPAWN( EVENT_DATA_INFO& data );		// ����
	bool		SM__NPC__DIE( EVENT_DATA_INFO& data );			// ����	
	bool		SM__NPC__USE__MAGIC( EVENT_DATA_INFO& data );	// NPC�������
	bool		SM__USE__SKILL( EVENT_DATA_INFO& data );		// ��ų ó��
	bool		SM__CHANT__READY( EVENT_DATA_INFO& data );		// ��â �غ� ��� ó��
	bool		SM__CHANT__SPELL( EVENT_DATA_INFO& data );		// ��â �ݺ� ��� ó��
	bool		SM__RESTSKILL( EVENT_DATA_INFO& data );			// �޽� ��ų ó�� 
	bool		SM__ITEM__DROP( EVENT_DATA_INFO& data );		// ������ ��� ��� ó�� 
	bool		SM__BOW_ATTACK( EVENT_DATA_INFO& data );		// ��ó ����
	bool		SM__ANI_QUEUE_PROCESS( EVENT_DATA_INFO& data );	// ���ڸ����� �׼����̺� ó�� By wxywxy
	bool		SM_ACT_PROCESS( EVENT_DATA_INFO& data );
	bool		SM_USER_STORE(EVENT_DATA_INFO &data);			// ���� ��� ó��
	bool		CM__ATTACK__NPC( EVENT_DATA_INFO& data );								// ���� ó��

	// Ŭ���̾�Ʈ -> ���� �޼��� ó�� 
	bool		CM__MOVE( int _nCurtX, int _nCurtY, int _nNextX, int _nNextY );			// �н� �̵� ó��
	

	// ���� �ִϸ��̼�
	void		RespawnProcess();
	int			DieProcess();
	
	void		UpdateAniProcess();
	
public:
	void		SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent );

public:
	NonPlayer() { Clear(); }
	~NonPlayer() { Destroy(); }


	//-------------------------------------------------------------------------
	DWORD			m_CID;						// ĳ�� ���̵�.
	TCHAR			m_Name[32];					// ���̵�...

	NBalloonType	m_BType;
	D3DCOLOR		m_Color;
	TCHAR			m_Message[128];				// ä�� �޼���....
	//-------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// Desc : �ʱ�ȭ
	//-----------------------------------------------------------------------------
	void		Clear();
	void		Destroy();
			
	//-----------------------------------------------------------------------------
	// Desc : �����б�, ĳ���� ����
	//-----------------------------------------------------------------------------
	bool		CreateNonPlayer(bool IsPet = false);			// ĳ���� ���� �б�	
	void		CreateItem();									// ĳ����������� ����
	void		SetWearItem();									// ��� ���� 
	void		CreateSTID();									// ��Ƽ�� ����
	
	void		ReSetWearItem();
	
	///-- By simwoosung
	///-- NPC - �÷��̾� ���� ������
	void		SetPlayer_NpcWearItem();

	void		SetPlayer_NpcChrImage();

	//-----------------------------------------------------------------------------
	// Desc : �ִϸ��̼�ó��
	//-----------------------------------------------------------------------------
	int			AniProcess(bool bFlag = false);									// �ִϸ��̼�ó��	
	int			AniProcessMoveEvent();							// �̵� �̺�Ʈ ó��
	int			AniProcessEvent();								// �̺�Ʈó��

	//-----------------------------------------------------------------------------
	// Desc : �׸���ó��, ���õ��׶��ó��
	//-----------------------------------------------------------------------------
	BOOL		ShadowMake(bool bFlag = false);					// �׸��� ����
	void		ShadowDisplay(bool bFlag = false);				// �׸��� �׸���
	void		ApplyWeaponShaowProj();

	void		SelectDisplay();								// ���õ��׶�̱׸���
	void		RenderShadow(SHADOW_INFO & sShadow);
	void		ApplyShaowProj(SHADOW_INFO & sShadow);

	//-----------------------------------------------------------------------------
	// Desc : �޼���ó��
	//-----------------------------------------------------------------------------
	void		SetQueue( const EVENT_DATA_INFO& data );		// ť�� ����Ÿ ����
	void		QueueProcess();									// ť�� ����� ����Ÿó��
	
	///- by simwoosung
	void		PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear = FALSE );
				// ���� ť�� ��� ���ʿ��� ��ó��
	void		AllActQueue();									
				// ���� ����� ť�� ���� �����Ų��.
				// ����, ��ųť�� ����
	void		UpdateHPData();
	
protected:

	BOOL		m_IsCDataQActed;								// ���� ������ ó���� �ʿ��� ť��
																// ������ ���ƴ��� ����	

public:
	// �α��� ���ϸ��̼� ���̺� ó�� By wxywxy
	static int				 m_StaticAniCount;					// Ŭ�����信�� ������ ���� ī��Ʈ
	static CLASS_VIEW_DATA	*m_pCharInfo;			

};


#endif