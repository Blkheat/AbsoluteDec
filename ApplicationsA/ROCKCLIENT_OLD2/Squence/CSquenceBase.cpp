#include "CSquenceBase.h"
#include "CSquenceManager.h"

CSquenceBase::CSquenceBase()
{
	m_pFuncList[0].Insert(this, &CSquenceBase::Squence00);
	m_pFuncList[1].Insert(this, &CSquenceBase::Squence01);
	m_pFuncList[2].Insert(this, &CSquenceBase::Squence02);
	m_pFuncList[3].Insert(this, &CSquenceBase::Squence03);
	m_pFuncList[4].Insert(this, &CSquenceBase::Squence04);
	m_pFuncList[5].Insert(this, &CSquenceBase::Squence05);
	m_pFuncList[6].Insert(this, &CSquenceBase::Squence06);
	m_pFuncList[7].Insert(this, &CSquenceBase::Squence07);
	m_pFuncList[8].Insert(this, &CSquenceBase::Squence08);
	m_pFuncList[9].Insert(this, &CSquenceBase::Squence09);
	m_pFuncList[10].Insert(this, &CSquenceBase::Squence10);
	m_pFuncList[11].Insert(this, &CSquenceBase::Squence11);
	m_pFuncList[12].Insert(this, &CSquenceBase::Squence12);
	m_pFuncList[13].Insert(this, &CSquenceBase::Squence13);
	m_pFuncList[14].Insert(this, &CSquenceBase::Squence14);
	m_pFuncList[15].Insert(this, &CSquenceBase::Squence15);
	m_pFuncList[16].Insert(this, &CSquenceBase::Squence16);
	m_pFuncList[17].Insert(this, &CSquenceBase::Squence17);
	m_pFuncList[18].Insert(this, &CSquenceBase::Squence18);
	m_pFuncList[19].Insert(this, &CSquenceBase::Squence19);
	m_pFuncList[20].Insert(this, &CSquenceBase::Squence20);
	m_pFuncList[21].Insert(this, &CSquenceBase::Squence21);
	m_pFuncList[22].Insert(this, &CSquenceBase::Squence22);
	m_pFuncList[23].Insert(this, &CSquenceBase::Squence23);
	m_pFuncList[24].Insert(this, &CSquenceBase::Squence24);
	m_pFuncList[25].Insert(this, &CSquenceBase::Squence25);
	m_pFuncList[26].Insert(this, &CSquenceBase::Squence26);
	m_pFuncList[27].Insert(this, &CSquenceBase::Squence27);
	m_pFuncList[28].Insert(this, &CSquenceBase::Squence28);
	m_pFuncList[29].Insert(this, &CSquenceBase::Squence29);


	m_pFuncCheckList[0].Insert(this, &CSquenceBase::SquenceCheck00);
	m_pFuncCheckList[1].Insert(this, &CSquenceBase::SquenceCheck01);
	m_pFuncCheckList[2].Insert(this, &CSquenceBase::SquenceCheck02);
	m_pFuncCheckList[3].Insert(this, &CSquenceBase::SquenceCheck03);
	m_pFuncCheckList[4].Insert(this, &CSquenceBase::SquenceCheck04);
	m_pFuncCheckList[5].Insert(this, &CSquenceBase::SquenceCheck05);
	m_pFuncCheckList[6].Insert(this, &CSquenceBase::SquenceCheck06);
	m_pFuncCheckList[7].Insert(this, &CSquenceBase::SquenceCheck07);
	m_pFuncCheckList[8].Insert(this, &CSquenceBase::SquenceCheck08);
	m_pFuncCheckList[9].Insert(this, &CSquenceBase::SquenceCheck09);
	m_pFuncCheckList[10].Insert(this, &CSquenceBase::SquenceCheck10);
	m_pFuncCheckList[11].Insert(this, &CSquenceBase::SquenceCheck11);
	m_pFuncCheckList[12].Insert(this, &CSquenceBase::SquenceCheck12);
	m_pFuncCheckList[13].Insert(this, &CSquenceBase::SquenceCheck13);
	m_pFuncCheckList[14].Insert(this, &CSquenceBase::SquenceCheck14);
	m_pFuncCheckList[15].Insert(this, &CSquenceBase::SquenceCheck15);
	m_pFuncCheckList[16].Insert(this, &CSquenceBase::SquenceCheck16);
	m_pFuncCheckList[17].Insert(this, &CSquenceBase::SquenceCheck17);
	m_pFuncCheckList[18].Insert(this, &CSquenceBase::SquenceCheck18);
	m_pFuncCheckList[19].Insert(this, &CSquenceBase::SquenceCheck19);
	m_pFuncCheckList[20].Insert(this, &CSquenceBase::SquenceCheck20);
	m_pFuncCheckList[21].Insert(this, &CSquenceBase::SquenceCheck21);
	m_pFuncCheckList[22].Insert(this, &CSquenceBase::SquenceCheck22);
	m_pFuncCheckList[23].Insert(this, &CSquenceBase::SquenceCheck23);
	m_pFuncCheckList[24].Insert(this, &CSquenceBase::SquenceCheck24);
	m_pFuncCheckList[25].Insert(this, &CSquenceBase::SquenceCheck25);
	m_pFuncCheckList[26].Insert(this, &CSquenceBase::SquenceCheck26);
	m_pFuncCheckList[27].Insert(this, &CSquenceBase::SquenceCheck27);
	m_pFuncCheckList[28].Insert(this, &CSquenceBase::SquenceCheck28);
	m_pFuncCheckList[29].Insert(this, &CSquenceBase::SquenceCheck29);

	m_pFuncFirstSceneList[0].Insert(this, &CSquenceBase::SquenceFirstScene00);
	m_pFuncFirstSceneList[1].Insert(this, &CSquenceBase::SquenceFirstScene01);
	m_pFuncFirstSceneList[2].Insert(this, &CSquenceBase::SquenceFirstScene02);
	m_pFuncFirstSceneList[3].Insert(this, &CSquenceBase::SquenceFirstScene03);
	m_pFuncFirstSceneList[4].Insert(this, &CSquenceBase::SquenceFirstScene04);
	m_pFuncFirstSceneList[5].Insert(this, &CSquenceBase::SquenceFirstScene05);
	m_pFuncFirstSceneList[6].Insert(this, &CSquenceBase::SquenceFirstScene06);
	m_pFuncFirstSceneList[7].Insert(this, &CSquenceBase::SquenceFirstScene07);
	m_pFuncFirstSceneList[8].Insert(this, &CSquenceBase::SquenceFirstScene08);
	m_pFuncFirstSceneList[9].Insert(this, &CSquenceBase::SquenceFirstScene09);
	m_pFuncFirstSceneList[10].Insert(this, &CSquenceBase::SquenceFirstScene10);
	m_pFuncFirstSceneList[11].Insert(this, &CSquenceBase::SquenceFirstScene11);
	m_pFuncFirstSceneList[12].Insert(this, &CSquenceBase::SquenceFirstScene12);
	m_pFuncFirstSceneList[13].Insert(this, &CSquenceBase::SquenceFirstScene13);
	m_pFuncFirstSceneList[14].Insert(this, &CSquenceBase::SquenceFirstScene14);
	m_pFuncFirstSceneList[15].Insert(this, &CSquenceBase::SquenceFirstScene15);
	m_pFuncFirstSceneList[16].Insert(this, &CSquenceBase::SquenceFirstScene16);
	m_pFuncFirstSceneList[17].Insert(this, &CSquenceBase::SquenceFirstScene17);
	m_pFuncFirstSceneList[18].Insert(this, &CSquenceBase::SquenceFirstScene18);
	m_pFuncFirstSceneList[19].Insert(this, &CSquenceBase::SquenceFirstScene19);
	m_pFuncFirstSceneList[20].Insert(this, &CSquenceBase::SquenceFirstScene20);
	m_pFuncFirstSceneList[21].Insert(this, &CSquenceBase::SquenceFirstScene21);
	m_pFuncFirstSceneList[22].Insert(this, &CSquenceBase::SquenceFirstScene22);
	m_pFuncFirstSceneList[23].Insert(this, &CSquenceBase::SquenceFirstScene23);
	m_pFuncFirstSceneList[24].Insert(this, &CSquenceBase::SquenceFirstScene24);
	m_pFuncFirstSceneList[25].Insert(this, &CSquenceBase::SquenceFirstScene25);
	m_pFuncFirstSceneList[26].Insert(this, &CSquenceBase::SquenceFirstScene26);
	m_pFuncFirstSceneList[27].Insert(this, &CSquenceBase::SquenceFirstScene27);
	m_pFuncFirstSceneList[28].Insert(this, &CSquenceBase::SquenceFirstScene28);
	m_pFuncFirstSceneList[29].Insert(this, &CSquenceBase::SquenceFirstScene29);


	m_pFuncAniFrameSceneList[0].Insert(this, &CSquenceBase::SquenceAniFrame00);
	m_pFuncAniFrameSceneList[1].Insert(this, &CSquenceBase::SquenceAniFrame01);
	m_pFuncAniFrameSceneList[2].Insert(this, &CSquenceBase::SquenceAniFrame02);
	m_pFuncAniFrameSceneList[3].Insert(this, &CSquenceBase::SquenceAniFrame03);
	m_pFuncAniFrameSceneList[4].Insert(this, &CSquenceBase::SquenceAniFrame04);
	m_pFuncAniFrameSceneList[5].Insert(this, &CSquenceBase::SquenceAniFrame05);
	m_pFuncAniFrameSceneList[6].Insert(this, &CSquenceBase::SquenceAniFrame06);
	m_pFuncAniFrameSceneList[7].Insert(this, &CSquenceBase::SquenceAniFrame07);
	m_pFuncAniFrameSceneList[8].Insert(this, &CSquenceBase::SquenceAniFrame08);
	m_pFuncAniFrameSceneList[9].Insert(this, &CSquenceBase::SquenceAniFrame09);
	m_pFuncAniFrameSceneList[10].Insert(this, &CSquenceBase::SquenceAniFrame10);
	m_pFuncAniFrameSceneList[11].Insert(this, &CSquenceBase::SquenceAniFrame11);
	m_pFuncAniFrameSceneList[12].Insert(this, &CSquenceBase::SquenceAniFrame12);
	m_pFuncAniFrameSceneList[13].Insert(this, &CSquenceBase::SquenceAniFrame13);
	m_pFuncAniFrameSceneList[14].Insert(this, &CSquenceBase::SquenceAniFrame14);
	m_pFuncAniFrameSceneList[15].Insert(this, &CSquenceBase::SquenceAniFrame15);
	m_pFuncAniFrameSceneList[16].Insert(this, &CSquenceBase::SquenceAniFrame16);
	m_pFuncAniFrameSceneList[17].Insert(this, &CSquenceBase::SquenceAniFrame17);
	m_pFuncAniFrameSceneList[18].Insert(this, &CSquenceBase::SquenceAniFrame18);
	m_pFuncAniFrameSceneList[19].Insert(this, &CSquenceBase::SquenceAniFrame19);
	m_pFuncAniFrameSceneList[20].Insert(this, &CSquenceBase::SquenceAniFrame20);
	m_pFuncAniFrameSceneList[21].Insert(this, &CSquenceBase::SquenceAniFrame21);
	m_pFuncAniFrameSceneList[22].Insert(this, &CSquenceBase::SquenceAniFrame22);
	m_pFuncAniFrameSceneList[23].Insert(this, &CSquenceBase::SquenceAniFrame23);
	m_pFuncAniFrameSceneList[24].Insert(this, &CSquenceBase::SquenceAniFrame24);
	m_pFuncAniFrameSceneList[25].Insert(this, &CSquenceBase::SquenceAniFrame25);
	m_pFuncAniFrameSceneList[26].Insert(this, &CSquenceBase::SquenceAniFrame26);
	m_pFuncAniFrameSceneList[27].Insert(this, &CSquenceBase::SquenceAniFrame27);
	m_pFuncAniFrameSceneList[28].Insert(this, &CSquenceBase::SquenceAniFrame28);
	m_pFuncAniFrameSceneList[29].Insert(this, &CSquenceBase::SquenceAniFrame29);


	m_pSquenceManager = NULL;
}

