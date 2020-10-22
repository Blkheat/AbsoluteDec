#include "RockPCH.h"

#include	<NETWORK\Protocol.h>
#include	"..\\quadlist.h"
#include	<BASE\\D3DBASE.H>
#include	"..\\RockClient.h"
#include	"..\\PC.h"
#include	"..\\Camera.h"
#include	"..\\Map.h"
#include	"ObjBase.H"
#include	"Character_Item.H"
#include	"Player_Manager.H"
#include	"Line_Vertex.H"
#include	<stdio.h>
#include	<assert.H>

#define		ARROW_MOVING_TIME		200.0f		// ȭ�� ���󰡴� �ð�(0.2��)
#define		ARROW_DEALAY_TIME		5000.0f		// ȭ���� Ÿ�ٿ� �����ִ� �ð�

extern		long	g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_pd3dDevice;
#else
extern		LPDIRECT3DDEVICE8	g_pd3dDevice;
#endif // DIRECT_VERSION_9_MJH


//-----------------------------------------------------------------------------
// Desc: Item �ʱ�ȭ, �ı���
//-----------------------------------------------------------------------------
void Character_Item::Clear()
{
	m_pParticle		 = NULL;
	m_pParticleInfo  = NULL;
	m_pNormalVB		 = NULL;
	m_pLineVB		 = NULL;

	m_mod_index		 = 0;	
	m_tex_index		 = 0;
	m_particle_count = 0;	

	m_sex		     = 0;
	m_race		     = 0;
	m_grav			 = 0;
	m_class			 = 0;
	m_type		     = 0;
	m_number	     = 255;	
	m_range		     = 0;
	m_tex		     = 0;
	m_AttackAniNum   = 0;
}

void Character_Item::Destroy()
{
	SAFE_DELETE_ARRAY( m_pParticle );
	SAFE_DELETE_ARRAY( m_pParticleInfo );	

	if( m_pNormalVB )	
		m_pNormalVB->Release();	
	if( m_pLineVB )	
		m_pLineVB->Release();	
	Clear();
}


//-----------------------------------------------------------------------------
// Desc: Item ���� �б�
//-----------------------------------------------------------------------------
bool Character_Item::File__Open( char* file_path )
{
	FILE*	file = NULL;
	if( NULL == (file = fopen( file_path, "rb" )) )	
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}

#ifdef GAMETYPE_TEST	
	assert( file != NULL && "!���������� �б� ����" );	
#endif
	
	fread( &m_mod_index,		sizeof(m_mod_index),		1,	file );	
	fread( &m_Material,			sizeof(m_Material),			1,	file );
	fread( &m_tex_index,		sizeof(m_tex_index),		1,	file );	
	fread( &m_particle_count,	sizeof(m_particle_count),	1,	file );

	fclose( file );
	return true;
}

//-----------------------------------------------------------------------------
// Desc: Item ���� �б�
//-----------------------------------------------------------------------------
bool	Character_Item::File__OpenVF( CVFileSystem* pVF, char* file_path )	/// sooree pack
{
	// ���������� ����
	VFileHandle* pFH = pVF->OpenFile( file_path );
	if( pFH == NULL ) 
	{
		CloseWnd_Error( "!%s ������ ã�� �� �����ϴ�.", file_path );			
		return false;
	}
	
	pFH->Read( &m_mod_index,		sizeof(m_mod_index)	);	
	pFH->Read( &m_Material,			sizeof(m_Material) );
	pFH->Read( &m_tex_index,		sizeof(m_tex_index) );	
	pFH->Read( &m_particle_count,	sizeof(m_particle_count) );

	// ���� ������ �ݱ�
	pVF->CloseFile( pFH );

	return true;
}

