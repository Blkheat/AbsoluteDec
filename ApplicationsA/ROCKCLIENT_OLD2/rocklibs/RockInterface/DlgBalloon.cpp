///////////////////////////////////////////////////////////////////////////////
///
///		File		: DlgBalloon.cpp
///		Desc		:
///
///		Author		: Park Soo-Hyun
///		Team		: Program - Client Team
///		Date		: 2004-05-24
///
///		Copyright(c) 2004, Rocksoft Co., Ltd. ALL RIGHTS RESERVED
///
///////////////////////////////////////////////////////////////////////////////
#include "DlgBalloon.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "CRenderManager.h"
#include "CBillBoardUIRender.h"

//-----------------------------------------------------------------------------
CDlgBalloon::CDlgBalloon( SDesktop* DT )
{
	Initialize( DT );
}

//-----------------------------------------------------------------------------
CDlgBalloon::~CDlgBalloon()
{
}

//-----------------------------------------------------------------------------
void CDlgBalloon::Initialize( SDesktop* DT )
{
	theDT		= DT;
	
	theFontg	= Fontman->GetFont( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );

	memset( &theStrInfo, 0, sizeof( SStringInfo ) );
	memset( theText, 0, sizeof( TCHAR ) * 256 );

	theBlankLR	= 2;
	theBlankTB	= 2;

	theMaxWidth	= 200;
	theLifeTime = 9;

	SAFE_STR_CPY( theText, _RT(""), 256 );
	theColor = D3DCOLOR_XRGB( 255, 255, 255 );
	theBType = n_BT_NORMAL;

	theCType	= enNONE;
	theID		= 0;
	thePcIdx	= 0;
	thePcInfo	= NULL;

	theBGAlpha	= 0;
	theTXTAlpha	= 0;
}

//-----------------------------------------------------------------------------
void CDlgBalloon::Set( NCHARTYPE ctype, DWORD id, SPcInfo* pc_info, SDlgBorder* border )
{
	theCType	= ctype;
	theID		= id;
	thePcInfo	= pc_info;

	theBorder	= border;
/*
	for( int i = 0; i < MAX_CHARACTER; ++i )
	{
		if( ( thePcInfo[i].ctype == ctype ) && ( thePcInfo[i].id == id ) )
		{
			thePcIdx = i;
			break;
		}
	}
	
	// *���� ���̿� ���� Ŭ���̾�Ʈ ũ�� ���� & ������ ũ�� ����
	// *ȭ����� ��ġ ���( ȭ�� ������ ����� �ʰ� ���� )
//	theClient.x = x;
//	theClient.y = y;

	CalcClientSize();
//*/	
}

//-----------------------------------------------------------------------------
bool CDlgBalloon::IsExistence( NCHARTYPE ctype, DWORD id )
{
	if( ( theCType == ctype ) && ( theID == id ) )
	{
		return( true );
	}

	return( false );
}

//-----------------------------------------------------------------------------
bool CDlgBalloon::IsLife()
{
	if( theLifeTime <= 0 )
	{
		return( false );
	}

	return( true );
}

//-----------------------------------------------------------------------------
void CDlgBalloon::Die()
{
	theLifeTime = 0;
}

//-----------------------------------------------------------------------------
void CDlgBalloon::CalcClientSize()
{
	if( theStrInfo.Line == 1 )
	{
		theClient.w = theStrInfo.Width + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}
	else
	{
		theClient.w = theMaxWidth + ( theBlankLR * 2 );
		theClient.h = theStrInfo.Height + ( theBlankTB * 2 );
	}


	//int sc_x = pDT->Width / 2;
	//int sc_y = pDT->Height / 2;

	//theClient.x = sc_x - ( theClient.w / 2 );
	//theClient.y = sc_y - ( theClient.h / 2 );
}

//-----------------------------------------------------------------------------
void CDlgBalloon::CalcFrameSize()
{
	//���� ������ �̹����� ��������..���� ������ �̹����� ���� ũ�⺸�� ������ �������� ���´�....
	//�ϴ�...ũ�⸦ �ٿ��� �ذ�....������?

	//��
	theNorthFrm.x = theClient.x;
	theNorthFrm.y = theClient.y - theBorder[theBType].width;
	theNorthFrm.w = theClient.w;
	theNorthFrm.h = theBorder[theBType].width;
	//
	theNorthEastFrm.x = theClient.x + theClient.w;
	theNorthEastFrm.y = theClient.y - theBorder[theBType].width;
	theNorthEastFrm.w = theBorder[theBType].width;
	theNorthEastFrm.h = theBorder[theBType].width;
	//������
	theEastFrm.x = theClient.x + theClient.w;
	theEastFrm.y = theClient.y;
	theEastFrm.w = theBorder[theBType].width;
	theEastFrm.h = theClient.h;
	//
	theSouthEastFrm.x = theClient.x + theClient.w;
	theSouthEastFrm.y = theClient.y + theClient.h;
	theSouthEastFrm.w = theBorder[theBType].width;
	theSouthEastFrm.h = theBorder[theBType].width;
	//�Ʒ�
	theSouthFrm.x = theClient.x;
	theSouthFrm.y = theClient.y + theClient.h;
	theSouthFrm.w = theClient.w;
	theSouthFrm.h = theBorder[theBType].width;
	//
	theSouthWestFrm.x = theClient.x - theBorder[theBType].width;
	theSouthWestFrm.y = theClient.y + theClient.h;
	theSouthWestFrm.w = theBorder[theBType].width;
	theSouthWestFrm.h = theBorder[theBType].width;
	//����
	theWestFrm.x = theClient.x - theBorder[theBType].width;
	theWestFrm.y = theClient.y;
	theWestFrm.w = theBorder[theBType].width;
	theWestFrm.h = theClient.h;
	//
	theNorthWestFrm.x = theClient.x - theBorder[theBType].width;
	theNorthWestFrm.y = theClient.y - theBorder[theBType].width;
	theNorthWestFrm.w = theBorder[theBType].width;
	theNorthWestFrm.h = theBorder[theBType].width;
}

