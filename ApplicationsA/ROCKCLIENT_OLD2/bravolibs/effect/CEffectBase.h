#ifndef __CEFFECTBASE_H
#define __CEFFECTBASE_H

#include "RockPCH.h"
#include "..\\Obj\\Character.h"
#include "EffectID.h"
#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9math.h>
#include <d3d9.h>
#else
#include <d3dx8math.h>
#include <d3d8.h>
#endif // DIRECT_VERSION_9_MJH

extern long g_nowTime;

typedef enum EFFECT_STATE
{
	ES_PLAY		     = 0 ,							// ������
	ES_SELF_DESTROY									// �����
};

typedef struct s_EnchantPoint
{
	
	D3DXVECTOR3		vPos;
	float			Range;

}ENCHANT_POINT;

#define		DistanceToTimer(t,d)			(((t)*(d))/16)

// t : ��Ÿ�� ( 16 px ) �ɸ� �ð� , d : ��ü �Ÿ�
// v0���� v1���� �Ÿ��̵��ϴµ� �ð��� t��ŭ �ɸ���. ��������� 16�� �̵��ϴµ� �ɸ��� �ð�
inline DWORD PositionToTimer(DWORD t,D3DXVECTOR3 v0,D3DXVECTOR3 v1)
{
	D3DXVECTOR3		vDistance = v1 - v0;

	return DWORD ( DistanceToTimer(t,D3DXVec3Length(&vDistance)) );
}

extern D3DXVECTOR3 GetCharPos(D3DXVECTOR3 vPos, Character * pSrc);

extern long g_nowTime;

//................................................................................................................
// ����Ʈ�� �ɷ��ִ� Ÿ�� ���� ( ���̸� �� ���� )
//................................................................................................................
typedef struct s_EffectTargetInfo
{
	BYTE			TargetType;
	DWORD			CellX;
	DWORD			CellY;
	D3DXVECTOR3		vPos;

	s_EffectTargetInfo()
	{
		TargetType = n_NPC;
		CellX      = 0;
		CellY      = 0;
		
		vPos	   = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	}

	void SetNPC(void)
	{
		TargetType = n_NPC;
		CellX      = 0;
		CellY      = 0;
		
		vPos	   = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	}

	void SetPC(void)
	{
		TargetType = n_PC;
		CellX      = 0;
		CellY      = 0;
		
		vPos	   = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	}

	void SetCell(DWORD X,DWORD Y,D3DXVECTOR3 v)
	{
		TargetType = n_Cell;
		CellX      = X;
		CellY      = Y;
		
		vPos	   = v;
	}

}EFFECT_TARGET_INFO,*LPEFFECT_TARGET_INFO;

