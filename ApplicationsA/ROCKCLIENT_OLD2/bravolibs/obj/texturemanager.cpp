#include "RockPCH.h"

#include	"TextureManager.H"
#include	"Player_Manager.H"
#include	"Object_Manager.H"
#include	<Base\\D3DBase.h>
#include	"..\\RockClient.h"
#include	<SOUND\\sOUND.h>

#include "..\\global.h"


extern		long				g_nowTime;
#ifdef DIRECT_VERSION_9_MJH
extern		LPDIRECT3DDEVICE9	g_lpDevice;
#else
extern		LPDIRECT3DDEVICE8	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH

TEXTURE_MANAGER		g_TexManager;


const WORD c_DeleteTex_Update_Time( 30 );
const WORD c_CreateTex_Update_Time( 50 );


// �ؽ��� �ǽð� ���� 
// �������� �����ʰ� �ִ� �ؽ��ĵ��� �����ش� (CHR, NPC, OBJ)
void TEXTURE_MANAGER::DeleteNotUsedTex()
{
	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_lPreTime , c_DeleteTex_Update_Time ) ) )
	{
		m_lPreTime = g_nowTime;
				
		if( g_Pc_Manager.m_pTex[theNpcDeleteCnt] )
		{
			g_Pc_Manager.m_pTex[theNpcDeleteCnt]->UpdateTextureUse();					
		}		
		if( g_Pc_Manager.m_pTex[theChrDeleteCnt] )
		{
			g_Pc_Manager.m_pTex[theChrDeleteCnt]->UpdateTextureUse();				
		}
		
		g_Obj_Manager.m_Tex[theObjDeleteCnt].UpdateTextureUse();
		
		if( ++theChrDeleteCnt >= c_Chr_Tex_Count )
		{
			theChrDeleteCnt = 0;
		}
		if( ++theNpcDeleteCnt >= c_Chr_Tex_Count+c_Npc_Tex_Count )
		{
			theNpcDeleteCnt = c_Chr_Tex_Count;
		}
		if( ++theObjDeleteCnt >= MAX_OBJ_TEXCOUNT )
		{
			theObjDeleteCnt = 0;
		}		
	}			
	
	return;
}

void TEXTURE_MANAGER::DeleteAllChrAndNpcTex()
{
	for(int i = 0; i < c_Chr_Tex_Count+c_Npc_Tex_Count ; i++)
	{
		if( g_Pc_Manager.m_pTex[i] )
		{
			g_Pc_Manager.m_pTex[i]->Destory();
		}
	}
}

void TEXTURE_MANAGER::CreateUseTex()
{

	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_lPreCreTexTime , 250 ) ) )
	{
		m_lPreCreTexTime = g_nowTime;

		int num = g_TexManager.theCreateTex.size();
		if( num > 0 )
		{		
			// �Ѳ����� ����� �������ϱ� �� �����Ӵ� �ϳ��� ����� 
			std::list <DATA_CREATE_ATTR *>::iterator i = g_TexManager.theCreateTex.begin();
			
			Texture* pTex = NULL;
			DATA_CREATE_ATTR * pData = (*i);
			
			switch( pData->byType )
			{
			case 0:				// NPC
			case 1:				// CHR
				pTex = g_Pc_Manager.m_pTex[pData->Index];
				break;
			case 2:				// OBJ
				pTex = &g_Obj_Manager.m_Tex[pData->Index];
				break;
			default:						
				break;
			}

			if( pTex )
			{
				pTex->UpdateTexture( pData->byType, pData->Index );
			}

			SAFE_DELETE(pData);
			(*i) = NULL;
		
			g_TexManager.theCreateTex.erase( i++ );
			
		}
	}
	
	return;
}




