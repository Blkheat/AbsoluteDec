#include "..\\..\\RockClient.h"
#include "..\\..\\brother.h"
#include "..\\RockPCH.h"
#include "Rui.h"

#include "Define.h"
#include "Util.h"
#include "Render.h"
#include "Fontman.h"
#include "Resource.h"
#include "Chain.h"
#include "Container.h"

#include "FrameWnd.h"
#include "Button.h"
#include "ImageBox.h"
#include "TextBox.h"
#include "EditBox.h"
#include "ProgressBar.h"
#include "SlideBar.h"
#include "CheckButton.h"
#include "RadioButton.h"
#include "ListBox.h"
#include "ScrollBar.h"
#include "ComboBox.h"
#include "Wnd.h"

#include "..\\..\\global.h"

#include "..\\..\\quadlist.h"
#include "..\\..\\bravolibs\\network\\Protocol.h"
#include "..\\..\\Pc.h"

#include "UiUtil.h"

#include "MessageWndProc.h"
#include "MCommunityWnd.h"
#include "MMsgWnd.h"
#include "stringmanager.h"

CMMsgWnd::CMMsgWnd()
{
	m_pMMsgFrame = NULL;

	m_pMMsgOkBtn = NULL;
	m_pMMsgCancleBtn = NULL;
	m_pMMsgInEdit = NULL;

	m_pMMsgTextBox = NULL;
	
	m_Msg_Type = MSG_MINPUTFRIEND;
	memset(m_strParam , NULL , MAX_SIZE_256);
	
}

CMMsgWnd::~CMMsgWnd()
{


}

void CMMsgWnd::Init()
{


}

void CMMsgWnd::Composition()
{
	m_pMMsgFrame = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MMsgWnd, WID_MCommunityBgUI, 
	30, 33, 346, 241, false );
	m_pMMsgFrame->SetClientImage( TID_None );//--^^TID_MConfirmBgUI);
	m_pMMsgFrame->SetCaption( _RT("MMsg Window") );
    m_pMMsgFrame->SetWndProc( this );
    m_pMMsgFrame->SetTitle( false );
    m_pMMsgFrame->SetVisible( false );    
    m_pMMsgFrame->SetZorder( n_zoTop );
    m_pMMsgFrame->Initialize();

	m_pMMsgImgBox = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MMsgWnd, 0, 0, 346, 209, false );   
	m_pMMsgImgBox ->SetClientImage( TID_MTEMP_BACK ); //-- ^^ TID_MSubBtnBgUI);

	//확인 버튼
	m_pMMsgOkBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MMsgOkBtn, WID_MMsgWnd, 
										80, 214, 58, 25, false ); 	
	m_pMMsgOkBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );	
	m_pMMsgOkBtn->SetFontR( n_fRsvFontWndButton ); 	
	m_pMMsgOkBtn->SetText(_RT(""));
	


	//취소 버튼
	m_pMMsgCancleBtn = (CButton*)nRui->CreateWnd( n_wtButton, WID_MMsgCancleBtn, WID_MMsgWnd, 
										200, 214, 58, 25, false );	
	m_pMMsgCancleBtn->SetBtnImage( TID_CTRL_BTN58, TID_CTRL_BTN58_OVR, TID_CTRL_BTN58_CLK, TID_None );		
	m_pMMsgCancleBtn->SetFontR( n_fRsvFontWndButton ); 	
	m_pMMsgCancleBtn->SetText(_RT(""));
	

	m_pMMsgInEdit = (CEditBox*)nRui->CreateWnd( n_wtEditBox, WID_MMsgInEdit, WID_MMsgWnd, 
									100, 104, 137, 17, false );
	m_pMMsgInEdit->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );
	m_pMMsgInEdit->SetTextSize( RP_MAX_PC_NAME-1 );
	m_pMMsgInEdit->SetClientImage( TID_CLIENT );
	m_pMMsgInEdit->SetClientColor(50, 50, 50);
	m_pMMsgInEdit->SetAlignText( n_atLeft, n_atCenter );
	m_pMMsgInEdit->SetBlank( 4, 0, 0, 0 );
	m_pMMsgInEdit->SetEditStyle( esText );
	m_pMMsgInEdit->SetText( _RT("") );
	m_pMMsgInEdit->SetReturnActionClear( false );
	m_pMMsgInEdit->InitEditBox();
	m_pMMsgInEdit->SetVisible(FALSE);
	
	
	m_pMMsgTextBox = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MMsgWnd, 
	                                0, 64, 333, 15, false );
    m_pMMsgTextBox->SetFontg( n_ftGulimChe, 12, n_fwNormal, n_fdNormal );   
    m_pMMsgTextBox->SetClientImage( TID_None );
    m_pMMsgTextBox->SetClientColor( 180, 175, 160 );
    m_pMMsgTextBox->SetAlignText( n_atCenter, n_atCenter );   
    m_pMMsgTextBox->SetText( _RT("") );
    m_pMMsgTextBox->SetTextColor( 234, 233, 211 );   
    m_pMMsgTextBox->SetZorder( n_zoTop );
	m_pMMsgTextBox->SetMultiLine( false );
}

void CMMsgWnd::Update()
{


}

void CMMsgWnd::Proc( SEventMessage* EMsg )
{
	if( EMsg->Notify == NM_BTN_CLICK )
	{
		BtnClickProc( EMsg->FocusWnd );				
	}
	
	if( EMsg->Notify == NM_EDT_RETURN_ACTION )
	{
		//확인버튼과 같은처리
		BtnClickProc( WID_MMsgOkBtn );
		nRui->SetEnterFlag( n_nfEditEnter );
	}
}

