///////////////////////////////////////////////////////////////////////////////
///
///		File		: HelpItemTip.h
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#ifndef __HELPITEMTIP_H__
#define __HELPITEMTIP_H__
//-----------------------------------------------------------------------------
#include "..\\RockPCH.h"

#ifdef DIRECT_VERSION_9_MJH
#include <d3dx9.h>
#else
#include <d3dx8.h>
#endif // DIRECT_VERSION_9_MJH

#include "Define.h"

#include "Fontman.h"
#include "Render.h"

enum N_ITEMTIP_TYPE
{
	n_TIP_ITEM_WEAPON,
	n_TIP_ITEM_WEAR,
	n_TIP_ITEM_ETC,
	n_TIP_SKILL,
	n_TIP_NONE
};

class CHelpItem
{
public:
	CHelpItem(){}
	CHelpItem( SDesktop* DT );
	~CHelpItem();
	
	void Initialize();
	void Refresh();			///-- ���� ��ġ�� Item �̶� ���� ����
	void Set( WNDID wndid, SRect* wc_size, SPcItem* pcitem );
	void RenderProc();

public:
	CFontg*			theFontg;   //by simwoosung ��ġ����

protected:
	SDesktop*		pDT;

	CWnd*			theSlotWnd;
	WNDID			theSlotWndID;
	SPcItem*		thePcItem;
	SPcItem			thePreSetPcItem;
	
	N_ITEMTIP_TYPE	theTipType;	
	SStringInfo		theStrInfo;

	int GetNumLine( int NumStr, int Count );

	void InitWeapon();
	void InitWear();
	void InitEtc();
	void InitSkill();
	
	void SetTipWeapon();
	void SetTipWear();
	void SetTipEtc();
	void SetTipSkill();

	void DrawTipWeapon();
	void DrawTipWear();
	void DrawTipEtc();
	void DrawTipSkill();


	void SetItemWear();
	void SetItemInventory();
	void SetItemUserStore();
	void SetItemUserStoreBuy();
	void SetMyItem();
	void SetYourItem();

	void CalcClientSize( SRect* wc_size );//calculation
	void CalcFrameSize();
	
	int  GetAlignCenterx( SStringInfo* strInfo );
	
	D3DCOLOR GetRareColor( BYTE rare );//Rare( ��͵� )�� ���� �÷���....

	BOOL EldaStoneMix(int Select);

	SRect			theClient;

	SRect			theNorthFrm;
	SRect			theNorthEastFrm;
	SRect			theEastFrm;
	SRect			theSouthEastFrm;
	SRect			theSouthFrm;
	SRect			theSouthWestFrm;
	SRect			theWestFrm;
	SRect			theNorthWestFrm;

	int				theFrameWidth;


	//-------------------------------------------------------------------------
	int			DescStartpos;
	
	// �ΰ�����
	//-------------------------------------------------------------------------
	//by simwoosung
	D3DCOLOR	theEldaColor;
	STipString	theAppEldTemp;		//"+" ��ȣ
	STipString	theAppEldInfo;		//���� ���ٽ�������

	D3DCOLOR	theMixEldaColor;
	STipString	theAppMixEldTemp; // +
	STipString	theAppMixEldInfo;
	bool		thebMixEldaStone;



	void InitExtraInfo();
	SItemBaseInfo * SetTipApplyEldastone(int iPosX, int iPosY, int iNameWidth, int & ClWidth);
	void SetItemProperty(SItemBaseInfo * pEldaInfo, STipString & tipString);
	void SetAccAddEffect( WORD aScrollCode, STipString & tipString );

	void SetItemAddEffectInfo(TCHAR * pStr , SItemAddEffectInfo * pItemAddEffectInfo);
	
	//HelpTip Ȯ���� ...  
	void SetAddHelpTip(int &ClWidth , int &nMaxStrWidthNum );
	void SetHideHelpTip(int &ClWidth , int &nMaxStrWidthNum );
	

	
	
	//////////////////////////////////////////////////////////////////////////
	
	// Weapon
	//-------------------------------------------------------------------------
	int			WeaponNumLine;

	STipString	theWeaponName;
	STipString	theWeaponGrade;
	STipString	theWeaponNeed;
	STipString	theWeaponDesc[10];
				
	STipString	theWeaponAttack;
	STipString	theWeaponMagic;
	STipString	theWeaponAttackAttri;
	STipString	theWeaponType;
	STipString	theWeaponproperty;
	
	STipString	theWeaponAttackSpeed;
	STipString	theWeaponMagicSpeed;
	STipString	theWeaponSkillName;
	STipString	theWeaponAddEffect1;
	STipString	theWeaponAddEffect2;
	STipString	theWeaponEBW;//E:Endurance, B:Bundle, W:Weight
	STipString	theWeaponPrice;
	STipString	theNeedFame;

	// Wear
	//-------------------------------------------------------------------------
	int			WearNumLine;

	STipString	theWearName;
	STipString	theWearGrade;
	STipString  theWearNeed;
	STipString	theWearDesc[10];
				
	STipString	theWearAttackDef;
	STipString	theWearMagicRes;
	STipString	theWearMentalRes;
	STipString	theWearCurseRes;
	STipString	theWearVitalRes;
	
	STipString	theWearDefensAttri;
	STipString	theWearPoint;//hp, sp, mp'

	STipString	theAccAddEffect;	///-- ��ű� �ΰ�ȿ��
	STipString	theWearproperty;
	STipString	theWearAttackEvade;
	STipString	theWearSkillName;
	
	STipString	theWearAddEffect1;
	STipString	theWearAddEffect2;
	STipString	theWearEBW;//E:Endurance, B:Bundle, W:Weight
	STipString	theWearPrice;

	// Etc
	//-------------------------------------------------------------------------
	int			EtcNumLine;

	STipString	theEtcName;
	STipString	theEtcGrade;
	STipString  theEtcNeed;
	STipString	theEtcTime;
	STipString	theEtcDesc[10];
				
	STipString	theEtcBWP;//B:Bundle, W:Weight, P:Price
	STipString	theEtcPrice;

	// Skill
	//-------------------------------------------------------------------------
	int			SkillNumLine;
	
	STipString	theSkillName;
	STipString	theSkillNeed;
	STipString	theSkillToggle;
	STipString	theSkillDesc[10];

	STipString	theSkillItem;
	STipString	theSkillGage;
	STipString	theSkillWastePoint;
	STipString	theSkillKeepupTime;
	STipString	theSkillProimaType;
	STipString	theSkillUpLevel;
	STipString	theNeedPoint; 
	



	STipString	theSkillType2;
	STipString	theTarget;
	
	STipString  theDelay;		//����ĵ����� 
	STipString	theNeedDelay;	// ���������� 
	STipString	theAttackAttri;	//���ݼӼ�
	STipString  theEffect[3];	//ȿ�� 3���� �ΰ�ȿ�� �߰�
	STipString	theAddEfect;	//�ΰ�ȿ�� 





	//SetItem String
	//-------------------------------------------
	STipString theWearSetItem[4];

public:
	D3DCOLOR    theTColor;

};

//-----------------------------------------------------------------------------
#endif	__HELPITEMTIP_H__