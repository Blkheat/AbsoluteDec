#include "..\\..\\RockClient.h"
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

#include "MBlockTabWnd.h"
#include "stringmanager.h"

CMBlockTabWnd::CMBlockTabWnd()
{
	m_pMBlockTabWnd = NULL;
	m_FocusOriginTxt = NULL;

	m_pBlockTitleField = NULL;

	memset( m_pBlockDataList, NULL, sizeof( SBlockData* ) * (MAX_SBLOCK_DATA_NUM + 1) );

	m_nNumOfBlock = 0;
	m_nStartListNum = 0;

	m_nSelectedBtn = -1;
}

CMBlockTabWnd::~CMBlockTabWnd()
{
	Init();
}

void CMBlockTabWnd::Init()
{
	for(int i = 0 ; i < (MAX_SBLOCK_DATA_NUM + 1) ; i++)
	{
		SAFE_DELETE(m_pBlockDataList[i]);
	}

	m_nNumOfBlock = 0;
	m_nStartListNum = 0;

	m_nSelectedBtn = -1;
}

void CMBlockTabWnd::Composition()
{
	
	m_pMBlockTabWnd = (CFrameWnd*)nRui->CreateWnd( n_wtFrameWnd, WID_MBlockTabWnd, WID_MCommunityBgUI, 
		30, 33, 333, 209, false );
	m_pMBlockTabWnd->SetClientImage( TID_None );
	m_pMBlockTabWnd->SetCaption( _RT("MBlockTab Window") );
	m_pMBlockTabWnd->SetWndProc( this );
	m_pMBlockTabWnd->SetTitle( false );
	m_pMBlockTabWnd->SetVisible( false );
	m_pMBlockTabWnd->Initialize();

	m_BackImg = (CImageBox*)nRui->CreateWnd( n_wtImageBox, WID_None, WID_MBlockTabWnd, 0, 0, 333, 29, false );   
	m_BackImg->SetClientImage( TID_MBACK_BLOCK );
	m_BackImg->SetVisible( true );
	

	m_pBlockTitleField = (CTextBox*)nRui->CreateWnd( n_wtTextBox, WID_None, WID_MBlockTabWnd, 
		0, 0, 333, 27, false );
	m_pBlockTitleField->SetAlignText(n_atCenter, n_atCenter);
	m_pBlockTitleField->SetClientImage(TID_None);
	m_pBlockTitleField->SetTextColor(255, 255, 200);
	m_pBlockTitleField->SetBlank( 0, 0, 0, 0 );
	m_pBlockTitleField->SetText( G_STRING_CHAR( IDS_WND_BLOCK_ID ) );				//HAN_MGS
	
	int iAddPosY = 17;
	
	for(int i = 0 ; i < MAX_BLOCKFIELD_LINE ; i++)
	{
		int xPos = ((i % 2) ? 162 : 6); 
		int yPos = (iAddPosY * (i/2)) + 36;		
		
		BlockField[i].pBDestName = (CLimitLenTextBox*)nRui->CreateWnd( n_wtLimitLenTextBox, WID_None, WID_MBlockTabWnd, 
		xPos, yPos , 156, 14, false );
		BlockField[i].pBDestName->SetClientImage(TID_None);
		BlockField[i].pBDestName->SetAlignText( n_atLeft, n_atCenter );
		BlockField[i].pBDestName->SetBlank( 5, 0, 0, 0 );		
		BlockField[i].pBDestName->SetText(_RT(""));
		BlockField[i].pBDestName->SetTextColor(255, 255, 255);
	}

	m_FocusOriginTxt = (CFrameTextBox*)nRui->CreateWnd( n_wtFrameTextBox, WID_None, WID_MBlockTabWnd, 
	0, 0, 0, 0, false );	
	m_FocusOriginTxt->SetAlignText( n_atLeft, n_atCenter);
	m_FocusOriginTxt->SetBlank( 5, 0, 0, 0 );
	m_FocusOriginTxt->SetText(_RT(""));			
	m_FocusOriginTxt->SetClientImage(TID_CLIENT);
	m_FocusOriginTxt->SetClientColor(55, 55, 74);
	m_FocusOriginTxt->SetFrameColor( D3DCOLOR_ARGB( 255, 151, 159, 186 ) );
	m_FocusOriginTxt->SetVisible(false);

	for(short i = 0 ; i < MAX_BLOCKFIELD_LINE ; i++ )
	{
		int xPos = ((i % 2) ? 162 : 6);
		int yPos = (iAddPosY * (i/2)) + 36;	
		
		BlockSelBtn[i] = (CButton*)nRui->CreateWnd( n_wtButton, (WID_MBlockSelBtn1 + i), WID_MBlockTabWnd, 
		xPos, yPos , 156, 17, false );
		BlockSelBtn[i]->SetClientImage(TID_None);
		BlockSelBtn[i]->SetClientAlpah(128);
		BlockSelBtn[i]->SetBtnImage(TID_None, TID_MBlockSelFocus, TID_MBlockSelFocus, TID_None);
		///BlockSelBtn[i]->SetBtnAlpha(128);
	}

	SetBlockField();
}