//----------------------------------------------------------------------------
void CDlgBalloon::RenderProc()
{
	Character* dlgballon_chr = NULL;	

    //by simwoosung ������ ���� - ��ǳ��
	
	RenderDlgBalloon( dlgballon_chr );	

	if( dlgballon_chr == NULL )
	{
		Die();
		return;
	}
	else
	{
		if( dlgballon_chr->m_nTableIndex == -1	  )//||
//			dlgballon_chr->m_bIsVisible  == false	 )
		{
			Die();			
			return;
		}
	}	
	
	//RenderDlgBalloonBill();
	
	///-- Alpha �ִ� ���� Life Time 1�ʿ� 1�� ���� 
	///-- Monster �� ��� chatting Box �� �ʹ� ��� �ൿ�� ���� ���� ���� ����.
	///-- ->������ �ð� ���� 2005.03.07 / Lyul
	if( theTXTAlpha >= 255)
	{		
		if( theDT->Tflag.One_1 )
		{
			//-- Lyul
			if(dlgballon_chr->m_bPC)
			{
				//.................................................................................................
				// ���� ���� �̸� ���ڸ� ��� ���
				//.................................................................................................
				if( dlgballon_chr->m_curt_event.status != SM_SELLSKILL_SITDOWN &&
					dlgballon_chr->m_curt_event.status != SM_SELLSKILL_KEEPING &&
					dlgballon_chr->m_curt_event.status != SM_SELLSKILL_STANDUP   )   
				{
					theLifeTime -= 1;
				}
			}
			else
			{
				theLifeTime -= 2;			
			}
		}
	}
}

void CDlgBalloon::RenderDlgBalloonBill()
{

}

