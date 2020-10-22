
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include	<list>

using namespace std;

typedef list < DWORD > AttackerList;
typedef list < DWORD >::iterator AttackerListPos;

#include	"Character.H"
#include	"ActionSystem.h"

#include	"..\\..\\RockLibs\\RockInterface\\Define.h"
#include	"..\\..\\bravolibs\\effect\\CComboEffect.h"
#include	"..\\..\\RockLibs\\RockInterface\\CArcherBar.h"

const WORD	c_NonplayerCode_Min( 5000 );
const WORD	c_NonplayerCode_Max( 5500 );

inline float DIST(float sx, float sz, float dx, float dz) { return float( sqrt( ((sx-dx) * (sx-dx)) + ((sz-dz) * (sz-dz)) ) ); }


typedef list < EVENT_DATA_INFO > ComBoAttackList;
typedef list < EVENT_DATA_INFO >::iterator ComBoAttackListPos;

class Player : public Character
{	
protected:
	///--JAPAN_BUG_MODIFY
	DWORD					m_PreSendCsMoveTime;	

public:
	EVENT_DATA_INFO			theTAtSave_Event;	// Ÿ�� ���� ���� �޼��� ó�� �̺�Ʈ
	EVENT_DATA_INFO			theAttack_Event;	// ���� �޼��� ó�� �̺�Ʈ 
//	EVENT_DATA_INFO			theCombo_Event;		// �޺� �̺�Ʈ
	BYTE 					theCombo_State;		// �ڽ��� �޺� ���� ���� 

	CActionSystem			theActionSystem;

	long					theWarpTime;
	NAnswerWarp				theMapType;
	BOOL					theFightZoneReady;
	BOOL					IsMyPlayerRender;

	bool					theIsAttack;
	long					theAttackDelayTime;
	
	WORD					theAddEffect_SkillCode[RP_MAX_KEEPUP_SKILL];
	BYTE					theAddEffect_State[MAX_ADDEFFECT_NUM];
	
	bool					m_OneTimeJump;

	DWORD					m_aMinAttackDelayTime;			///-- ���ݰ��� ������ Ÿ��					
	DWORD					m_aLastAttackSendTime;		///-- ���������� ���ݰ��� ��Ŷ�� �����ð�
	ComBoAttackList			m_ComBoAttackList;			///-- �޺����� ������Ŷ ����Ʈ

protected:	
	bool			m_bPath;					// ��ã��		
	bool			m_bLatency;					// ����ȭ üũ		
	bool			m_bSendMove;				// ������ �̵� ���ȴ�?	

public:	
	bool			m_bWait;					// ��ٸ���(������)
	unsigned int	m_AttackType;				// ���� ����
	unsigned int	m_AttackStep;				// ���� �ܰ�



	//-------------------------------------------------------------------------
	DWORD			m_CID;						// ĳ�� ���̵�.
	TCHAR			m_Name[32];					// ���̵�...

	NBalloonType	m_BType;
	D3DCOLOR		m_Color;
	TCHAR			m_Message[128];				// ä�� �޼���....
	//-------------------------------------------------------------------------
	
	//by simwoosung - ����Ʈ������ �ڽ��� �׾�������� �ڽ��� �����̵Ǵ� ����� �ε�����ȣ
	int				m_nViewRefDestIndex;

protected:		
	void	AniProcessMove( bool middle );								// �̵� �ִϸ��̼�	
	bool	FindDestTile( EVENT_DATA_INFO& data, bool bNpc = false );		// �̵����� Ÿ������ �˻�		
	int		FindNextTile();												// �̵��� ����Ÿ�� ã��		
	bool 	FindPath( short pre_x, short pre_y, EVENT_DATA_INFO& data, int count = 1 );		// ���� Ÿ�� ã��( A* )	
	int		GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data );

	//-----------------------------------------------------------------------------
	// Desc: ����ó�� 
	//-----------------------------------------------------------------------------	
	void	AttackSelectProcess();												// ���� ����
	bool	FindNpcTile( int now_x, int now_y, int range, EVENT_DATA_INFO& data );	// �ش��������� �����ε��鼭 �˻�
	void	CheckTargetTile( EVENT_DATA_INFO& data );

	void	UpdateAniProcess();
	
