
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
	EVENT_DATA_INFO			theTAtSave_Event;	// 타겟 공격 저장 메세지 처리 이벤트
	EVENT_DATA_INFO			theAttack_Event;	// 공격 메세지 처리 이벤트 
//	EVENT_DATA_INFO			theCombo_Event;		// 콤보 이벤트
	BYTE 					theCombo_State;		// 자신의 콤보 성공 여부 

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

	DWORD					m_aMinAttackDelayTime;			///-- 공격관련 딜레이 타임					
	DWORD					m_aLastAttackSendTime;		///-- 마지막으로 공격관련 패킷을 보낸시간
	ComBoAttackList			m_ComBoAttackList;			///-- 콤보에서 공격패킷 리스트

protected:	
	bool			m_bPath;					// 길찾기		
	bool			m_bLatency;					// 동기화 체크		
	bool			m_bSendMove;				// 서버에 이동 날렸니?	

public:	
	bool			m_bWait;					// 기다리기(딜레이)
	unsigned int	m_AttackType;				// 현재 공격
	unsigned int	m_AttackStep;				// 공격 단계



	//-------------------------------------------------------------------------
	DWORD			m_CID;						// 캐릭 아이디.
	TCHAR			m_Name[32];					// 아이디...

	NBalloonType	m_BType;
	D3DCOLOR		m_Color;
	TCHAR			m_Message[128];				// 채팅 메세지....
	//-------------------------------------------------------------------------
	
	//by simwoosung - 파이트존에서 자신이 죽어있을경우 자신의 시점이되는 상대의 인덱스번호
	int				m_nViewRefDestIndex;

protected:		
	void	AniProcessMove( bool middle );								// 이동 애니메이션	
	bool	FindDestTile( EVENT_DATA_INFO& data, bool bNpc = false );		// 이동가능 타일인지 검샘		
	int		FindNextTile();												// 이동중 다음타일 찾기		
	bool 	FindPath( short pre_x, short pre_y, EVENT_DATA_INFO& data, int count = 1 );		// 다음 타일 찾기( A* )	
	int		GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data );

	//-----------------------------------------------------------------------------
	// Desc: 공격처리 
	//-----------------------------------------------------------------------------	
	void	AttackSelectProcess();												// 공격 선택
	bool	FindNpcTile( int now_x, int now_y, int range, EVENT_DATA_INFO& data );	// 해당지점에서 원으로돌면서 검사
	void	CheckTargetTile( EVENT_DATA_INFO& data );

	void	UpdateAniProcess();
	
public:
	void	SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent );
	bool	GetAddEffectState(NAddEffectType EffectType);

public:
	bool	SM__MOVE__BROADCAST( EVENT_DATA_INFO& data );							// 이동체크
	bool	SM__ATTACK__NPC( EVENT_DATA_INFO& data );								// 공격 처리
	bool	SM__UNDERATTACK( EVENT_DATA_INFO& data );								// 맞는 모션 처리
	bool	SM__STATE__DIE( EVENT_DATA_INFO& data );								// 죽는 모션 처리
	bool	SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc = false );				// 이동		
	bool	SM__USE__SKILL( EVENT_DATA_INFO& data );									// 스킬 처리
	bool	SM__CHANT__READY( EVENT_DATA_INFO& data );								// 영창 준비 모션 처리
	bool	SM__CHANT__SPELL( EVENT_DATA_INFO& data );								// 영창 반복 모션 처리
	bool	SM__RESTSKILL( EVENT_DATA_INFO& data );									// 휴식 스킬 처리
	
	bool	CM__ATTACK__NPC( EVENT_DATA_INFO& data );								// 공격 처리

	bool    SM__BOW_ATTACK( EVENT_DATA_INFO& data );								// 화살 공격시 처리
	bool	CM__ATTACK__BOW( int Level , HIT_TYPE HitType );						// 화살 공격 처리
	bool    SM_ACT_PROCESS( EVENT_DATA_INFO& data );
	
	bool	SM_USER_STORE(EVENT_DATA_INFO &data);									// 개인 상점 처리
	bool	SM_ANI_SQUENCE_PROCESS(EVENT_DATA_INFO &data);							// 시퀀스 처리시 에니메이션
	
	//-----------------------------------------------------------------------------
	// Desc: 서버 -> 클라이언트 메세지
	//-----------------------------------------------------------------------------	
/*	bool	SM__MOVE__BROADCAST( EVENT_DATA_INFO data );							// 이동체크
	bool	SM__MOVE( EVENT_DATA_INFO data, bool bFindNpc = false );				// 이동		
	bool	SM__ATTACK__NPC( EVENT_DATA_INFO data, bool bFirst = true );			// 공격		
	void	SM__USER__DIE( EVENT_DATA_INFO data );								// 유저사망

	//-----------------------------------------------------------------------------
	// Desc: 클라이언트 -> 서버 메세지	
	//-----------------------------------------------------------------------------		
	void	CM__ATTACK( int x, int y );										// 공격
	bool	CM__USE__SKILL( EVENT_DATA_INFO data );								// 스킬 사용
	bool	CM__PICKUP__ITEM( EVENT_DATA_INFO data );							// 아이템 줍기
	bool	CM__HORSE__RIDE( EVENT_DATA_INFO data );								// 말타기
*/
	bool	CM__MOVE( int now_x, int now_y, int next_x, int next_y );		// 이동
	bool	CM__READY__WARP( long aNowTime, NAnswerWarp aMapType );


