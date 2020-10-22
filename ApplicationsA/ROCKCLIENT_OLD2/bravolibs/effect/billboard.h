
#ifndef		__BILLBOARD_H__
#define		__BILLBOARD_H__

#define		MAXBUFFER	10
#include	"Timer.h"

#include    "..\\obj\\character.h"

enum eBillboardDataType
{
	eBDType_Shield = 0,			// 0. 실드
	eBDType_VerticalPlane,		// 1. 세로 방향 면
	eBDType_HorizonPlane,		// 2. 가로 방향 면
	eBDType_VerticalLongPlane,	// 3. 세로 방향 직사각형		
	eBDType_HorizonLongPlane,	// 4. 가로 방향 직사각형
	eBDType_Cylinder,			// 5. 실린더
	eBDType_CylinderUp,			// 6. 실린더 위가 큰거
	eBDType_Hemisphere,			// 7. 반구	
	eBDType_CylinderDown,		// 8. 실린더 아래가 큰거
	eBDType_Sphere,				// 9. 구
	eBDType_Donut,				// 10. 도넛 ( UV 개별적 )
	eBDType_DonutUVOne,			// 11. 도넛 ( UV 전체 )
	eBDType_Spot,				// 12. 스팟 
	eBDType_Spot1,				// 13. 스팟 ( 뒤로 굽은거 )
	eBDType_Spot2,				// 14. 스팟 ( 뒤로 굽은거 )
	eBDType_Sword,				// 15. 칼흔
	eBDType_CrossPlane,			// 16. 크로스 평면
	eBDType_Cylinder6,			// 17. 실린더 각진거 6각
	eBDType_Cylinder8,			// 18. 실린더 각진거 8각

	eBDType_Count,
};

class		CBillboardData
{
public:
	//------------------------------------------------------------------------------
	// type
	//------------------------------------------------------------------------------
	int				m_iType;		// 어떤놈인가.

	//------------------------------------------------------------------------------
	// multytex
	//------------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTexture;
#else
	LPDIRECT3DTEXTURE8	m_pTexture;
#endif // DIRECT_VERSION_9_MJH
	char			m_cTexName[ 256 ];	// 보통 텍스쳐면 이름 
	bool			m_bTexAni;		// 텍스쳐 에니메이션이냐.
	int				m_iTexType;		// 멀티텍스쳐넘버

	//------------------------------------------------------------------------------
	// time
	//------------------------------------------------------------------------------
	
	bool			m_bInfinityLoop;	// 무한 루프 이냐.
	int				m_iLoop;			// 루프 몇번 돌것인지.
	float			m_fLife;			// 한번 루프의 시간.

	//------------------------------------------------------------------------------
	// size
	//------------------------------------------------------------------------------
	bool			m_bSizePump;		// 펌핑 한다.
	D3DXVECTOR3		m_vStartSize;
	D3DXVECTOR3		m_vEndSize;

	//------------------------------------------------------------------------------
	// moving
	//------------------------------------------------------------------------------
	bool			m_bMovePump;	// 움직임이 왔다 갔다 한다.
	float			m_fHeight;		// 어느정도 높이에서 나오는지. 1.0f가 맥스.
	D3DXVECTOR3		m_vMovVec;		// 움직이는 방향 m_vMoveVec이 가르키는 방향이 마지막으로 도착 하는 방향 

	//------------------------------------------------------------------------------
	// cam
	//------------------------------------------------------------------------------
	float			m_fRadius;		// 떨어진 거리.
	bool			m_bCharSightRot;// 캐릭터 보는 방향으로 따라 당김.( xz로만 )

	bool			m_bXZCamRot;	// xz로 카메라 따라 댕김.
	bool			m_bYZCamRot;	// yz로 카메라 따라 댕김.
	bool			m_bXYZCamRot;	// xyz로 카메라 따라 댕김.

	float			m_fRotVel;		// 자체 축회전 한다면 속도와 방향 ( 음수이면 시계 반대 방향 )
	bool			m_bXAxisRot;	// x 축 회전
	bool			m_bYAxisRot;	// y 축 회전
	bool			m_bZAxisRot;	// z 축 회전

	int				m_iSequence;	// 회전 순서.( XYZ XZY YXZ YZX ZXY ZYX )
	float			m_fAngleX;		// 고정각 X
	float			m_fAngleY;		// 고정각 Y
	float			m_fAngleZ;		// 고정각 Z

	//------------------------------------------------------------------------------
	// blend
	//------------------------------------------------------------------------------
	bool			m_bBlend;		// 블렌딩.
	bool			m_bBlendLoop;	// 루프 도는 블렌딩.

	// 내 번호 자동 저장
	int				m_iMyType;		// 내 인덱스 번호

	//............................................................................................................
	// 알파값 조정 By wxywxy
	//............................................................................................................
	FLOAT			m_fAlphaStart;
	FLOAT			m_fAlphaVar;
	FLOAT			m_fAlphaEnd;

	//............................................................................................................
	// 색 조정 
	//............................................................................................................
	D3DXCOLOR		m_ColorStart;
	D3DXCOLOR		m_ColorVar;
	D3DXCOLOR		m_ColorEnd;

	//............................................................................................................
	// 블렌딩 모드 
	//............................................................................................................
	BYTE			m_SrcBlend;
	BYTE			m_DestBlend;