//-----------------------------------------------------------------------------
// Desc: Item ���ؽ� ���� ���� 
//-----------------------------------------------------------------------------
bool Character_Item::CreateVrxBuf( BYTE byType, bool bCreate )
{	
	///-- �÷��̾� ���� �������� ���...
	if( byType == SORT_PC )
	{	

		int i = 0;
		bool IsFind_Mod = false;
		bool IsFind_Tex = false;

		for( i = 0; i < g_Pc_Manager.m_mod_count ; i++ )
		{
			if( g_Pc_Manager.theChrMod[i].m_Index == m_mod_index )
			{					
				m_mod_index = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
				IsFind_Mod = true;
				break;
			}
		}
		for( i = 0; i < g_Pc_Manager.m_tex_count ; i++ )
		{
			if( g_Pc_Manager.theChrTex[i].m_Index == m_tex_index )
			{
				m_tex_index = i; /// ���ϸ����� �� �迭 �ε����� ã�� �� �ε����� ���� 
				IsFind_Tex = true;
				break;
			}
		} 

		if( IsFind_Mod == false )
		{		
			CloseWnd_Error( "!%d ���� ã�� �� �����ϴ�.", m_mod_index );			
			return false;
		}
		
		if( IsFind_Tex == false )
		{
			CloseWnd_Error( "!%d �ؽ��ĸ� ã�� �� �����ϴ�.", m_tex_index );			
			return false;
		} 

		// �޸� �Ҵ�
		if( bCreate == true )
		{
			//-----------------------------------------------------------------------------
			// Desc: ���ؽ����� ����Ÿ �ֱ�
			//-----------------------------------------------------------------------------	
			bool bFind = false;
			switch( m_class )
			{
				// Į
			case 0:			
				bFind = true;			
				break;
				// ������, Ȱ
			case 1:			
				bFind = true;			
				break;
				// �ǵ�
			case 3:			
				bFind = true;			
				break;
				// ��Ÿ(��Ÿ, Ÿ��)
			case 9:			
				bFind = true;			
				break;
			}
			
			HRESULT hr;
			OBJECTVERTEX*	pOBJVERTEX;	
			if( bFind == true )
			{					
				int nVtxCount = g_Pc_Manager.theChrMod[m_mod_index].m_header.m_num_vertex;	

				if( FAILED( g_RenderManager.CreateVertexBuffer( nVtxCount*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY,
															D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pNormalVB ) ) )
				{
					return false;		
				}
			
	
				
				// ���ؽ����� ���ɾ ���ֱ�
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pNormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pNormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}
//				hr = m_pNormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
//				assert( hr == 0 && "���ؽ� ���� �� ����" );
				for( int i = 0; i < nVtxCount; ++i )
				{
					pOBJVERTEX[i].Vertex = g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Normal = g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_normal;
					pOBJVERTEX[i].u      = g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_u;
					pOBJVERTEX[i].v      = g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_v;
				}
				hr = m_pNormalVB->Unlock();
				assert( hr == 0 && "!���ؽ� ���� ��� ����" );
			}
			
			//-----------------------------------------------------------------------------
			// Desc: �ܰ������� ����Ÿ �ֱ�
			//-----------------------------------------------------------------------------			
			bFind = true;
			if( bFind == true )
			{
				int nVtxCount = g_Pc_Manager.theChrMod[m_mod_index].m_header.m_num_vertex;	
				if( FAILED( hr = g_RenderManager.CreateVertexBuffer( nVtxCount*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
					D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pLineVB ) ) )
				{
					return false;
				}
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pLineVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pLineVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}

//				HRESULT hr = m_pLineVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 );
				for( int i = 0; i < nVtxCount; ++i )
				{
					pOBJVERTEX[i].Vertex	= g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Vertex.x += g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_normal.x*0.07f;
					pOBJVERTEX[i].Vertex.y += g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_normal.y*0.07f;
					pOBJVERTEX[i].Vertex.z += g_Pc_Manager.theChrMod[m_mod_index].m_pVertex[i].m_normal.z*0.07f;
				}
				hr = m_pLineVB->Unlock();
			}
		}
	}
	else ///-- NPC���� �������� ���...
	{	
		int vtxMod_index = m_mod_index - c_Chr_Mod_Count;

		// �޸� �Ҵ�
		if( bCreate == true )
		{
			//-----------------------------------------------------------------------------
			// Desc: ���ؽ����� ����Ÿ �ֱ�
			//-----------------------------------------------------------------------------	
			bool bFind = false;
			switch( m_class )
			{
				// Į
			case 0:			
				bFind = true;			
				break;
				// ������, Ȱ
			case 1:			
				bFind = true;			
				break;
				// �ǵ�
			case 3:			
				bFind = true;			
				break;
				// ��Ÿ(��Ÿ, Ÿ��)
			case 9:			
				bFind = true;			
				break;
			}
			
			HRESULT hr;
			OBJECTVERTEX*	pOBJVERTEX;	
			if( bFind == true )
			{		
#ifdef DYNAMIC_RESOURCE_LOADING
				int nVtxCount = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_header.m_num_vertex;
#else
				int nVtxCount = g_Pc_Manager.theNpcMod[vtxMod_index].m_header.m_num_vertex;	
#endif				
				if( FAILED( hr = g_RenderManager.CreateVertexBuffer( nVtxCount*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY,
					D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pNormalVB ) ) )
				{
					return false;
				}			
				
				// ���ؽ����� ���ɾ ���ֱ�
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pNormalVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pNormalVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}

				for( int i = 0; i < nVtxCount; ++i )
				{

#ifdef DYNAMIC_RESOURCE_LOADING					
					pOBJVERTEX[i].Vertex = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Normal = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_normal;
					pOBJVERTEX[i].u      = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_u;
					pOBJVERTEX[i].v      = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_v;
#else
					pOBJVERTEX[i].Vertex = g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Normal = g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_normal;
					pOBJVERTEX[i].u      = g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_u;
					pOBJVERTEX[i].v      = g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_v;
#endif								
				}
				hr = m_pNormalVB->Unlock();
				assert( hr == 0 && "!���ؽ� ���� ��� ����" );
			}
			
			//-----------------------------------------------------------------------------
			// Desc: �ܰ������� ����Ÿ �ֱ�
			//-----------------------------------------------------------------------------			
			bFind = true;			
			if( bFind == true )
			{
#ifdef DYNAMIC_RESOURCE_LOADING
				int nVtxCount = ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_header.m_num_vertex;
#else				
				int nVtxCount = g_Pc_Manager.theNpcMod[vtxMod_index].m_header.m_num_vertex;	
#endif
				if( FAILED( g_RenderManager.CreateVertexBuffer( nVtxCount*sizeof(OBJECTVERTEX), D3DUSAGE_WRITEONLY, 
															D3DFVF_OBJECTVERTEX, D3DPOOL_MANAGED, &m_pLineVB ) ) )
				{
					return false;		
				}
#ifdef DIRECT_VERSION_9_MJH
				if( FAILED( m_pLineVB->Lock( 0, 0, (void**)&pOBJVERTEX, 0 ) ) )
#else
				if( FAILED( m_pLineVB->Lock( 0, 0, (BYTE**)&pOBJVERTEX, 0 ) ) )
#endif // DIRECT_VERSION_9_MJH
				{
					return false;		
				}

				for( int i = 0; i < nVtxCount; ++i )
				{
#ifdef DYNAMIC_RESOURCE_LOADING
					pOBJVERTEX[i].Vertex	= ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Vertex.x += ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_normal.x*0.07f;
					pOBJVERTEX[i].Vertex.y += ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_normal.y*0.07f;
					pOBJVERTEX[i].Vertex.z += ((Model *)g_Pc_Manager.theNpcMod[vtxMod_index])->m_pVertex[i].m_normal.z*0.07f;
#else					
					pOBJVERTEX[i].Vertex	= g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_vertex;
					pOBJVERTEX[i].Vertex.x += g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_normal.x*0.07f;
					pOBJVERTEX[i].Vertex.y += g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_normal.y*0.07f;
					pOBJVERTEX[i].Vertex.z += g_Pc_Manager.theNpcMod[vtxMod_index].m_pVertex[i].m_normal.z*0.07f;
#endif	
				}

				hr = m_pLineVB->Unlock();
			}
		}
	} 

	return true;
}




