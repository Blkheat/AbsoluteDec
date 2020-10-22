#include "RockPCH.h"

#include	<NETWORK\Protocol.h>
//#include	"..\Network.H"
#include	"..\\quadlist.h"
#include	"Character.H"
#include	"..\\PC.h"
#include	"..\\Effect.h"
#include	"..\\Camera.H"
//#include	"..\\Menu_Interface.h"
//#include	"..\\Weather_Manager.H"
#include	"TextureManager.H"
#include	<SOUND\\Sound.h>
//#include	<INTERFACE\\Item.H>
#include    "..\\Effect\\CEffectBase.h"

#include	<assert.H>


#include "..\\RockClient.h"
#include "..\\global.h"

#include	"..\\\rocklibs\RockInterface\\PlayerInfoWnd.h"
#include    "..\\\rocklibs\RockInterface\\PartyInforWnd.h"
#include    "..\\\rocklibs\RockInterface\\EmblemTexManager.h"
#include    "..\\\rocklibs\RockInterface\\UiUtil.h"

#include "..\\..\\CRenderManager.h"
#include "..\\..\\MapDataManager.h"

extern		bool	g_bBlockDebug; 
extern		long	g_nowTime;
extern		int		g_chr_face;
extern		int		g_chr_count;
extern		float	FtoDW(float f);
extern		TEXTURE_MANAGER			g_TexManager;
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9		g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8		g_lpDevice;
#endif // DIRECT_VERSION_9_MJH 
	
extern		bool	g_IsEffectMinimize;

#define		FAIRY_MOVERANGE			8			// 페어리이동거리
#define		ROTATE_FRAME			4			// 회전 프레임
#define		XROTATE_VALUE			20			// X회전값
#define		XROTATE_HEIGHT			1			// X회전하기 위한 높이값
#define		OUTLINE_WIDTH			0.1			// 외각선스케일값 15=1 ~ 80=2... 65로 1을 나눈다
#define		STELTH_ALPHA			0.25f		// 스텔스 알파값
#define		EMITTER_TIME			100			// 발광효과 애니타임	

extern		bool g_bTest;

int s_KeepUpSkillChecker::SetCode(WORD Code)
{
	int Num = GetEmpty();
	
	if( -1 == Num )
		return -1;

	theKeepupSkill[ Num ] = Code;

	return Num;
}

int s_KeepUpSkillChecker::DeleteCode(WORD Code)
{
	int Num = FindCode( Code );
	
	if( -1 == Num )
		return -1;

	theKeepupSkill[ Num ] = 0;

	return Num;
}

void USER_INFO::Clear()
{
	bDown			= false;
	bPkMode			= false;
	bExtacy			= false;
	bCharge			= false;
	bAttack			= false;
	bDefense		= false; 
	EtcItem			= 0;
	fEmitterScale	= 1.0f;
	
	HeadCurFrame	= 9; 
	HeadPreFrame	= 9; 	
	HeadTime		= g_nowTime;

	ArmorInfo.Clear();
	
	// 칼 검기 초기화 ( By wxywxy )
	vSwordTop = vSwordBottom = vSwordAxis[ 0 ] = vSwordAxis[ 1 ] = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
}

//-----------------------------------------------------------------------------
// Desc: 저장 큐 클리어
//-----------------------------------------------------------------------------
void EVENT_DATA_INFO::Init()
{	
	type		 = 0; 
	race		 = 0; 
	x			 = 0; 
	y			 = 0; 
	tbl_Idx		 = 0;
	index		 = 0; 
	unique		 = 0;  	
	motion		 = 0;
	time		 = 0.0f; 	
	loop		 = true;
	kill		 = false;
	damage		 = 0;
	critical	 = 0;
	cell		 = 0;
	count		 = 0;
	code		 = 0;
	status		 = 0;
	number		 = 0;
	skill_unique = 0;
	IsActProc	 = FALSE;

	target_list  = NULL;
	
	weaponView[0] = 1;
	weaponView[1] = 1;
	
	IsHideWing	 = 0;

	comcomple	 = 0;
	
	nADisErrCnt = 0;

	Clear();

	return;	
}
void EVENT_DATA_INFO::Clear()
{
	type		 = 0; 
	race		 = 0; 
	x			 = 0; 
	y			 = 0; 
	tbl_Idx		 = 0;
	index		 = 0; 
	unique		 = 0;  	
	motion		 = 0;
	time		 = 0.0f; 	
	loop		 = true;
	kill		 = false;
	damage		 = 0;
	critical	 = 0;
	cell		 = 0;
	count		 = 0;
	code		 = 0;
	status		 = 0;
	number		 = 0;
	skill_unique = 0;
	IsActProc	 = FALSE;
	lStartTime	 = 0;

	dKeepSkillCode = 0;

	weaponView[0] = 1;
	weaponView[1] = 1;

	IsHideWing	  = 0;
	
	comcomple	 = 0;

	nADisErrCnt = 0;
	
	SAFE_DELETE_ARRAY( target_list );

	return;
}


void EVENT_DATA_INFO::operator=(const EVENT_DATA_INFO & event )
{
	type			= event.type;	
	race			= event.race;
	x				= event.x;
	y				= event.y;
	tbl_Idx			= event.tbl_Idx;
	index			= event.index;
	unique			= event.unique;
	motion			= event.motion;
	time			= event.time;
	loop			= event.loop;
	kill			= event.kill;
	vPos			= event.vPos;
	damage			= event.damage;
	critical		= event.critical;
	cell			= event.cell;
	count			= event.count;
	code			= event.code;
	status			= event.status;
	number			= event.number;	
	skill_unique	= event.skill_unique;
	IsActProc		= event.IsActProc;
	lStartTime		= event.lStartTime;

	dKeepSkillCode  = event.dKeepSkillCode;

	weaponView[0]	= event.weaponView[0];
	weaponView[1]	= event.weaponView[1];

	IsHideWing		= event.IsHideWing;

	comcomple		= event.comcomple;
	
	nADisErrCnt		= event.nADisErrCnt;

	if( event.target_list )
	{		
		SAFE_DELETE_ARRAY( target_list );

		if( event.type == SM_ATTACK_NPC )
		{
			if( event.number )
			{
				target_list = SAFE_NEW_ARRAY( _SAttacked , event.number );				
				memcpy( target_list, event.target_list, sizeof(_SAttacked)*event.number );
			}
		}
		else if( event.type == SM_USE_SKILL )
		{
			if( event.number )
			{
				target_list = SAFE_NEW_ARRAY( _STargetInfo , event.number );				
				memcpy( target_list, event.target_list, sizeof(_STargetInfo)*event.number );
			}
		}
		else if( event.type == SM_MOVE_SKILL)
		{
			if( event.number )
			{
				target_list = SAFE_NEW_ARRAY( _STargetInfo , event.number );				
				memcpy( target_list, event.target_list, sizeof(_STargetInfo)*event.number );
			}
		}


	}	
	
	return;
}

//-----------------------------------------------------------------------------
// Desc: 위치찾기함수
//-----------------------------------------------------------------------------
void	MOVE_INFO::Clear()
{
	vStPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vCVtPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	now_Yrad	= 0.0f; 
	nex_Yrad	= 0.0f; 
	add_Yrad	= 0.0f;
	now_Xrad	= 0.0f; 
	nex_Xrad	= 0.0f; 
	add_Xrad	= 0.0f; 
	step_x		= 0.0f;   
	step_y		= 0.0f;   
	step_height = 0.0f;
	rad_count	= 0;
	bYRotate	= false;

	frad_remain = 0.0f;
}

void	MOVE_INFO::Init( int x, int y )
{ 
	curt.x = x; 
	curt.y = y; 
	curt.height = g_Pc.GetMapHeight( x, y );
	path = dest = next = curt;	
}

bool	MOVE_INFO::ValueUpdate()
{
	if( curt.x == dest.x && curt.y == dest.y )
	{
		if( curt.x == next.x && curt.y == next.y )
			return true;	
		else		
		{
			dest = next;
			return false;
		}
	}
	return false;
}

// 해당 타일값 있니?
bool	MOVE_INFO::FindValue( int x, int y, bool bNext )
{
	if( x == curt.x && y == curt.y )
		return false;
	if( bNext == true )
	{
		if( x == next.x && y == next.y )
			return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Desc: 큐빅커브함수
//-----------------------------------------------------------------------------	
inline	float Eval( float v[4], float u ) { return ((((v[3]*u) + v[2])*u + v[1])*u + v[0]); }

struct _X
{
	float r;
	float no_use[2];
};

void CalcNaturalCubic( float get_abcd[][TOTAL_POINTS], int n, float *vptr )
{
	float gamma[ TOTAL_POINTS ];
	float delta[ TOTAL_POINTS ];
	float D[ TOTAL_POINTS ];
	int i;
	_X *x = (_X *)vptr;

	gamma[ 0 ] = 0.5f;
	for( i = 1; i < n; ++i)
	{
		gamma[ i ] = 1/( 4 - gamma[ i-1 ] );
	}
	gamma[ n ] = 1/( 2-gamma[ n-1 ] );
	
	delta[ 0 ] = 3*(x[1].r - x[0].r) * 0.5f;
	for( i = 1; i < n; ++i)
	{
		delta[ i ] = (3*(x[i+1].r-x[i-1].r)-delta[i-1]) * gamma[i];
	}
	delta[ n ] = (3*(x[n].r-x[n-1].r) - delta[n-1]) * gamma[n];
	
	D[ n ] = delta[ n ];
	for( i = n-1; i >= 0; --i )
	{
		D[ i ] = delta[ i ] - gamma[i]*D[i+1];
	}
	
	for( i = 0; i < n; ++i )
	{
		get_abcd[i][0] = (float)x[i].r;
		get_abcd[i][1] = (float)D[i];
		get_abcd[i][2] = (float)3 * (x[i+1].r - x[i].r) - 2*D[i] - D[i+1];
		get_abcd[i][3] = (2.0f*(x[i].r - x[i+1].r) + D[i] + D[i+1]);
	}
}


//-----------------------------------------------------------------------------
// 버텍스버퍼 생성자, 파괴자
//-----------------------------------------------------------------------------
void	VTX_BUFFER_INFO::Clear()
{
	NormalVB	= NULL;	
	pVertex		= NULL;
	
	bInit		= false;
	nSize		= 0;
}

void	VTX_BUFFER_INFO::Destroy()
{	
	bool IsVBIB = false;
	
	if( NormalVB != NULL )
	{
		NormalVB->Release();
		IsVBIB = true;
	}

	SAFE_DELETE_ARRAY( pVertex );

	Clear();
}

void	VTX_BUFFER_INFO::CreateBuffer( int _nVtxCount )
{
	if( _nVtxCount <= 0 )
		return;

	// 메모리삭제
	Destroy();

	if( FAILED( g_RenderManager.CreateVertexBuffer( _nVtxCount*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
							 					 D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &NormalVB ) ) ) 
	{
		return;		
	}

	// 버퍼값 초기화하기					
	OBJECTVERTEX*	pOBJVERTEX;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
	if( FAILED( NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		return;	

	memset( pOBJVERTEX, 0, sizeof(OBJECTVERTEX) * _nVtxCount);

	NormalVB->Unlock();

	nSize = _nVtxCount;

	pVertex	= SAFE_NEW_ARRAY( D3DXVECTOR3 , _nVtxCount );
	
	if( pVertex )
	{
		memset(pVertex, 0, sizeof(D3DXVECTOR3) * _nVtxCount);
	}

	return;
}


//-----------------------------------------------------------------------------
// Desc: 2개 변수중에서 높은값 리턴하기
//-----------------------------------------------------------------------------
int GetMax( int x, int xx )
{
	if( x < xx )	
		return xx; 
	return x;
}

void Character::SetRealDie( BOOL bFlag, BYTE aAttackType, DWORD aAttackID )
{
	if( bFlag )
	{
		if( m_curt_event.type == SM_STATE_DIE )
		{
			ClientFilelog( "I was Aready Dead But Kill Packet Access!!" );	
		}

		/*if( g_Pc.GetPlayer() == this )
		{
			if( aAttackID )
			{
				if( aAttackType == n_PC )
				{
					PCTABLE * pPcTable = NULL;
					pPcTable = g_Pc.FindPcTable( aAttackID );
					if( pPcTable )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYUSER ), 
										pPcTable->char_info.thePCName );
					}
					else
					{

					}					
				}
				else
				{
					NPCTABLE *pNpcTable = NULL;
					pNpcTable = g_Map.GetNpcTable( aAttackID );
					if( pNpcTable )
					{
						SystemPrint( n_DarkRed, n_SystemMsg, G_STRING_CHAR( IDS_SYSPR_PC_DIE_BYMOB ), 
										pNpcTable->szName );
					}
					else
					{

					}
				}
			}
			else
			{

			}
		}*/
	}
	else
	{
		m_aAttackType = 0;
		m_aAttackID = 0;
	}
	
	m_IsRealDie = bFlag;
}

//-----------------------------------------------------------------------------
// Desc: Object 초기화, 파괴자 
//-----------------------------------------------------------------------------
void	Character::Init()
{				
	m_nDust				= 0;
	m_vScale			= D3DXVECTOR3(   1.0f,  1.0f,   1.0f );	
	m_vLight			= D3DXVECTOR3( -15.0f, 70.0f, -25.0f );
	m_vLight			*= 50.0f;
	m_ani_number		= -1;
	m_pre_ani_number	= -1;
	m_ani_time			= 0.3f;
	m_realani_time		= 0.3f;
	m_end_frame			= 0;
	m_pre_end_frame		= 0;
	m_ani_frame			= 0;
	m_ani_index			= 0;
	
	m_start_blend_frame = 0;
	
	m_next_frame		= 0;
	m_curt_frame		= 0;
		
	m_move_frame		= 0;
	m_move_middle_time	= 0.0f;	

	m_move_percent		= 1.0f;
	m_ani_percent		= 1.0f;

	m_step_time			= 0.0f;
	m_middle_time		= 0.0f;	
	m_PreTime			= g_nowTime;	

	m_byAngle			= 1;	
	m_nTableIndex		= -1;
	m_fDistance			= 0.0f;

	m_bSoundPlay1		= false;
	m_bSoundPlay2		= false;
	m_bIsVisible		= false;
	m_bIsLoaded			= false;		
	m_bShadowDisplay	= false;
	m_bShadowMake		= true;		
	m_bPC				= true;	

	m_pFMove			= NULL;	
	m_pUserInfo			= NULL;		
	
	m_Mov.Clear();			// 이동값	
	m_State.Clear();		// 현재상태	
	m_curt_event.Clear();	// 현재 이벤트
	m_save_event.Clear();	// 저장 이벤트
	m_temp_event.Clear();	// 임시 이벤트		

	// 매트릭스 초기화
	D3DXMatrixIdentity( &m_matPreChr );
	D3DXMatrixIdentity( &m_matChr );
	D3DXMatrixIdentity( &m_matRHand ); 
	D3DXMatrixIdentity( &m_matLHand );	

	D3DXMatrixIdentity( &m_matORHand );
	D3DXMatrixIdentity( &m_matOLHand );
	D3DXMatrixIdentity( &m_matORHand2 );

	D3DXMatrixIdentity( &m_matRealORHand );
	D3DXMatrixIdentity( &m_matRealOLHand );
	D3DXMatrixIdentity( &m_matRealOHair );
	D3DXMatrixIdentity( &m_matRealOCenter );
	D3DXMatrixIdentity( &m_matRealObreast );
	D3DXMatrixIdentity( &m_matRealOLFoot );
	D3DXMatrixIdentity( &m_matRealORFoot );

	D3DXMatrixIdentity( &m_matRealRHand );
	D3DXMatrixIdentity( &m_matRealLHand );
	D3DXMatrixIdentity( &m_matRealHair );
	D3DXMatrixIdentity( &m_matRealCenter );
	D3DXMatrixIdentity( &m_matRealbreast );
	D3DXMatrixIdentity( &m_matRealLFoot );
	D3DXMatrixIdentity( &m_matRealRFoot );
	
	D3DXMatrixIdentity( &m_matRideCenter );
	D3DXMatrixIdentity( &m_matBoneCenter );	

	D3DXMatrixIdentity( &m_matRideChr );

	theFindWay			= NULL;
	memset( thePath, 0, sizeof(DWORD)*6 );
	theCurtPath			= 0;
	theDestPath			= 0;
	
	m_Attack_DelayTime	= 0;
	m_Weapon_DelayTime	= c_WeaponDelayTime;

	theVibrationTime	= 0;
	theIsVibration		= false;

	m_ISQuestEffectCreated = FALSE;

	theIsDrawWeapon = true;
	theDrawWeaponFadeTime = 0;

	ZeroMemory( m_pItm_index, sizeof( int ) * c_Max_WearItemType );
	ZeroMemory( m_Basic_DrawItm, sizeof( int ) * c_Max_WearItemType );
	ZeroMemory( m_Basic_Itm_index, sizeof( int ) * c_Max_WearItemType );
	
	for( int i = 0; i < c_Max_RideItemType ; i++ )
	{
		m_Ride_Item_index[i] = -1;
	}

	// 선점 설정 By wxywxy
	m_ISPriority = TRUE;
	
	// 퀘스트 마크
	m_QuestMarkType = NONE_QUEST_MARK;

	//최대 높이값
	m_fMaxHeight = 0;
	m_fMinHeight = 10000;
	m_nAddNum = 0;
	
	m_BIsBoundSet = FALSE;

	//...........................................................................................................
	// 부가 효과 By wxywxy
	//...........................................................................................................
	m_EffectColor	 = D3DCOLOR_XRGB( 0 , 0 , 0 );				// 이펙트 걸렸을때 색상값
	m_ISEffectColor	 = FALSE;									// 이펙트 칼라값 적용

	m_FaceExpTimer         = g_nowTime;
	m_pCurrentFaceExpData  = NULL;	
	m_NowFaceType          = CFaceExpData::FACE_EXP_NORMAL;

	//...........................................................................................................
	// 근접 무기 들었을때 플래그들 By wxywxy
	//...........................................................................................................
	m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
	m_AttackMotion	   = 0;				// 아이템 공격모션 번호

	//..........................................................................................
	// 아이템 착용 상태 
	//..........................................................................................
	m_Char_WearType = CHAR_WEAR_NORMAL;

	m_KeepUpChecker.Clear();
	
	
	//..........................................................................................
	// 펫 관련
	//..........................................................................................
	m_pCurrentState = NULL;

	dDestCell = 0;
	m_IsDefenceBack = false;
	m_IsOwnerBack = false;
	theAttackedID = 0;
	dAttackDestCell = 0;
	
	m_TempIndex = -1;

	//..........................................................................................
	// 라이딩 관련
	//..........................................................................................	
	m_fAddHeight = 0;
	m_fAddRideHeight = 0.0f;
	m_fAddForward = 0.0f;
	
	m_lPreTime = g_nowTime;
	m_lLevelUpTime = g_nowTime;
	m_lModifyHpTime = g_nowTime;
	
	m_uShadowSizeType = NSHADOW_SIZE_128;
	m_nShadowSize = 128;
	
	m_IsRealDie = FALSE;
	m_aAttackType = 0;
	m_aAttackID = 0;

	m_IsApplyConAniSkill = FALSE;
	m_aConAniTime = 30;

	m_IsRenderImg = FALSE;
	
	m_IsNextApplyVA = FALSE;

	m_IsInitUpdate = FALSE;
	m_IsApplyVA = FALSE;
	m_nPrevVATime = g_nowTime;
	m_nDelVATime = 1;
	
	m_nCamDistance = 0;
	
	m_IsAllLTexture = TRUE;
	
	m_IsSkillNoAni = FALSE;
	
	return;
}

void Character::SetCamDistance(int nDis)
{
	if(m_bPC)
	{
		if( g_Pc.GetPlayer() == this )
		{
			m_nCamDistance = nDis;
			int DelVATime = (long)(nDis + 20) * 0.25f;
			m_nDelVATime = max(30, DelVATime);

			if( m_curt_event.type == SM_ANI_SQUENCE )
			{
				m_nDelVATime = 1;
			}
		}
		else
		{
			m_nCamDistance = nDis;
			int DelVATime = (long)(nDis + 20) * 0.25f;
			m_nDelVATime = max(30, DelVATime);
		}
		
		if(m_State.bRide)
		{
			g_Map.m_Npc[m_State.TransIndex].m_nCamDistance = m_nCamDistance;
			g_Map.m_Npc[m_State.TransIndex].m_nDelVATime = m_nDelVATime;	
		}
	}
	else
	{
		if( m_State.nRideState == nRideState_Ride )
		{
			return;
		}

		m_nCamDistance = nDis;
		int DelVATime = (long)(nDis + 20) * 0.25f;
		m_nDelVATime = max(30, DelVATime);
	}
}

void	Character::Free()
{
	m_pCurrentState = NULL;
	
	SAFE_DELETE( theFindWay );
	SAFE_DELETE( m_pUserInfo );
	SAFE_DELETE( m_pFMove );

	// 초기화
	Init();
}


//-----------------------------------------------------------------------------
// Desc: 텍스쳐검사
//-----------------------------------------------------------------------------
void Character::UpdateTexture()
{
	//-----------------------------------------------------------------------------	
	// 보이는지 검사
	//-----------------------------------------------------------------------------	
	if( m_bIsVisible == false )
		return;

	//-----------------------------------------------------------------------------	
	// 캐릭터몸통
	//-----------------------------------------------------------------------------	
//	int itm_index, tex_index;	

	int itm_count = 0;

	itm_count = g_Pc_Manager.GetItmCount( this );
	int i = 0;
	for(  i = 0; i < itm_count; ++i )
	{		
		// 텍스쳐인덱스
		int tex_index = g_Pc_Manager.GetTexIndex( this, i );
		if( tex_index == -1 )
			continue;

		// 텍스쳐인덱스가 배열크기안에 있나?
//		if( tex_index < CHR_TEX_COUNT+NPC_TEX_COUNT )	
		if( tex_index < c_Chr_Tex_Count+c_Npc_Tex_Count )				
		{
			BYTE _bType = (BYTE)m_bPC;		
			if(m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC)	
			{
				_bType = (BYTE)true;
			}
			
			g_TexManager.SetTextureIndex( _bType, tex_index );
		}
	}	
	//-----------------------------------------------------------------------------	
	// 아이템종류
	//-----------------------------------------------------------------------------	
	if( m_bPC == true || (m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC) )
	{
		BYTE _bType = (BYTE)m_bPC;		
		if(m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC)	
		{
			_bType = (BYTE)true;
		}
		
		for( i = 0; i < LINK_ITEM_COUNT; ++i )
		{
			if( m_pUserInfo->ArmorInfo.index[i] != -1 )
			{						
				int tex_index = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[i]]->m_tex_index;				
				g_TexManager.SetTextureIndex( _bType, tex_index );
			}
		}
	}
}

void Character::AddMatPosMatPos(D3DXMATRIX &matOut, D3DXMATRIX &mat1, D3DXMATRIX &mat2)
{
	D3DXQUATERNION	qtRot;
	D3DXMATRIX	matTemp;
	
	matOut = mat1;	
	//-----------------------------------------------------------------------------
	// 오브젝트 회전한 만큼 메트릭스 회전하기
	//-----------------------------------------------------------------------------
	D3DXQuaternionRotationMatrix( &qtRot, &mat2 );	
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );	
	//-----------------------------------------------------------------------------
	// 월드상의 위치값 더하기
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matOut, &matOut, &matTemp );
	
	matOut._41 += mat2._41;
	matOut._42 += mat2._42;
	matOut._43 += mat2._43;
}

DWORD Character::FindFrontierID()
{
	DWORD FrontierID = 0;
	
	if(m_bPC)
	{	
		if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			if( nRui->GetPcInvenInfo()->FrontierMyInfo )
			{
				FrontierID = nRui->GetPcInvenInfo()->FrontierMyInfo->FrontierID;
			}
		}
		else
		{
			FrontierID = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theFrontierID;
		}		
	}
	else
	{
		if(MAP_TYPE_COLONY == g_Map.m_uMapType)
		{
			DWORD dNPCCode = (DWORD)g_Map.m_NpcTable[m_nTableIndex].code;
			///-- 이부분은 NPC데이터 성별부분을 보고 수성과 공성을 구분한다.
		}
	}

	return FrontierID;
}

void Character::ApplyIItemSlotAndFounded(int idx, BOOL * ISEmblemSlot, BOOL & ISEmblemWearFounded)
{
	static WORD ItemCode = 0;

	if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{

		if( idx == n_WearSlot_CoatArmor )
		{
			bool IsWearItem = false; 

			if(g_RockClient.m_PcInven->WearItem[ idx ])
			{
				if( g_RockClient.m_PcInven->WearItem[ n_WearSlot_SKIN_Suit ] )
				{
					ItemCode = g_RockClient.m_PcInven->WearItem[ n_WearSlot_SKIN_Suit ]->Code;
					IsWearItem = true; 
				}
				
			}
			else
			{
				if( g_RockClient.m_PcInven->WearItem[ n_WearSlot_SKIN_Suit ] )
				{
					ItemCode = g_RockClient.m_PcInven->WearItem[ n_WearSlot_SKIN_Suit ]->Code;
					IsWearItem = true;
				}
			}
			
			if( IsWearItem )
			{
				if( ISEmblemWear( ItemCode ) )
				{	
					ISEmblemSlot[ 1 ] = TRUE;							
					ISEmblemWearFounded = TRUE;
				}

				return;
			}
		}

		if( g_RockClient.m_PcInven->WearItem[ idx ] )
		{
			ItemCode = g_RockClient.m_PcInven->WearItem[ idx ]->Code;

			if( ISEmblemWear( ItemCode ) )
			{	
				ISEmblemSlot[ 1 ] = TRUE;							
				ISEmblemWearFounded = TRUE;
			}
		}
	}
	else
	{
		ItemCode = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theWearItem[ idx ].theCode;
				
		if( ISEmblemWear( ItemCode ) )
		{	
			ISEmblemSlot[ 1 ] = TRUE;							
			ISEmblemWearFounded = TRUE;
		}
	}
}

void Character::GetXAndYRadian(float & fXRadian, float & fYRadian, D3DXVECTOR3 vStartPos, D3DXVECTOR3 vDestPos)
{
	
	/// 시작점과 도착점의 거리체크
	float fDistance = D3DXVec3Length( &(vDestPos - vStartPos) );
	if(fDistance < 0.1f)
	{
		return;	
	}	
	
	D3DXVECTOR3 vDir1, vDir2;	
	
	D3DXVECTOR3 vXDestPos = vDestPos;
	vXDestPos.y = vStartPos.y;
	vDir1 = vXDestPos - vStartPos;
	D3DXVec3Normalize( &vDir1 , &vDir1 );
	
	vDir2 = vDestPos - vStartPos;
	D3DXVec3Normalize( &vDir2 , &vDir2 );

	if(ISZERO(vDir1.y - vDir2.y))
	{
		fXRadian = 0.0f;
		return;
	}

	fXRadian = D3DXVec3Dot( &vDir1 , &vDir2 );	
	fXRadian = acos((double)fXRadian);

	if(vDir2.y < 0.0f)
	{
	    fXRadian = (2.0f * D3DX_PI) - fXRadian;
	}
	
	vDir1 = vDestPos - vStartPos;
	vDir1.y = 0.0f;
	D3DXVec3Normalize(&vDir1, &vDir1);

	if(ISZERO(vDir1.z - (-1.0f)))
	{
		fYRadian = 0.0f;
		return;
	}

	fYRadian = D3DXVec3Dot( &vDir1 , &D3DXVECTOR3(0.0f, 0.0f, -1.0f) );
	fYRadian = acos((double)fYRadian);

	if(vDir1.x > 0.0f)
	{
	    fYRadian = (2.0f * D3DX_PI) - fYRadian;
	}
}

//-----------------------------------------------------------------------------
// Desc: 애니메이션 처리 & 버퍼계산
//-----------------------------------------------------------------------------
bool Character::AniRender(bool bFlag)
{					
	if( bFlag == false && m_State.nRideState == nRideState_Ride )
	{
		return false;
	}

	// 애니번호 없거나 버퍼 NULL이면	
	if( -1 == m_ani_number || NULL == m_pVtxBuffer[0].NormalVB )
		return false;	
	
    m_next_frame = 0;
	m_curt_frame = 0;
	
	float factor = 0.0f;
	bool bMotionBlend = true;
	
	int MotionBlendFrame = 0;
	int MotionBlendHalfFrame = 0;

	if( m_start_blend_frame >= (m_end_frame) )
	{
		m_start_blend_frame = m_end_frame - c_MotionBlendHalfFrame;
	}

	MotionBlendFrame = c_MotionBlendFrame + m_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + m_start_blend_frame;

	///--  현재 블렌딩 도중이라면....
	if( m_blend_frame < c_MotionBlendFrame )
	{
		m_curt_frame = m_blend_frame + m_start_blend_frame;
		m_next_frame = m_blend_frame + m_start_blend_frame + 1;

		if( m_curt_frame >= MotionBlendFrame )
		{
			m_pre_ani_number = -1;
			m_pre_end_frame = 0;
		}	

		if( m_ani_frame >= MotionBlendHalfFrame)		// 이어질 모션을 해당 프레임까지만 블렌딩 시킨다 
		{
			m_ani_frame = MotionBlendHalfFrame;
		}
	}
	///-- 블렌딩이 종료되었다면...
	else										// 블렌딩 끝났으니 실제 애니메이션 플레이...
	{
		m_curt_frame = m_ani_frame;		
		m_next_frame = m_ani_frame + 1;

		m_pre_ani_number = -1;					// 블렌딩 되었던 전 애니메이션 초기화 
		m_pre_end_frame = 0;
		
		m_start_blend_frame = 0;

		m_blend_frame = c_MotionBlendFrame;		// 한번 블렌딩 했으면 다음번까지 유지...
	}
	
	if( m_curt_frame >= (m_end_frame) )
	{
		m_curt_frame = m_end_frame - 1;
	}
	
	if( m_next_frame >= (m_end_frame) )
	{
		m_next_frame = 0;
	}
	
	if( m_curt_frame == m_next_frame )
	{
		m_middle_time = 0.0f;
	}

	int	ani, pre, mod, id;
	ani = m_ani_number;
	pre = m_pre_ani_number;
	
	// 이전애니메이션 없다
	if( pre == -1 )
	{
		pre = ani;
		bMotionBlend = false;		
	}
	else
	{
		factor = (float)(m_curt_frame - m_start_blend_frame)
				  / (MotionBlendFrame - m_start_blend_frame);
	}
	
	D3DXMATRIX matTemp;
	D3DXQUATERNION qtRot;		
	int	nItmCount;

	nItmCount = g_Pc_Manager.GetItmCount( this );

	D3DXVECTOR3	minVec = D3DXVECTOR3( 100.0f, 100.0f, 100.0f );	
	D3DXVECTOR3	maxVec = D3DXVECTOR3( -100.0f, -100.0f, -100.0f );	
	
	//배열 영역에서 벗어나면 by simwoosung
	if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return false;
	}

	if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return false;
	}	
	
	if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
		( NULL == g_Pc_Manager.m_pAni[pre] ) )
	{
		return false;			
	}

	if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) ||
		( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) )
	{
		return false;
	}
	
	//by simwoosung - 버텍스 버퍼 초기화 여부
	bool bVtxBufferIsInit = true;	

	m_IsApplyVA = FALSE;
	
	if( m_IsInitUpdate )
	{
		if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , m_nPrevVATime ) , >= , m_nDelVATime ) )
		{
			m_nPrevVATime = g_nowTime;
			m_IsApplyVA = TRUE;
		}
	}
	else
	{
		m_IsInitUpdate = TRUE;
		m_nPrevVATime = g_nowTime;
		m_IsApplyVA = TRUE;
	}
	
	if( m_IsRenderImg )
	{
		m_nPrevVATime = g_nowTime;
		m_IsApplyVA = TRUE;
	}

	if(m_IsNextApplyVA)
	{
		m_nPrevVATime = g_nowTime;
		m_IsApplyVA = TRUE;
		m_IsNextApplyVA = FALSE;
	}

	///--float fLeng = 0.0f;
	
	for( int i = 0; i < nItmCount; ++i )
	{			
		if( !m_IsApplyVA )
		{
			break;
		}		
		
		if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			&& m_Basic_DrawItm[i] )
		{
			mod = g_Pc_Manager.GetBasicModIndex( this, i );
			if( -1 ==  mod )
			{
				continue;
			}

			for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
			{		
				id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;

				if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id || g_Pc_Manager.m_pAni[pre]->m_num_node <= id )
					continue;
				
				if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation ) || 
					( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation ) )
				{
					return false;
				}
				
				if( bMotionBlend )
				{
					blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
					
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z;
				}
				else
				{
					D3DXQuaternionSlerp( &qtRot, 
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_qtRot,
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_qtRot,
										m_middle_time );		
					
					// 쿼터니온 -> 메트릭스
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.x -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.y -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.z -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z;
				}
				
				//-----------------------------------------------------------------------------
				// 버텍스 x 메트릭스 = 애니 버텍스
				//-----------------------------------------------------------------------------
				D3DXVec3TransformCoord( &m_Basic_VtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
			
			}
		}			

		if( ( m_pVtxBuffer[i].bInit == false ) && ( n_Wear_Serve != i ) )
		{
			//..........................................................................................
			// 셋트 아이템 하의 부츠 장갑 스키닝 애니메이션 막기 by simwoosung			// 
			//..........................................................................................
			if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				&& ( CHAR_WEAR_SETITEM == m_Char_WearType ) )
			{
				if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
				{					
					continue;			
				}
			}
		
			bVtxBufferIsInit = false;		
		}

		if( m_pItm_index[i]	== -1 )
			continue;
		
		//-----------------------------------------------------------------------------
		// 모델 인덱스
		//-----------------------------------------------------------------------------		
		mod = g_Pc_Manager.GetModIndex( this, i );
		
		if( -1 ==  mod )
		{
			continue;
		}

		// 버텍스 갯수만큼 루프돌기
		for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
		{				
			//-----------------------------------------------------------------------------
			// 뼈대에 연결된 ID
			//-----------------------------------------------------------------------------
			id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;

			//배열 영역에서 벗어나면 by simwoosung
			if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
			{
				return false;
			}

			if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
			{
				return false;
			}
			
			//링크 포인터값이 NULL 이면 by simwoosung
			if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
				( NULL == g_Pc_Manager.m_pAni[pre] ) )
			{
				return false;
			}
			
			if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) || 
				( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) )
			{
				return false;
			}

			//배열 영역 체크
			if( ( id < 0 ) || ( g_Pc_Manager.m_pAni[pre]->m_num_node <= id ) ||
	            ( g_Pc_Manager.m_pAni[ani]->m_num_node <= id )  )
			{
				return false;
			}

			if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id || g_Pc_Manager.m_pAni[pre]->m_num_node <= id )
			{
				continue;
			}
			
			if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation ) || 
				( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation ) )
			{
				return false;
			}


			// 쿼터니온 중간값 구하기			
			
			if( bMotionBlend )
			{
				blendQuaternion( &qtRot, factor, 
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_qtRot, 
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );

				// 쿼터니온 -> 메트릭스
				D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
				
				//-----------------------------------------------------------------------------
				// 위치값
				//-----------------------------------------------------------------------------
				matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x;

				matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y;

				matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z;
			}
			else
			{
				D3DXQuaternionSlerp( &qtRot, 
									 &g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_qtRot,
									 &g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_qtRot,
									 m_middle_time );		

				// 쿼터니온 -> 메트릭스
				D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
				
				//-----------------------------------------------------------------------------
				// 위치값
				//-----------------------------------------------------------------------------
				matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.x -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x ) * m_middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x;
				
				matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.y -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y ) * m_middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y;
				
				matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.z -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z ) * m_middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z;
			}
			
			//-----------------------------------------------------------------------------
			// 버텍스 x 메트릭스 = 애니 버텍스
			//-----------------------------------------------------------------------------
			D3DXVec3TransformCoord( &m_pVtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
			
			if( minVec.x > m_pVtxBuffer[i].pVertex[j].x )
			{
				minVec.x = m_pVtxBuffer[i].pVertex[j].x;
			}
			if( minVec.y > m_pVtxBuffer[i].pVertex[j].y )
			{
				minVec.y = m_pVtxBuffer[i].pVertex[j].y;
			}
			if( minVec.z > m_pVtxBuffer[i].pVertex[j].z )
			{
				minVec.z = m_pVtxBuffer[i].pVertex[j].z;
			}

			if( maxVec.x < m_pVtxBuffer[i].pVertex[j].x )
			{
				maxVec.x = m_pVtxBuffer[i].pVertex[j].x;
			}
			if( maxVec.y < m_pVtxBuffer[i].pVertex[j].y )
			{
				maxVec.y = m_pVtxBuffer[i].pVertex[j].y;
			}
			if( maxVec.z < m_pVtxBuffer[i].pVertex[j].z )
			{
				maxVec.z = m_pVtxBuffer[i].pVertex[j].z;
			}			
		}
	}

	if( m_State.nRideState == nRideState_Ride )
	{
		for( int i = 0; i < c_Max_RideItemType ; ++i )
		{
			if( !m_IsApplyVA )
			{
				break;
			}		
			
			if( m_Ride_Item_index[i] == -1 )
			 continue;
			
			mod = g_Pc_Manager.GetRideModIndex( this, i );

			if( -1 ==  mod )
			{
				continue;
			}
			
			for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
			{		
				id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;

				if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id || g_Pc_Manager.m_pAni[pre]->m_num_node <= id )
					continue;
				
				if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation ) || 
					( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation ) )
				{
					return false;
				}
				
				if( bMotionBlend )
				{
					blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
					
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z;
				}
				else
				{
					D3DXQuaternionSlerp( &qtRot, 
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_qtRot,
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_qtRot,
										m_middle_time );		
					
					// 쿼터니온 -> 메트릭스
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.x -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.y -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_next_frame].m_vPos.z -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z ) * m_middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[m_curt_frame].m_vPos.z;
				}
				
				//-----------------------------------------------------------------------------
				// 버텍스 x 메트릭스 = 애니 버텍스
				//-----------------------------------------------------------------------------
				D3DXVec3TransformCoord( &m_Ride_VtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
			}
		}
	}

	D3DXMATRIX matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 
	
	if(m_IsApplyVA)
	{
		D3DXVec3TransformCoord(&m_MaxBoundVec, &maxVec, &matScale);
		D3DXVec3TransformCoord(&m_MinBoundVec, &minVec, &matScale);
	}

	if( m_BIsBoundSet == FALSE )
	{
		//버텍스 버퍼가 초기화 될 경우
		if( m_IsRenderImg || bVtxBufferIsInit == true )
		{
			m_fMaxHeight = m_MaxBoundVec.y + 3.5f;
			m_fMinHeight = m_MinBoundVec.y - 1.0f;
			m_BIsBoundSet = TRUE;
		}
	}
	else
	{
		//모션이 해당 상황일때는 빌보드의 높이를 조정한다.
		if( m_curt_event.type == NULL )		
		{
			if( (m_fMaxHeight < m_MaxBoundVec.y) && 
				(m_MaxBoundVec.y < (m_fMaxHeight * 3.0f) ) )	
			{
			   m_fMaxHeight = m_MaxBoundVec.y + 3.5f;
			}
			
			if( m_fMinHeight > m_MinBoundVec.y )
			{
			   m_fMinHeight = m_MinBoundVec.y - 1.0f;
			}
		}
		else if( m_curt_event.status == SM_RESTSKILL_SITDOWN  ||
				 m_curt_event.status == SM_RESTSKILL_KEEPING  ||
				 m_curt_event.status == SM_RESTSKILL_STANDUP  ||
				 m_curt_event.status == SM_SELLSKILL_SITDOWN  ||	
				 m_curt_event.status == SM_SELLSKILL_KEEPING  ||	
				 m_curt_event.status == SM_SELLSKILL_STANDUP    )
		{
			m_fMaxHeight = m_MaxBoundVec.y + 3.5f;
			m_fMinHeight = m_MinBoundVec.y - 1.0f;
		}
	}

	//...........................................................................................................
	// 거대 몬스터 By wxywxy
	//...........................................................................................................
	if( c_NotMoveNPCCode_Min <= g_Map.m_NpcTable[ m_nTableIndex ].code &&
		c_NotMoveNPCCode_Max >= g_Map.m_NpcTable[ m_nTableIndex ].code &&
		!m_bPC )
	{
		if( g_Map.m_NpcTable[ m_nTableIndex ].code == 3800 ) 
		{
			m_MinBoundVec   *= 0.2f;
			m_MaxBoundVec.x *= 0.2f;
			m_MaxBoundVec.z *= 0.2f;
			m_MaxBoundVec.y *= 0.7f;

		}
		else if( g_Map.m_NpcTable[ m_nTableIndex ].code == 3801 )
		{
			m_MinBoundVec   *= 0.4f;
			m_MaxBoundVec.x *= 0.4f;
			m_MaxBoundVec.z *= 0.4f;
		}
		else
		{
			m_MinBoundVec *= 0.4f;
			m_MaxBoundVec *= 0.4f;
		}
	}

	// 바운딩 박스 새로 설정 
	g_Pc_Manager.SetMinBoundVec( this, m_MinBoundVec );
	g_Pc_Manager.SetMaxBoundVec( this, m_MaxBoundVec );			

#ifdef BOUND_BOX_DISPLAY
	
	if( m_CollisionBox.m_nVtxCount )
	{
		m_CollisionBox.Update( minVec, maxVec );
	}
	
#endif
	

	//-----------------------------------------------------------------------------
	// 링크되는 본 구하기
	//-----------------------------------------------------------------------------

	if(m_State.bRide)
	{
		g_Map.m_Npc[m_State.TransIndex].AniRender(true);
		
		m_matRideChr = m_matChr;
		
		D3DXVECTOR3 vTempPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vTempPosition.x = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._41;
		vTempPosition.y = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._42 + m_fAddHeight;
		vTempPosition.z = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._43;	
		
		D3DXVECTOR3 vForW = m_vDir * m_fAddForward;

		vTempPosition += vForW;

		m_matRideChr._41 = vTempPosition.x;
		m_matRideChr._42 = vTempPosition.y;
		m_matRideChr._43 = vTempPosition.z;
		
		if( ISZERO(m_fAddRideHeight) && m_BIsBoundSet && 
			g_Map.m_Npc[m_State.TransIndex].m_BIsBoundSet )
		{
			SNPCBaseInfo* npc_Info = NULL;
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[m_State.TransIndex].code );			
			m_fAddHeight = (float)( (npc_Info->theAttackRange) / 10.0f);

			m_fAddForward = (float)( ( (int)npc_Info->theLuck - 25 ) / 10.0f);

			m_matRideChr._42 = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._42 + m_fAddHeight;
			m_fAddRideHeight = m_matRideChr._42 - m_matChr._42 - 10.0f;
			
			if( m_nTableIndex == MAX_USER_INTABLE )
			{
				g_Camera.Update();
				g_Camera.MovCamera();
			}
		}
		
	}
	else
	{
		m_fAddRideHeight = 0.0f;
		m_fAddHeight = 0.0f;
		m_fAddForward = 0.0f;
	}
	
	CalLinkBone(ani,pre,m_curt_frame,m_next_frame,MotionBlendHalfFrame,factor,bMotionBlend);	

	//-----------------------------------------------------------------------------
	// 무기, 방패 애니메이션(현재 지원하는 링크갯수 4개)
	//-----------------------------------------------------------------------------
	if( ( m_bPC == true || ( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) )
	{
		//-----------------------------------------------------------------------------
		// 무기에 사용
		//-----------------------------------------------------------------------------
		D3DXMATRIX	matRot;

		///-- 오른손 부분은 반드시 구한다.
		ItemAnimation( 0 , false, m_next_frame, m_curt_frame );
		
		for(int i = 1; i < LINK_ITEM_COUNT; ++i )
		{			
			int itm_index = m_pUserInfo->ArmorInfo.index[i];
			
			BOOL IsGetAni = FALSE;
			if( itm_index != -1 )
			{
				IsGetAni = TRUE;
			}
			
			if( IsGetAni )
			{
				ItemAnimation( i, false, m_next_frame, m_curt_frame );
			}
		}
	
		CalWeaponAxis();	// 칼 축 처리
	}	

	return true;
}

bool Character::BoneRender()
{
	// 애니번호 없거나 버퍼 NULL이면	
	if( -1 == m_ani_number || NULL == m_pVtxBuffer[0].NormalVB )
		return false;	
	
    int next_frame = 0;
	int curt_frame = 0;

	int pre_ani_number = m_pre_ani_number;
	int pre_end_frame = 0;
	
	float factor = 0.0f;
	bool bMotionBlend = true;
	
	int MotionBlendFrame = 0;
	int MotionBlendHalfFrame = 0;

	MotionBlendFrame = c_MotionBlendFrame + m_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + m_start_blend_frame;	

	///--  현재 블렌딩 도중이라면....
	if( m_blend_frame < c_MotionBlendFrame )
	{
		curt_frame = m_blend_frame + m_start_blend_frame;
		next_frame = m_blend_frame + m_start_blend_frame + 1;

		if( curt_frame >= MotionBlendFrame )
		{
			pre_ani_number = -1;
			pre_end_frame = 0;
		}
	}
	///-- 블렌딩이 종료되었다면...
	else										// 블렌딩 끝났으니 실제 애니메이션 플레이...
	{
		curt_frame = m_ani_frame;		
		next_frame = m_ani_frame + 1;

		pre_ani_number = -1;	
	}
	
	if( next_frame >= (m_end_frame) )
	{
		next_frame = m_end_frame;
	}

	int	ani, pre;
	ani = m_ani_number;
	pre = pre_ani_number;
	
	// 이전애니메이션 없다
	if( pre == -1 )
	{
		pre = ani;
		bMotionBlend = false;		
	}
	else
	{
		factor = (float)(curt_frame - m_start_blend_frame)
				  / (MotionBlendFrame - m_start_blend_frame);
	}
	
	D3DXMATRIX matTemp;
	D3DXQUATERNION qtRot;		
	int	nItmCount;

	//배열 영역에서 벗어나면 by simwoosung
	if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return false;
	}

	if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return false;
	}

	nItmCount = g_Pc_Manager.GetItmCount( this );
	
	if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
		( NULL == g_Pc_Manager.m_pAni[pre] ) )
	{
		return false;			
	}

	if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) ||
		( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) )
	{
		return false;
	}

	CalLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend);

	//-----------------------------------------------------------------------------
	// 무기, 방패 애니메이션(현재 지원하는 링크갯수 4개)
	//-----------------------------------------------------------------------------
	if( m_bPC == true || 
		( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		//-----------------------------------------------------------------------------
		// 무기에 사용
		//-----------------------------------------------------------------------------
		D3DXMATRIX	matRot;

		///-- 오른손 부분은 반드시 구한다.
		ItemAnimation( 0 , false, next_frame, curt_frame );
		
		for(int i = 1; i < LINK_ITEM_COUNT; ++i )
		{			
			int itm_index = m_pUserInfo->ArmorInfo.index[i];
			
			if( itm_index != -1 )
			{
				ItemAnimation( i, false, next_frame, curt_frame );
			}
		}
	
		CalWeaponAxis();	// 칼 축 처리
	}


	if(m_State.bRide)
	{
		g_Map.m_Npc[m_State.TransIndex].BoneRender();		
		m_matRideChr = m_matChr;

		m_matRideChr._41 = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._41;
		m_matRideChr._42 = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._42 + m_fAddHeight;
		m_matRideChr._43 = g_Map.m_Npc[m_State.TransIndex].m_matRealCenter._43;
	}

	return true;
}

//.............................................................................................................
// 본 매트릭스 구하기 By wxywxy
//.............................................................................................................
D3DXMATRIX  Character::GetLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id, bool bRide )
{
	D3DXMATRIX	   matTemp;
	D3DXMATRIX	   matScale;

	D3DXMatrixIdentity( &matTemp );

	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 

	//배열 영역에서 벗어나면 by simwoosung
	if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return matTemp;
	}

	if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return matTemp;
	}
	
	if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
		( NULL == g_Pc_Manager.m_pAni[pre] ) )
	{
		return matTemp;
	}

	if( ( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) || 
		( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) )
	{
		return matTemp;
	}

	//.........................................................................................................
	// 본 정보 없는 것들 ( NPC 쪽에서 )
	//.........................................................................................................
	if( -1 == id )
	{
//		RLGS( "Error Character::GetLinkBone -> id = -1\n" );

		return matTemp;
	}

	//배열 영역 체크 - 배열 영역에서 벗어날 경우
	if( (id < 0)  || (g_Pc_Manager.m_pAni[pre]->m_num_node <= id) ||
		  (g_Pc_Manager.m_pAni[ani]->m_num_node <= id)   )
	{
		if( m_bPC || 
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )  )
		{
//			char TempStr[ 1024 ] = "";
//			wsprintf( TempStr , "Error Character::GetLinkBone -> %d [ pre( %d ) - ( %d ) ]\n" , 
//				id , g_Pc_Manager.m_pAni[pre]->m_num_node , g_Pc_Manager.m_pAni[ani]->m_num_node );
//			RLGS( TempStr );
		}
		else
		{
			SNPCBaseInfo* npc_Info;
			
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ m_nTableIndex ].code ); 
			
			if( npc_Info != NULL )
			{
//				DebugPrint( "GetLinkBone : Code : %d - Bone : %d ( Name : %s - Max bone : %d  )\n" , 
//					g_Map.m_NpcTable[ m_nTableIndex ].code ,
//					id , 
//					g_Map.m_NpcTable[ m_nTableIndex ].szName ,
//					g_Pc_Manager.m_pAni[pre]->m_num_node );
			}

		}
	
		return matTemp;
	}

	int   MotionBlendFrame     = 0;
	
	MotionBlendFrame = c_MotionBlendFrame + m_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + m_start_blend_frame;
	
	int ani_num				   = ani;
	int pre_ani_num			   = pre;

	D3DXQUATERNION	qtRot;

	if( pre_ani_num == -1 )
	{
		pre_ani_num  = ani_num;
		bMotionBlend = false;
	}
	else
	{
		factor = (float) (curt_frame - m_start_blend_frame) 
			      / (MotionBlendFrame - m_start_blend_frame);
	}

	if( g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node <= id || 
		g_Pc_Manager.m_pAni[ani_num]->m_num_node <= id )
	{
		return matTemp;
	}

	if( ( NULL == g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation ) || 
		( NULL == g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation ) )
	{
		return matTemp;
	}
	
	if( bMotionBlend )
	{
		if( m_pre_end_frame < 0 || g_Pc_Manager.m_pAni[pre_ani_num]->m_ani_frame <= m_pre_end_frame )
		{
			return matTemp;
		}		
		if( MotionBlendHalfFrame < 0 || g_Pc_Manager.m_pAni[ani_num]->m_ani_frame <= MotionBlendHalfFrame )
		{
			return matTemp;
		}
		
		blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
	}
	else
	{
		if( curt_frame < 0 || g_Pc_Manager.m_pAni[ani_num]->m_ani_frame <= curt_frame )
		{
			return matTemp;
		}

		if( next_frame < 0 || g_Pc_Manager.m_pAni[ani_num]->m_ani_frame <= next_frame )
		{
			return matTemp;
		}
		
		// 쿼터니온 중간값 구하기
		D3DXQuaternionSlerp( &qtRot, 
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_qtRot,
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_qtRot,
							 m_middle_time );
	}

	// 쿼터니온 -> 메트릭스 ( 현재 회전각 알아내기 )
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );		

	//-----------------------------------------------------------------------------
	// 현재 동작의 로칼메트릭스
	//-----------------------------------------------------------------------------	
	{
		if(  m_bPC == true ||
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
		{
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, id ), &matTemp );
		}
		else		
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, id ), &matTemp );
	}	

	if( bMotionBlend )
	{	
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x;
		
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y;
		
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z;
	}
	else
	{
		//-----------------------------------------------------------------------------
		// 위치값
		//-----------------------------------------------------------------------------
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.x -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x;			 
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.y -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y;
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.z -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z;
	}
	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );

	return matTemp;
}

D3DXMATRIX  Character::GetRideLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend,int id)
{
	
	D3DXMATRIX	   matTemp;
	D3DXMATRIX	   matScale;

	D3DXMatrixIdentity( &matTemp );

	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 

	//배열 영역에서 벗어나면 by simwoosung
	if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return matTemp;
	}

	if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
	{
		return matTemp;
	}
	
	if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
		( NULL == g_Pc_Manager.m_pAni[pre] ) )
	{
		return matTemp;
	}

	if( ( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) || 
		( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) )
	{
		return matTemp;
	}

	//.........................................................................................................
	// 본 정보 없는 것들 ( NPC 쪽에서 )
	//.........................................................................................................
	if( -1 == id )
	{
//		RLGS( "Error Character::GetLinkBone -> id = -1\n" );

		return matTemp;
	}

	//배열 영역 체크 - 배열 영역에서 벗어날 경우
	if( (id < 0)  || (g_Pc_Manager.m_pAni[pre]->m_num_node <= id) ||
		  (g_Pc_Manager.m_pAni[ani]->m_num_node <= id)   )
	{
		if(  m_bPC || 
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
//			char TempStr[ 1024 ] = "";
//			wsprintf( TempStr , "Error Character::GetLinkBone -> %d [ pre( %d ) - ( %d ) ]\n" , 
//				id , g_Pc_Manager.m_pAni[pre]->m_num_node , g_Pc_Manager.m_pAni[ani]->m_num_node );
//			RLGS( TempStr );
		}
		else
		{
			SNPCBaseInfo* npc_Info;
			
			npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ m_nTableIndex ].code ); 
			
			if( npc_Info != NULL )
			{
//				DebugPrint( "GetLinkBone : Code : %d - Bone : %d ( Name : %s - Max bone : %d  )\n" , 
//					g_Map.m_NpcTable[ m_nTableIndex ].code ,
//					id , 
//					g_Map.m_NpcTable[ m_nTableIndex ].szName ,
//					g_Pc_Manager.m_pAni[pre]->m_num_node );
			}

		}
	
		return matTemp;
	}

	int   MotionBlendFrame     = 0;
	
	MotionBlendFrame           = c_MotionBlendFrame;
	MotionBlendHalfFrame	   = c_MotionBlendHalfFrame;
	
	int ani_num				   = ani;
	int pre_ani_num			   = pre;

	D3DXQUATERNION	qtRot;

	if( pre_ani_num == -1 )
	{
		pre_ani_num  = ani_num;
		bMotionBlend = false;
	}
	else
	{
		factor = (float) curt_frame / MotionBlendFrame;
	}

	
	if( bMotionBlend )
	{
		blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
	}
	else
	{
		// 쿼터니온 중간값 구하기
		D3DXQuaternionSlerp( &qtRot, 
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_qtRot,
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_qtRot,
							 m_middle_time );
	}

	// 쿼터니온 -> 메트릭스 ( 현재 회전각 알아내기 )
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );		

	//-----------------------------------------------------------------------------
	// 현재 동작의 로칼메트릭스
	//-----------------------------------------------------------------------------	
	{
		if(  m_bPC == true ||
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
		{
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, id ), &matTemp );
		}
		else		
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, id ), &matTemp );
	}	

	if( bMotionBlend )
	{	
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.x;
		
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.y;
		
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[id].m_pAnimation[m_pre_end_frame].m_vPos.z;
	}
	else
	{
		//-----------------------------------------------------------------------------
		// 위치값
		//-----------------------------------------------------------------------------
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.x -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x;			 
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.y -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y;
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[next_frame].m_vPos.z -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z;
	}
	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );

	return matTemp;
}

//-----------------------------------------------------------------------------
// Desc: 링크 처리 본 계산하기
//-----------------------------------------------------------------------------
void Character::CalLinkBone(int ani,int pre,int curt_frame,int next_frame,int MotionBlendHalfFrame,float factor,bool bMotionBlend)
{
	//.........................................................................................................
	// PC 일때 
	//.........................................................................................................
	if( m_bPC ||
		( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) 
	{	
		int AttrIndex = GetAttrIndex();
		if( AttrIndex == -1 )
		{
			return;
		}

		if( m_IsApplyVA )
		{
			m_matRealORHand  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_hand_node );
			m_matRealOLHand  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_hand_node );
			m_matRealObreast = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].breast_node );
			m_matRealOCenter = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].center_node );
			m_matRealOLFoot  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_foot_node );
			m_matRealORFoot  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_foot_node );
			m_matRealOHair   = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].hair_node   );
		}
		
		if(!m_State.bRide)
		{
			AddMatPosMatPos(m_matRealRHand, m_matRealORHand, m_matChr);
			AddMatPosMatPos(m_matRealLHand, m_matRealOLHand, m_matChr);
			AddMatPosMatPos(m_matRealbreast, m_matRealObreast, m_matChr);
			AddMatPosMatPos(m_matRealCenter, m_matRealOCenter, m_matChr);
			AddMatPosMatPos(m_matRealLFoot, m_matRealOLFoot, m_matChr);
			AddMatPosMatPos(m_matRealRFoot, m_matRealORFoot, m_matChr);
			AddMatPosMatPos(m_matRealHair, m_matRealOHair, m_matChr);			
		}
		else
		{
			AddMatPosMatPos(m_matRealRHand, m_matRealORHand, m_matRideChr);
			AddMatPosMatPos(m_matRealLHand, m_matRealOLHand, m_matRideChr);
			AddMatPosMatPos(m_matRealbreast, m_matRealObreast, m_matRideChr);
			AddMatPosMatPos(m_matRealCenter, m_matRealOCenter, m_matRideChr);
			AddMatPosMatPos(m_matRealLFoot, m_matRealOLFoot, m_matRideChr);
			AddMatPosMatPos(m_matRealRFoot, m_matRealORFoot, m_matRideChr);
			AddMatPosMatPos(m_matRealHair, m_matRealOHair, m_matRideChr);
		}

#ifdef DEFENSE_EFFECT

		if( m_IsApplyVA )
		{
			m_matRealOLShoulder	= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_shoulder_node   );
			m_matRealORShoulder	= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_shoulder_node   );
			m_matRealOLElbow		= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_elbow_node   ); 
			m_matRealORElbow		= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_elbow_node   );
			m_matRealOLKnee		= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_knee_node   ); 
			m_matRealORKnee		= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_knee_node   ); 
			m_matRealOLPelvis	= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].l_pelvis_node   ); 
			m_matRealORPelvis	= GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, g_Pc_Manager.m_PcAttr[ AttrIndex ].r_pelvis_node   );
		}
		
		if(!m_State.bRide)
		{
			AddMatPosMatPos(m_matRealLShoulder, m_matRealOLShoulder, m_matChr);
			AddMatPosMatPos(m_matRealRShoulder, m_matRealORShoulder, m_matChr);
			AddMatPosMatPos(m_matRealLElbow, m_matRealOLElbow, m_matChr);
			AddMatPosMatPos(m_matRealRElbow, m_matRealORElbow, m_matChr);
			AddMatPosMatPos(m_matRealLKnee, m_matRealOLKnee, m_matChr);
			AddMatPosMatPos(m_matRealRKnee, m_matRealORKnee, m_matChr);
			AddMatPosMatPos(m_matRealLPelvis, m_matRealOLPelvis, m_matChr);
			AddMatPosMatPos(m_matRealRPelvis, m_matRealORPelvis ,m_matChr);		
		}
		else
		{
			AddMatPosMatPos(m_matRealLShoulder, m_matRealOLShoulder, m_matRideChr);
			AddMatPosMatPos(m_matRealRShoulder, m_matRealORShoulder, m_matRideChr);
			AddMatPosMatPos(m_matRealLElbow, m_matRealOLElbow, m_matRideChr);
			AddMatPosMatPos(m_matRealRElbow, m_matRealORElbow, m_matRideChr);
			AddMatPosMatPos(m_matRealLKnee, m_matRealOLKnee, m_matRideChr);
			AddMatPosMatPos(m_matRealRKnee, m_matRealORKnee, m_matRideChr);
			AddMatPosMatPos(m_matRealLPelvis, m_matRealOLPelvis, m_matRideChr);
			AddMatPosMatPos(m_matRealRPelvis, m_matRealORPelvis ,m_matRideChr);	
		}
#endif 

	}
	else
	{
		SNPCBaseInfo* npc_Info;				
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ m_nTableIndex ].code ); 

		if( npc_Info == NULL )
			return;
		
		if( m_IsApplyVA )
		{		
			m_matRealORHand  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 2 ] );
			m_matRealOLHand  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 3 ] );
			m_matRealObreast = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 0 ] );
			m_matRealOCenter = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 6 ] );
			m_matRealOLFoot  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 5 ] );
			m_matRealORFoot  = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 4 ] );
			m_matRealOHair   = GetLinkBone(ani,pre,curt_frame,next_frame,MotionBlendHalfFrame,factor,bMotionBlend, npc_Info->theBorn[ 1 ] );
		}	
		
		if(!m_State.bRide)
		{
			AddMatPosMatPos(m_matRealRHand, m_matRealORHand, m_matChr);
			AddMatPosMatPos(m_matRealLHand, m_matRealOLHand, m_matChr);
			AddMatPosMatPos(m_matRealbreast, m_matRealObreast, m_matChr);
			AddMatPosMatPos(m_matRealCenter, m_matRealOCenter, m_matChr);
			AddMatPosMatPos(m_matRealLFoot, m_matRealOLFoot, m_matChr);
			AddMatPosMatPos(m_matRealRFoot, m_matRealORFoot, m_matChr);
			AddMatPosMatPos(m_matRealHair, m_matRealOHair, m_matChr);			
		}
		else
		{
			AddMatPosMatPos(m_matRealRHand, m_matRealORHand, m_matRideChr);
			AddMatPosMatPos(m_matRealLHand, m_matRealOLHand, m_matRideChr);
			AddMatPosMatPos(m_matRealbreast, m_matRealObreast, m_matRideChr);
			AddMatPosMatPos(m_matRealCenter, m_matRealOCenter, m_matRideChr);
			AddMatPosMatPos(m_matRealLFoot, m_matRealOLFoot, m_matRideChr);
			AddMatPosMatPos(m_matRealRFoot, m_matRealORFoot, m_matRideChr);
			AddMatPosMatPos(m_matRealHair, m_matRealOHair, m_matRideChr);
		}
	
	}

	// 이펙트에서 슬 중심점 구하기 ( 0 번본의 수직 으로 내려온 지형의 속성 높이값으로 )
	m_matBoneCenter = m_matRealCenter;
	m_matBoneCenter._42 = g_Map.Get__MapHeightF( m_matRealCenter._41 , m_matRealCenter._43 );
	
	if(m_State.bRide)
	{
		m_matRideCenter = m_matRealCenter;
		m_matRideCenter._42 -= 6.8f; 
	}
}

//-----------------------------------------------------------------------------
// Desc: 아이템처리 계산
//-----------------------------------------------------------------------------
void	Character::ItemAnimation( int type, bool bChange, int next_frame, int curt_frame )
{
	int	node;
	D3DXQUATERNION	qtRot;
	D3DXMATRIX		matReady;
	D3DXMATRIX		matTemp;
	D3DXMatrixIdentity( &matTemp  );
	D3DXMatrixIdentity( &matReady );
	
	if(m_IsApplyVA)
	{
		//....
		switch( type )
		{
			//......................................................................................................
			// 오른손
			//......................................................................................................
			case 0:
			{
				int	rhand_itm_index = m_pUserInfo->ArmorInfo.index[0];				
				int	lhand_itm_index = m_pUserInfo->ArmorInfo.index[1];				
				
				//		if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_sex == 0 )
				if( GetAttrIndex() == n_Millena_Man || GetAttrIndex() == n_Rain_Man )
				{
					if( rhand_itm_index== -1 )
					{
						m_pUserInfo->ArmorInfo.RHand = 45;
					}
					else
					{				
						if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 0 )
							m_pUserInfo->ArmorInfo.RHand = 45;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 1 )			
							m_pUserInfo->ArmorInfo.RHand = 45;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 2 )
							m_pUserInfo->ArmorInfo.RHand = 45;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 6 )
							m_pUserInfo->ArmorInfo.RHand = 45;			
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 7 )
							m_pUserInfo->ArmorInfo.RHand = 45; // 23;
					}
				}
				//		else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_sex == 1 )
				else if( GetAttrIndex() == n_Millena_Woman || GetAttrIndex() == n_Rain_Woman )
				{
					if( rhand_itm_index== -1 )
					{
						m_pUserInfo->ArmorInfo.RHand = 59;
					}
					else
					{					
						if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 0 )
							m_pUserInfo->ArmorInfo.RHand = 59;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 1 )			
							m_pUserInfo->ArmorInfo.RHand = 59;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 2 )
							m_pUserInfo->ArmorInfo.RHand = 59;
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 6 )
							m_pUserInfo->ArmorInfo.RHand = 59;			
						else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 7 )
							m_pUserInfo->ArmorInfo.RHand = 59; // 29;
					}
				}
			}
			break;

			case 1:
			{
				int	lhand_itm_index = m_pUserInfo->ArmorInfo.index[1];
				
				if(lhand_itm_index == -1)
				{
					return;
				}
				
				if( GetAttrIndex() == n_Millena_Man || GetAttrIndex() == n_Rain_Man )
				{
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 1 )	
					{					
						m_pUserInfo->ArmorInfo.LHand = 15; // 15;

						// 활일때는 오른손에 화살 본을 구한다
						///ItemAnimation( 0 , bChange, next_frame, curt_frame );
					}
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 3 )			
						m_pUserInfo->ArmorInfo.LHand = 14;
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 5 )			
						m_pUserInfo->ArmorInfo.LHand = 45; // 15;
				}
				else if( GetAttrIndex() == n_Millena_Woman || GetAttrIndex() == n_Rain_Woman )
				{
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 1 )			
					{
						m_pUserInfo->ArmorInfo.LHand = 21;

						// 활일때는 오른손에 화살 본을 구한다
						///ItemAnimation( 0 , bChange, next_frame, curt_frame );
					}
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 3 )			
						m_pUserInfo->ArmorInfo.LHand = 20;
					if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 5 )			
						m_pUserInfo->ArmorInfo.LHand = 59; // 21;
				}
			}
				break;
		}

		int AttrIndex = GetAttrIndex();
		if( AttrIndex == -1 )
		{
			return;
		}

		if(  m_bPC == true ||
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			switch( type )
			{		
			case 0:				
				node = m_pUserInfo->ArmorInfo.RHand;
				if( m_curt_event.type == SM_ACT)
				{
					if( m_curt_event.weaponView[0] )
					{
						node = g_Pc_Manager.m_PcAttr[ AttrIndex ].r_hand_node;
					}
				}		
				break;			
			case LINK_ITEM_COUNT:				
				node = m_pUserInfo->ArmorInfo.RHand;				
				break;
			case 1:				
				node = m_pUserInfo->ArmorInfo.LHand;
				if( m_curt_event.type == SM_ACT)
				{
					if( m_curt_event.weaponView[1] )
					{
						node = g_Pc_Manager.m_PcAttr[ AttrIndex ].l_hand_node;
					}
				}
				break;
			default:				
				node = type;				
			}
		}
		else
		{
			node = type;
		}

		float factor = 0.0f;
		bool bMotionBlend = true;

		int MotionBlendFrame = 0;
		int MotionBlendHalfFrame = 0;
		
	//	if( m_curt_event.type == SM_ATTACK_NPC )
		{
	//		MotionBlendFrame = c_MotionBlendFrame + c_MotionBlendHalfFrame;		// 앞, 위의 블렌딩 될 총 프레임 수...공격 모션의 경우 뒤의 모션은 c_MotionBlendHalfFrame으로 줄이고, 앞의 모션은 c_MotionBlendFrame으로 줄인다 
	//		MotionBlendHalfFrame = c_MotionBlendFrame;		
		}
	//	else
		{
			MotionBlendFrame = c_MotionBlendFrame + m_start_blend_frame;
			MotionBlendHalfFrame = c_MotionBlendHalfFrame + m_start_blend_frame;
		}

		int ani_num = m_ani_number;
		int pre_ani_num = m_pre_ani_number;
		
		// 이전애니메이션 없다
		if( pre_ani_num == -1 )
		{
			pre_ani_num = ani_num;
			bMotionBlend = false;		
		}
		else
		{
			factor = (float)(curt_frame - m_start_blend_frame) 
					  /(MotionBlendFrame - m_start_blend_frame);
		}
		
		if( g_Pc_Manager.m_pAni[ani_num] == NULL || g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node == NULL )
		{
			return;
		}

		//-----------------------------------------------------------------------------
		// 현재 노드 회전값
		//-----------------------------------------------------------------------------	
		if( node >= g_Pc_Manager.m_pAni[ani_num]->m_num_node || 
			node >= g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node )
			return;

		if( bMotionBlend )
		{
			blendQuaternion( &qtRot, factor, 
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_qtRot, 
							g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
		}
		else
		{
			// 쿼터니온 중간값 구하기
			D3DXQuaternionSlerp( &qtRot, 
								 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_qtRot,
								 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame ].m_qtRot,
								 m_middle_time );
		}

		// 쿼터니온 -> 메트릭스 ( 현재 회전각 알아내기 )
		D3DXMatrixRotationQuaternion( &matTemp, &qtRot );		

		//-----------------------------------------------------------------------------
		// 현재 동작의 로칼메트릭스
		//-----------------------------------------------------------------------------	
		{
			if( m_bPC == true ||
				( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
			{
				D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, node ), &matTemp );
			}
			else		
				D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, node ), &matTemp );		
		}

		if( bMotionBlend )
		{	
			matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.x;
			
			matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.y;
			
			matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
							g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[m_pre_end_frame].m_vPos.z;
		}
		else
		{
			//-----------------------------------------------------------------------------
			// 위치값
			//-----------------------------------------------------------------------------
			matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.x -
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.x ) * m_middle_time ) +
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.x;			 
			matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.y -
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.y ) * m_middle_time ) +
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.y;
			matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.z -
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.z ) * m_middle_time ) +
							 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.z;
		}		
		//-----------------------------------------------------------------------------
		// 스케일값 곱하기
		//-----------------------------------------------------------------------------
		D3DXMATRIX	matTemp2;
		D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
		D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
		
		//-----------------------------------------------------------------------------
		// 등에 차고 있다면.. 해당 지정값만큼 곱한다 
		//-----------------------------------------------------------------------------
		D3DXMatrixMultiply( &matTemp, &matReady, &matTemp );

		//-----------------------------------------------------------------------------
		// 노드메트릭스에 저장
		//-----------------------------------------------------------------------------
		if(  m_bPC == true ||
			( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			switch( type )
			{		
			case 0:			
				m_matORHand = matTemp;			
				break;
			case LINK_ITEM_COUNT:			
				m_matORHand2 = matTemp;			
				break;
			case 1:			
				m_matOLHand = matTemp;			
				break;
			default:			
				m_matORHand = matTemp;			
				break;
			}
		}
		else
		{
			m_matORHand = matTemp;
		}
	}
	
	AddMatPosMatPos(m_matRHand, m_matORHand, m_matChr);
	AddMatPosMatPos(m_matRHand2, m_matORHand2, m_matChr);	
	AddMatPosMatPos(m_matLHand, m_matOLHand, m_matChr);
}


//-----------------------------------------------------------------------------
// 머리 애니메이션
//-----------------------------------------------------------------------------
/*void	Character::SM__CHR__DIR( DWORD dir )
{
	m_pUserInfo->HeadPreFrame	= m_pUserInfo->HeadCurFrame;
	m_pUserInfo->HeadCurFrame	= dir;
	m_pUserInfo->HeadTime		= g_nowTime;
}
*/

D3DXMATRIX Character::HeadAnimation()
{		
	// 보간타임
	float mid_time;
	if( SAFE_TIME_COMPARE( SAFE_TIME_ADD( m_pUserInfo->HeadTime, HEAD_ROTATE_TIME ) , <= , g_nowTime ) )	
		mid_time = 1.0f;
	else
	{
		mid_time = SAFE_TIME_SUB( g_nowTime , m_pUserInfo->HeadTime ) / HEAD_ROTATE_TIME;
	}

	// 현재프레임 
	int		cur_frame	= m_pUserInfo->HeadCurFrame;
	int		pre_frame	= m_pUserInfo->HeadPreFrame;

	// 애니메이션 인덱스	
	int		ani_index;
	if( m_nTableIndex == MAX_USER_INTABLE )	
		ani_index	= g_Pc.m_MainPC.nIndex;		
	else	
		ani_index	= g_Pc.m_PcTable[m_nTableIndex].nIndex;	

	//-----------------------------------------------------------------------------
	// 쿼터니온 중간값 구하기		
	//-----------------------------------------------------------------------------
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot, 
						 &g_Pc_Manager.m_HeadAni[ani_index].m_pNode[HEAD_INDEX].m_pAnimation[pre_frame].m_qtRot,
						 &g_Pc_Manager.m_HeadAni[ani_index].m_pNode[HEAD_INDEX].m_pAnimation[cur_frame].m_qtRot,
						 mid_time );

	//-----------------------------------------------------------------------------
	// 쿼터니온을 메트릭스로
	//-----------------------------------------------------------------------------
	D3DXMATRIX		matTemp;
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
	return matTemp;
}


//-----------------------------------------------------------------------------
// Desc: 외각선 그리기
//-----------------------------------------------------------------------------
bool	Character::GetRange( int nCheckRange )
{		
	if( m_nTableIndex != MAX_USER_INTABLE )
	{
		int nRangeX, nRangeY;
		if( m_bPC == true )
		{
			//nRangeX = (int)g_Pc.GetDestX()-(int)(g_Pc.m_PcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize);
			//nRangeY = (int)g_Pc.GetDestY()-(int)(g_Pc.m_PcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize);
			nRangeX = (int)g_Pc.GetReceiveX()-(int)(g_Pc.m_PcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize);
			nRangeY = (int)g_Pc.GetReceiveY()-(int)(g_Pc.m_PcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize);
		}
		else
		{
			//nRangeX = (int)g_Pc.GetDestX()-(int)(g_Map.m_NpcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize);
			//nRangeY = (int)g_Pc.GetDestY()-(int)(g_Map.m_NpcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize);
			nRangeX = (int)g_Pc.GetReceiveX()-(int)(g_Map.m_NpcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize);
			nRangeY = (int)g_Pc.GetReceiveY()-(int)(g_Map.m_NpcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize);
		}
		if( abs(nRangeX) > nCheckRange || abs(nRangeY) > nCheckRange )
			return false;
	}
	return true;
}


bool	Character::DisplayLine( BYTE _byType, BYTE _bySort )
{		
	// 파괴오브젝트는 그리지 않는다	
//	if( GetAttrIndex() == DESTROY_OBJECT_INDEX )
//		return false;	
	// 카메라와의 거리가 300 이상이면 그리지 않는다	
	if( g_Camera.m_fCameraRadius >= 300.0f )
		return false;	
	// 16타일 이상은 그리지 않는다		
	else if( GetRange( XBLOCK_RANGE/2 ) == false )
		return false;

	//-----------------------------------------------------------------------------
	// 메터리얼 조정(기본 까만색)
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material;
	ZeroMemory( &material, sizeof(D3DMATERIAL9) );
#else
	D3DMATERIAL8 material;
	ZeroMemory( &material, sizeof(D3DMATERIAL8) );
#endif // DIRECT_VERSION_9_MJH	
	material.Diffuse.a = m_pMaterial[0].Diffuse.a;
	material.Ambient.a = m_pMaterial[0].Ambient.a;	

	// 자기 캐릭터와 파티한 유저인가?	
	if( m_nTableIndex != MAX_USER_INTABLE && m_bPC == true )
	{
//		for( int i = 0; i < g_Menu.m_iPartyMemberCount; i++ )
		{
//			if( g_Menu.m_pMember[i].m_lUnique == g_Pc.m_PcTable[m_nTableIndex].lUnique )
			{
				material.Diffuse.r = 0.0f;
				material.Diffuse.g = 0.9f;
				material.Diffuse.b = 0.2f;
				material.Ambient.r = 0.0f;
				material.Ambient.g = 0.9f;
				material.Ambient.b = 0.2f;
//				break;
			}
		}
	}
	/*
	// 선택한 대상은 빨간색으로 외각선 그리기
	if( m_nTableIndex != MAX_USER_INTABLE && g_Pc.m_Select.bPC == m_bPC && g_Pc.m_Select.index == m_nTableIndex )
	{		
		material.Diffuse.r = 0.8f;
		material.Diffuse.g = 0.2f;
		material.Diffuse.b = 0.1f;
		material.Ambient.r = 0.8f;
		material.Ambient.g = 0.2f;
		material.Ambient.b = 0.1f;		
	}
	*/
	
	g_RenderManager.SetMaterial( &material );

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------	
	D3DXMATRIX	matScale, matWorld;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matWorld, &matScale, &m_matChr );		
	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );


	//-----------------------------------------------------------------------------
	// 텍스쳐 셋팅 끄기
	//-----------------------------------------------------------------------------	
	g_RenderManager.SetTexture( 0, NULL );


	//-----------------------------------------------------------------------------	
	// 현재모드에 따라서 외각선 굵기
	//-----------------------------------------------------------------------------
	float	fLineSize;
	switch( _byType )
	{
	case DISPLAY_SELECT:		// 선택창
		fLineSize = 0.12f;		
		break;
	case DISPLAY_MAKE:			// 생성창		
		fLineSize = 0.07f;		
		break;
	default:
		{
			// 카메라와의거리
			fLineSize = g_Camera.m_fCameraRadius/700.0f;

			// 최대
			if( fLineSize > 0.12f )
				fLineSize = 0.12f;
			// 최소
			else if( fLineSize < 0.03f )
				fLineSize = 0.03f;
			/*
			// 내가 선택한 NPC는 외각선 두께를 더 준다
			if( m_nTableIndex != MAX_USER_INTABLE && g_Pc.m_Select.bPC == m_bPC && g_Pc.m_Select.index == m_nTableIndex )
				fLineSize *= 2.0f;
				*/
		}
		break;
	}

	//-----------------------------------------------------------------------------
	// 캐릭터 렌더링
	//-----------------------------------------------------------------------------
	int mod_index, tex_index, itm_index;	
		int itm_count;// = g_Pc_Manager.GetItmCount( this );
//	if( m_bPC == true && GetAttrIndex() == UNION_FAIRY )	
//		itm_count--;	

	itm_count = g_Pc_Manager.GetItmCount( this );
	int i = 0;
	for(  i = 0; i < itm_count; ++i )
	{					
		if( m_pItm_index[i]	== -1 )
			continue;

		//-----------------------------------------------------------------------------
		// 텍스쳐, 모델 인덱스
		//-----------------------------------------------------------------------------
		mod_index = g_Pc_Manager.GetModIndex( this, i );
		tex_index = g_Pc_Manager.GetTexIndex( this, i );		
		if( tex_index == -1 )
			continue;

		//-----------------------------------------------------------------------------				
		// 외각선 안그리는 예외처리
		//-----------------------------------------------------------------------------				
		bool bFind = true;		
		switch( g_Pc_Manager.GetItmIndex(this, i) )
		{
		// 요정머리
		case 10:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 53:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 54:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 55:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 56:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;			
		case 120:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;	
		case 154:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 155:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 222:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		case 223:
			{
				if( m_bPC == true )
					bFind = false;
			}
			break;
		// 요정상인 날개
		case 38:
			{
				if( m_bPC == false )
					bFind = false;
			}
			break;
		// 말머리카락
		case 39:
			{
				if( m_bPC == false )
					bFind = false;
			}
			break;
		case 40:
			{
				if( m_bPC == false )
					bFind = false;
			}
			break;
		case 41:
			{
				if( m_bPC == false )
					bFind = false;
			}
			break;
		}						
		if( bFind == true )
		{
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			BYTE byType = (BYTE)m_bPC;
			if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				byType = (BYTE)true;
			}			
			
			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			{
				//-----------------------------------------------------------------------------
				// 외각선 버퍼 락 걸어서 갱신하기
				//-----------------------------------------------------------------------------
				OBJECTVERTEX*	pOBJVERTEX;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}
				
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
				{					
					pOBJVERTEX[j].Vertex.x	= m_pVtxBuffer[i].pVertex[j].x+(g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal.x*fLineSize);
					pOBJVERTEX[j].Vertex.y	= m_pVtxBuffer[i].pVertex[j].y+(g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal.y*fLineSize);
					pOBJVERTEX[j].Vertex.z	= m_pVtxBuffer[i].pVertex[j].z+(g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal.z*fLineSize);
					//pOBJVERTEX[j].Vertex.x	*= m_vScale.x;
					//pOBJVERTEX[j].Vertex.y	*= m_vScale.y;
					//pOBJVERTEX[j].Vertex.z	*= m_vScale.z;
				}
				m_pVtxBuffer[i].NormalVB->Unlock();

				//-----------------------------------------------------------------------------
				// 렌더링
				//-----------------------------------------------------------------------------
				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
										g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
			}
		}
	}	

	//-----------------------------------------------------------------------------
	// 아이템 렌더링
	//-----------------------------------------------------------------------------
	if( m_bPC == true )
	{		
		//-----------------------------------------------------------------------------	
		// 앉아있다면 캐릭터 속성에 따라서 알맞는 의자는 그려준다 
		//-----------------------------------------------------------------------------	
		if( m_pUserInfo->bDown == true )
		{
			//-----------------------------------------------------------------------------	
			// 아이템인덱스, 페어리 아이템은 그릴 필요 없다
			//-----------------------------------------------------------------------------	
			bool bFind = true;
			if( GetAttrIndex() == UNION_FAIRY )
				bFind = false;
			else
				itm_index = CHAIR_ITEM_INDEX;
			
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			if( true == bFind )
			{
				// 모델, 텍스쳐 인덱스
				mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
				tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;

				BYTE byType = (BYTE)m_bPC;
				if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					byType = (BYTE)true;
				}				

				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
				{
					// 렌더링
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_matChr );
					g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pNormalVB, sizeof(OBJECTVERTEX) );
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
					DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
											g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				}
			}
		}

		//-----------------------------------------------------------------------------
		// 넥타, 타르 그리기
		//-----------------------------------------------------------------------------
		if( m_pUserInfo->EtcItem > 0 )
		{
			// 해당 아이템 인덱스
			bool bFind = true;
			switch( m_pUserInfo->EtcItem )
			{
			// 넥타
			case 1:
				itm_index = NECTA_ITEM_INDEX;
				break;
			case 11:
				itm_index = NECTA_ITEM_INDEX;
				break;
			// 타르
			case 2:
				itm_index = TAR_ITEM_INDEX;
				break;
			case 12:
				itm_index = TAR_ITEM_INDEX;
				break;
			// 기타상황(렌더링하며 안된다)
			default:
				bFind = false;
				break;
			}
			if( true == bFind )
			{
				//-----------------------------------------------------------------------------
				// 모델, 텍스쳐 인덱스
				//-----------------------------------------------------------------------------
				mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
				tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;

				//-----------------------------------------------------------------------------
				// 텍스쳐 있는지 검사
				//-----------------------------------------------------------------------------
				BYTE byType = (BYTE)m_bPC;
				if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
				{
					byType = (BYTE)true;
				}		

				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
				{
					// 렌더링
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_matRHand2 );
					g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pLineVB, sizeof(OBJECTVERTEX) );
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
					DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
											g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				}
			}
		}

		//-----------------------------------------------------------------------------
		// 무기그리기
		//-----------------------------------------------------------------------------
		for( int i = 0; i < LINK_ITEM_COUNT; ++i )
		{					
			itm_index = m_pUserInfo->ArmorInfo.index[i];
			if( itm_index != -1 )
			{				
				//-----------------------------------------------------------------------------
				// 아이템, 모델, 텍스쳐인덱스
				//-----------------------------------------------------------------------------				
				mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
				tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;				
				
				//-----------------------------------------------------------------------------
				// 월드메트릭스설정
				//-----------------------------------------------------------------------------
				bool bFind = true;
				switch( i )
				{
				case 0:
					matWorld = m_matRHand;
					break;
				case 1:
					matWorld = m_matLHand;
					break;
				}
				if( g_Pc_Manager.m_pItm[itm_index]->m_class == 1 && g_Pc_Manager.m_pItm[itm_index]->m_type == 3 )				
					bFind = false;				
								
				//-----------------------------------------------------------------------------
				// 텍스쳐 있는지 검사
				//-----------------------------------------------------------------------------
				if( true == bFind )
				{
					BYTE byType = (BYTE)m_bPC;
					if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
					{
						byType = (BYTE)true;
					}	
					
					if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
					{
						// 렌더링
						g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
						g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pLineVB, sizeof(OBJECTVERTEX) );
						g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
						DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
												g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
					}
				}
			}		
		}
	}
	return true;
}

//..............................................................................................................
// 유니크 얻기 by wxywxy
//..............................................................................................................
long Character::GetUnique(void)
{
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		return g_Pc.m_MainPC.lUnique;
	}
	else
	{
		if( m_bPC == TRUE )
		{
			return g_Pc.m_PcTable[m_nTableIndex].lUnique;
		}
		else
		{
			return g_Map.m_NpcTable[m_nTableIndex].lUnique;
		}
	}

	return -1;
}

//..............................................................................................................
// 인덱스 얻기 by wxywxy
//..............................................................................................................
int Character::GetIndex(void)
{
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		return -1;
	}
	else
	{
		if( m_bPC == true )
		{
			return -1;
		}
		else
		{
			if( m_nTableIndex == -1 || GetUnique() == - 1 )
				return MAX_NPC_INTABLE;
			else
				return g_Map.m_NpcTable[m_nTableIndex].index;
		}
	}

	return -1;
}

bool    Character::ViewModelPC(void)
{
	if( !PreTextureLoad() )
	{
		return false;
	}	
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH	
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	material.Diffuse.a = 1.0f;
	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z );	

	g_RenderManager.SetTransform( D3DTS_WORLD, &matScale );	

	if( g_Map.theWireframe )
		g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
	else
		g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	//-----------------------------------------------------------------------------
	// 아이템 갯수만큼
	//-----------------------------------------------------------------------------	
	int tex_index, mod_index;	
	int itm_count;
	int AttrIndex		= GetAttrIndex();	

	itm_count = g_Pc_Manager.GetItmCount( this );

	for( int i = 0; i < itm_count; ++i )
	{	
		if( m_pItm_index[i]	== -1 )
			continue;

		if( m_bPC && m_Basic_DrawItm[ i ] )
		{
			mod_index = g_Pc_Manager.GetBasicModIndex( this, i );
			tex_index = g_Pc_Manager.GetBasicTexIndex( this, i );
			if( tex_index == -1 || mod_index == -1)
				continue;

			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( m_bPC, tex_index ) == true )
			{				
				// 버퍼에넣기					
				OBJECTVERTEX*	pOBJVERTEX;
				
				if( m_Basic_VtxBuffer[ i ].NormalVB == NULL )
					continue;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_Basic_VtxBuffer[ i ].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_Basic_VtxBuffer[ i ].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}
								
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
				{
					memcpy( pOBJVERTEX[j].Vertex, m_Basic_VtxBuffer[ i ].pVertex[j], sizeof(m_Basic_VtxBuffer[ i ].pVertex[j]) );
					
					// 버퍼초기화안되었다면 노말, U,V값넣기
					if( m_Basic_VtxBuffer[ i ].bInit == false )
					{
						pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
						pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
						pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
					}						
				}
				if( m_Basic_VtxBuffer[ i ].bInit == false )
					m_Basic_VtxBuffer[ i ].bInit = true;

				//..........................................................................................
				// 셋트 아이템 하의 부츠 장갑 그려주기 막기 by simwoosung
				//..........................................................................................
				if( m_bPC && ( CHAR_WEAR_SETITEM == m_Char_WearType ) )
				{
					if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
					{
						m_pVtxBuffer[i].NormalVB->Unlock();
						continue;
					}
				}

				m_Basic_VtxBuffer[ i ].NormalVB->Unlock();
				g_RenderManager.SetStreamSource( 0, m_Basic_VtxBuffer[ i ].NormalVB, sizeof(OBJECTVERTEX) );
				
				//g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
				g_RenderManager.SetMaterial( &material );	
				
				// 렌더링			
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
										g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				
			}
			else
			{
				return false;
			}
		}

		//-----------------------------------------------------------------------------
		// 텍스쳐, 모델 인덱스
		//-----------------------------------------------------------------------------
		mod_index = g_Pc_Manager.GetModIndex( this,  i );
		tex_index = g_Pc_Manager.GetTexIndex( this,  i );
		if( tex_index == -1 || mod_index == -1)
			continue;
		
		//-----------------------------------------------------------------------------
		// 텍스쳐 있는지 검사
		//-----------------------------------------------------------------------------
		if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( m_bPC, tex_index ) == true )
		{				
			// 버퍼에넣기					
			OBJECTVERTEX*	pOBJVERTEX;
			
			if( m_pVtxBuffer[ i ].NormalVB == NULL )
				continue;
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( m_pVtxBuffer[ i ].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
			if( FAILED( m_pVtxBuffer[ i ].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
			{
				return false;		
			}		
			
			for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
			{
				memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[ i ].pVertex[j], sizeof(m_pVtxBuffer[ i ].pVertex[j]) );
				
				// 버퍼초기화안되었다면 노말, U,V값넣기
				if( m_pVtxBuffer[ i ].bInit == false )
				{
					pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
					pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
					pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
				}						
			}
			if( m_pVtxBuffer[ i ].bInit == false )
				m_pVtxBuffer[ i ].bInit = true;
			

			m_pVtxBuffer[ i ].NormalVB->Unlock();
			g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[ i ].NormalVB, sizeof(OBJECTVERTEX) );
			
			g_RenderManager.SetMaterial( &material );					
		
			// 렌더링			
			g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
			DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
									g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );		
			
		}
		else
		{
			return false;
		}

	}	

	return true;
}

bool    Character::ViewModelNPC(void)
{
	if( !PreTextureLoad() )
	{
		return false;
	}

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH	
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 	

	g_RenderManager.SetTransform( D3DTS_WORLD, &matScale );	

	if( g_Map.theWireframe )
		g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
	else
		g_RenderManager.SetFillMode( D3DFILL_SOLID );
	
	//-----------------------------------------------------------------------------
	// 아이템 갯수만큼
	//-----------------------------------------------------------------------------	
	int tex_index, mod_index;	
	int itm_count;
	int AttrIndex		= GetAttrIndex();	

	itm_count = g_Pc_Manager.GetItmCount( this );

	for( int i = 0; i < itm_count; ++i )
	{	
		if( m_pItm_index[i]	== -1 )
			continue;

		if( byType && m_Basic_DrawItm[i] )
		{
			mod_index = g_Pc_Manager.GetBasicModIndex( this, i );
			tex_index = g_Pc_Manager.GetBasicTexIndex( this, i );
			if( tex_index == -1 || mod_index == -1)
				continue;

			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			{				
				// 버퍼에넣기					
				OBJECTVERTEX*	pOBJVERTEX;
				
				if( m_Basic_VtxBuffer[i].NormalVB == NULL )
					continue;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}			
				
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
				{
					memcpy( pOBJVERTEX[j].Vertex, m_Basic_VtxBuffer[i].pVertex[j], sizeof(m_Basic_VtxBuffer[i].pVertex[j]) );
					
					// 버퍼초기화안되었다면 노말, U,V값넣기
					if( m_Basic_VtxBuffer[i].bInit == false )
					{
						pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
						pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
						pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
					}						
				}
				if( m_Basic_VtxBuffer[i].bInit == false )
					m_Basic_VtxBuffer[i].bInit = true;
				m_Basic_VtxBuffer[i].NormalVB->Unlock();
				g_RenderManager.SetStreamSource( 0, m_Basic_VtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				
				g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
				g_RenderManager.SetMaterial( &material );	
				
				// 렌더링			
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
										g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				
			}
			else
			{
				return false;
			}
		}

		//-----------------------------------------------------------------------------
		// 텍스쳐, 모델 인덱스
		//-----------------------------------------------------------------------------
		mod_index = g_Pc_Manager.GetModIndex( this, i );
		tex_index = g_Pc_Manager.GetTexIndex( this, i );
		if( tex_index == -1 || mod_index == -1)
			continue;
		
		//-----------------------------------------------------------------------------
		// 텍스쳐 있는지 검사
		//-----------------------------------------------------------------------------
		if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
		{				
			// 버퍼에넣기					
			OBJECTVERTEX*	pOBJVERTEX;
			
			if( m_pVtxBuffer[i].NormalVB == NULL )
				continue;
#ifdef DIRECT_VERSION_9_MJH
			if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
			if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
			{
				return false;		
			}

			for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
			{
				memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[i].pVertex[j], sizeof(m_pVtxBuffer[i].pVertex[j]) );
				
				// 버퍼초기화안되었다면 노말, U,V값넣기
				if( m_pVtxBuffer[i].bInit == false )
				{
					pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
					pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
					pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
				}						
			}
			if( m_pVtxBuffer[i].bInit == false )
				m_pVtxBuffer[i].bInit = true;
			m_pVtxBuffer[i].NormalVB->Unlock();
			g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
			
			g_RenderManager.SetMaterial( &material );					
		
			// 렌더링			
			g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
			DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
									g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );		
			
		}
		else
		{
			return false;
		}
	}	

	return true;
}

// 미리 텍스처 읽기
BOOL    Character::PreTextureLoad(void)
{
	int tex_idx = 0;
	int itm_idx = 0;
	int itm_cnt = 0;
	int idx = 0;

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}

	if( m_bPC == true || 
		( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		itm_cnt = c_Max_WearItemType;		
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{		
			tex_idx = g_Pc_Manager.GetBasicTexIndex( this, idx );	
			
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return FALSE;
			}
			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return FALSE;
			}
		}

		if( m_pUserInfo )
		{
			for( idx = 0; idx < LINK_ITEM_COUNT; ++idx )
			{
				itm_idx = m_pUserInfo->ArmorInfo.index[idx];
				if( itm_idx != -1 )
				{
					tex_idx = g_Pc_Manager.m_pItm[itm_idx]->m_tex_index;				
					if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
					{
						return FALSE;
					}
				}
			}
		}
	}
	else 
	{
		itm_cnt = g_Pc_Manager.GetItmCount( this );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return FALSE;
			}
		}
	}
	
	if( m_State.bRide )
	{
		itm_cnt = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[m_State.TransIndex] );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( &g_Map.m_Npc[m_State.TransIndex], idx );		
			if( tex_idx != -1 )
			{	
				g_Pc_Manager.m_pTex[tex_idx]->SetTexture( 0, tex_idx );				
			}
		}
	}

	if( m_State.nRideState == nRideState_Ride )
	{
		for( int i = 0; i < c_Max_RideItemType ; ++i )
		{			
			if( m_Ride_Item_index[i] != -1 )
			{
				tex_idx = g_Pc_Manager.GetRideTexIndex( this, i );
				if( tex_idx != -1 )
				{	
					g_Pc_Manager.m_pTex[tex_idx]->SetTexture( 1, tex_idx );				
				}
			}			
		}	
	}

	return TRUE;
}

//#define _DECLARE_DEPENCE_INCHANT
//#define EFFECT_WEPON_INCHANT
char TestNameImg[19][60]= 
{ 
		"basic_blur_05.tga" ,
		"basic_blur_04.tga" , 
		"basic_blur_11.tga" , 
		"basic_blur-r_02.tga" , 
		"basic_blur-rr_03.tga" , 
		"bazizic.tga",
		"bazizic-2.tga"
		"blood_02.tga",
		"blu-04.tga",
		"bubble_03.tga"
		"cilcle_orange_001.tga"
		"cross.tga",
		"detact_01.tga",
		"energy_10002.tga",
		"fire_7.tga",
		"fire_7-2.tga",
		"firering.tga",
		"folw_04.tga",
		"gi_02.tga",
};


void	Character::Display( BYTE _byType, BYTE _bySort, bool bFlag, BOOL IsRender )
{

	if( bFlag == false && m_State.nRideState == nRideState_Ride )
	{
		return;
	}

#ifdef JAPAN_HPZERO_BUG_MODIFY
	if( !m_bPC && !g_bFindRHpZeroAliveMob )
	{
		if( !m_IsRealDie )
		{		
			if( !g_Map.m_NpcTable[ m_nTableIndex ].theRealHP && 
				 g_Map.m_NpcTable[ m_nTableIndex ].race == c_NpcRace_Mob )
			{
				///--PRINT_DLG_LOG("Find Bug RealHP = 0 And Mob Alive");
			}
		}
	}
#endif
	

#ifdef _DECLARE_DEPENCE_INCHANT
	//.......................................................................................................
	// 무기 강화 테스트 by wxywxy
	//.......................................................................................................
	g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
	g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );
	
	g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_MIRROR );
	g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_MIRROR );
	g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_MIRROR );
	g_RenderManager.SetTextureCoordIndex( 1 , 0 );

	static DWORD OldTime = timeGetTime();
#ifdef DIRECT_VERSION_9_MJH
	static LPDIRECT3DTEXTURE9 pTexture = NULL;
#else
	static LPDIRECT3DTEXTURE8 pTexture = NULL;
#endif // DIRECT_VERSION_9_MJH
	static BOOL OneTime = TRUE;
	static DWORD Time1000 = g_nowTime;
/*
	
	D3DMATERIAL8	TempMaterial;
	TempMaterial.Diffuse.r = TempMaterial.Ambient.r = 1.0f;
	TempMaterial.Diffuse.g = TempMaterial.Ambient.g = 1.0f;
	TempMaterial.Diffuse.b = TempMaterial.Ambient.b = 1.0f;		
	TempMaterial.Diffuse.a = TempMaterial.Ambient.a = 0.5f ;
	D3DLIGHT8	TempLight2;
	TempLight2.Diffuse.r = TempLight2.Ambient.r = 1.0f;
	TempLight2.Diffuse.g = TempLight2.Ambient.g = 1.0f;
	TempLight2.Diffuse.b = TempLight2.Ambient.b = 1.0f;		
	TempLight2.Diffuse.a = TempLight2.Ambient.a = 0.0f ;
*/
	D3DXMATRIX matTexTrans;
	static float	   fMove = 0.5f; //( float )SAFE_TIME_SUB( timeGetTime() , OldTime ) / 40000.0f;
	static int Count = 0 ; 

	if( SAFE_TIME_COMPARE( SAFE_TIME_SUB( g_nowTime , Time1000  ) , >= , 1000 ) )
	{
		Time1000 = g_nowTime;
		
		if(( Count++) > 19 )
		{
			Count = 0 ;
		}	

		fMove+= 0.1f;

		if( fMove > 0.8f )
		{
			fMove = 0.5f;
		}

	}	
	

	D3DXMatrixIdentity( &matTexTrans );

	D3DXMatrixRotationZ( &matTexTrans , D3DXToRadian( fMove * 360 ) );
	
	matTexTrans._32 = fMove;
	matTexTrans._11 = fMove * 1.0f;
	matTexTrans._22 = fMove * 1.0f;

	g_RenderManager.SetTransform( D3DTS_TEXTURE1 , &matTexTrans );


	if( OneTime )
	{
		pTexture = g_Particle.m_EffectTextureManager.GetTexture( TestNameImg[Count] );
	}

	//.......................................................................................................
#endif


#ifdef EFFECT_WEPON_INCHANT
				//.......................................................................................................
				// 무기 강화 테스트 by wxywxy
				//.......................................................................................................
				g_RenderManager.SetTextureAddressU( 0 , D3DTADDRESS_WRAP );
				g_RenderManager.SetTextureAddressV( 0 , D3DTADDRESS_WRAP );
				g_RenderManager.SetTextureAddressW( 0 , D3DTADDRESS_WRAP );
				
				g_RenderManager.SetTextureAddressU( 1 , D3DTADDRESS_MIRROR );
				g_RenderManager.SetTextureAddressV( 1 , D3DTADDRESS_MIRROR );
				g_RenderManager.SetTextureAddressW( 1 , D3DTADDRESS_MIRROR );
				g_RenderManager.SetTextureCoordIndex( 1 , 0 );
				
				static DWORD OldTime = timeGetTime();	
#ifdef DIRECT_VERSION_9_MJH
				static LPDIRECT3DTEXTURE9 pTexture = NULL;
#else
				static LPDIRECT3DTEXTURE8 pTexture = NULL;
#endif // DIRECT_VERSION_9_MJH
				static BOOL OneTime = TRUE;
				
				D3DXMATRIX matTexTrans;
				float	   fMove = ( float )SAFE_TIME_SUB( timeGetTime() , OldTime ) / 4000.0f;
				
				D3DXMatrixIdentity( &matTexTrans );
				
				D3DXMatrixRotationZ( &matTexTrans , D3DXToRadian( fMove * 360 ) );
				
				matTexTrans._32 = fMove;
				matTexTrans._11 = fMove * 1.0f;
				matTexTrans._22 = fMove * 1.0f;
				
				g_RenderManager.SetTransform( D3DTS_TEXTURE1 , &matTexTrans );
				
				
				if( OneTime )
				{
					pTexture = g_Particle.m_EffectTextureManager.GetTexture( "special_05_2t3.tga" );
				}
				
				//.......................................................................................................
#endif



	if( m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
	{
		return;
	}	
	
	Character*	pDisplayChr = NULL;

	pDisplayChr = this;

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}
	
	// 캐릭터가 가지고 있는 아이템 텍스쳐가 모두 생성된 상태이면 그려준다 
	// 헤어, 얼굴, 상의, 하의, 장갑, 신발, 무기
	int tex_idx = 0;
	int itm_idx = 0;
	int itm_cnt = 0;
	int idx = 0;

	m_IsAllLTexture = TRUE;

#ifdef DIRECT_VERSION_9_MJH
	static LPDIRECT3DTEXTURE9	pEmblemTexture					 = NULL;		// 엠블렘이 있다면 엠블렘 텍스처 
#else
	static LPDIRECT3DTEXTURE8	pEmblemTexture					 = NULL;		// 엠블렘이 있다면 엠블렘 텍스처 
#endif // DIRECT_VERSION_9_MJH
	pEmblemTexture      = NULL;

#ifndef NEW_EMBLEM_ITEM_APPLY
	static BOOL					ISEmblemSlot[c_Max_WearItemType] = { 0 , };		// 엠블렘 관련 아이템이면 참
	static BOOL					ISEmblemWearFounded				 = FALSE;		// 엠블렘 관련 옷이 있을때
	ISEmblemWearFounded = FALSE;
	memset( ISEmblemSlot , 0 , sizeof( BOOL ) * c_Max_WearItemType );	
#endif
	
	///-- 먼저 엠블렌적용 아이템을 먼저파악
	DWORD	FrontierID = FindFrontierID();		
	
	///-- 플레이어 or 유저형 NPC
	if(  m_bPC == true || 
		( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		itm_cnt = c_Max_WearItemType;
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{		
			tex_idx = g_Pc_Manager.GetBasicTexIndex( pDisplayChr, idx );			
			
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				m_IsAllLTexture = FALSE;
				return;
			}
			
			tex_idx = g_Pc_Manager.GetTexIndex( pDisplayChr, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				m_IsAllLTexture = FALSE;
				return;
			}
			
#ifndef NEW_EMBLEM_ITEM_APPLY			
			//..................................................................................................
			// 엠블렘 처리할 아이템 찾기 By wxywxy
			//..................................................................................................
			if(FrontierID && m_bPC)
			{
				ApplyIItemSlotAndFounded(idx, ISEmblemSlot, ISEmblemWearFounded);
			}			
#endif
		}		

		for( idx = 0; idx < LINK_ITEM_COUNT; ++idx )
		{
			itm_idx = m_pUserInfo->ArmorInfo.index[idx];
			if( itm_idx != -1 )
			{
				tex_idx = g_Pc_Manager.m_pItm[itm_idx]->m_tex_index;	
				if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
				{
					m_IsAllLTexture = FALSE;
					return;
				}
			}
		}		
	}
	else 
	{
		itm_cnt = g_Pc_Manager.GetItmCount( pDisplayChr );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( pDisplayChr, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				m_IsAllLTexture = FALSE;
				return;
			}
		}
	}

	if( m_State.bRide )
	{
		itm_cnt = g_Pc_Manager.GetItmCount( &g_Map.m_Npc[m_State.TransIndex] );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( &g_Map.m_Npc[m_State.TransIndex], idx );		
			if( tex_idx != -1 )
			{
				g_Pc_Manager.m_pTex[tex_idx]->SetTexture( 0, tex_idx );			
			}
		}
	}
	
	if( m_State.nRideState == nRideState_Ride )
	{
		for( int i = 0; i < c_Max_RideItemType ; ++i )
		{			
			if( m_Ride_Item_index[i] != -1 )
			{
				tex_idx = g_Pc_Manager.GetRideTexIndex( pDisplayChr, i );
				if( tex_idx != -1 )
				{
					g_Pc_Manager.m_pTex[tex_idx]->SetTexture( 1, tex_idx );					
				}
			}			
		}	
	}	

	BOOL IsApplNationEmblem = FALSE; 

	if( FrontierID )
	{
		VESIONTEX *pData = g_EmblemTexManager.FindForntierVersionTex( FrontierID );
		
		if( NULL != pData )
		{
			if( NULL != pData->pRenderTexture )
			{
				pEmblemTexture = pData->pRenderTexture->GetTexture();
			}
		}
		else
		{
			///-- 프론티어에 길드 아이콘이 없을경우
			if( m_bPC == true )
			{
				IsApplNationEmblem = TRUE;	
			}
		}
	}
	else
	{
		///-- 프론티어에 소속되어있지 않을 경우
		if( m_bPC == true )
		{
			IsApplNationEmblem = TRUE;				
		}
	}

	if( IsApplNationEmblem )
	{
		if( GetAttrIndex() == n_Millena_Man ||
			GetAttrIndex() == n_Millena_Woman )
		{
			pEmblemTexture = g_EmblemTexManager.m_MEmbTexture;	
		}
		else
		{
			pEmblemTexture = g_EmblemTexManager.m_REmbTexture;
		}	
	}

	BOOL IsPick = FALSE;
	
	//-----------------------------------------------------------------------------
	// 메테리얼셋팅
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH
	
	if( m_State.nCharState == nCharState_Detect_Illusion )
	{
		material.Diffuse.r = material.Ambient.r = 0.25f;
		material.Diffuse.g = material.Ambient.g = 0.25f;
		material.Diffuse.b = material.Ambient.b = 0.5f;		
	}
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	// 거리에 따른 블렌딩
	bool bRes;

	if( m_bPC )
	{
		if( m_nTableIndex == MAX_USER_INTABLE )
			bRes = g_Pc.m_MainPC.bRender;
		else
			bRes = g_Pc.m_PcTable[m_nTableIndex].bRender;		
	}
	else
	{
		bRes = g_Map.m_Npc[ m_nTableIndex ].m_bIsVisible;
	}
	
	if( bRes == true && m_curt_event.type != SM_NPC_RESPAWN && m_curt_event.type != SM_STATE_DIE )
	{			
		if( c_NotMoveNPCCode_Min <= g_Map.m_NpcTable[ m_nTableIndex ].code &&
			c_NotMoveNPCCode_Max >= g_Map.m_NpcTable[ m_nTableIndex ].code &&
			!m_bPC )
		{
			material.Diffuse.a = 1.0f;		
		}
		else
		{
			if( m_fDistance <= 480.0f )					
				material.Diffuse.a = 1.0f;		
			else if( m_fDistance > 480.0f )		
				material.Diffuse.a = 1.0f-((m_fDistance-480.0f)/80.0f);		
			
			if( material.Diffuse.a < 0.0f )
				material.Diffuse.a = 0.0f;
			
			if( g_Pc.GetPlayer()->theMapType == n_FightZone )
			{
				material.Diffuse.a = 1.0f;		
			}
		}
	}		

	if( GetUnique() == g_RockClient.m_PickUnique )
	{
		IsPick = TRUE;
	}

	bool IsNpcObject = false;			// 퀘스트에 필요한 오브젝트들은 NPC로 넣기 때문에 알파테스트로 뺀다 
	
	if( m_bPC == false && ( g_Map.m_NpcTable[m_nTableIndex].code >= c_ObjectNPCCode_Min && 
		g_Map.m_NpcTable[m_nTableIndex].code <= c_ObjectNPCCode_Max ) )
	{
		IsNpcObject = true;
		g_RenderManager.SetAlphaBlendEnable( FALSE );
	}

	//-----------------------------------------------------------------------------
	// 외각선 그리기
	//-----------------------------------------------------------------------------	

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale, matWorld,matLocal;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 
	
	if(m_State.nRideState != nRideState_Master)
	 D3DXMatrixMultiply( &matWorld, &matScale, &m_matChr );
	else
	 D3DXMatrixMultiply( &matWorld, &matScale, &m_matRideChr );	

	//.........................................................................................................
	// 캐릭터 죽었을때 높이 수정 By wxywxy
	//.........................................................................................................
	if( m_curt_event.type == SM_STATE_DIE )
	{
		m_matChr._42 = m_matBoneCenter._42;
	}
	//.........................................................................................................

	SetVibrationPos( &matWorld );
	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
	matLocal = matWorld;

	if( g_Map.theWireframe )
		g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
	else
		g_RenderManager.SetFillMode( D3DFILL_SOLID );	

	if( FALSE == m_bPC )
	{
		if( g_Map.m_NpcTable[ m_nTableIndex ].race == c_NpcRace_Mob )
		{
			if( FALSE == m_ISPriority )//&& SM_STATE_DIE != m_curt_event.type )
			{
				material.Diffuse.r = material.Ambient.r = 0.3f;
				material.Diffuse.g = material.Ambient.g = 0.3f;
				material.Diffuse.b = material.Ambient.b = 0.3f;
			}
		}
	}

	//...........................................................................................................
	// 부가 효과 By wxywxy
	//...........................................................................................................
	if( m_ISEffectColor )
	{
		material.Diffuse.r = material.Ambient.r = m_EffectColor.r;
		material.Diffuse.g = material.Ambient.g = m_EffectColor.g;
		material.Diffuse.b = material.Ambient.b = m_EffectColor.b;		
	}

	//.................................................................................................................
	// 아이템 갯수만큼 출력
	//.................................................................................................................
	int tex_index, mod_index, itm_index;	
	int itm_count;//		
	int AttrIndex		= GetAttrIndex();	

	itm_count = g_Pc_Manager.GetItmCount( pDisplayChr );

	int  RenderMaxCount = 1;

	
	// 투명 부분
	switch( m_CharAlpha.GetState() )
	{
		case CHAR_HIDE:	
			
			RenderMaxCount = 2;
			
			material.Diffuse.a = m_CharAlpha.fAlpha;
			
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			
			g_RenderManager.SetZWriteEnable( TRUE );
			g_RenderManager.SetZEnable( TRUE );
			
			g_RenderManager.SetAlphaBlendEnable( TRUE );

			m_bShadowDisplay	= FALSE;

			break;

		case CHAR_NORMAL:
			
			RenderMaxCount = 1;
			
			g_RenderManager.SetCullMode( D3DCULL_NONE );
			
			g_RenderManager.SetZWriteEnable( TRUE );
			g_RenderManager.SetZEnable( TRUE );
			
			g_RenderManager.SetAlphaBlendEnable( TRUE );
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );

			g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

			m_bShadowDisplay	= TRUE;
			
			break;
	}	

	//.................................................................................................................
	// 알파 블렌딩이면 Z 값 비교후 출력 ( 두번 출력 )
	//.................................................................................................................
	for( int TwoPass = 0 ; TwoPass < RenderMaxCount ; ++TwoPass )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD, &matLocal );	

		//.............................................................................................................
		// 아이템 갯수만큼
		//.............................................................................................................
		for( int i = 0; i < itm_count; ++i )
		{	
			if( m_pItm_index[i]	== -1 )
				continue;

#ifdef _DECLARE_DEPENCE_INCHANT
			//.......................................................................................................
			// 무기 강화 테스트 by wxywxy
			//.......................................................................................................
			if( m_bPC == TRUE ||  ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			{
				switch( i )
				{
				case n_Wear_Face:
				case n_Wear_Hair: 
					g_RenderManager.DisableTextureStageState( 1 );
					g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
					
					g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
					g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
					
					
					g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
					g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
					g_RenderManager.SetTextureCoordIndex( 1 , 1 );
					break;
					
				case n_Wear_CoatArmor:
				case n_Wear_PantsArmor:
				case n_Wear_Gloves:
				case n_Wear_Shoes:
				case n_Wear_Serve:
			
			//		g_RenderManager.SetLighting(TRUE);
			//		g_RenderManager.SetLight( 1 , &TempLight2 );
			//		g_RenderManager.SetAlphaBlendEnable( TRUE );	
			//		g_RenderManager.SetMaterial( &TempMaterial );
					g_RenderManager.SetTexture( 1 , pTexture );
					
				

					g_RenderManager.SetTextureCoordIndex( 1 , 0 );

					g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT3 );
					g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
					g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
					
					
					g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
					g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
					
					g_RenderManager.SetTFactorColor( D3DCOLOR_ARGB( 50, 255, 255, 255 ) );
					g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT     );
				
				//	g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_TFACTOR );
					
				//	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADDSIGNED2X      );
				//	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
				//	g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD       );
					g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT     );
					g_RenderManager.SetTextureColorArg2( 1 , D3DTA_TEXTURE     );					
			
										

					
					break;
				}
			}
#endif		
			
			//.........................................................................................................
			// 플레이어일때 아이템 출력
			//.........................................................................................................
			if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				&& m_Basic_DrawItm[i] )
			{
				mod_index = g_Pc_Manager.GetBasicModIndex( pDisplayChr, i );
				tex_index = g_Pc_Manager.GetBasicTexIndex( pDisplayChr, i );
				
				if( tex_index == -1 || mod_index == -1)
					continue;
				
				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
				{				
					// 버퍼에넣기					
					OBJECTVERTEX*	pOBJVERTEX;
					
					if( m_Basic_VtxBuffer[i].NormalVB == NULL )
						continue;					
					
					if( m_IsApplyVA )
					{
#ifdef DIRECT_VERSION_9_MJH
						if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
						if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
							return;						
						
						for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
						{
							memcpy( pOBJVERTEX[j].Vertex, m_Basic_VtxBuffer[i].pVertex[j], sizeof(m_Basic_VtxBuffer[i].pVertex[j]) );
													
							// 버퍼초기화안되었다면 노말, U,V값넣기
							if( m_Basic_VtxBuffer[i].bInit == false )
							{
								pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
								pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
								pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
							}						
						}

						if( m_Basic_VtxBuffer[i].bInit == false )
							m_Basic_VtxBuffer[i].bInit = true;

						m_Basic_VtxBuffer[i].NormalVB->Unlock();
					}					
					
					g_RenderManager.SetStreamSource( 0, m_Basic_VtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
					
					g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
					g_RenderManager.SetMaterial( &material );	
					
					// 렌더링			
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
					
					if( m_CharAlpha.GetState() == CHAR_HIDE )
					{
						switch( TwoPass )
						{
							case 0 :
								g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
								g_RenderManager.SetDestBlend( D3DBLEND_ONE  );
								g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
								break;
							case 1:
								g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
								g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA  );
								g_RenderManager.SetZFunc( D3DCMP_EQUAL );
								break;
						}
					}
					
					///-- 은신중이거나 죽을경우에는 알파적용을 먹기때문에 알파테스트를 뺀다.
					if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE )
					{
						g_RenderManager.SetAlphaTestEnable(FALSE);
					}
					
					if(IsRender)
					{
						DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
						g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
					}

					if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE )
					{
						g_RenderManager.SetAlphaTestEnable(TRUE);
					}
				}
				else
				{
					m_bShadowDisplay = false;				
				}
			} 

			//.........................................................................................................
			
			//-----------------------------------------------------------------------------
			// 텍스쳐, 모델 인덱스
			//-----------------------------------------------------------------------------
			mod_index = g_Pc_Manager.GetModIndex( pDisplayChr, i );
			tex_index = g_Pc_Manager.GetTexIndex( pDisplayChr, i );
			
			if( tex_index == -1 || mod_index == -1)
				continue;
			
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			{				
				// 표정 
				if( i == n_Wear_Face && 
					( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) )
				{
					FaceExpProcess();
				}

				// 버퍼에넣기					
				OBJECTVERTEX*	pOBJVERTEX = NULL ;
				
				if( m_pVtxBuffer[i].NormalVB == NULL )
					continue;						
				
				if( m_IsApplyVA )
				{
#ifdef DIRECT_VERSION_9_MJH
					if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
					if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH				
					{
						return;		
					}					
					
					for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
					{
						memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[i].pVertex[j], sizeof(m_pVtxBuffer[i].pVertex[j]) );
									
						// 버퍼초기화안되었다면 노말, U,V값넣기
						if( m_pVtxBuffer[i].bInit == false )
						{
							pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
							pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
							pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;							
						}						
					}

					if( m_pVtxBuffer[i].bInit == false )
						m_pVtxBuffer[i].bInit = true;

					//..........................................................................................
					// 셋트 아이템 하의 부츠 장갑 그려주기 막기 by simwoosung
					//..........................................................................................
					if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) 
						&& ( CHAR_WEAR_SETITEM == m_Char_WearType ) )
					{
						if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
						{
							m_pVtxBuffer[i].NormalVB->Unlock();
							continue;
						}
					}

					m_pVtxBuffer[i].NormalVB->Unlock();
				}
				else
				{
					//..........................................................................................
					// 셋트 아이템 하의 부츠 장갑 그려주기 막기 by simwoosung
					//..........................................................................................
					if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) ) 
						&& ( CHAR_WEAR_SETITEM == m_Char_WearType ) )
					{
						if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
						{
							continue;
						}
					}
				}

				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );				
				
				g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
				g_RenderManager.SetMaterial( &material );				
				
				// 발광옵션 ON(선택한 캐릭터)			
				bool bEmitter = false;					
				
				// 렌더링			
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );				
				
				if( m_CharAlpha.GetState() == CHAR_HIDE )
				{
					switch( TwoPass )
					{
					case 0 :
						g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
						g_RenderManager.SetDestBlend( D3DBLEND_ONE );
						g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL  );
						break;
					case 1:
						g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
						g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
						g_RenderManager.SetZFunc( D3DCMP_EQUAL  );
						break;
					}
				}
				
				
				//..........................................................................................
				// 엠블렘 관련 텍스처 조합모드 넣기 By wxywxy
				//..........................................................................................
				static BOOL ISMultyTexture = FALSE;
				//..........................................................................................

				BOOL bEmblemItem = FALSE;
				BOOL bIsNoApplyAlphaTestItem = FALSE;
				DWORD dRealFileIndex = g_Pc_Manager.GetRealItemFileCode( pDisplayChr, i );

#ifndef NEW_EMBLEM_ITEM_APPLY
				
				bEmblemItem = ISEmblemSlot[ i ];

#else
				
				if(dRealFileIndex)
				{
					bEmblemItem = g_Pc_Manager.IsEmblemItem(pDisplayChr, dRealFileIndex);
				}
#endif			
				if(dRealFileIndex)
				{
					bIsNoApplyAlphaTestItem = g_Pc_Manager.IsNoApplyAlphaTestItem(pDisplayChr, dRealFileIndex);
				}
				
				if( bEmblemItem )
				{					
					if( pEmblemTexture )
					{			
					
						g_RenderManager.SetTexture( 1 , pEmblemTexture );			
						
				//		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
						g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
						g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
						g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );

						g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
						g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
						g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
						
						g_RenderManager.SetTextureCoordIndex ( 1 , 0 );						
						
						g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD	 );
						g_RenderManager.SetTextureColorArg1( 1 , D3DTA_TEXTURE   );
						g_RenderManager.SetTextureColorArg2( 1 , D3DTA_CURRENT  );

						g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_MODULATE );
						g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );
						g_RenderManager.SetTextureAlphaArg2( 1 , D3DTA_TEXTURE );
							
						ISMultyTexture = TRUE;
					
						
						g_RenderManager.SetTextureMipFilter ( 1 , D3DTEXF_NONE );
						g_RenderManager.SetTextureMagFilter ( 1 , D3DTEXF_LINEAR );
						g_RenderManager.SetTextureMinFilter ( 1 , D3DTEXF_POINT );

						
					}
				}			
				
				if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE ||
					bIsNoApplyAlphaTestItem )
				{
					g_RenderManager.SetAlphaTestEnable(FALSE);
				}				
				
				if(IsRender)
				{
					DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
						g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				}
				
				if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE ||
					bIsNoApplyAlphaTestItem )
				{
					g_RenderManager.SetAlphaTestEnable(TRUE);
				}

				//..........................................................................................
				// 엠블렘 관련 텍스처 돌려주기 By wxywxy
				//..........................................................................................
				if( ISMultyTexture )
				{
					g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
					g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );
					
					
					g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
					g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
					g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
					
					g_RenderManager.DisableTextureStageState( 1 );
					g_RenderManager.DisableTextureStageState( 2 );
					
					g_RenderManager.SetTexture( 1 , NULL );		

					ISMultyTexture = FALSE;
				}
				//..........................................................................................

				// 발광옵션 OFF			
				if( true == bEmitter )	
					g_RenderManager.SetTextureColorOP( 1 , D3DTOP_DISABLE );
				
			}
			//-----------------------------------------------------------------------------
			// 텍스쳐로딩이 되지 않은 경우가 있다면 그림자 플래그를 꺼야한다
			//-----------------------------------------------------------------------------
			else
			{
				m_bShadowDisplay = false;
				m_IsAllLTexture = FALSE;
			}
		}


#ifdef _DECLARE_DEPENCE_INCHANT
		//.......................................................................................................
		// 무기 강화 테스트 by wxywxy
		//.......................................................................................................
	
			
		g_RenderManager.SetAlphaBlendEnable( FALSE );
	//	g_RenderManager.LightEnable( 1 , FALSE );
		g_RenderManager.DisableTextureStageState( 1 );
		g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );

		g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
		g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );		
		
		g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
		g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
		g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
		g_RenderManager.SetTextureCoordIndex( 1 , 1 );
		//.......................................................................................................
#endif

		if( m_State.nRideState == nRideState_Ride )
		{
			for( int i = 0; i < c_Max_RideItemType ; ++i )
			{			
				if( m_Ride_Item_index[i] == -1 )
				{
					continue;
				}
				
				mod_index = g_Pc_Manager.GetRideModIndex( pDisplayChr, i );
				tex_index = g_Pc_Manager.GetRideTexIndex( pDisplayChr, i );
				
				if( tex_index == -1 || mod_index == -1)
					continue;

				// by dongs 말자체는 NPC bytype = 0 , SetTexture( bytype  , tex_index );
				// 그러므로 NPC tex 쳐로 로딩됨.. 
				// 강제로 1로 셋트 시킴 
				
				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( 1, tex_index ) == true )
				{				
					// 버퍼에넣기					
					OBJECTVERTEX*	pOBJVERTEX;
					
					if( m_Ride_VtxBuffer[i].NormalVB == NULL )
						continue;						
					
					if( m_IsApplyVA )
					{
#ifdef DIRECT_VERSION_9_MJH
						if( FAILED( m_Ride_VtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
						if( FAILED( m_Ride_VtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
						return;						
						
						for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
						{
							memcpy( pOBJVERTEX[j].Vertex, m_Ride_VtxBuffer[i].pVertex[j], sizeof(m_Ride_VtxBuffer[i].pVertex[j]) );
													
							// 버퍼초기화안되었다면 노말, U,V값넣기
							if( m_Basic_VtxBuffer[i].bInit == false )
							{
								pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
								pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
								pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
							}						
						}

						if( m_Ride_VtxBuffer[i].bInit == false )
						m_Ride_VtxBuffer[i].bInit = true;

						m_Ride_VtxBuffer[i].NormalVB->Unlock();
					}					
					
					g_RenderManager.SetStreamSource( 0, m_Ride_VtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
					
					g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
					g_RenderManager.SetMaterial( &material );	
					
					// 렌더링			
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
					
					if( m_CharAlpha.GetState() == CHAR_HIDE )
					{
						switch( TwoPass )
						{
							case 0 :
								g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
								g_RenderManager.SetDestBlend( D3DBLEND_ONE  );
								g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
								break;
							case 1:
								g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
								g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA  );
								g_RenderManager.SetZFunc( D3DCMP_EQUAL );
								break;
						}
					}
					
					if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE )
					{
						g_RenderManager.SetAlphaTestEnable(FALSE);
					}			
					
					if(IsRender)
					{
						DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
							g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
					}

					if( m_CharAlpha.GetState() == CHAR_HIDE || m_curt_event.type == SM_STATE_DIE )
					{
						g_RenderManager.SetAlphaTestEnable(TRUE);
					}

				}
				else
				{
					m_bShadowDisplay = false;
					m_IsAllLTexture = FALSE;
				}
			}
		}

		if(!IsRender)
		{
			return;
		}

		//-----------------------------------------------------------------------------
		// 무기, 방패
		//-----------------------------------------------------------------------------		
		g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;		
		
		g_RenderManager.SetMaterial( &material );
#ifdef DIRECT_VERSION_9_MJH
		D3DLIGHT9 tem_Light;
#else
		D3DLIGHT8 tem_Light;
#endif // DIRECT_VERSION_9_MJH
	
		if(  m_bPC == true || 
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{

			bool bIsDrawWeapon = DrawWeaponCheckStatus(false);
	//		bool bIsDrawWeapon = DrawWeaponFade();
		
			if( bIsDrawWeapon )
			{
				BOOL IsArcherReady = FALSE;
				
				if( (!m_bPC && m_ani_index == n_Attack10_BowReady0 && m_pre_ani_number != -1 ) ||
					(!m_bPC && m_ani_index == n_Attack10_BowShoot0 && m_pre_ani_number != -1 &&
					 m_ani_frame < g_Pc_Manager.GetImpactFrame(this, m_ani_index) ) )
				{
					IsArcherReady = TRUE;
				}

				if(  (m_bPC && m_ani_index == n_Attack10_BowReady0 && m_pre_ani_number != -1 ) ||
				     (m_bPC && m_ani_index == n_Attack10_BowShoot0 && m_pre_ani_number != -1 &&
					  m_ani_frame < g_Pc_Manager.GetImpactFrame(this, m_ani_index) ) )
				{
					IsArcherReady = TRUE;
				}
				
				// ( 아처 공격시 활 그리기 ) 스킬시 , 공격대기 , 공격준비
				if(  ( m_curt_event.type != SM_USE_SKILL && IsArcherReady ) ||
					 ( ( m_curt_event.type == SM_BOW_ATTACK && 
					   ( m_curt_event.status == SM_BOW_WAIT || m_curt_event.status == SM_BOW_READY ) ) )
				  )
				{
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_matRHand );		
					g_Particle.m_ArrowDisplay.Render( g_lpDevice );
				}
				else if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				{
					if( m_curt_event.type == SM_USE_SKILL )
					{
						if(	m_curt_event.status != SM_CHANT_READY       &&
							m_curt_event.status != SM_CHANT_SPELLING    &&
							m_curt_event.status != SM_RESTSKILL_SITDOWN &&
							m_curt_event.status != SM_SELLSKILL_SITDOWN &&
							m_curt_event.status != SM_SELLSKILL_KEEPING &&
							m_curt_event.status != SM_SELLSKILL_STANDUP 
						  )	
						{
							BYTE MainClass = 0;
						
							if( GetUnique() == g_Pc.GetMainPCUniqe() )
							{
								MainClass = nRui->thePcParam.MainClass;
							}
							else
							{
								if(m_bPC)
								{
									MainClass = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theMainClass;
								}
								else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) 
								{
									MainClass = g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theMainClass;
								}
							}
						
							if( MainClass == Archer || MainClass == Sniper )
							{
								int ImpactFrame = g_Pc_Manager.GetImpactFrame( pDisplayChr, m_ani_index );
							
								if( m_ani_frame < ImpactFrame && ImpactFrame != 0 )	
								{
									g_RenderManager.SetTransform( D3DTS_WORLD, &m_matRHand );								
									g_Particle.m_ArrowDisplay.Render( g_lpDevice );
								}
							}
						}
					}
				}			
			
				//-----------------------------------------------------------------------------
				// 무기그리기
				//-----------------------------------------------------------------------------
			
	

				
				for(int i = 0; i < LINK_ITEM_COUNT; ++i )
				{
					// 화살통이 있는지 검사하기			
					itm_index = m_pUserInfo->ArmorInfo.index[i];
					
					if( itm_index != -1 )
					{
						// 모델, 텍스쳐 인덱스				
						mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
						tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;					
						
						if( m_curt_event.type == SM_ACT )
						{
							if( !m_curt_event.weaponView[i] )
							{
								continue;
							}
						}
						
						// 월드메트릭스설정
						switch( i )
						{				
							case 0:	// 오른손
								matWorld = m_matRHand;
								break;				

							case 1:	// 왼손
								matWorld = m_matLHand;
								break;					
						}				
					
						// 앉아있다
						bool bChange = false;
						
						if( m_pUserInfo->bDown == true )
							bChange = true;					
						// 등에 차고 있어야한다
						else if( m_pUserInfo->bAttack == false )
							bChange = true;	
					
						if( g_Pc_Manager.m_pItm[itm_index]->m_class == 1 && g_Pc_Manager.m_pItm[itm_index]->m_type == 3 )
						{
							bool bFind = true;
							
							if( m_pUserInfo->bAttack == true )
							{
								if( bChange == true )
									bFind = false;
								else
									bFind = true;
							}
							else
							{	
								if( bChange == true )
									bFind = false;
								else
									bFind = true;
							}
							if( bFind == true )
								matWorld = m_matChr;
							else
							{
								itm_index = g_Pc_Manager.GetEqualItemIndex( itm_index );
								
								if( itm_index == -1 )
									return;
								else
								{
									mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
									tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;
								}
							}
						}
					
						//-----------------------------------------------------------------------------
						// 텍스쳐 있는지 검사
						//-----------------------------------------------------------------------------
						if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
						{					
							// 발광옵션 ON(선택한 캐릭터)				
							bool bEmitter = false;
							
							if( m_nTableIndex != MAX_USER_INTABLE && g_Pc.m_Select.bPC == m_bPC && 
								g_Pc.m_Select.index == m_nTableIndex )
							{
								bEmitter = true;
								g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATEALPHA_ADDCOLOR );
								g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT );
							}
						
						
							
#ifdef EFFECT_WEPON_INCHANT 
							g_RenderManager.SetTexture( 1 , pTexture );
							
							g_RenderManager.SetTextureCoordIndex( 1 , 0 );
							
							g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_COUNT3 );
							g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
							g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
							g_RenderManager.SetTextureColorArg2( 0 , D3DTA_CURRENT );
							
							
							g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
							g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
							g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
							
							g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_SELECTARG1 );
							g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT     );
							
							
							//					g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADDSIGNED2X      );
							//					g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
							//					g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATE );
							g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD       );
							g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT     );
							g_RenderManager.SetTextureColorArg2( 1 , D3DTA_TEXTURE     );					
#endif		
					
							

							// 렌더링
							SetVibrationPos( &matWorld );
							g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
							g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pNormalVB, 
															 sizeof(OBJECTVERTEX) );
							g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
						
							if( m_CharAlpha.GetState() == CHAR_HIDE )
							{
								switch( TwoPass )
								{
									case 0 :
										{
											g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
											g_RenderManager.SetDestBlend( D3DBLEND_ONE );
											g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
											break;
										}
									case 1:
										{
											g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
											g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
											g_RenderManager.SetZFunc( D3DCMP_EQUAL );
											break;
										}
								}
							}
						
							///-- 무기류는 기본적으로 알파테스팅을 뺀다.
							g_RenderManager.SetAlphaTestEnable(FALSE);
							
							BOOL IsAlphaTestItem = FALSE;
							BOOL IsNormalRender = TRUE;
							
							///-- 예외적으로 수정구는 알파테스팅을 적용한다.
							///-- 아 쉬팍 수정구.. 어떤건.. 알파 먹어야 하고.. 
							///-- 어떤건 아니고... 기준도 없고... 
							///-- 알파 테스팅을 그냥 줘버리면... 은신효과로 들어가서..
							///-- 반투명 상태로 들어가면... 그냥 안보인다.
							if( i == 0 )
							{
								int RHand_index = m_pUserInfo->ArmorInfo.index[0];
								if( RHand_index != -1 ) 	
								{
									if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 || // 지팡이
										g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 )  // 수정구
									{
										IsAlphaTestItem = TRUE;
										
										if( m_CharAlpha.GetState() == CHAR_HIDE )
										{
											IsNormalRender = FALSE;
										}
									}			
								}
							}
							else if( i == 1 )
							{
								int LHand_index = m_pUserInfo->ArmorInfo.index[1];
								if( LHand_index != -1 ) 	
								{
									if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )  // 활
									{
										IsAlphaTestItem = TRUE;
										
										if( m_CharAlpha.GetState() == CHAR_HIDE )
										{
											IsNormalRender = FALSE;
										}
									}			
								}
							}
							
							g_RenderManager.GetLight( 0, &tem_Light );

							DWORD dAlphaRef = g_RenderManager.GetAlphaRef();
							DWORD dAlphaFunc = g_RenderManager.GetAlphaFunc();
							
							if( IsAlphaTestItem && IsNormalRender )
							{
								g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );
								g_RenderManager.SetAlphaTestEnable(TRUE);								
							}
							
							g_RenderManager.SetZBias( 0 );
							
							DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
								g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );

							
							
							
#ifdef EFFECT_WEPON_INCHANT
							//.......................................................................................................
							// 무기 강화 테스트 by wxywxy
							//.......................................................................................................
							g_RenderManager.DisableTextureStageState( 1 );
							g_RenderManager.SetTextureTransFormFlags( 1 , D3DTTFF_DISABLE );
							
							g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_MODULATE );
							g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
							g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );		
							
							g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_MODULATE );
							g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_CURRENT );
							g_RenderManager.SetTextureAlphaArg2( 0 , D3DTA_TEXTURE );
							g_RenderManager.SetTextureCoordIndex( 1 , 1 );
							//.......................................................................................................
#endif


							if( IsAlphaTestItem && !IsNormalRender )
							{
								g_RenderManager.SetLight( 0, &g_Pc_Manager.theCharacter_Light );
								g_RenderManager.SetAlphaTestEnable(TRUE);								
							}						
//////////////////////////////////////////////////////////////////////////
//							이부분 무기 이펙트시 들어가는 부분이다... 
//////////////////////////////////////////////////////////////////////////
							if( m_State.nWeaponLightType != nWeaponLightNone )
							{
								switch( m_State.nWeaponLightType )
								{
									case EF_SPECIAL_ITEM_EFFECT_PROMINAS:
									case EF_SPECIAL_ITEM_EFFECT_KNIGHT:
									case EF_SPECIAL_ITEM_EFFECT_SNIPER:
									case EF_BASE_ITEM_EFFECT_PROMINAS:
									case EF_BASE_ITEM_EFFECT_KNIGHT:
									case EF_BASE_ITEM_EFFECT_SNIPER:
																			

										{
											///-- 마법서, 방패, 활 - 왼손장착 아이템이 아니면..
											if( i != 1 )
											{
												continue;	
											}
									
										}
										break;
									case EF_SPECIAL_ITEM_EFFECT_PSYCHE:
									case EF_SPECIAL_ITEM_EFFECT_MERCENARY:
									case EF_SPECIAL_ITEM_EFFECT_SLAYER:
									case EF_SPECIAL_ITEM_EFFECT_PRIEST:
									case EF_SPECIAL_ITEM_EFFECT_HOLY:
									case EF_BASE_ITEM_EFFECT_PSYCHE:
									case EF_BASE_ITEM_EFFECT_MERCENARY:
									case EF_BASE_ITEM_EFFECT_SLAYER:
									case EF_BASE_ITEM_EFFECT_PRIEST:
									case EF_BASE_ITEM_EFFECT_HOLY:
								

										{
											///-- 수정구, 한손검, 대검, 지팡이 - 오른손 장착 아이템이 아니면..
											if( i != 0 )
											{
												continue;	
											}
										}
										break;		
								}	
								
								
								DWORD dtime = g_nowTime % 1000;
								int dAlpha = 0;
								int nEndAlpah = 255;

								if( dtime < 500 )
								{
									dAlpha = (int)( nEndAlpah * ( dtime / 500.0f ) );					
								}	
								else
								{
									dtime -= 500;
									dAlpha = nEndAlpah - (int)( nEndAlpah * ( dtime / 500.0f ) );						
								}																
								
								DWORD dColorOption = D3DTOP_MODULATE4X;

								DWORD dPreColorOption = g_RenderManager.GetTextureColorOP( 0 );
								DWORD dPreColorArg1 = g_RenderManager.GetTextureColorArg1( 0 );
								DWORD dPreColorArg2 = g_RenderManager.GetTextureColorArg2( 0 );

								DWORD dAlpOp = g_RenderManager.GetTextureAlphaOP( 0 );
								DWORD dAlpArg1 = g_RenderManager.GetTextureAlphaArg1( 0 );
								DWORD dAlpArg2 = g_RenderManager.GetTextureAlphaArg2( 0 );
								
								DWORD dMipFilter = 0;
								DWORD dMagFilter = 0;
								DWORD dMinFilter = 0;
#ifdef DIRECT_VERSION_9_MJH
								g_RenderManager.GetSamplerState(0, D3DSAMP_MIPFILTER, &dMipFilter);
								g_RenderManager.GetSamplerState(0, D3DSAMP_MAGFILTER, &dMagFilter);
								g_RenderManager.GetSamplerState(0, D3DSAMP_MINFILTER, &dMinFilter);
#else
								g_RenderManager.GetTextureStageState(0, D3DTSS_MIPFILTER, &dMipFilter);
								g_RenderManager.GetTextureStageState(0, D3DTSS_MAGFILTER, &dMagFilter);
								g_RenderManager.GetTextureStageState(0, D3DTSS_MINFILTER, &dMinFilter);
#endif // DIRECT_VERSION_9_MJH								

								g_RenderManager.SetTextureMipFilter(0, D3DTEXF_LINEAR );
								g_RenderManager.SetTextureMagFilter(0, D3DTEXF_LINEAR );
								g_RenderManager.SetTextureMinFilter(0, D3DTEXF_LINEAR );
								
								switch( m_State.nWeaponLightType )
								{
									case EF_SPECIAL_ITEM_EFFECT_PROMINAS:
									case EF_BASE_ITEM_EFFECT_PROMINAS:
										{
											g_RenderManager.SetTFactorColor( D3DCOLOR_ARGB( dAlpha, 150, 150, 150 ) );
											g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
											g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TFACTOR );
											dColorOption = D3DTOP_MODULATE2X;
										}
										break;								
									case EF_SPECIAL_ITEM_EFFECT_PSYCHE:									
									case EF_SPECIAL_ITEM_EFFECT_PRIEST:
									case EF_SPECIAL_ITEM_EFFECT_HOLY:
									case EF_BASE_ITEM_EFFECT_PSYCHE:									
									case EF_BASE_ITEM_EFFECT_PRIEST:
									case EF_BASE_ITEM_EFFECT_HOLY:
										{
											g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
											g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );										
											dColorOption = D3DTOP_MODULATE;
										}
										break;
									case EF_SPECIAL_ITEM_EFFECT_SNIPER:
									case EF_BASE_ITEM_EFFECT_SNIPER:
										{
											g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
											g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );										
											dColorOption = D3DTOP_MODULATE;
										}
										break;	
									default:
										{
											g_RenderManager.SetTFactorColor( D3DCOLOR_ARGB( dAlpha, 150, 150, 150 ) );
											g_RenderManager.SetTextureAlphaOP( 0 , D3DTOP_SELECTARG1 );
											g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TFACTOR );
											dColorOption = D3DTOP_MODULATE2X;
										}
										break;
								}						

								g_RenderManager.SetTextureColorOP( 0 , dColorOption );								
								g_RenderManager.SetTextureColorArg1( 0 , D3DTA_CURRENT );
								g_RenderManager.SetTextureColorArg2( 0 , D3DTA_TEXTURE );

								g_RenderManager.SetZBias( 2 );
							
								DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
									g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );

								g_RenderManager.SetZBias( 0 );

								g_RenderManager.SetTextureAlphaArg1( 0 , dAlpArg1 );
								g_RenderManager.SetTextureAlphaArg2( 0 , dAlpArg2 );
								
								g_RenderManager.SetTextureAlphaOP( 0 , dAlpOp );
								g_RenderManager.SetTextureColorOP( 0 , dPreColorOption );
								
								g_RenderManager.SetTextureColorArg1( 0 , dPreColorArg1 );
								g_RenderManager.SetTextureColorArg2( 0 , dPreColorArg2 );

								g_RenderManager.SetTextureMipFilter( 0 , dMipFilter );
								g_RenderManager.SetTextureMagFilter( 0 , dMagFilter );
								g_RenderManager.SetTextureMinFilter( 0 , dMinFilter );
							}
							
							g_RenderManager.SetAlphaTestEnable(TRUE);

							if(IsAlphaTestItem)
							{
								g_RenderManager.SetLight( 0, &tem_Light );
							}		
						}
						else
						{
							m_bShadowDisplay = false;							
						}
					}	
				}
			}
		}
	}	
	
	if( IsNpcObject )
	{
		g_RenderManager.SetAlphaBlendEnable( TRUE );
	}

	//-----------------------------------------------------------------------------
	// 모션블로어
	//-----------------------------------------------------------------------------
//	DisplayMotionBlow();
	g_chr_count++;
	
	// 플레이어가 수행해야 할 퀘스트를 가지고 있는 NPC이면 머리위에 느낌표를 띄운다.
	if( thePreQuestCodeList.size() )
	{
		if( m_ISQuestEffectCreated == FALSE )
		{
			switch( m_QuestMarkType )
			{
				case START_QUEST_MARK:
					g_Particle.SetEffSetCon( 0.0f , EF_QUEST_MARK   , 0 , EFFECT_SET_LOOP_TIME , GetUnique() , GetIndex() );
					m_ISQuestEffectCreated = TRUE;
					break;

				case REPORT_QUEST_MARK:
					g_Particle.SetEffSetCon( 0.0f , EF_QUEST_MARK2  , 0 , EFFECT_SET_LOOP_TIME , GetUnique() , GetIndex() );
					m_ISQuestEffectCreated = TRUE;
					break;

				case NONE_QUEST_MARK:
					{
						
						std::vector<QUESTDATA>::iterator ir;		
						
						ir = g_Map.m_Npc[idx].thePreQuestCodeList.begin();
						
						for( ; ir != g_Map.m_Npc[idx].thePreQuestCodeList.end() ; ++ir )
						{
							if( m_QuestMarkType != REPORT_QUEST_MARK )
							{
								m_QuestMarkType = ir->Type;
							}
						}				
						
						switch( m_QuestMarkType )
						{
							case START_QUEST_MARK:
								g_Particle.SetEffSetCon( 0.0f , EF_QUEST_MARK   , 0 , EFFECT_SET_LOOP_TIME , GetUnique() , GetIndex() );
								m_ISQuestEffectCreated = TRUE;
								break;
							
							case REPORT_QUEST_MARK:
								g_Particle.SetEffSetCon( 0.0f , EF_QUEST_MARK2  , 0 , EFFECT_SET_LOOP_TIME , GetUnique() , GetIndex() );
								m_ISQuestEffectCreated = TRUE;
								break;
						}
					}

					break;
			}

		}
	}
	else
	{
		if( m_ISQuestEffectCreated == TRUE )
		{
			// 퀘스트 이펙트 지우기
			g_Particle.DelEffSetCon( EF_QUEST_MARK  , 0  , GetUnique() , GetIndex() );
			g_Particle.DelEffSetCon( EF_QUEST_MARK2 , 0  , GetUnique() , GetIndex() );

			m_ISQuestEffectCreated = FALSE;			
		}
	}

#ifdef BOUND_BOX_DISPLAY

	if( m_CollisionBox.m_nVtxCount )
		m_CollisionBox.Render( m_matChr );


	g_RenderManager.SetFVF( D3DFVF_OBJECTVERTEX );	

	g_RenderManager.SetAlphaBlendEnable( TRUE );
	g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
	g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	g_RenderManager.SetCullMode( D3DCULL_NONE );

#endif

	if(m_State.bRide)
	{
		g_Map.m_Npc[m_State.TransIndex].Display(_byType, _bySort, true);
	}

	return;
}


void Character::RenderToAlpha(D3DXVECTOR3 vPos,float Alpha, BOOL IsRenderWeapon, SAImageData * psIDat)
{
	if( m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
		return;
	
	// 캐릭터가 가지고 있는 아이템 텍스쳐가 모두 생성된 상태이면 그려준다 
	// 헤어, 얼굴, 상의, 하의, 장갑, 신발, 무기
	int tex_idx = 0;
	int itm_idx = 0;
	int itm_cnt = 0;
	int idx = 0;

	Character*	pDisplayChr = NULL;
	pDisplayChr = this;

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}	

	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		itm_cnt = c_Max_WearItemType;		
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{		
			tex_idx = g_Pc_Manager.GetBasicTexIndex( this, idx );
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
		}
		
		for( idx = 0; idx < LINK_ITEM_COUNT; ++idx )
		{
			itm_idx = m_pUserInfo->ArmorInfo.index[idx];
			if( itm_idx != -1 )
			{
				tex_idx = g_Pc_Manager.m_pItm[itm_idx]->m_tex_index;	
				if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
				{
					return;
				}
			}
		}		
	}
	else 
	{
		itm_cnt = g_Pc_Manager.GetItmCount( this );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 메테리얼셋팅
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	material = g_Pc_Manager.theCharacter_Material;

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale,matWorld,matLocal;

	D3DXMatrixIdentity( &matLocal );
	
	matLocal = m_matChr;

	matLocal._41 = vPos.x;
	matLocal._42 = vPos.y;
	matLocal._43 = vPos.z;

	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 
	
	D3DXMatrixMultiply( &matWorld, &matScale, &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
	
	matLocal = matWorld;

	//.................................................................................................................
	// 아이템 갯수만큼 출력
	//.................................................................................................................
	int tex_index, mod_index, itm_index;;	
	int itm_count;//		= g_Pc_Manager.GetItmCount( this );
	int AttrIndex		= GetAttrIndex();	

	itm_count = g_Pc_Manager.GetItmCount( this );

	int  RenderMaxCount = 1;

	
	// 투명 부분
	RenderMaxCount = 2;

	material.Diffuse.a = Alpha;
	
	if(psIDat)
	{
		material.Ambient.r = material.Diffuse.r = psIDat->sDiffuse.r;
		material.Ambient.g = material.Diffuse.g = psIDat->sDiffuse.g;
		material.Ambient.b = material.Diffuse.b = psIDat->sDiffuse.b;
	}

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );

	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.SetMaterial( &material );

	//.................................................................................................................
	// 알파 블렌딩이면 Z 값 비교후 출력 ( 두번 출력 )
	//.................................................................................................................
	for( int TwoPass = 0 ; TwoPass < RenderMaxCount ; ++TwoPass )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD, &matLocal );	

		//.............................................................................................................
		// 아이템 갯수만큼
		//.............................................................................................................
		for( int i = 0; i < itm_count; ++i )
		{	
			if( m_pItm_index[i]	== -1 )
				continue;
			
			if(  CHAR_WEAR_SETITEM == m_Char_WearType  )
			{
				if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
				{
					continue;
				}
			}	

			//.........................................................................................................
			// 플레이어일때 아이템 출력
			//.........................................................................................................
			if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				&& m_Basic_DrawItm[i] )
			{
				mod_index = g_Pc_Manager.GetBasicModIndex( this, i );
				tex_index = g_Pc_Manager.GetBasicTexIndex( this, i );
				
				if( tex_index == -1 || mod_index == -1)
					continue;
				
				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
				{				
					// 버퍼에넣기					
					OBJECTVERTEX*	pOBJVERTEX;
					
					if( m_Basic_VtxBuffer[i].NormalVB == NULL )
						continue;
#ifdef DIRECT_VERSION_9_MJH
					if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
					if( FAILED( m_Basic_VtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH					
						return;		
					
					for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
					{
						memcpy( pOBJVERTEX[j].Vertex, m_Basic_VtxBuffer[i].pVertex[j], sizeof(m_Basic_VtxBuffer[i].pVertex[j]) );
						
						// 버퍼초기화안되었다면 노말, U,V값넣기
						if( m_Basic_VtxBuffer[i].bInit == false )
						{
							pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
							pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
							pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
						}						
					}

					if( m_Basic_VtxBuffer[i].bInit == false )
						m_Basic_VtxBuffer[i].bInit = true;

					m_Basic_VtxBuffer[i].NormalVB->Unlock();
					
					g_RenderManager.SetStreamSource( 0, m_Basic_VtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
					
					g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
					g_RenderManager.SetMaterial( &material );	
					
					// 렌더링			
					g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
					
					switch( TwoPass )
					{
						case 0 :
							g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
							g_RenderManager.SetDestBlend( D3DBLEND_ONE );
							g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
							break;
						case 1:
							g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
							g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
							g_RenderManager.SetZFunc( D3DCMP_EQUAL );
							break;
					}
					
					DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
						g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				}
				else
				{
					m_bShadowDisplay = false;
				}
			} 
			//.........................................................................................................
			
			//-----------------------------------------------------------------------------
			// 텍스쳐, 모델 인덱스
			//-----------------------------------------------------------------------------
			mod_index = g_Pc_Manager.GetModIndex( this, i );
			tex_index = g_Pc_Manager.GetTexIndex( this, i );
			
			if( tex_index == -1 || mod_index == -1)
				continue;
			
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			{				
				// 버퍼에넣기					
				OBJECTVERTEX*	pOBJVERTEX;
				
				if( m_pVtxBuffer[i].NormalVB == NULL )
					continue;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH				
				{
					return;		
				}
				
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
				{
					memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[i].pVertex[j], sizeof(m_pVtxBuffer[i].pVertex[j]) );
					
					// 버퍼초기화안되었다면 노말, U,V값넣기
					if( m_pVtxBuffer[i].bInit == false )
					{
						pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
						pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
						pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
					}						
				}
				
				if( m_pVtxBuffer[i].bInit == false )
					m_pVtxBuffer[i].bInit = true;

				m_pVtxBuffer[i].NormalVB->Unlock();
				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				
				g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
				g_RenderManager.SetMaterial( &material );					
				
				// 발광옵션 ON(선택한 캐릭터)			
				bool bEmitter = false;
				
				// 렌더링			
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				
				
				switch( TwoPass )
				{
				case 0 :
					g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
					g_RenderManager.SetDestBlend( D3DBLEND_ONE );
					g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
					break;
				case 1:
					g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
					g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
					g_RenderManager.SetZFunc( D3DCMP_EQUAL );
					break;
				}
				
				
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
					g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				
				// 발광옵션 OFF			
				if( true == bEmitter )				
					g_RenderManager.SetTextureColorOP( 1 , D3DTOP_DISABLE );						
			}
			//-----------------------------------------------------------------------------
			// 텍스쳐로딩이 되지 않은 경우가 있다면 그림자 플래그를 꺼야한다
			//-----------------------------------------------------------------------------
			else
				m_bShadowDisplay = false;
		}		

		g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;	
	
		g_RenderManager.SetMaterial( &material );
	
		if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			bool bIsDrawWeapon = DrawWeaponCheckStatus();

			if(!IsRenderWeapon)
			{
			   bIsDrawWeapon = IsRenderWeapon;
			}
		
			if( bIsDrawWeapon )
			{
				// ( 아처 공격시 활 그리기 ) 스킬시 , 공격대기 , 공격준비
				if(  ( !m_bPC && m_ani_index == n_Attack10_BowReady0 ) 
					 || ( ( m_curt_event.type == SM_BOW_ATTACK && 
					   ( m_curt_event.status == SM_BOW_WAIT || m_curt_event.status == SM_BOW_READY ) ) ||
					 ( 1 == GetWeaponType() && 
					   ( m_curt_event.status == SM_CHANT_READY || m_curt_event.status == SM_CHANT_SPELLING ) ) )
				  )
				{
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_matRHand );		
					g_Particle.m_ArrowDisplay.Render( g_lpDevice );
				}
				else if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				{
					if( m_curt_event.type == SM_USE_SKILL )
					{
						if(	m_curt_event.status != SM_CHANT_READY       &&
							m_curt_event.status != SM_CHANT_SPELLING    &&
							m_curt_event.status != SM_RESTSKILL_SITDOWN &&
							m_curt_event.status != SM_SELLSKILL_SITDOWN &&
							m_curt_event.status != SM_SELLSKILL_KEEPING &&
							m_curt_event.status != SM_SELLSKILL_STANDUP 
						  )	
						{
							BYTE MainClass = 0;
						
							if( GetUnique() == g_Pc.GetMainPCUniqe() )
							{
								MainClass = nRui->thePcParam.MainClass;
							}
							else
							{
								if(m_bPC)
								{
									MainClass = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theMainClass;
								}
								else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) 
								{
									MainClass = g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theMainClass;
								}
							}
						
							if( MainClass == Archer || MainClass == Sniper )
							{
								int ImpactFrame = g_Pc_Manager.GetImpactFrame( pDisplayChr, m_ani_index );
							
								if( m_ani_frame < ImpactFrame && ImpactFrame != 0 )	
								{
									g_RenderManager.SetTransform( D3DTS_WORLD, &m_matRHand );								
									g_Particle.m_ArrowDisplay.Render( g_lpDevice );
								}
							}
						}
					}
				}			
			
				//-----------------------------------------------------------------------------
				// 무기그리기
				//-----------------------------------------------------------------------------
				for(int i = 0; i < LINK_ITEM_COUNT; ++i )
				{
					// 화살통이 있는지 검사하기			
					itm_index = m_pUserInfo->ArmorInfo.index[i];
					
					if( itm_index != -1 )
					{
						// 모델, 텍스쳐 인덱스				
						mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
						tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;					
						
						if( m_curt_event.type == SM_ACT)
						{
							if( !m_curt_event.weaponView[i] )
							{
								continue;
							}
						}
						
						// 월드메트릭스설정
						switch( i )
						{				
							case 0:	// 오른손
								matWorld = m_matRHand;
								break;				

							case 1:	// 왼손
								matWorld = m_matLHand;
								break;					
						}				
					
						// 앉아있다
						bool bChange = false;
						
						if( m_pUserInfo->bDown == true )
							bChange = true;					
						// 등에 차고 있어야한다
						else if( m_pUserInfo->bAttack == false )
							bChange = true;	
					
						if( g_Pc_Manager.m_pItm[itm_index]->m_class == 1 && g_Pc_Manager.m_pItm[itm_index]->m_type == 3 )
						{
							bool bFind = true;
							
							if( m_pUserInfo->bAttack == true )
							{
								if( bChange == true )
									bFind = false;
								else
									bFind = true;
							}
							else
							{	
								if( bChange == true )
									bFind = false;
								else
									bFind = true;
							}
							if( bFind == true )
								matWorld = m_matChr;
							else
							{
								itm_index = g_Pc_Manager.GetEqualItemIndex( itm_index );
								
								if( itm_index == -1 )
									return;
								else
								{
									mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
									tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;
								}
							}
						}
					
						//-----------------------------------------------------------------------------
						// 텍스쳐 있는지 검사
						//-----------------------------------------------------------------------------
						if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
						{					
							// 발광옵션 ON(선택한 캐릭터)				
							bool bEmitter = false;
							if( m_nTableIndex != MAX_USER_INTABLE && g_Pc.m_Select.bPC == m_bPC && g_Pc.m_Select.index == m_nTableIndex )
							{
								bEmitter = true;
								g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_MODULATEALPHA_ADDCOLOR );
								g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT );
							}
						
							// 렌더링
							SetVibrationPos( &matWorld );
							g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
							g_RenderManager.SetStreamSource( 0, g_Pc_Manager.m_pItm[itm_index]->m_pNormalVB, sizeof(OBJECTVERTEX) );
							g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
						
							if( m_CharAlpha.GetState() == CHAR_HIDE )
							{
								switch( TwoPass )
								{
									case 0 :
									g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
									g_RenderManager.SetDestBlend( D3DBLEND_ONE );
									g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
									break;

								case 1:
									g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
									g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
									g_RenderManager.SetZFunc( D3DCMP_EQUAL );
									break;
								}
							}							
						
							DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
								g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );				
						
							// 발광옵션 OFF
							if( true == bEmitter )				
								g_RenderManager.SetTextureColorOP( 1 , D3DTOP_DISABLE );
						}
					}	
				}
			}
		}
	} 
	
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	return;
}


void Character::RenderToAlphaEffect(D3DXVECTOR3 vPos,float Alpha, float Scale, D3DXCOLOR Color , BOOL IsRenderWeapon /* = TRUE */, SAImageData * psIDat /* = NULL */)
{
	if( m_CharAlpha.GetState() == CHAR_NONE_DISPLAY )
		return;
	
	// 캐릭터가 가지고 있는 아이템 텍스쳐가 모두 생성된 상태이면 그려준다 
	// 헤어, 얼굴, 상의, 하의, 장갑, 신발, 무기
	int tex_idx = 0;
	int itm_idx = 0;
	int itm_cnt = 0;
	int idx = 0;


	Character*	pDisplayChr = NULL;
	pDisplayChr = this;

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}	

	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		itm_cnt = c_Max_WearItemType;		
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{		
			tex_idx = g_Pc_Manager.GetBasicTexIndex( this, idx );
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
		}
		
		for( idx = 0; idx < LINK_ITEM_COUNT; ++idx )
		{
			itm_idx = m_pUserInfo->ArmorInfo.index[idx];
			if( itm_idx != -1 )
			{
				tex_idx = g_Pc_Manager.m_pItm[itm_idx]->m_tex_index;	
				if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
				{
					return;
				}
			}
		}		
	}
	else 
	{
		itm_cnt = g_Pc_Manager.GetItmCount( this );
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->SetTexture( byType, tex_idx ) == false )
			{
				return;
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 메테리얼셋팅
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH
	
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	material = g_Pc_Manager.theCharacter_Material;

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale,matWorld,matLocal;

	D3DXMatrixIdentity( &matLocal );
	
	matLocal = m_matChr;


	matLocal._41 = vPos.x;
	matLocal._42 = vPos.y;
	matLocal._43 = vPos.z;

	D3DXMatrixScaling( &matScale, m_vScale.x*Scale, m_vScale.y*Scale, m_vScale.z*Scale ); 
	
	D3DXMatrixMultiply( &matWorld, &matScale, &matLocal );

	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	
	
	matLocal = matWorld;

	//.................................................................................................................
	// 아이템 갯수만큼 출력
	//.................................................................................................................
	int tex_index, mod_index;//itm_index;
	int itm_count;//		= g_Pc_Manager.GetItmCount( this );
	int AttrIndex		= GetAttrIndex();	

	itm_count = g_Pc_Manager.GetItmCount( this );

	int  RenderMaxCount = 1;
	
	// 투명 부분
	RenderMaxCount = 2;

	material.Diffuse.a = Alpha;
	material.Ambient.r = material.Diffuse.r = Color.r ;
	material.Ambient.g = material.Diffuse.g = Color.g ;
	material.Ambient.b = material.Diffuse.b = Color.b ;

	g_RenderManager.SetZWriteEnable( TRUE );
	g_RenderManager.SetZEnable( TRUE );
	g_RenderManager.SetCullMode( D3DCULL_CCW );
	g_RenderManager.SetAlphaBlendEnable( TRUE );

	g_RenderManager.SetMaterial( &material );

	//.................................................................................................................
	// 알파 블렌딩이면 Z 값 비교후 출력 ( 두번 출력 )
	//.................................................................................................................
	for( int TwoPass = 0 ; TwoPass < RenderMaxCount ; ++TwoPass )
	{
		g_RenderManager.SetTransform( D3DTS_WORLD, &matLocal );	

		//.............................................................................................................
		// 아이템 갯수만큼
		//.............................................................................................................
		for( int i = 0; i < itm_count; ++i )
		{	
			if( m_pItm_index[i]	== -1 )
				continue;
			
			if(  CHAR_WEAR_SETITEM == m_Char_WearType  )
			{
				if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
				{
					continue;
				}
			}	
	
			//.........................................................................................................
			
			//-----------------------------------------------------------------------------
			// 텍스쳐, 모델 인덱스
			//-----------------------------------------------------------------------------
			mod_index = g_Pc_Manager.GetModIndex( this, i );
			tex_index = g_Pc_Manager.GetTexIndex( this, i );
			
			if( tex_index == -1 || mod_index == -1)
				continue;
			
			//-----------------------------------------------------------------------------
			// 텍스쳐 있는지 검사
			//-----------------------------------------------------------------------------
			if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			{				
				// 버퍼에넣기					
				OBJECTVERTEX*	pOBJVERTEX;
				
				if( m_pVtxBuffer[i].NormalVB == NULL )
					continue;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH				
				{
					return;		
				}
				
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++j )
				{
					memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[i].pVertex[j], sizeof(m_pVtxBuffer[i].pVertex[j]) );
					
					// 버퍼초기화안되었다면 노말, U,V값넣기
					if( m_pVtxBuffer[i].bInit == false )
					{
						pOBJVERTEX[j].Normal = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_normal;
						pOBJVERTEX[j].u		 = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_u;
						pOBJVERTEX[j].v	     = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[j].m_v;
					}						
				}
				
				if( m_pVtxBuffer[i].bInit == false )
					m_pVtxBuffer[i].bInit = true;

				m_pVtxBuffer[i].NormalVB->Unlock();
				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );
				
				g_Pc_Manager.theCharacter_Material.Diffuse.a = material.Diffuse.a;
				g_RenderManager.SetMaterial( &material );					
				
				// 발광옵션 ON(선택한 캐릭터)			
				bool bEmitter = false;
				
				// 렌더링			
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod_index]->m_pIB, 0 );
				
				
				switch( TwoPass )
				{
				case 0 :
					g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
					g_RenderManager.SetDestBlend( D3DBLEND_ONE );
					g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
					break;
				case 1:
					g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
					g_RenderManager.SetDestBlend( D3DBLEND_ONE  );
					g_RenderManager.SetZFunc( D3DCMP_EQUAL );
					break;
				}
				
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex, 
					g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
				
				// 발광옵션 OFF			
				if( true == bEmitter )				
					g_RenderManager.SetTextureColorOP( 1 , D3DTOP_DISABLE );						
			}
			//-----------------------------------------------------------------------------
			// 텍스쳐로딩이 되지 않은 경우가 있다면 그림자 플래그를 꺼야한다
			//-----------------------------------------------------------------------------
			else
				m_bShadowDisplay = false;
		}		

	}
	g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

	return;
}


bool Character::DisplayAfterImage(SAImageData & sIData)
{
	// 애니번호 없거나 버퍼 NULL이면	
	if( -1 == sIData.n_ani_number || NULL == m_pVtxBuffer[0].NormalVB )
		return false;
	
    int nNext_frame = 0;
	int nCurt_frame = 0;
	
	float factor = 0.0f;
	bool bMotionBlend = true;
	
	int MotionBlendFrame = 0;
	int MotionBlendHalfFrame = 0;

	MotionBlendFrame = c_MotionBlendFrame + sIData.n_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + sIData.n_start_blend_frame;

	int pre_ani_number = sIData.n_pre_ani_number;
	int pre_end_frame = sIData.n_pre_end_frame;
	int ani_frame = sIData.n_ani_frame;

	///--  현재 블렌딩 도중이라면....
	if( sIData.n_blend_frame < c_MotionBlendFrame )
	{
		nCurt_frame = sIData.n_blend_frame + sIData.n_start_blend_frame;
		nNext_frame = sIData.n_blend_frame + sIData.n_start_blend_frame + 1;

		if( nCurt_frame >= MotionBlendFrame )
		{
			pre_ani_number = -1;
			pre_end_frame = 0;
		}

		if( ani_frame >= MotionBlendHalfFrame)		// 이어질 모션을 해당 프레임까지만 블렌딩 시킨다 
		{
			ani_frame = MotionBlendHalfFrame;
		}
	}
	///-- 블렌딩이 종료되었다면...
	else										// 블렌딩 끝났으니 실제 애니메이션 플레이...
	{
		nCurt_frame = ani_frame;
		nNext_frame = ani_frame + 1;	

		pre_ani_number = -1;
		pre_end_frame = 0;
	}
	
	if( nNext_frame >= sIData.n_end_frame )
	{
		nNext_frame = sIData.n_end_frame;
	}

	int	ani, pre, mod, id;
	ani = sIData.n_ani_number;
	pre = pre_ani_number;
	
	float middle_time = sIData.f_middle_time;	
	
	// 이전애니메이션 없다
	if( pre == -1 )
	{
		pre = ani;
		bMotionBlend = false;		
	}
	else
	{
		factor = (float)(nCurt_frame - sIData.n_start_blend_frame)
				  / (MotionBlendFrame - sIData.n_start_blend_frame);
	}

	
	D3DXMATRIX matTemp;
	D3DXQUATERNION qtRot;		
	int	nItmCount;

	nItmCount = g_Pc_Manager.GetItmCount( this );

	
	if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
		( NULL == g_Pc_Manager.m_pAni[pre] ) )
	{
		return false;			
	}

	if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) ||
		( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) )
	{
		return false;
	}

	for( int i = 0; i < nItmCount; ++i )
	{			
	
		if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
			&& m_Basic_DrawItm[i] )
		{
			mod = g_Pc_Manager.GetBasicModIndex( this, i );			
			
			for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
			{		
				id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;

				if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id || g_Pc_Manager.m_pAni[pre]->m_num_node <= id )
					continue;
				
				if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation ) || 
					( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation ) )
				{
					return false;
				}
				
				if( bMotionBlend )
				{
					blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
					
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.x ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.y ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.z ) * factor ) +
									g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.z;
				}
				else
				{
					D3DXQuaternionSlerp( &qtRot, 
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_qtRot,
										&g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_qtRot,
										m_middle_time );		
					
					// 쿼터니온 -> 메트릭스
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
										
					//-----------------------------------------------------------------------------
					// 위치값
					//-----------------------------------------------------------------------------
					matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.x -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.x ) * middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.x;
					
					matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.y -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.y ) * middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.y;
					
					matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.z -
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.z ) * middle_time ) +
									g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.z;
				}
				
				//-----------------------------------------------------------------------------
				// 버텍스 x 메트릭스 = 애니 버텍스
				//-----------------------------------------------------------------------------
				D3DXVec3TransformCoord( &m_Basic_VtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
			}
		}

		if(  n_Wear_Serve != i  )
		{
			//..........................................................................................
			// 셋트 아이템 하의 부츠 장갑 스키닝 애니메이션 막기 by simwoosung			// 
			//..........................................................................................
			if( ( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
				&& ( CHAR_WEAR_SETITEM == m_Char_WearType ) )
			{
				if( n_Wear_PantsArmor == i || n_Wear_Gloves == i || n_Wear_Shoes == i )
				{					
					continue;			
				}
			}				
		}

		if( m_pItm_index[i]	== -1 )
			continue;
		
		//-----------------------------------------------------------------------------
		// 모델 인덱스
		//-----------------------------------------------------------------------------		
		mod = g_Pc_Manager.GetModIndex( this, i );		

		// 버텍스 갯수만큼 루프돌기
		for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
		{				
			//-----------------------------------------------------------------------------
			// 뼈대에 연결된 ID
			//-----------------------------------------------------------------------------
			id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;

			//배열 영역에서 벗어나면 by simwoosung
			if( !( ( 0 <= ani ) && (ani < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
			{
				return false;
			}

			if( !( ( 0 <= pre ) && (pre < (c_Chr_Ani_Count+c_Npc_Ani_Count) ) ) )
			{
				return false;
			}
			
			//링크 포인터값이 NULL 이면 by simwoosung
			if( ( NULL == g_Pc_Manager.m_pAni[ani] ) || 
				( NULL == g_Pc_Manager.m_pAni[pre] ) )
			{
				return false;
			}
			
			if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode ) || 
				( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode ) )
			{
				return false;
			}

			//배열 영역 체크
			if( ( id < 0 ) || ( g_Pc_Manager.m_pAni[pre]->m_num_node <= id ) ||
	            ( g_Pc_Manager.m_pAni[ani]->m_num_node <= id )  )
			{
				return false;
			}

			if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id || g_Pc_Manager.m_pAni[pre]->m_num_node <= id )
			{
				continue;
			}
			
			if( ( NULL == g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation ) || 
				( NULL == g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation ) )
			{
				return false;
			}


			// 쿼터니온 중간값 구하기			
			if( bMotionBlend )
			{
				blendQuaternion( &qtRot, factor, 
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_qtRot, 
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_qtRot );

				// 쿼터니온 -> 메트릭스
				D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
				
				//-----------------------------------------------------------------------------
				// 위치값
				//-----------------------------------------------------------------------------
				matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.x ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.x;

				matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.y ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.y;

				matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.z ) * factor ) +
								g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[pre_end_frame].m_vPos.z;
			}
			else
			{
				D3DXQuaternionSlerp( &qtRot, 
									 &g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_qtRot,
									 &g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_qtRot,
									 m_middle_time );		

				// 쿼터니온 -> 메트릭스
				D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
				
				//-----------------------------------------------------------------------------
				// 위치값
				//-----------------------------------------------------------------------------
				matTemp._41 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.x -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.x ) * middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.x;
				
				matTemp._42 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.y -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.y ) * middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.y;
				
				matTemp._43 = (( g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nNext_frame].m_vPos.z -
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.z ) * middle_time ) +
								g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[nCurt_frame].m_vPos.z;
			}
			
			//-----------------------------------------------------------------------------
			// 버텍스 x 메트릭스 = 애니 버텍스
			//-----------------------------------------------------------------------------
			D3DXVec3TransformCoord( &m_pVtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
		}
	}

	BOOL IsWeaponAImage = (BOOL)(sIData.IsAfterWImage);
	
	if(IsWeaponAImage)
	{
		//-----------------------------------------------------------------------------
		// 무기, 방패 애니메이션(현재 지원하는 링크갯수 4개)
		//-----------------------------------------------------------------------------
		if( m_bPC == true || 
			( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
		{
			//-----------------------------------------------------------------------------
			// 무기에 사용
			//-----------------------------------------------------------------------------
			D3DXMATRIX	matRot;

			///-- 오른손 부분은 반드시 구한다.
			ItemAnimation(sIData, 0 , false, nNext_frame, nCurt_frame );
			
			for(int i = 1; i < LINK_ITEM_COUNT; ++i )
			{			
				int itm_index = m_pUserInfo->ArmorInfo.index[i];
				
				if( itm_index != -1 )
				{
					ItemAnimation(sIData, i, false, nNext_frame, nCurt_frame );
				}
			}		
		}
	}
		
	D3DXVECTOR3 vPos = D3DXVECTOR3(sIData.sPos.x, sIData.sPos.y, sIData.sPos.z);
	
	RenderToAlpha(vPos, sIData.fAlpha, IsWeaponAImage, &sIData);

	return TRUE;
}

// 아이템 애니메이션
void Character::ItemAnimation(SAImageData & sIData, int type, bool bChange, int next_frame, int curt_frame)
{
	int	node;
	D3DXQUATERNION	qtRot;
	D3DXMATRIX		matReady;
	D3DXMATRIX		matTemp;
	D3DXMatrixIdentity( &matTemp  );
	D3DXMatrixIdentity( &matReady );

	switch( type )
	{
		//......................................................................................................
		// 오른손
		//......................................................................................................
		case 0:
		{
			int	rhand_itm_index = m_pUserInfo->ArmorInfo.index[0];				
			int	lhand_itm_index = m_pUserInfo->ArmorInfo.index[1];			
		
			if( GetAttrIndex() == n_Millena_Man || GetAttrIndex() == n_Rain_Man )
			{
				if( rhand_itm_index== -1 )
				{
					m_pUserInfo->ArmorInfo.RHand = 45;
				}
				else
				{				
					if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 0 )
						m_pUserInfo->ArmorInfo.RHand = 45;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 1 )			
						m_pUserInfo->ArmorInfo.RHand = 45;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 2 )
						m_pUserInfo->ArmorInfo.RHand = 45;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 6 )
						m_pUserInfo->ArmorInfo.RHand = 45;			
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 7 )
						m_pUserInfo->ArmorInfo.RHand = 45; // 23;
				}
			}	
			else if( GetAttrIndex() == n_Millena_Woman || GetAttrIndex() == n_Rain_Woman )
			{
				if( rhand_itm_index== -1 )
				{
					m_pUserInfo->ArmorInfo.RHand = 59;
				}
				else
				{					
					if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 0 )
						m_pUserInfo->ArmorInfo.RHand = 59;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 1 )			
						m_pUserInfo->ArmorInfo.RHand = 59;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 2 )
						m_pUserInfo->ArmorInfo.RHand = 59;
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 6 )
						m_pUserInfo->ArmorInfo.RHand = 59;			
					else if( g_Pc_Manager.m_pItm[rhand_itm_index]->m_type == 7 )
						m_pUserInfo->ArmorInfo.RHand = 59; // 29;
				}
			}
		}
		break;

		case 1:
		{
			int	lhand_itm_index = m_pUserInfo->ArmorInfo.index[1];
			
			if(lhand_itm_index == -1)
			{
				return;
			}
			
			if( GetAttrIndex() == n_Millena_Man || GetAttrIndex() == n_Rain_Man )
			{
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 1 )	
				{					
					m_pUserInfo->ArmorInfo.LHand = 15; // 15;
				}
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 3 )			
					m_pUserInfo->ArmorInfo.LHand = 14;
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 5 )			
					m_pUserInfo->ArmorInfo.LHand = 45; // 15;
			}
			else if( GetAttrIndex() == n_Millena_Woman || GetAttrIndex() == n_Rain_Woman )
			{
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 1 )			
				{
					m_pUserInfo->ArmorInfo.LHand = 21;	
				}
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 3 )			
					m_pUserInfo->ArmorInfo.LHand = 20;
				if( g_Pc_Manager.m_pItm[lhand_itm_index]->m_type == 5 )			
					m_pUserInfo->ArmorInfo.LHand = 59; // 21;
			}
		}
			break;
	}

	int AttrIndex = GetAttrIndex();
	if( AttrIndex == -1 )
	{
		return;
	}

	if(  m_bPC == true ||
		( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( type )
		{		
		case 0:				
			node = m_pUserInfo->ArmorInfo.RHand;
			if( m_curt_event.type == SM_ACT)
			{
				if( m_curt_event.weaponView[0] )
				{
					node = g_Pc_Manager.m_PcAttr[ AttrIndex ].r_hand_node;
				}
			}		
			break;			
		case LINK_ITEM_COUNT:				
			node = m_pUserInfo->ArmorInfo.RHand;				
			break;
		case 1:				
			node = m_pUserInfo->ArmorInfo.LHand;
			if( m_curt_event.type == SM_ACT)
			{
				if( m_curt_event.weaponView[1] )
				{
					node = g_Pc_Manager.m_PcAttr[ AttrIndex ].l_hand_node;
				}
			}
			break;
		default:				
			node = type;				
		}
	}
	else
	{
		node = type;
	}

	float factor = 0.0f;
	bool bMotionBlend = true;

	int MotionBlendFrame = 0;
	int MotionBlendHalfFrame = 0;

	MotionBlendFrame = c_MotionBlendFrame + sIData.n_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + sIData.n_start_blend_frame;



	int ani_num = sIData.n_ani_number;
	int pre_ani_num = sIData.n_pre_ani_number;
	int pre_end_frame = sIData.n_pre_end_frame;
	float middle_time = sIData.f_middle_time;
	
	// 이전애니메이션 없다
	if( pre_ani_num == -1 )
	{
		pre_ani_num = ani_num;
		bMotionBlend = false;		
	}
	else
	{
		factor = (float)(curt_frame - sIData.n_start_blend_frame) 
			      /(MotionBlendFrame - sIData.n_start_blend_frame);
	}
	
	if( g_Pc_Manager.m_pAni[ani_num] == NULL || g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node == NULL )
	{
		return;
	}

	//-----------------------------------------------------------------------------
	// 현재 노드 회전값
	//-----------------------------------------------------------------------------	
	if( node >= g_Pc_Manager.m_pAni[ani_num]->m_num_node || 
		node >= g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node )
		return;

	if( bMotionBlend )
	{
		blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
	}
	else
	{
		// 쿼터니온 중간값 구하기
		D3DXQuaternionSlerp( &qtRot, 
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_qtRot,
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_qtRot,
							 middle_time );
	}

	// 쿼터니온 -> 메트릭스 ( 현재 회전각 알아내기 )
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );		

	//-----------------------------------------------------------------------------
	// 현재 동작의 로칼메트릭스
	//-----------------------------------------------------------------------------	
	{
		if( m_bPC == true ||
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
		{
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, node ), &matTemp );
		}
		else		
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, node ), &matTemp );		
	}

	if( bMotionBlend )
	{	
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.x ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.x;
		
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.y ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.y;
		
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.z ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[node].m_pAnimation[pre_end_frame].m_vPos.z;
	}
	else
	{
		//-----------------------------------------------------------------------------
		// 위치값
		//-----------------------------------------------------------------------------
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.x -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.x ) * middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.x;			 
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.y -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.y ) * middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.y;
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[next_frame].m_vPos.z -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.z ) * middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[node].m_pAnimation[curt_frame].m_vPos.z;
	}		
	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );

	//-----------------------------------------------------------------------------
	// 오브젝트 회전한 만큼 메트릭스 회전하기
	//-----------------------------------------------------------------------------
	D3DXQuaternionRotationMatrix( &qtRot, &m_matChr );	
	D3DXMatrixRotationQuaternion( &matTemp2, &qtRot );
	
	//-----------------------------------------------------------------------------
	// 월드상의 위치값 더하기
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
	matTemp._41 += sIData.sPos.x;
	matTemp._42 += sIData.sPos.y;
	matTemp._43 += sIData.sPos.z;	
	
	//-----------------------------------------------------------------------------
	// 등에 차고 있다면.. 해당 지정값만큼 곱한다 
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matReady, &matTemp );

	//-----------------------------------------------------------------------------
	// 노드메트릭스에 저장
	//-----------------------------------------------------------------------------
	if(  m_bPC == true ||
		( m_bPC == false && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		switch( type )
		{		
		case 0:			
			m_matRHand = matTemp;			
			break;
		case LINK_ITEM_COUNT:			
			m_matRHand2 = matTemp;			
			break;
		case 1:			
			m_matLHand = matTemp;			
			break;
		default:			
			m_matRHand = matTemp;			
			break;
		}
	}
	else
	{
		m_matRHand = matTemp;
	}
}

//-----------------------------------------------------------------------------
// Desc: 모션블로어
//
// 1. 자기캐릭터만 처리하며 특정상황에서만 나오도록 한다
//    (퍼포먼스를 상당히 느리게 하기 때문에...)
// 2. 첫프레임에서 현재프레임까지 중간프레임을 그린다
// 3. 단계에 따라서 알파를 서서히 주어서 효과를 낸다
//
// Copyright (c) 2003.11.19 By KStaR79
//-----------------------------------------------------------------------------
#define		MOTIONBLOW_LOOP_COUNT	6

bool Character::DisplayMotionBlow()
{
	//-----------------------------------------------------------------------------
	// 모션블로어하는 대상, 애니메이션 인덱스 찾기
	// 현재는 자기캐릭터만 하는데.. 나중에 수정될 것을 생각해서.. 변수인자 남겨둔다.
	//-----------------------------------------------------------------------------
	bool bFind = false;	
	if( m_bPC == true )
	{
		if( m_curt_event.type == SM_USE_SKILL )
		{
			// 해당스킬의 애니메이션효과있니?
			// 0이면 없음, 1이면 모션블로어
/*			BYTE byType = g_Skill.GetSkillAniEffect( m_curt_event.motion );
			switch( byType )
			{
			case 0:	// 없음
				return false;
			case 1:	// 모션블로어
				bFind = true;
				break;
			default:
				return false;
			}
			*/
		}
	}
//	if( bFind == false )
//		return false;

	//-----------------------------------------------------------------------------
	// 시작프레임을 1로해야 이전프레임이 0이되어서 문제가 생기지 않는다
	//-----------------------------------------------------------------------------
	D3DXMATRIX		matTemp;
	D3DXQUATERNION	qtRot;
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9	material;
#else
	D3DMATERIAL8	material;
#endif // DIRECT_VERSION_9_MJH
	int next_frame, curt_frame, ani, pre, mod, tex, id;	
	int AttrIndex = GetAttrIndex();	
	
	//-----------------------------------------------------------------------------
	// 루프되는 총프레임을 MOTIONBLOW_LOOP_COUNT 제한하여서 
	// 모션블로어 동작이 사라지는 효과를 준다.
	//-----------------------------------------------------------------------------
	int frame_loop = 1;

	if( m_ani_frame > MOTIONBLOW_LOOP_COUNT )
		frame_loop += (m_ani_frame-MOTIONBLOW_LOOP_COUNT);

	int itm_count = 0;
	
	itm_count = g_Pc_Manager.GetItmCount( this );
	
	for( frame_loop; frame_loop < m_ani_frame; frame_loop++ )
	{	
		//-----------------------------------------------------------------------------
		// 월드매트릭스셋팅(예외처리)
		//-----------------------------------------------------------------------------
		switch( m_curt_event.motion )
		{
		case ARCHER_SKILL_50:	// 백스텝
			{
				D3DXMATRIX	matWorld = m_matChr;
				matWorld._41 -= (m_curt_event.vPos.x*(m_ani_frame-frame_loop));
				matWorld._42 -= (m_curt_event.vPos.y*(m_ani_frame-frame_loop));
				matWorld._43 -= (m_curt_event.vPos.z*(m_ani_frame-frame_loop));
				g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
			}
			break;
		}

		//-----------------------------------------------------------------------------
		// 현재, 다음프레임
		//-----------------------------------------------------------------------------
		next_frame = frame_loop;
		curt_frame = frame_loop-1;		
		
		//-----------------------------------------------------------------------------
		// 캐릭터 애니메이션 버퍼 만들기
		//-----------------------------------------------------------------------------		
		for( int i = 0; i < itm_count; ++i )
		{			
			//-----------------------------------------------------------------------------
			// 모델 인덱스
			//-----------------------------------------------------------------------------
			mod = g_Pc_Manager.GetModIndex( this, i );

			if( mod == -1)
				continue;

			//-----------------------------------------------------------------------------
			// 연결된 애니메이션번호 
			//-----------------------------------------------------------------------------
			ani = m_ani_number;
			pre = m_ani_number;
			
			//-----------------------------------------------------------------------------
			// 버텍스 갯수만큼 루프돌기
			//-----------------------------------------------------------------------------
			for( unsigned int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
			{						
				// 뼈대에 연결된 ID
				id = g_Pc_Manager.m_pMod[mod]->m_pNodeID[j].ID;				
				if( g_Pc_Manager.m_pAni[ani]->m_num_node <= id )
					continue;
				
				//-----------------------------------------------------------------------------
				// 자기캐릭터... 머리와 연결된 버텍스
				//-----------------------------------------------------------------------------
				bool bFind = false;
				/*
				if( g_RockClient.GetGameMode() == GMODE_NORMAL && m_bPC == true && id == HEAD_INDEX )
				{
					// 현재 이벤트에 따라서 적용
					switch( m_curt_event.type )
					{
					case NULL:
						bFind = true;
						break;
					case SM_MOVE_BROADCAST:
						bFind = true;
						break;
					}
					if( bFind == true )
						matTemp = HeadAnimation();
				}
				*/
				//-----------------------------------------------------------------------------
				// 머리회전 안했다면.. 회전쿼터니온 계산하기
				//-----------------------------------------------------------------------------
				if( bFind == false )
				{
					D3DXQuaternionSlerp( &qtRot, 
										 &g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_qtRot,
										 &g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[next_frame].m_qtRot,
										 0.5f );		
					
					// 쿼터니온 -> 메트릭스
					D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
				}
				
				//-----------------------------------------------------------------------------
				// 위치값
				//-----------------------------------------------------------------------------
				matTemp._41 = (( g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[next_frame].m_vPos.x -
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x ) * 0.5f ) +
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.x;

				matTemp._42 = (( g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[next_frame].m_vPos.y -
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y ) * 0.5f ) +
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.y;

				matTemp._43 = (( g_Pc_Manager.m_pAni[pre]->m_pNode[id].m_pAnimation[next_frame].m_vPos.z -
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z ) * 0.5f ) +
								 g_Pc_Manager.m_pAni[ani]->m_pNode[id].m_pAnimation[curt_frame].m_vPos.z;

				//-----------------------------------------------------------------------------
				// 버텍스 x 메트릭스 = 애니 버텍스
				//-----------------------------------------------------------------------------
				D3DXVec3TransformCoord( &m_pVtxBuffer[i].pVertex[j], &g_Pc_Manager.m_pMod[mod]->m_pVertex[j].m_vertex, &matTemp );
			}	
		}	

		//-----------------------------------------------------------------------------
		// 일반 렌더링
		//-----------------------------------------------------------------------------		
		g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
		for(int i = 0; i < itm_count; ++i )
		{			
			//-----------------------------------------------------------------------------
			// 메터리얼셋팅(프레임에 따라서 알파효과준다)
			//-----------------------------------------------------------------------------
			material = m_pMaterial[i];
			material.Diffuse.a = ((float)frame_loop/(float)m_ani_frame)*0.5f;
			g_RenderManager.SetMaterial( &material );

			//-----------------------------------------------------------------------------
			// 텍스쳐, 모델 인덱스
			//-----------------------------------------------------------------------------
			mod = g_Pc_Manager.GetModIndex( this, i );
			tex = g_Pc_Manager.GetTexIndex( this, i );
			if( mod == -1 || tex == -1)
				continue;
			
			//-----------------------------------------------------------------------------
			// 텍스쳐관리자에 텍스쳐셋팅하기
			//-----------------------------------------------------------------------------
			if( g_Pc_Manager.m_pTex[tex]->SetTexture( m_bPC, tex ) == true )
			{
				//-----------------------------------------------------------------------------
				// 버퍼에넣기
				//-----------------------------------------------------------------------------
				OBJECTVERTEX*	pOBJVERTEX;
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pVtxBuffer[i].NormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}
				
				for( int j = 0; j < g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex; ++j )
					memcpy( pOBJVERTEX[j].Vertex, m_pVtxBuffer[i].pVertex[j], sizeof(m_pVtxBuffer[i].pVertex[j]) );
				m_pVtxBuffer[i].NormalVB->Unlock();
				g_RenderManager.SetStreamSource( 0, m_pVtxBuffer[i].NormalVB, sizeof(OBJECTVERTEX) );

				//-----------------------------------------------------------------------------
				// 렌더링			
				//-----------------------------------------------------------------------------
				g_RenderManager.SetIndices( g_Pc_Manager.m_pMod[mod]->m_pIB, 0 );
				DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod]->m_header.m_num_vertex,
										g_Pc_Manager.m_pMod[mod]->m_header.m_num_face );								
			}
		}
		g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
	}
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 바운딩박스
//-----------------------------------------------------------------------------
void	Character::DisplayBound( BYTE _byType )
{		
	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}	
	
	//-----------------------------------------------------------------------------
	// 메테리얼셋팅
	//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material = m_pMaterial[0];
#else
	D3DMATERIAL8 material = m_pMaterial[0];
#endif // DIRECT_VERSION_9_MJH
	if( g_RockClient.GetGameMode() == GMODE_NORMAL && g_Map.m_byMapType != MS_INDOOR )	
		g_EtcTable.ChangeMaterial( &material, -1.0f );	
	
	// 거리에 따른 블렌딩
	bool bRes;
	if( m_nTableIndex == MAX_USER_INTABLE )
		bRes = g_Pc.m_MainPC.bRender;
	else
		bRes = g_Pc.m_PcTable[m_nTableIndex].bRender;		
	if( bRes == true && m_curt_event.type != SM_NPC_RESPAWN && m_curt_event.type != SM_STATE_DIE )
	{			
		if( m_fDistance <= 480.0f )					
			material.Diffuse.a = 1.0f;		
		else if( m_fDistance > 480.0f )		
			material.Diffuse.a = 1.0f-((m_fDistance-480.0f)/80.0f);		

		if( material.Diffuse.a < 0.0f )
			material.Diffuse.a = 0.0f;
	}	

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정-스케일곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matScale, matWorld;
	D3DXMatrixScaling( &matScale, m_vScale.x, m_vScale.y, m_vScale.z ); 
	D3DXMatrixMultiply( &matWorld, &matScale, &m_matChr );
	g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );	

	//-----------------------------------------------------------------------------
	// 아이템 갯수만큼
	//-----------------------------------------------------------------------------	
	int tex_index, mod_index, itm_index;	

	int itm_count = 0;
	
	itm_count = g_Pc_Manager.GetItmCount( this );

	int AttrIndex		= GetAttrIndex();

	for( int i = 0; i < itm_count; ++i )
	{		
		//-----------------------------------------------------------------------------
		// 텍스쳐, 모델 인덱스
		//-----------------------------------------------------------------------------
		mod_index = g_Pc_Manager.GetModIndex( this, i );
		tex_index = g_Pc_Manager.GetTexIndex( this, i );
		if( tex_index == -1 || mod_index == -1)
			continue;
		
		//-----------------------------------------------------------------------------
		// 텍스쳐 있는지 검사
		//-----------------------------------------------------------------------------
		if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
			m_CollisionBox.Render(m_matChr);
	}	


	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		//-----------------------------------------------------------------------------
		// 무기그리기
		//-----------------------------------------------------------------------------
		for(int i = 0; i < LINK_ITEM_COUNT; ++i )
		{
			// 화살통이 있는지 검사하기			
			itm_index = m_pUserInfo->ArmorInfo.index[i];
			if( itm_index != -1 )
			{
				// 모델, 텍스쳐 인덱스				
				mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
				tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;				
				
				// 월드메트릭스설정				
				switch( i )
				{				
				case 0:	// 오른손
					matWorld = m_matRHand;
					break;				
				case 1:	// 왼손
					matWorld = m_matLHand;
					break;					
				}				

				// 앉아있다
				bool bChange = false;
				if( m_pUserInfo->bDown == true )
					bChange = true;					
				// 등에 차고 있어야한다
				else if( m_pUserInfo->bAttack == false )
					bChange = true;	
				
				if( g_Pc_Manager.m_pItm[itm_index]->m_class == 1 && g_Pc_Manager.m_pItm[itm_index]->m_type == 3 )
				{
					bool bFind = true;
					if( m_pUserInfo->bAttack == true )
					{
						if( bChange == true )
							bFind = false;
						else
							bFind = true;
					}
					else
					{
						if( bChange == true )
							bFind = false;
						else
							bFind = true;
					}
					if( bFind == true )
						matWorld = m_matChr;
					else
					{
						itm_index = g_Pc_Manager.GetEqualItemIndex( itm_index );
						if( itm_index == -1 )
							return;
						else
						{
							mod_index = g_Pc_Manager.m_pItm[itm_index]->m_mod_index;
							tex_index = g_Pc_Manager.m_pItm[itm_index]->m_tex_index;
						}
					}
				}					

				// 렌더링
				if( g_Pc_Manager.m_pTex[tex_index]->SetTexture( byType, tex_index ) == true )
				{
					if ( g_bBlockDebug )
						m_CollisionBox.Render(matWorld);
				}
			}		
		}
	}
}

void Character::DisplayBoundBox()
{
	m_CollisionBox.RenderBox((void *)this);
}


//-----------------------------------------------------------------------------
// Desc: 아이템에 링크된 이펙트 렌더링
//-----------------------------------------------------------------------------
void Character::DisplayEffect()
{
	return;	
	for( int i = 0; i < LINK_ITEM_COUNT; ++i )
	{
		if( m_bPC == true )
		{			
			if( m_pUserInfo->ArmorInfo.index[i] != -1 )
			{	
				//-----------------------------------------------------------------------------
				// 월드메트릭스설정
				//-----------------------------------------------------------------------------
				D3DXMATRIX matWorld;
				if( i == 0 )
					matWorld = m_matRHand;				
				else if( i == 1 )
					matWorld = m_matLHand;										

				// 렌더링
				/*
				if( g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_particle_count > 0 )
				{
					for( unsigned int j = 0; j < g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_particle_count; ++j )
					{
						if( g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticleInfo[i].link == ArmorInfo.index[i] )
						{				
							// 방향				
							g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticle[j].Test( matWorld, 
							g_Pc_Manager.m_pMod[g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_mod_index].m_header.m_vNormal );

							// 렌더링
							g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticle[j].m_bIsSuppressed = false;
							g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticle[j].m_bIsCreate	  = true;
							g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticle[j].Update( 0.02f );
							g_Pc_Manager.m_pItm[ArmorInfo.index[i]].m_pParticle[j].Render();				
						}
					}
				}
				*/
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Desc: 메쉬 애니메이션
//-----------------------------------------------------------------------------	
void Character::MeshAnimation( int mod )
{	
	//-----------------------------------------------------------------------------
	// 애니메이션 프레임 
	//-----------------------------------------------------------------------------	
	int next_frame = 0;	
	
	// 죽을때
	if( m_ani_index == n_NPC_Die0 )
	{		
		next_frame = m_ani_frame+1;
		if( m_ani_frame == -1 )
			next_frame  = 1;		
		if( next_frame > (int)m_end_frame )
			next_frame = (int)m_end_frame;
	}
	// 일반
	else
	{
		m_ani_frame = 0;	
	}

	//-----------------------------------------------------------------------------
	// 애니메이션 메트릭스 계산
	//-----------------------------------------------------------------------------	
	D3DXMATRIX		matTemp;
	D3DXMatrixIdentity( &matTemp );	

	// 쿼터니온 중간값 구하기		
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot, 
						 &g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_qtRot,
						 &g_Pc_Manager.m_pMod[mod]->m_pAnimation[next_frame ].m_qtRot,
						 m_middle_time );

	// 쿼터니온 -> 메트릭스			
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
						
	// 위치값
	matTemp._41 = (( g_Pc_Manager.m_pMod[mod]->m_pAnimation[next_frame ].m_vPos.x -
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.x ) * m_middle_time ) +
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.x;			 

	matTemp._42 = (( g_Pc_Manager.m_pMod[mod]->m_pAnimation[next_frame ].m_vPos.y -
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.y ) * m_middle_time ) +
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.y;

	matTemp._43 = (( g_Pc_Manager.m_pMod[mod]->m_pAnimation[next_frame ].m_vPos.z -
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.z ) * m_middle_time ) +
					 g_Pc_Manager.m_pMod[mod]->m_pAnimation[m_ani_frame].m_vPos.z;

	// 현재위치값 더하기	
	matTemp._41 += m_matChr._41;
	matTemp._42 += m_matChr._42;
	matTemp._43 += m_matChr._43;	

	//-----------------------------------------------------------------------------
	// 월드메트릭스 설정
	//-----------------------------------------------------------------------------		
	g_RenderManager.SetTransform( D3DTS_WORLD, &matTemp );	
}

//-----------------------------------------------------------------------------
// Desc: 검기(잔상)렌더링
//-----------------------------------------------------------------------------	
bool Character::DisplayAfterImage()
{	
	//-----------------------------------------------------------------------------	
	// 페어리가 아니어야하고, 무기장착상태
	//-----------------------------------------------------------------------------	
//	if( GetAttrIndex() == UNION_FAIRY )
//		return false;

	if(!m_IsAllLTexture)
	{
		return true;
	}
	
	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}	
	
	if( m_pUserInfo->bAttack == false || DrawWeaponCheckStatus() == false ) 	
		return false;

	//-----------------------------------------------------------------------------	
	// 자기캐릭터는 공격연타가 끝난 경우는 검기그리지 않는다
	//-----------------------------------------------------------------------------	
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		if( g_Pc.GetPlayer()->m_bWait == true )
			return false;
	}
	
	//-----------------------------------------------------------------------------	
	// 검기 가능상황
	//-----------------------------------------------------------------------------		
	switch( m_curt_event.type )
	{
		case SM_ATTACK_NPC:		// 일반공격
			break;
		
		case SM_USE_SKILL:		// 스킬공격
			break;

		case SM_ANI_QUEUE_PROCESS:
			break;

		default:
			return false;
	}	
	//-----------------------------------------------------------------------------	
	// 검기 가능아이템
	//-----------------------------------------------------------------------------	
	int itm_index = g_Pc_Manager.GetAttackItemIndex( this , m_nTableIndex );
	
	if( itm_index == -1 )
		return false;
	

	if( g_Pc_Manager.m_pItm[itm_index]->m_tex == 0 )
		return false;
	
	//.............................................................................................................
	// 칼위치 더해주기 ( By wxywxy )
	//.............................................................................................................
	m_SwordTail.AddPoint( m_pUserInfo->vSwordAxis[ 1 ] , m_pUserInfo->vSwordAxis[ 0 ] , g_lpDevice );
	

	if(m_pUserInfo->ArmorInfo.index[0] == -1)
	{
		return false;
	}

	//.............................................................................................................
	// 검기 렌더링 
	//.............................................................................................................
	switch( g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_tex )			
	{
		case 1:				
		
			m_SwordTail.SetTexture( &g_Pc.m_TexTable.pAfterImage1 );
		
			break;
		
		case 2:				
		
			m_SwordTail.SetTexture( &g_Pc.m_TexTable.pAfterImage2 );
		
			break;
	
		case 3:				
		
			m_SwordTail.SetTexture( &g_Pc.m_TexTable.pAfterImage3 );
		
			break;

		default:				
		
			m_SwordTail.SetTexture( &g_Pc.m_TexTable.pAfterImage2 );
		
			break;
	}

	m_SwordTail.Render( g_lpDevice );
	
	return TRUE;
}



//-----------------------------------------------------------------------------
// Desc: 심심한 행동처리
//-----------------------------------------------------------------------------
bool	Character::SetSimSim()
{
	// 게임중일때만 처리
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return false;
	
	if(m_IsApplyConAniSkill)
	{
		return false;
	}

	//-----------------------------------------------------------------------------
	// 유저
	//-----------------------------------------------------------------------------
	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{	
		//-----------------------------------------------------------------------------
		// 심심한행동을 할 시간이 지났다
		//-----------------------------------------------------------------------------
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{			
			if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_SimTime , SIMSIM_TIME * 2 ) ) )
			{
				// 심심한시간 저장
				m_SimTime = g_nowTime;			
				
				if(!m_State.bRide)
				{
					AnimationPlay( n_Suddenly9 );
				}
				else
				{
					AnimationPlay( n_Ride_Sudden1 );
					g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand1 );
				}

				// 이벤트저장
				m_curt_event.type = SM_SIMSIM;
				return true;
			}
			else if( ( m_ani_index >= n_Stand_Battle0 && m_ani_index <= n_Stand_Battle9 ) )
			{
				if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_SimTime , ( c_WeaponDelayTime * 2 ) ) ) )	
				{
					AnimationPlay( n_Stand_Peace9 );
				}
			}
			else
			{
				if(!m_State.bRide)
				{
					AnimationPlay( n_Stand_Peace9 );
				}
				else
				{
					AnimationPlay( n_Ride_Wait );
					g_Map.m_Npc[m_State.TransIndex].AnimationPlay( n_NPC_Stand0 );
				}				
			}
		}
	}
	//-----------------------------------------------------------------------------
	// NPC
	//-----------------------------------------------------------------------------
	else 
	{
		//-----------------------------------------------------------------------------
		// 심심한행동을 할 시간이 지났어야 한다.
		// 다른 유저가 말을 탄 경우가 아니어야 한다.
		//-----------------------------------------------------------------------------
		if( m_ani_frame + c_MotionBlendHalfFrame > (int)m_end_frame )
		{			
			//...................................................................................................
			// 거대 몬스터 by wxywxy
			//...................................................................................................
			if( g_Map.m_NpcTable[ m_nTableIndex ].code >= c_NotMoveNPCCode_Min && 
				g_Map.m_NpcTable[ m_nTableIndex ].code <= c_NotMoveNPCCode_Max   )
			{
				if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_SimTime , SIMSIM_TIME * 2 ) ) )
				{
					m_SimTime			= g_nowTime;
					
					// 확률로 한다
					if( rand()%5 == 0 )
					{
						// 해당 애니메이션이 있어야 한다
						//					int nAniIndex = n_NPC_Stand0+(rand()%2);
						int nAniIndex = n_NPC_Stand1;
						if( AnimationPlay( nAniIndex ) == 0 )					
						{
							m_curt_event.type	= SM_SIMSIM;
							return true;
						}
					}
				}
			}
			else
			if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_SimTime , SIMSIM_TIME ) ) )
			{
				m_SimTime			= g_nowTime;
				// 절반의 확률로 한다
				if( rand()%2 == 0 )
				{
					// 해당 애니메이션이 있어야 한다
//					int nAniIndex = n_NPC_Stand0+(rand()%2);
					int nAniIndex = n_NPC_Stand1;
					if( AnimationPlay( nAniIndex ) == 0 )					
					{
						m_curt_event.type	= SM_SIMSIM;
						return true;
					}
				}
			}
			// 기본행동
			AnimationPlay( n_NPC_Stand0 );
		}					
	}		
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 마지막 이동위치로 이동
//-----------------------------------------------------------------------------
void	Character::SetBreakEvent()
{	
	// 현재 대상의 테이블 위치값 얻어오기

	int nPosX, nPosY;
	if( m_bPC == true )
	{			
		nPosX = g_Pc.m_PcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize;
		nPosY = g_Pc.m_PcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize;
	}
	else if(!m_bPC && m_State.nPetState == nPetState_Pet)
	{
		nPosX = g_Map.m_NpcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize;
		nPosY = g_Map.m_NpcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize;
	}
#ifdef NPC_MOVE_TO_SKILL
	else if(!m_bPC && m_State.nActiveState == nActiveTypeMove )
	{
		nPosX = g_Map.m_NpcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize;
		nPosY = g_Map.m_NpcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize;
	}
#endif 
	else
	{
#ifdef	NETWORK_CLIENT 		
		g_Map.m_NpcTable[m_nTableIndex].lDestBlock = thePath[theCurtPath];
//		DebugPrint( "\n  ***** NonPlayer::SM_ATTACK_NPC thePath[theCurtPath] = %u, theCurtPath = %d ***** \n", thePath[theCurtPath], theCurtPath);
#endif

		nPosX = g_Map.m_NpcTable[m_nTableIndex].lDestBlock%g_Pc.m_cSize;
		nPosY = g_Map.m_NpcTable[m_nTableIndex].lDestBlock/g_Pc.m_cSize;
	}	

	// 클라이언트 위치와 테이블 위치가 틀리다면 해당 타일로 이동
	if( m_Mov.curt.x == nPosX && m_Mov.curt.y == nPosY )
		return;
	else
	{
		// 메트릭스 갱신
		m_matChr._41 = (nPosX*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);	
		m_matChr._42 = g_Pc.GetMapHeight( nPosX, nPosY );
		m_matChr._43 = (nPosY*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);

		BOOL Is4FootUnit = FALSE;

		if( !m_bPC )	///-- NPC일 경우
		{
			DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
			SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if(npc_Info)
			{
				if( npc_Info->theGender == n_4FootUnit )
				{
					Is4FootUnit = TRUE;
				}
			}
		}

		if(Is4FootUnit)
		{
			D3DXMATRIX matRotX;	
			
			D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
			D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
			vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

			float fTempYRadian = 0.0f;
			float fXRadian = 0.0f;

			GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

			m_Mov.now_Xrad = fXRadian;			
		}

		// 위치값 초기화
		m_Mov.Init( nPosX, nPosY );
	}	
}


//-----------------------------------------------------------------------------
// Desc: 맞는 애니메이션
//-----------------------------------------------------------------------------
void	Character::SetUnderAttack()
{	
/*	//-----------------------------------------------------------------------------
	// 맞는 애니메이션 하는경우
	//-----------------------------------------------------------------------------	
	bool bFind = false;
	switch( m_curt_event.type )
	{	
	case NULL:		
		bFind = true;		
		break;	
	case MODE_DOWN:			// 앉아있다
		bFind = true;		
		break;
	}
	if( bFind == true )
	{
		// 일어나기
		if( m_curt_event.type == MODE_DOWN )
		{
			m_pUserInfo->EtcItem = 0;
//			g_Pc.CM__MODE__CHANGE( MODE_DOWN );
		}

		// 애니메이션 바꾸기		
		if( m_bPC == true )
		{
			if( m_State.bRide == true )
				AnimationPlay( STANDARD0 );
			else
				AnimationPlay( DEFENSE3 );
		}
		else		
			AnimationPlay( NPC_DEFENSE0 );			
		m_curt_event.type = SM_UNDERATTACK;
	}

	//-----------------------------------------------------------------------------
	// 슬라이드가 저장되어 있다면.. 현재 이벤트로 처리한다
	//-----------------------------------------------------------------------------
	if( m_save_event.type == SM_SLIDE_NPC )
	{
		m_curt_event	= m_save_event;		
		D3DXVECTOR3 v	= D3DXVECTOR3( m_Mov.curt.x*8+4, m_Mov.curt.height, m_Mov.curt.y*8+4 );
		g_Particle.DisplayUpdate( 24, v );
	} */
}


//-----------------------------------------------------------------------------
// Desc: 화살쏘기
//-----------------------------------------------------------------------------
bool	Character::SetArrowCreate()
{
	//-----------------------------------------------------------------------------
	// 현재애니메이션 프레임과 사운드발생 키값이 같을때 사용한다
	//-----------------------------------------------------------------------------
	//if( m_ani_frame == m_pAniInfo[m_ani_index].sound_key )
// 임시 By wxywxy
//	if( m_ani_frame == g_Pc_Manager.GetSoundkey( this, m_ani_index ) )
	{
		//-----------------------------------------------------------------------------
		// 애니메이션 프레임이 -1 또는 종료프레임보다 크다면 실패
		//-----------------------------------------------------------------------------		
		if( m_ani_frame == -1 || m_ani_frame >= m_end_frame )
			return false;		

		//-----------------------------------------------------------------------------
		// 쏘는위치
		//-----------------------------------------------------------------------------
		// PC
		int arrow_node;
		if( m_bPC == true )
		{
			// PC의 속성에 따라서 링크되는 화살본을 찾는다
			switch( GetAttrIndex() )
			{			
			case UNION_MAN:	// 남자
				{
					arrow_node = g_Pc_Manager.m_PcAttr[GetAttrIndex()].l_hand_node;
				}
				break;
			default:
				{
//					#ifdef GAMETYPE_TEST					
//						MessageBox( NULL, "화살쏘려는 PC가 정의되지 않았습니다", "Error", MB_OK );					
//					#endif					
					return false;
				}
				break;
			}									
		}
		// NPC
		else			
		{
			// NPC의 속성에 따라서 링크되는 화살본을 찾는다
			switch( g_Map.m_NpcTable[m_nTableIndex].index )
			{			
			case 29:	// 베트				
				arrow_node = 26;				
				break;			
			case 31:	// 베레스				
				arrow_node = 26;				
				break;
			default:
				{
//					#ifdef GAMETYPE_TEST
//						MessageBox( NULL, "화살쏘려는 NPC가 정의되지 않았습니다", "Error", MB_OK );
//					#endif
					return false;
				}
				break;
			}	
		}
		// 링크되는 화살본의 애니메이션값 리턴
		int next_frame = m_ani_frame+1;
		if( next_frame >= m_end_frame )
			next_frame = 0;
		ItemAnimation( arrow_node, false, next_frame, m_ani_frame );
		D3DXVECTOR3	vCurt = D3DXVECTOR3( m_matRHand._41, m_matRHand._42, m_matRHand._43 );		

		//-----------------------------------------------------------------------------
		// 타겟위치
		//-----------------------------------------------------------------------------
		// PC
		bool	bPC;
		int		target;
		int		link = -1;
		D3DXVECTOR3	vDest;
		if( m_curt_event.race >= 0 )
		{
			// 플래그유저
			bPC	= true;

			// 도착위치	
			int ani, attr, num, frame;			
			if( m_curt_event.unique == g_Pc.m_MainPC.lUnique )
			{
				attr	= g_Pc.m_MainPC.nIndex;
				ani		= g_Pc.GetPlayer()->m_ani_index;
				//num		= g_Pc.GetPlayer()->m_pAniInfo[ani].ani_number;
				num		= g_Pc_Manager.GetAniNumber( g_Pc.GetPlayer(), ani );
				frame	= g_Pc.GetPlayer()->m_ani_frame;
				target	= MAX_USER_INTABLE;
			}
			else
			{
				if( MAX_USER_INTABLE == (target	= g_Pc.FindUser(m_curt_event.unique)) )
					return false;
				
				attr	= g_Pc.m_PcTable[target].nIndex;
				ani		= g_Pc.m_Pc[target].m_ani_index;
				//num		= g_Pc.m_Pc[target].m_pAniInfo[ani].ani_number;
				num		= g_Pc_Manager.GetAniNumber( &g_Pc.m_Pc[target], ani );
				frame	= g_Pc.m_Pc[target].m_ani_frame;
			}			
			switch( attr )
			{
			case UNION_MAN:
				{
					if( rand()%2 == 0 )
						link = 3;
					else
						link = 9;
				}
				break;
			case UNION_WOMAN:
				{
					if( rand()%2 == 0 )
						link = 3;
					else
						link = 9;
				}
				break;
			case UNION_FAIRY:
				{
					if( rand()%2 == 0 )
						link = 2;
					else
						link = 3;
				}
				break;
			default:
				{
//					#ifdef GAMETYPE_TEST
//						MessageBox( NULL, "PC속성이 정의되지 않았습니다", "Error", MB_OK );
//					#endif
					return false;
				}
				break;
			}						
			vDest.x = g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.x;
			vDest.y = g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.y;
			vDest.z = g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.z;
			
			// 말탔을 경우는 높이값 올려준다
//			if( m_State.bRide == true )
//				vDest.y += 4.0f;
		}
		// NPC
		else
		{			
			if( MAX_NPC_INTABLE == (target = g_Map.FindNpc(m_curt_event.unique)) )			
				return false;

			// NPC플래그저장
			bPC		= false;
			link	= 2;
//			switch( g_Map.m_NpcTable[target].index )
//			{
//			case DESTROY_OBJECT_INDEX:
//				{
//					vDest.x = g_Map.m_Npc[target].GetBoundMaxPos().x/2;
//					vDest.y = g_Map.m_Npc[target].GetBoundMaxPos().y/2;
//					vDest.z = g_Map.m_Npc[target].GetBoundMaxPos().z/2;
//				}
//				break;
//			default:
				{
					int ani		= g_Map.m_Npc[target].m_ani_index;
					//int num		= g_Map.m_Npc[target].m_pAniInfo[ani].ani_number+g_Pc_Manager.m_ani_count;
					int	num		= g_Pc_Manager.GetAniNumber( &g_Map.m_Npc[target], ani );
					int frame	= g_Map.m_Npc[target].m_ani_frame;
					vDest.x		= g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.x;
					vDest.y		= g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.y;
					vDest.z		= g_Pc_Manager.m_pAni[num]->m_pNode[link].m_pAnimation[frame].m_vPos.z;
				}
//				break;
//			}			
		}
		// 링크된 값이 없다면 화살쏘기 실패
		if( link == -1 )
			return false;
		
		//-----------------------------------------------------------------------------
		// 자기 캐릭터는 계속 공격해야기 때문에, 저장이벤트에 화살쏘끼를 넣는다
		//-----------------------------------------------------------------------------
		if( m_nTableIndex != MAX_USER_INTABLE )
			m_curt_event.Clear();

		//-----------------------------------------------------------------------------
		// 화살 생성하기
		//-----------------------------------------------------------------------------
		g_Pc_Manager.CreateArrow( bPC, target, link, vCurt, vDest );
	}
	return true;
}

int Character::UpdateReplyUnite()
{
	//-----------------------------------------------------------------------------
	// 시간얻기
	//-----------------------------------------------------------------------------
	float lPreTime;
	if( m_nTableIndex == MAX_USER_INTABLE )
		lPreTime = g_Pc.m_MainPC.lPreTime;
	else
		lPreTime = g_Pc.m_PcTable[m_nTableIndex].lPreTime;
	
	float value;
	float end_time		= lPreTime+FAIRYUNITE_TIME;
	float change_time	= FAIRYUNITE_TIME/3;
	
	int itm_count = 0;
	
	itm_count = g_Pc_Manager.GetItmCount( this );
	
	
	//-----------------------------------------------------------------------------
	// 3초넘어갔다면 알파값1.0으로초기화
	//-----------------------------------------------------------------------------
	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( lPreTime , FAIRYUNITE_TIME ) ) )
	{		
		for( int i = 0; i < itm_count; ++i )	
			m_pMaterial[i].Diffuse.a = 1.0f;		
		return -1;
	}
	//-----------------------------------------------------------------------------
	// 0~1초
	//-----------------------------------------------------------------------------
	else if( g_nowTime <= lPreTime + change_time )	
		value = (float)1.0f-( SAFE_TIME_SUB( g_nowTime , lPreTime ) / change_time );			
	//-----------------------------------------------------------------------------
	// 2~3초
	//-----------------------------------------------------------------------------
	else if( end_time < g_nowTime + change_time )
		value = (float)1.0f- SAFE_TIME_SUB( end_time , g_nowTime ) / change_time;	
	//-----------------------------------------------------------------------------
	// 1~2초사이 false로 날려서 렌더링하지 않게 한다 
	//-----------------------------------------------------------------------------
	else
		return 0;

	//-----------------------------------------------------------------------------
	// 메터리얼 알파값 변경	
	//-----------------------------------------------------------------------------
	//for( int i = 0; i < m_itm_count; ++i )	
	for( int i = 0; i < itm_count; ++i )	
		m_pMaterial[i].Diffuse.a = value;

	//-----------------------------------------------------------------------------
	// 카메라이동
	//-----------------------------------------------------------------------------	
	return 1;
}
//-----------------------------------------------------------------------------
// Desc: 자기 상태 저장
//-----------------------------------------------------------------------------
void Character::SetState( STATE_INFO State )
{
	// 상태 저장
	m_State = State;
/*	if( GetAttrIndex() != UNION_FAIRY )	
		m_State.bUnite = false;

	// 말타고 있니?
	if( m_State.bRide == true )
		g_Map.m_Npc[m_State.horse_index].m_State.horse_index = m_nTableIndex;	

	// 요정일 경우
	if( GetAttrIndex() == UNION_FAIRY )
	{
		InitFairyMove();
//		if( m_State.bUnite == true )		
//			SM__REPLY__UNITE( m_State.bUnite, m_State.Unite_unique );		
	}

	// 디펜스모드?
	if( m_pUserInfo->bDefense == true )	
		AnimationPlay( DEFENSE0 );
	// 일반상태
	else	
		AnimationPlay( STANDARD0 ); */
}


//-----------------------------------------------------------------------------
// Desc: 해당뱡향으로 회전 
//-----------------------------------------------------------------------------
void Character::SetDirection( float x, float y )
{
	if( x == m_matChr._41 && y == m_matChr._43 )
	{
		return;
	}

	x -= m_matChr._41;
	y -= m_matChr._43;		

	m_Mov.now_Yrad = -atan2(y, x)-1.5705f;
	m_Mov.nex_Yrad = m_Mov.now_Yrad;
	
	D3DXMATRIX matRotY;

	D3DXMATRIX	matPos = m_matChr;
	D3DXMatrixRotationY( &matRotY, m_Mov.now_Yrad );

	m_vDir = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	D3DXVec3TransformCoord( &m_vDir , &m_vDir , &matRotY );

	BOOL Is4FootUnit = FALSE;

	if( !m_bPC )	///-- NPC일 경우
	{
		DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
		SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

		if(npc_Info)
		{
			if( npc_Info->theGender == n_4FootUnit )
			{
				Is4FootUnit = TRUE;
			}
		}
	}

	if(Is4FootUnit)
	{
		D3DXMATRIX matRotX;	
		
		D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
		vNowPos.y = g_Map.PHF_GetHeight( vNowPos.x , vNowPos.z );
		D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
		vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

		float fTempYRadian = 0.0f;
		float fXRadian = 0.0f;

		GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

		m_Mov.now_Xrad = fXRadian;

		D3DXMatrixRotationX( &matRotX , fXRadian );
		D3DXMatrixMultiply( &m_matChr , &matRotX , &matRotY );		
	}
	else
	{
		m_matChr = matRotY;
	}
	
	m_matChr._41 = matPos._41;
	m_matChr._42 = matPos._42;
	m_matChr._43 = matPos._43;

	if( m_Mov.now_Yrad < 0 )
	{
		m_Mov.now_Yrad += (D3DX_PI * 2) ;
	}
	
	return;
}

/*
//-----------------------------------------------------------------------------
// Desc: 밀려나기
//-----------------------------------------------------------------------------
bool Character::SM__SLIDE__NPC( EVENT_DATA_INFO data )
{
	// 이벤트초기화
	m_curt_event.Clear();
	m_save_event.Clear();

	// 슬라이드이벤트 저장
	m_curt_event = data;

	// 위치값 초기화
	m_Mov.Init( data.x, data.y );	

	// 이동거리를 애니프레임으로 나누기
	int frame;
	if( m_bPC == true )
		//frame = g_Pc_Manager.GetEndFrame( m_pAniInfo[DEFENSE3].ani_number );		
		frame  = g_Pc_Manager.GetEndFrame( this, DEFENSE3 );
	else
		//frame = g_Pc_Manager.GetEndFrame( g_Pc_Manager.m_ani_count+m_pAniInfo[NPC_DEFENSE0].ani_number );		
		frame  = g_Pc_Manager.GetEndFrame( this, NPC_DEFENSE0 );
	m_curt_event.vPos.x /= frame;
	m_curt_event.vPos.y /= frame;
	m_curt_event.vPos.z /= frame;

	return false;
}
*/

//-----------------------------------------------------------------------------
// Desc: 스킬 사용
//-----------------------------------------------------------------------------
/*
bool Character::SM__USE__SKILL( EVENT_DATA_INFO data )
{	
	//-----------------------------------------------------------------------------
	// 현재 사용하려는 스킬인덱스와 애니메이션 인덱스 찾기
	// 애니메이션선택.. 실패했다면 리턴한다.
	//-----------------------------------------------------------------------------	
	m_curt_event		= data;	
	int skill_number	= g_Skill.FindSkill( m_curt_event.motion );
	int ani_number		= g_Skill.m_skill.m_attr[skill_number].aniindex;
	if( AnimationPlay( PC_SKILL_NUMBER+ani_number ) == -1 )
	{
		m_curt_event.Clear();
		return false;
	}

	//-----------------------------------------------------------------------------
	// 나일때만..
	//-----------------------------------------------------------------------------
	if( m_nTableIndex == MAX_USER_INTABLE )	
	{		
		// 자동으로 공격되는 스킬처리
		bool bFind = false;
		switch( m_curt_event.motion )
		{		
		case FAIRY_SKILL_100:	// 트윈클
			bFind = true;			
			break;
		}
		if( bFind == false )
			m_save_event.Clear();
	}
	//-----------------------------------------------------------------------------
	// 스킬사용한 유저가 나와 합체한 유저라면 NPC에너지 요청한다
	//-----------------------------------------------------------------------------
	if( m_bPC == true )
	{		
		if( g_Pc.GetPlayer()->m_State.Unite_unique == g_Pc.m_PcTable[m_nTableIndex].lUnique )
		{
			// 타겟이 PC인지 NPC인지 구분하기
			if( m_curt_event.race < 0 )
			{
				// 서버에 타겟NPC에너지 요청하기
				CP_NPC_Energy	Msg;
				Msg.m_dwIndex	= data.index;
				Msg.m_dwUnique	= data.unique;
				g_Net.m_Socket[0].Write( (char*)&Msg, sizeof(CP_NPC_Energy) );

				// 선택대상저장
				int npc_index = g_Map.FindNpc(m_curt_event.index, m_curt_event.unique);
				if( g_Pc.m_Select.index != npc_index )
				{
					g_Pc.m_Select.Clear();
					g_Pc.m_Select.bPC	= false;
					g_Pc.m_Select.index = npc_index;
				}
			}
		}

	}	

	//-----------------------------------------------------------------------------
	// 스킬속성에 맞는 처리
	//-----------------------------------------------------------------------------
	switch( m_curt_event.motion )
	{
	case ARCHER_SKILL_50:	// 백스텝
		{
			m_curt_event.vPos.x /= (m_end_frame+2);
			m_curt_event.vPos.y /= (m_end_frame+2);
			m_curt_event.vPos.z /= (m_end_frame+2);
			m_Mov.Init( m_curt_event.x, m_curt_event.y );
		}
		break;
	default:
		{
			// 타겟위치로 회전
			if( m_Mov.curt.x != m_curt_event.x || m_Mov.curt.y != m_curt_event.y )
				SetDirection( m_curt_event.vPos.x, m_curt_event.vPos.z );
		}
		break;
	}
	
	//-----------------------------------------------------------------------------
	// 무기에 맞는 처리
	//-----------------------------------------------------------------------------
	int itm_index = g_Pc_Manager.GetAttackItemIndex( m_bPC, m_nTableIndex );
	switch( g_Pc_Manager.m_pItm[itm_index].m_class )
	{
	// 한손검
	case 0:
		{
		}
		break;
	// 양손검
	case 1:
		{
			// 활
			//if( g_Pc_Manager.m_pItm[itm_index].m_type == 3 )
		}
		break;
	// 방패
	case 3:
		{
		}
		break;
	}

	//-----------------------------------------------------------------------------
	// 직업에 맞는 처리
	//-----------------------------------------------------------------------------
	int job = -1;
	if( m_bPC == true )
	{
		if( m_nTableIndex == MAX_USER_INTABLE )
			job = g_Pc.m_MainPC.char_info.job;
		else
			job = g_Pc.m_PcTable[m_nTableIndex].char_info.job;
	}
	switch( job )
	{	
	case NOVICE:	// 무직		
		break;	
	case KNIGHT:	// 기사		
		break;	
	case ARCHER:	// 궁수		
		break;	
	case MAGICIAN:	// 마법사		
		break;	
	case FAIRY:		// 페어리		
		break;
	default:		
		break;
	}
	
	//-----------------------------------------------------------------------------
	// 검기프레임 초기화
	//-----------------------------------------------------------------------------	
	if( m_pUserInfo != NULL )
		m_pUserInfo->AfterImage.nCount = -1;
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 화살쏘기
//--------------------------------------- -------------------------------------
bool Character::SM__ARROW__GOING( EVENT_DATA_INFO data )
{		
	if( m_bPC == true )	
		AnimationPlay( PC_ATTACK_NUMBER+16 );

	// 이벤트 저장
	m_curt_event = data;
	m_save_event.Clear();
	SetDirection( data.vPos.x, data.vPos.z );
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 말타기
//--------------------------------------- -------------------------------------
bool Character::SM__HORSE__RIDE( EVENT_DATA_INFO data )
{
	// 말탄 상태 갱신			
	m_State.bRide = true;
	m_State.horse_index = data.index;

	// 말정보 갱신
	g_Map.m_Npc[m_State.horse_index].m_State.horse_index = m_nTableIndex;		

	// 애니메이션 변경		
	if( data.motion == 0 )		
		AnimationPlay( STANDARD22 );		
	else		
		AnimationPlay( STANDARD23 );	

	// 자기일 경우 
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		// 카메라 이동값 구하기
		g_Pc.m_vCameraMove.x = (float)(((int)m_Mov.dest.x-(int)data.x)*8)/(float)(m_end_frame+1);
		g_Pc.m_vCameraMove.y = (float)((int)m_Mov.dest.height-g_Pc.GetMapHeight(data.x, data.y))/(float)(m_end_frame+1);
		g_Pc.m_vCameraMove.z = (float)(((int)m_Mov.dest.y-(int)data.y)*8)/(float)(m_end_frame+1);
		m_Mov.Init( data.x, data.y );

		// 보이는 타일 넓히기
		g_Map.UpdateTerrain( m_Mov.curt.x, m_Mov.curt.y );
	}	

	// 이벤트 저장		
	m_curt_event = data;
	m_save_event.Clear();	
	return true;
}


//-----------------------------------------------------------------------------
// Desc: 말에서 내리기
// 6 5 4
// 7 I 3
// 0 1 2 
//-----------------------------------------------------------------------------
void Character::SM__HORSE__DOWN( EVENT_DATA_INFO data )
{		
	//-----------------------------------------------------------------------------
	// 내리는곳에 따른 처리
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp;
	float		fRot1, fRot2;
	int			nMove, nAni = -1 ;
	int			nAngle = GetAngle( m_Mov.dest.x, m_Mov.dest.y, data.x, data.y );	

	// 아래
	if( m_byAngle == 0 || m_byAngle == 1 || m_byAngle == 2 )
	{
		if( nAngle == 3 )
		{			
			nMove	= 0;
			fRot2	= 90*ANGLE;
			nAni	= STANDARD22;
		}
		else if( nAngle == 7 )
		{
			nMove	= 1;			
			fRot2	= -90*ANGLE;
			nAni	= STANDARD23;			
		}		
		fRot1		= 0.0f;
		m_byAngle	= 1;
	}	
	// 위
	else if( m_byAngle == 4 || m_byAngle == 5 || m_byAngle == 6 )
	{	
		if( nAngle == 3 )
		{			
			nMove	= 0;
			fRot2	= 90*ANGLE;
			nAni	= STANDARD23;			
		}
		else if( nAngle == 7 )
		{
			nMove	= 1;			
			fRot2	= -90*ANGLE;
			nAni	= STANDARD22;			
		}
		fRot1		= 180*ANGLE;
		m_byAngle	= 5;
	}	
	// 오른쪽
	else if( m_byAngle == 3 )
	{
		if( nAngle == 5 )
		{
			nMove	= 2;						
			fRot2	= 0*ANGLE;			
			nAni	= STANDARD22;
		}
		else if( nAngle == 1 )
		{			
			nMove	= 3;
			fRot2	= 180*ANGLE;
			nAni	= STANDARD23;
		}
		fRot1		= -90*ANGLE;
	}
	// 왼쪽
	else if( m_byAngle == 7 )
	{
		if( nAngle == 5 )
		{
			nMove	= 2;		
			fRot2	= 0*ANGLE;
			nAni	= STANDARD23;
		}
		else if( nAngle == 1 )
		{	
			nMove	= 3;
3			fRot2	= 180*ANGLE;
			nAni	= STANDARD22;
		}
		fRot1		= 90*ANGLE;
	}

	//-----------------------------------------------------------------------------
	// 이벤트초기화
	//-----------------------------------------------------------------------------
	m_curt_event.Clear();
	m_save_event.Clear();
	m_State.bRide = false;
	
	//-----------------------------------------------------------------------------
	// 말타기 예외처리
	//-----------------------------------------------------------------------------
	if( -1 == nAni )
	{
		// 캐릭터위치
		D3DXMatrixIdentity( &matTemp );
		matTemp._41 = (data.x*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);
		matTemp._42 = g_Pc.GetMapHeight( data.x, data.y );
		matTemp._43 = (data.y*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);

		// 애니메이션 기본값으로
		nAni = STANDARD0;
	}
	//-----------------------------------------------------------------------------
	// 정상처리
	//-----------------------------------------------------------------------------
	else
	{
		// 이벤트 저장
		m_curt_event = data;

		// 말위치
		D3DXMatrixRotationY( &matTemp, fRot1 );
		matTemp._41 = g_Map.m_Npc[m_State.horse_index].GetPosTM()._41;
		matTemp._42 = g_Map.m_Npc[m_State.horse_index].GetPosTM()._42;
		matTemp._43 = g_Map.m_Npc[m_State.horse_index].GetPosTM()._43;
		g_Map.m_Npc[m_State.horse_index].SetPosTM( matTemp );			

		// 캐릭터위치
		D3DXMatrixRotationY( &matTemp, fRot2 );	
		matTemp._41	= m_matChr._41;
		matTemp._42 = g_Pc.GetMapHeight( data.x, data.y );
		matTemp._43	= m_matChr._43;		
		switch( nMove )
		{
		case 0:
			matTemp._41 = m_matChr._41+CLIENT_TILESIZE;
			break;
		case 1:
			matTemp._41 = m_matChr._41-CLIENT_TILESIZE;
			break;
		case 2:
			matTemp._43 = m_matChr._43+CLIENT_TILESIZE;
			break;
		case 3:
			matTemp._43 = m_matChr._43-CLIENT_TILESIZE;
			break;		
		}
	}

	// 타고있는 말의 인덱스 초기화
	g_Map.m_Npc[m_State.horse_index].m_State.horse_index = -1;

	// 캐릭터위치값 수정
	m_matChr = matTemp;	

	// 애니메이션플레이
	AnimationPlay( nAni );

	//-----------------------------------------------------------------------------
	// 자기캐릭터일 경우 카메라, 지형업데이트 
	//-----------------------------------------------------------------------------
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		// 예외처리
		if( STANDARD0 == nAni )
			g_Camera.Update();		
		else
		{
			// 애니메이션 프레임만큼 카메라이동값 구하기
			g_Pc.m_vCameraMove.x = (float)(((int)m_Mov.dest.x-(int)data.x)*8)/(float)(m_end_frame+1);
			g_Pc.m_vCameraMove.y = (float)(m_Mov.dest.height-g_Pc.GetMapHeight(data.x, data.y))/(float)(m_end_frame+1);
			g_Pc.m_vCameraMove.z = (float)(((int)(int)m_Mov.dest.y-data.y)*8)/(float)(m_end_frame+1);
			m_Mov.Init( data.x, data.y );

			// 보이는 타일 넓히기
			g_Map.UpdateTerrain( data.x, data.y );
		}
	}
	// 다른 사람은 해당위치로 이동
	else
	{
		SetBreakEvent();
	}
}
*/
//-----------------------------------------------------------------------------
// Desc: 캐릭터 이동, 높이보폭, 회전값 구하기
//-----------------------------------------------------------------------------
bool Character::SetMoveStep( TILE_INFO TilePos )
{	
	// 월드상의 실제좌표값
	float fPosX, fPosY;
	///-- 캐릭터의 실제위치를 가져온다.
	GetRealTM( fPosX, fPosY );

	// 목표타일과 현재타일까지의 거리
	D3DXVECTOR3 vStep;
	vStep.x = ((TilePos.x*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2))-fPosX;
	vStep.y = ((TilePos.y*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2))-fPosY;
	
	if( abs(vStep.x) <= EPSILON )
		vStep.x = 0.0f;
	if( abs(vStep.y) <= EPSILON )
		vStep.y = 0.0f;
			
	// 타일거리와 애니프레임을 이용한 보폭값 계산
	int nRange = 1;	
//	if( m_bPC == true )
	{
		nRange = GetMax( abs((long)TilePos.x- (long)m_Mov.curt.x), abs((long)TilePos.y- (long)m_Mov.curt.y) );
	
		if( nRange == 0 )
		{			
			nRange = 1;
		}
	}

	if(!m_bPC)
	{
		if( m_State.nHolyState == nHolyState_Holy )
		{
			int a = 0;
		}
		else if(m_State.nPetState != nPetState_Pet)
		{
			int a = 0;
		}
	}

	m_Mov.step_x = vStep.x/(GetRunFrame()*nRange);
	m_Mov.step_y = vStep.y/(GetRunFrame()*nRange);
	
	return true;
}


void Character::GetMoveStep( int _nDestX, int _nDestY, int _nCurtX, int _nCurtY, float &fPosX, float &fPosY )
{	
	// 도착할위치
	D3DXVECTOR3 vStep;		
	vStep.x = ((_nDestX*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2))-((_nCurtX*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2));
	vStep.y = ((_nDestY*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2))-((_nCurtY*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2));	

	if( abs(vStep.x) <= EPSILON )	
		vStep.x = 0.0f;
	if( abs(vStep.y) <= EPSILON )
		vStep.y = 0.0f;

	// 도착할타일값		
	int nRange = GetMax(abs(_nDestX-_nCurtX), abs(_nDestY-_nCurtY));

	// 사용된 최대 블럭에 따라서 Step값을 구한다 
	fPosX = vStep.x/(GetRunFrame()*nRange);
	fPosY = vStep.y/(GetRunFrame()*nRange);
}


//-----------------------------------------------------------------------------
// Desc: 다음타일값으로 이동할 높이값 구하기
//-----------------------------------------------------------------------------
void Character::SetHeightStep( bool _bInit )
{
	if( true == _bInit )
	{		
		// 현재 타일높이값으로 수정
		m_matChr._42		= g_Pc.GetMapHeight(m_Mov.curt.x, m_Mov.curt.y);
		m_Mov.curt.height	= m_matChr._42;

		BOOL Is4FootUnit = FALSE;

		if( !m_bPC )	///-- NPC일 경우
		{
			DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
			SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if(npc_Info)
			{
				if( npc_Info->theGender == n_4FootUnit )
				{
					Is4FootUnit = TRUE;
				}
			}
		}

		if(Is4FootUnit)
		{
			D3DXMATRIX matRotX;	
			
			D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
			D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
			vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

			float fTempYRadian = 0.0f;
			float fXRadian = 0.0f;

			GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

			m_Mov.now_Xrad = fXRadian;			
		}

		// 말타고있다면 말위치도 수정
//		if( true == m_bPC && true == m_State.bRide && -1 != m_State.horse_index )		
//			g_Map.m_Npc[m_State.horse_index].SetPosTM( m_matChr );		
	}
	else
	{
		m_Mov.next.height = g_Pc.GetMapHeight( m_Mov.next.x, m_Mov.next.y );

		float distance = DIST( m_matChr._41, m_matChr._43, (m_Mov.next.x*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2), (m_Mov.next.y*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2) );
//		float tiledistance = DIST( (m_Mov.curt.x*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2), 
//								(m_Mov.curt.y*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2), 
//								(m_Mov.next.x*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2), 
//								(m_Mov.next.y*CLIENT_TILESIZE) + (CLIENT_TILESIZE/2) );
//		RLG1( "SetHeightStep distance = %3.3f", distance );
//		int dist_block = ( fabs(distance) / CLIENT_TILESIZE ) + 1;
		
//		int runframe = GetRunFrame() * ( fabs(distance) / (CLIENT_TILESIZE * dist_block) );
		int runframe = GetRunFrame() * ( fabs(distance) / CLIENT_TILESIZE );
		
		// 다음 타일값이 다를 경우
		if( m_Mov.next.height != m_matChr._42 )	
		{
//			m_Mov.step_height = (m_Mov.next.height-m_matChr._42) / GetRunFrame();
			m_Mov.step_height = (m_Mov.next.height-m_matChr._42) / runframe;

//			RLG4( "m_Mov.step_height = %3.3f, runframe = %d, distance = %3.3f, dist_block = %d", m_Mov.step_height, runframe, distance, dist_block );
		}
		// 다음 타일과 현재 타일의 높이가 같다
		else
		{
			// 이동값 초기화
			m_Mov.step_height	= 0.0f;
			m_Mov.now_Xrad		= 0.0f;
			m_Mov.nex_Xrad		= 0.0f;

			// 매트릭스 초기화
			D3DXMATRIX	matPos;			
			D3DXMatrixIdentity( &matPos );
			matPos._41 = m_matChr._41;
			matPos._42 = m_matChr._42;
			matPos._43 = m_matChr._43;			
			D3DXMatrixRotationYawPitchRoll( &m_matChr, m_Mov.nex_Yrad, m_Mov.nex_Xrad, 0.0f );
			D3DXMatrixMultiply( &m_matChr, &m_matChr, &matPos );
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: 해당 타일값으로 바라보기
//-----------------------------------------------------------------------------
void Character::SetRotate( TILE_INFO Tile )
{				
	if( Tile.x == m_Mov.curt.x && Tile.y == m_Mov.curt.y )
	{
		return;
	}

	// 현재 위치저장
	float fPosX, fPosY;
	GetRealTM( fPosX, fPosY );
	int nDestX = ((Tile.x*8)+4)-fPosX;
	int nDestY = ((Tile.y*8)+4)-fPosY;
	
	// 현재 각하고 다르다면..
	m_Mov.nex_Yrad = -atan2((float)nDestY, (float)nDestX ) - ( D3DX_PI / 2.0f );

	if( m_Mov.now_Yrad != m_Mov.nex_Yrad )
	{	
		// 회전플래그 true
		m_Mov.bYRotate = true;
		m_Mov.rad_count = 0;
		m_Mov.frad_remain = 0.0f;

		// 회전각 계산
		if( m_Mov.nex_Yrad-m_Mov.now_Yrad >= D3DX_PI )				
		{
			m_Mov.add_Yrad = ( (m_Mov.nex_Yrad - m_Mov.now_Yrad ) - ( D3DX_PI * 2.0f ) ) / (float)ROTATE_FRAME;
		}
		else if( m_Mov.nex_Yrad-m_Mov.now_Yrad <= -D3DX_PI )
		{
			m_Mov.add_Yrad = ( ( D3DX_PI * 2.0f ) + ( m_Mov.nex_Yrad - m_Mov.now_Yrad ) ) / (float)ROTATE_FRAME;
		}
		else		
		{
			m_Mov.add_Yrad = ( m_Mov.nex_Yrad-m_Mov.now_Yrad ) / (float)ROTATE_FRAME;
		}
	}
}

//-----------------------------------------------------------------------------
// Desc: 해당 회전값으로 바라보기
//-----------------------------------------------------------------------------
void Character::SetDirection( float fRot, bool bHorse )
{	
	// 회전정보 갱신
	m_Mov.bYRotate	= false;
	m_Mov.nex_Yrad	= fRot;
	m_Mov.now_Yrad	= fRot;	
	m_Mov.rad_count = 0;
	m_Mov.frad_remain = 0.0f;

	// 메트릭스값 조정
	if( m_curt_event.type != NULL && m_curt_event.type != SM_ACT )
	{
	//	m_matChr._41 = (m_curt_event.x*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);
	//	m_matChr._42 = g_Pc.GetMapHeight( m_curt_event.x, m_curt_event.y );
	//	m_matChr._43 = (m_curt_event.y*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);

		D3DXMATRIX	matPos = m_matChr;
		
		D3DXMATRIX matRotY;

		D3DXMatrixRotationY( &matRotY, fRot );
		m_vDir = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		D3DXVec3TransformCoord( &m_vDir , &m_vDir , &matRotY );

		BOOL Is4FootUnit = FALSE;

		if( !m_bPC )	///-- NPC일 경우
		{
			DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
			SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

			if(npc_Info)
			{
				if( npc_Info->theGender == n_4FootUnit )
				{
					Is4FootUnit = TRUE;
				}
			}
		}

		if(Is4FootUnit)
		{
			D3DXMATRIX matRotX;	
			
			D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
			vNowPos.y = g_Map.PHF_GetHeight( vNowPos.x , vNowPos.z );
			D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
			vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

			float fTempYRadian = 0.0f;
			float fXRadian = 0.0f;

			GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

			m_Mov.now_Xrad = fXRadian;

			D3DXMatrixRotationX( &matRotX , fXRadian );
			D3DXMatrixMultiply( &m_matChr , &matRotX , &matRotY );		
		}
		else
		{
			m_matChr = matRotY;
		}

		m_matChr._41 = matPos._41;
		m_matChr._42 = matPos._42;
		m_matChr._43 = matPos._43;
	}

	if( m_Mov.now_Yrad < 0 )
	{
		m_Mov.now_Yrad += (D3DX_PI * 2);
	}

	return;
}


//-----------------------------------------------------------------------------
// Desc: 캐릭터의 실제 좌표값
//-----------------------------------------------------------------------------
bool Character::GetRealTM( float& _fX, float& _fY )
{
//	if( m_bPC == true )
//	{
		_fX = m_matChr._41;
		_fY = m_matChr._43;
//	}
//	else
//	{
//		BYTE byBlock = g_Pc_Manager.GetNpcBlockType( m_nTableIndex );
//		_fX	= m_matChr._41-(byBlock*(CLIENT_TILESIZE/2));
//		_fY = m_matChr._43-(byBlock*(CLIENT_TILESIZE/2));
//	}
	
	return m_bPC;
}

D3DXMATRIX Character::GetPosTM()
{
	return m_matChr;
}

D3DXMATRIX Character::GetPosEyeTM()
{
	if(m_State.bRide)
	{
		D3DXMATRIX mat;
		mat = m_matChr;
		mat._42 += m_fAddRideHeight;
		return	mat;
	}
	
	return m_matChr;
}

D3DXMATRIX  Character::GetBonCenterTM()
{
	D3DXVECTOR3 vTemp;
	D3DXQUATERNION	qtRot;
	D3DXMATRIX matTemp;
	D3DXVECTOR3 vMax, vMin;
	
	if( g_Map.IsGiantUnit( this ) )
	{
		D3DXQuaternionRotationMatrix( &qtRot, &m_matChr );
		D3DXMatrixRotationQuaternion( &matTemp, &qtRot );

		vMax = GetBoundMaxPos();
		vMin = GetBoundMinPos();

		vTemp = D3DXVECTOR3( ( (vMax.x + vMin.x) / 2.0f),
								0.0f,
							 ( (vMax.z + vMin.z) / 2.0f) );
		D3DXVec3TransformCoord( &vTemp, &vTemp, &matTemp );

		matTemp = m_matChr;
		matTemp._41 += vTemp.x;
		matTemp._43 += vTemp.z;
		
		return matTemp;
	}
	else
	{
		if(m_State.bRide)
		{
			return	m_matRideCenter;
		}
		
		return m_matBoneCenter;
	}	

	return m_matBoneCenter;
}

D3DXMATRIX Character::GetPosEffTM()
{
	if(m_State.bRide)
	{
		return	m_matRideChr;
	}
	
	return m_matChr;
}

//-----------------------------------------------------------------------------
// Desc: 노드값에 해당하는 애니메이션값 리턴
//-----------------------------------------------------------------------------
D3DXMATRIX	Character::GetBoneTM( DWORD _dwNode )
{		
	// 노드에 따른 링크되는 본	
	_dwNode = g_Pc_Manager.GetArmorIndex( this, _dwNode );
	
	// 다음프레임	
	int next_frame = GetNextFrame();

	//-----------------------------------------------------------------------------
	// 쿼터니온*로컬*위치 = 해당 본의 메트릭스
	//-----------------------------------------------------------------------------
	D3DXMATRIX		matTemp;
	D3DXQUATERNION	qtRot;

	// 회전
	D3DXQuaternionSlerp( &qtRot,
						 &g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_qtRot,
						 &g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_qtRot,
						 m_middle_time );
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );

	// 로컬
	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )	
	{
		D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, _dwNode ), &matTemp );
	}
	else
		D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, _dwNode ),	&matTemp );

	// 위치
	matTemp._41 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.x -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.x ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.x;			 

	matTemp._42 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.y -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.y ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.y;

	matTemp._43 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.z -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.z ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.z;

	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );

	//-----------------------------------------------------------------------------
	// 오브젝트 회전한 만큼 메트릭스 회전하기
	//-----------------------------------------------------------------------------
	D3DXQuaternionRotationMatrix( &qtRot, &m_matChr );
	D3DXMatrixRotationQuaternion( &matTemp2, &qtRot );
	
	//-----------------------------------------------------------------------------
	// 월드상의 위치값 더하기
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
	
	matTemp._41 += m_matChr._41;
	matTemp._42 += m_matChr._42;
	matTemp._43 += m_matChr._43;
	
	return matTemp;
}

//-----------------------------------------------------------------------------
// Desc: 노드값에 해당하는 애니메이션값 리턴
//-----------------------------------------------------------------------------
D3DXMATRIX	Character::GetBoneRealTM( DWORD _dwNode )
{		
	// 다음프레임	
	int next_frame = GetNextFrame();

	//-----------------------------------------------------------------------------
	// 쿼터니온*로컬*위치 = 해당 본의 메트릭스
	//-----------------------------------------------------------------------------
	D3DXMATRIX		matTemp;
	D3DXQUATERNION	qtRot;

	// 회전
	D3DXQuaternionSlerp( &qtRot,
						 &g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_qtRot,
						 &g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_qtRot,
						 m_middle_time );
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );

	// 로컬
	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )	
	{
		D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, _dwNode ), &matTemp );
	}
	else
		D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, _dwNode ),	&matTemp );

	// 위치
	matTemp._41 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.x -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.x ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.x;			 

	matTemp._42 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.y -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.y ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.y;

	matTemp._43 = (( g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[next_frame].m_vPos.z -
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.z ) * m_middle_time ) +
					 g_Pc_Manager.m_pAni[m_ani_number]->m_pNode[_dwNode].m_pAnimation[m_ani_frame].m_vPos.z;

	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );

	//-----------------------------------------------------------------------------
	// 오브젝트 회전한 만큼 메트릭스 회전하기
	//-----------------------------------------------------------------------------
	D3DXQuaternionRotationMatrix( &qtRot, &m_matChr );
	D3DXMatrixRotationQuaternion( &matTemp2, &qtRot );
	
	//-----------------------------------------------------------------------------
	// 월드상의 위치값 더하기
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
	
	matTemp._41 += m_matChr._41;
	matTemp._42 += m_matChr._42;
	matTemp._43 += m_matChr._43;

	return matTemp;
}


//-----------------------------------------------------------------------------
// Desc: 캐릭터가 바라보는 방향값 구하기(타일단위임)
//-----------------------------------------------------------------------------
int Character::GetAngle( int prex, int prey, int curx, int cury )
{
	if( prex > curx )
	{
		if( prey > cury ) 
			return 0;
		if( prey < cury ) 
			return 6;
		else 
			return 7;
	}
	else if( prex < curx )
	{
		if( prey > cury ) 
			return 2;
		if( prey < cury ) 
			return 4;
		else 
			return 3;
	}
	else
	{
		if( prey > cury ) 
			return 1;
		else if ( prey < cury ) 
			return 5;
		else 
			return -1;	//거기가 거기임
	}
	return -1;
}


int Character::GetAngle( long pre, long current )
{
	int prex, prey, curx, cury;
	prex = pre % g_Pc.GetMapSize();
	prey = pre / g_Pc.GetMapSize();
	curx = current % g_Pc.GetMapSize();
	cury = current / g_Pc.GetMapSize();

	if( prex > curx )
	{
		if( prey > cury ) 
			return 0;
		if( prey < cury ) 
			return 6;
		else 
			return 7;
	}
	else if( prex < curx )
	{
		if( prey > cury ) 
			return 2;
		if( prey < cury ) 
			return 4;
		else 
			return 3;
	}
	else
	{
		if( prey > cury ) 
			return 1;
		else if ( prey < cury ) 
			return 5;
		else 
			return -1;	//거기가 거기임
	}

	return -1;
}

//-----------------------------------------------------------------------------
// Desc: 이동애니메이션처리
//-----------------------------------------------------------------------------
void Character::Move( float middle_time )
{	
	// 자기캐릭터이거나, 합체한 인간 유니트이거나	
	float value_x = (m_Mov.step_x-(m_Mov.step_x*middle_time));
	float value_y = (m_Mov.step_height-(m_Mov.step_height*middle_time));
	float value_z = (m_Mov.step_y-(m_Mov.step_y*middle_time));

	///-- 이동 변위양을 체크해본다.
	D3DXVECTOR3 vDel = D3DXVECTOR3(value_x, 0.0f, value_z);
	///-- By simwoosung 이값이 같지 않아서 이동중에 속도의 차이가 발생하게된다.
	///-- 어떤 수정작업이 필요할듯....
	///-- 그렇다고 이걸 같은 변위량으로 쓰면... 도착위치에 다다를때 감속현상이 
	///-- 발생하지 않게된다..
	///-- 변위량이 어떤값 이하일때만 그냥써주고.. 
	///-- 나머지는 일정한 값으로 바꿔주는게 좋을듯...
	float fDel = D3DXVec3Length(&vDel);
		
	///D3DXVECTOR3	voPos = D3DXVECTOR3(m_matChr._41, 0.0f, m_matChr._43);
	// 캐릭터이동
	m_matChr._41 += value_x;
///	m_matChr._42 += value_y;
	m_matChr._43 += value_z;
	/// sooree height
	m_matChr._42 = g_Map.Get__CharacterMoveHeight( m_matChr._41, m_matChr._43, m_Mov.curt.x, m_Mov.curt.y, m_Mov.next.x, m_Mov.next.y );

	// 유저일때만
	if( m_bPC == true )
	{
		// 말타고 있다면 말도 이동
//		if( m_State.bRide == true )		
//			g_Map.m_Npc[m_State.horse_index].SetPosTM( m_matChr );

		if ( !(m_ani_frame%5) && m_matChr._42 <= g_Map.m_fBaseHeightWater )
			g_Map.m_WaveList.AddWave(m_matChr._41, g_Map.m_fBaseHeightWater, m_matChr._43);
	}
}

//-----------------------------------------------------------------------------
// Desc: 이동애니메이션처리
//-----------------------------------------------------------------------------
void Character::MoveRatio( float fRatio )
{	
	// 자기캐릭터이거나, 합체한 인간 유니트이거나	
	float value_x = (m_Mov.step_x * fRatio);
	float value_y = (m_Mov.step_height * fRatio);
	float value_z = (m_Mov.step_y * fRatio);

	///-- 이동 변위양을 체크해본다.
	D3DXVECTOR3 vDel = D3DXVECTOR3(value_x, 0.0f, value_z);
	///-- By simwoosung 이값이 같지 않아서 이동중에 속도의 차이가 발생하게된다.
	///-- 어떤 수정작업이 필요할듯....
	///-- 그렇다고 이걸 같은 변위량으로 쓰면... 도착위치에 다다를때 감속현상이 
	///-- 발생하지 않게된다..
	///-- 변위량이 어떤값 이하일때만 냥써주고.. 
	///-- 나머지는 일정한 값으로 바꿔주는게 좋을듯...
	float fDel = D3DXVec3Length(&vDel);
		
	///D3DXVECTOR3	voPos = D3DXVECTOR3(m_matChr._41, 0.0f, m_matChr._43);

	// 캐릭터이동
	m_matChr._41 += value_x;
///	m_matChr._42 += value_y;
	m_matChr._43 += value_z;
	/// sooree height
	m_matChr._42 = g_Map.Get__CharacterMoveHeight( m_matChr._41, m_matChr._43, m_Mov.curt.x, m_Mov.curt.y, m_Mov.next.x, m_Mov.next.y );	
	
	// 유저일때만
	if( m_bPC == true )
	{
		// 말타고 있다면 말도 이동
//		if( m_State.bRide == true )		
//			g_Map.m_Npc[m_State.horse_index].SetPosTM( m_matChr );

		if ( !(m_ani_frame%5) && m_matChr._42 <= g_Map.m_fBaseHeightWater )
			g_Map.m_WaveList.AddWave(m_matChr._41, g_Map.m_fBaseHeightWater, m_matChr._43);
	}
}


void Character::MiddleMove( float middle_time )
{	
	// 자기캐릭터이거나, 합체한 인간 유니트이거나
	float value_x = (m_Mov.step_x*(middle_time-m_step_time));
	float value_y = (m_Mov.step_height*(middle_time-m_step_time));
	float value_z = (m_Mov.step_y*(middle_time-m_step_time));	

	// 캐릭터 보간 이동
	m_matChr._41	+= value_x;
//	m_matChr._42	+= value_y;
	m_matChr._43	+= value_z;
	/// sooree height
	m_matChr._42 = g_Map.Get__CharacterMoveHeight( m_matChr._41, m_matChr._43, m_Mov.curt.x, m_Mov.curt.y, m_Mov.next.x, m_Mov.next.y );	

	m_step_time		= middle_time;

	// 말타고 있다면 말도 이동
//	if( m_bPC == true && m_State.bRide == true )	
//		g_Map.m_Npc[m_State.horse_index].SetPosTM( m_matChr );
}


void Character::Rotate( float fRatio, int aAddFrame )
{	
	// 위치값
	D3DXMATRIX	matPos;			
	D3DXMatrixIdentity( &matPos );
	matPos._41 = m_matChr._41;
	matPos._42 = m_matChr._42;
	matPos._43 = m_matChr._43;

	// Y회전
	if( m_Mov.bYRotate == true )
	{				
		if( m_Mov.rad_count < ROTATE_FRAME )
		{
			///--JAPAN_BUG_MODIFY
			m_Mov.rad_count += aAddFrame;			
			float fCumul = m_Mov.frad_remain;
			fCumul += fRatio;
			m_Mov.rad_count += (int)fCumul;
			m_Mov.frad_remain = fCumul - (float)((int)fCumul);			
			
			int nCount = (aAddFrame) + (int)fCumul;
			for(int i = 0 ; i < nCount ; i++)
			{
				m_Mov.now_Yrad += m_Mov.add_Yrad;
			}
		}
		
		if( m_Mov.rad_count >= ROTATE_FRAME ) 
		{
			m_Mov.bYRotate = false;			
			m_Mov.now_Yrad = m_Mov.nex_Yrad;			
		}		
	}

	m_radian = m_Mov.now_Yrad;

	D3DXMATRIX matRotY;
	D3DXMatrixRotationY( &matRotY , m_Mov.now_Yrad );
	m_vDir = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	D3DXVec3TransformCoord( &m_vDir , &m_vDir , &matRotY );

	D3DXMatrixRotationYawPitchRoll( &m_matChr, m_Mov.now_Yrad, m_Mov.now_Xrad, 0.0f );
	D3DXMatrixMultiply( &m_matChr, &m_matChr, &matPos );	

	if( m_Mov.now_Yrad < 0 )
	{
		m_Mov.now_Yrad += (D3DX_PI * 2);
	}

	return;
}


//-----------------------------------------------------------------------------
// Desc: 아이템 장착,교체
//-----------------------------------------------------------------------------
/*int Character::SM__CHANGE__ITEM( DWORD item_id, int item_unique )
{	
	// 0이면 장착실패
	if( item_id == 0 )	
		return -1;	
	
	// ID 분석해서 어떤 아이템인지 알아내야함
	int result = 0;
	int index  = g_Pc_Manager.GetItemID( item_id );
	if( index == -1 )
	{
		return -1;	
	}

	switch( g_Pc_Manager.m_pItm[index].m_class )
	{
	// 한손검
	case 0:
		{
			// log
			if( m_pUserInfo == NULL )
			{
				#ifdef	GAMETYPE_TEST
					MessageBox( NULL, "SM_CHANGE_ITEM Error", "Error", MB_OK );
				#endif
				return -1;
			}

			// 아이템인덱스저장
			m_pUserInfo->ArmorInfo.index[0] = index;

			// 활
			if( g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]].m_class == 1 )		
				m_pUserInfo->ArmorInfo.index[1] = -1;		

			// 칼검기(잔상)만들기 위해서 높이값 구하기
			int mod_index = g_Pc_Manager.m_pItm[index].m_mod_index;
			for( int num = 0; num < g_Pc_Manager.m_pMod[mod_index].m_header.m_num_vertex; ++num )
			{
				if( m_pUserInfo->AfterImage.vTop.y < g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex.y )
					m_pUserInfo->AfterImage.vTop = g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex;
				if( m_pUserInfo->AfterImage.vBottom.y > g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex.y )					
					m_pUserInfo->AfterImage.vBottom = g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex;
			}
		}
		break;
	// 활
	case 1:
		{	
			// log
			if( m_pUserInfo == NULL )
			{
				#ifdef	GAMETYPE_TEST
					MessageBox( NULL, "SM_CHANGE_ITEM Error", "Error", MB_OK );
				#endif
				return -1;
			}

			// 화살통이 아니라면.. 오른손 아이템은 그리지 않느다
			int RHand = m_pUserInfo->ArmorInfo.index[0];
			if( !(g_Pc_Manager.m_pItm[RHand].m_class == 3 && g_Pc_Manager.m_pItm[RHand].m_type == 4) )
				m_pUserInfo->ArmorInfo.index[0] = -1;
			m_pUserInfo->ArmorInfo.index[1] = index;			
		}
		break;
	// 방패, 화살통
	case 3:
		{
			switch( g_Pc_Manager.m_pItm[index].m_type )
			{
			// 화살통
			case 4:				
				m_pUserInfo->ArmorInfo.index[0] = index;				
				break;
			// 요정날개
			case 12:				
				m_pItm_index[5] = index;				
				break;
			// 방패
			default:				
				m_pUserInfo->ArmorInfo.index[1] = index;				
				break;
			}
		}
		break;	
	// 갑옷(몸통,손,발,머리카락)
	default:
		{
			//for( int i = 0; i < m_itm_count; ++i )
			for( int i = 0; i < g_Pc_Manager.GetItmCount( this ); ++i )
			{
				if( g_Pc_Manager.m_pItm[m_pItm_index[i]].m_class == g_Pc_Manager.m_pItm[index].m_class && 
					g_Pc_Manager.m_pItm[m_pItm_index[i]].m_type  == g_Pc_Manager.m_pItm[index].m_type )
				{
					// 인덱스 저장
					m_pItm_index[i] = index;

					// 버퍼재할당
					SetItemBuffer( i );	
					break;
				}
			}
		}
		break;
	}
	
	// 게임중일때만 저장한다
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return -1;

	// 인벤토리에 저장
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		// 장비저장
		for( int i = 0; i < MAX_EQUIPED; ++i )
		{
			int curt_item = g_Pc_Manager.GetItemID( g_Pc.m_Equip[i].index );
			int next_item = g_Pc_Manager.GetItemID( item_id );
			if( g_Pc.m_Equip[i].index == 0 )
			{				
				g_Pc.m_Equip[i].index  = item_id;
				g_Pc.m_Equip[i].unique = item_unique;			
				break;
			}
			else if( g_Pc_Manager.m_pItm[curt_item].m_class == g_Pc_Manager.m_pItm[next_item].m_class )
			{				
				g_Pc.m_Equip[i].index  = item_id;
				g_Pc.m_Equip[i].unique = item_unique;
				break;
			}
		}
		if( i != MAX_EQUIPED )
		{
			for( int j = 0; j < MAX_INVENTORY1; ++j )
			{				
				if( g_Pc.m_Tool_Inventory1[j].index == item_id)
				{
					int TempLife = g_Pc.m_Tool_Inventory1[j].life;					
					g_Pc.m_Tool_Inventory1[j].life = g_Pc.m_Equip[i].life;
					g_Pc.m_Equip[i].life = TempLife;
					break;
				}
			}			
		}
		else
		{
			// 인벤토리저장
			for( i = 0; i < MAX_INVENTORY1; ++i )
			{
				int curt_item = g_Pc_Manager.GetItemID( g_Pc.m_Tool_Inventory1[i].index  );
				int next_item = g_Pc_Manager.GetItemID( item_id );
				if( g_Pc.m_Tool_Inventory1[i].index == 0 )
				{
					g_Pc.m_Tool_Inventory1[i].index  = item_id;
					g_Pc.m_Tool_Inventory1[i].unique = item_unique;
					break;
				}
				else if( g_Pc_Manager.m_pItm[curt_item].m_class == g_Pc_Manager.m_pItm[next_item].m_class )
				{					
					g_Pc.m_Tool_Inventory1[i].index  = item_id;
					g_Pc.m_Tool_Inventory1[i].unique = item_unique;
					break;
				}
			}
		}
	}

	// 무기, 방패 장착했다면 애니메이션 변경
	if( m_pUserInfo->ArmorInfo.index[0] != -1 && m_pUserInfo->ArmorInfo.index[1] != -1 )
	{		
		if( m_curt_event.type != SM_MOVE_BROADCAST )
			AnimationPlay( STANDARD0 );		
	}

	//-----------------------------------------------------------------------------
	// 텍스쳐업데이트
	//-----------------------------------------------------------------------------
	UpdateTexture();
	return result;
}


//-----------------------------------------------------------------------------
// Desc: 아이템 벗기
//-----------------------------------------------------------------------------
int Character::SM__TAKEOFF__ITEM( DWORD item_id, int item_unique )
{	
	// 0이면 장착실패
	if( item_id == 0 )
		return -1;

	// ID 분석해서 어떤 아이템인지 알아내야함
	int result = 0;
	int index = g_Pc_Manager.GetItemID( item_id );
	if( item_id == -1 )
	{
		return -1;	
	}

	switch( g_Pc_Manager.m_pItm[index].m_class )
	{
	// 칼
	case 0:
		{
			if( index == m_pUserInfo->ArmorInfo.index[0] )
				m_pUserInfo->ArmorInfo.index[0] = -1;
		}
		break;
	// 활
	case 1:
		{
			m_pUserInfo->ArmorInfo.index[1] = -1;
		}
		break;
	// 방패
	case 3:
		{
			switch( g_Pc_Manager.m_pItm[index].m_type )
			{
			// 화살통
			case 4:
				m_pUserInfo->ArmorInfo.index[0]	= -1;
				break;
			// 요정날개
			case 12:				
				m_pItm_index[5] = g_Pc_Manager.m_PcAttr[GetAttrIndex()].wing;				
				break;
			// 방패
			default:				
				m_pUserInfo->ArmorInfo.index[1]	= -1;				
				break;
			}	
		}
		break;
	// 갑옷(몸통,손,발,머리카락)
	default:
		{
			//for( int i = 0; i < m_itm_count; ++i )
			for( int i = 0; i < g_Pc_Manager.GetItmCount( this ); ++i )
			{
				if( g_Pc_Manager.m_pItm[m_pItm_index[i]].m_class == g_Pc_Manager.m_pItm[index].m_class && 
					g_Pc_Manager.m_pItm[m_pItm_index[i]].m_type  == g_Pc_Manager.m_pItm[index].m_type )
				{
					if( index == m_pItm_index[i] )
					{
						int attr_index = GetAttrIndex();

						// 갑옷						
						if( g_Pc_Manager.m_pItm[index].m_class == 2 )							
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].body;
						// 투구
						else if( g_Pc_Manager.m_pItm[index].m_class == 4 )
						{
							if( attr_index != UNION_FAIRY )
							{
								if( m_nTableIndex == MAX_USER_INTABLE )	
									m_pItm_index[i] = g_EtcTable.GetHairIndex( g_Pc.m_MainPC.char_info.hair );
								else
									m_pItm_index[i] = g_EtcTable.GetHairIndex( g_Pc.m_PcTable[m_nTableIndex].char_info.hair );
							}
							else
							{
								m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hair;
							}
						}
						// 신발
						else if( g_Pc_Manager.m_pItm[index].m_class == 5 )
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].foot;
						// 손
						else if( g_Pc_Manager.m_pItm[index].m_class == 6 )
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hand;
											
						// 버퍼 재할당					
						SetItemBuffer( i );			
						break;
					}
				}
			}
		}
		break;
	}
	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		// 장비에서 해제
		bool bFind = false;
		for( int i = 0; i < MAX_EQUIPED; ++i )
		{
			if( g_Pc.m_Equip[i].index == item_id )
			{
				if( g_Pc.m_Equip[i].unique == item_unique || item_unique == 0 )
				{
					g_Pc.m_Equip[i].index	= 0;
					g_Pc.m_Equip[i].unique	= 0;
					g_Pc.m_Equip[i].life	= 0;
					bFind = true;
					break;
				}
			}
		}
		// 인벤토리에서 삭제
		if( bFind == false )
		{			
			for( int j = 0; j < MAX_INVENTORY1; ++j )
			{
				if( g_Pc.m_Tool_Inventory1[j].index == item_id )
				{
					if( g_Pc.m_Tool_Inventory1[j].unique == item_unique || item_unique == 0 )
					{
						g_Pc.m_Tool_Inventory1[j].index		= 0;
						g_Pc.m_Tool_Inventory1[j].unique	= 0;
						g_Pc.m_Tool_Inventory1[j].life		= 0;
						break;
					}
				}
			}
		}
	}

	//-----------------------------------------------------------------------------
	// 아무것도 장착하지 않고 있다면 기본 서있기로 애니메이션 변경
	//-----------------------------------------------------------------------------
	if( m_pUserInfo->ArmorInfo.index[0] == -1 && m_pUserInfo->ArmorInfo.index[1] == -1 )	
	{		
		m_pUserInfo->bAttack = false;
		if( m_curt_event.type != SM_MOVE_BROADCAST )
			AnimationPlay( STANDARD0 );	
	}	

	//-----------------------------------------------------------------------------
	// 텍스쳐업데이트
	//-----------------------------------------------------------------------------
	UpdateTexture();
	return result;
}

*/
//-----------------------------------------------------------------------------
// Desc: 아이템 버퍼 재할당하기
//-----------------------------------------------------------------------------
void Character::SetItemBuffer( int _nIndex )
{	
	//-----------------------------------------------------------------------------
	// 머리카락은 선택한 스타일때문에 따로 처리한다
	//-----------------------------------------------------------------------------
/*	if( _nIndex == HAIR_INDEX && GetAttrIndex() != UNION_FAIRY )
	{
		if( g_EtcTable.FindHairIndex( m_pItm_index[_nIndex] ) == true )
		{
			int hair_style;
			if( m_nTableIndex == MAX_USER_INTABLE )					
				hair_style = g_Pc.m_MainPC.char_info.hair;
			else
				hair_style = g_Pc.m_PcTable[m_nTableIndex].char_info.hair;		

			// 자기 머리카락 스타일로 변경
			if( ChangeHair( m_nTableIndex, hair_style ) == true )
				return;
		}
	} */

	//-----------------------------------------------------------------------------
	// 버텍스배열의 초기화플래그 끄기
	//-----------------------------------------------------------------------------
	m_pVtxBuffer[_nIndex].CreateBuffer( g_Pc_Manager.GetVtxCount(this, _nIndex) );
}


void Character::SetBasicItemBuffer( int _nIndex )
{	
	m_Basic_VtxBuffer[_nIndex].CreateBuffer( g_Pc_Manager.GetBasicVtxCount( this, _nIndex ) );

	return;
}

void Character::SetRideItemBuffer( int _nIndex )
{
	m_Ride_VtxBuffer[_nIndex].CreateBuffer( g_Pc_Manager.GetRideVtxCount( this , _nIndex ) );
	
	return;	
}

//-----------------------------------------------------------------------------
// Desc: 아이템 집기
//-----------------------------------------------------------------------------
/*void Character::SM__PICKUP__ITEM( DWORD item_id, DWORD unique, DWORD life )
{
	// ID 분석해서 어떤 아이템인지 알아내야함	
	if( item_id == 0 )
		return;	
	int index = g_Pc_Manager.GetItemID( item_id );
	if( index == -1 )
	{
		return;
	}
	
	// 인벤토리에 넣기
	for( int i = 0; i < MAX_INVENTORY1; ++i )
	{
		if( g_Pc.m_Tool_Inventory1[i].index == 0 )
		{
			g_Pc.m_Tool_Inventory1[i].index		= item_id;
			g_Pc.m_Tool_Inventory1[i].unique	= unique;
			g_Pc.m_Tool_Inventory1[i].life		= life;
			return;
		}
	}	
}


//-----------------------------------------------------------------------------
// Desc: 아이템 버리기
//-----------------------------------------------------------------------------
void Character::SM__DROP__ITEM( DWORD item_id, DWORD unique )
{	
	// ID 분석해서 어떤 아이템인지 알아내야함	
	if( item_id == 0 )
		return;	
	int index = g_Pc_Manager.GetItemID( item_id );	
	if( item_id == -1 )	
		return;		

	// 현재 장착중인지 검색
	for( int i = 0; i < MAX_EQUIPED; ++i )
	{
		if( g_Pc.m_Equip[i].index == item_id && g_Pc.m_Equip[i].unique == unique )
		{	
			// 갑옷, 장갑, 투구, 신발
			//for( int j = 0; j < m_itm_count; ++j )
			for( int j = 0; j < g_Pc_Manager.GetItmCount( this ); ++j ) 
			{				
				if( m_pItm_index[i] == index )
				{
					SM__TAKEOFF__ITEM( item_id, unique );
					i = MAX_EQUIPED;
					break;
				}
			}
			// 무기종류
			for( j = 0; j < LINK_ITEM_COUNT; ++j )
			{		
				if( m_pUserInfo->ArmorInfo.index[j] == index )
				{
					SM__TAKEOFF__ITEM( item_id, unique );
					i = MAX_EQUIPED;
					break;
				}
			}
		}		
	}	
	// 인벤토리1에서 찾기
	for( i = 0; i < MAX_INVENTORY1; ++i )
	{
		if( g_Pc.m_Tool_Inventory1[i].index == item_id && g_Pc.m_Tool_Inventory1[i].unique == unique )
		{
			g_Pc.m_Tool_Inventory1[i].index		= 0;
			g_Pc.m_Tool_Inventory1[i].unique	= 0;
			g_Pc.m_Tool_Inventory1[i].life		= 0;
			break;
		}
	}		
}

*/
//-----------------------------------------------------------------------------
// Desc: 캐릭터 위치.. 매트릭스로 받아서 타일에 맞게 수정
//-----------------------------------------------------------------------------
void Character::ClearEvent()
{
	m_curt_event.Clear();
	m_save_event.Clear();
	m_next_event.Clear();
	m_temp_event.Clear();

	return;
}


void Character::SetTilePos( int _nPosX, int _nPosY )
{	
	// 매트릭스저장 
	m_matChr._41 = (_nPosX*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);
	m_matChr._42 = g_Pc.GetMapHeight( _nPosX, _nPosY );
	m_matChr._43 = (_nPosY*CLIENT_TILESIZE)+(CLIENT_TILESIZE/2);

	BOOL Is4FootUnit = FALSE;

	if( !m_bPC )	///-- NPC일 경우
	{
		DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
		SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

		if(npc_Info)
		{
			if( npc_Info->theGender == n_4FootUnit )
			{
				Is4FootUnit = TRUE;
			}
		}
	}

	if(Is4FootUnit)
	{
		D3DXMATRIX matRotX;	
		
		D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
		D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
		vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

		float fTempYRadian = 0.0f;
		float fXRadian = 0.0f;

		GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

		m_Mov.now_Xrad = fXRadian;			
	}

	if( g_Map.m_nPickedNpc != -1 )
	{
		float dist = 0 ; 
		if( GetUnique() == g_Map.m_Npc[g_Map.m_nPickedNpc].GetUnique())
		{
			
			dist = DIST(g_Map.m_Npc[g_Map.m_nPickedNpc].m_Mov.curt.x , g_Map.m_Npc[g_Map.m_nPickedNpc].m_Mov.curt.y 
				, _nPosX , _nPosY );
			if(dist > 2.0f)
			{
				int a=0;
			}
		}
	}



	// 위치값초기화
	m_Mov.Init( _nPosX, _nPosY );

	if(theFindWay)
	{
		m_Mov.vStPos.x = m_matChr._41;
		m_Mov.vStPos.z = m_matChr._43;
		m_Mov.vCVtPos.x = m_matChr._41;
		m_Mov.vCVtPos.z = m_matChr._43;
	}
}


void Character::SetPosTM( const D3DXMATRIX mat )
{	
	BOOL Is4FootUnit = FALSE;

	if( !m_bPC )	///-- NPC일 경우
	{
		DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
		SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

		if(npc_Info)
		{
			if( npc_Info->theGender == n_4FootUnit )
			{
				Is4FootUnit = TRUE;
			}
		}
	}	
	
	if( Is4FootUnit && (m_State.nRideState == nRideState_Ride) )
	{
		D3DXMATRIX matRotX, matRotY;		
		
		D3DXVECTOR3 vDir;
		float fYRadian = 0.0f;
		
		if( m_State.TransIndex == MAX_USER_INTABLE)
		{
			vDir = g_Pc.GetPlayer()->m_vDir;
			fYRadian = g_Pc.GetPlayer()->m_Mov.now_Yrad;
		}
		else
		{
			vDir = g_Pc.m_Pc[m_State.TransIndex].m_vDir;
			fYRadian = g_Pc.m_Pc[m_State.TransIndex].m_Mov.now_Yrad;
		}

		D3DXVECTOR3 vNowPos = D3DXVECTOR3(mat._41, mat._42, mat._43);
		vNowPos.y = g_Map.PHF_GetHeight( vNowPos.x , vNowPos.z );
		D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * vDir);
		vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );
		
		float fTempYRadian = 0.0f;
		float fXRadian = 0.0f;

		GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

		D3DXMatrixRotationX( &matRotX , fXRadian );
		D3DXMatrixRotationY( &matRotY , fYRadian );		
		D3DXMatrixMultiply( &m_matChr , &matRotX , &matRotY );

		m_matChr._41 = mat._41;
		m_matChr._42 = mat._42;
		m_matChr._43 = mat._43;
	}
	else
	{
		// 매트릭스저장
		m_matChr = mat;	
	}	

	// 위치값초기화
	m_Mov.Init( (int)m_matChr._41/CLIENT_TILESIZE, (int)m_matChr._43/CLIENT_TILESIZE );

	// 시간저장	
	m_SimTime = g_nowTime;
}


//-----------------------------------------------------------------------------
// Desc: 머리색, 머리종류, 얼굴 종류 바꾸기
//-----------------------------------------------------------------------------
bool Character::SetMaterialColor( int index, int itm, int color )
{	
	//-----------------------------------------------------------------------------
	// 페어리는 제외
	//-----------------------------------------------------------------------------
	/*
	//-----------------------------------------------------------------------------
	// 메터리얼값 갱신
	//-----------------------------------------------------------------------------
	// 상태에 따라서 메터리얼변화
	if( itm == 255 )
	{
		// 유저
		if( m_bPC == true )
		{
			// 일반상태
			if( g_Pc.m_PcTable[m_nTableIndex].nStatus < 5 )
				return false;
			// 특수상태			
			return true;			
		}
		// NPC
		else
		{
			// 일반상태
			if( g_Map.m_NpcTable[m_nTableIndex].nStatus < 5 )
				return false;
			// 특수상태			
			return true;
		}

		// 자기 아이템색을 모두 변화
		for( int i = 0; i < m_itm_count; ++i )
		{
			m_pMaterial[i].Diffuse = g_EtcTable.GetHairMaterial( color );
			m_pMaterial[i].Ambient = g_EtcTable.GetHairMaterial( color );
		}		
	}
	// 넣어준 칼라값을.. 헤어에서 찾기
	else
	*/
	{			
		m_pMaterial[itm].Diffuse = g_EtcTable.GetHairMaterial( color );
		m_pMaterial[itm].Ambient = g_EtcTable.GetHairMaterial( color );		
	}
	return true;
}

void Character::ChangeHair( DWORD aItemCode )
{	
	SItemBaseInfo* item_Info = NULL;	
	int chrAttrIndex = GetAttrIndex();


	// 여기 들어오기 전에 해주는게 좋겠지?? 일단은 걍 테스뚜~~~
	if( m_nTableIndex == MAX_USER_INTABLE )
	{					
		g_Pc.m_MainPC.char_info.theHair = aItemCode;

		if( nRui->GetPcInvenInfo()->WearItem[ n_WearSlot_SKIN_Helmet ])
		{
			
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->GetPcInvenInfo()->WearItem[ n_WearSlot_SKIN_Helmet ]->Code );
			if( item_Info && item_Info->theType == n_Type_Skin_KeepypHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
			{
				m_Basic_DrawItm[n_Wear_Hair] = 1;	
				
				m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
				SetBasicItemBuffer( n_Wear_Hair );
			}
			else
			{
				m_Basic_DrawItm[n_Wear_Hair] = 0;
				m_Basic_Itm_index[n_Wear_Hair] = 0;
			}		
		}
		else if( nRui->GetPcInvenInfo()->WearItem[ n_WearSlot_Helmet ] )
		{
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->GetPcInvenInfo()->WearItem[ n_WearSlot_Helmet ]->Code );
			if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
			{	
				m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode ); 
				SetBasicItemBuffer( n_Wear_Hair );
			}
		}
		else
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
			SetItemBuffer( n_Wear_Hair );	
		}
	}
	else
	{
		g_Pc.m_PcTable[m_nTableIndex].char_info.theHair = aItemCode;

		// 헤어 바꾸기 전에 착용하고 있던 방어구가 있다면 그 방어구가 Hair + Helmet 인지 Only Helmet 인지에 따라서 재설정해야 한다. 
		if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[ n_WearSlot_Helmet ].theCode )
		{
			item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[ n_WearSlot_Helmet ].theCode );
			if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
			{	
				m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode ); 
				SetBasicItemBuffer( n_Wear_Hair );
			}
		}
		else
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
			SetItemBuffer( n_Wear_Hair );	
		}
	}

	PreCharTextureCreate();

	return;
}

void Character::ChangeFace( DWORD aItemCode )
{
	if( m_nTableIndex == MAX_USER_INTABLE )
	{					
		g_Pc.m_MainPC.char_info.theFace = aItemCode;
	}
	else
	{
		g_Pc.m_PcTable[m_nTableIndex].char_info.theFace = aItemCode;
	}
	
	m_pItm_index[n_Wear_Face] = g_Pc_Manager.GetItemInfo_FileCode12( GetAttrIndex(), aItemCode );
	SetItemBuffer( n_Wear_Face );	

	PreCharTextureCreate();

	return;
}



//-----------------------------------------------------------------------------
// Desc: 리턴b
//-----------------------------------------------------------------------------
int		Character::GetAttrIndex()
{
	if( -1 != m_TempIndex )
	{
		return m_TempIndex;
	}

	// 유저
	if( true == m_bPC )
	{
		if( MAX_USER_INTABLE == m_nTableIndex )
			return g_Pc.m_MainPC.nIndex;
		else
			return g_Pc.m_PcTable[m_nTableIndex].nIndex;
	}

	return g_Map.m_NpcTable[m_nTableIndex].index;
}


// 어택타임
float	Character::GetAttackTime()
{	
	return GetAttackTime( m_ani_index );	
}

float	Character::GetAttackTime( unsigned int motion )
{		
	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
	{
		return PCATTACK_SPEED*g_Pc_Manager.m_PcAni[GetAttrIndex()][motion].sound_key;
	}

#ifdef DYNAMIC_RESOURCE_LOADING
	return PCATTACK_SPEED*((CHR_FILE_INFO *)g_Pc_Manager.m_pNpcFileInfo[GetAttrIndex()])->pAniInfo[motion+NPC_ATTACK_NUMBER].sound_key;
#else
	return PCATTACK_SPEED*g_Pc_Manager.m_pNpcFileInfo[GetAttrIndex()].pAniInfo[motion+NPC_ATTACK_NUMBER].sound_key;
#endif
}

// 버텍스값
D3DXVECTOR3		Character::GetVertex( int index, int number )
{
	// 파괴오브젝트
//	if( GetAttrIndex() == DESTROY_OBJECT_INDEX )	
//		return g_Pc_Manager.GetVertex( index, number );	

	if( m_pVtxBuffer[index].pVertex == NULL )
	{
		D3DXVECTOR3 aVec( 1.0f, 1.0f, 1.0f );
		return aVec;
	}

	// 기타
	return m_pVtxBuffer[index].pVertex[number];	
}

// 현재 공격이벤트 중이냐?
int		Character::GetAttackEvent( bool bSave )
{
	if( m_curt_event.type  == SM_ATTACK_NPC || m_curt_event.type  == SM_USE_SKILL )
		return 0;	
	if( bSave == true )
	{
		if( m_save_event.type == SM_ATTACK_NPC /* || m_save_event.type == CM_USE_SKILL */ )
			return 1; 
	}	
	return -1;
}


//-----------------------------------------------------------------------------
// Desc: 현재 정지이벤트
//-----------------------------------------------------------------------------
bool	Character::GetStopEvent( bool _bSave )
{
	// 현재 상태에 따라서 처리
	int nStatus = 0;
	if( MAX_USER_INTABLE == m_nTableIndex )
		nStatus = g_Pc.m_MainPC.nStatus;
	else
		nStatus = g_Pc.m_PcTable[m_nTableIndex].nStatus;
	switch( nStatus )
	{
	case STUN:			// 스턴		
		return true;		
	case SLEEP:			// 슬립
		return true;		
	}	
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 애니메이션 관련된 처리
//-----------------------------------------------------------------------------
int		Character::GetNextFrame()
{		
	// 프레임을 앞으로 돌린 것인지, 뒤로 돌릴 것인지
	int next_frame;
	if( AniFrameProcess() == true )
		next_frame = m_ani_frame+1;
	else
		next_frame = m_ani_frame-1;	
				
	// -1 프레임이거나, 마지막 프레임 넘겼다면 
	if( m_ani_frame == -1 )
	{		
		m_ani_frame = 0;
		if( m_curt_event.loop == false )	
			next_frame = 0;			
		else		
			next_frame = 1;		
	}
	// 다음프레임이 -1이라면
	else if( next_frame == -1 )
	{
		next_frame = 0;
	}
	// 다음프레임이 마지막프레임보다 크다면
	else if( next_frame > (int)m_end_frame )
	{		
		if( m_curt_event.loop == false )
			next_frame = m_ani_frame;
		else		
			next_frame = 0;
	}
	return next_frame;
}

bool	Character::AniFrameProcess()
{
	return true;
}

bool	Character::UpdateFrame() 
{
	bool bFind = false;
	switch( m_curt_event.type )
	{
	// 밀려나기	
	case SM_SLIDE_NPC:
		{
			m_matChr._41 += m_curt_event.vPos.x;
			//m_matChr._42 += m_curt_event.vPos.y;
			m_matChr._42 = g_Pc.GetMapHeightF( m_matChr._41, m_matChr._43 );
			m_matChr._43 += m_curt_event.vPos.z;

			BOOL Is4FootUnit = FALSE;

			if( !m_bPC )	///-- NPC일 경우
			{
				DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	///-- NPC가 4족보행인지 여부판단...
				SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

				if(npc_Info)
				{
					if( npc_Info->theGender == n_4FootUnit )
					{
						Is4FootUnit = TRUE;
					}
				}
			}

			if(Is4FootUnit)
			{
				D3DXMATRIX matRotX;	
				
				D3DXVECTOR3 vNowPos = D3DXVECTOR3(m_matChr._41, m_matChr._42, m_matChr._43);
				D3DXVECTOR3 vForwardPos = vNowPos + ( 8.0f * m_vDir);
				vForwardPos.y = g_Map.PHF_GetHeight( vForwardPos.x , vForwardPos.z );

				float fTempYRadian = 0.0f;
				float fXRadian = 0.0f;

				GetXAndYRadian(fXRadian, fTempYRadian, vNowPos, vForwardPos);

				m_Mov.now_Xrad = fXRadian;			
			}


			if( m_nTableIndex == MAX_USER_INTABLE )
			{
				// 카메라업데이트
				g_Camera.MatchingViewPoint();
				g_Camera.Update();
			}
		}
		break;
	// 공격하기
	case SM_ATTACK_NPC:
		{	
			// 공격하는 무기가 활일 경우는 화살 생성하기
			// 아이템을 들고 있어야 하고, 무기장착상태이어야 한다
/*			int itm_index = g_Pc_Manager.GetAttackItemIndex( m_bPC, m_nTableIndex );
			if( m_bPC == true && itm_index != -1 && m_pUserInfo->bAttack == true ) 
			{
				switch( g_Pc_Manager.m_pItm[itm_index]->m_class )
				{
				case 1:	// 양손무기
					{
						switch( g_Pc_Manager.m_pItm[itm_index]->m_type )
						{
						case 3:	// 활
							{
								if( SetArrowCreate() == false )
								{
									m_curt_event.Clear();
									m_save_event.Clear();
								}
							}
							break;
						}
					}
					break;
				}				
			}*/
		}
		break;
	
/*	// 화살쏘기
	case SM_ARROW_GOING:
		{
			if( SetArrowCreate() == false )
			{
				m_curt_event.Clear();
				m_save_event.Clear();
			}
		}
		break;
	// 스킬사용
	case SM_USE_SKILL:
		{
			bool bRes = false;
			switch( m_curt_event.motion )
			{
			case ARCHER_SKILL_50:	// 백스텝
				{
					bRes = true;
				}
				break;
			}
			if( bRes == true )
			{
				m_matChr._41 += m_curt_event.vPos.x;
				m_matChr._42 += m_curt_event.vPos.y;
				m_matChr._43 += m_curt_event.vPos.z;
				if( m_nTableIndex == MAX_USER_INTABLE )
				{
					// 카메라업데이트
					g_Camera.MatchingViewPoint();
					g_Camera.Update();

					// 지형업데이트
					if( m_ani_frame%2 == 0 )
						g_Map.UpdateTerrain( (int)(m_matChr._41/8), (int)(m_matChr._43/8) );
				}
			}			
		}
		break; */	
	}	
/*	if( bFind == true )
	{
		if( m_nTableIndex == MAX_USER_INTABLE )
			g_Pc.GetPlayer()->AniProcessEvent();
		else
			g_Pc.GetUser((short)m_nTableIndex)->AniProcessEvent();
		return true;
	} */
	return false;
}

//-----------------------------------------------------------------------------
// Desc: 유저 상태값 리턴
//-----------------------------------------------------------------------------
int	Character::GetStatus()
{
	// PC
	if( true == m_bPC )
	{
		// 상태값 리턴
		if( MAX_USER_INTABLE == m_nTableIndex )
			return g_Pc.m_MainPC.nStatus;
		return g_Pc.m_PcTable[m_nTableIndex].nStatus;
	}
	// NPC
	else
		return g_Map.m_NpcTable[m_nTableIndex].nStatus;	
	return 0;
}


int Character::GetWearItemCode( int aPos )
{
	int item_code = 0;

	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		nRui->GetPcInvenInfo()->WearItem[ aPos ]->Code;
	}
	else
	{				
		if(m_bPC)
		{
			g_Pc.m_PcTable[ m_nTableIndex ].char_info.theWearItem[ aPos ].theCode;
		}
		else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) 
		{
			g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theWearItem[ aPos ];
		}		
	}	

	return item_code;
}


//-----------------------------------------------------------------------------
// Desc : 바운드 박스
//-----------------------------------------------------------------------------	
D3DXVECTOR3		Character::GetBoundMinPos()
{
	return g_Pc_Manager.GetBoundMinPos( this );
}

D3DXVECTOR3		Character::GetBoundMaxPos()
{
	return g_Pc_Manager.GetBoundMaxPos( this );
}

D3DXVECTOR3		Character::GetCBoundMinPos(void)
{
	return g_Pc_Manager.GetCBoundMinPos(this);
}

D3DXVECTOR3		Character::GetCBoundMaxPos(void)
{
	return g_Pc_Manager.GetCBoundMaxPos(this);
}

D3DXVECTOR3		Character::GetBoundMaxVec(void)
{
	return g_Pc_Manager.GetMaxBoundVec(this);
}

D3DXVECTOR3		Character::GetBoundMinVec(void)
{
	return g_Pc_Manager.GetMinBoundVec(this);
}

//-----------------------------------------------------------------------------
// Desc : 아이템인덱스
//-----------------------------------------------------------------------------	
int		Character::GetItmIndex( int _nIndex )
{
	return g_Pc_Manager.GetItmIndex( this, _nIndex );
}

int		Character::GetItmCount()
{
	return g_Pc_Manager.GetItmCount( this );
}

int Character::SM__CHANGE__ITEM( DWORD item_id, int item_unique )
{	
	// 0이면 장착실패
	if( item_id == 0 )	
		return -1;	
	
	// ID 분석해서 어떤 아이템인지 알아내야함
	int result = 0;
	int index  = g_Pc_Manager.GetItemID( item_id );
	if( index == -1 )
	{
		return -1;	
	}

	int mod_index = 0;
	int num = 0;
	
	switch( g_Pc_Manager.m_pItm[index]->m_grav )
	{
	case 0:		// 장착 아이템 
		{			
			//int nItmCount = g_Pc_Manager.GetItmCount( this );
			for( int i = 0; i < c_Max_WearItemType; ++i )
			{
				if( g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_race == g_Pc_Manager.m_pItm[index]->m_race && 
					g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_sex == g_Pc_Manager.m_pItm[index]->m_sex && 
					g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_type  == g_Pc_Manager.m_pItm[index]->m_type )
				{
					// 인덱스 저장
					m_pItm_index[i] = index;

					// 버퍼재할당
					SetItemBuffer( i );	
					break;
				}
			}
		}
		break;
	case 1:	
		{
			// 무기 아이템 
			// 아이템인덱스저장
			if( g_Pc_Manager.m_pItm[index]->m_type == 0 ||
				g_Pc_Manager.m_pItm[index]->m_type == 2 ||
				g_Pc_Manager.m_pItm[index]->m_type == 6 ||
				g_Pc_Manager.m_pItm[index]->m_type == 7 )		// 오른손
				m_pUserInfo->ArmorInfo.index[0] = index;
			else if( g_Pc_Manager.m_pItm[index]->m_type == 1 ||
				g_Pc_Manager.m_pItm[index]->m_type == 3 ||
				g_Pc_Manager.m_pItm[index]->m_type == 5 )	// 왼손
				m_pUserInfo->ArmorInfo.index[1] = index;
			
			// 칼검기(잔상)만들기 위해서 높이값 구하기
			m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[index]->m_AxisTop;
			m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[index]->m_AxisBottom;					
/*			
			// 칼검기(잔상)만들기 위해서 높이값 구하기
			int mod_index = g_Pc_Manager.m_pItm[index]->m_mod_index;
			
			int num;
			
			for( num = 0; num < g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex; ++num )
			{
				if( m_pUserInfo->vSwordTop.z < g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex.z )
					m_pUserInfo->vSwordTop = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex;
				
				if( m_pUserInfo->vSwordBottom.z > g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex.z )					
					m_pUserInfo->vSwordBottom = g_Pc_Manager.m_pMod[mod_index]->m_pVertex[num].m_vertex;
			}
*/			
			m_pUserInfo->bAttack = true;
		}
		break;
	
	// 한손검
/*	case 0:
		{
			// log
			if( m_pUserInfo == NULL )
			{
				#ifdef	GAMETYPE_TEST
					MessageBox( NULL, "SM_CHANGE_ITEM Error", "Error", MB_OK );
				#endif
				return -1;
			}

			// 아이템인덱스저장
			m_pUserInfo->ArmorInfo.index[0] = index;

			// 활
			if( g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]].m_class == 1 )		
				m_pUserInfo->ArmorInfo.index[1] = -1;		

			// 칼검기(잔상)만들기 위해서 높이값 구하기
			int mod_index = g_Pc_Manager.m_pItm[index].m_mod_index;
			for( int num = 0; num < g_Pc_Manager.m_pMod[mod_index].m_header.m_num_vertex; ++num )
			{
				if( m_pUserInfo->AfterImage.vTop.y < g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex.y )
					m_pUserInfo->AfterImage.vTop = g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex;
				if( m_pUserInfo->AfterImage.vBottom.y > g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex.y )					
					m_pUserInfo->AfterImage.vBottom = g_Pc_Manager.m_pMod[mod_index].m_pVertex[num].m_vertex;
			}
		}
		break;
	// 활
	case 1:
		{	
			// log
			if( m_pUserInfo == NULL )
			{
				#ifdef	GAMETYPE_TEST
					MessageBox( NULL, "SM_CHANGE_ITEM Error", "Error", MB_OK );
				#endif
				return -1;
			}

			// 화살통이 아니라면.. 오른손 아이템은 그리지 않느다
			int RHand = m_pUserInfo->ArmorInfo.index[0];
			if( !(g_Pc_Manager.m_pItm[RHand].m_class == 3 && g_Pc_Manager.m_pItm[RHand].m_type == 4) )
				m_pUserInfo->ArmorInfo.index[0] = -1;
			m_pUserInfo->ArmorInfo.index[1] = index;			
		}
		break;
	// 방패, 화살통
	case 3:
		{
			switch( g_Pc_Manager.m_pItm[index].m_type )
			{
			// 화살통
			case 4:				
				m_pUserInfo->ArmorInfo.index[0] = index;				
				break;
			// 요정날개
			case 12:				
//				m_pItm_index[5] = index;				
				break;
			// 방패
			default:				
				m_pUserInfo->ArmorInfo.index[1] = index;				
				break;
			}
		}
		break;	*/
	// 갑옷(몸통,손,발,머리카락)
	default:		
		break;
	}
	
	// 게임중일때만 저장한다
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return -1;

	// 인벤토리에 저장
/*	if( m_nTableIndex == MAX_USER_INTABLE )
	{
		// 장비저장
		for( int i = 0; i < c_Max_Equiped; ++i )
		{
			int curt_item = g_Pc_Manager.GetItemID( g_Pc.m_Equip[i].index );
			int next_item = g_Pc_Manager.GetItemID( item_id );
			if( g_Pc.m_Equip[i].index == 0 )
			{				
				g_Pc.m_Equip[i].index  = item_id;
				g_Pc.m_Equip[i].unique = item_unique;			
				break;
			}
			else if( g_Pc_Manager.m_pItm[curt_item].m_class == g_Pc_Manager.m_pItm[next_item].m_class )
			{				
				g_Pc.m_Equip[i].index  = item_id;
				g_Pc.m_Equip[i].unique = item_unique;
				break;
			}
		}
		if( i != c_Max_Equiped )
		{
			for( int j = 0; j < MAX_INVENTORY1; ++j )
			{				
				if( g_Pc.m_Tool_Inventory1[j].index == item_id)
				{
					int TempLife = g_Pc.m_Tool_Inventory1[j].life;					
					g_Pc.m_Tool_Inventory1[j].life = g_Pc.m_Equip[i].life;
					g_Pc.m_Equip[i].life = TempLife;
					break;
				}
			}			
		}
		else
		{
			// 인벤토리저장
			for( i = 0; i < MAX_INVENTORY1; ++i )
			{
				int curt_item = g_Pc_Manager.GetItemID( g_Pc.m_Tool_Inventory1[i].index  );
				int next_item = g_Pc_Manager.GetItemID( item_id );
				if( g_Pc.m_Tool_Inventory1[i].index == 0 )
				{
					g_Pc.m_Tool_Inventory1[i].index  = item_id;
					g_Pc.m_Tool_Inventory1[i].unique = item_unique;
					break;
				}
				else if( g_Pc_Manager.m_pItm[curt_item].m_class == g_Pc_Manager.m_pItm[next_item].m_class )
				{					
					g_Pc.m_Tool_Inventory1[i].index  = item_id;
					g_Pc.m_Tool_Inventory1[i].unique = item_unique;
					break;
				}
			}
		}
	} */

	// 무기, 방패 장착했다면 애니메이션 변경
	if( m_pUserInfo->ArmorInfo.index[0] != -1 && m_pUserInfo->ArmorInfo.index[1] != -1 )
	{		
		if( m_curt_event.type != SM_MOVE_BROADCAST )
			AnimationPlay( n_Stand_Battle9 );		 // 무기에 따라 각각 설정 해줘야 함.
	}

	//-----------------------------------------------------------------------------
	// 텍스쳐업데이트
	//-----------------------------------------------------------------------------
	UpdateTexture();
	return result;
}

//-----------------------------------------------------------------------------
// Desc: 아이템 벗기
//-----------------------------------------------------------------------------
int Character::SM__TAKEOFF__ITEM( DWORD item_id, int item_unique )
{	
	// 0이면 장착실패
	if( item_id == 0 )
		return -1;

	// ID 분석해서 어떤 아이템인지 알아내야함
	int result = 0;
	int index = g_Pc_Manager.GetItemID( item_id );
	if( item_id == -1 )
	{
		return -1;	
	}

	switch( g_Pc_Manager.m_pItm[index]->m_grav )
	{
	case 0:		// 장착 아이템 
		{			
			for( int i = 0; i < c_Max_WearItemType; ++i )
			{
//				if( n_Wear_Serve == i )
//				{
//					m_pItm_index[i]	= -1;			
//					continue;
//				}

				if( g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_class == g_Pc_Manager.m_pItm[index]->m_class && 
					g_Pc_Manager.m_pItm[m_pItm_index[i]]->m_type  == g_Pc_Manager.m_pItm[index]->m_type )
				{
					if( index == m_pItm_index[i] )
					{
						int attr_index = GetAttrIndex();

						// 갑옷						
/*						if( g_Pc_Manager.m_pItm[index].m_class == 2 )							
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].body;
						// 투구
						else if( g_Pc_Manager.m_pItm[index].m_class == 4 )
						{
							if( attr_index != UNION_FAIRY )
							{
								
//								if( m_nTableIndex == MAX_USER_INTABLE )	
//									m_pItm_index[i] = g_EtcTable.GetHairIndex( GetAttrIndex(), g_Pc.m_MainPC.char_info.hair );
//								else
//									m_pItm_index[i] = g_EtcTable.GetHairIndex( GetAttrIndex(), g_Pc.m_PcTable[m_nTableIndex].char_info.hair );
									
							}
							else
							{
								m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hair;
							}
						}
						// 신발
						else if( g_Pc_Manager.m_pItm[index].m_class == 5 )
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].foot;
						// 손
						else if( g_Pc_Manager.m_pItm[index].m_class == 6 )
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hand; */

						if( g_Pc_Manager.m_pItm[index]->m_type == 0 )					// 상반신 
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].body; 
						else if( g_Pc_Manager.m_pItm[index]->m_type == 1 )				// 하반신 
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].pants; 
						else if( g_Pc_Manager.m_pItm[index]->m_type == 2 )				// 장갑
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hand; 
						else if( g_Pc_Manager.m_pItm[index]->m_type == 3 )				// 부츠
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].foot; 
						else if( g_Pc_Manager.m_pItm[index]->m_type == 7 )				// 얼굴
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].face; 
						else if( g_Pc_Manager.m_pItm[index]->m_type == 8 )				// 헤어
							m_pItm_index[i] = g_Pc_Manager.m_PcAttr[attr_index].hair; 
											
						// 버퍼 재할당					
						SetItemBuffer( i );			
						break;
					}
				}
			}
		}
		break;
	case 1:		// 무기 아이템 
		// 아이템인덱스저장
		if( g_Pc_Manager.m_pItm[index]->m_type == 0 ||
			g_Pc_Manager.m_pItm[index]->m_type == 2 ||
			g_Pc_Manager.m_pItm[index]->m_type == 6 ||
			g_Pc_Manager.m_pItm[index]->m_type == 7 )		// 오른손
			m_pUserInfo->ArmorInfo.index[0] = -1;
		else if( g_Pc_Manager.m_pItm[index]->m_type == 1 ||
				 g_Pc_Manager.m_pItm[index]->m_type == 3 ||
				 g_Pc_Manager.m_pItm[index]->m_type == 5 )	// 왼손
			m_pUserInfo->ArmorInfo.index[1] = -1;
		break;
	default:
		break;
	}
	//-----------------------------------------------------------------------------
	// 아무것도 장착하지 않고 있다면 기본 서있기로 애니메이션 변경
	//-----------------------------------------------------------------------------
	if( m_pUserInfo->ArmorInfo.index[0] == -1 && m_pUserInfo->ArmorInfo.index[1] == -1 )	
	{		
		m_pUserInfo->bAttack = false;
		if( m_curt_event.type != SM_MOVE_BROADCAST )
			AnimationPlay( n_Stand_Peace9 );	
	}	
	
	// 텍스쳐업데이트	
	UpdateTexture();
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////
/// blendQuaternion( D3DXQUATERNION* blend, float factor, D3DXQUATERNION src, D3DXQUATERNION dest )
/// desc : This function is quaternion blend for motion blending.
///		   Interpolates the quaternion...by factor...
/// param : factor - blending factor in range [0.0f ~ 1.0f] [src ~ dest]
///////////////////////////////////////////////////////////////////////////////////////////
void Character::blendQuaternion( D3DXQUATERNION* blend, float factor, D3DXQUATERNION src, D3DXQUATERNION dest )
{
	float norm;
	norm = src.x * dest.x + src.y * dest.y + src.z * dest.z + src.w * dest.w;
	
	bool bFlip;
	bFlip = false;
	
	if(norm < 0.0f)
	{
		norm = -norm;
		bFlip = true;
	}
	
	float inv_factor;
	if(1.0f - norm < 0.000001f)
	{
		inv_factor = 1.0f - factor;
	}
	else
	{
		float theta;
		theta = (float) acos(norm);
		
		float s;
		s = (float) (1.0f / sin(theta));
		
		inv_factor = (float) sin((1.0f - factor) * theta) * s;
		factor = (float) sin(factor * theta) * s;
	}
	
	if(bFlip)
	{
		factor = -factor;
	}
	
	blend->x = inv_factor * src.x + factor * dest.x;
	blend->y = inv_factor * src.y + factor * dest.y;
	blend->z = inv_factor * src.z + factor * dest.z;
	blend->w = inv_factor * src.w + factor * dest.w;

	return;
}



bool Character::ReadyUnderAttackMotion( BOOL IsCritiCal )
{
	if( m_curt_event.type == SM_MOVE_BROADCAST || 
		m_curt_event.type == SM_ATTACK_NPC || 
		m_curt_event.type == SM_UNDERATTACK || 
		m_curt_event.type == SM_STATE_DIE || 
		m_curt_event.type == SM_SLIDE_NPC ||
		m_curt_event.type == SM_USE_SKILL ||
		m_curt_event.type == SM_BOW_ATTACK 		)
		return false;
	
	///-- 현재 플레이어이며 콤보중이면 맞는동작을 발생시키지 않는다. 
	if( this == g_Pc.GetPlayer() &&
		g_Pc.GetPlayer()->theActionSystem.GetComboCnt() )
	{
		return false;
	}
	
	if( !IsCritiCal )
	{
		if( !m_bPC && m_State.nCharacterType == nCharacterTypeBigMoster )
		{
			return false;
		}
	}

	return true;
}


DWORD Character::SetUnderAttackMotion( DWORD aDamage )
{
	DWORD motionIdx = 0;
//	DWORD theHP = 100.0f;
//	float DamageLevel = (float)aDamage / nRui->thePcParam.Stat1.theHP;
	float DamageLevel = (float)aDamage / nRui->thePcParam.MaxHP;
//	float DamageLevel = (float)aDamage / 1000;

	if( m_bPC || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		if( DamageLevel > 0.3f )
		{
			motionIdx = n_ReAction_StrongDmg9;
		}
		else
		{
			motionIdx = n_ReAction_LightDmg9;
		}
	}
	else
	{
		if( DamageLevel > 0.3f )
		{
			motionIdx = n_NPC_ReAction1;
		}
		else
		{
			motionIdx = n_NPC_ReAction0;
		}
	}
	return motionIdx; 
}


//-----------------------------------------------------------------------------
// Desc: 밀려나기
//-----------------------------------------------------------------------------
bool Character::SM__SLIDE__NPC( EVENT_DATA_INFO& data )
{
	// 이벤트초기화
	m_curt_event.Clear();
	m_save_event.Clear();

	// 슬라이드이벤트 저장
	m_curt_event = data;

	int curt_x = 0;
	int curt_y = 0;
	int dest_x = 0;
	int dest_y = 0;
	float height = 0.0f;

	// 밀려가는 거리
	if( m_bPC )
	{
		curt_x	= g_Pc.m_PcTable[m_nTableIndex].lDestBlock%(g_Pc.m_cSize);
		curt_y	= g_Pc.m_PcTable[m_nTableIndex].lDestBlock/(g_Pc.m_cSize);
		height = g_Pc.m_Pc[m_nTableIndex].m_Mov.curt.height;

		g_Map.SetMapData( CLEAR_BLOCK, m_nTableIndex, g_Pc.m_PcTable[m_nTableIndex].lDestBlock, true );		
		g_Map.SetMapData( PC_BLOCK, m_nTableIndex, m_curt_event.cell, true );

		g_Pc.m_PcTable[m_nTableIndex].lDestBlock  = m_curt_event.cell;
		g_Pc.m_PcTable[m_nTableIndex].lBlock		= m_curt_event.cell;
	}
	else
	{
		curt_x	= g_Map.m_NpcTable[m_nTableIndex].lDestBlock%(g_Pc.m_cSize);
		curt_y	= g_Map.m_NpcTable[m_nTableIndex].lDestBlock/(g_Pc.m_cSize);
		height = g_Map.m_Npc[m_nTableIndex].m_Mov.curt.height;

		g_Map.SetMapData( CLEAR_BLOCK, m_nTableIndex, g_Map.m_NpcTable[m_nTableIndex].lDestBlock, false );		
		g_Map.SetMapData( NPC_BLOCK, m_nTableIndex, m_curt_event.cell );
		
		g_Map.m_NpcTable[m_nTableIndex].lDestBlock  = m_curt_event.cell;
		g_Map.m_NpcTable[m_nTableIndex].lBlock		= m_curt_event.cell;
	}
	dest_x	= m_curt_event.cell %(g_Pc.m_cSize);
	dest_y	= m_curt_event.cell /(g_Pc.m_cSize);

//	RLG4( "curtX = %d, curtY = %d, destX = %d, destY = %d", curt_x, curt_y, dest_x, dest_y );	
	
	m_curt_event.x		= dest_x;
	m_curt_event.y		= dest_y;
	m_curt_event.vPos.x	= (dest_x - curt_x) * CLIENT_TILESIZE;
	m_curt_event.vPos.y	= g_Pc.GetMapHeight( dest_x, dest_y ) - height;
	m_curt_event.vPos.z	= (dest_y - curt_y) * CLIENT_TILESIZE;
		
	// 위치값 초기화
//	m_Mov.Init( m_curt_event.x, m_curt_event.y );	
		
	// 이동거리를 애니프레임으로 나누기
	int frame = 0;

	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		frame  = g_Pc_Manager.GetEndFrame( this, n_ReAction_Critical9 );
		AnimationPlay( n_ReAction_Critical9 );
	}
	else
	{
		frame  = g_Pc_Manager.GetEndFrame( this, n_NPC_ReAction2 );
		AnimationPlay( n_NPC_ReAction2 );
	}

	m_curt_event.vPos.x = m_curt_event.vPos.x / frame;
	m_curt_event.vPos.y = m_curt_event.vPos.y / frame;
	m_curt_event.vPos.z = m_curt_event.vPos.z / frame;
	
	return true;
}

//................................................................................................................
// 프레임 체크 ( 이펙트 처리 ) By wxywxy 
//................................................................................................................
int Character::EffectCheck(void)
{
	CEffectBase * pData = NULL;
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();

	for( ; i != m_EffectList.end() ; ++i )
	{
		pData = (CEffectBase *)(*i);
		
		if( pData )
		{
			pData->AddSrcAniFrameCount();
			pData->CheckFrame();
		}
	}

	return TRUE;
}

//................................................................................................................
// 소트된 캐릭터 찍을때 이펙트 찍기 By wxywxy 
//................................................................................................................
int Character::EffectSortRender(void)
{
	///--JAPAN_BUG_MODIFY
	DWORD dFogEnable = g_RenderManager.GetFogEnable();
	g_RenderManager.SetFogEnable( FALSE );
	
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();

	for( ; i != m_EffectList.end() ; ++i )
	{
		if( !((*i)->m_IsAfterSortRender) )
		{
			(*i)->SortRender(g_Particle.m_fTimeD);
		}
	}

	i = m_EffectList.begin();

	for( ; i != m_EffectList.end() ; ++i )
	{
		if( ((*i)->m_IsAfterSortRender) )
		{
			(*i)->SortRender(g_Particle.m_fTimeD);
		}
	}

	///--JAPAN_BUG_MODIFY
	g_RenderManager.SetFogEnable( dFogEnable );	

	return TRUE;
}


//................................................................................................................
// 이펙트 추가 ( 이펙트 처리 ) By wxywxy 
//................................................................................................................
int Character::PushEffectBase(CEffectBase **ppEffectBase)
{
	m_EffectList.push_back( *ppEffectBase );

	return TRUE;
}

//................................................................................................................
// 이펙트 삭제 ( 이펙트 처리 ) By wxywxy 
//................................................................................................................
int Character::PopEffectBase(CEffectBase **ppEffectBase)
{
	std::list	<CEffectBase *> ::iterator	i = m_EffectList.begin();

	for( ; i != m_EffectList.end() ; )
	{
		if( (*i) == *ppEffectBase )
		{
			m_EffectList.erase( i++ );
			
			return TRUE;
		}
		else
		{
			i++;
		}
	}

	return FALSE;
}

//................................................................................................................
// 칼축 처리 ( By wxywxy )
//................................................................................................................
void Character::CalWeaponAxis(void)
{
	if( m_pUserInfo->ArmorInfo.index[0] != -1 )
	{
		Float3MatrixMultiply( m_pUserInfo->vSwordAxis[ 0 ].x ,
			m_pUserInfo->vSwordAxis[ 0 ].y ,
			m_pUserInfo->vSwordAxis[ 0 ].z ,	
			m_pUserInfo->vSwordTop, m_matRHand );
		
		Float3MatrixMultiply( m_pUserInfo->vSwordAxis[ 1 ].x ,
			m_pUserInfo->vSwordAxis[ 1 ].y ,
			m_pUserInfo->vSwordAxis[ 1 ].z ,	
			m_pUserInfo->vSwordBottom, m_matRHand );
	}
	else
	{
		Float3MatrixMultiply( m_pUserInfo->vSwordAxis[ 0 ].x ,
			m_pUserInfo->vSwordAxis[ 0 ].y ,
			m_pUserInfo->vSwordAxis[ 0 ].z ,	
			m_pUserInfo->vSwordTop, m_matLHand );
		
		Float3MatrixMultiply( m_pUserInfo->vSwordAxis[ 1 ].x ,
			m_pUserInfo->vSwordAxis[ 1 ].y ,
			m_pUserInfo->vSwordAxis[ 1 ].z ,	
			m_pUserInfo->vSwordBottom, m_matLHand );
	}
	
/*	
	D3DXVECTOR3	vDir = m_pUserInfo->vSwordAxis[ 1 ] - m_pUserInfo->vSwordAxis[ 0 ];
	
	D3DXVec3Normalize( &vDir , &vDir );
	
	m_pUserInfo->vSwordAxis[ 0 ] += vDir * 5.5f;										// 칼 손잡이 빼주기
*/
  
}


//-----------------------------------------------------------------------------
// 아이템 장착 
//-----------------------------------------------------------------------------
int Character::Item_Equip( DWORD aItemCode, int aEquipPos )
{	
	SItemBaseInfo* item_Info = NULL;	
	int chrAttrIndex = GetAttrIndex();
	bool IsEquipWeapon = false;
	//스킨을 우선순위로 한다 . 

	//스킨이 우선순위가 있어서 스킨이 관련된 아이템은 리턴시킨다. 
	if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{
		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Suit])
		{
			switch(aEquipPos)
			{
			case n_WearSlot_CoatArmor:
			case n_WearSlot_PantsArmor:
			case n_WearSlot_Gloves:
			case n_WearSlot_Shoes:
				{
					return TRUE;
				}				
			}
		}	

		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Helmet ] && 	( aEquipPos == n_WearSlot_Helmet ) )
		{
				return TRUE;
		}

		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Serve ] &&  (aEquipPos == n_WearSlot_Serve) )
		{
				return TRUE;
		}
	}
	

	if( aEquipPos == n_WearSlot_SKIN_Suit)
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_CoatArmor );
		///-- 셋트 아이템 웨어처리
		m_Char_WearType = CHAR_WEAR_SETITEM;
		//방어구 이펙트 .. 
		g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
	}
	else if(aEquipPos == n_WearSlot_SKIN_Serve )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		SetItemBuffer( n_Wear_Serve );	
		
		//기존에 날개를 해제 한다. 
		if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			g_Particle.m_EffectManager.DeleteItemLinkEffect( n_WearSlot_Serve , this );
		}
	}
	else if( aEquipPos == n_WearSlot_SKIN_Helmet )
	{
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_Hair );	
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
		if( item_Info && item_Info->theType == n_Type_Skin_KeepypHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;	
			
			if( m_Basic_Itm_index[n_Wear_Hair] == 0 )
			{
				if( m_nTableIndex == MAX_USER_INTABLE )
				{
					m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
					SetBasicItemBuffer( n_Wear_Hair );
				}
				else
				{
					m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theHair ); 
					SetBasicItemBuffer( n_Wear_Hair );
				}
			}
		}
		else
		{
			m_Basic_DrawItm[n_Wear_Hair] = 0;
			m_Basic_Itm_index[n_Wear_Hair] = 0;
		}		
	}
	
	if( aEquipPos == n_WearSlot_CoatArmor )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_CoatArmor );
		///-- 셋트 아이템 웨어처리
		SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
		if( m_bPC && pSItemBaseInfo && (n_Type_SetCloth == pSItemBaseInfo->theType ||n_Type_Skin_Suit == pSItemBaseInfo->theType))
		{
			m_Char_WearType = CHAR_WEAR_SETITEM;
			//방어구 이펙트 .. 
			g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
		}
		else
		{
			m_Char_WearType = CHAR_WEAR_NORMAL;	
		}
	}
	else if( aEquipPos == n_WearSlot_PantsArmor )
	{
		m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_PantsArmor );	
	}
	else if( aEquipPos == n_WearSlot_Gloves )
	{
		m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_Gloves );	
	}
	else if( aEquipPos == n_WearSlot_Shoes )
	{
		m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_Shoes );	
	}
	else if( aEquipPos == n_WearSlot_Helmet )
	{
		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		SetItemBuffer( n_Wear_Hair );	
		
		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
		if( item_Info && (item_Info->theType == n_Type_KeepupHair ||item_Info->theType == n_Type_Skin_KeepypHair) )
		
		{
			m_Basic_DrawItm[n_Wear_Hair] = 1;	
			
			if( m_Basic_Itm_index[n_Wear_Hair] == 0 )
			{
				if( m_nTableIndex == MAX_USER_INTABLE )
				{
					m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
					SetBasicItemBuffer( n_Wear_Hair );
				}
				else
				{
					m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theHair ); 
					SetBasicItemBuffer( n_Wear_Hair );
				}
			}
		}
		else
		{
			m_Basic_DrawItm[n_Wear_Hair] = 0;
			m_Basic_Itm_index[n_Wear_Hair] = 0;
		}		
	}
	else if( aEquipPos == n_WearSlot_Serve )
	{
		m_pItm_index[n_Wear_Serve] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode ); // 아이템 코드에 맞는 텍스쳐 번호 로딩  
		SetItemBuffer( n_Wear_Serve );			
	}
	else if( aEquipPos == n_WearSlot_Shield )
	{
		IsEquipWeapon = true;
		
		m_pUserInfo->ArmorInfo.index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		m_pUserInfo->ArmorInfo.Code[1] = aItemCode;
		m_pUserInfo->bAttack = true;

		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
		if( item_Info && item_Info->theType != n_Type_Shield ) 
		{
//			m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);					

			if( m_nTableIndex == MAX_USER_INTABLE )
			{
				g_Pc.GetPlayer()->m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
			}
		}

		if( n_Type_Bow == item_Info->theType || n_Type_MagicBook == item_Info->theType )
		{
			// 칼검기(잔상)만들기 위해서 높이값 구하기
			m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisTop;
			m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[1]]->m_AxisBottom;					
			g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );

	
		}
	
	}
	else if( aEquipPos == n_WearSlot_Weapon )
	{
		IsEquipWeapon = true;
		
		m_pUserInfo->ArmorInfo.index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
		m_pUserInfo->ArmorInfo.Code[0] = aItemCode;

		// 칼검기(잔상)만들기 위해서 높이값 구하기
		m_pUserInfo->vSwordTop     = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisTop;
		m_pUserInfo->vSwordBottom  = g_Pc_Manager.m_pItm[m_pUserInfo->ArmorInfo.index[0]]->m_AxisBottom;					
		
		m_pUserInfo->bAttack = true;

//		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );

//		if( item_Info )
//		{
//			m_Weapon_DelayTime = c_WeaponDelayTime - (item_Info->theAttackSpeed * 100);		
//		}

		if( m_nTableIndex == MAX_USER_INTABLE )
		{
			g_Pc.GetPlayer()->m_Weapon_DelayTime = nRui->GetPcParamInfo()->Stat2.theAttackSpeed * 100;		
		}

		g_Particle.m_EffectManager.CreateWeaponEnChantEffect( this , 255 );

	

		//.....................................................................................................
		// 근접 무기 체크 By wxywxy
		//.....................................................................................................
		m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
		m_AttackMotion	   = 0;				// 아이템 공격모션 번호

		item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );

		if( NULL != item_Info )
		{
			if( item_Info->theType == n_Type_ShortWeapon )
			{
				m_ISApproachWeapon = TRUE;
				m_AttackMotion	   = item_Info->theAttackMotion;
			}
		}
		//.....................................................................................................
	}

#ifdef RIDING_SYSTEM
	if( m_State.bRide )
	{
		if(aEquipPos == n_WearSlot_RidingItem1)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 0 );
		}
		else if(aEquipPos == n_WearSlot_RidingItem2)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 1 );
		}
		else if(aEquipPos == n_WearSlot_RidingItem3)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
			g_Map.m_Npc[m_State.TransIndex].SetRideItemBuffer( 2 );
		}	
	}
#endif
	
	//.........................................................................................................
	// 아이템 기본 링크 이펙트 By wxywxy
	//.........................................................................................................
	g_Particle.m_EffectManager.CreateItemLinkEffect( aItemCode , this );
	//.........................................................................................................
	if( m_bPC && IsEquipWeapon && !m_State.bRide )
	{
		//by dong 명상시 아이템 변경후 서있는 모션으로 되있었어서 ..  자리에 앉는것 .서는 것.. 처리 .. 
		if(	m_curt_event.status != SM_RESTSKILL_SITDOWN && 
			m_curt_event.status != SM_RESTSKILL_KEEPING && 
			m_curt_event.status !=SM_RESTSKILL_STANDUP )
		{
			///-- by simwoosung
			///-- 아이템을 교체할시에... 해당 아이템이 연결된 노드가 애니메이션에 따라 
			///-- 틀리게 설정되어있다. 바로 장비를 장착한 애니로 전환 
			///-- 블렌딩을 피하고 아이템을 렌더링한다. 
			AnimationPlay( n_Stand_Peace9 );
			m_ani_frame = c_MotionBlendHalfFrame;
			m_blend_frame = c_MotionBlendFrame;				
		}
	
	}
	
	PreCharTextureCreate();

	return 1;
}


//-----------------------------------------------------------------------------
// 아이템 벗기
//-----------------------------------------------------------------------------
int Character::Item_TakeOff( int aEquipPos )
{	
	SItemBaseInfo* item_Info = NULL;	
	int chrAttrIndex = GetAttrIndex();

	//스킨이 우선순위가 있어서 스킨이 관련된 아이템은 리턴시킨다. 
	if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
	{
		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Suit] && aEquipPos == n_WearSlot_CoatArmor )
		{
			return TRUE;
		}	
		
		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Helmet ] && 	( aEquipPos == n_WearSlot_Helmet ) )
		{
			return TRUE;
		}

		if( nRui->thePcInven.WearItem[ n_WearSlot_SKIN_Serve ] &&  (aEquipPos == n_WearSlot_Serve) )
		{
			return TRUE;
		}
	}

	if( aEquipPos == n_WearSlot_SKIN_Suit)
	{
		//슈트를 벗으면 인반 코트아머를 확인을 한다. 
		
		if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			//코트확인하여 
			if( nRui->thePcInven.WearItem[ n_WearSlot_CoatArmor] )
			{
				int aItemCode = nRui->thePcInven.WearItem[ n_WearSlot_CoatArmor]->Code;
				m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
				SetItemBuffer( n_Wear_CoatArmor );
				///-- 셋트 아이템 웨어처리
				SItemBaseInfo* pSItemBaseInfo = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
				if( m_bPC && pSItemBaseInfo &&  n_Type_SetCloth == pSItemBaseInfo->theType)
				{
					m_Char_WearType = CHAR_WEAR_SETITEM;
					//방어구 이펙트 .. 
					g_Particle.m_EffectManager.CreateDefenceEnChantEffect(this , 255 );
				}
				else
				{
					m_Char_WearType = CHAR_WEAR_NORMAL;	
				}
			}
			else
			{
				m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.m_PcAttr[chrAttrIndex].body;
				SetItemBuffer( n_Wear_CoatArmor );
				m_Char_WearType = CHAR_WEAR_NORMAL;
				g_Particle.m_EffectManager.DeleteDefenceEnChantEffect(this);
				
			}
		}
	
	}
	else if( aEquipPos == n_WearSlot_SKIN_Serve)
	{
		/*
		if( GetUnique() == g_Pc.GetPlayer()->GetUnique())
		{
			if( nRui->thePcInven.WearItem[ n_WearSlot_Serve])
			{
				g_Particle.m_EffectManager.CreateItemLinkEffect( nRui->thePcInven.WearItem[ n_WearSlot_Serve]->Code , this );
			}
		}
		*/
	}
	else if( aEquipPos == n_WearSlot_SKIN_Helmet )
	{
		if( m_nTableIndex == MAX_USER_INTABLE )
		{
			if( nRui->thePcInven.WearItem[ n_WearSlot_Helmet ] ) 
			{
				int aItemCode = nRui->thePcInven.WearItem[ n_WearSlot_Helmet ]->Code;

				m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, aItemCode );
				SetItemBuffer( n_Wear_Hair );	
				
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( aItemCode );
				if( item_Info && item_Info->theType == n_Type_KeepupHair )	// 헤어도 동시에 그려줘야 된다 (악세사리 형)
				{
					m_Basic_DrawItm[n_Wear_Hair] = 1;	
					
					if( m_Basic_Itm_index[n_Wear_Hair] == 0 )
					{
						if( m_nTableIndex == MAX_USER_INTABLE )
						{
							m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair ); 
							SetBasicItemBuffer( n_Wear_Hair );
						}
						else
						{
							m_Basic_Itm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex].char_info.theHair ); 
							SetBasicItemBuffer( n_Wear_Hair );
						}
					}
				}
				else
				{
					m_Basic_DrawItm[n_Wear_Hair] = 0;
					m_Basic_Itm_index[n_Wear_Hair] = 0;
				}		
			}	
			else
			{
				m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair); 
				SetItemBuffer( n_Wear_Hair );	
				m_Basic_DrawItm[n_Wear_Hair] = 0;
				m_Basic_Itm_index[n_Wear_Hair] = 0;

			}
		
		}
		/*
		else
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex ].char_info.theHair );
			SetItemBuffer( n_Wear_Hair );	
		
			m_Basic_DrawItm[n_Wear_Hair] = 0;
			m_Basic_Itm_index[n_Wear_Hair] = 0;
		}
		*/
	
	}

	if( aEquipPos == n_WearSlot_CoatArmor )
	{
		m_pItm_index[n_Wear_CoatArmor] = g_Pc_Manager.m_PcAttr[chrAttrIndex].body;
		SetItemBuffer( n_Wear_CoatArmor );
		m_Char_WearType = CHAR_WEAR_NORMAL;
		g_Particle.m_EffectManager.DeleteDefenceEnChantEffect(this);
	}
	else if( aEquipPos == n_WearSlot_PantsArmor )
	{
		m_pItm_index[n_Wear_PantsArmor] = g_Pc_Manager.m_PcAttr[chrAttrIndex].pants;
		SetItemBuffer( n_Wear_PantsArmor );	
	}
	else if( aEquipPos == n_WearSlot_Gloves )
	{
		m_pItm_index[n_Wear_Gloves] = g_Pc_Manager.m_PcAttr[chrAttrIndex].hand;
		SetItemBuffer( n_Wear_Gloves );	
	}
	else if( aEquipPos == n_WearSlot_Shoes )
	{
		m_pItm_index[n_Wear_Shoes] = g_Pc_Manager.m_PcAttr[chrAttrIndex].foot;
		SetItemBuffer( n_Wear_Shoes );	
	}
	else if( aEquipPos == n_WearSlot_Helmet )
	{
//		m_pItm_index[n_Wear_Hair] = g_Pc_Manager.m_PcAttr[chrAttrIndex].hair;
		if( m_nTableIndex == MAX_USER_INTABLE )
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_MainPC.char_info.theHair); 
		}
		else
		{
			m_pItm_index[n_Wear_Hair] = g_Pc_Manager.GetItemInfo_FileCode12( chrAttrIndex, g_Pc.m_PcTable[m_nTableIndex ].char_info.theHair );
		}
		
		SetItemBuffer( n_Wear_Hair );	

		m_Basic_DrawItm[n_Wear_Hair] = 0;
		m_Basic_Itm_index[n_Wear_Hair] = 0;
	}
	else if( aEquipPos == n_WearSlot_Serve )	// 망토 처리...
	{
		m_pItm_index[n_Wear_Serve] = -1;
	}
	else if( aEquipPos == n_WearSlot_Shield )
	{
		m_pUserInfo->ArmorInfo.index[1] = -1;
		m_pUserInfo->ArmorInfo.Code[1] = -1;

		if( m_pUserInfo->ArmorInfo.index[0] == -1 )
		{
			m_Weapon_DelayTime = c_WeaponDelayTime;
		}

		if( GetUnique() == g_Pc.GetPlayer()->GetUnique() )
		{
			if( nRui->thePcInven.WearItem[ n_WearSlot_Shield ] )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[ n_WearSlot_Shield ]->Code );
			}
		}
		else
		{
			if( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[ n_WearSlot_Shield ].theCode )
			{
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( g_Pc.m_PcTable[m_nTableIndex].char_info.theWearItem[ n_WearSlot_Shield ].theCode );
			}
		}

		if( item_Info )
		{
			if( n_Type_Bow == item_Info->theType || n_Type_MagicBook == item_Info->theType ||  n_Type_Shield == item_Info->theType )
			{
				g_Particle.m_EffectManager.DeleteWeaponEnChantEffect( this );
			}
		}
	}
	else if( aEquipPos == n_WearSlot_Weapon )
	{
		m_ISApproachWeapon = FALSE;			// 근접 무기 유/무
		m_AttackMotion	   = 0;				// 아이템 공격모션 번호

		g_Particle.m_EffectManager.DeleteWeaponEnChantEffect( this );

		m_pUserInfo->ArmorInfo.index[0] = -1;
		m_pUserInfo->ArmorInfo.Code[0] = -1;
		
		if( m_pUserInfo->ArmorInfo.index[1] == -1 )
		{
			m_Weapon_DelayTime = c_WeaponDelayTime;		
		}
/*		else
		{
			// 원래는 있으면 안되는 건데...ㅡㅡ; 일단 죽지는 말아야쥐...ㅋ
			if( m_pItm_index[n_WearSlot_Shield]->Code )
			{
				SItemBaseInfo* item_Info = NULL;
				item_Info = g_Pc_Manager.GetItemBaseInfoPtr( nRui->thePcInven.WearItem[n_WearSlot_Shield]->Code );

				if( item_Info->theType == n_Type_Shield ) 
				{
					m_Weapon_DelayTime = c_WeaponDelayTime;		
				}
			}
		} 
*/
	}

#ifdef RIDING_SYSTEM
	if( m_State.bRide )
	{
		if(aEquipPos == n_WearSlot_RidingItem1)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[0] = -1;			
		}
		else if(aEquipPos == n_WearSlot_RidingItem2)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[1] = -1;
		}
		else if(aEquipPos == n_WearSlot_RidingItem3)
		{
			g_Map.m_Npc[m_State.TransIndex].m_Ride_Item_index[2] = -1;
		}	
	}
#endif
	//.........................................................................................................
	// 아이템 기본 링크 이펙트 By wxywxy
	//.........................................................................................................
	g_Particle.m_EffectManager.DeleteItemLinkEffect( aEquipPos , this );
	//.........................................................................................................

	PreCharTextureCreate();
	
	return 1;
}


void Character::PreCharTextureCreate()
{
	// 캐릭터가 입고 있는 텍스쳐는 모두 생성해 놓는다 (맵이동후 첫 화면 및 charimage 등)	
	// 헤어, 얼굴, 상의, 하의, 장갑, 신발, 무기
	int tex_idx = 0;
	int itm_idx = 0;
	int itm_cnt = 0;
	int idx = 0;

	BYTE byType = (BYTE)m_bPC;
	if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
	{
		byType = (BYTE)true;
	}	
	
	if( m_bPC == true || ( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )
	{
		itm_cnt = c_Max_WearItemType;		
		
		for( idx = 0; idx < itm_cnt; ++idx )
		{		
			tex_idx = g_Pc_Manager.GetBasicTexIndex( this, idx );
			if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->m_bIsLoaded == false )
			{
				g_Pc_Manager.m_pTex[tex_idx]->UpdateTexture( byType, tex_idx );				
			}
			
			tex_idx = g_Pc_Manager.GetTexIndex( this, idx );		
			if(  tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->m_bIsLoaded == false )
			{
				g_Pc_Manager.m_pTex[tex_idx]->UpdateTexture( byType, tex_idx );				
			}
		}
		
		if( m_pUserInfo == NULL )
		{
			return;
		}
		
		for( idx = 0; idx < LINK_ITEM_COUNT; ++idx )
		{
			itm_idx = m_pUserInfo->ArmorInfo.index[idx];
			if( itm_idx != -1 )
			{
				tex_idx = g_Pc_Manager.m_pItm[itm_idx]->m_tex_index;	
				if( tex_idx != -1 && g_Pc_Manager.m_pTex[tex_idx]->m_bIsLoaded == false )
				{
					g_Pc_Manager.m_pTex[tex_idx]->UpdateTexture( byType, tex_idx );				
				}
			}
		}		
	}
	
	return;
}

bool Character::DrawWeaponCheckStatus(bool bActCheck)
{
	if( m_State.bRide )
	{
		return false;
	}
	
	if( this == g_Pc.GetPlayer() )
	{
		if( !g_RockClient.m_SceneManager.GetPlayerWeaponDisplay() )
			return false;
	}
	else if( !g_RockClient.m_SceneManager.GetOtherPlayerWeaponDisplay() )
	{
		return false;
	}

	if( m_curt_event.type == SM_ACT )
	{
		if(bActCheck)
		{
		    return false;
		}
	}
	else if( m_curt_event.status == SM_SELLSKILL_SITDOWN ||
			 m_curt_event.status == SM_SELLSKILL_KEEPING ||  
			 m_curt_event.status == SM_SELLSKILL_STANDUP   )
	{
		return false;
	}
	else if( m_curt_event.status == SM_RESTSKILL_SITDOWN ||
		 m_curt_event.status == SM_RESTSKILL_KEEPING ||
		 m_curt_event.status == SM_RESTSKILL_STANDUP )
	{
		return false;
	}
	else if( m_curt_event.type == SM_ATTACK_NPC || 			 
			 m_curt_event.type == SM_USE_SKILL || 
			 m_curt_event.type == SM_ATTACK_DELAY ||
			 m_curt_event.type == SM_UNDERATTACK )
//			 (m_curt_event.type == SM_USE_SKILL && m_curt_event.status == SM_USE_SKILL) )
	{
		// 현재 무기에 따라 결정 
		int RHand_index = m_pUserInfo->ArmorInfo.index[0];		
		int LHand_index = m_pUserInfo->ArmorInfo.index[1];

		if( LHand_index != -1 )   
		{
			if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 )  // 마법서
			{
				return false;
			}
		}
		
		if( RHand_index != -1 ) 	
		{
			if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 )  // 수정구
			{
				return false;
			}			
		}	
	}
	else if( m_ani_index == n_Stand_Battle5 ||	// 마법서 공격 대기 동작일때...마법서를 그리지 않는다 
			 m_ani_index == n_Stand_Battle7 ) 
	{
		return false;
	}

	return true;
}


void Character::SetVibrationPos( D3DMATRIX* aMatWorld )
{
	if( !theIsVibration )
	{
		return;
	}
	
	int vibrationPosX, vibrationPosY, vibrationPosZ;
	vibrationPosX = RandomNum( -2.5f, 2.5f );
	vibrationPosY = RandomNum( -1.5f, 1.5f );
	vibrationPosZ = RandomNum( -2.5f, 2.5f );

	aMatWorld->_41 += vibrationPosX;
	aMatWorld->_42 += vibrationPosY;
	aMatWorld->_43 += vibrationPosZ;	

	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( theVibrationTime , 130 ) ) )
	{
		theIsVibration = false;
		theVibrationTime = g_nowTime;
	}

	return;
}

bool Character::DrawWeaponFade()
{
	return true;

	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( theDrawWeaponFadeTime , 150 ) ) )
	{
		return false;
	}

	theIsDrawWeapon = true;

#ifdef DIRECT_VERSION_9_MJH
	D3DMATERIAL9 material;
#else
	D3DMATERIAL8 material;
#endif // DIRECT_VERSION_9_MJH
	g_RenderManager.SetMaterial( &material );	

	return true;
}



int Character::GetWeaponType(void)
{
	// NPC 이면 실패
	if( !m_bPC && m_State.nCharacterType != nCharacterTypePlayerNPC)
	{
		return -1;
	}
	
	// 현재 무기에 따라 결정 
	int RHand_index = m_pUserInfo->ArmorInfo.index[0];
	int LHand_index = m_pUserInfo->ArmorInfo.index[1];

	if( RHand_index != -1 && LHand_index == -1 )				// 오른손 무기만 장착
	{		
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 0 )		// 대검 
		{
			return 0; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 ) // 한손검 
		{
			return 2; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 6 ) // 지팡이 
		{
			return 6; 
		}
		else if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 7 ) // 흑수정
		{
			return 7; 
		}
	}
	else if( RHand_index == -1 && LHand_index != -1 )			// 왼손 무기만 장착
	{
		if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 1 )		// 활 
		{
			return 1; 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 ) // 방패 
		{
			return 3; 
		}
		else if( g_Pc_Manager.m_pItm[LHand_index]->m_type == 5 ) // 마법서 
		{
			return 5; 
		}
	}
	else if( RHand_index != -1 && LHand_index != -1 )			// 양손에 무기 장착
	{
		if( g_Pc_Manager.m_pItm[RHand_index]->m_type == 2 && 
			g_Pc_Manager.m_pItm[LHand_index]->m_type == 3 )		// 한손검 + 방패
		{
			return 2; 
		}
	}
	else														// 무기 없음 (맨손 공격)
	{
			return -1; 
	}

	return -1;
}

int Character::GetDirectBoneLinkTM(int Num,  D3DXMATRIX &mat )
{
	int nNode = Num;	
	
	D3DXQUATERNION	qtRot;
	D3DXMATRIX		matReady;
	D3DXMATRIX		matTemp;
	D3DXMatrixIdentity( &matTemp  );
	D3DXMatrixIdentity( &matReady );

	mat = matTemp;

	float factor = 0.0f;
	bool bMotionBlend = true;

	int MotionBlendFrame = 0;
	int MotionBlendHalfFrame = 0;	

	MotionBlendFrame = c_MotionBlendFrame + m_start_blend_frame;
	MotionBlendHalfFrame = c_MotionBlendHalfFrame + m_start_blend_frame;

	int ani_num = m_ani_number;
	int pre_ani_num = m_pre_ani_number;
	
	// 이전애니메이션 없다
	if( pre_ani_num == -1 )
	{
		pre_ani_num = ani_num;
		bMotionBlend = false;		
	}
	else
	{
		factor = (float)(m_curt_frame - m_start_blend_frame) 
				  /(MotionBlendFrame - m_start_blend_frame);
	}
	
	if( g_Pc_Manager.m_pAni[ani_num] == NULL || g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node == NULL )
	{
		return FALSE;
	}

	//-----------------------------------------------------------------------------
	// 현재 노드 회전값
	//-----------------------------------------------------------------------------	
	if( nNode >= g_Pc_Manager.m_pAni[ani_num]->m_num_node || 
		nNode >= g_Pc_Manager.m_pAni[pre_ani_num]->m_num_node )
		return FALSE;

	if( bMotionBlend )
	{
		blendQuaternion( &qtRot, factor, 
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_qtRot, 
						g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[MotionBlendHalfFrame].m_qtRot );
	}
	else
	{
		// 쿼터니온 중간값 구하기
		D3DXQuaternionSlerp( &qtRot, 
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_qtRot,
							 &g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_next_frame].m_qtRot,
							 m_middle_time );
	}

	// 쿼터니온 -> 메트릭스 ( 현재 회전각 알아내기 )
	D3DXMatrixRotationQuaternion( &matTemp, &qtRot );		

	//-----------------------------------------------------------------------------
	// 현재 동작의 로칼메트릭스
	//-----------------------------------------------------------------------------	
	{
		if( m_bPC == true ||
			( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC ) )		
		{
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_Standard, nNode ), &matTemp );
		}
		else		
			D3DXMatrixMultiply(	&matTemp, &g_Pc_Manager.GetAniLocalTM( this, n_NPC_Stand0, nNode ), &matTemp );		
	}

	if( bMotionBlend )
	{	
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[MotionBlendHalfFrame].m_vPos.x -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.x ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.x;
		
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[MotionBlendHalfFrame].m_vPos.y -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.y ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.y;
		
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[MotionBlendHalfFrame].m_vPos.z -
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.z ) * factor ) +
						g_Pc_Manager.m_pAni[pre_ani_num]->m_pNode[nNode].m_pAnimation[m_pre_end_frame].m_vPos.z;
	}
	else
	{
		//-----------------------------------------------------------------------------
		// 위치값
		//-----------------------------------------------------------------------------
		matTemp._41 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_next_frame].m_vPos.x -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.x ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.x;			 
		matTemp._42 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_next_frame].m_vPos.y -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.y ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.y;
		matTemp._43 = (( g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_next_frame].m_vPos.z -
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.z ) * m_middle_time ) +
						 g_Pc_Manager.m_pAni[ani_num]->m_pNode[nNode].m_pAnimation[m_curt_frame].m_vPos.z;
	}		
	//-----------------------------------------------------------------------------
	// 스케일값 곱하기
	//-----------------------------------------------------------------------------
	D3DXMATRIX	matTemp2;
	D3DXMatrixScaling( &matTemp2, m_vScale.x, m_vScale.y, m_vScale.z );
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
	
	//-----------------------------------------------------------------------------
	// 등에 차고 있다면.. 해당 지정값만큼 곱한다 
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matReady, &matTemp );	
		
	//-----------------------------------------------------------------------------
	// 오브젝트 회전한 만큼 메트릭스 회전하기
	//-----------------------------------------------------------------------------
	D3DXQuaternionRotationMatrix( &qtRot, &m_matChr );	
	D3DXMatrixRotationQuaternion( &matTemp2, &qtRot );	
	//-----------------------------------------------------------------------------
	// 월드상의 위치값 더하기
	//-----------------------------------------------------------------------------
	D3DXMatrixMultiply( &matTemp, &matTemp, &matTemp2 );
	
	if(!m_State.bRide)
	{
		matTemp._41 += m_matChr._41;
		matTemp._42 += m_matChr._42;
		matTemp._43 += m_matChr._43;
	}
	else
	{
		matTemp._41 += m_matRideChr._41;
		matTemp._42 += m_matRideChr._42;
		matTemp._43 += m_matRideChr._43;
	}

	mat = matTemp;
	
	return TRUE;
}

//...............................................................................................................
// 이펙트 출력시 본 메트릭스 구하기 ( NPC 도 추가 )
//...............................................................................................................
int Character::GetBoneLinkTM(int Num,  D3DXMATRIX &mat, BOOL IsModify )
{
 	if( !m_bPC && IsModify && g_Map.IsGiantUnit( this ) )
	{
		mat = GetBonCenterTM();
		float fHeight	= ( GetBoundMaxPos().y - GetBoundMinVec().y );
		//fHeight *= 0.3f;
		fHeight = 30.0f;
		mat._42 += fHeight;
			
		return TRUE;
	}	
	
	if(!m_bPC && m_State.nCharacterType != nCharacterTypePlayerNPC )	///NPC일 경우
	{
		SNPCBaseInfo* npc_Info;				
		npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( g_Map.m_NpcTable[ m_nTableIndex ].code ); 

		if( npc_Info == NULL )
		{
			D3DXMatrixIdentity( &mat );
			return TRUE;
		}
		
		///-- 해당 노드에 링크된 본이 존재하지 않는다면..
		if( -1 == npc_Info->theBorn[ Num ] )
		{
			///-- 중심 센터값을 리턴시킨다.
			D3DXMatrixIdentity( &mat );
			
			mat._41 = GetPosTM()._41;
			mat._42 = GetPosTM()._42 + GetBoundMaxPos().y * 0.5f;
			mat._43 = GetPosTM()._43;

			return TRUE;
		}	
	}
	
	if(!m_bIsVisible)
	{
		m_IsApplyVA = TRUE;
		BoneRender();
	}
	
	switch( Num )
	{
		//.......................................................................................................
		// 링크된 본 없음
		//.......................................................................................................
		case 0:
			
			D3DXMatrixIdentity( &mat );

			return TRUE;

		//.......................................................................................................
		// 머리
		//.......................................................................................................
		case 1:

			mat = m_matRealHair;

			return TRUE;

		//.......................................................................................................
		// 가슴
		//.......................................................................................................
		case 2:

			mat = m_matRealbreast;

			return TRUE;

		//.......................................................................................................
		// 낭심
		//.......................................................................................................
		case 3:

			mat = m_matRealCenter;

			return TRUE;

		//.......................................................................................................
		// 왼손
		//.......................................................................................................
		case 4:
		case 10:

			mat = m_matRealLHand;

			return TRUE;

		//.......................................................................................................
		// 오른손
		//.......................................................................................................
		case 5:
		case 11:


			mat = m_matRealRHand;

			return TRUE;

		//.......................................................................................................
		// 왼발
		//.......................................................................................................
		case 6:

			mat = m_matRealLFoot;

			return TRUE;

		//.......................................................................................................
		// 오른발
		//.......................................................................................................
		case 7:

			mat = m_matRealRFoot;

			return TRUE;

		//.......................................................................................................
		// 칼끝
		//.......................................................................................................
		case 8:

			D3DXMatrixIdentity( &mat );

			if( m_pUserInfo == NULL )
				return FALSE;

			mat._41 = m_pUserInfo->vSwordAxis[ 0 ].x;
			mat._42 = m_pUserInfo->vSwordAxis[ 0 ].y;
			mat._43 = m_pUserInfo->vSwordAxis[ 0 ].z;

			return TRUE;

		//.......................................................................................................
		// 칼끝
		//.......................................................................................................
		case 9:

			D3DXMatrixIdentity( &mat );

			if( m_pUserInfo == NULL )
				return FALSE;

			mat._41 = m_pUserInfo->vSwordAxis[ 1 ].x;
			mat._42 = m_pUserInfo->vSwordAxis[ 1 ].y;
			mat._43 = m_pUserInfo->vSwordAxis[ 1 ].z;

			return TRUE;
					
		// 왼 어깨
		case 12:
			mat = m_matRealLShoulder;
			return TRUE;
		//오른 어깨 
		case 13:
			mat = m_matRealRShoulder;
			return TRUE;
		//왼 꿈치 
		case 14:
			mat = m_matRealLElbow;
			return TRUE;
		// 오른 꿈치 
		case 15:
			mat = m_matRealRElbow;
			return TRUE;
		// 왼 무릎 
		case 16:
			mat = m_matRealLKnee;
			return TRUE;
		// 오른 무릎 
		case 17:
			mat = m_matRealRKnee;
			return TRUE;
		// 왼 골반 
		case 18:
			mat = m_matRealLPelvis;
			return TRUE;
		// 오른 골반 
		case 19:
			mat = m_matRealRPelvis;	
			return TRUE;

		default:
			return TRUE;



	}


	return FALSE;
}

//.............................................................................................................
// 선점권자 정보 n_PC, n_Party , 아이디 ( 유니크 )			
// PC 는 해당 사항 없음 ( AppearNPC , NpcPriority 설정 )	 By wxywxy
//.............................................................................................................
void Character::SetPriority(BYTE thePriorityType,DWORD thePriorityID)
{
	if( m_bPC )
	{
		m_ISPriority = TRUE;
		return;
	}

	if( !m_bPC )
	{
		DWORD dNPCCode = g_Map.m_NpcTable[m_nTableIndex].code;	
		SNPCBaseInfo* npc_Info = g_Pc_Manager.GetNPCBaseInfoPtr( dNPCCode );

		if(npc_Info)
		{
			if( npc_Info->theGender == n_Millena_Unit )
			{
				thePriorityType = n_NONE;
				thePriorityID = -1;
			}

			if( npc_Info->theGender == n_Rain_Unit )
			{
				thePriorityType = n_NONE;
				thePriorityID = -1;
			}

			if( npc_Info->theGender == n_Neutral_Unit )
			{
				thePriorityType = n_NONE;
				thePriorityID = -1;
			}
		}
	}

	SPARTY_INFO *pPartyInfo = g_PartyInforWnd.GetPartyInfo();

	switch( thePriorityType )
	{
		//.....................................................................................................	
		// 선점자 PC 일때
		//.....................................................................................................	
		case n_PC:
			
			if( g_Pc.GetPlayer()->GetUnique() == thePriorityID )
				m_ISPriority = TRUE;
			else
				m_ISPriority = FALSE;

			return;

		//.....................................................................................................	
		// 선점자 파티일때
		//.....................................................................................................	
		case n_PARTY:
			
			if( pPartyInfo->m_dPartyID == thePriorityID )
				m_ISPriority = TRUE;
			else
				m_ISPriority = FALSE;			
			return;

		//.....................................................................................................	
		// 그외 해당사항 없을때 선점
		//.....................................................................................................	
		default:

			m_ISPriority = TRUE;

			return;
	}
}



void Character::ChangeFaceExp(CFaceExpData::FACE_EXP eType)
{
	if( m_pCurrentFaceExpData == NULL )
		return;
	
	m_NowFaceType  = eType;
	m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , m_pCurrentFaceExpData->GetDisplayTime( eType ) );
}

void Character::FaceExpProcess(void)
{
	static DWORD Code = 0;
	//............................................................................................
	// 표정 관리
	//............................................................................................
	if( m_pCurrentFaceExpData == NULL )
	{
		
		if( g_Pc.GetPlayer() == this )
		{
			Code = g_Pc.m_MainPC.char_info.theFace;
		}
		else
		{
			if(m_bPC)
			{
				Code = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theFace;
			}
			else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				Code = g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theFace;
			}
		}
		
		m_pCurrentFaceExpData = g_RockClient.m_FaceExp.GetFaceExpInfo( Code );
		
	}
	else
	{
		if( g_Pc.GetPlayer() == this )
		{
			Code = g_Pc.m_MainPC.char_info.theFace;
		}
		else
		{
			if(m_bPC)
			{
				Code = g_Pc.m_PcTable[ m_nTableIndex ].char_info.theFace;
			}
			else if( !m_bPC && m_State.nCharacterType == nCharacterTypePlayerNPC )
			{
				Code = g_Map.m_NpcTable[ m_nTableIndex ].theExPcInfo.theFace;
			}
		}

		if( m_pCurrentFaceExpData->m_FaceCode != Code )
		{
			m_pCurrentFaceExpData = g_RockClient.m_FaceExp.GetFaceExpInfo( Code );
		}
	}

	if( m_pCurrentFaceExpData )
	{
		// 누워 있을때는 계속 눈 감고 있기
		if( ( m_curt_event.type == SM_ACT && m_ani_number == 301 ) || m_curt_event.type == SM_STATE_DIE )
		{
			m_NowFaceType = CFaceExpData::FACE_EXP_TWINKLE;
			m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , m_pCurrentFaceExpData->GetDisplayTime( CFaceExpData::FACE_EXP_TWINKLE ) );
		}
		
		if( m_NowFaceType == CFaceExpData::FACE_EXP_NORMAL )
		{
			m_NowFaceType  = CFaceExpData::FACE_EXP_TWINKLE_WAIT;
			m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , Random( ( int ) m_pCurrentFaceExpData->m_TwinkleWaitMin , ( int )m_pCurrentFaceExpData->m_TwinkleWaitMax ) );
		}
		else if( m_NowFaceType == CFaceExpData::FACE_EXP_TWINKLE_WAIT )
		{
			if( SAFE_TIME_COMPARE( m_FaceExpTimer , < , g_nowTime ) )
			{
				m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , m_pCurrentFaceExpData->GetDisplayTime( CFaceExpData::FACE_EXP_TWINKLE ) );
				m_NowFaceType  = CFaceExpData::FACE_EXP_TWINKLE;
			}
			
		}
		
		if( g_RockClient.m_SceneManager.GetFixFaceExp() != CFaceExpData::FACE_EXP_MAXCOUNT )
		{
			if( g_RockClient.m_SceneManager.GetFixFaceExp() == CFaceExpData::FACE_EXP_NORMAL )
			{
				m_NowFaceType  = CFaceExpData::FACE_EXP_TWINKLE_WAIT;
				m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , Random( ( int ) m_pCurrentFaceExpData->m_TwinkleWaitMin , ( int )m_pCurrentFaceExpData->m_TwinkleWaitMax ) );
			}
			else if( g_RockClient.m_SceneManager.GetFixFaceExp() == CFaceExpData::FACE_EXP_TWINKLE )
			{
				m_FaceExpTimer = SAFE_TIME_ADD( g_nowTime , m_pCurrentFaceExpData->GetDisplayTime( CFaceExpData::FACE_EXP_TWINKLE ) );
				m_NowFaceType  = CFaceExpData::FACE_EXP_TWINKLE;
			}
		}
		
		if( m_NowFaceType >= CFaceExpData::FACE_EXP_TWINKLE )
		{
			if( SAFE_TIME_COMPARE( m_FaceExpTimer , < , g_nowTime ) )
			{
				m_FaceExpTimer = 0;
				m_NowFaceType  = CFaceExpData::FACE_EXP_NORMAL;
			}
			else
			{
#ifdef DIRECT_VERSION_9_MJH
				LPDIRECT3DTEXTURE9 pTex = NULL;
#else
				LPDIRECT3DTEXTURE8 pTex = NULL;
#endif // DIRECT_VERSION_9_MJH
				
				pTex = m_pCurrentFaceExpData->GetTexture( m_NowFaceType );
				
				if( pTex != NULL )
					g_RenderManager.SetTexture( 0 , pTex );
			}
		}
		
		
	}
}


bool Character::MultiAttackTargetMgr( EVENT_DATA_INFO& aEvent )
{
	int tgNum = 0;
	int damage = 0; 
	int attackedIdx = 0;
	SAttacked* tg_Info = (SAttacked*) aEvent.target_list;
	bool bIsAttackBack = false;
	
	if( tg_Info )
	{
		for( tgNum = 0; tgNum < aEvent.number; ++tgNum )
		{			
//			DWORD		theAttackedID;		// 맞은 객체
//			DWORD		theAttackedHP;		// 맞은 객체의 남은 HP (유저는 WORD로 변환해야 한다. )
//			BYTE		theAttackedType:1;	// (1)맞은 객체의 타입 PC, NPC
//			BYTE		theDieFlag:2;		// (1)n_Success(죽음) : 죽음 모션처리를 위해 수정됨 
//			BYTE		theCritical:2;		// (1)크리티컬 성공 여부 (n_Success/n_Fail)
						
			if( tg_Info[tgNum].theAttackedType == n_PC )
			{
				if( tg_Info[tgNum].theAttackedID == g_Pc.m_MainPC.lUnique )				
				{
					g_Particle.m_EffectManager.StandAttack( (Character*) this, (Character*)g_Pc.GetPlayer(),
						SM_ATTACK_NPC, 0, tg_Info[tgNum].theDieFlag, tg_Info[tgNum].theAttackedHP, 
						tg_Info[tgNum].theCritical, aEvent.lStartTime );
				}				
				else
				{
					attackedIdx = g_Pc.FindUser( tg_Info[tgNum].theAttackedID );		
					if( attackedIdx != MAX_USER_INTABLE )
					{
						g_Particle.m_EffectManager.StandAttack( (Character*) this, (Character*) &g_Pc.m_Pc[attackedIdx],
							SM_ATTACK_NPC, 0, tg_Info[tgNum].theDieFlag, tg_Info[tgNum].theAttackedHP, 
							tg_Info[tgNum].theCritical, aEvent.lStartTime );
					}
				}				
			}
			else if( tg_Info[tgNum].theAttackedType == n_NPC )
			{
				attackedIdx = g_Map.FindNpc( tg_Info[tgNum].theAttackedID );							
				if( attackedIdx != MAX_NPC_INTABLE )
				{		
					g_Particle.m_EffectManager.StandAttack( (Character*) this, (Character*) &g_Map.m_Npc[attackedIdx],
						SM_ATTACK_NPC, 0, tg_Info[tgNum].theDieFlag, tg_Info[tgNum].theAttackedHP, 
						tg_Info[tgNum].theCritical, aEvent.lStartTime );											
				}
			}

			if( bIsAttackBack == false )
			{
				if( ( !m_bPC == tg_Info[tgNum].theAttackedType ) && ( m_nTableIndex == attackedIdx ) && ( GetUnique() == tg_Info[tgNum].theAttackedID ) )
				{
					bIsAttackBack = true;
				}
			}
		}
	}		

	// 받아치기 당한 상태라면 지역 공격 마법 이펙트는 보여주지 않는다. 
	// 받아치기는 당한 상태라면 자신한테만 되돌려 주기 때문에 개수는 한명뿐이다. 
	if( bIsAttackBack && ( aEvent.number == 1 ) )
	{
		return false;
	}

	return true;
}

BOOL Character::ISEmblemWear(WORD Code)
{
	// 깃발과 엠블렘 세트아이템코드 같이 사용 By wxywxy
	// 하드코딩 정말 싫오 .. 나중에 아이템 테이블에서 쓸만한 것으로 대체 .. 
	if( (12601 <= Code && Code <= 12700) || 
		(16001 <= Code && Code <= 16060) || 
		(16291 <= Code && Code <= 16500) || 
		(20347 <= Code && Code <= 20358) ||
		(17801 <= Code && Code <= 17856) 
		) 
	{		
		WORD ItemType = g_Pc_Manager.GetItemClsss( Code );
	
		// 깃발이 아닌것 분류 By wxywxy
		if( ItemType != n_Class_Serve )
			return TRUE;
	}
     
	return FALSE;
}

void Character::ChangeState(CState * pNewState)
{
	if(m_pCurrentState)
	{
		m_pCurrentState->Exit(this);	
	}

	if(pNewState)
	{
		m_pCurrentState = pNewState;
		m_pCurrentState->Enter(this);
	}
}

void Character::SetApplyConAniSkill(BOOL bFlag)
{
	m_IsApplyConAniSkill = bFlag;
	
	if(!m_IsApplyConAniSkill)
	{
		m_aConAniTime = c_Stand_AniTime;
	}
}

void Character::SetConAniTime(int nTime)
{
	if(m_IsApplyConAniSkill)
	{
		m_aConAniTime = c_Stand_AniTime * (nTime / 100.0f);
	}
	else
	{
		m_aConAniTime = c_Stand_AniTime;
	}
}