CSquenceBase::~CSquenceBase()
{
	m_pSquenceManager = NULL;
}

void CSquenceBase::SetWaitFlag(SQUENCE_WAIT_TYPE Value,DWORD Timer)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->m_WaitTye = Value;
	m_pSquenceManager->m_Timer   = Timer;
}

void  CSquenceBase::NextSquence(void)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->m_WaitTye = SWT_NONE;
	m_pSquenceManager->m_Timer   = 0;
}

void CSquenceBase::SetPlayerAni(int AniIndex)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->SetPlayerAni( AniIndex );
}

void CSquenceBase::SetPlayerNextAni(int AniIndex)
{
	m_pSquenceManager->SetPlayerNextAni( AniIndex );
}

// 이펙트 생성 모드 변경
void CSquenceBase::EffectLock(void)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->EffectSquenceLock();
}

// 이펙트 생성 모드 기존 모드로
void CSquenceBase::EffectUnLock(void)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->EffectSquenceUnLock();
}

void CSquenceBase::PlaySound(int Index,float fx,float fy,float fz)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->PlaySquenceSound( Index , fx ,fy ,fx );
}

void CSquenceBase::PlaySound(int Index,D3DXVECTOR3 *pvPos)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->PlaySquenceSound( Index , pvPos );
}

void CSquenceBase::UpdateSound(int Index)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->UpdateSound( Index );
}

void CSquenceBase::StopAllSound(void)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->StopAllSound();
}

void CSquenceBase::SetPlayerAniTime(float Range)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->SetPlayerAniTime( Range );
}

void CSquenceBase::SetOtherPlayerAniTime(float Range)
{
	if( m_pSquenceManager == NULL )
		return;

	m_pSquenceManager->SetOtherPlayerAniTime( Range );
}