void CMMsgWnd::BtnClickProc( int WIDBtn )
{
	static TCHAR m_strTemp[256] = {0,};

	switch(m_Msg_Type)
	{
		case MSG_MINPUTFRIEND:
			{
				if( WIDBtn == WID_MMsgOkBtn )
				{
								
					m_pMMsgInEdit->GetText( m_strParam, 20 );
					m_pMMsgInEdit->SetText( _RT("") );

					if( Rstrcmp(m_strParam, _RT("")) != 0 )
					{
						if(g_MCommunityWnd.m_MFriendTabWnd.FindFriendData(m_strParam))
						{
							MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_AREADY_FRIEND ), 1500, NULL);	
						}
						else
						{						
							g_RockClient.Send_CsMsngrMemberInsert(m_strParam);
						}
						
						SetVisible(false);
						g_MCommunityWnd.SetMessengerState(n_MS_Friend, false);
					}
					else
					{
						nRui->SendEvent( WID_MMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
					}
				}
				if( WIDBtn == WID_MMsgCancleBtn )
				{
					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Friend, false);
				}				
			}
			break;
		case MSG_MDELETEFRIEND:
			{
				if( WIDBtn == WID_MMsgOkBtn )
				{
					g_RockClient.Send_CsMsngrMemberDelete(m_strParam);							
					
					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Friend, false);
				}
				if( WIDBtn == WID_MMsgCancleBtn )
				{
					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Friend, false);
				}
			}
			break;
		case MSG_MINPUTBLOCK:
			{
				if( WIDBtn == WID_MMsgOkBtn )
				{
					m_pMMsgInEdit->GetText( m_strParam, 20 );
					m_pMMsgInEdit->SetText( _RT("") );

					if( Rstrcmp(m_strParam, _RT("")) != 0 )
					{
						if(g_MCommunityWnd.m_MBlockTabWnd.ExistBlockName(m_strParam))
						{
							MessageBoxWnd.Open( G_STRING_CHAR( IDS_SYSPR_AREADY_BLOCK ), 1500, NULL);							
						}
						else
						{
							g_RockClient.Send_CsMsngrMemberBlock((BYTE)n_MsngrInsert, m_strParam );
						}
						
						SetVisible(false);
						g_MCommunityWnd.SetMessengerState(n_MS_Block, false);
					}
					else
					{
						nRui->SendEvent( WID_MMsgInEdit, n_emSetFocus, 0, 0, 0, 0 );
					}					
				}
				if( WIDBtn == WID_MMsgCancleBtn )
				{							
					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Block, false);
				}
			}
			break;
		case MSG_MDELETEBLOCK:
			{
				if( WIDBtn == WID_MMsgOkBtn )
				{					
					g_RockClient.Send_CsMsngrMemberBlock((BYTE)n_MsngrDelete, m_strParam );

					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Block, false);
				}
				if( WIDBtn == WID_MMsgCancleBtn )
				{					
					SetVisible(false);
					g_MCommunityWnd.SetMessengerState(n_MS_Block, false);
				}
			}
			break;	
	}
}

void CMMsgWnd::SetParam( TCHAR * strName )
{
	Rstrcpy( RWCHAR(m_strParam), RWCHAR(strName) );
}

void CMMsgWnd::SetMsgType( UINT MsgType )
{
	TCHAR strTempBuf[512] = {0,};
	m_Msg_Type = MsgType;

	m_pMMsgInEdit->SetVisible(FALSE);
	
	switch(m_Msg_Type)
	{
		case MSG_MINPUTFRIEND:
			{
				m_pMMsgInEdit->SetVisible(TRUE);

				m_pMMsgTextBox->SetText( G_STRING_CHAR( IDS_WND_MMSG_INPUT_FR ) );		    				
				m_pMMsgOkBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );							    
				m_pMMsgCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ));						    
			}
			break;
		case MSG_MDELETEFRIEND:
			{
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_MMSG_DEL ), RWCHAR(m_strParam));	
				m_pMMsgTextBox->SetText( strTempBuf );		
				m_pMMsgOkBtn->SetText(G_STRING_CHAR( IDS_WND_COM_YES ));								
				m_pMMsgCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_NO ) );						
			}
			break;
		case MSG_MINPUTBLOCK:
			{
			    m_pMMsgInEdit->SetVisible(TRUE);	

				m_pMMsgTextBox->SetText( G_STRING_CHAR( IDS_WND_MMSG_INPUT_BLOCK ) );								
				m_pMMsgOkBtn->SetText( G_STRING_CHAR( IDS_WND_COM_OK ) );								
				m_pMMsgCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_CANCEL ));							

			}
			break;
		case MSG_MDELETEBLOCK:
			{
				
				Rsprintf( RWCHAR(strTempBuf), G_STRING_CHAR( IDS_WND_MMSG_CANCEL_BLOCK ), RWCHAR(m_strParam));	
				m_pMMsgTextBox->SetText( strTempBuf );								
				m_pMMsgOkBtn->SetText( G_STRING_CHAR( IDS_WND_COM_YES ) );								
				m_pMMsgCancleBtn->SetText( G_STRING_CHAR( IDS_WND_COM_NO ));						
			}
			break;
	}
}

void CMMsgWnd::SetVisible(bool bFlag)
{
	if(bFlag)
	{
		nRui->SendEvent( WID_MMsgWnd, n_emSetVisible, (EPARAM)true, 0, 0, 0 );
	}
	else
	{	
		m_pMMsgInEdit->ClearText();
		m_pMMsgInEdit->GetWindowEditText();
		nRui->SendEvent( WID_MMsgWnd, n_emSetVisible, (EPARAM)false, 0, 0, 0 );
	}
}