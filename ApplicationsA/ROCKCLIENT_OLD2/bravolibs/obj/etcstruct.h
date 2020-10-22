
#ifndef __ETC_STRUCT_H__
#define __ETC_STRUCT_H__

#include	"..\\rocklibs\\RockPCH.h"
#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH
#include	"Define.H"
#include	"..\\..\\CRenderManager.h"

//#define	SHADOW_SIZE	32
#define	SHADOW_SIZE	64
//#define	SHADOW_SIZE	128

#ifdef DIRECT_VERSION_9_MJH
extern	LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern	LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

//-----------------------------------------------------------------------------
// Desc: ����
//-----------------------------------------------------------------------------
struct ALPHA_INFO
{
	bool			m_enable;
	unsigned int	m_src;
	unsigned int	m_dest;
}; 


//-----------------------------------------------------------------------------
// Desc: �ٿ��ڽ�
//-----------------------------------------------------------------------------
struct BOUND_INFO
{	
	D3DXVECTOR3		m_min_vertex;	// �ּҰ� ���ؽ�
	D3DXVECTOR3		m_max_vertex;	// �ִ밪 ���ؽ�
};


//-----------------------------------------------------------------------------
// Desc: �ִϸ��̼� �ε���
//-----------------------------------------------------------------------------
struct ANI_INFO
{
	int		ani_number;		// �ִϸ��̼� �ε���
	int		sound_number;	// ���� �ε���
	int		sound_key;		// ���尡 ������ Ű�����ӹ�ȣ

	ANI_INFO()	{ ani_number = -1; sound_number = -1; sound_key = -1; }
	~ANI_INFO() {}
};


struct ANI_INFO_ACTION
{
	int		Impact_frame;		// ���ݽ� Ÿ�ݵǴ� ������ 
	int		range_startframe;	// �޺� ���ݽ� Ÿ�̹� ��Ʈ ���� �������� 
	int		range_endframe;		// �޺� ���ݽ� Ÿ�̹� ��Ʈ ���� �������� 

	ANI_INFO_ACTION() { Impact_frame = -1; range_startframe = -1; range_endframe = -1; }
	~ANI_INFO_ACTION() {}
};

//-----------------------------------------------------------------------------
// Desc: ��ƼŬ ����ü
//-----------------------------------------------------------------------------
struct ParticleInfo
{
	int				index;	// ��ƼŬ �ε���
	int				link;	// �� ��ũ �ε���
	D3DXVECTOR3		vPos;	// ��ƼŬ ��ġ

	void Init()
	{
		index = -1;
		link  = -1;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
};


//-----------------------------------------------------------------------------
// Desc: ������ ����
//-----------------------------------------------------------------------------
struct ARMOR_INFO
{
	int				index[LINK_ITEM_COUNT];
	//by simwoosung
	int				Code[LINK_ITEM_COUNT];
	unsigned int	RHand;
	unsigned int	LHand;	
	unsigned int	Temp1;
	unsigned int	Temp2;		

	ARMOR_INFO() { Clear(); }	 
	void Clear(){ 
					RHand = 0; LHand = 0; Temp1 = 0; Temp2 = 0; 
					memset( &index, -1, sizeof(index) );
					memset( &Code, -1, sizeof(Code) ); 
				}
};


//-----------------------------------------------------------------------------
// Desc: ������ ����
//-----------------------------------------------------------------------------
struct BillboardInfo
{
	int				index;	// ������ �ε���
	int				link;	// �� ��ũ �ε���
	D3DXVECTOR3		vPos;	// ������ ��ġ

	void Init()
	{
		index = -1;
		link  = -1;
		vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	}
};


enum NSHADOW_SIZE
{
	NSHADOW_SIZE_64  = 0,
	NSHADOW_SIZE_128,
	NSHADOW_SIZE_256,
	NSHADOW_SIZE_512,
};

#define Shadow_ReNewTime 0.5f

//-----------------------------------------------------------------------------
// Desc: �׸��� ����
//-----------------------------------------------------------------------------
struct SHADOW_INFO
{
	bool				m_IsInitTex;
	bool				m_IsRender;
	bool				m_IsOneTimeRender;
	float				m_fCumulativeTime;		//�ؽ��İ� �������� �����ð��Ŀ� �簻��-�ð� ����

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9			pTex;				// �ؽ��� ������
	LPDIRECT3DSURFACE9			pSurface;			// Ÿ��
#else
	LPDIRECT3DTEXTURE8			pTex;				// �ؽ��� ������
	LPDIRECT3DSURFACE8			pSurface;			// Ÿ��
#endif // DIRECT_VERSION_9_MJH
	LPD3DXRENDERTOSURFACE		pRenderToSurface;	// ���������̽�				
	D3DXMATRIX					matShadowUV;		// U,V ��Ʈ����
	D3DXMATRIX					matShadowProj;		// ��������
	D3DXMATRIX					matWorldToLight;	// ����Ʈ��
	
	int							m_ShadowSize;
	
	float						fXMax; 
	float						fYMax;

	SHADOW_INFO()	
	{
		m_ShadowSize = 64;
		Init(); 
	}

	~SHADOW_INFO()	{ Clear(); }

	void	Init()
	{			
		pTex			 = NULL;
		pSurface		 = NULL;
		pRenderToSurface = NULL;		

		m_fCumulativeTime = 0.0f;
		m_IsRender = false;
		m_IsInitTex = false;
		m_IsOneTimeRender = false;

		fXMax = 0.0f;
		fYMax = 0.0f;
	}
	void	Clear()
	{
		SAFE_RELEASE( pRenderToSurface );
		SAFE_RELEASE( pSurface );
		SAFE_RELEASE( pTex );

		Init();
	}	

