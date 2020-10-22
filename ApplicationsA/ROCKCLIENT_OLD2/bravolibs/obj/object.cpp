#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"..\\RockClient.h"
#include	"..\\quadlist.h"
#include	"Object.H"
#include	<Map\\Field.h>
#include	"..\\Map.H"
#include	"..\\PC.H"
#include	"..\\Effect.H"
#include	"..\\Camera.H"
//#include	"..\\Weather_Manager.H"
#include	<SOUND\Sound.H>
#include	<stdio.h>
#include	<assert.H>
#include	<mmsystem.h>

#include "..\\global.h"

#include	"profile\profile.h"
#include	"profile\custom_time.h"

#include "..\\..\\CRenderManager.h"
#include "..\\..\\MapDataManager.h"
#include    "..\\\rocklibs\RockInterface\\EmblemTexManager.h"

extern CBravoMap			g_Map;

 
#define		SELECTOBJ_ROTTIME		100.0f		// 0.01��

extern	bool	g_bShowCollisionBox;
extern	bool	g_bBlockDebug;
				
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH
extern		int		g_obj_face;
extern		long	g_nowTime;
extern		DWORD	FloatToDWORD( float f );

void Object_AniPos::Clear() 
{	
	m_bRotate		= false;	
	m_bIsVisible	= false;
	m_btBlended		= 2;	
	m_bIsParticleAlive = false;
	m_bIsBillboardAlive	= false;

	m_scale			= 1.0f;
	m_ani_frame		= 0;
	m_middle_time	= 0.0f;
	m_skip_frame	= 0;
	m_lPreSoundTime	= g_nowTime;
	m_pre_time		= g_nowTime;
	m_lCnt			= g_nowTime;
	m_lLightTime	= g_nowTime;	
	m_LightType		= 2;		

	m_particle_count	= 0;
	m_pParticle			= NULL;
	m_pParticleInfo		= NULL;

	m_billboard_count	= 0;	
	m_pBillboard		= NULL;
	m_pBillboardInfo	= NULL;

	m_texani_count		= 0;
	m_pTexAni			= NULL;	

	D3DXMatrixIdentity( &m_matAni );	
	D3DXMatrixIdentity( &m_matPos );
	D3DXMatrixIdentity( &m_matRot );

}	


void Object_AniPos::Destroy()
{
	SAFE_DELETE_ARRAY( m_pTexAni );	
	SAFE_DELETE_ARRAY( m_pParticle );
	SAFE_DELETE_ARRAY( m_pParticleInfo );
	SAFE_DELETE_ARRAY( m_pBillboard );
	SAFE_DELETE_ARRAY( m_pBillboardInfo );
	
	Clear();
}



// ��Ʈ���� ����
void Object_AniPos::SetPosTM( const D3DXMATRIX& mat )
{
	// ��Ʈ��������
	m_matPos = mat;	

	// ������
#ifdef DIRECT_VERSION_9_MJH
	float fDet = D3DXMatrixDeterminant( &m_matPos );
#else
	float fDet = D3DXMatrixfDeterminant( &m_matPos );
#endif // DIRECT_VERSION_9_MJH
	if( fDet != 1.0f )
	{
		float i = 0.5f;
		for( i; ; i+=0.1f )
		{
			if( (i*i*i) > fDet )
				break;
		}	
		m_scale = i;
	}

	// ȸ����
	D3DXQUATERNION	qtRot;
	D3DXQuaternionRotationMatrix( &qtRot, &mat );
	
	float rotate;
	float fw = 0.0f;

	if( qtRot.w != 0.0f )
	{
		if( qtRot.w > 1.0f )
		{
			fw = qtRot.w - ( int ) qtRot.w;
		}
		else if( qtRot.w < 1.0f )
		{
			fw = qtRot.w + ( int ) qtRot.w;
		}
	}
	
	if( qtRot.y < 0 )		
		rotate = (float)( acos( fw ) * -2.0f );	
	else	
		rotate = (float)( acos( fw ) * 2.0f );	
	
	D3DXMatrixRotationY( &m_matRot, rotate );

	if( rotate != 0.0f )	
		m_bRotate = true;	
	/*
	// ��ƼŬ ���� ���
	if( m_particle_count > 0 )
	{		
		D3DXMATRIX matWorld;				

		for( unsigned int i = 0; i < m_particle_count; ++i )
		{											
			if( m_bRotate == true )
			{									
				D3DXMatrixTranslation( &matWorld, m_pParticleInfo[i].vPos.x, m_pParticleInfo[i].vPos.y, m_pParticleInfo[i].vPos.z );
				D3DXMatrixMultiply( &matWorld, &matWorld, &m_matRot );			 
				m_pParticle[i].m_vMoveLocation.x = m_matPos._41 + matWorld._41;
				m_pParticle[i].m_vMoveLocation.y = m_matPos._42 + matWorld._42;
				m_pParticle[i].m_vMoveLocation.z = m_matPos._43 + matWorld._43;
			}			
			else
			{
				m_pParticle[i].m_vMoveLocation.x = m_matPos._41;
				m_pParticle[i].m_vMoveLocation.y = m_matPos._42;
				m_pParticle[i].m_vMoveLocation.z = m_matPos._43;
				m_pParticle[i].m_vMoveLocation  += m_pParticleInfo[ i ].vPos;				
			}
		}
	}
	*/
}

//-----------------------------------------------------------------------------
// Desc: ����Ʈ ��ġ �ʱ�ȭ
//-----------------------------------------------------------------------------
void Object_AniPos::SetPosTM2( D3DXMATRIX mat )
{ 
	m_matPos = mat; 
	m_matPos._42 += 0.5f;	// ����Ÿ�Ͽ� ���� �ʵ��� �ణ ������ �Ѵ�
}
void Object_AniPos::SetPosTM3( D3DXMATRIX mat )
{ 
	m_matPos = mat; 
}

//-----------------------------------------------------------------------------
// Desc: Object �ʱ�ȭ, �ı���
//-----------------------------------------------------------------------------
void Object::Clear()
{		
	m_vLight		= D3DXVECTOR3( -15.0f, 70.0f, -25.0f );
	m_ani_index		= -1;
	m_mod_count		= 0;
	m_ani_time		= 0.0f;
	m_end_frame		= 0;	
	m_sound_index	= 0;
	m_child_count	= 1;
	m_nLodAttr		= 0;
	m_byBlendAttr	= 0;

	m_pMod_Index	= NULL;	
	m_pTex_Index	= NULL;
	m_pAni_Type		= NULL;	
	m_pAttr			= NULL;
	m_pAniPos		= NULL;	

	m_bScale		 = false;
	m_bAniPlay		 = true;
	m_bIsLoaded		 = false;
	m_bShadowDisplay = false;

	
	m_CollisionBox.m_bIsReady = false;
//#ifdef	GAMETYPE_TEST
//		m_CollisionBox.FinalCleanup();
//#endif
}

void Object::Destroy()
{	
	SAFE_DELETE_ARRAY( m_pAttr );
	SAFE_DELETE_ARRAY( m_pAniPos );
	SAFE_DELETE_ARRAY( m_pMod_Index );
	SAFE_DELETE_ARRAY( m_pTex_Index );
	SAFE_DELETE_ARRAY( m_pAni_Type );

	// �ʱ�ȭ
	Clear();
}


