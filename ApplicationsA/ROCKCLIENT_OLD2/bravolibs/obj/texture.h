
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include	"VFileSystem.h"	/// sooree pack 

#ifdef DIRECT_VERSION_9_MJH
#include	<D3DX9.H>
#else
#include	<D3DX8.H>
#endif // DIRECT_VERSION_9_MJH 


typedef enum NTextureType
{
	n_Texture = 0,
	n_Interface

} NTextureType;

//-----------------------------------------------------------------------------
// Desc: 텍스쳐 클래스. 텍스쳐의 포인터를 가지고 있다. 
//       텍스쳐 갯수만큼 클래스 생성된다.
//-----------------------------------------------------------------------------
class Texture
{
public:		
	void	Clear();		// 생성자
	void	Destory();		// 파괴자

public:	
	
	bool		m_bIsLoaded;		// 로딩했니?
	bool		m_bIsLoading;
	long		m_lPreTime;			// 마지막셋팅시간
	DWORD		m_Index;			// 파일인덱스
	BYTE		m_byAttr;			// 텍스쳐속성값
	BYTE		m_byType;			// 타입

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pD3DTex;			// 텍스쳐 버퍼 포인터
#else
	LPDIRECT3DTEXTURE8		m_pD3DTex;			// 텍스쳐 버퍼 포인터
#endif // DIRECT_VERSION_9_MJH

public:
	Texture()	{ Clear(); }
	~Texture()	{ Destory(); }	
	
	///-- Texture Loading : a_nDevSkip 이 0이 아닐 경우 Texture Loading 을 Skip
	bool	ReadVF( CVFileSystem* pVF, char* file_path, int type, int a_nDevSkip = 0 );	/// sooree pack

	bool	Read( char* file_path, int type );		// 텍스쳐 읽기
	bool	Read( char* _szFilePath, int type , BOOL ISSkip = TRUE );

	bool	SetTexture( BYTE byType, int index );			// 텍스쳐 셋팅	
	bool	UpdateTexture( BYTE byType, int index );		// 텍스쳐 사용하니?
	void	UpdateTextureUse();								// 텍스쳐 재할당검사
};

#endif