//-----------------------------------------------------------------------------
// Desc: ȭ�� ������ �ʱ�ȭ
//-----------------------------------------------------------------------------
void ARROW_TABLE::Clear()
{	
	bPC			= false;
	Index		= -1;
	btDisplay	= 255;	
	btLink		= 255;
	fTargetRot	= 0.0f;
	DestTime	= g_nowTime;
	vCurtPos	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vDestPos	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXMatrixIdentity( &matWorld );
}


//-----------------------------------------------------------------------------
// Desc: ȭ�����
//-----------------------------------------------------------------------------
void ARROW_TABLE::Create( bool PC, int table_index, BYTE link, D3DXVECTOR3 vCurt, D3DXVECTOR3 vDest )
{
	//-----------------------------------------------------------------------------
	// Ÿ����������
	//-----------------------------------------------------------------------------
	bPC			= PC;
	Index		= table_index;
	btDisplay	= 0;
	btLink		= link;
	vCurtPos	= vCurt;
	vDestPos	= vDest;
	DestTime	= g_nowTime;
	fTargetRot	= 0.0f;		

	//-----------------------------------------------------------------------------
	// �Ÿ��� ���� ���󰡴� �ð����
	//-----------------------------------------------------------------------------
	float x = vCurtPos.x-vDestPos.x;
	float y = vCurtPos.y-vDestPos.y;
	float z = vCurtPos.z-vDestPos.z;
	int range;
	if( abs(x) > abs(y) )
	{
		if( abs(x) > abs(z) )
			range = (int)(x/8);
		else
			range = (int)(z/8);
	}
	else
	{
		if( abs(y) > abs(z) )
			range = (int)(y/8);
		else
			range = (int)(z/8);
	}	
	if( range >= 8 )
		DestTime += ARROW_MOVING_TIME;
	else if( range <= 1 )
		DestTime += (ARROW_MOVING_TIME/8);
	else
		DestTime += (ARROW_MOVING_TIME-(ARROW_MOVING_TIME/range));

	//-----------------------------------------------------------------------------
	// ȭ���� ���󰡴� ȸ���� ����
	//-----------------------------------------------------------------------------
	// Y���� ȸ��
	float dest_x, dest_y, fYRot, fXRot;	
	dest_x	= GetTargetPos().x-vCurtPos.x;
	dest_y	= GetTargetPos().z-vCurtPos.z;	
	fYRot	= -atan2( dest_y, dest_x )-1.57f;	
		
	// X����ȸ��
	dest_y	= vCurtPos.y-(GetTargetPos().y+vDestPos.y);
	fXRot	= -dest_y*ANGLE;

	// ȸ���� �����ϱ�
	D3DXMatrixRotationYawPitchRoll( &matWorld, fYRot, fXRot, 0.0f );
}