//-----------------------------------------------------------------------------
// Desc: Object ���� �б�
//-----------------------------------------------------------------------------
bool Object::File__Open( const char* file_path, bool bEffect )
{
	FILE*	file = NULL;
	if( NULL == (file = fopen(file_path, "rb")) )	
	{
		char strError[255] = {0,};
		sprintf( strError, "!%d.orm �ʿ� �߸��� ������Ʈ�� ����Ǿ� �ֽ��ϴ�.", g_Map.m_NowMapID );
//		MessageBox(NULL, strError, "Error", MB_OK);		
		return false;
	}
	 
	int index = 0;
	int cnt = 0;

	//-----------------------------------------------------------------------------
	// �ִ�, ��ġ ���� Ŭ���� �޸��Ҵ�
	//-----------------------------------------------------------------------------	
	m_pAniPos = SAFE_NEW_ARRAY( Object_AniPos , m_child_count );	
	D3DXVECTOR3	scale;

	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ������, ������, ���ĺ���, �ٿ�� �ڽ�
	//-----------------------------------------------------------------------------
	fread( &m_end_frame,	sizeof(m_end_frame),	1,	file );		
	fread( &m_ani_time,		sizeof(m_ani_time),		1,	file );
	fread( &scale,			sizeof(D3DXVECTOR3),	1,	file );
	fread( &m_alpha,		sizeof(m_alpha),		1,	file );
	fread( &m_Bound,		sizeof(m_Bound),		1,	file );

	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ���� �ε���, �� ����
	//-----------------------------------------------------------------------------
	//fread( &m_ani_index,	sizeof(m_ani_index),	1,	file );
	fread( &index,	sizeof(index),	1,	file );						

	for( cnt = 0; cnt < MAX_OBJ_ANICOUNT; cnt++ )
	{			 		
		if( g_Obj_Manager.m_Ani[cnt].m_Index == index )
		{
			m_ani_index = cnt; 
			break;
		}
	} 

	fread( &m_mod_count,	sizeof(m_mod_count),	1,	file );

	m_pMod_Index	= SAFE_NEW_ARRAY( int , m_mod_count );			// ���ε���				
	m_pAni_Type		= SAFE_NEW_ARRAY( int , m_mod_count );			// ��Ÿ��		
	m_pTex_Index	= SAFE_NEW_ARRAY( int , m_mod_count ); 		// �ؽ��� �ε���		
	m_pAttr			= SAFE_NEW_ARRAY( Object_Attr, m_mod_count );	// ������Ʈ �Ӽ�

	for( unsigned int i = 0; i < m_mod_count; ++i )
	{
//		fread( &m_pMod_Index[i],	sizeof(m_pMod_Index[i]),	1,	file );						

		fread( &index,	sizeof(index),	1,	file );						

		int result = -1;

		// ����Ʈ ������Ʈ ���� ó�� By wxywxy 
		if( bEffect )
		{
			m_pMod_Index[i] = ( int ) index;
			result = ( int ) index;
		}
		else
		{
			for( cnt = 0; cnt < MAX_OBJ_MODCOUNT; cnt++ )
			{					
				if( g_Obj_Manager.m_Mod[cnt].m_Index == index )
				{
					m_pMod_Index[i] = cnt; 
					result = cnt;
					break;
				}
			} 
		}

		if( result == -1 )
		{
			char strError[255];
			sprintf( strError, "!%s.obj ������ %d.msh �� ã�� ���� �����ϴ�", file_path, index );
//			MessageBox( NULL, strError, "Error", MB_OK );					
		}

		fread( &m_pAni_Type[i],		sizeof(m_pAni_Type[i]),		1,	file );									
		fread( &m_pTex_Index[i],	sizeof(m_pTex_Index[i]),	1,	file );
		fread( &m_Material,			sizeof(m_Material),			1,	file );
		fread( &m_pAttr[i],			sizeof(m_pAttr[i]),			1,	file );
	}						
	
	//-----------------------------------------------------------------------------
	// �ؽ����б�
	//-----------------------------------------------------------------------------
	unsigned int tex_count;		
	unsigned int tex_number[10] = {0,};
	fread( &tex_count, sizeof(tex_count), 1, file );		
	fread( tex_number, sizeof(unsigned int)*tex_count, 1, file );		

	//-----------------------------------------------------------------------------
	// �ؽ��ľִϸ��̼�
	//-----------------------------------------------------------------------------
	unsigned int texani_count = 0;
	fread( &texani_count, sizeof(texani_count), 1, file );
	if( texani_count > 0 )
	{
		TexAniInfo*	pTexAni = SAFE_NEW_ARRAY( TexAniInfo , texani_count );
		
		for( unsigned int i = 0; i < texani_count; ++i )
		{
			pTexAni[i].Init();
			fread( &pTexAni[i].index,  sizeof(pTexAni[i].index),  1, file );
			fread( &pTexAni[i].count,  sizeof(pTexAni[i].count),  1, file );
			fread( &pTexAni[i].number, sizeof(pTexAni[i].number), 1, file );
		}
		
		// ������Ʈ�� �ؽ��ľִϸ��̼� �ֱ�			
		for( int j = 0; j < m_child_count; ++j )
		{
			m_pAniPos[j].m_texani_count = texani_count;
			m_pAniPos[j].m_pTexAni = SAFE_NEW_ARRAY( TexAniInfo , texani_count );			
			for( unsigned int k = 0; k < texani_count; ++k )			
				m_pAniPos[j].m_pTexAni[k] = pTexAni[k];			
		}							
		
		SAFE_DELETE_ARRAY( pTexAni );			
	}	

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------		
	fread( &m_sound_time, sizeof(int), 1, file );
	if( m_sound_time > 0 )	
		fread( &m_sound_index,  sizeof(unsigned int),  1, file );

	//-----------------------------------------------------------------------------
	// ��ƼŬ
	//-----------------------------------------------------------------------------	
	unsigned int particle_count = 0;
	fread( &particle_count, sizeof(particle_count), 1, file );
	if( particle_count > 0 )
	{	
		ParticleInfo*	particle_info = SAFE_NEW_ARRAY( ParticleInfo , particle_count );		
		unsigned int i = 0;
		for(  i = 0; i < particle_count; ++i )
		{
			particle_info[i].Init();
			fread( &particle_info[i], sizeof(particle_info[i]), 1, file );
		}		
		// �� ������Ʈ�� ��ƼŬ �ֱ�
		for( i = 0; i < m_child_count; ++i )
		{
			m_pAniPos[i].m_particle_count = particle_count;
			m_pAniPos[i].m_pParticleInfo	= SAFE_NEW_ARRAY( ParticleInfo , particle_count );
			for( unsigned int j = 0; j < particle_count; ++j )
				m_pAniPos[i].m_pParticleInfo[j] = particle_info[j];	
		}
		
		SAFE_DELETE_ARRAY( particle_info );		
	}

	//-----------------------------------------------------------------------------
	// �׸��ڶ���Ʈ��ġ
	//-----------------------------------------------------------------------------
	fread( &m_vLight, sizeof(m_vLight), 1, file );

	//-----------------------------------------------------------------------------
	// ������ 
	//-----------------------------------------------------------------------------
	unsigned int billboard_count = 0;
	fread( &billboard_count, sizeof(billboard_count), 1, file );
	if( billboard_count > 0 )
	{
		BillboardInfo*	billboard_info = SAFE_NEW_ARRAY( BillboardInfo , billboard_count );	
		unsigned int i = 0;
		for(  i = 0; i < billboard_count; ++i )
		{
			billboard_info[i].Init();
			fread( &billboard_info[i], sizeof(billboard_info[i]), 1, file );
		}		
		// �� ������Ʈ�� ��ƼŬ �ֱ�
		for( i = 0; i < m_child_count; ++i )
		{
			m_pAniPos[i].m_billboard_count = billboard_count;
			m_pAniPos[i].m_pBillboardInfo	= SAFE_NEW_ARRAY( BillboardInfo , billboard_count );			
			for( unsigned int j = 0; j < billboard_count; ++j )
				m_pAniPos[i].m_pBillboardInfo[j] = billboard_info[j];	
		}
		
		SAFE_DELETE_ARRAY( billboard_info );	
	}
/*
	fread( &m_billboard_count, sizeof(m_billboard_count), 1, file );
	if( m_billboard_count > 0 )
	{
		char file_path[256] = {0,};
		for( int i = 0; i < m_billboard_count; ++i )
		{
			fread( &m_BillboardInfo[i], sizeof(m_BillboardInfo[i]), 1, file );

		}
	}
*/
	fclose( file );			

	//-----------------------------------------------------------------------------
	// �ؽ��� ���̵� ����
	//-----------------------------------------------------------------------------
	for(int i = 0; i < m_mod_count; ++i )			
	{
		index = tex_number[m_pTex_Index[i]];	
		

//		fread( &index,	sizeof(index),	1,	file );						
		
		for( cnt = 0; cnt < MAX_OBJ_TEXCOUNT; cnt++ )
		{					
			if( g_Obj_Manager.m_Tex[cnt].m_Index == index )
			{
				m_pTex_Index[i] = cnt; 
				break;
			}
		}
	}


	//-----------------------------------------------------------------------------
	// �ڽ� ������Ʈ�� �ؽ��ľִϸ��̼� ����
	//-----------------------------------------------------------------------------
	for(int i = 0; i < m_child_count; ++i )
	{			
		for( int j = 0; j < m_pAniPos[i].m_texani_count; ++j )
		{									
			for( int k = 0; k < m_pAniPos[i].m_pTexAni[j].count; ++k )			
				m_pAniPos[i].m_pTexAni[j].number[k] = tex_number[m_pAniPos[i].m_pTexAni[j].number[k]];			
		}
	}
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ���� ������ ���� 
	//-----------------------------------------------------------------------------
	if( bEffect == false && m_end_frame > 0 )
	{		
		for(int i = 0; i < m_child_count; ++i )		
			m_pAniPos[i].m_ani_frame = rand()%m_end_frame;		
	}
	// �׼Ǿִϸ��̼� �÷��� �ִٸ� �ִϸ��̼� ����
	if( GetActionAni() == true )
		m_bAniPlay = false;

	//-----------------------------------------------------------------------------
	// LOD, �����ɼ� ����
	//-----------------------------------------------------------------------------
	m_nLodAttr		= g_Obj_Manager.GetLodAttr( m_dwIndex );
	m_byBlendAttr	= g_Obj_Manager.GetBlendAttr( m_dwIndex );
	m_ISBlend		= g_Obj_Manager.GetISBlendAttr( m_dwIndex );

	//-----------------------------------------------------------------------------
	// Ÿ�Ӱ� 1/1000������ ����
	//-----------------------------------------------------------------------------
//	m_ani_time		*= 1000.0f;
	m_sound_time	*= 1000.0f;

//#ifdef	GAMETYPE_TEST
//	{	
//		m_CollisionBox.InitDeviceObjects(g_lpDevice);
//		m_CollisionBox.RestoreDeviceObjects();
//		m_CollisionBox.Update(GetBoundMinPos(),GetBoundMaxPos());
//	}
//#endif
	return m_bIsLoaded = true;
}

