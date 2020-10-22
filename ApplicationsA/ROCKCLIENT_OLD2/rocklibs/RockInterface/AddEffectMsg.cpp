// AddEffectMsg.cpp: implementation of the AddEffectMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "Rui.h"
#include "RockClient.h"
#include "WndProc.h"
#include "FrameWnd.h"
#include "ScrollBar.h"
#include "ImageBox.h"
#include "AddEffectMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// �ΰ� ȿ�� �޼����� ������ .. Update���Ͽ� ������ �����Ѵ�.. 
// ����� ������ �޼��� ���.. 
AddEffectMsg * g_pAddEffectMsg = NULL;

struct sAddMsg
{
	
	char Msg[256];
};




AddEffectMsg::AddEffectMsg()
{

	ZeroMemory(&m_Effect_State , sizeof(m_Effect_State));

}

	

AddEffectMsg::~AddEffectMsg()
{

}

void AddEffectMsg::Init()
{
	

}

void AddEffectMsg::Update()
{
	


}

void AddEffectMsg::Proc(SEventMessage* EMsg )
{


}

void AddEffectMsg::Composition()
{
  
	
#ifdef ADDEFFECTMSG
	int iAddPosY = 15;

	m_pFrameWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_AddEffectMsgFrame , WID_None, 
	0, 400 , 330 , 0, false );
	m_pFrameWnd->SetClientImage( TID_None );
	m_pFrameWnd->SetWndProc( this );
	m_pFrameWnd->SetTitle( false );
	m_pFrameWnd->SetVisible( false );
	m_pFrameWnd->SetZorder( n_zoBottom );
	m_pFrameWnd->SetMove(false);	
	m_pFrameWnd->Initialize();
	m_pFrameWnd->SetFocusCheck(false);
	
	m_BgImgBox = (CNoImageBox*)nRui->CreateWnd( n_wtNoImageBox, WID_None, WID_AddEffectMsgFrame , 
		20 , 0 , 130, 16, false );
	m_BgImgBox->SetMaxCount(ADDEFFECT_MAXCOUNT + 1);
		
	for( int i = 0 ; i < ADDEFFECT_MAXCOUNT ; i++ )
	{
		m_TextEffectMsg[i] = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_AddEffectMsgFrame , 
		18, ( iAddPosY * i ) , 330, 18, false );
		m_TextEffectMsg[i]->SetFontg( n_ftGulimChe, 12, n_fwBold, n_fdShadow );
		m_TextEffectMsg[i]->SetClientImage(TID_None);
		m_TextEffectMsg[i]->SetAlignText( n_atLeft, n_atCenter );
		m_TextEffectMsg[i]->SetBlank( 10 , 0, 0, 0 );	
		m_TextEffectMsg[i]->SetText(_RT(""));
		m_TextEffectMsg[i]->SetTextColor( 255 , 0, 0 );
		//m_TextEffectMsg[i]->SetVisible(false);
	}	
	
//	CSystemOutList * m_pSystemOutList;

#endif
	
}

void AddEffectMsg::UpdateEffectMessage(BYTE *Effect_State)
{

#ifndef ADDEFFECTMSG
	return;
#endif

	memcpy( m_Effect_State , Effect_State , sizeof(m_Effect_State) );
	char  chStr[256]	 = "";
	int iCount = 0 ; 
	int i =0 ;
	for ( i  = 0 ; i < ADDEFFECT_MAXCOUNT ; ++i )
	{
		m_TextEffectMsg[i]->SetText(_RT(""));
	}
	
	for(i =0 ; i < MAX_ADDEFFECT_NUM ; ++i)
	{
		if( iCount > ADDEFFECT_MAXCOUNT )
		{
			iCount	 = 0 ; 
		}

		switch(i)
		{
			case n_TypeNotMove:	
			{
				if( m_Effect_State[i] )
				{
					Rsprintf(RWCHAR(chStr), G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_NOT_MOVE));
					m_TextEffectMsg[iCount]->SetText(chStr);	
					iCount++;
				}
				
			}
			break;
			case n_TypeNotAttack: 
				{
					if( m_Effect_State[i] )
					{
						Rsprintf(RWCHAR(chStr), G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_NOT_ATTACK));
						m_TextEffectMsg[iCount]->SetText(chStr);	
						iCount++;
					}
					
				}break; 
			case n_TypeNotSkill: 
				{
					if( m_Effect_State[i] )
					{
						Rsprintf(RWCHAR(chStr),  G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_NOT_SKILL));
						m_TextEffectMsg[iCount]->SetText(chStr);	
						iCount++;
					}
				}
				break;
			case n_TypeNotCombo: 
				{
					if( m_Effect_State[i] )
					{
						Rsprintf(RWCHAR(chStr), G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_NOT_COMBO));
						m_TextEffectMsg[iCount]->SetText(chStr);	
						iCount++;
					}
				}
				break; 
			case n_TypeNotItem:	
				{
					if( m_Effect_State[i] )
					{
						Rsprintf(RWCHAR(chStr), G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_NOT_ITEMUSE));
						m_TextEffectMsg[iCount]->SetText(chStr);	
						iCount++;

					}
				}
				break;
			case n_TypeProvoke:
				{
					if( m_Effect_State[i] )
					{
						Rsprintf(RWCHAR(chStr), G_STRING_CHAR(IDS_SYSPR_ADDEFFECT_PROVOKE));
						m_TextEffectMsg[iCount]->SetText(chStr);	
						iCount++;
					}
				}
				break;
		}
	}
	
	m_BgImgBox->SetShowCount( iCount );

/*
	if( m_Effect_State[n_TypeNotMove] )
	{

	}
	else if(m_Effect_State[n_TypeNotSkill]  ) 
	{


	}
	else if(m_Effect_State[n_TypeNotCombo]  ) 
	{
		
		
	}
	else if(m_Effect_State[n_TypeNotItem]  ) 
	{
		
		
	}
	else if(m_Effect_State[n_TypeNotItem]  ) 
	{
		
		
	}
	else if(m_Effect_State[n_TypeNotItem]  ) 
	{
		
		
	}
	else if(m_Effect_State[n_TypeNotItem]  ) 
	{
		
		
	}
*/
	/*
  	n_TypeNotMove,				// �̵��Ұ�
	n_TypeNotAttack,			// ���ݺҰ�
	n_TypeNotSkill,				// ��ų�Ұ�
	n_TypeNotCombo,				// �޺��Ұ�
//	n_TypeNotSpelling,			// ��â�Ұ�
	n_TypeNotItem,				// ������ ��� �Ұ�

	// ���� Ȯ���� ���� ���� ���� 
//	n_TypeCancelAttack,			// ���ݹ޾��� �� ���� ����
//	n_TypeCritical,				// 100% ũ��Ƽ�� ���� ����
//	n_TypeReflexAttack,			// �ݰ�-�Ϲ�
//	n_TypeReflexSkill,			// �ݰ�-�ʻ��
	n_TypeProvoke,				// ����
//	n_TypeIndomitable,			// �ұ��� ����
	n_TypeAlive,				// ��Ȱ
	*/



}