void CDlgBalloon::RenderDlgBalloon(Character*  & pCharacter)
{
    
	D3DXVECTOR3 vPos;
	Character *pPlayer = NULL;

	D3DCOLOR color = D3DCOLOR_ARGB( theBGAlpha, 255, 255, 255 );
	D3DCOLOR bcolor = D3DCOLOR_ARGB( theBGAlpha, 255, 255, 255 );
	D3DCOLOR tcolor = D3DCOLOR_ARGB( theTXTAlpha, 0, 0, 0 );

	theBlankLR	= 2;
	theBlankTB	= 2;
	
	bool bFindPlayer = false;	
	
	for( int i = 0; i < MAX_CHARACTER; ++i )
	{
		if( ( thePcInfo[i].ctype == theCType ) && ( thePcInfo[i].id == theID ) )
		{
			bFindPlayer = true;

			//�� thePcIdx ������ ����ؼ� ���ŵǹǷ� theCType�� theID�� ������ ���ؾ���
			thePcIdx = i;
			
			vPos.x = thePcInfo[thePcIdx].wpos.x;
	        vPos.y = thePcInfo[thePcIdx].wpos.y;
	        vPos.z = thePcInfo[thePcIdx].wpos.z;

			pPlayer = (Character *)(thePcInfo[thePcIdx].pCharacter);			
			pCharacter = pPlayer;

			vPos.y += (pPlayer->m_fMaxHeight);

			D3DXMATRIX mat;
			D3DXMatrixMultiply(&mat, &(g_Camera.m_matView), &(g_Camera.m_matProj));			
			D3DXVec3TransformCoord(&vPos, &vPos, &mat);				
			
			theBType = thePcInfo[thePcIdx].btype;
			theColor = thePcInfo[thePcIdx].color;

			if(pPlayer)
			{
				if( pPlayer->m_curt_event.status == SM_SELLSKILL_SITDOWN ||
					pPlayer->m_curt_event.status == SM_SELLSKILL_KEEPING ||
					pPlayer->m_curt_event.status == SM_SELLSKILL_STANDUP   ) 
				{
					theBType = n_BT_MYSHOP;
					
					theBlankLR	= 4;
					theBlankTB	= 5;					
					
					color = D3DCOLOR_ARGB( 255 ,255, 255, 255 );
					bcolor = D3DCOLOR_ARGB( 255, 0, 0, 0 );
					tcolor = D3DCOLOR_ARGB( 255, 255, 175, 255 );
				}
			}
			
			// *���ڿ��� ���̿� ���� ���ϱ�
			SAFE_STR_CPY( theText, thePcInfo[thePcIdx].message, 256 );
			theFontg->GetStringInfo( theText, &theStrInfo, theMaxWidth );			
			
			if( theStrInfo.Line == 1 )
			{
				theClient.x = thePcInfo[thePcIdx].spos.x - ( theStrInfo.Width / 2 );
			}
			else
			{
				theClient.x = thePcInfo[thePcIdx].spos.x - ( theMaxWidth / 2 );
			}
			
			theClient.y = thePcInfo[thePcIdx].spos.y - theBorder[theBType].tail_h 
				- ( ( theStrInfo.Height + 12 ) + theBorder[theBType].width + theBlankTB ) - pPlayer->m_nAddNum;           

			CalcClientSize();
			CalcFrameSize();		
			
			break;
		}
	}
	
	if( !bFindPlayer )
	{
		return;
	}

	if( ( theTXTAlpha != 255 ) && ( theDT->Tflag.One_20 ) )
	{
		theBGAlpha	+= 12;
		theTXTAlpha += 15;

		if( theBGAlpha >= 150 )
		{
			theBGAlpha = 150;
		}
		if( theTXTAlpha >= 255 )
		{
			theTXTAlpha = 255;
		}
	}


	Render->Draw( TID_CLIENT, theClient.x, theClient.y, theClient.w, theClient.h, bcolor , vPos.z);


	Render->Draw( theBorder[theBType].north,		theNorthFrm.x,		theNorthFrm.y,		theNorthFrm.w,		theNorthFrm.h,		color , vPos.z );
	Render->Draw( theBorder[theBType].north_east,	theNorthEastFrm.x,	theNorthEastFrm.y,	theNorthEastFrm.w,	theNorthEastFrm.h,	color , vPos.z );
	Render->Draw( theBorder[theBType].east,			theEastFrm.x,		theEastFrm.y,		theEastFrm.w,		theEastFrm.h,		color , vPos.z );
	Render->Draw( theBorder[theBType].south_east,	theSouthEastFrm.x,	theSouthEastFrm.y,	theSouthEastFrm.w,	theSouthEastFrm.h,	color , vPos.z );
	Render->Draw( theBorder[theBType].south,		theSouthFrm.x,		theSouthFrm.y,		theSouthFrm.w,		theSouthFrm.h,		color , vPos.z );
	Render->Draw( theBorder[theBType].south_west,	theSouthWestFrm.x,	theSouthWestFrm.y,	theSouthWestFrm.w,	theSouthWestFrm.h,	color , vPos.z );
	Render->Draw( theBorder[theBType].west,			theWestFrm.x,		theWestFrm.y,		theWestFrm.w,		theWestFrm.h,		color , vPos.z );
	Render->Draw( theBorder[theBType].north_west,	theNorthWestFrm.x,	theNorthWestFrm.y,	theNorthWestFrm.w,	theNorthWestFrm.h,	color , vPos.z );

    if(theBType != n_BT_MYSHOP)
	{
		theTail.x = thePcInfo[thePcIdx].spos.x - ( theBorder[theBType].tail_w / 2);
		theTail.y = thePcInfo[thePcIdx].spos.y - theBorder[theBType].tail_h - 12 - pPlayer->m_nAddNum;
		theTail.w = theBorder[theBType].tail_w;
		theTail.h = theBorder[theBType].tail_h;
		Render->Draw( theBorder[theBType].tail,	theTail.x, theTail.y, theTail.w, theTail.h, color , vPos.z);
	}
	else
	{
		SRect rShopIConRect;
		rShopIConRect.x = theNorthFrm.x;
		rShopIConRect.y = theNorthFrm.y - 30;
		rShopIConRect.w = 31;
		rShopIConRect.h = 30;
		Render->Draw( TID_DB_SP_Icon, rShopIConRect.x, rShopIConRect.y, 
					  rShopIConRect.w, rShopIConRect.h , 
					  D3DCOLOR_ARGB( 255, 255, 255, 255 ), vPos.z);
	}
	
	int x = theClient.x + theBlankLR;
	int y = theClient.y + theBlankTB;
	int w = theClient.w - ( theBlankLR * 2 );
	int h = theClient.h - ( theBlankTB * 2 );

	g_RenderManager.SetZBias( 2 );
	
	Render->DrawText( theFontg, theText, x, y, w, h, tcolor, true, 0 , vPos.z);

	g_RenderManager.SetZBias( 0 );
}


//-----------------------------------------------------------------------------