//-----------------------------------------------------------------------------
// Desc: Object ���� �б�
//-----------------------------------------------------------------------------
/// sooree pack
bool Object::File__OpenVF( CVFileSystem* pVF, const char* file_path, bool bEffect )
{
	VFileHandle* pFH = pVF->OpenFile( file_path );
	if( pFH == NULL )
	{
		char strError[255] = {0,};
		sprintf( strError, "!%d.orm �ʿ� �߸��� ������Ʈ %s�� ����Ǿ� �ֽ��ϴ�.", g_Map.m_NowMapID, file_path );
		MessageBox(NULL, strError, "Error", MB_OK);		
		return false;
	}
	 
	int index = 0;
	int cnt = 0;

	//-----------------------------------------------------------------------------
	// �ִ�, ��ġ ���� Ŭ���� �޸��Ҵ�
	//-----------------------------------------------------------------------------	
	m_pAniPos = SAFE_NEW_ARRAY( Object_AniPos , m_child_count );	
	D3DXVECTOR3	scale;

	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ������, ������, ���ĺ���, �ٿ�� �ڽ�
	//-----------------------------------------------------------------------------
	pFH->Read( &m_end_frame,	sizeof(m_end_frame) );		
	pFH->Read( &m_ani_time,		sizeof(m_ani_time) );
	pFH->Read( &scale,			sizeof(D3DXVECTOR3) );
	pFH->Read( &m_alpha,		sizeof(m_alpha) );
	pFH->Read( &m_Bound,		sizeof(m_Bound) );
	
	///-- ������Ʈ �ٿ�� �ڽ� �缳��
	ReSetBoundBox(m_Bound.m_min_vertex, m_Bound.m_max_vertex);

	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ���� �ε���, �� ����
	//-----------------------------------------------------------------------------
	//pFH->Read( &m_ani_index,	sizeof(m_ani_index) );
	pFH->Read( &index,	sizeof(index) );

	for( cnt = 0; cnt < MAX_OBJ_ANICOUNT; cnt++ )
	{			 		
		if( g_Obj_Manager.m_Ani[cnt].m_Index == index )
		{
			m_ani_index = cnt; 
			break;
		}
	} 

	pFH->Read( &m_mod_count,	sizeof(m_mod_count) );

	m_pMod_Index	= SAFE_NEW_ARRAY( int , m_mod_count );		// ���ε���				
	m_pAni_Type		= SAFE_NEW_ARRAY( int , m_mod_count );		// ��Ÿ��		
	m_pTex_Index	= SAFE_NEW_ARRAY( int , m_mod_count );		// �ؽ��� �ε���		
	m_pAttr			= SAFE_NEW_ARRAY( Object_Attr , m_mod_count );	// ������Ʈ �Ӽ�

	for( unsigned int i = 0; i < m_mod_count; ++i )
	{
//		pFH->Read( &m_pMod_Index[i],	sizeof(m_pMod_Index[i]) );						
		pFH->Read( &index,	sizeof(index) );						
		m_pMod_Index[i] = index;

		int result = -1;

		// ����Ʈ ������Ʈ ���� ó�� By wxywxy 
		if( bEffect )
		{
			m_pMod_Index[i] = ( int ) index;
			result = ( int ) index;
		}
		else
		{
			for( cnt = 0; cnt < MAX_OBJ_MODCOUNT; cnt++ )
			{					
				if( g_Obj_Manager.m_Mod[cnt].m_Index == index )
				{
					m_pMod_Index[i] = cnt; 
					result = cnt;
					break;
				}
			} 
		}

		if( result == -1 )
		{
			char strError[255];
			sprintf( strError, "!%s.obj ������ %d.msh �� ã�� ���� �����ϴ�", file_path, index );
			MessageBox( NULL, strError, "Error", MB_OK );					
		}

		pFH->Read( &m_pAni_Type[i],		sizeof(m_pAni_Type[i]) );									
		pFH->Read( &m_pTex_Index[i],	sizeof(m_pTex_Index[i]) );
		pFH->Read( &m_Material,			sizeof(m_Material) );
		pFH->Read( &m_pAttr[i],			sizeof(m_pAttr[i]) );
	}						
	
	//-----------------------------------------------------------------------------
	// �ؽ����б�
	//-----------------------------------------------------------------------------
	unsigned int tex_count;		
	unsigned int tex_number[126] = {0,};
	pFH->Read( &tex_count, sizeof(tex_count) );		
	pFH->Read( tex_number, sizeof(unsigned int)*tex_count );		

	//-----------------------------------------------------------------------------
	// �ؽ��ľִϸ��̼�
	//-----------------------------------------------------------------------------
	unsigned int texani_count = 0;
	pFH->Read( &texani_count, sizeof(texani_count) );
	if( texani_count > 0 )
	{
		TexAniInfo*	pTexAni = SAFE_NEW_ARRAY( TexAniInfo , texani_count );		
		for( unsigned int i = 0; i < texani_count; ++i )
		{
			pTexAni[i].Init();
			pFH->Read( &pTexAni[i].index,  sizeof(pTexAni[i].index) );
			pFH->Read( &pTexAni[i].count,  sizeof(pTexAni[i].count) );
			pFH->Read( &pTexAni[i].number, sizeof(pTexAni[i].number) );
		}
		
		// ������Ʈ�� �ؽ��ľִϸ��̼� �ֱ�			
		for( int j = 0; j < m_child_count; ++j )
		{
			m_pAniPos[j].m_texani_count = texani_count;
			m_pAniPos[j].m_pTexAni = SAFE_NEW_ARRAY( TexAniInfo , texani_count );
			for( unsigned int k = 0; k < texani_count; ++k )			
				m_pAniPos[j].m_pTexAni[k] = pTexAni[k];			
		}							
		
		SAFE_DELETE_ARRAY( pTexAni );	
	}	

	//-----------------------------------------------------------------------------
	// ����
	//-----------------------------------------------------------------------------		
	pFH->Read( &m_sound_time, sizeof(int) );
	if( m_sound_time > 0 )	
		pFH->Read( &m_sound_index,  sizeof(unsigned int) );

	//-----------------------------------------------------------------------------
	// ��ƼŬ
	//-----------------------------------------------------------------------------	
	unsigned int particle_count = 0;
	pFH->Read( &particle_count, sizeof(particle_count) );
	if( particle_count > 0 )
	{	
		ParticleInfo*	particle_info = SAFE_NEW_ARRAY( ParticleInfo , particle_count );	
		for( unsigned int i = 0; i < particle_count; ++i )
		{
			particle_info[i].Init();
			pFH->Read( &particle_info[i], sizeof(particle_info[i]) );
		}		
		// �� ������Ʈ�� ��ƼŬ �ֱ�
		for(unsigned int i = 0; i < m_child_count; ++i )
		{
			m_pAniPos[i].m_particle_count = particle_count;
			m_pAniPos[i].m_pParticleInfo = SAFE_NEW_ARRAY( ParticleInfo , particle_count );
			for( unsigned int j = 0; j < particle_count; ++j )
				m_pAniPos[i].m_pParticleInfo[j] = particle_info[j];	
		}
				
		SAFE_DELETE_ARRAY( particle_info );		
	}

	//-----------------------------------------------------------------------------
	// �׸��ڶ���Ʈ��ġ
	//-----------------------------------------------------------------------------
	pFH->Read( &m_vLight, sizeof(m_vLight) );

	//-----------------------------------------------------------------------------
	// ������ 
	//-----------------------------------------------------------------------------
	unsigned int billboard_count = 0;
	pFH->Read( &billboard_count, sizeof(billboard_count) );
	unsigned int i = 0;
	if( billboard_count > 0 )
	{
		BillboardInfo*	billboard_info = SAFE_NEW_ARRAY( BillboardInfo , billboard_count );	
		for(  i = 0; i < billboard_count; ++i )
		{
			billboard_info[i].Init();
			pFH->Read( &billboard_info[i], sizeof(billboard_info[i]) );
		}		
		// �� ������Ʈ�� ��ƼŬ �ֱ�
		for( i = 0; i < m_child_count; ++i )
		{
			m_pAniPos[i].m_billboard_count = billboard_count;
			m_pAniPos[i].m_pBillboardInfo	= SAFE_NEW_ARRAY( BillboardInfo , billboard_count );
			for( unsigned int j = 0; j < billboard_count; ++j )
				m_pAniPos[i].m_pBillboardInfo[j] = billboard_info[j];	
		}
		
		SAFE_DELETE_ARRAY( billboard_info );	
	}

	pVF->CloseFile( pFH );

	//-----------------------------------------------------------------------------
	// �ؽ��� ���̵� ����
	//-----------------------------------------------------------------------------
	for( i = 0; i < m_mod_count; ++i )			
	{
		index = tex_number[m_pTex_Index[i]];	
		for( cnt = 0; cnt < MAX_OBJ_TEXCOUNT; cnt++ )
		{					
			if( g_Obj_Manager.m_Tex[cnt].m_Index == index )
			{
				m_pTex_Index[i] = cnt; 
				break;
			}
		}
	}

	//-----------------------------------------------------------------------------
	// �ڽ� ������Ʈ�� �ؽ��ľִϸ��̼� ����
	//-----------------------------------------------------------------------------
	for( i = 0; i < m_child_count; ++i )
	{			
		for( int j = 0; j < m_pAniPos[i].m_texani_count; ++j )
		{									
			for( int k = 0; k < m_pAniPos[i].m_pTexAni[j].count; ++k )			
				m_pAniPos[i].m_pTexAni[j].number[k] = tex_number[m_pAniPos[i].m_pTexAni[j].number[k]];			
		}
	}
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ���� ������ ���� 
	//-----------------------------------------------------------------------------
	if( bEffect == false && m_end_frame > 0 )
	{		
		for( i = 0; i < m_child_count; ++i )		
			m_pAniPos[i].m_ani_frame = rand()%m_end_frame;		
	}
	// �׼Ǿִϸ��̼� �÷��� �ִٸ� �ִϸ��̼� ����
	if( GetActionAni() == true )
		m_bAniPlay = false;

	//-----------------------------------------------------------------------------
	// LOD, �����ɼ� ����
	//-----------------------------------------------------------------------------
	m_nLodAttr		= g_Obj_Manager.GetLodAttr( m_dwIndex );
	m_byBlendAttr	= g_Obj_Manager.GetBlendAttr( m_dwIndex );
	m_ISBlend		= g_Obj_Manager.GetISBlendAttr( m_dwIndex );

	//-----------------------------------------------------------------------------
	// Ÿ�Ӱ� 1/1000������ ����
	//-----------------------------------------------------------------------------
//	m_ani_time		*= 1000.0f;
	m_sound_time	*= 1000.0f;

//#ifdef	GAMETYPE_TEST
//	{	
//		m_CollisionBox.InitDeviceObjects(g_lpDevice);
//		m_CollisionBox.RestoreDeviceObjects();
//		m_CollisionBox.Update(GetBoundMinPos(),GetBoundMaxPos());
//	}
//#endif

	return m_bIsLoaded = true;
}

//-----------------------------------------------------------------------------
// Desc: �׼Ǿִϸ��̼�ó���ϴ���
//-----------------------------------------------------------------------------
bool Object::GetActionAni()
{	
	for( unsigned int i = 0; i < m_mod_count; ++i )
	{
		if( m_pAttr[i].m_bAction == true )		
			return true;		
	}
	return false;
}

long Time = 0;
int  skip_frame = 0;
//-----------------------------------------------------------------------------
// �ִϸ��̼� ������, ����
//-----------------------------------------------------------------------------
void Object::AnimationProcess( int obj )
{			
	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_pAniPos[obj].m_pre_time , m_ani_time ) ) )
	{		
		//-----------------------------------------------------------------------------
		// �ؽ��ľִϸ��̼��� Ű����������
		//-----------------------------------------------------------------------------
		for( unsigned int j = 0; j < m_pAniPos[obj].m_texani_count; ++j )
		{
			m_pAniPos[obj].m_pTexAni[j].frame++;
			if( m_pAniPos[obj].m_pTexAni[j].frame == (int)m_pAniPos[obj].m_pTexAni[j].count )			
				m_pAniPos[obj].m_pTexAni[j].frame = 0;			
		}	
	}
	
	if( m_bAniPlay == true )
	{					
			Time = SAFE_TIME_SUB( g_nowTime , m_pAniPos[obj].m_pre_time );
	
			skip_frame = (int)( ( float ) Time / ( float ) m_ani_time );
			
			m_pAniPos[obj].m_ani_frame = skip_frame;				

			if( m_pAniPos[obj].m_ani_frame > (int)m_end_frame )		
			{								
				if( GetActionAni() == true )
				{
					m_pAniPos[obj].m_ani_frame = m_end_frame;
				}
				else
				{
					m_pAniPos[obj].m_ani_frame   = 0;
					m_pAniPos[obj].m_middle_time = 0;
					m_pAniPos[obj].m_pre_time    = g_nowTime;
				}

				if( m_dwIndex == MOUSEPOINT_INDEX )
					m_pAniPos[obj].m_bIsVisible = false;										
			}
			else
			{
				m_pAniPos[obj].m_middle_time = 
					( float ) ( ( float ) Time - ( ( float ) m_pAniPos[obj].m_ani_frame * m_ani_time ) ) / m_ani_time;
			}

	}
	
	//-----------------------------------------------------------------------------
	// ���� �÷���  
	//-----------------------------------------------------------------------------
	if( m_sound_index > 0 && 
		SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_pAniPos[obj].m_lPreSoundTime , m_sound_time ) ) )
	{		
		m_pAniPos[obj].m_lPreSoundTime = g_nowTime;
		if( m_pAniPos[obj].m_bIsVisible == true )
			g_Sound.Play( m_sound_index, m_pAniPos[obj].m_matPos._41, m_pAniPos[obj].m_matPos._42, m_pAniPos[obj].m_matPos._43 );		
	}

}


