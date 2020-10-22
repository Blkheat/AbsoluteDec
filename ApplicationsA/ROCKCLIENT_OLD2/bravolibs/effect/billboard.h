
#ifndef		__BILLBOARD_H__
#define		__BILLBOARD_H__

#define		MAXBUFFER	10
#include	"Timer.h"

#include    "..\\obj\\character.h"

enum eBillboardDataType
{
	eBDType_Shield = 0,			// 0. �ǵ�
	eBDType_VerticalPlane,		// 1. ���� ���� ��
	eBDType_HorizonPlane,		// 2. ���� ���� ��
	eBDType_VerticalLongPlane,	// 3. ���� ���� ���簢��		
	eBDType_HorizonLongPlane,	// 4. ���� ���� ���簢��
	eBDType_Cylinder,			// 5. �Ǹ���
	eBDType_CylinderUp,			// 6. �Ǹ��� ���� ū��
	eBDType_Hemisphere,			// 7. �ݱ�	
	eBDType_CylinderDown,		// 8. �Ǹ��� �Ʒ��� ū��
	eBDType_Sphere,				// 9. ��
	eBDType_Donut,				// 10. ���� ( UV ������ )
	eBDType_DonutUVOne,			// 11. ���� ( UV ��ü )
	eBDType_Spot,				// 12. ���� 
	eBDType_Spot1,				// 13. ���� ( �ڷ� ������ )
	eBDType_Spot2,				// 14. ���� ( �ڷ� ������ )
	eBDType_Sword,				// 15. Į��
	eBDType_CrossPlane,			// 16. ũ�ν� ���
	eBDType_Cylinder6,			// 17. �Ǹ��� ������ 6��
	eBDType_Cylinder8,			// 18. �Ǹ��� ������ 8��

	eBDType_Count,
};

class		CBillboardData
{
public:
	//------------------------------------------------------------------------------
	// type
	//------------------------------------------------------------------------------
	int				m_iType;		// ����ΰ�.

	//------------------------------------------------------------------------------
	// multytex
	//------------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9	m_pTexture;
#else
	LPDIRECT3DTEXTURE8	m_pTexture;
#endif // DIRECT_VERSION_9_MJH
	char			m_cTexName[ 256 ];	// ���� �ؽ��ĸ� �̸� 
	bool			m_bTexAni;		// �ؽ��� ���ϸ��̼��̳�.
	int				m_iTexType;		// ��Ƽ�ؽ��ĳѹ�

	//------------------------------------------------------------------------------
	// time
	//------------------------------------------------------------------------------
	
	bool			m_bInfinityLoop;	// ���� ���� �̳�.
	int				m_iLoop;			// ���� ��� ��������.
	float			m_fLife;			// �ѹ� ������ �ð�.

	//------------------------------------------------------------------------------
	// size
	//------------------------------------------------------------------------------
	bool			m_bSizePump;		// ���� �Ѵ�.
	D3DXVECTOR3		m_vStartSize;
	D3DXVECTOR3		m_vEndSize;

	//------------------------------------------------------------------------------
	// moving
	//------------------------------------------------------------------------------
	bool			m_bMovePump;	// �������� �Դ� ���� �Ѵ�.
	float			m_fHeight;		// ������� ���̿��� ��������. 1.0f�� �ƽ�.
	D3DXVECTOR3		m_vMovVec;		// �����̴� ���� m_vMoveVec�� ����Ű�� ������ ���������� ���� �ϴ� ���� 

	//------------------------------------------------------------------------------
	// cam
	//------------------------------------------------------------------------------
	float			m_fRadius;		// ������ �Ÿ�.
	bool			m_bCharSightRot;// ĳ���� ���� �������� ���� ���.( xz�θ� )

	bool			m_bXZCamRot;	// xz�� ī�޶� ���� ���.
	bool			m_bYZCamRot;	// yz�� ī�޶� ���� ���.
	bool			m_bXYZCamRot;	// xyz�� ī�޶� ���� ���.

	float			m_fRotVel;		// ��ü ��ȸ�� �Ѵٸ� �ӵ��� ���� ( �����̸� �ð� �ݴ� ���� )
	bool			m_bXAxisRot;	// x �� ȸ��
	bool			m_bYAxisRot;	// y �� ȸ��
	bool			m_bZAxisRot;	// z �� ȸ��

	int				m_iSequence;	// ȸ�� ����.( XYZ XZY YXZ YZX ZXY ZYX )
	float			m_fAngleX;		// ������ X
	float			m_fAngleY;		// ������ Y
	float			m_fAngleZ;		// ������ Z

	//------------------------------------------------------------------------------
	// blend
	//------------------------------------------------------------------------------
	bool			m_bBlend;		// ����.
	bool			m_bBlendLoop;	// ���� ���� ����.

	// �� ��ȣ �ڵ� ����
	int				m_iMyType;		// �� �ε��� ��ȣ

	//............................................................................................................
	// ���İ� ���� By wxywxy
	//............................................................................................................
	FLOAT			m_fAlphaStart;
	FLOAT			m_fAlphaVar;
	FLOAT			m_fAlphaEnd;

	//............................................................................................................
	// �� ���� 
	//............................................................................................................
	D3DXCOLOR		m_ColorStart;
	D3DXCOLOR		m_ColorVar;
	D3DXCOLOR		m_ColorEnd;

	//............................................................................................................
	// ���� ��� 
	//............................................................................................................
	BYTE			m_SrcBlend;
	BYTE			m_DestBlend;

	unsigned long	m_HashCode;

	void			LoadData(char *FileName);
	void			LoadTexture( char* path );

	CBillboardData() 
	{
		 //............................................................................................................
		 // ���İ� ���� By wxywxy
		 //............................................................................................................
		 m_fAlphaStart   = 1.0f;
		 m_fAlphaVar     = 0.0f;
		 m_fAlphaEnd     = 1.0f;
		 
		 //............................................................................................................
		 // �� ����    By wxywxy
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
		 // ���� ���
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
	int				m_nHeight;				// ����
	int				m_iEffSetCon;
	int				m_BoneLink;				// ��ũ�Ǵ� ��
	int				m_nDirectEffType;		// ������ũ ����Ʈ Ÿ��
	
	BYTE			m_aDirectLinkBone;		// ���� ��ũ�Ǵ� ����ȣ	

public:
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matLocalWorld;
	D3DXMATRIX		m_matPostWorld;
	// ���� ���������� ������
	int				m_iCurLoop;		// ���� ���� �� ����.
	float			m_fCurLife;		// ���� ������

	bool			m_bPumping;		// ���� ���̳�.
	bool			m_bMoving;		// �̵���?
	BOOL			m_LoopBlending; // ���� ����

	long			m_lUnique;		// ��� ����ũ
	int				m_iIndex;		// �ε���
	float			m_fDelayTime;	// ������ Ÿ��

	float			m_fHeight;		// ����
	D3DXVECTOR3		m_vLoc;			// ��ġ 
	D3DXVECTOR3		m_vLinkBone;	// �� ��ġ

	CTimer			m_Timer;

	D3DXVECTOR3		m_vStartSize;	
	D3DXVECTOR3		m_vEndSize;

	D3DXVECTOR3		m_vMovVec;	
	float			m_fRadius;

	D3DXCOLOR		m_Color;
	float			m_fAlpha;
	D3DXCOLOR		m_ColorDelta;
	float			m_fAlphaDelta;

	float		    m_AddRadian;		// �����Ǵ� ��

	float			m_fSizeX;
	float			m_fSizeZ;

	Character		*m_pSrc;			// ��ġ ��� �ö� ���
	D3DXMATRIX		m_mLinkBoneMatrix;

	float			m_fSetRadian;	// ù ���۽� ������ ������ ó���Ҷ�

	e_SCENE_MODE	m_SceneMode;

	float			m_fScale;

	D3DXVECTOR3		m_vAttackSrcDir;	// ���� ��ü ����
	UINT			m_uSetType;
	Character		*m_pAttackUnit;
	BOOL			m_IsDirAttatack;
	BOOL			m_IsGUnitLinkBoneAndSizeFix;
	
	BOOL			m_IsInitUpdate;			// �ʱ� ������Ʈ ����
	
	long			m_nPrevUpdateTime;		// ������Ʈ ����Ÿ��
	long			m_nDelUpdateTime;		// ������Ʈ ����Ÿ��
	BOOL			m_IsUpdate;				// ������Ʈ ����
	FLOAT			m_fDelCumtime;			// ����Ÿ��

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
