#include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	"..\\quadlist.h"
#include	<Base\\D3DBase.h>
#include	"..\\RockClient.h"
#include	"Grass.h"
#include	"GTexture.h"
#include	"..\\RockClient.h"

#include	<stdio.h>
#include	"GrassNormal.h"
#include	<time.h>

extern	CGrassData	g_GrassList[1024];
//extern	KEYGRSNORMAL	g_Key[2];

CGrass::CGrass()
{
}

CGrass::~CGrass()
{
	FinalCleanup();
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CGrass::Create(LPDIRECT3DDEVICE9 pDevice)
#else
HRESULT	CGrass::Create(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "Invalid device", "GRASS", MB_OK);
		return	E_FAIL;
	}
	m_pDevice	= pDevice;

	char	path[MAX_PATH];
	sprintf( path, "%s\\Env\\grs.dat", g_RockClient.GetRootDir() );

	m_cCount = GetPrivateProfileInt( "GRASS", "COUNT", 0, path );

	char	key[128], buf[128];
	int		nTexIndex;
	float	fHeight;

	m_szNameList	= SAFE_NEW_ARRAY( char* , m_cCount );	
	for( int i = 0; i < m_cCount; i ++ )
	{
		m_szNameList[i]	= SAFE_NEW_ARRAY( char , 128 );		

		sprintf( key, "GRS%d", i );
		GetPrivateProfileString( "GRASS", key, "", buf, sizeof( buf ), path );
		if( strlen( buf ) > 0 )
		{
			sscanf( buf, "%d %f", &nTexIndex, &fHeight );
			g_GrassList[i].InitDevice( m_pDevice );
			g_GrassList[i].Load( nTexIndex, fHeight );

			sprintf( m_szNameList[i], "%s\\Env\\%d.grs", g_RockClient.GetRootDir(), i );
		}
	}

	return	S_OK;
}

HRESULT	CGrass::Update()
{
/*	for ( unsigned short n = 0; n < 2; n ++ )
	{
		g_Key[n].UpdateFrame();
	}

	for ( n = 0; n < m_cCount; n ++ )
	{
		g_GrassList[n].Update();
	}
*/
	for ( short i = 0; i < (short)m_cCount; i ++ )
	{
		g_GrassList[i].Update();
	}
	return	S_OK;
}

void	CGrass::FinalCleanup()
{
	for ( DWORD i = 0; i < m_cCount; i ++ )
		_DELETE_ARRAY(m_szNameList[i]);
	_DELETE_ARRAY(m_szNameList);
}


DWORD	CGrass::GetIndexByName(char* Name)
{
	for ( DWORD i = 0; i < m_cCount; i ++ )
	{
		if ( strcmp(Name, m_szNameList[i]) == 0 )
			return i;
	}
	return 0;
}