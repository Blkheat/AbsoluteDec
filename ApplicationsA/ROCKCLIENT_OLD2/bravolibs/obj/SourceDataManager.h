#ifndef __SOURCEDATAMANAGER_H
#define __SOURCEDATAMANAGER_H

#pragma warning(disable:4786)

#include "..\\..\\rocklibs\\RockPCH.h"

#include <map>
#include <vector>
#include <list>

using namespace std;

#pragma pack(push,1)
struct SAniSkllData
{
	DWORD dAniNum;
	BYTE  bVelRatio;
	DWORD dImpEff;
	DWORD dDamEff;
};

struct SConSkillData
{
	DWORD	dSkillCode;
	DWORD   dLinkEffect;
	BYTE	bIsAfterImage;
	BYTE    bIsAfterWImage;

	SAniSkllData sAniList[15];

	SConSkillData()
	{
		memset(this, 0, sizeof(SConSkillData));
	}	
};
#pragma pack(pop)

typedef map < DWORD, SConSkillData > ConSkillDataList;
typedef map < DWORD, SConSkillData >::iterator ConSkillDataListPos;

#pragma pack(push,1)
struct SEmblemItemCode
{
	BYTE	IsPc;
	DWORD	dItemCode;

	SEmblemItemCode()
	{
		memset(this, 0, sizeof(SEmblemItemCode));
	}
};
#pragma pack(pop)

typedef map < DWORD, SEmblemItemCode > EmblemItemCodeList;
typedef map < DWORD, SEmblemItemCode >::iterator EmblemItemCodeListPos;

typedef list < SNPCMoveInfo * > NPCMoveInfoList;
typedef list < SNPCMoveInfo * >::iterator NPCMoveInfoListPos;

struct SItemAttr
{
	int			Number;
	BYTE		m_sex;
	BYTE		m_race;
	BYTE		m_grav;
	BYTE		m_class;
	BYTE		m_type;
	BYTE		m_number;
	BYTE		m_range;
	BYTE		m_tex;
	float		m_AxisBottomX;
	float		m_AxisBottomY;
	float		m_AxisBottomZ;
	float		m_AxisTopX;
	float		m_AxisTopY;
	float		m_AxisTopZ;
	BYTE		m_min;
	BYTE		m_AttackAniNum;

	SItemAttr()
	{
		memset(this, 0, sizeof(SItemAttr));
	}

	void operator=(struct SItemAttr & temp)
	{
		memcpy(this, &temp, sizeof(SItemAttr));	
	}
};

typedef map < int, SItemAttr > ItemAttrList;
typedef map < int, SItemAttr >::iterator ItemAttrListPos;
typedef map < int, SItemAttr >::value_type ItemAttrListValue;

#pragma pack(push,1)
struct SSkillField
{
	DWORD	dCorresValue;
	DWORD	dStringIndex;

	SSkillField()
	{
		memset(this, 0, sizeof(SSkillField));
	}
};
#pragma pack(pop)

typedef map < DWORD, SSkillField > SkillFieldList;
typedef map < DWORD, SSkillField >::iterator SkillFieldListPos;
typedef map < DWORD, SSkillField >::value_type SkillFieldListValue;

class CSourceDataManager
{
public:	 
	 CSourceDataManager();
	~CSourceDataManager();

	BOOL LoadData(void);
	BOOL LoadConSkillData();
	BOOL LoadEmblemItemCode();
	BOOL LoadNpcMoveInfo();
	BOOL LoadSkillField();

	void ClearData();

	SConSkillData FindConSkillData(DWORD dLinkEffect);
	SConSkillData * FindConSkillDataPtr(DWORD dLinkEffect);

	SEmblemItemCode FindChrEmblemItemCode(DWORD dItemCode);
	SEmblemItemCode FindNpcEmblemItemCode(DWORD dItemCode);

	SEmblemItemCode * FindChrEmblemItemCodePtr(DWORD dItemCode);
	SEmblemItemCode * FindNpcEmblemItemCodePtr(DWORD dItemCode);

	SNPCMoveInfo * FindNpcMoveInfoPtr(WORD NPCCode, WORD Mapid );

	SItemAttr FindNpcMoveInfoPtr( int nNumber );

	SSkillField * FindSkillFieldPtr( DWORD dCorresValue );

protected:
	ConSkillDataList m_ConSkillDataList;

	EmblemItemCodeList m_ChrEmblemItemCodeList;
	EmblemItemCodeList m_NpcEmblemItemCodeList;

	NPCMoveInfoList	m_NpcMoveInfoList;
	
	ItemAttrList m_ItemAttrList;

	SkillFieldList m_SkillFieldList;
};

#endif