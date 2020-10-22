#include "CLoginShutter.h"
#include "..\\..\\CRenderManager.h"
#include "EmblemTexManager.h"


#define SHUTTER_HEIGHT		91

#ifdef DIRECT_VERSION_9_MJH
extern IDirect3DDevice9*	g_lpDevice;
#else
extern IDirect3DDevice8*	g_lpDevice;
#endif // DIRECT_VERSION_9_MJH


CLoginShutter::CLoginShutter()
{
	m_State = SHUTTER_DN_STOP;
}

CLoginShutter::~CLoginShutter()
{
}

void CLoginShutter::Start(LOGIN_SHUTTER_STATE State,DWORD Time)
{
	m_State     = State;
	m_RangeTime = Time;
	m_OldTimer  = timeGetTime();
}

void CLoginShutter::Update(void)
{
	if( SHUTTER_UP_STOP == m_State )
		return;

	//............................................................................................................
	// 贸府 风凭
	//............................................................................................................
	int UpY = 0;
	int DnY = 768 - SHUTTER_HEIGHT;

	DWORD Time  = timeGetTime();
	float fRange;

	switch( m_State )
	{
		case SHUTTER_UP_PROCESS:
			
			fRange = ( float ) ( Time - m_OldTimer ) / ( float ) m_RangeTime;
			
			if( fRange >= 1.0f )
			{
				m_State = SHUTTER_UP_STOP;

				return;
			}

			UpY    = UpY - ( fRange * SHUTTER_HEIGHT );
			DnY    = DnY + ( fRange * SHUTTER_HEIGHT );
			
			break;
		
		case SHUTTER_DN_PROCESS:
			
			fRange = ( float ) ( Time - m_OldTimer ) / ( float ) m_RangeTime;
			
			if( fRange >= 1.0f )
			{
				m_State = SHUTTER_DN_STOP;
			}
			else
			{
				UpY    = -SHUTTER_HEIGHT + ( fRange * SHUTTER_HEIGHT );
				DnY    = 768 - ( fRange * SHUTTER_HEIGHT );
			}
			
			break;
	}

	Render->SetState();
	//............................................................................................................
	// 嘛绰 风凭
	//............................................................................................................
	Render->Draw( TID_Shutter_Up , 0 , UpY , 1024 , SHUTTER_HEIGHT , D3DCOLOR_XRGB( 255 , 255 , 255 ) );
	Render->Draw( TID_Shutter_Dn , 0 , DnY , 1024 , SHUTTER_HEIGHT , D3DCOLOR_XRGB( 255 , 255 , 255 ) );
	
	Render->ReleaseState();
}

void CLoginShutter::Reset(void)
{
	m_State = SHUTTER_DN_STOP;
}

