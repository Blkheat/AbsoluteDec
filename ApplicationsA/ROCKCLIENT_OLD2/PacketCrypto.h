////////////////////////////////////////////////////////////////////////////////
///
///		@file
///
///		@brief	PacketCrypto.h: interface for the CNetPackMgr class.
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

#if !defined(__PACKETCRYPTO_H_INCLUDED__)
#define __PACKETCRYPTO_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Crypto/RockCrypto.h"

RB_USING

class CNetPackMgr  
{
public:
	CNetPackMgr();
	virtual ~CNetPackMgr();

	CRockCrypto*	C2SGetNetPackObj();
	CRockCrypto*	S2CGetNetPackObj();

	void			InitializeIdx();

	void			Encrypt( void* aMsgPtr, int aSize );
	void			Decrypt( void* aMsgPtr, int aSize );

	inline DWORD	GetLatestSeq()			{ return theSequence;	}
	inline DWORD	GetNextSeq()			{ return ++theSequence; }

protected:
	DWORD			theSequence;

	int				theCSCryptoIndex;
	CRockCrypto*	theCSRockCryptoPtr;

	int				theSCCryptoIndex;
	CRockCrypto*	theSCRockCryptoPtr;
};

#endif 