//-----------------------------------------------------------------------------
// Desc: �ؽ��İ˻� ������
//
// ������ �ؽ��ĸ� �˻��ϴ� �Լ�
//
// Copyright (c) 2003.11.25 By KStaR79
//-----------------------------------------------------------------------------
unsigned int __stdcall TextrueUpdateThread( LPVOID pVoid )
{
	for(;;)
	{	
		if( true == g_RockClient.m_bGameEnd )
			return -1;

//		else if( GMODE_NORMAL == g_RockClient.GetGameMode() )
		{
			// ó�� ���� �����Ҵ뵵 �Ҹ� �ʿ� By wxywxy
			g_TexManager.UpdateTextureCreate();
			g_TexManager.UpdateSoundCreate();
			g_TexManager.UpdateDataUse();
		}		
		Sleep( 1 );
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Desc: �ʱ�ȭ
//
// Copyright (c) 2003.10.25 By KStaR79
//-----------------------------------------------------------------------------
void	TEXTURE_MANAGER::Clear()
{
	m_byType		= 0;	
	m_lPreTime		= g_nowTime;
	m_lPreCreTexTime = g_nowTime;

	theChrDeleteCnt = 0;
	theNpcDeleteCnt = 0;
	theObjDeleteCnt = 0;

	for( int i = 0; i < TEXTURE_INPUT_COUNT; ++i )
	{
		m_CreateTex[i].Clear();
	}

	std::list <DATA_CREATE_ATTR *>::iterator ir = theCreateTex.begin();

	for( ; ir != theCreateTex.end(); ++ir )		
	{
		DATA_CREATE_ATTR *p = *ir;
		SAFE_DELETE(p);		
		theCreateTex.erase( ir++ );
	}

	return;
}

void    TEXTURE_MANAGER::ClearTheCreateTexList()
{
	std::list <DATA_CREATE_ATTR *>::iterator ir = theCreateTex.begin();

	for( ; ir != theCreateTex.end(); ++ir )		
	{
		DATA_CREATE_ATTR *p = *ir;
		SAFE_DELETE(p);		
		theCreateTex.erase( ir++ );
	}
}

void	TEXTURE_MANAGER::Destroy()
{
	Clear();

	return;
}


//-----------------------------------------------------------------------------
// Desc: �ؽ��İ˻� �ð� �ʱ�ȭ
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
void	TEXTURE_MANAGER::InitTextrueTime()
{
	if( true == g_RockClient.m_bGameEnd )
		return;

	// ����ð�����
	m_lPreTime = g_nowTime;

	// ĳ����
	int i = 0;
	for(  i = 0; i < c_Chr_Tex_Count+c_Npc_Tex_Count; ++i )	
	{
		if( g_Pc_Manager.m_pTex[i] )
		{
			g_Pc_Manager.m_pTex[i]->m_lPreTime = g_nowTime;
		}
	}

	// ������Ʈ
	for( i = 0; i < MAX_OBJ_TEXCOUNT; ++i )
		g_Obj_Manager.m_Tex[i].m_lPreTime = g_nowTime;

	// ����
	InitSoundTime();
}


//-----------------------------------------------------------------------------
// Desc: �ؽ��İ˻��ϱ�
//
// 1. �����ð��� ���� ��� ����Ÿ�Կ� ���� �ؽ����� ��뿩�θ� �˻��Ѵ�.
// 2. ���� �˻��� �ð��� �����Ͽ��� �Լ��� ��� ȣ����� �ʵ��� �Ѵ�.
//
// Copyright (c) 2003.10.25 By KStaR79
//-----------------------------------------------------------------------------
void	TEXTURE_MANAGER::UpdateDataUse()
{
	// UPDATA_TIME �ؽ��Ļ�� �˻�ð� �����ٸ�
	if( SAFE_TIME_COMPARE( g_nowTime , >= , SAFE_TIME_ADD( m_lPreTime , DATA_UPDATA_TIME ) ) )
	{
		// ����Ÿ�Կ� ���� ó��
		m_lPreTime = g_nowTime;
		switch( m_byType )
		{
		case 0:		// NPC
			{
				for( int i = c_Chr_Tex_Count; i < c_Chr_Tex_Count+c_Npc_Tex_Count; ++i )		
				{
					if( g_Pc_Manager.m_pTex[i] )
					{
						g_Pc_Manager.m_pTex[i]->UpdateTextureUse();					
					}
				}
			}
			break;
		case 1:		// CHR
			{
				for( int i = 0; i < c_Chr_Tex_Count; ++i )
				{
					if( g_Pc_Manager.m_pTex[i] )
					{
						g_Pc_Manager.m_pTex[i]->UpdateTextureUse();				
					}
				}
			}
			break; 
		case 2:		// OBJ
			{
				for( int i = 0; i < MAX_OBJ_TEXCOUNT; ++i )
					g_Obj_Manager.m_Tex[i].UpdateTextureUse();
			}
			break;
		case 3:		// SOUND
			{
				g_Sound.UpdateSoundUse();
			}
			break;
		}

		// ����Ÿ������ �̵�
		m_byType++;
		if( m_byType > 3 )
			m_byType = 0;
	}
}


//-----------------------------------------------------------------------------
// Desc: �ؽ��� �����ϱ�
//
// m_CreateTex[]�� �ִ� �ε����� �ؽ��ĸ� �����Ѵ�
//
// Copyright (c) 2003.10.25 By KStaR79
//-----------------------------------------------------------------------------
void	TEXTURE_MANAGER::UpdateTextureCreate()
{
	for( int i = 0; i < TEXTURE_INPUT_COUNT; ++i )
	{
		if( m_CreateTex[i].byType != 255 && m_CreateTex[i].Index != -1 )
		{
			// Ÿ�Կ� ���� �ؽ���ã��
			Texture*	pTex = NULL;
			switch( m_CreateTex[i].byType )
			{
			case 0:		// NPC
			case 1:		// CHR
				pTex = g_Pc_Manager.m_pTex[m_CreateTex[i].Index];
				break;
			case 2:		// OBJ
				pTex = &g_Obj_Manager.m_Tex[m_CreateTex[i].Index];
				break;
			default:
				continue;				
				break;
			}

			// �ؽ��� ����
			if( pTex == NULL )
			{

			}
			else
			{
				pTex->UpdateTexture( m_CreateTex[i].byType, m_CreateTex[i].Index );
			}

			// ������ �ʱ�ȭ
			m_CreateTex[i].Clear();
		}
	}
}


//-----------------------------------------------------------------------------
// Desc: ������ �ؽ����ε��� �����ϱ�
//
// �ε����� �޾Ƽ� �ؽ��� ���������ڿ��� ���� �迭�� �ִ´�.
//
// Copyright (c) 2003.10.25 By KStaR79
//-----------------------------------------------------------------------------
bool	TEXTURE_MANAGER::SetTextureIndex( BYTE _byType, int _nIndex )
{
	if( true == g_RockClient.m_bGameEnd )
		return false;

	for( int i = 0; i < TEXTURE_INPUT_COUNT; ++i )
	{
		// �����迭�� �ִٸ� ����
		if( m_CreateTex[i].byType == _byType && m_CreateTex[i].Index == _nIndex )
			break;
		// ����ִ� �迭�̶�� ����
		else if( m_CreateTex[i].byType == 255 && m_CreateTex[i].Index == -1 )
		{
			m_CreateTex[i].byType	= _byType;
			m_CreateTex[i].Index	= _nIndex;
			break;
		}
	}

	return true;
}





//-----------------------------------------------------------------------------
// Desc: ����ð� �ʱ�ȭ
//
// Copyright (c) 2004.1.2 By KStaR79
//-----------------------------------------------------------------------------
void	TEXTURE_MANAGER::InitSoundTime()
{
	g_Sound.InitSoundTime();
}


//-----------------------------------------------------------------------------
// Desc: ������ �����ε��� �����ϱ�
//
// �ε����� �޾Ƽ� ���������ڿ��� ���� �迭�� �ִ´�.
//
// Copyright (c) 2004.1.2 By KStaR79
//-----------------------------------------------------------------------------
bool	TEXTURE_MANAGER::SetSoundIndex( int _nIndex )
{
	if( true == g_RockClient.m_bGameEnd )
		return false;

//	EnterCriticalSection( &m_csSoundFlag );
	for( int i = 0; i < SOUND_INPUT_COUNT; ++i )
	{
		// �����迭�� �ִٸ� ����
		if( m_CreateSound[i].Index == _nIndex )
			break;
		// ����ִ� �迭�̶�� ����
		else if( m_CreateSound[i].Index == -1 )
		{
			m_CreateSound[i].Index = _nIndex;
			break;
		}
	}
//	LeaveCriticalSection( &m_csSoundFlag );
	return true;
}

void	TEXTURE_MANAGER::UpdateSoundCreate()
{
	if( true == g_RockClient.m_bGameEnd )
		return;

//	EnterCriticalSection( &m_csSoundFlag );	
	for( int i = 0; i < SOUND_INPUT_COUNT; ++i )
	{						
		if( m_CreateSound[i].Index != -1 )
		{
			// �������
//			LeaveCriticalSection( &m_csSoundFlag );
			g_Sound.UpdateSound( m_CreateSound[i].Index );			

			// �������̺� �ʱ�ȭ
//			EnterCriticalSection( &m_csSoundFlag );	
			m_CreateSound[i].Clear();			
		}
	}
//	LeaveCriticalSection( &m_csSoundFlag );
}