void CMBlockTabWnd::Update()
{
	static CLimitLenTextBox *pTextBox = NULL;
	static SBlockData *pBlockData = NULL;

	m_FocusOriginTxt->SetVisible(false);

	if(pTextBox)
	{
		pTextBox->SetVisible(true);		
	}

	POINT	ptCursor;
	GetCursorPos(&ptCursor);
	::ScreenToClient(g_RockClient.GetApp(), &ptCursor);

	static TCHAR strTemp[512] = {0,};

	SRect temRect;
	SStringInfo *pStringInfo = NULL;
	
	for(int i = 0 ; i < MAX_BLOCKFIELD_LINE ; i++ )
	{
		if( BlockField[i].pBDestName->FocusCheck(ptCursor.x, ptCursor.y, false) )
		{
			if(!BlockField[i].pBDestName->IsEqualString())
			{
				if(BlockField[i].pBlockData == NULL)
				{
					continue;
				}

				BlockField[i].pBDestName->GetClientSize(&temRect);
				pStringInfo = BlockField[i].pBDestName->GetStringInfo();			
				
				temRect.w = max((pStringInfo->Width + 10), 98);
				m_FocusOriginTxt->ResetSize(&temRect);			
				
				BlockField[i].pBDestName->GetText(strTemp, 512);
				m_FocusOriginTxt->SetText(strTemp);
				m_FocusOriginTxt->SetVisible(true);
				
				pTextBox = BlockField[i].pBDestName;				
				pTextBox->SetVisible(false);

				break;
			}
		}
	}
}

void CMBlockTabWnd::Proc( SEventMessage* EMsg )
{
	if( (WID_MBlockSelBtn1 <= EMsg->FocusWnd) && ( EMsg->FocusWnd <= (WID_MBlockSelBtn1 + 23) ) )
	{
		if( EMsg->Notify == NM_BTN_CLICK )
		{
			FieldBntSelected( (EMsg->FocusWnd - WID_MBlockSelBtn1) );				
		}
	}
}

void CMBlockTabWnd::InsertBlockData(SBlockData * pBlockData, bool bReNewFlag)
{
	if( m_nNumOfBlock < MAX_SBLOCK_DATA_NUM )
	{
		m_pBlockDataList[m_nNumOfBlock] = pBlockData;
		m_nNumOfBlock++;
		
		if(bReNewFlag)
		{
			SetBlockField();
		}		
	}
	else
	{
		SAFE_DELETE(pBlockData);	
	}
}


void CMBlockTabWnd::DeleteBlockData(TCHAR * strName, bool bReNewFlag)
{
	for(int i = 0; i < m_nNumOfBlock ; i++)
	{
		SBlockData *pBlockData = m_pBlockDataList[i];
		
		if( pBlockData && !Rstrcmp(strName, pBlockData->strBDestName) )
		{
			SAFE_DELETE(pBlockData);
			m_pBlockDataList[i] = NULL;
			
			for(int j = i ; j < m_nNumOfBlock ; j++)
			{
				m_pBlockDataList[j] = m_pBlockDataList[j + 1];
			}
			
			m_nNumOfBlock--;
			
			if(bReNewFlag)
			{
				SetBlockField();
			}

			break;
		}	
	}	
}

bool CMBlockTabWnd::ExistBlockName(TCHAR *strName)
{
	for(int i = 0; i < m_nNumOfBlock ; i++)
	{
		SBlockData *pBlockData = m_pBlockDataList[i];
		
		if( pBlockData && !Rstrcmp(strName, pBlockData->strBDestName) )
		{
			return true;
		}
	}

	return false;
}

void CMBlockTabWnd::SetBlockField()
{
	FieldBntSelected(-1);
	
	TCHAR tempStr[512] = {0,};
	
	for( int i = 0; i < MAX_BLOCKFIELD_LINE; ++i )
	{
		BlockField[i].pBlockData = m_pBlockDataList[m_nStartListNum + i];

		if(BlockField[i].pBlockData)
		{
			Rstrcpy(RWCHAR(tempStr), RWCHAR(BlockField[i].pBlockData->strBDestName));			
			BlockField[i].pBDestName->SetText(tempStr);
			BlockField[i].pBDestName->SetLimitLength(12);
		}
		else
		{
			BlockField[i].pBDestName->SetText(_RT(""));
			BlockField[i].pBDestName->SetLimitLength(12);
		}	
	}
}

void  CMBlockTabWnd::FieldBntSelected( int _num_btn )
{
	m_nSelectedBtn = _num_btn;

	for(int i = 0; i < MAX_BLOCKFIELD_LINE ; i++)
	{
		BlockSelBtn[i]->SetClientImage( TID_None );
	}

	if(m_nSelectedBtn != -1)
	{
		BlockSelBtn[m_nSelectedBtn]->SetClientImage( TID_MFriendSelFocus );
	}
}

void  CMBlockTabWnd::SetListScroll( int _linenum )
{
	FieldBntSelected(-1);
	m_nStartListNum = _linenum * 2;

	SetBlockField();
}

SBlockData * CMBlockTabWnd::GetSelectData()
{
	if(m_nSelectedBtn != -1)
	{
		return BlockField[m_nSelectedBtn].pBlockData;
	}

	return NULL;
}