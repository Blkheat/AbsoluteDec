#ifndef __CCLASSVIEW_H
#define __CCLASSVIEW_H

#include "RockPCH.h"

typedef struct s_ClassViewData
{

	BYTE	Race;							// 종족
	BYTE	Sex;							// 성별

	int		Hair;							// 머리
	int		Face;							// 얼굴
	int     Helmet;							// 헬멧
	int		Coat;							// 상의
	int		Pants;							// 하의
	int		Gloves;							// 장갑
	int		Shoes;							// 부츠
	int		Armor;							// 무기
	int		Shiled;							// 방패

	int		AniCount;						// 에니메이션 개수

	std::vector <int>			AniList;	// 에니메이션

}CLASS_VIEW_DATA;

typedef struct s_ClassViewHelp
{
#ifdef UNICODE_ROCK
	std::vector <std::wstring>	StrHelp;	// 도움말
#else
	std::vector <std::string>	StrHelp;	// 도움말
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