//-----------------------------------------------------------------------------
// Desc: Ÿ����ġ
//-----------------------------------------------------------------------------
D3DXVECTOR3	ARROW_TABLE::GetTargetPos()
{
	D3DXVECTOR3	vPos;
#ifdef DIRECT_VERSION_9_MJH
#else
	if( GetTargetPos( vPos ) == false )
		return false;
#endif // DIRECT_VERSION_9_MJH
	
	return vPos;
}


bool	ARROW_TABLE::GetTargetPos( D3DXVECTOR3& vPos )
{	
	// ����	
	if( bPC == true )
	{
		if( Index == MAX_USER_INTABLE )
		{					
			vPos.x = g_Pc.GetPlayer()->GetPosTM()._41;
			vPos.y = g_Pc.GetPlayer()->GetPosTM()._42;
			vPos.z = g_Pc.GetPlayer()->GetPosTM()._43;
		}
		else
		{
			if( g_Pc.m_Pc[Index].m_bIsLoaded == false )
			{
				Clear();
				return false;
			}
			vPos.x = g_Pc.m_Pc[Index].GetPosTM()._41;
			vPos.y = g_Pc.m_Pc[Index].GetPosTM()._42;
			vPos.z = g_Pc.m_Pc[Index].GetPosTM()._43;
		}
	}
	// NPC
	else
	{
		if( g_Map.m_Npc[Index].m_bIsLoaded == false )
		{
			Clear();
			return false;
		}
		vPos.x = g_Map.m_Npc[Index].GetPosTM()._41;
		vPos.y = g_Map.m_Npc[Index].GetPosTM()._42;
		vPos.z = g_Map.m_Npc[Index].GetPosTM()._43;
	}
	return true;
}