	BOOL	Create()
	{
		Clear();
		
		m_fCumulativeTime = 0.0f;
		m_IsRender = false;		
		
		if( pRenderToSurface == NULL )
		{
			if( FAILED( g_RenderManager.CreateRenderToSurface( m_ShadowSize, m_ShadowSize,  D3DFMT_A8R8G8B8,
													FALSE, D3DFMT_UNKNOWN, &pRenderToSurface ) ) )
			{
				return FALSE;
			}
		}
		
		if( NULL == pTex )
		{			
			if( FAILED( g_RenderManager.CreateTexture( m_ShadowSize, m_ShadowSize, 1, 
											D3DUSAGE_RENDERTARGET,  D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex ) ) )
			{
				return FALSE;
			}

			if( pTex )
			{
				if( FAILED( pTex->GetSurfaceLevel( 0, &pSurface ) ) )
				{
					return FALSE;
				}
			}		
		}

		m_IsInitTex = true;

		return TRUE;
	}

	BOOL	Create(int nSize)
	{
		Clear();
		
		m_fCumulativeTime = 0.0f;
		m_IsRender = false;
		m_IsOneTimeRender = false;
		
		m_ShadowSize = nSize;
		
		if( pRenderToSurface == NULL )
		{
			if( FAILED( g_RenderManager.CreateRenderToSurface( m_ShadowSize, m_ShadowSize,  D3DFMT_A8R8G8B8, FALSE, 
												   D3DFMT_UNKNOWN, &pRenderToSurface ) ) )
			{
				return FALSE;
			}
		}
		
		if( NULL == pTex )
		{			
			if( FAILED( g_RenderManager.CreateTexture( m_ShadowSize, m_ShadowSize, 1, D3DUSAGE_RENDERTARGET, 
										    D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTex ) ) )
			{
				return FALSE;
			}

			if( pTex )
			{
				if( FAILED( pTex->GetSurfaceLevel( 0, &pSurface ) ) )
				{
					return FALSE;
				}
			}
		}
		
		m_IsInitTex = true;

		return TRUE;
	}
};


//-----------------------------------------------------------------------------
// Desc: ����Ʈ ���̺�
//-----------------------------------------------------------------------------
#define		QUEST_ATTR_COUNT	5		// ����Ʈ��� ����

// ����Ʈ �Ӽ�
struct	ATTR
{	
	BYTE		Class;			// ����Ʈ ����
	DWORD		Index;			// ����Ʈ �ε���
	DWORD		Text;			// ����Ʈ �ε���
};

// ����Ʈ ����
struct	REQ
{	
	BYTE		Order;			// �������		
	BYTE		Job;			// ���� ����	
	BYTE		Level;			// ���� ����
	BYTE		Time;			// ���� �ð�
	int			Quest;			// �ʿ� ����Ʈ	
	int			OrderNpc[QUEST_ATTR_COUNT];	// ���� NPC		
	REQ()	{ Init(); }
	void	Init() { Quest = -1; memset( OrderNpc, -1, sizeof(OrderNpc) ); }
};

// ����Ʈ ���
struct	TARGET
{
	BYTE		Type[QUEST_ATTR_COUNT];
	int			Index[QUEST_ATTR_COUNT];	
	BYTE		CurCount[QUEST_ATTR_COUNT];
	BYTE		MaxCount[QUEST_ATTR_COUNT];	
	
	BYTE		Order[QUEST_ATTR_COUNT];	// ��ǥ����
	DWORD		MapID[QUEST_ATTR_COUNT];	// �� ��ȣ
	TARGET()	{ Init(); }
	void		Init() 	{ memset( Index,	-1, sizeof(Index) ); }
};


// ����Ʈ ���̺�
struct QUEST_TABLE
{
	ATTR		attr;
	REQ			req;
	TARGET		target;
};

/*
//-----------------------------------------------------------------------------
// Desc: �����ӽ�ŵ
//-----------------------------------------------------------------------------
class FrameSkip 
{ 
public: 
    inline void Clear() 
    { 
        SetFramePerSec( 60.0f ); 
        m_Timer = 0.0f; 
    } 
    inline void SetFramePerSec( float fps ) 
    { 
        m_SecPerFrame = 1.0f/fps; 
    } 
    
    // ���ϴ� �����Ӻ��� �ʹ� ������, 
    // false�� ��ȯ�ؼ� �ڵ带 ���۽�Ű�� �ʵ��� �Ѵ�. 
    // dt�� '��'���� (�и��� �ƴ�!!!) 
    inline bool Update( float dt ) 
    { 
        m_Timer += dt;       
        if( m_Timer < 0 ) 
		{
			return false;       
		}
        // �������ӿ� �ش��ϴ� �ð��� ����. 
        m_Timer -= m_SecPerFrame; 
        return true; 
    } 
    
    // Update�Ŀ� ȣ���ؼ� frame skip�� �����ؾ� �ϴ��� �˻��Ѵ�. 
    // frame skip�� �Ͼ�� �Ѵٸ� true�� ��ȯ�Ѵ�. 
    inline bool IsFrameSkip() 
    { 
        return m_Timer >= 0; 
    } 
    
    // ��������� ����/�Ҹ���. 
    FrameSkip(){ Clear(); } 
    virtual ~FrameSkip(){} 

protected: 
    float m_Timer; 
    float m_SecPerFrame; 
}; 
*/

#endif