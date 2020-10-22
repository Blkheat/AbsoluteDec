////////////////////////////////////////////////////////////////////////////////
///
///		@file
///
///		@brief	PacketCrypto.cpp: implementation of the CNetPackMgr class.
///
///		Copyright (c) 2003, RockSoft Co., Ltd.
///		ALL RIGHTS RESERVED
///
///		RockEngine(R), Rock3D(R), RockServer(R) are registered trademarks of
///		RockSoft Co., Ltd.
///
///		Author	: sooree
///		Team	: Rock Server team.
///		Date	: 2005-09-26
///
////////////////////////////////////////////////////////////////////////////////

#include "RockPCH.h"
#include "PacketCrypto.h"

///////////////////////////////////////////* global *///////////////////////////////////////////

const int	c_CSMaxCryptoObj = 4;
const int	c_SCMaxCryptoObj = 2;

CRockCrypto* g_CSCrypto = NULL;
CRockCrypto* g_SCCrypto = NULL;


void	CleanupNetPack()
{
	SAFE_DELETE_ARRAY( g_CSCrypto );
	SAFE_DELETE_ARRAY( g_SCCrypto );
}

void	CreateNetPack()
{
	CleanupNetPack();

	///=========================================================================================
	/// Server -> Client
	///=========================================================================================
	g_SCCrypto = SAFE_NEW_ARRAY( CRockCrypto, c_SCMaxCryptoObj );
	
	{
		//	1 : CRcCrypto - 16 Byte Key		: Stream RC4 Compatible Crypto
		BYTE key0[] = { 0xe8, 0x17, 0x33, 0x00, 0x00, 0x8d, 0x45, 0xc8, 0x8b, 0xcf, 0x50, 0xe8, 0xf9, 0x23, 0x00, 0xff };
		g_SCCrypto[0].SetKey( NULL, (const char*)key0, NULL, NULL );
		
		//	1 : CRcCrypto - 16 Byte Key		: Stream RC4 Compatible Crypto
		BYTE key1[] = { 0x66, 0x8d, 0x45, 0xec, 0xf6, 0x50, 0x8b, 0xcf, 0x89, 0x35, 0x8c, 0x4a, 0x00, 0xe8, 0x2c, 0x86 };
		g_SCCrypto[1].SetKey( NULL, (const char*)key1, NULL, NULL );
	}

	///=========================================================================================
	///	Client -> Server
	///=========================================================================================
	g_CSCrypto = SAFE_NEW_ARRAY( CRockCrypto, c_CSMaxCryptoObj );		
	
	{
		//	1 : CRcCrypto - 16 Byte Key		: Stream RC4 Compatible Crypto
		BYTE key0[] = { 0x7d, 0x44, 0x01, 0x00, 0x83, 0xec, 0x24, 0x83, 0x25, 0xb8, 0x8c, 0x4a, 0x0d, 0x56, 0x8b, 0x75 };
		g_CSCrypto[0].SetKey( NULL, (const char*)key0, NULL, NULL );

 		//	0 : CStrmCrypto - 12 Byte Key	: Stream Bit Scramble Crypto
		BYTE key1[] = { 0x1c, 0x8d, 0x1c, 0x57, 0x50, 0xce, 0xe8, 0x6f, 0x85, 0xfe, 0xff, 0x8b };
		g_CSCrypto[1].SetKey( (const char *)key1,  NULL,  NULL,  NULL );
		
		//	1 : CRcCrypto - 16 Byte Key		: Stream RC4 Compatible Crypto
		BYTE key2[] = { 0x76, 0x0c, 0x50, 0x45, 0x14, 0x83, 0x65, 0xfc, 0x56, 0x50, 0x7d, 0xd1, 0x74, 0x03, 0xb8, 0x43 };
		g_CSCrypto[2].SetKey( NULL, (const char*)key2, NULL, NULL );
		
 		//	0 : CStrmCrypto - 12 Byte Key	: Stream Bit Scramble Crypto
		BYTE key3[] = { 0x8b,  0x47,  0xdd,  0x6a,  0xe8,  0x14,  0x83,  0xc4,  0xbc,  0xf3,  0x7f,  0x75 };
		g_CSCrypto[3].SetKey( (const char *)key3,  NULL,  NULL,  NULL );
	}
}

CRockCrypto* C2SGetNetPackObj( int aIndex )
{
	if( g_CSCrypto == NULL )
	{
		CreateNetPack();
	}

	return &( g_CSCrypto[aIndex] );
}

CRockCrypto* S2CGetNetPackObj( int aIndex )
{
	if( g_SCCrypto == NULL )
	{
		CreateNetPack();
	}

	return &( g_SCCrypto[aIndex] );
}

///////////////////////////////////////////* global *///////////////////////////////////////////


CNetPackMgr::CNetPackMgr()
{
	theSequence = 0;

	theCSCryptoIndex = 0;	
	theCSRockCryptoPtr = NULL;	

	theSCCryptoIndex = 0;	
	theSCRockCryptoPtr = NULL;	
}

CNetPackMgr::~CNetPackMgr()
{
}

CRockCrypto*	CNetPackMgr::C2SGetNetPackObj()
{
	return( ::C2SGetNetPackObj( ( ++theCSCryptoIndex ) % c_CSMaxCryptoObj ) );
}

CRockCrypto*	CNetPackMgr::S2CGetNetPackObj()
{
	return( ::S2CGetNetPackObj( ( ++theSCCryptoIndex ) % c_SCMaxCryptoObj ) );
}

void	CNetPackMgr::InitializeIdx()
{
	theSequence = 0;

	theCSCryptoIndex = 0;
	theSCCryptoIndex = 0;
}

void	CNetPackMgr::Encrypt( void* aMsgPtr, int aSize )
{
	C2SGetNetPackObj()->Encrypt( aMsgPtr, aSize );
}

void	CNetPackMgr::Decrypt( void* aMsgPtr, int aSize )
{
	S2CGetNetPackObj()->Decrypt( aMsgPtr, aSize );
}