//-----------------------------------------------------------------------------
// Desc: Ÿ�� ȸ����
//-----------------------------------------------------------------------------
float ARROW_TABLE::GetTargetRot()
{
	// ����	
	float rot;
	if( bPC == true )
	{
		if( Index == MAX_USER_INTABLE )		
			rot = g_Pc.GetPlayer()->m_Mov.now_Yrad;		
		else
		{
			if( g_Pc.m_Pc[Index].m_bIsLoaded == false )
			{
				Clear();
				return -1.0f;
			}
			rot = g_Pc.m_Pc[Index].m_Mov.now_Yrad;
		}
	}
	// NPC
	else
	{
		if( g_Map.m_Npc[Index].m_bIsLoaded == false )
		{
			Clear();
			return -1.0f;
		}
		rot = g_Map.m_Npc[Index].m_Mov.now_Yrad;
	}
	return rot;
}


//-----------------------------------------------------------------------------
// Desc: Ÿ���� ���� ����
//-----------------------------------------------------------------------------
int	ARROW_TABLE::GetTargetEvent()
{
	int event;
	if( bPC == true )
	{
		if( Index == MAX_USER_INTABLE )		
			event = g_Pc.GetPlayer()->m_curt_event.type;
		else
			event = g_Pc.GetUser((short)Index)->m_curt_event.type;
	}
	// NPC
	else
		event = g_Map.m_Npc[Index].m_curt_event.type;
	return event;
}


