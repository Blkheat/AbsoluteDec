

#ifndef _NONPLAYER_H_
#define _NONPLAYER_H_

#include	"Character.H"
#include	"..\\..\\RockLibs\\RockInterface\\Define.h"
#include	"CClassView.h"		// 카메라 워킹 관련 By wxywxy

const WORD	c_ObjectNPCCode_Min( 5300 );
const WORD	c_ObjectNPCCode_Max( 5499 );

const WORD  c_NotMoveNPCCode_Min( 3800 );
const WORD  c_NotMoveNPCCode_Max( 3801 );

class NonPlayer : public Character
{
public:
	EVENT_DATA_INFO		theAttack_Event;	// 공격 메세지 처리 이벤트
	
	bool				theIsAttack;
	long				theAttackDelayTime;
	
	bool				m_OneTimeJump;
	int					m_nNpcCode;

protected:		
	void		AniProcessMove( bool middle );			// 이동 이벤트처리	
	bool		FindDestTile( EVENT_DATA_INFO& data, bool bNpc = false );		// 이동가능 타일인지 검샘
	bool		GetNextTile();							// 다음타일 알아내기
	bool		FindPath( short now_x, short now_y, int count ); // 유저 길찾기
	bool		FindPath( short now_x, short now_y, EVENT_DATA_INFO& data, int count );
	int			GetNextPath( DWORD& next_x, DWORD& next_y, EVENT_DATA_INFO& data );
	bool		FindNpcTile( int _nCurtX, int _nCurtY, int _nRange, EVENT_DATA_INFO& data );
	void		CheckTargetTile( EVENT_DATA_INFO& data );

	// 서버 -> 클라이언트 메세지
	bool		SM__MOVE__BROADCAST( EVENT_DATA_INFO& data );
	bool		SM__MOVE( EVENT_DATA_INFO& data );				// 이동
	bool		SM__MOVE( EVENT_DATA_INFO& data, bool bFindNpc );				// 이동
	bool		SM__ATTACK__NPC( EVENT_DATA_INFO& data );		// 공격
	bool		SM__UNDERATTACK( EVENT_DATA_INFO& data );		// 맞는 모션 처리
	bool		SM__STATE__DIE( EVENT_DATA_INFO& data );		// 맞는 모션 처리
	bool		SM__NPC__RESPAWN( EVENT_DATA_INFO& data );		// 생성
	bool		SM__NPC__DIE( EVENT_DATA_INFO& data );			// 죽음	
	bool		SM__NPC__USE__MAGIC( EVENT_DATA_INFO& data );	// NPC마법사용
	bool		SM__USE__SKILL( EVENT_DATA_INFO& data );		// 스킬 처리
	bool		SM__CHANT__READY( EVENT_DATA_INFO& data );		// 영창 준비 모션 처리
	bool		SM__CHANT__SPELL( EVENT_DATA_INFO& data );		// 영창 반복 모션 처리
	bool		SM__RESTSKILL( EVENT_DATA_INFO& data );			// 휴식 스킬 처리 
	bool		SM__ITEM__DROP( EVENT_DATA_INFO& data );		// 아이템 드롭 모션 처리 
	bool		SM__BOW_ATTACK( EVENT_DATA_INFO& data );		// 아처 공격
	bool		SM__ANI_QUEUE_PROCESS( EVENT_DATA_INFO& data );	// 제자리에서 액션테이블 처리 By wxywxy
	bool		SM_ACT_PROCESS( EVENT_DATA_INFO& data );
	bool		SM_USER_STORE(EVENT_DATA_INFO &data);			// 상점 모션 처리
	bool		CM__ATTACK__NPC( EVENT_DATA_INFO& data );								// 공격 처리

	// 클라이언트 -> 서버 메세지 처리 
	bool		CM__MOVE( int _nCurtX, int _nCurtY, int _nNextX, int _nNextY );			// 분신 이동 처리
	

	// 생성 애니메이션
	void		RespawnProcess();
	int			DieProcess();
	
	void		UpdateAniProcess();
	
public:
	void		SetUnderAttackEventMgr( EVENT_DATA_INFO& aEvent );

public:
	NonPlayer() { Clear(); }
	~NonPlayer() { Destroy(); }


	//-------------------------------------------------------------------------
	DWORD			m_CID;						// 캐릭 아이디.
	TCHAR			m_Name[32];					// 아이디...

	NBalloonType	m_BType;
	D3DCOLOR		m_Color;
	TCHAR			m_Message[128];				// 채팅 메세지....
	//-------------------------------------------------------------------------


	//-----------------------------------------------------------------------------
	// Desc : 초기화
	//-----------------------------------------------------------------------------
	void		Clear();
	void		Destroy();
			
	//-----------------------------------------------------------------------------
	// Desc : 파일읽기, 캐릭터 생성
	//-----------------------------------------------------------------------------
	bool		CreateNonPlayer(bool IsPet = false);			// 캐릭터 파일 읽기	
	void		CreateItem();									// 캐릭터정보대로 생성
	void		SetWearItem();									// 장비 착용 
	void		CreateSTID();									// 스티드 생성
	
	void		ReSetWearItem();
	
	///-- By simwoosung
	///-- NPC - 플레이어 관련 장비생성
	void		SetPlayer_NpcWearItem();

	void		SetPlayer_NpcChrImage();

	//-----------------------------------------------------------------------------
	// Desc : 애니메이션처리
	//-----------------------------------------------------------------------------
	int			AniProcess(bool bFlag = false);									// 애니메이션처리	
	int			AniProcessMoveEvent();							// 이동 이벤트 처리
	int			AniProcessEvent();								// 이벤트처리

	//-----------------------------------------------------------------------------
	// Desc : 그림자처리, 선택동그라미처리
	//-----------------------------------------------------------------------------
	BOOL		ShadowMake(bool bFlag = false);					// 그림자 생성
	void		ShadowDisplay(bool bFlag = false);				// 그림자 그리기
	void		ApplyWeaponShaowProj();

	void		SelectDisplay();								// 선택동그라미그리기
	void		RenderShadow(SHADOW_INFO & sShadow);
	void		ApplyShaowProj(SHADOW_INFO & sShadow);

	//-----------------------------------------------------------------------------
	// Desc : 메세지처리
	//-----------------------------------------------------------------------------
	void		SetQueue( const EVENT_DATA_INFO& data );		// 큐에 데이타 저장
	void		QueueProcess();									// 큐에 저장된 데이타처리
	
	///- by simwoosung
	void		PushQueue(EVENT_DATA_INFO & data, BOOL IsAllClear = FALSE );
				// 들어온 큐는 모두 이쪽에서 전처리
	void		AllActQueue();									
				// 현재 저장된 큐를 전부 실행시킨다.
				// 공격, 스킬큐에 한해
	void		UpdateHPData();
	
protected:

	BOOL		m_IsCDataQActed;								// 현재 데이터 처리가 필요한 큐가
																// 동작을 마쳤는지 여부	

public:
	// 로그인 에니메이션 테이블 처리 By wxywxy
	static int				 m_StaticAniCount;					// 클래스뷰에서 순차적 에니 카운트
	static CLASS_VIEW_DATA	*m_pCharInfo;			

};


#endif