//................................................................................................................
// ������ �⺻���̿��� ���� ( ��Ÿ�� ���� )
//................................................................................................................
typedef struct s_EffectExtraInfo
{
	Character *pDest;
	DWORD	   EventType;
	BYTE       Critical;
	WORD	   SkillCode;
	DWORD	   SkillUnique;
	
	//by simwoosung
	DWORD	   AfterEffect;
	int		   DestType;	
	float	   lifeTime;
	float	   fRadian;
	long	   lStartTime;	

	struct _STargetInfo TargetInfo;

	// 1:N ���ݽ� ���
	vector  <s_EffectExtraInfo>		SubExtraInfoList;
	
	s_EffectExtraInfo()
	{
		memset( this , 0 , sizeof( s_EffectExtraInfo ) );
	
		SkillUnique = -1;		
	}

	~s_EffectExtraInfo()
	{
		pDest = NULL;

		SubExtraInfoList.clear();
	}


	void Init(Character *pChar,WORD event,BYTE critical, WORD skillcode, struct _STargetInfo *pTargetInfo)
	{
		pDest       = pChar;
		EventType   = event;		
		Critical    = critical;
		SkillCode	= skillcode;		

		memcpy( &TargetInfo , pTargetInfo , sizeof( struct _STargetInfo ) );
	}

	void SubInit(Character *pChar,WORD event,BYTE critical, WORD skillcode, struct _STargetInfo *pTargetInfo)
	{
		s_EffectExtraInfo TempExtraInfo;
		
		TempExtraInfo.pDest       = pChar;
		TempExtraInfo.EventType   = event;		
		TempExtraInfo.Critical    = critical;
		TempExtraInfo.SkillCode	  = skillcode;
		TempExtraInfo.lStartTime  = lStartTime;

		memcpy( &TempExtraInfo.TargetInfo , pTargetInfo , sizeof( struct _STargetInfo ) );
	
		SubExtraInfoList.push_back( TempExtraInfo );
	}

	void SubInit(Character *pChar,WORD event,BYTE critical, WORD skillcode, struct _STargetInfo TargetInfo)
	{
		s_EffectExtraInfo TempExtraInfo;
		
		TempExtraInfo.pDest       = pChar;
		TempExtraInfo.EventType   = event;		
		TempExtraInfo.Critical    = critical;
		TempExtraInfo.SkillCode	  = skillcode;
		TempExtraInfo.lStartTime  = lStartTime;

		memcpy( &TempExtraInfo.TargetInfo , &TargetInfo , sizeof( struct _STargetInfo ) );
	
		SubExtraInfoList.push_back( TempExtraInfo );
	}

	void operator=(const struct s_EffectExtraInfo &temp)
	{
		pDest      = temp.pDest;
		EventType  = temp.EventType;
		Critical   = temp.Critical;
		SkillCode  = temp.SkillCode;
		lStartTime = temp.lStartTime;
		
		memcpy( &TargetInfo , &temp.TargetInfo , sizeof( struct _STargetInfo ) );

		for( int i = 0 ; i < temp.SubExtraInfoList.size() ; ++i )
		{
			SubInit( temp.SubExtraInfoList[ i ].pDest     , 
				     temp.SubExtraInfoList[ i ].EventType ,
					 temp.SubExtraInfoList[ i ].Critical  ,
					 temp.SubExtraInfoList[ i ].SkillCode ,
					 temp.SubExtraInfoList[ i ].TargetInfo  );
		}
	}

	// Ÿ�� ī��Ʈ ���
	int GetTargetCount(void)
	{
		if( pDest == NULL )
			return 0;

		return ( SubExtraInfoList.size() + 1 );
	}

	int GetTargetInfo(Character **pSrc,struct s_EffectExtraInfo *pExtraInfo,int Num)
	{
		if( pDest == NULL )
			return FALSE;

		if( Num >= ( SubExtraInfoList.size() + 1 ) )
			return FALSE;

		if( Num == 0 )
		{
			*pSrc = pDest;
			
			pExtraInfo->pDest      = pDest;
			pExtraInfo->EventType  = EventType;
			pExtraInfo->Critical   = Critical;
			pExtraInfo->SkillCode  = SkillCode;
			pExtraInfo->lStartTime = lStartTime;
			
			memcpy( &pExtraInfo->TargetInfo , &TargetInfo , sizeof( struct _STargetInfo ) );

			return TRUE;
		}

		*pSrc = SubExtraInfoList[ ( Num - 1 ) ].pDest;
		
		pExtraInfo->pDest      = SubExtraInfoList[ ( Num - 1 ) ].pDest;
		pExtraInfo->EventType  = SubExtraInfoList[ ( Num - 1 ) ].EventType;
		pExtraInfo->Critical   = SubExtraInfoList[ ( Num - 1 ) ].Critical;
		pExtraInfo->SkillCode  = SubExtraInfoList[ ( Num - 1 ) ].SkillCode;
		pExtraInfo->lStartTime = SubExtraInfoList[ ( Num - 1 ) ].lStartTime;
			
		memcpy( &pExtraInfo->TargetInfo , &SubExtraInfoList[ ( Num - 1 ) ].TargetInfo , sizeof( struct _STargetInfo ) );

		return TRUE;
	}
	
	void Clear()
	{
		pDest = NULL;
		SubExtraInfoList.clear();
		
		memset( this , 0 , sizeof( s_EffectExtraInfo ) );	
		SkillUnique = -1;	
	}

}EFFECT_EXTRA_INFO,*LPEFFECT_EXTRA_INFO;


class CEffectBase
{
public:
	
	         CEffectBase();
	virtual ~CEffectBase();


public:

	virtual int		 CheckFrame(void);
	virtual int      Update(float dtime);
	virtual int		 Render(float dtime);							
	virtual int		 SortRender(float dtime);
	virtual int		 Render(int Num) { return TRUE;}
	virtual int      Setup(EFFECT_ID id,Character *pSrc,Character *pDest,LPEFFECT_EXTRA_INFO pExtraInfo);
	virtual int      SetAfterEffect(void);
	
	virtual HRESULT	 InvalidateDeviceObject();
	virtual HRESULT	 RestoreDeviceObject();

	virtual int		 RenderShadow(void)	{ return TRUE; }
	virtual void     ApplyShaowProj() {}	

	virtual void	 SetDestroy(BOOL ISDamageAct = FALSE, BOOL ISBasicAttack = FALSE);

	EFFECT_STATE     GetState(void)					{ return m_State; }
	

	Character		*GetSrcChar(void)				{ return m_pSrcChr;  }
	Character		*GetDestChar(void)				{ return m_pDestChr; }

	EFFECT_ID		&GetID(void)					{ return m_ID;		 }

	float			 GetCellCenterX(void)			{ return m_EffectTargetInfo.vPos.x;}
	float			 GetCellCenterZ(void)			{ return m_EffectTargetInfo.vPos.z;}

	BOOL			 ISLinkItem(void)				{ return m_ISLinkItem; }
	BOOL			 ISConEffect(void)				{ return m_IsConEffect;	}
	void			 SetLinkItem(void)				{ m_ISLinkItem = TRUE; }
	
	void			 GetXAndYRadian(float &fXRadian, float &fYRadian, 
									D3DXVECTOR3 vStartPos, D3DXVECTOR3 vDestPos);

	void SetNPC(void)
	{
		m_EffectTargetInfo.SetNPC();
	}

	void SetPC(void)
	{
		m_EffectTargetInfo.SetPC();
	}

	void SetCell(DWORD X,DWORD Y,D3DXVECTOR3 v)
	{
		m_EffectTargetInfo.SetCell( X , Y , v );
	}

	void SetSkillUnique(DWORD unique)
	{
		m_SkillUnique = unique;
	}

	DWORD GetSkillUnique(void)
	{
		return m_SkillUnique;
	}

	///--JAPAN_BUG_MODIFY
	DWORD GetSkillCode(void)
	{
		return m_SkillCode;
	}

	void  AddSrcAniFrameCount(void);				// Check Frame �� ���ϸ��̼� ī��Ʈ
	int   GetSrcAniCount(void);						// �ҽ��� ���ϸ��̼� ī��Ʈ
	int   GetSrcImpactFrame(void);					// �ҽ��� ����Ʈ ������

protected:

	virtual void     Init(void);
	virtual void	 Destroy(void);

protected:
	
	EFFECT_STATE			m_State;

	Character			   *m_pSrcChr;
	Character			   *m_pDestChr;

	EFFECT_ID			    m_ID;	
	DWORD					m_SkillUnique;
	///--JAPAN_BUG_MODIFY
	DWORD					m_SkillCode;

	BOOL					m_ISLinkItem;			// �����ۿ� ��ũ���ִ� ����Ʈ
	BOOL					m_IsConEffect;			
	///-- ���� ��ü�� �׾ ��� ����־�� �ϴ� ����Ʈ���� ���� 

public:

	LPEFFECT_EXTRA_INFO     m_pExtraInfo;			// �⺻�� �̿��� ���� ( Ÿ�� ���� )

public:

	EFFECT_TARGET_INFO	    m_EffectTargetInfo;
	BOOL					m_ISLandLink;			// ĳ���Ϳ� ���� �ʰ� ���� �پ��ִ� ����Ʈ

	int						m_SrcImpactFrame;
	int						m_SrcAniFrame;
	
	e_SCENE_MODE			m_SceneMode;
	
	int						m_Src_Ani_Index;
	unsigned long			m_HashCode;

	BOOL					m_ISDamageAct;			//������� ó���ؾ��ϴ��� ����
	BOOL					m_ISBasicAttack;		//�⺻��������  ���� 

	DWORD					m_dNpcRealFileCode;	
	
	BOOL					m_IsAfterSortRender;

};

#endif