//-----------------------------------------------------------------------------
// Desc: ȭ�� �׸���
//-----------------------------------------------------------------------------
void ARROW_TABLE::Display()
{
	if( btDisplay < 255 )
	{
		//-----------------------------------------------------------------------------
		// Ÿ����ġ ����
		//-----------------------------------------------------------------------------
		D3DXVECTOR3	vPos;
		if( GetTargetPos( vPos ) == false )			
			return;		
				
		//-----------------------------------------------------------------------------
		// Ÿ�Կ� ���� ó��
		//-----------------------------------------------------------------------------		
		switch( btDisplay )
		{
		//-----------------------------------------------------------------------------
		// ȭ���� ���󰡴���
		//-----------------------------------------------------------------------------
		case 0:	
			{				
				if( SAFE_TIME_COMPARE( DestTime , <= , g_nowTime ) )
				{
					Clear();
					return;
				}				
				else
				{
					// ������ġ
					float fCurt	= ( SAFE_TIME_SUB( DestTime , g_nowTime ) / ARROW_MOVING_TIME );
					vPos.x += vDestPos.x;
					vPos.y += vDestPos.y;
					vPos.z += vDestPos.z;
					vPos.x = (vPos.x+((vCurtPos.x-vPos.x)*fCurt));
					vPos.y = (vPos.y+((vCurtPos.y-vPos.y)*fCurt));
					vPos.z = (vPos.z+((vCurtPos.z-vPos.z)*fCurt));			

					// �����Ʈ����					
					matWorld._41 = vPos.x;
					matWorld._42 = vPos.y;
					matWorld._43 = vPos.z;
					g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
				}
			}
			break;
		//-----------------------------------------------------------------------------
		// ȭ���� Ÿ�ٿ� �����ִ�
		//-----------------------------------------------------------------------------
		case 1:	
			{
				//-----------------------------------------------------------------------------
				// Ÿ���� �׾��ٸ� ȭ�������
				//-----------------------------------------------------------------------------	
				if( bPC == true )
				{
					if( Index == MAX_USER_INTABLE )
					{
						if( g_Pc.GetPlayer()->m_curt_event.type == SM_STATE_DIE )
						{
							Clear();
							return;
						}
					}
					else 
					{
						if( g_Pc.m_Pc[Index].m_bIsLoaded == false )
						{
							Clear();
							return;
						}
						else if( g_Pc.m_Pc[Index].m_curt_event.type == SM_STATE_DIE )
						{
							Clear();
							return;
						}
					}
				}
				else
				{
					if( g_Map.m_Npc[Index].m_bIsLoaded == false )
					{
						Clear();
						return;
					}
					else if( g_Map.m_Npc[Index].m_curt_event.type == SM_STATE_DIE )
					{
						Clear();
						return;
					}
				}

				//-----------------------------------------------------------------------------
				// ȭ���� �����ִ� �ð� �����ٸ� ����
				//-----------------------------------------------------------------------------
				if( SAFE_TIME_COMPARE( DestTime , <= , g_nowTime ) )
				{
					Clear();
					return;
				}

				// ������ġ�� ����			
				matWorld._41 = vPos.x;
				matWorld._42 = vPos.y;
				matWorld._43 = vPos.z;

				// Ÿ�� ����� ȸ������ ���ߴٸ� ȸ����Ű��
				if( fTargetRot != GetTargetRot() )
				{				
					D3DXMATRIX	matRot;	
					D3DXMatrixRotationY( &matRot, GetTargetRot()-fTargetRot );
					D3DXMatrixMultiply( &matWorld, &matWorld, &matRot );
					fTargetRot = GetTargetRot();
				}	
				
				// ��ũ ����� �ִϸ��̼ǰ��� �����ش�
				float time;
				int num, local, type, frame;
				if( bPC == true )
				{
					if( Index == MAX_USER_INTABLE )
					{
						num		= g_Pc.GetPlayer()->m_ani_number;
						frame	= g_Pc.GetPlayer()->m_ani_frame;					
						type	= g_Pc.GetPlayer()->m_curt_event.type;
						//local	= g_Pc.GetPlayer()->m_pAniInfo[ATTACK0].ani_number;
						local	= g_Pc.GetPlayer()->m_ani_number;
						time	= g_Pc.GetPlayer()->m_middle_time;
					}
					else
					{
						num		= g_Pc.m_Pc[Index].m_ani_number;
						frame	= g_Pc.m_Pc[Index].m_ani_frame;					
						type	= g_Pc.m_Pc[Index].m_curt_event.type;
						//local	= g_Pc.m_Pc[Index].m_pAniInfo[ATTACK0].ani_number;
						local	= g_Pc.m_Pc[Index].m_ani_number;
						time	= g_Pc.m_Pc[Index].m_middle_time;
					}
				}
				else
				{
					num		= g_Map.m_Npc[Index].m_ani_number;
					frame	= g_Map.m_Npc[Index].m_ani_frame;
					type	= g_Map.m_Npc[Index].m_curt_event.type;
					//local	= g_Map.m_Npc[Index].m_pAniInfo[NPC_STANDARD0].ani_number;
					local	= g_Map.m_Npc[Index].m_ani_number;
					time	= g_Map.m_Npc[Index].m_middle_time;

				}

				// ���� ���¿� ���� ó��
				switch( type )
				{
				case SM_MOVE_BROADCAST:	// �̵���
					{
						float ani_x		= g_Pc_Manager.m_pAni[num]->m_pNode[btLink].m_pAnimation[frame].m_vPos.x;
						float ani_y		= g_Pc_Manager.m_pAni[num]->m_pNode[btLink].m_pAnimation[frame].m_vPos.y;
						float ani_z		= g_Pc_Manager.m_pAni[num]->m_pNode[btLink].m_pAnimation[frame].m_vPos.z;
						matWorld._41	= vPos.x+ani_x;
						matWorld._42	= vPos.y+ani_y;
						matWorld._43	= vPos.z+ani_z;
						g_RenderManager.SetTransform( D3DTS_WORLD, &matWorld );
					}
					break;
				case SM_ATTACK_NPC:	// �Ϲݰ���
					{
						Clear();
						return;
					}
					break;
				case SM_USE_SKILL:	// ��ų���
					{
						Clear();
						return;
					}
					break;
				default:
					{
						D3DXMATRIX	matTemp = g_Pc_Manager.NodeAnimaion( num, btLink, local, frame, time );
						D3DXMatrixMultiply( &matTemp, &matTemp, &matWorld );
						g_RenderManager.SetTransform( D3DTS_WORLD, &matTemp );
					}
					break;
				}
			}
			break;
		default:
			{
				Clear();
				return;
			}
			break;
		}
		
		//-----------------------------------------------------------------------------
		// ȭ�� ������
		//-----------------------------------------------------------------------------
		int	mod_index = g_Pc_Manager.m_pItm[ARROW_INDEX]->m_mod_index;
		DrawIndexedPrimitive(	g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_vertex,
								g_Pc_Manager.m_pMod[mod_index]->m_header.m_num_face );
	}
}