	unsigned long	m_HashCode;

	void			LoadData(char *FileName);
	void			LoadTexture( char* path );

	CBillboardData() 
	{
		 //............................................................................................................
		 // 알파값 조정 By wxywxy
		 //............................................................................................................
		 m_fAlphaStart   = 1.0f;
		 m_fAlphaVar     = 0.0f;
		 m_fAlphaEnd     = 1.0f;
		 
		 //............................................................................................................
		 // 색 조정    By wxywxy
		 //............................................................................................................
		 
		 m_ColorStart.r    = 1.0f;
		 m_ColorStart.g    = 1.0f;
		 m_ColorStart.b    = 1.0f;
		 
		 m_ColorVar.r      = 0.0f;
		 m_ColorVar.g      = 0.0f;
		 m_ColorVar.b      = 0.0f;
		 
		 m_ColorEnd.r      = 1.0f;
		 m_ColorEnd.g      = 1.0f;
		 m_ColorEnd.b      = 1.0f;
		 
		 //............................................................................................................
		 // 블렌딩 모드
		 //............................................................................................................
		 
		 m_SrcBlend        = ( BYTE ) D3DBLEND_SRCALPHA;
		 m_DestBlend       = ( BYTE ) D3DBLEND_DESTALPHA;
		 
		 m_pTexture = NULL;
		 
		 m_HashCode = 0;
	}

	~CBillboardData() 
	{
		m_pTexture = NULL;
	}
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9 GetTexture();
#else
	LPDIRECT3DTEXTURE8 GetTexture();
#endif // DIRECT_VERSION_9_MJH	
};




class		CBillboard
{
public:
	int				m_nHeight;				// 높이
	int				m_iEffSetCon;
	int				m_BoneLink;				// 링크되는 본
	int				m_nDirectEffType;		// 직접링크 이펙트 타입
	
	BYTE			m_aDirectLinkBone;		// 직접 링크되는 본번호	

public:
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matLocalWorld;
	D3DXMATRIX		m_matPostWorld;
	// 현재 움직여야할 변수들
	int				m_iCurLoop;		// 현재 루프 돈 개수.
	float			m_fCurLife;		// 현재 라이프

	bool			m_bPumping;		// 펌핑 중이냐.
	bool			m_bMoving;		// 이동중?
	BOOL			m_LoopBlending; // 블렌딩 펌핑

	long			m_lUnique;		// 사람 유니크
	int				m_iIndex;		// 인덱스
	float			m_fDelayTime;	// 딜레이 타임

	float			m_fHeight;		// 높이
	D3DXVECTOR3		m_vLoc;			// 위치 
	D3DXVECTOR3		m_vLinkBone;	// 본 위치

	CTimer			m_Timer;

	D3DXVECTOR3		m_vStartSize;	
	D3DXVECTOR3		m_vEndSize;

	D3DXVECTOR3		m_vMovVec;	
	float			m_fRadius;

	D3DXCOLOR		m_Color;
	float			m_fAlpha;
	D3DXCOLOR		m_ColorDelta;
	float			m_fAlphaDelta;

	float		    m_AddRadian;		// 증감되는 각

	float			m_fSizeX;
	float			m_fSizeZ;

	Character		*m_pSrc;			// 위치 얻어 올때 사용
	D3DXMATRIX		m_mLinkBoneMatrix;

	float			m_fSetRadian;	// 첫 시작시 고정된 각도로 처리할때

	e_SCENE_MODE	m_SceneMode;

	float			m_fScale;

	D3DXVECTOR3		m_vAttackSrcDir;	// 공격 주체 방향
	UINT			m_uSetType;
	Character		*m_pAttackUnit;
	BOOL			m_IsDirAttatack;
	BOOL			m_IsGUnitLinkBoneAndSizeFix;
	
	BOOL			m_IsInitUpdate;			// 초기 업데이트 여부
	
	long			m_nPrevUpdateTime;		// 업데이트 이전타임
	long			m_nDelUpdateTime;		// 업데이트 간격타임
	BOOL			m_IsUpdate;				// 업데이트 여부
	FLOAT			m_fDelCumtime;			// 누적타임

public:
	CBillboard();
	virtual ~CBillboard();

	CBillboardData	*m_pBillboardData;

	int				UpdateBillboard( float TimeD );
	int				UpdateBillboardNoneSet( float TimeD );
	BOOL			ChkBatchAble();
	void			DrawBatchBillboard(BOOL bApyPostMat = FALSE, BOOL bNoDraw = FALSE );
	void			DrawBillboard(BOOL bApyPostMat = FALSE, BOOL bNoDraw = FALSE );
	void  		    MakeLink(CBillboardData *pData);

	void			ReStart(void);	
	void			ColorReset(void);

	CTimer &Timer() { return(m_Timer); }

	bool operator < (CBillboard &a) {
		return m_pBillboardData->m_HashCode < a.m_pBillboardData->m_HashCode;
	};

	bool operator == (CBillboard &a) {
		return m_pBillboardData->m_HashCode == a.m_pBillboardData->m_HashCode;
	};

// 	bool operator > (const CBillboard &a) const {
// 		return m_pBillboardData->m_HashCode > a.m_pBillboardData->m_HashCode;
// 	};
};

#endif