public:
	void	SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent );
	bool	GetAddEffectState(NAddEffectType EffectType);

public:
	bool	SM__MOVE__BROADCAST( EVENT_DATA_INFO& data );							// �̵�üũ
	bool	SM__ATTACK__NPC( EVENT_DATA_INFO& data );								// ���� ó��
	bool	SM__UNDERATTACK( EVENT_DATA_INFO& data );								// �´� ��� ó��
	bool	SM__STATE__DIE( EVENT_DATA_INFO& data );								// �״� ��� ó��
	bool	SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc = false );				// �̵�		
	bool	SM__USE__SKILL( EVENT_DATA_INFO& data );									// ��ų ó��
	bool	SM__CHANT__READY( EVENT_DATA_INFO& data );								// ��â �غ� ��� ó��
	bool	SM__CHANT__SPELL( EVENT_DATA_INFO& data );								// ��â �ݺ� ��� ó��
	bool	SM__RESTSKILL( EVENT_DATA_INFO& data );									// �޽� ��ų ó��
	
	bool	CM__ATTACK__NPC( EVENT_DATA_INFO& data );								// ���� ó��

	bool    SM__BOW_ATTACK( EVENT_DATA_INFO& data );								// ȭ�� ���ݽ� ó��
	bool	CM__ATTACK__BOW( int Level , HIT_TYPE HitType );						// ȭ�� ���� ó��
	bool    SM_ACT_PROCESS( EVENT_DATA_INFO& data );
	
	bool	SM_USER_STORE(EVENT_DATA_INFO &data);									// ���� ���� ó��
	bool	SM_ANI_SQUENCE_PROCESS(EVENT_DATA_INFO &data);							// ������ ó���� ���ϸ��̼�
	
	//-----------------------------------------------------------------------------
	// Desc: ���� -> Ŭ���̾�Ʈ �޼���
	//-----------------------------------------------------------------------------	
/*	bool	SM__MOVE__BROADCAST( EVENT_DATA_INFO data );							// �̵�üũ
	bool	SM__MOVE( EVENT_DATA_INFO data, bool bFindNpc = false );				// �̵�		
	bool	SM__ATTACK__NPC( EVENT_DATA_INFO data, bool bFirst = true );			// ����		
	void	SM__USER__DIE( EVENT_DATA_INFO data );								// �������

	//-----------------------------------------------------------------------------
	// Desc: Ŭ���̾�Ʈ -> ���� �޼���	
	//-----------------------------------------------------------------------------		
	void	CM__ATTACK( int x, int y );										// ����
	bool	CM__USE__SKILL( EVENT_DATA_INFO data );								// ��ų ���
	bool	CM__PICKUP__ITEM( EVENT_DATA_INFO data );							// ������ �ݱ�
	bool	CM__HORSE__RIDE( EVENT_DATA_INFO data );								// ��Ÿ��
*/
	bool	CM__MOVE( int now_x, int now_y, int next_x, int next_y );		// �̵�
	bool	CM__READY__WARP( long aNowTime, NAnswerWarp aMapType );