//-----------------------------------------------------------------------------
// Desc: ȭ�쿡 �ٴ� ȿ���׸���. ȭ���� ���󰥶��� �׸��� �ȴ�
//-----------------------------------------------------------------------------
void	ARROW_TABLE::LineDisplay()
{
	return;
	/*
	if( btDisplay == 0 )
	{						
		if( g_Pc_Manager.m_ArrowLine.Mod.m_pVB == NULL )
			return;

		HRESULT hr = g_Pc_Manager.m_ArrowLine.Mod.m_pVB->Lock( 0, 0, (BYTE**)&pVtx, 0 );
		assert( hr == 0 && "arrowline vtxbuffer lock fail" );
		{		
			D3DXMATRIX	matTemp	= matWorld;
			D3DXVec3TransformCoord( &pVtx[0].Vertex, &g_Pc_Manager.m_ArrowLine.Mod.m_pVertex[0].m_vertex, &matTemp );
			D3DXVec3TransformCoord( &pVtx[1].Vertex, &g_Pc_Manager.m_ArrowLine.Mod.m_pVertex[1].m_vertex, &matTemp );

			matTemp._41 = vCurtPos.x;
			matTemp._42 = vCurtPos.y;
			matTemp._43 = vCurtPos.z;
			D3DXVec3TransformCoord( &pVtx[2].Vertex, &g_Pc_Manager.m_ArrowLine.Mod.m_pVertex[2].m_vertex, &matTemp );
			D3DXVec3TransformCoord( &pVtx[3].Vertex, &g_Pc_Manager.m_ArrowLine.Mod.m_pVertex[3].m_vertex, &matTemp );
		}				
		hr = g_Pc_Manager.m_ArrowLine.Mod.m_pVB->Unlock();
		assert( hr == 0 && "arrowline vtxbuffer unlock fail" );	

		// ������	
		g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
										0, g_Pc_Manager.m_ArrowLine.Mod.m_header.m_num_vertex, 
										0, g_Pc_Manager.m_ArrowLine.Mod.m_header.m_num_face );
	}
	*/
}