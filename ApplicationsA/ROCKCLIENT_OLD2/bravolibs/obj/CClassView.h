#ifndef __CCLASSVIEW_H
#define __CCLASSVIEW_H

#include "RockPCH.h"

typedef struct s_ClassViewData
{

	BYTE	Race;							// ����
	BYTE	Sex;							// ����

	int		Hair;							// �Ӹ�
	int		Face;							// ��
	int     Helmet;							// ���
	int		Coat;							// ����
	int		Pants;							// ����
	int		Gloves;							// �尩
	int		Shoes;							// ����
	int		Armor;							// ����
	int		Shiled;							// ����

	int		AniCount;						// ���ϸ��̼� ����

	std::vector <int>			AniList;	// ���ϸ��̼�

}CLASS_VIEW_DATA;

typedef struct s_ClassViewHelp
{
#ifdef UNICODE_ROCK
	std::vector <std::wstring>	StrHelp;	// ����
#else
	std::vector <std::string>	StrHelp;	// ����
#endif
	

}CLASS_VIEW_HELP;

class CClassView
{
public:
	 
	 CClassView();
	~CClassView();

public:

	int Load(void);

	CLASS_VIEW_DATA *GetData(BYTE class_num);
	CLASS_VIEW_HELP *GetHelpData(BYTE class_num);

private:

	std::map<BYTE,CLASS_VIEW_DATA>		m_ClassMap;
	std::map<BYTE,CLASS_VIEW_HELP>		m_ClassHelpMap;
};

#endif