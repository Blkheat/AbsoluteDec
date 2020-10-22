
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
// Desc: �ؽ��� Ŭ����. �ؽ����� �����͸� ������ �ִ�. 
//       �ؽ��� ������ŭ Ŭ���� �����ȴ�.
//-----------------------------------------------------------------------------
class Texture
{
public:		
	void	Clear();		// ������
	void	Destory();		// �ı���

public:	
	
	bool		m_bIsLoaded;		// �ε��ߴ�?
	bool		m_bIsLoading;
	long		m_lPreTime;			// ���������ýð�
	DWORD		m_Index;			// �����ε���
	BYTE		m_byAttr;			// �ؽ��ļӼ���
	BYTE		m_byType;			// Ÿ��

#ifdef DIRECT_VERSION_9_MJH
	LPDIRECT3DTEXTURE9		m_pD3DTex;			// �ؽ��� ���� ������
#else
	LPDIRECT3DTEXTURE8		m_pD3DTex;			// �ؽ��� ���� ������
#endif // DIRECT_VERSION_9_MJH

public:
	Texture()	{ Clear(); }
	~Texture()	{ Destory(); }	
	
	///-- Texture Loading : a_nDevSkip �� 0�� �ƴ� ��� Texture Loading �� Skip
	bool	ReadVF( CVFileSystem* pVF, char* file_path, int type, int a_nDevSkip = 0 );	/// sooree pack

	bool	Read( char* file_path, int type );		// �ؽ��� �б�
	bool	Read( char* _szFilePath, int type , BOOL ISSkip = TRUE );

	bool	SetTexture( BYTE byType, int index );			// �ؽ��� ����	
	bool	UpdateTexture( BYTE byType, int index );		// �ؽ��� ����ϴ�?
	void	UpdateTextureUse();								// �ؽ��� ���Ҵ�˻�
};

#endif