public:
	Player() { Clear(); }
	~Player() { Destroy(); }

	//-----------------------------------------------------------------------------
	// Desc: �ʱ�ȭ, �ı���
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();
	void	SetHeadRotate();	

	//-----------------------------------------------------------------------------
	// Desc: ĳ���� ���� �б�	
	//-----------------------------------------------------------------------------
	void	CreatePlayer( BYTE _byType = 0 );
	
	//-----------------------------------------------------------------------------
	// Desc: ĳ���� ������ ����
	//-----------------------------------------------------------------------------
	void	SetWearItem();				// �����ϰ� �ִ� ������ �ڵ忡 ���� ������ �׷��ش� 
	void	SetTempItem();
	
	void	ReSetWearItem();

	//-----------------------------------------------------------------------------
	// Desc: �ִϸ��̼�ó��, �������� ���� ����Ÿó��
	//-----------------------------------------------------------------------------	
	bool	AniProcessMoveEvent();												// �̵� �̺�Ʈ ó��
	bool	AniProcessEvent();													// �����̺�Ʈó��
	void	AniProcess();														// �ִϸ��̼�ó��	
	void	SetQueue( const EVENT_DATA_INFO& data );									// ť�� ����Ÿ ����
	void	QueueProcess();														// ����Ÿó��
	void	QueueClear();														// ����Ÿ����
	bool	GetHorsePos( int& x, int& y, int index );							// ����ġ ã��
	bool	GetTargetState( EVENT_DATA_INFO data, bool bFirst = true );			// Ÿ���� ���ݰ������� �˻�
	
	int		DieProcess();														// �÷��̾� �׾����� ó��
	
	///- by simwoosung
	void		PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear = FALSE );
				// ���� ť�� ��� ���ʿ��� ��ó��
	void		AllActQueue();									
				// ���� ����� ť�� ���� �����Ų��.
				// ����, ��ųť�� ����	

	BOOL	m_IsCDataQActed;
	
	void	UpdateHPData();

	//-----------------------------------------------------------------------------
	// Desc: �׸���ó��
	//-----------------------------------------------------------------------------
	BOOL	ShadowMake();							// �׸��� ����
	void	ShadowDisplay();						// �׸��� �׸���
	void	ApplyWeaponShaowProj();		

	//-----------------------------------------------------------------------------
	// Desc: �÷��̾� ������ ���� ����Ʈ
	//-----------------------------------------------------------------------------
	CComboEffect	m_ComboEffectSet;				// By wxywxy
	
	//-----------------------------------------------------------------------------
	// �н� ��Ʈ�� ����...
	//-----------------------------------------------------------------------------
	void		Illusion_CsStartMove( DWORD aDestCell , bool bMove = false);
	void		Illusion_CsAttack( DWORD aAttackedID, BYTE aAttackedType, WORD aMotion, BYTE aComboType, BYTE aComboCount, BYTE aComboStatus, BYTE aComboSeq );
	void		Illusion_CsSkill( DWORD aTarget, BYTE aTargetType, WORD aSkillCode );// ��ų ���
	void		DetectPartyIllusion( DWORD aPartyID, BOOL bParty );
	
	//-----------------------------------------------------------------------------
	// �� ��Ʈ��..
	//-----------------------------------------------------------------------------
	void		Pet_CsStartMove(DWORD aDestCell, bool m_IsDefenceBack = false, EVENT_DATA_INFO *pData = NULL);
	void		Pet_CsOwnerBackMove();	
	void		PetSetAttackQueue( EVENT_DATA_INFO& data, bool bAttackFlag = true );


	///-- �߱� PK��� �����۾� By simwoosung

private:
	BYTE				m_PKMode;				///-- �� PK���
	AttackerList		m_MyAttackerList;		///-- ���� ������ ������ ����Ʈ

public:

	void		SetPKMode( BYTE bMode ) { m_PKMode = bMode; }
	BYTE		GetPKMOde()	{ return m_PKMode; }
	
	BOOL		IsMyAttacker(DWORD nAPCId);					///-- ���� ������ �����ΰ�?
	BOOL		InsertMyAttackerList(DWORD nAPCId);			///-- ���� ������ ��������Ʈ�� �߰�
	BOOL		DeleteMyAttackerList(DWORD nAPCId);			///-- ���� ������ ��������Ʈ���� ����
	void		ClearAllMyAttackerList();					///-- ���� ������ ��������Ʈ ���������

	void		ExcutePKModeAction();						///-- PK��� ��� ���� ���� ���� 	

};


#endif