public:
	Player() { Clear(); }
	~Player() { Destroy(); }

	//-----------------------------------------------------------------------------
	// Desc: 초기화, 파괴자
	//-----------------------------------------------------------------------------
	void	Clear();
	void	Destroy();
	void	SetHeadRotate();	

	//-----------------------------------------------------------------------------
	// Desc: 캐릭터 파일 읽기	
	//-----------------------------------------------------------------------------
	void	CreatePlayer( BYTE _byType = 0 );
	
	//-----------------------------------------------------------------------------
	// Desc: 캐릭터 아이템 장착
	//-----------------------------------------------------------------------------
	void	SetWearItem();				// 장착하고 있는 아이템 코드에 따라서 실제로 그려준다 
	void	SetTempItem();
	
	void	ReSetWearItem();

	//-----------------------------------------------------------------------------
	// Desc: 애니메이션처리, 서버에서 받은 데이타처리
	//-----------------------------------------------------------------------------	
	bool	AniProcessMoveEvent();												// 이동 이벤트 처리
	bool	AniProcessEvent();													// 현재이벤트처리
	void	AniProcess();														// 애니메이션처리	
	void	SetQueue( const EVENT_DATA_INFO& data );									// 큐에 데이타 저장
	void	QueueProcess();														// 데이타처리
	void	QueueClear();														// 데이타삭제
	bool	GetHorsePos( int& x, int& y, int index );							// 말위치 찾기
	bool	GetTargetState( EVENT_DATA_INFO data, bool bFirst = true );			// 타겟이 공격가능한지 검사
	
	int		DieProcess();														// 플레이어 죽었을때 처리
	
	///- by simwoosung
	void		PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear = FALSE );
				// 들어온 큐는 모두 이쪽에서 전처리
	void		AllActQueue();									
				// 현재 저장된 큐를 전부 실행시킨다.
				// 공격, 스킬큐에 한해	

	BOOL	m_IsCDataQActed;
	
	void	UpdateHPData();

	//-----------------------------------------------------------------------------
	// Desc: 그림자처리
	//-----------------------------------------------------------------------------
	BOOL	ShadowMake();							// 그림자 생성
	void	ShadowDisplay();						// 그림자 그리기
	void	ApplyWeaponShaowProj();		

	//-----------------------------------------------------------------------------
	// Desc: 플레이어 레벨에 따른 이펙트
	//-----------------------------------------------------------------------------
	CComboEffect	m_ComboEffectSet;				// By wxywxy
	
	//-----------------------------------------------------------------------------
	// 분신 컨트롤 관련...
	//-----------------------------------------------------------------------------
	void		Illusion_CsStartMove( DWORD aDestCell , bool bMove = false);
	void		Illusion_CsAttack( DWORD aAttackedID, BYTE aAttackedType, WORD aMotion, BYTE aComboType, BYTE aComboCount, BYTE aComboStatus, BYTE aComboSeq );
	void		Illusion_CsSkill( DWORD aTarget, BYTE aTargetType, WORD aSkillCode );// 스킬 사용
	void		DetectPartyIllusion( DWORD aPartyID, BOOL bParty );
	
	//-----------------------------------------------------------------------------
	// 펫 컨트롤..
	//-----------------------------------------------------------------------------
	void		Pet_CsStartMove(DWORD aDestCell, bool m_IsDefenceBack = false, EVENT_DATA_INFO *pData = NULL);
	void		Pet_CsOwnerBackMove();	
	void		PetSetAttackQueue( EVENT_DATA_INFO& data, bool bAttackFlag = true );


	///-- 중국 PK모드 관련작업 By simwoosung

private:
	BYTE				m_PKMode;				///-- 내 PK모드
	AttackerList		m_MyAttackerList;		///-- 나를 선공한 유저의 리스트

public:

	void		SetPKMode( BYTE bMode ) { m_PKMode = bMode; }
	BYTE		GetPKMOde()	{ return m_PKMode; }
	
	BOOL		IsMyAttacker(DWORD nAPCId);					///-- 나를 선공한 유저인가?
	BOOL		InsertMyAttackerList(DWORD nAPCId);			///-- 나를 선공한 유저리스트에 추가
	BOOL		DeleteMyAttackerList(DWORD nAPCId);			///-- 나를 선공한 유저리스트에서 삭제
	void		ClearAllMyAttackerList();					///-- 나를 선공한 유저리스트 전부지우기

	void		ExcutePKModeAction();						///-- PK토글 모드 관련 동작 수행 	

};


#endif