//-----------------------------------------------------------------------------
// Desc: ���÷���
//-----------------------------------------------------------------------------
void	Object::BilDisplay( Object* pObj, int child )
{
	//-----------------------------------------------------------------------------
	// �ؽ��İ� �־�� �׸���
	//-----------------------------------------------------------------------------
	if( g_Obj_Manager.m_Tex[m_pTex_Index[0]].SetTexture( 2, m_pTex_Index[0] ) == true )	/// by sooree
//	if( g_Obj_Manager.GetTexturebyFileName( m_pTex_Index[0] ).SetTexture( 2, m_pTex_Index[0] ) == true )
	{
		//-----------------------------------------------------------------------------
		// ��ġ��
		//-----------------------------------------------------------------------------
		Object* pTemp = (Object*)pObj;
		float scale = pTemp->m_pAniPos[child].m_scale;
		D3DXMATRIX	matScale;
		D3DXMatrixScaling( &matScale, scale, scale, scale ); 		
		D3DXMATRIX matRot = g_Camera.GetBillboardRot();
		matRot._41 = pTemp->m_pAniPos[child].m_matPos._41;
		matRot._42 = pTemp->m_pAniPos[child].m_matPos._42;
		matRot._43 = pTemp->m_pAniPos[child].m_matPos._43;
		D3DXMatrixMultiply( &matRot, &matScale, &matRot );	

		//-----------------------------------------------------------------------------
		// ������		
		//-----------------------------------------------------------------------------
		g_RenderManager.SetCullMode( D3DCULL_NONE );
		g_RenderManager.SetMaterial( &pTemp->m_pAniPos[child].m_Material );			
		g_RenderManager.SetTransform( D3DTS_WORLD, &matRot );			
///		by sooree 
		g_RenderManager.SetIndices( g_Obj_Manager.m_Mod[m_pMod_Index[0]].m_pIB, 0 );
		g_RenderManager.SetStreamSource( 0, g_Obj_Manager.m_Mod[m_pMod_Index[0]].m_pVB, sizeof(OBJECTVERTEX) );
		DrawIndexedPrimitive(	g_Obj_Manager.m_Mod[m_pMod_Index[0]].m_header.m_num_vertex, 
								g_Obj_Manager.m_Mod[m_pMod_Index[0]].m_header.m_num_face );

	}
}

//-----------------------------------------------------------------------------
// ����Ʈ ������Ʈó��
//-----------------------------------------------------------------------------
void Object::EffectMeshAnimation( int obj, int mod )
{
	int next_frame;
	next_frame = m_pAniPos[obj].m_ani_frame+1;
	if( next_frame > (int)m_end_frame )
	{
		if( GetActionAni() == true )
			next_frame = m_end_frame;
		else
			next_frame = 0;
	}	

	D3DXQUATERNION	qtRot;
	D3DXMATRIX	matScale;
	D3DXMatrixIdentity( &matScale );

	//-----------------------------------------------------------------------------
	// ȸ��, �̵�
	//-----------------------------------------------------------------------------
	if( m_pAni_Type[mod] == 1 )
	{
		// ���ʹϿ�		
		D3DXQuaternionSlerp( &qtRot,  
							 &g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_qtRot,
							 &g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_qtRot, 
							 m_pAniPos[obj].m_middle_time );						 

		// ���ʹϿ� -> ��Ʈ����		
		D3DXMatrixRotationQuaternion( &m_pAniPos[obj].m_matAni, &qtRot );					

		// �̵���			
		m_pAniPos[obj].m_matAni._41 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.x -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x;

		m_pAniPos[obj].m_matAni._42 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.y -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y;

		m_pAniPos[obj].m_matAni._43 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.z -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z;	
	}
	//-----------------------------------------------------------------------------
	// ȸ��, �̵�, ������
	//-----------------------------------------------------------------------------
	else
	{
		// ���ʹϿ�
		D3DXQUATERNION	qtRot;
		D3DXQuaternionSlerp( &qtRot,  
							 &g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_qtRot,
							 &g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_qtRot, 
							 m_pAniPos[obj].m_middle_time );						 

		// ���ʹϿ� -> ��Ʈ����		
		D3DXMatrixRotationQuaternion( &m_pAniPos[obj].m_matAni, &qtRot );					

		// �̵���			
		m_pAniPos[obj].m_matAni._41 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vPos.x -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.x;

		m_pAniPos[obj].m_matAni._42 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vPos.y -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.y;

		m_pAniPos[obj].m_matAni._43 = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vPos.z -
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
										g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vPos.z;	

		// ������				
		D3DXVECTOR3	vScale;
		vScale.x = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vScale.x-
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.x)*m_pAniPos[obj].m_middle_time )+
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.x;

		vScale.y = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vScale.y -
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.y)*m_pAniPos[obj].m_middle_time )+
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.y;

		vScale.z = ((g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[next_frame].m_vScale.z -
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.z)*m_pAniPos[obj].m_middle_time )+
					 g_Particle.m_Mod[m_pMod_Index[mod]].m_pFullAni[m_pAniPos[obj].m_ani_frame].m_vScale.z;							
		D3DXMatrixScaling( &matScale, vScale.x, vScale.y, vScale.z ); 		
	}
	
	// ȸ���� ���ϱ�
	D3DXMATRIX	matWorld = m_pAniPos[obj].m_matPos;
	matWorld._41 = 0.0f;
	matWorld._42 = 0.0f;
	matWorld._43 = 0.0f;
	matWorld._44 = 1.0f;
	D3DXMatrixMultiply( &m_pAniPos[obj].m_matAni, &m_pAniPos[obj].m_matAni, &matWorld );	

	// ������ġ�� ���ϱ�	
	m_pAniPos[obj].m_matAni._41 += m_pAniPos[obj].m_matPos._41;
	m_pAniPos[obj].m_matAni._42 += m_pAniPos[obj].m_matPos._42;
	m_pAniPos[obj].m_matAni._43 += m_pAniPos[obj].m_matPos._43;	
	
	// �����ϰ� ���ϱ�
	D3DXMatrixMultiply( &m_pAniPos[obj].m_matAni, &matScale, &m_pAniPos[obj].m_matAni );

	// ��Ʈ���� ������ķ� ����
	g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matAni );
}


void Object::EffectRender( int obj, float alpha )
{		
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ������ó��
	//-----------------------------------------------------------------------------		
	AnimationProcess( obj );
	
	//-----------------------------------------------------------------------------
	// ���͸��� ����
	//-----------------------------------------------------------------------------
	m_Material.Diffuse.a = alpha;
	g_RenderManager.SetMaterial( &m_Material );

	//-----------------------------------------------------------------------------
	// MOD ������ŭ
	//-----------------------------------------------------------------------------	
	for( int mod = 0; mod < m_mod_count; ++mod )
	{				
		//-----------------------------------------------------------------------------
		// �޽��ִϸ��̼�
		//-----------------------------------------------------------------------------
		if( m_pAni_Type[mod] == 1 || m_pAni_Type[mod] == 3 )
			EffectMeshAnimation( obj, mod );
		//-----------------------------------------------------------------------------
		// �ִϸ��̼� ����
		//-----------------------------------------------------------------------------
		else
			g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matPos );
						
		//-----------------------------------------------------------------------------
		// ������
		//-----------------------------------------------------------------------------
		g_RenderManager.SetTexture( 0, g_Particle.m_Tex[m_pTex_Index[mod]].m_pD3DTex );
		g_RenderManager.SetIndices( g_Particle.m_Mod[m_pMod_Index[mod]].m_pIB, 0 );
		g_RenderManager.SetStreamSource( 0, g_Particle.m_Mod[m_pMod_Index[mod]].m_pVB, sizeof(OBJECTVERTEX) );
		DrawIndexedPrimitive(	g_Particle.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex, 
								g_Particle.m_Mod[m_pMod_Index[mod]].m_header.m_num_face );
	}
}


//-----------------------------------------------------------------------------
// Desc: �ڽ� ������ŭ ���÷���
//-----------------------------------------------------------------------------
void	Object::DisplayBound(int child)
{
	if( m_pAniPos[child].m_bIsVisible == true )
	{
		D3DXMATRIX matWorld;

		matWorld = m_pAniPos[child].m_matPos;
/*
		unsigned int mod		= 0;
		unsigned int mod_count	= m_mod_count;
		
		for( mod; mod < 1; ++mod )
		{

			if( m_pAni_Type[mod] == 0 )
			{

			}
			else
			{
				return;
			}
			
		}
*/
		m_CollisionBox.Render( matWorld );
	}
}

void Object::Display( int child )
{
	m_Material = g_Pc_Manager.theObject_Material;

	// �ӽ� �� ���߿� ���� By wxywxy
	//|| g_RockClient.GetGameMode() == GMODE_SELECT 
	if( m_pAniPos[child].m_bIsVisible == true )
	{
		unsigned int mod		= 0;
		unsigned int mod_count	= m_mod_count;

		//-----------------------------------------------------------------------------
		// LOD RANGE���� ���� ó��
		//-----------------------------------------------------------------------------		
/*
		if( m_nLodAttr != -1 )
		{
			// 1800�̻�, LOD �ɼ��� -1�� �ƴϸ� �ȱ׸���
			if( m_pAniPos[child].m_fDistance >= g_Obj_Manager.m_LodRange[2] )
			{
				if( m_nLodAttr != -5 )				
					return;				
			}		
			// 500�̻�( LOD �Ѵ� )
			else if( m_pAniPos[child].m_fDistance >= g_Obj_Manager.m_LodRange[0] )
			{	
				// �Ÿ��� ���� LOD
				if( m_nLodAttr == -2 || m_nLodAttr == -5 )
					mod_count = m_mod_count-1;
				// �ȱ׸���
				else if( m_nLodAttr == -3 )
					return;
				// ������
				else if( m_nLodAttr > 0 )
				{
					MaterialBlend( child );
					g_Map.DisplayBilObj( m_nLodAttr, child, this );
					return;
				}
			}
		}
*/
		switch( m_nLodAttr )
		{
			case -1:
				break;
			case -2:
			case -5:
				if( m_pAniPos[child].m_fDistance >= g_Obj_Manager.m_LodRange[0] )
				{
					mod_count = m_mod_count-1;
				}
				break;

			case -3:
				if( m_pAniPos[child].m_fDistance >= g_Obj_Manager.m_LodRange[0] ) 
				{
					return;
				}
				break;
			case -4:
				if( m_pAniPos[child].m_fDistance >= g_Obj_Manager.m_LodRange[2] )
				{
					return;
				}
				break;
			default:
				MaterialBlend( child );
				g_Map.DisplayBilObj( m_nLodAttr, child, this );
				return;
				break;
		}

//		ProfileBegin( "AnimationProcess( child )" );
		//-----------------------------------------------------------------------------
		// �ִϸ��̼� ������ó��
		//-----------------------------------------------------------------------------
		AnimationProcess( child );
//		ProfileEnd( "AnimationProcess( child )" );
		
		g_RenderManager.SetCullMode( D3DCULL_NONE );

//		ProfileBegin( "Render( child, mod )" );
		//-----------------------------------------------------------------------------
		// MOD ������ŭ...
		//-----------------------------------------------------------------------------
		for( mod; mod < mod_count; ++mod )
		{
			Render( child, mod );		
//			m_CollisionBox.Render();
		}
//		ProfileEnd( "Render( child, mod )" );
	}
}


