
#ifndef		_TEXTURN_MANAGER_H_
#define		_TEXTURN_MANAGER_H_

#include <windows.h>
#include <list>


#define		DATA_UPDATA_TIME		30000	// ����Ÿ �˻��ð�(30��)
#define		TEXTURE_INPUT_COUNT		32		// �ؽ��� ��������
#define		SOUND_INPUT_COUNT		16		// ���� ��������


//-----------------------------------------------------------------------------
// Desc: �ؽ��� ������
//
// 1. �ش� Ŭ������ ������ ���ӿ��� ���Ǵ� �ؽ��ĵ��� �˻��Ͽ���,
// ���󵵼��� ���� �ؽ��Ĵ� �޸𸮿��� ������Ű�� ���� �Ѵ�.
// 
// 2. ĳ����(����, NPC) ������ Ÿ���� ����ϴ� �ؽ��İ� �ִ��� �˻��Ͽ���,
// �ؽ��İ� ���ٸ� �ǽð����� �ε����ش�.
//
// Copyright (c) 2003.10.20 By KStaR79
//-----------------------------------------------------------------------------
struct	DATA_CREATE_ATTR 
{
	int		Index;
	BYTE	byType;

	DATA_CREATE_ATTR()	{ Clear(); }
	void	Clear()	{ Index = -1; byType = 255; }
};


class TEXTURE_MANAGER
{
public:	
	BYTE		m_byType;			// �˻�Ÿ��( 0:CHR 1:NPC 2:OBJ )
	long		m_lPreCreTexTime;	// ���� �ؽ��� �����ð� 	
	long		m_lPreTime;			// �����˻��ð�		

	WORD		theChrDeleteCnt;
	WORD		theNpcDeleteCnt;
	WORD		theObjDeleteCnt;

	std::list < DATA_CREATE_ATTR * > theCreateTex;

	// �ؽ���
	DATA_CREATE_ATTR	m_CreateTex[TEXTURE_INPUT_COUNT];
	CRITICAL_SECTION	m_csTextureFlag;

	// ���� 
	DATA_CREATE_ATTR	m_CreateSound[SOUND_INPUT_COUNT];

public:
	TEXTURE_MANAGER()	
	{ 
		Clear();		
		InitializeCriticalSection( &m_csTextureFlag );
	}
	~TEXTURE_MANAGER()
	{ 
		Destroy();	
		DeleteCriticalSection( &m_csTextureFlag );
	}

	void	Clear();										// �����ʱ�ȭ
	void	Destroy();										// �����ʱ�ȭ
	void	UpdateDataUse();								// ����Ÿ���˻�
	void	CreateUseTex();									// ��ߵ� �ؽ��� ����
	void	DeleteNotUsedTex();								// ���� �ʴ� �ؽ��� ����� 

	// �ؽ���
	void	InitTextrueTime();								// �ؽ��� ���ð� �ʱ�ȭ	
	bool	SetTextureIndex( BYTE byType, int _nIndex );	// �ؽ��� �����ϱ� ���ؼ� �ε��� �����ϱ�
	void	UpdateTextureCreate();							// �ؽ��� �����ϱ�

	void    ClearTheCreateTexList();						///-- theCreateTex�� ����� ����Ÿ �����

	// ����
	void	InitSoundTime();								// �ؽ��� ���ð� �ʱ�ȭ
	void	UpdateSoundUse();								// �ؽ��� �˻��ϱ�
	bool	SetSoundIndex( int _nIndex );					// ���� �����ϱ� ���ؼ� �ε��� �����ϱ�
	void	UpdateSoundCreate();							// �ؽ��� �����ϱ�
	
	void	DeleteAllChrAndNpcTex();
};

extern TEXTURE_MANAGER		g_TexManager;

#endif