void Object::Display()
{				
	//-----------------------------------------------------------------------------
	// �ڽ� ������ŭ
	//-----------------------------------------------------------------------------
	for( unsigned int obj = 0; obj < m_child_count; ++obj )
	{
		//-----------------------------------------------------------------------------
		// ȭ�鿡 ������ �Ѵ�(LOD -> true�̸� ������� ��ü�ؼ� �׸��� ������ ������ �Ǿ�� �Ѵ�)
		//-----------------------------------------------------------------------------		
		if( m_pAniPos[obj].m_bIsVisible == true )
		{	
			//-----------------------------------------------------------------------------
			// �ִϸ��̼� ������ó��
			//-----------------------------------------------------------------------------
			AnimationProcess( obj );
	
			//-----------------------------------------------------------------------------
			// mod������ŭ			
			//-----------------------------------------------------------------------------
			for( int i = 0; i < m_mod_count; ++i )							
				Render( obj, i );				
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: ����Ʈ ���÷���
//-----------------------------------------------------------------------------
void Object::EffectDisplayUpdate()
{
	D3DXVECTOR3	pos;

	for( unsigned int obj = 0; obj < m_child_count; ++obj )
	{
		if( m_pAniPos[obj].m_bIsVisible == true && m_pAniPos[obj].m_bIsParticleAlive == false )
		{
			// ��ƼŬ  ������
			if( m_pAniPos[obj].m_particle_count > 0 )
			{
				if( g_RockClient.GetGameMode() == GMODE_LOGINPAGE || g_RockClient.GetGameMode() == GMODE_SELECT )
					continue;

				// ĳ���� ����Ʈ ����϶� �Ÿ� ������� ��� By wxywxy
				if( g_Obj_Manager.m_LodRange[0] >= m_pAniPos[obj].m_fDistance || m_nLodAttr == -1 )
				{
					for( unsigned int i = 0; i < m_pAniPos[obj].m_particle_count; ++i )
					{
						D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pParticleInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
						g_Particle.DisplayUpdate( m_pAniPos[obj].m_pParticleInfo[i].index, pos );
					}

					m_pAniPos[obj].m_bIsParticleAlive = true;
				}
			}
		}
		else if( m_pAniPos[obj].m_bIsVisible == false && m_pAniPos[obj].m_bIsParticleAlive == true )
		{
			// ��ƼŬ  ������
			if( m_pAniPos[obj].m_particle_count > 0 )
			{				
				for( unsigned int i = 0; i < m_pAniPos[obj].m_particle_count; ++i )
				{
					D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pParticleInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
					g_Particle.StopDisplay( m_pAniPos[obj].m_pParticleInfo[i].index, pos );
				}

				m_pAniPos[obj].m_bIsParticleAlive = false;
			}
		}
		else if( m_pAniPos[obj].m_bIsVisible == true && m_pAniPos[obj].m_bIsParticleAlive == true )
		{
			// ��ƼŬ  ������
			if( m_pAniPos[obj].m_particle_count > 0 )
			{
				if( g_RockClient.GetGameMode() == GMODE_LOGINPAGE || 
					g_RockClient.GetGameMode() == GMODE_SELECT    ||
					m_nLodAttr == -1								 )
					continue;

				if( g_Obj_Manager.m_LodRange[0] <= m_pAniPos[obj].m_fDistance )
				{
					for( unsigned int i = 0; i < m_pAniPos[obj].m_particle_count; ++i )
					{
						D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pParticleInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
						g_Particle.StopDisplay( m_pAniPos[obj].m_pParticleInfo[i].index, pos );
					}

					m_pAniPos[obj].m_bIsParticleAlive = false;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: ������ ���÷���
//-----------------------------------------------------------------------------
void Object::BillboardDisplayUpdate()
{
	D3DXVECTOR3	pos;

	for( unsigned int obj = 0; obj < m_child_count; ++obj )
	{
		if( m_pAniPos[obj].m_bIsVisible == true && m_pAniPos[obj].m_bIsBillboardAlive == false )
		{
			// ������  ������
			if( m_pAniPos[obj].m_billboard_count > 0 )
			{
				if( g_Obj_Manager.m_LodRange[0] >= m_pAniPos[obj].m_fDistance ||
					( g_RockClient.GetGameMode() == GMODE_LOGINPAGE || 
					  g_RockClient.GetGameMode() == GMODE_SELECT    ||
					  m_nLodAttr == -1 )
					)
				{
					for( unsigned int i = 0; i < m_pAniPos[obj].m_billboard_count; ++i )
					{
						D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pBillboardInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
						g_Billboard.SetManager( 0.0f, m_pAniPos[obj].m_pBillboardInfo[i].index, pos );
					}
					m_pAniPos[obj].m_bIsBillboardAlive = true;
				}
			}
		}
		else if( m_pAniPos[obj].m_bIsVisible == false && m_pAniPos[obj].m_bIsBillboardAlive == true )
		{
			// ������  ������
			if( m_pAniPos[obj].m_billboard_count > 0 )
			{				
				for( unsigned int i = 0; i < m_pAniPos[obj].m_billboard_count; ++i )
				{
					D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pBillboardInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
					g_Billboard.DelManager( m_pAniPos[obj].m_pBillboardInfo[i].index, pos );
				}
				m_pAniPos[obj].m_bIsBillboardAlive = false;
			}
		}
		else if( m_pAniPos[obj].m_bIsVisible == true && m_pAniPos[obj].m_bIsBillboardAlive == true )
		{
			if( m_pAniPos[obj].m_billboard_count > 0 )
			{
				if( ( g_RockClient.GetGameMode() == GMODE_LOGINPAGE || 
					g_RockClient.GetGameMode() == GMODE_SELECT )     )
					continue;

				if( g_Obj_Manager.m_LodRange[0] <= m_pAniPos[obj].m_fDistance &&
					m_nLodAttr != -1 )
				{
					for( unsigned int i = 0; i < m_pAniPos[obj].m_billboard_count; ++i )
					{
						D3DXVec3TransformCoord( &pos, &m_pAniPos[obj].m_pBillboardInfo[i].vPos, &m_pAniPos[obj].GetPosTM() );
						g_Billboard.DelManager( m_pAniPos[obj].m_pBillboardInfo[i].index, pos );
					}
					m_pAniPos[obj].m_bIsBillboardAlive = false;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: ���İ� �ִ� ������Ʈ ���÷���
//-----------------------------------------------------------------------------
bool Object::GetAlpahEnable()
{
	/*
	for( unsigned int mod = 0; mod < m_mod_count; ++mod )
	{												
		if( g_Obj_Manager.m_Tex[m_pTex_Index[mod]].m_bAlpha == true )
			return true;
	}
	*/
	return false;
}

void Object::ReSetBoundBox(D3DXVECTOR3 & min, D3DXVECTOR3 & max)
{
	float fTemp = 0.0f;

	if(min.x > max.x)
	{
		fTemp = min.x;
		min.x = max.x;
		max.x = fTemp;
	}

	if(min.y > max.y)
	{
		fTemp = min.y;
		min.y = max.y;
		max.y = fTemp;
	}

	if(min.z > max.z)
	{
		fTemp = min.z;
		min.z = max.z;
		max.z = fTemp;
	}
}

//-----------------------------------------------------------------------------
// Desc: ���͸��� ���İ� �����ϱ�
//-----------------------------------------------------------------------------
void Object::MaterialBlend( int index )
{
	// �����Ϲݻ���
	if( g_RockClient.GetGameMode() != GMODE_NORMAL )
		return;

	//-----------------------------------------------------------------------------
	// ����ð��� ���� ���͸���� �ٲٱ�
	// attr2,3 : ������°� ���Ѵ�
	//-----------------------------------------------------------------------------			
	m_pAniPos[index].m_Material = m_Material;
	if( g_Map.m_byMapType != MS_INDOOR )
	{				
		float value = -1.0f;
		// ���͸��󺯰� 
		g_EtcTable.ChangeMaterial( &m_pAniPos[index].m_Material, value );	
	}		

	m_pAniPos[index].m_Material.Diffuse.a = 1.0f;	
	g_RenderManager.SetAlphaBlendEnable( FALSE );

	//-----------------------------------------------------------------------------
	// �Ÿ�, �þ߿����� ����ó��. 
	//-----------------------------------------------------------------------------		
	switch( m_nLodAttr )
	{
	case -1:				// ������ �׸���
		break;		
	
	case -3:				// 900 ���� 
//	case -4:				// 1800 ���� 
		{
			if( g_Obj_Manager.m_LodRange[0]-m_pAniPos[index].m_fDistance <= 80.0f )
			{
				float fValue = (g_Obj_Manager.m_LodRange[0]-m_pAniPos[index].m_fDistance)/80.0f;
				m_pAniPos[index].m_Material.Diffuse.a = fValue;					
			}	
		}
		break;
	
	case -5:				// ������� ����  
		break;		
	
	default:
		{
			//-----------------------------------------------------------------------------
			// g_Obj_Manager.m_LodRange[0] �̻�Ÿ��� �ִ� ������Ʈ��,
			// g_Obj_Manager.m_LodRange[1]~g_Obj_Manager.m_LodRange[2]���̿��� �����Ѵ�
			//-----------------------------------------------------------------------------
			bool bFind = false;
			if( m_pAniPos[index].m_fDistance >= g_Obj_Manager.m_LodRange[1] && 
					 m_pAniPos[index].m_fDistance <= g_Obj_Manager.m_LodRange[2] )
			{
				float fValue = (g_Obj_Manager.m_LodRange[2]-m_pAniPos[index].m_fDistance)/
							   (g_Obj_Manager.m_LodRange[2]-g_Obj_Manager.m_LodRange[1]);
				m_pAniPos[index].m_Material.Diffuse.a = fValue;
				bFind = true;
			}
		}
		break;
	}		


	switch( m_ISBlend )
	{
		case 1:
			//-----------------------------------------------------------------------------
			// �� ĳ���͸� ������ �ִٸ� ����ó��
			// lod �ɼǰ��� 0�̿����Ѵ�.. 1�̸� �þߺ��� ���Ѵ�
			//-----------------------------------------------------------------------------
			if( m_pAniPos[index].m_btBlended < 2 )
			{	
					if( SAFE_TIME_COMPARE( m_pAniPos[index].m_lCnt , >= , g_nowTime ) )
					{
						if( m_pAniPos[index].m_btBlended == 0 )
						{
							m_pAniPos[index].m_Material.Diffuse.a = 0.3f+( (float)SAFE_TIME_SUB( m_pAniPos[index].m_lCnt , g_nowTime ) / 500 );
						}
						else
							m_pAniPos[index].m_Material.Diffuse.a = 1.0f-( (float)SAFE_TIME_SUB( m_pAniPos[index].m_lCnt , g_nowTime ) / 500 );
					}
					else
					{
						if( m_pAniPos[index].m_btBlended == 0 )
							m_pAniPos[index].m_Material.Diffuse.a = 0.3f;
						else
							m_pAniPos[index].m_Material.Diffuse.a = 1.0f;
					}
			}

			break;
	}

	if( m_pAniPos[index].m_Material.Diffuse.a < 0.8f )
	{
		g_RenderManager.SetAlphaRef( 0 );
		g_RenderManager.SetAlphaFunc( D3DCMP_NOTEQUAL );

		g_RenderManager.SetAlphaBlendEnable( TRUE );
	}
	else
	{
		/// �ٰŸ��� ���� mesh �ȿ��� alpha sorting�� �ȵǹǷ� alpha test�θ� ����. 
		float d = g_Obj_Manager.m_LodRange[0] - m_pAniPos[index].m_fDistance;
		if( d > 0 )
		{
			int ref = (int)( d / g_Obj_Manager.m_LodRange[0] * 64.0f ) + 96;	/// 96���� 160 ���� 

			g_RenderManager.SetAlphaRef( ref );
			g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );

			g_RenderManager.SetAlphaBlendEnable( FALSE );

		}
		else
		{
			//g_RenderManager.SetAlphaRef( 0x00000020 );
			//g_RenderManager.SetAlphaFunc( D3DCMP_GREATEREQUAL );
			g_RenderManager.SetAlphaRef( 0xA0 );
			g_RenderManager.SetAlphaFunc( D3DCMP_GREATER );

			g_RenderManager.SetAlphaBlendEnable( FALSE );
		}
	}
}

//-----------------------------------------------------------------------------
// Desc: ������
//-----------------------------------------------------------------------------
void Object::Render( int obj, int mod )
{	
	//-----------------------------------------------------------------------------
	// ĳ���� ����ȭ���̶�� ������Ʈ ������
	//-----------------------------------------------------------------------------
	{		
		switch( m_pAni_Type[mod] )
		{
			// �޽� ���ϸ��̼�
			case 1:
			case 3:
				{		
					if( m_pAniPos[obj].m_fDistance <= g_Obj_Manager.m_LodRange[0] ||
						g_RockClient.GetGameMode() == GMODE_SELECT ||
						g_RockClient.GetGameMode() == GMODE_LOGINPAGE )
						MeshAnimation( obj, mod );		
					
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matAni );
					
				}		
				break;
			
			case 2:
				{
					if( m_pAniPos[obj].m_fDistance <= g_Obj_Manager.m_LodRange[0] ||
						g_RockClient.GetGameMode() == GMODE_SELECT ||
						g_RockClient.GetGameMode() == GMODE_LOGINPAGE )
					{
						if( !g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_ISOneTimeLock )
							BoneAnimation( obj, mod );			
					}
					
					g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matPos );

				}
				break;
			
			// ���ؽ� ���� �߰� By wxywxy
			case 4:
				{
					// ĳ���� ����ȭ���� �ӽ÷� LOD ���� ����
					if( m_pAniPos[obj].m_fDistance <= g_Obj_Manager.m_LodRange[0] ||
						g_RockClient.GetGameMode() == GMODE_SELECT ||
						g_RockClient.GetGameMode() == GMODE_LOGINPAGE
						)
					{
						if( !g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_ISOneTimeLock )

						VertexAnimation( obj, mod );		
					}

					g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matPos );

				}
				break;

			default:
				{
					if( m_nLodAttr == -6 )
					{
						D3DXMATRIX matRot = g_Camera.GetBillboardRot();
						matRot._41 = m_pAniPos[obj].m_matPos._41;
						matRot._42 = m_pAniPos[obj].m_matPos._42;
						matRot._43 = m_pAniPos[obj].m_matPos._43;
						g_RenderManager.SetTransform( D3DTS_WORLD, &matRot );

					}
					else
					{
						g_RenderManager.SetTransform( D3DTS_WORLD, &m_pAniPos[obj].m_matPos );

					}

				}
				break;
		}
	}
	
	//-----------------------------------------------------------------------------
	// ��/�� �Ӽ������� ���͸��� ���İ� �������� �ʱ�
	//-----------------------------------------------------------------------------
	bool bFind = false;	
			
	//-----------------------------------------------------------------------------
	// ���͸���	����(����)
	//-----------------------------------------------------------------------------
	m_pAniPos[obj].m_Material = m_Material;		
//	if( bFind == false && g_Map.m_byMapType != MS_INDOOR )	
	{
		MaterialBlend( obj );	
	}
	
	g_RenderManager.SetMaterial( &m_pAniPos[obj].m_Material );		

	g_RenderManager.SetZWriteEnable( TRUE );

	//-----------------------------------------------------------------------------
	// ������
	//-----------------------------------------------------------------------------
//	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
//	{
		//-----------------------------------------------------------------------------
		// �ؽ��ļ���(�ؽ��ľִϸ��̼��ִٸ� �ִϸ��̼������� �ؽ��� ���)
		//-----------------------------------------------------------------------------
		bFind = false;
		unsigned int j = 0;
		for(  j = 0; j < m_pAniPos[obj].m_texani_count; ++j )
		{
			if( m_pAniPos[obj].m_pTexAni[j].index == mod )
			{									
				bFind = true;
				break;
			}
		}

		int TexNum = m_pTex_Index[mod];
		if( bFind == true )
		{
			TexNum = m_pAniPos[obj].m_pTexAni[j].number[m_pAniPos[obj].m_pTexAni[j].frame];
		}
		
		///-- By simwoosung
		///--  ���� �������� ������ �ٸ� obj�� ������ ���� �ʴ´�.
		BOOL IsSameNationObj = TRUE;
		if(MAP_TYPE_COLONY == g_Map.m_uMapType)
		{
			SNationOBJ * pNationObj = g_Map.FineNationOBJPtr((WORD)m_dwIndex);
			if( pNationObj )
			{
				if(g_Map.m_nExColInfo.m_nDeffenNation == pNationObj->nNation)
				{
					IsSameNationObj = TRUE;
				}
				else
				{
					IsSameNationObj = FALSE;
				}
			}
		}		
		
		if( FALSE == IsSameNationObj )
		{
			return;
		}
		
		//-----------------------------------------------------------------------------
		// ������		
		//-----------------------------------------------------------------------------
		if( g_Obj_Manager.m_Tex[TexNum].SetTexture( 2, TexNum ) == true )	/// by sooree
		{
			m_CollisionBox.Render(m_pAniPos[obj].m_matPos);
#ifdef DIRECT_VERSION_9_MJH
			static LPDIRECT3DTEXTURE9	pEmblemTexture	= NULL;
#else
			static LPDIRECT3DTEXTURE8	pEmblemTexture	= NULL;
#endif // DIRECT_VERSION_9_MJH
			static BOOL ISMultyTexture = FALSE;
			///-- ���⼭ ������  ����� ������ ã�´�.
			BOOL IsEmblemObj = FALSE;
			///-- ���⼭ ���� ���� obj�������� ���θ� Ȯ���Ѵ�.
			if( MAP_TYPE_COLONY == g_Map.m_uMapType && g_Map.IsEmbObj((WORD)m_dwIndex) )
			{
				IsEmblemObj = TRUE;
				
				DWORD FrontierID = g_Map.m_nExColInfo.m_dDeffenFronId;			
				
				VESIONTEX *pData = g_EmblemTexManager.FindForntierVersionTex( FrontierID );					
				if( NULL != pData )
				{
					if( NULL != pData->pRenderTexture )
					{
						pEmblemTexture = pData->pRenderTexture->GetTexture();
					}
				}			
			}

			if( IsEmblemObj && NULL == pEmblemTexture )
			{
				return;		///-- ������ ��� ������ ���ų� ��ϵ��� �ʾ����� 
							///-- ������ Obj������ �׸��� �ʴ´�.
			}
			
			if( IsEmblemObj )
			{					
				if( pEmblemTexture )
				{
					g_RenderManager.SetTexture( 1 , pEmblemTexture );			
					
					g_RenderManager.SetTextureColorOP  ( 0 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureColorArg1( 0 , D3DTA_TEXTURE );
					
					g_RenderManager.SetTextureAlphaOP  ( 0 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureAlphaArg1( 0 , D3DTA_TEXTURE );
					
					g_RenderManager.SetTextureCoordIndex ( 1 , 0 );						
					
					g_RenderManager.SetTextureColorOP  ( 1 , D3DTOP_ADD );
					g_RenderManager.SetTextureColorArg1( 1 , D3DTA_CURRENT   );
					g_RenderManager.SetTextureColorArg2( 1 , D3DTA_TEXTURE   );

					g_RenderManager.SetTextureAlphaOP  ( 1 , D3DTOP_SELECTARG1 );
					g_RenderManager.SetTextureAlphaArg1( 1 , D3DTA_CURRENT );

					ISMultyTexture = TRUE;

					g_RenderManager.SetTextureMipFilter ( 1 , D3DTEXF_NONE );
					g_RenderManager.SetTextureMagFilter ( 1 , D3DTEXF_LINEAR );
					g_RenderManager.SetTextureMinFilter ( 1 , D3DTEXF_POINT );
				}
			}		

//			by sooree 
			if( m_pAttr[mod].m_bNight )
			{
				g_RenderManager.SetCullMode( D3DCULL_NONE );
				
				g_RenderManager.SetZWriteEnable( TRUE );
				g_RenderManager.SetZEnable( TRUE );
				
				g_RenderManager.SetAlphaBlendEnable( TRUE );
				g_RenderManager.SetAlphaTestEnable( FALSE );

				g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_COUNT3 );

				g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &g_Map.m_matWaterTrans );

				g_RenderManager.SetIndices( g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pIB, 0 );
				g_RenderManager.SetStreamSource( 0, g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVB, sizeof(OBJECTVERTEX) );

				g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
				g_RenderManager.SetDestBlend( D3DBLEND_ONE );
				g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );

				DrawIndexedPrimitive(	g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex,
										g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_face );
				

				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
				g_RenderManager.SetZFunc( D3DCMP_EQUAL );


				DrawIndexedPrimitive(	g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex,
										g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_face );

				g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
				g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
				g_RenderManager.SetZFunc( D3DCMP_LESSEQUAL );
				g_RenderManager.SetAlphaBlendEnable( FALSE );
				g_RenderManager.SetAlphaTestEnable( FALSE );

				g_RenderManager.SetTransform( D3DTS_TEXTURE0 , &g_Map.m_matIdentity );
				g_RenderManager.SetTextureTransFormFlags( 0 , D3DTTFF_DISABLE );
			}
			else
			{
				if( m_pAniPos[obj].m_btBlended == 0 )
				{
					g_RenderManager.SetCullMode( D3DCULL_CCW );
					g_RenderManager.SetZWriteEnable( FALSE );
				}
				else
				{
					g_RenderManager.SetCullMode( D3DCULL_NONE );
					g_RenderManager.SetZWriteEnable( TRUE );
				}
				
				g_RenderManager.SetIndices( g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pIB, 0 );
				g_RenderManager.SetStreamSource( 0, g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVB, sizeof(OBJECTVERTEX) );
				
				DrawIndexedPrimitive(	g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex,
										g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_face );

				g_RenderManager.SetZWriteEnable( TRUE );
			}

			//..........................................................................................
			// ���� ���� �ؽ�ó �����ֱ� By wxywxy
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

				ISMultyTexture = FALSE;
			}

//			by sooree
			g_obj_face += g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_face;	
		}
}

void Object::RenderBound( int obj, int mod )
{	
	//-----------------------------------------------------------------------------
	// ������
	//-----------------------------------------------------------------------------
	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		//-----------------------------------------------------------------------------
		// �ؽ��ļ���(�ؽ��ľִϸ��̼��ִٸ� �ִϸ��̼������� �ؽ��� ���)
		//-----------------------------------------------------------------------------
		bool	bFind = false;
		unsigned int j = 0;
		for( j = 0; j < m_pAniPos[obj].m_texani_count; ++j )
		{
			if( m_pAniPos[obj].m_pTexAni[j].index == mod )
			{									
				bFind = true;
				break;
			}
		}
		int TexNum = m_pTex_Index[mod];
		if( bFind == true )					
			TexNum = m_pAniPos[obj].m_pTexAni[j].number[m_pAniPos[obj].m_pTexAni[j].frame];					
			
		//-----------------------------------------------------------------------------
		// ������		
		//-----------------------------------------------------------------------------
		if( g_Obj_Manager.m_Tex[TexNum].SetTexture( 2, TexNum ) == true )	/// by sooree
//		if( g_Obj_Manager.GetTexturebyFileName( TexNum ).SetTexture( 2, TexNum ) == true )
			m_CollisionBox.Render(m_pAniPos[obj].m_matPos);
	}
}


//-----------------------------------------------------------------------------
// �� �ִϸ��̼� 
//-----------------------------------------------------------------------------
D3DXVECTOR3	g_AniVertex[3000];
D3DXVECTOR3 g_AniNormal[3000];					// ���ؽ� ���Ͽ� By wxywxy

void Object::BoneAnimation( int obj, int mod )
{	
	/// by sooree
	Model& Mod = g_Obj_Manager.m_Mod[m_pMod_Index[mod]];
//	Model& Mod = g_Obj_Manager.GetModelbyFileName( m_pMod_Index[mod] );

	/// by sooree
//	for( unsigned int i = 0; i < g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex; ++i )
	for( unsigned int i = 0; i < Mod.m_header.m_num_vertex; ++i )
	{
		// ����������
		int next_frame = m_pAniPos[obj].m_ani_frame + 1;
		if( next_frame > (int)m_end_frame )		
			next_frame = 0;		

		// ���ẻ ���̵�
		/// by sooree
//		int id = g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pNodeID[i].ID;
		int id = Mod.m_pNodeID[i].ID;

		
		// ���ʹϿ� �߰��� ���ϱ�
		D3DXQUATERNION	qtRot;		
		D3DXQuaternionSlerp( &qtRot, 
							 &g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_qtRot,
							 &g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[next_frame].m_qtRot,
							 m_pAniPos[obj].m_middle_time );		

		// ���ʹϿ� -> ��Ʈ����
		D3DXMATRIX		matTemp;
		D3DXMatrixRotationQuaternion( &matTemp, &qtRot );
						
		matTemp._41 = (( g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[next_frame].m_vPos.x -
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x;			 

		matTemp._42 = (( g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[next_frame].m_vPos.y -
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y;

		matTemp._43 = (( g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[next_frame].m_vPos.z -
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
						 g_Obj_Manager.m_Ani[m_ani_index].m_pNode[id].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z;


		// ���ؽ� x ��Ʈ���� = �ִ� ���ؽ�
		/// by sooree
//		D3DXVec3TransformCoord( &g_AniVertex[i], &g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVertex[i].m_vertex, &matTemp );
		D3DXVec3TransformCoord( &g_AniVertex[i], &Mod.m_pVertex[i].m_vertex, &matTemp );
	}			

	// ���� �� �ɱ�
	/// by sooree
//	if( g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVB != NULL )
	if( Mod.m_pVB != NULL )
	{
		OBJECTVERTEX*	pOBJVERTEX;	
		/// by sooree
//		g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );		
//		for( i = 0; i < g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_header.m_num_vertex; ++i )
//			pOBJVERTEX[i].Vertex = g_AniVertex[i]; 		
//		g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pVB->Unlock();	
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		{
			return;		
		}
//		Mod.m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );		
		for(unsigned int i = 0; i < Mod.m_header.m_num_vertex; ++i )
			pOBJVERTEX[i].Vertex = g_AniVertex[i]; 		
		Mod.m_pVB->Unlock();	

		Mod.m_ISOneTimeLock = TRUE;
	}
}

// ���ؽ� ���ϸ��̼� By wxywxy
void Object::VertexAnimation( int obj, int mod )
{
	Model& Mod = g_Obj_Manager.m_Mod[m_pMod_Index[mod]];

	for( unsigned int i = 0; i < Mod.m_header.m_num_vertex; ++i )
	{
		// ����������
		int next_frame = m_pAniPos[obj].m_ani_frame + 1;
		
		if( next_frame > (int) m_end_frame )		
			next_frame = 0;		
	
		if( &( g_AniVertex[ i ] ) == NULL )
		{
			return;
		}
		
		if( NULL == m_pAniPos || NULL == Mod.m_ppVertexList )
			return;

		if( Mod.m_ppVertexList[ m_pAniPos[obj].m_ani_frame ] == NULL || Mod.m_ppVertexList[ next_frame ] == NULL )
		{
			return;
		}

		if( &( Mod.m_ppVertexList[ m_pAniPos[obj].m_ani_frame ][ i ] ) == NULL || &( Mod.m_ppVertexList[ next_frame ][ i ] ) == NULL ) 
		{
			return;
		}
		
		if( &( m_pAniPos[obj] ) == NULL )
		{
			return;
		}

		D3DXVec3Lerp( &g_AniVertex[ i ] , 
			&Mod.m_ppVertexList[ m_pAniPos[obj].m_ani_frame ][ i ].m_vertex , 
			&Mod.m_ppVertexList[ next_frame                 ][ i ].m_vertex , m_pAniPos[obj].m_middle_time );

		D3DXVec3Lerp( &g_AniNormal[ i ] , 
			&Mod.m_ppVertexList[ m_pAniPos[obj].m_ani_frame ][ i ].m_normal , 
			&Mod.m_ppVertexList[ next_frame                 ][ i ].m_normal , m_pAniPos[obj].m_middle_time );
	}			

	// ���� �� �ɱ�

	if( Mod.m_pVB != NULL )
	{
		OBJECTVERTEX*	pOBJVERTEX;	
#ifdef DIRECT_VERSION_9_MJH
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
		if( FAILED( Mod.m_pVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
		{
			return;		
		}

		for( unsigned int i = 0; i < Mod.m_header.m_num_vertex; ++i )
		{
			pOBJVERTEX[i].Vertex = g_AniVertex[i]; 		
			pOBJVERTEX[i].Normal = g_AniNormal[i]; 		
		}
		
		Mod.m_pVB->Unlock();
		
		Mod.m_ISOneTimeLock = TRUE;
	}
}


//-----------------------------------------------------------------------------
// �޽� �ִϸ��̼�
//-----------------------------------------------------------------------------
void Object::MouseAnimation( int obj, int mod )
{
	int next_frame = m_pAniPos[obj].m_ani_frame+1;
	if( next_frame > (int)m_end_frame )
		next_frame = m_pAniPos[obj].m_ani_frame;	
	
	// ���ʹϿ�
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot,  
						 &g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_qtRot,
						 &g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_qtRot, 
						 m_pAniPos[obj].m_middle_time );
	
	// ���ʹϿ� -> ��Ʈ����		
	D3DXMatrixRotationQuaternion( &m_pAniPos[obj].m_matAni, &qtRot );					

	// �̵���			
	m_pAniPos[obj].m_matAni._41 = ((g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.x -
									g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x;

	m_pAniPos[obj].m_matAni._42 = ((g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.y -
								    g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y;

	m_pAniPos[obj].m_matAni._43 = ((g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.z -
									g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_PointMod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z;	

	// ������ġ�� ���ϱ�	
	m_pAniPos[obj].m_matAni._41 += m_pAniPos[obj].m_matPos._41;
	m_pAniPos[obj].m_matAni._42 += m_pAniPos[obj].m_matPos._42;
	m_pAniPos[obj].m_matAni._43 += m_pAniPos[obj].m_matPos._43;	
}


void Object::MeshAnimation( int obj, int mod )
{
	int next_frame;

	// ������ ĳ���ͼ��ýÿ� �����߿��� ���� �ٸ���ó��
	if( m_dwIndex == CASTTLE_DOOR && g_RockClient.GetGameMode() == GMODE_NORMAL )
	{		
		m_pAniPos[obj].m_ani_frame = m_end_frame;	
		next_frame = m_end_frame;		
	}
	else
	{		
		next_frame = m_pAniPos[obj].m_ani_frame+1;
		if( next_frame > (int)m_end_frame )
		{
			if( GetActionAni() == true )
				next_frame = m_end_frame;
			else
				next_frame = 0;
		}
	}	
	
	/// by sooree ------------------------------------------
	Model& Mod = g_Obj_Manager.m_Mod[m_pMod_Index[mod]];
//	Model& Mod = g_Obj_Manager.GetModelbyFileName( m_pMod_Index[mod] );
	
/*
	// ���ʹϿ�
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot,  
						 &g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_qtRot,
						 &g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_qtRot, 
						 m_pAniPos[obj].m_middle_time );						 

	// ���ʹϿ� -> ��Ʈ����		
	D3DXMatrixRotationQuaternion( &m_pAniPos[obj].m_matAni, &qtRot );					

	// �̵���			
	m_pAniPos[obj].m_matAni._41 = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.x -
									g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x;

	m_pAniPos[obj].m_matAni._42 = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.y -
								    g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y;

	m_pAniPos[obj].m_matAni._43 = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vPos.z -
									g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
									g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z;	
*/	
	// ���ʹϿ�
	D3DXQUATERNION	qtRot;
	D3DXQuaternionSlerp( &qtRot,  
						 &Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_qtRot,
						 &Mod.m_pAnimation[next_frame].m_qtRot, 
						 m_pAniPos[obj].m_middle_time );						 

	// ���ʹϿ� -> ��Ʈ����		
	D3DXMatrixRotationQuaternion( &m_pAniPos[obj].m_matAni, &qtRot );					

	// �̵���			
	m_pAniPos[obj].m_matAni._41 = ((Mod.m_pAnimation[next_frame].m_vPos.x -
									Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x ) * m_pAniPos[obj].m_middle_time ) +
									Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.x;

	m_pAniPos[obj].m_matAni._42 = ((Mod.m_pAnimation[next_frame].m_vPos.y -
								    Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y ) * m_pAniPos[obj].m_middle_time ) +
									Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.y;

	m_pAniPos[obj].m_matAni._43 = ((Mod.m_pAnimation[next_frame].m_vPos.z -
									Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z ) * m_pAniPos[obj].m_middle_time ) +
									Mod.m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vPos.z;	

	/// ------------------------------------------ by sooree 
	
	/*
	// ������
	D3DXMATRIX	matScale;
	D3DXMatrixIdentity( &matScale );
	if( m_pAni_Type[m_pMod_Index[mod]] == 3 )
	{
		D3DXVECTOR3	vScale;
		vScale.x = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vScale.x-
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.x)*m_pAniPos[obj].m_middle_time )+
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.x;

		vScale.y = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vScale.y -
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.y)*m_pAniPos[obj].m_middle_time )+
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.y;

		vScale.z = ((g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[next_frame].m_vScale.z -
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.z)*m_pAniPos[obj].m_middle_time )+
					 g_Obj_Manager.m_Mod[m_pMod_Index[mod]].m_pAnimation[m_pAniPos[obj].m_ani_frame].m_vScale.z;							
		D3DXMatrixScaling( &matScale, vScale.x, vScale.y, vScale.z ); 
	}
	*/
		
	// ������Ʈ�ִϸ��̼� -> ������Ʈ�� ȸ���Ǿ� �ִ� ���
	if( m_pAniPos[obj].m_bRotate == true && m_mod_count > 1 )
	{					
		D3DXMATRIX	matRot;			
		D3DXMATRIX	matWorld;
		D3DXMatrixTranslation( &matWorld, m_pAniPos[obj].m_matAni._41, m_pAniPos[obj].m_matAni._42, m_pAniPos[obj].m_matAni._43 );		
		D3DXMatrixMultiply( &matWorld, &matWorld, &m_pAniPos[obj].m_matRot );
		D3DXMatrixRotationQuaternion( &matRot, &qtRot );
		D3DXMatrixMultiply( &matWorld, &matRot, &matWorld );
		m_pAniPos[obj].m_matAni = matWorld;
	}

/*
	// ������ġ�� ���ϱ�	
	m_pAniPos[obj].m_matAni._41 += m_pAniPos[obj].m_matPos._41;
	m_pAniPos[obj].m_matAni._42 += m_pAniPos[obj].m_matPos._42;
	m_pAniPos[obj].m_matAni._43 += m_pAniPos[obj].m_matPos._43;	
	
	// �����ϰ� ���ϱ�
	//D3DXMatrixMultiply( &m_pAniPos[obj].m_matAni, &matScale, &m_pAniPos[obj].m_matAni );
*/

	// ���� By wxywxy
	D3DXMatrixMultiply( &m_pAniPos[obj].m_matAni, &m_pAniPos[obj].m_matAni , &m_pAniPos[obj].m_matPos );
}


bool Object::AlphaProcess()
{	
	if( m_alpha.m_enable == true )
	{
		g_RenderManager.SetAlphaBlendEnable( TRUE );
	}
	else
	{
		g_RenderManager.SetAlphaBlendEnable( FALSE );

		return false;
	}

	switch( m_alpha.m_src )
	{
		case 1:
			g_RenderManager.SetSrcBlend( D3DBLEND_ZERO );
			break;
		case 2:
			g_RenderManager.SetSrcBlend( D3DBLEND_ONE );
			break;
		case 3:
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCCOLOR );
			break;
		case 4:
			g_RenderManager.SetSrcBlend( D3DBLEND_INVSRCCOLOR );
			break;
		case 5:
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHA );
			break;
		case 6:
			g_RenderManager.SetSrcBlend( D3DBLEND_INVSRCALPHA );
			break;
		case 7:
			g_RenderManager.SetSrcBlend( D3DBLEND_DESTALPHA );
			break;
		case 8:
			g_RenderManager.SetSrcBlend( D3DBLEND_INVDESTALPHA );
			break;
		case 9:
			g_RenderManager.SetSrcBlend( D3DBLEND_DESTCOLOR );
			break;
		case 10:
			g_RenderManager.SetSrcBlend( D3DBLEND_INVDESTCOLOR );
			break;
		case 11:
			g_RenderManager.SetSrcBlend( D3DBLEND_SRCALPHASAT );
			break;
		case 12:
			g_RenderManager.SetSrcBlend( D3DBLEND_BOTHSRCALPHA );
			break;
		case 13:
			g_RenderManager.SetSrcBlend( D3DBLEND_BOTHINVSRCALPHA );
			break;
	}

	switch( m_alpha.m_dest )
	{
		case 1:
			g_RenderManager.SetDestBlend( D3DBLEND_ZERO );
			break;
		case 2:
			g_RenderManager.SetDestBlend( D3DBLEND_ONE );
			break;
		case 3:
			g_RenderManager.SetDestBlend( D3DBLEND_SRCCOLOR );
			break;
		case 4:
			g_RenderManager.SetDestBlend( D3DBLEND_INVSRCCOLOR );
			break;
		case 5:
			g_RenderManager.SetDestBlend( D3DBLEND_SRCALPHA );
			break;
		case 6:
			g_RenderManager.SetDestBlend( D3DBLEND_INVSRCALPHA );
			break;
		case 7:
			g_RenderManager.SetDestBlend( D3DBLEND_DESTALPHA );
			break;
		case 8:
			g_RenderManager.SetDestBlend( D3DBLEND_INVDESTALPHA );
			break;
		case 9:
			g_RenderManager.SetDestBlend( D3DBLEND_DESTCOLOR );
			break;
		case 10:
			g_RenderManager.SetDestBlend( D3DBLEND_INVDESTCOLOR );
			break;
		case 11:
			g_RenderManager.SetDestBlend( D3DBLEND_SRCALPHASAT );
			break;
		case 12:
			g_RenderManager.SetDestBlend( D3DBLEND_BOTHSRCALPHA );
			break;
		case 13:
			g_RenderManager.SetDestBlend( D3DBLEND_BOTHINVSRCALPHA );
			break;
	}

	return true;
}


void Object::ShadowMake()
{

	for( unsigned int obj = 0; obj < m_child_count; ++obj )
	{
		// �׸��� �ؽ��� ����
		m_pAniPos[obj].m_Shadow.Create();

		// ����Ʈ���� �� ���Ʈ����
		D3DXMATRIX	matLocalToLight;
		D3DXMATRIX	matShadow;
		D3DXVECTOR4	VOut;
		float fXMax = 0.0f; 
		float fYMax = 0.0f;

		D3DXMatrixLookAtLH( &m_pAniPos[obj].m_Shadow.matWorldToLight, 
							&D3DXVECTOR3(m_pAniPos[obj].m_matPos._41+m_Bound.m_min_vertex.x, 
							m_pAniPos[obj].m_matPos._42+(m_Bound.m_max_vertex.y*2), 
							m_pAniPos[obj].m_matPos._43+m_Bound.m_max_vertex.z),

							&D3DXVECTOR3(m_pAniPos[obj].m_matPos._41, 
							m_pAniPos[obj].m_matPos._42, 
							m_pAniPos[obj].m_matPos._43), 
							&D3DXVECTOR3(0.0f, 1.0f, 0.0f) );	
		D3DXMatrixMultiply( &matLocalToLight, &m_pAniPos[obj].m_matPos, &m_pAniPos[obj].m_Shadow.matWorldToLight );

		// ��������
		D3DXMatrixPerspectiveFovLH( &m_pAniPos[obj].m_Shadow.matShadowProj, D3DX_PI/4, 1.3f, 1.0f, 1000.0f );								
		for( int i = 0; i < (int)m_mod_count; ++i )
		{
			for( int j = 0; j < g_Obj_Manager.GetVertexCount( m_pMod_Index[i] ); ++j )
			{									
				D3DXVec3Transform( &VOut, &g_Obj_Manager.GetVertex( m_pMod_Index[i], j ), &matLocalToLight );			
				
				if( (float)fabs(VOut.x/VOut.z) > fXMax ) 
					fXMax = (float)fabs(VOut.x/VOut.z);
				if( (float)fabs(VOut.y/VOut.z) > fYMax )
					fYMax = (float)fabs(VOut.y/VOut.z);
			}
		}		
		
		// 32 = 92, 64 = 96, 128 = 98
		if( SHADOW_SIZE == 32 )
		{
			m_pAniPos[obj].m_Shadow.matShadowProj._11 = 0.92f/fXMax;
			m_pAniPos[obj].m_Shadow.matShadowProj._22 = 0.92f/fYMax;	
		}
		else if( SHADOW_SIZE == 64 )
		{
			m_pAniPos[obj].m_Shadow.matShadowProj._11 = 0.96f/fXMax;
			m_pAniPos[obj].m_Shadow.matShadowProj._22 = 0.96f/fYMax;	
		}
		else
		{
			m_pAniPos[obj].m_Shadow.matShadowProj._11 = 0.98f/fXMax;
			m_pAniPos[obj].m_Shadow.matShadowProj._22 = 0.98f/fYMax;
		}	

		// �׸��� 
		D3DXMatrixMultiply( &matShadow, &matLocalToLight, &m_pAniPos[obj].m_Shadow.matShadowProj );	

		// �׸��� ������Ÿ���� Ŭ����				
		if( SUCCEEDED(m_pAniPos[obj].m_Shadow.pRenderToSurface->BeginScene(m_pAniPos[obj].m_Shadow.pSurface,NULL)) )
		{					
			g_RenderManager.Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,255,255,255), 0, 0 );
			g_RenderManager.SetTransform( D3DTS_PROJECTION,	&matShadow );			
			for( int i = 0; i < (int)m_mod_count; ++i )
			{			
				g_RenderManager.SetIndices( g_Obj_Manager.m_Mod[m_pMod_Index[i]].m_pIB, 0 );
				g_RenderManager.SetStreamSource( 0, g_Obj_Manager.m_Mod[m_pMod_Index[i]].m_pVB, sizeof(OBJECTVERTEX) );
				DrawIndexedPrimitive(	g_Obj_Manager.m_Mod[m_pMod_Index[i]].m_header.m_num_vertex, 
										g_Obj_Manager.m_Mod[m_pMod_Index[i]].m_header.m_num_face );
			}	
#ifdef DIRECT_VERSION_9_MJH
			m_pAniPos[obj].m_Shadow.pRenderToSurface->EndScene( D3DX_FILTER_LINEAR );
#else
			m_pAniPos[obj].m_Shadow.pRenderToSurface->EndScene();
#endif // DIRECT_VERSION_9_MJH
			g_Camera.ReloadProjTM();
		}	
	}
	m_bShadowDisplay = true;
	
}

void Object::ShadowDisplay()
{
	
	// �׸��� ������
	if( m_bShadowDisplay == true )
	{		
		for( unsigned int obj = 0; obj < m_child_count; ++obj )
		{
			if( m_pAniPos[obj].m_bIsVisible == true )
			{
				D3DXMATRIX	offset;					
				D3DXMATRIX	matCameraToWorld;
				D3DXMatrixInverse( &matCameraToWorld, NULL, &g_Camera.m_matView );
				D3DXMatrixIdentity( &offset );
				offset._11 =  0.5f;
				offset._22 = -0.5f;
				offset._31 =  0.5f;
				offset._32 =  0.5f;
				
				D3DXMatrixMultiply( &m_pAniPos[obj].m_Shadow.matShadowUV, 
									&matCameraToWorld, &m_pAniPos[obj].m_Shadow.matWorldToLight );
				D3DXMatrixMultiply( &m_pAniPos[obj].m_Shadow.matShadowUV, 
									&m_pAniPos[obj].m_Shadow.matShadowUV, &m_pAniPos[obj].m_Shadow.matShadowProj );
				D3DXMatrixMultiply( &m_pAniPos[obj].m_Shadow.matShadowUV,
									&m_pAniPos[obj].m_Shadow.matShadowUV, &offset );

				// �׸���				
				g_RenderManager.SetTexture( 1, m_pAniPos[obj].m_Shadow.pTex );
				g_RenderManager.SetTransform( D3DTS_TEXTURE1, &m_pAniPos[obj].m_Shadow.matShadowUV );
				g_Map.DisplayShadowTile( m_pAniPos[obj].m_matPos, m_Bound.m_min_vertex, m_Bound.m_max_vertex );
			}
		}
	}
	
}