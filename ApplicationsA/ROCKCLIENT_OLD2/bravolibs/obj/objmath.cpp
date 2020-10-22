#include "RockPCH.h"

#include	<Base\\D3DBase.h>
#include	"..\\quadlist.h"
#include	"..\\RockClient.h"
#include	<Map\\Field.h>
#include	"..\\Map.H"
//#include	"..\\Weather_Manager.H"
#include	"ObjMath.H"
#include	<math.h>


#define		ANGLE	0.01745f	// 회전각. 라디안
extern		int		g_chr_face;


/*
//-----------------------------------------------------------------------------
// Desc: 라디안
//-----------------------------------------------------------------------------
Radian g_Radian;


void Radian::Clear()
{
	memset( &m_radian, 0, sizeof(m_radian) );

	// 라디안값 만들기 
	for( unsigned int i = 0; i < 361; ++i )
	{
		m_radian[ i ] = i * ANGLE;
	}
}

void Radian::Destory()
{


}


//-----------------------------------------------------------------------------
// Desc: 코사인
//-----------------------------------------------------------------------------
Cos	g_Cos;


void Cos::Clear()
{
	memset( &m_cos, 0, sizeof(m_cos) );

	// 코사인값 만들기
	for( unsigned int i = 0; i < 361; ++i )
	{
		m_cos[ i ] = (float)cos( g_Radian.m_radian[ i ] );
	}
}

void Cos::Destory()
{

}


//-----------------------------------------------------------------------------
// Desc: 사인
//-----------------------------------------------------------------------------
Sin		g_Sin;


void Sin::Clear()
{
	memset( &m_sin, 0, sizeof(m_sin) );

	// 싸인값 만들기	
	for( unsigned int i = 0; i < 361; ++i )
	{
		float radian = i * ANGLE;
		m_sin[ i ] = (float)sin( g_Radian.m_radian[ i ] );
	}
}

void Sin::Destory()
{

}
*/

//-----------------------------------------------------------------------------
// Desc: 변수값 초기화
//-----------------------------------------------------------------------------
void	EtcTable::Clear()
{
	//-----------------------------------------------------------------------------
	// 요정애니메이션 인덱스
	//-----------------------------------------------------------------------------	
	memset( &m_fairy_ani, -1, sizeof(m_fairy_ani) );

	m_fairy_ani[0]   = 0;	// 서있기
	m_fairy_ani[1]   = 1;	// 걷기
	m_fairy_ani[2]   = 4;	// 무기들고 걷기
	m_fairy_ani[7]   = 2;	// 달리기
	m_fairy_ani[8]   = 5;	// 무기들고 달리기
	m_fairy_ani[19]  = 6;	// 죽기
	m_fairy_ani[20]  = 7;	// 로그인	
	m_fairy_ani[25]  = 8;	// 인사하기
	m_fairy_ani[26]  = 9;	// 인사하기
	m_fairy_ani[30]  = 12;	// 칼넣고,빼기
	m_fairy_ani[37]  = 10;	// 앉기
	m_fairy_ani[38]  = 11;	// 앉아있기
	m_fairy_ani[47]  = 3;	// 무기들고 서있기
	m_fairy_ani[48]  = 3;	// 무기들고 서있기
	m_fairy_ani[87]  = 87;
	m_fairy_ani[88]  = 88;
	m_fairy_ani[89]  = 89;
	m_fairy_ani[90]  = 90;
	m_fairy_ani[91]  = 91;
	m_fairy_ani[92]  = 92;
	m_fairy_ani[93]  = 93;
	m_fairy_ani[94]  = 94;
	m_fairy_ani[95]  = 95;
	m_fairy_ani[96]  = 96;
	m_fairy_ani[97]  = 97;
	m_fairy_ani[98]  = 98;
	m_fairy_ani[99]  = 99;
	m_fairy_ani[100] = 100;
	m_fairy_ani[101] = 101;
	m_fairy_ani[102] = 102;
	m_fairy_ani[103] = 103;
	m_fairy_ani[104] = 104;
	m_fairy_ani[105] = 105;
	m_fairy_ani[106] = 106;
	m_fairy_ani[107] = 107;
	m_fairy_ani[108] = 108;
	m_fairy_ani[109] = 109;
	m_fairy_ani[110] = 110;
	m_fairy_ani[111] = 111;
	m_fairy_ani[112] = 112;
	m_fairy_ani[113] = 113;
	m_fairy_ani[127] = 132;
	m_fairy_ani[128] = 133;
	m_fairy_ani[129] = 134;

	//-----------------------------------------------------------------------------
	// 머리색깔, 머리종류, 얼굴 종류
	//-----------------------------------------------------------------------------	
	// 얼굴
	m_face_type[0] = 1;
	m_face_type[1] = 93;
	m_face_type[2] = 94;
	m_face_type[3] = 95;
	m_face_type[4] = -1;
	m_face_type[5] = -1;
	m_face_type[6] = -1;
	m_face_type[7] = -1;
	m_face_type[8] = -1;
	m_face_type[9] = -1;	

	// 머리
	m_hair_type[0] = 0;			// 약간긴머리
	m_hair_type[1] = 57;		// 범생머리
	m_hair_type[2] = 64;		// 구리뱅뱅
	m_hair_type[3] = 65;		// 짧은머리
	m_hair_type[4] = 312;		// 폭탄머리
	m_hair_type[5] = -1;
	m_hair_type[6] = -1;
	m_hair_type[7] = -1;
	m_hair_type[8] = -1;
	m_hair_type[9] = -1;	

	// 머리색
	m_hair_color[0].r = (float)(186.0f/255.0f);
	m_hair_color[0].g = (float)(118.0f/255.0f);
	m_hair_color[0].b = (float)(63.0f/255.0f);

	m_hair_color[1].r = (float)(238.0f/255.0f);
	m_hair_color[1].g = (float)(93.0f/255.0f);
	m_hair_color[1].b = (float)(56.0f/255.0f);

	m_hair_color[2].r = (float)(65.0f/255.0f);
	m_hair_color[2].g = (float)(95.0f/255.0f);
	m_hair_color[2].b = (float)(223.0f/255.0f);

	m_hair_color[3].r = (float)(63.0f/255.0f);
	m_hair_color[3].g = (float)(186.0f/255.0f);
	m_hair_color[3].b = (float)(138.0f/255.0f);

	m_hair_color[4].r = (float)(46.0f/255.0f);
	m_hair_color[4].g = (float)(30.0f/255.0f);
	m_hair_color[4].b = (float)(24.0f/255.0f);

	m_hair_color[5].r = (float)(114.0f/255.0f);
	m_hair_color[5].g = (float)(69.0f/255.0f);
	m_hair_color[5].b = (float)(151.0f/255.0f);

	m_hair_color[6].r = (float)(115.0f/255.0f);
	m_hair_color[6].g = (float)(109.0f/255.0f);
	m_hair_color[6].b = (float)(121.0f/255.0f);

	m_hair_color[7].r = (float)(225.0f/255.0f);
	m_hair_color[7].g = (float)(126.0f/255.0f);
	m_hair_color[7].b = (float)(39.0f/255.0f);

	m_hair_color[8].r = (float)(240.0f/255.0f);
	m_hair_color[8].g = (float)(240.0f/255.0f);
	m_hair_color[8].b = (float)(240.0f/255.0f);

	m_hair_color[9].r = (float)(255.0f/255.0f);
	m_hair_color[9].g = (float)(215.0f/255.0f);
	m_hair_color[9].b = (float)(93.0f/255.0f);

	//-----------------------------------------------------------------------------
	// 메터리얼값
	//-----------------------------------------------------------------------------
	// 새벽
	m_TimeColor[0].diffuse.r =  0.4f;
	m_TimeColor[0].diffuse.g =  0.8f;
	m_TimeColor[0].diffuse.b =  0.6f;
	m_TimeColor[0].ambient.r =  0.7f;
	m_TimeColor[0].ambient.g =  0.7f;
	m_TimeColor[0].ambient.b =  0.7f;

	// 아침
	m_TimeColor[1].diffuse.r =  0.8f;
	m_TimeColor[1].diffuse.g =  0.7f;
	m_TimeColor[1].diffuse.b =  0.5f;
	m_TimeColor[1].ambient.r =  0.7f;
	m_TimeColor[1].ambient.g =  0.7f;
	m_TimeColor[1].ambient.b =  0.7f;

	// 점심
	m_TimeColor[2].diffuse.r =  0.8f;
	m_TimeColor[2].diffuse.g =  0.7f;
	m_TimeColor[2].diffuse.b =  0.3f;
	m_TimeColor[2].ambient.r =  0.7f;
	m_TimeColor[2].ambient.g =  0.7f;
	m_TimeColor[2].ambient.b =  0.7f;

	// 노을 
	m_TimeColor[3].diffuse.r =  0.8f;
	m_TimeColor[3].diffuse.g =  0.5f;
	m_TimeColor[3].diffuse.b =  0.4f;
	m_TimeColor[3].ambient.r =  0.7f;
	m_TimeColor[3].ambient.g =  0.7f;
	m_TimeColor[3].ambient.b =  0.7f;

	// 저녁
	m_TimeColor[4].diffuse.r =  0.4f;
	m_TimeColor[4].diffuse.g =  0.8f;
	m_TimeColor[4].diffuse.b =  0.6f;
	m_TimeColor[4].ambient.r =  0.7f;
	m_TimeColor[4].ambient.g =  0.7f;
	m_TimeColor[4].ambient.b =  0.7f;

	// 밤
	m_TimeColor[5].diffuse.r =  0.2f;
	m_TimeColor[5].diffuse.g =  0.8f;
	m_TimeColor[5].diffuse.b =  0.6f;
	m_TimeColor[5].ambient.r =  0.7f;
	m_TimeColor[5].ambient.g =  0.7f;
	m_TimeColor[5].ambient.b =  0.7f;
}


//-----------------------------------------------------------------------------
// Desc: 머리카락, 얼굴 인덱스
//-----------------------------------------------------------------------------
bool	EtcTable::FindHairIndex( int index )
{
	for( int i = 0; i < CHANGE_COLOR_COUNT; ++i )
	{
		if( m_hair_type[i] == index )
			return true;
	}	
	return false;
}

bool	EtcTable::FindFaceIndex( int index )
{
	for( int i = 0; i < CHANGE_COLOR_COUNT; ++i )
	{
		if( m_face_type[i] == index )
			return true;
	}	
	return false;
}


//-----------------------------------------------------------------------------
// Desc: 시간에 따른 메터리얼값
//-----------------------------------------------------------------------------
#ifdef DIRECT_VERSION_9_MJH
void	EtcTable::ChangeMaterial( D3DMATERIAL9* pMaterial, float range )
#else
void	EtcTable::ChangeMaterial( D3DMATERIAL8* pMaterial, float range )
#endif // DIRECT_VERSION_9_MJH
{	
/*	// 현재시간
	// 메터리얼포인터
	D3DMATERIAL8*	pTemp = (D3DMATERIAL8*)pMaterial;

	// 메터리얼값 변경	
	if( range == -1.0f )
	{		
		pTemp->Diffuse.r = m_TimeColor[time].diffuse.r;
		pTemp->Diffuse.g = m_TimeColor[time].diffuse.g;
		pTemp->Diffuse.b = m_TimeColor[time].diffuse.b;

		pTemp->Ambient.r = m_TimeColor[time].ambient.r;
		pTemp->Ambient.g = m_TimeColor[time].ambient.g;
		pTemp->Ambient.b = m_TimeColor[time].ambient.b;
	}
	else
	{		
		pTemp->Diffuse.r -= m_TimeColor[time].diffuse.r;
		pTemp->Diffuse.r  = m_TimeColor[time].diffuse.r+(pTemp->Diffuse.r*range);
		pTemp->Diffuse.g -= m_TimeColor[time].diffuse.g;
		pTemp->Diffuse.g  = m_TimeColor[time].diffuse.g+(pTemp->Diffuse.g*range);
		pTemp->Diffuse.b -= m_TimeColor[time].diffuse.b;
		pTemp->Diffuse.b  = m_TimeColor[time].diffuse.b+(pTemp->Diffuse.b*range);

		pTemp->Ambient.r -= m_TimeColor[time].ambient.r;
		pTemp->Ambient.r  = m_TimeColor[time].ambient.r+(pTemp->Ambient.r*range);
		pTemp->Ambient.g -= m_TimeColor[time].ambient.g;
		pTemp->Ambient.g  = m_TimeColor[time].ambient.g+(pTemp->Ambient.g*range);
		pTemp->Ambient.b -= m_TimeColor[time].ambient.b;
		pTemp->Ambient.b  = m_TimeColor[time].ambient.b+(pTemp->Ambient.b*range);
	}		
	*/
}


//-----------------------------------------------------------------------------
// Desc: 말타야하는 위치값
//-----------------------------------------------------------------------------
void	EtcTable::GetHorseRidePos( int angle1, int angle2, int& dest1, int& dest2, int size )
{
	int posx = dest1;
	int posy = dest2;

	// 첫번째: 말이 보는 방향
	// 두번째: 나와 말의 방향
	// 6 5 4
	// 7 I 3
	// 0 1 2
	if( angle1 == 0 || angle1 == 1 || angle1 == 2 || angle1 == 4 || angle1 == 5 || angle1 == 6 )
	{
		if( angle2 == 0 || angle2 == 1 || angle2 == 7 || angle2 == 6 || angle2 == 5 )
		{
			dest1 = 7;
			dest2 = 3;
		}
		else
		{
			dest1 = 3;
			dest2 = 7;
		}
	}
	else if( angle1 == 3 || angle1 == 7 )
	{
		if( angle2 == 7 || angle2 == 0 || angle2 == 1 || angle2 == 2 || angle2 == 3 )
		{
			dest1 = 5;
			dest2 = 1;			
		}
		else
		{
			dest1 = 1;
			dest2 = 5;			
		}
	}
	
	// 타일값만들기
	switch( dest1 )
	{
	case 1:
		{
			dest1 = ((posy-1)*size)+posx;
			dest2 = ((posy+1)*size)+posx;
		}
		break;
	case 3:
		{
			dest1 = ((posy)*size)+(posx+1);
			dest2 = ((posy)*size)+(posx-1);
		}
		break;
	case 5:
		{
			dest1 = ((posy+1)*size)+posx;
			dest2 = ((posy-1)*size)+posx;	
		}
		break;
	case 7:
		{
			dest1 = ((posy)*size)+(posx+1);
			dest2 = ((posy)*size)+(posx-1);
		}
		break;
	}	
}
 

//-----------------------------------------------------------------------------
// Desc: 요정애니메이션 인덱스리턴
//-----------------------------------------------------------------------------
int		EtcTable::GetFairyAnimationIndex( int index )
{	
	return m_fairy_ani[index];
}


//-----------------------------------------------------------------------------
// Desc: 던전맵이니?
//-----------------------------------------------------------------------------
bool	EtcTable::GetDungeonMap( int _nMapIndex )
{
	switch( _nMapIndex )
	{
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			return true;
	}
	return false;
}





//-----------------------------------------------------------------------------
// Desc: 벡터 x 메트릭스 = 벡터
//-----------------------------------------------------------------------------
void VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, D3DXMATRIX& mat )
{
	/*
	float x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	float y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	float z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
	float w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;

	w = 1 / w;

	vDest.x = x / w;
	vDest.y = y / w;
	vDest.z = z / w;
	*/
		
	vDest.x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z*mat._31 + mat._41;
	vDest.y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z*mat._32 + mat._42;
	vDest.z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z*mat._33 + mat._43;	
}


//-----------------------------------------------------------------------------
// Desc: float x,y,z, x 메트릭스 
//-----------------------------------------------------------------------------
void Float3MatrixMultiply( float& x, float& y, float& z, D3DXVECTOR3& vSrc, D3DXMATRIX& mat )
{
	x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
	y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
	z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;	
}


//-----------------------------------------------------------------------------
// Desc: 수학식
//-----------------------------------------------------------------------------
DWORD	FloatToDWORD( float f )
{
	DWORD d = (DWORD)f;
	if( d <= 0 )
		return 1;
	return d;
}


//-----------------------------------------------------------------------------
// Desc: 렌더링
//-----------------------------------------------------------------------------
HRESULT DrawIndexedPrimitive( int nVtxCount, int nIdxCount )
{	
	if( nVtxCount <= 0 )
		MessageBox( NULL, "DrawIndexedPrimitive nVtxCount Error", "", MB_OK );
	if( nIdxCount <= 0 )
		MessageBox( NULL, "DrawIndexedPrimitive nIdxCount Error", "", MB_OK );

	HRESULT hr = g_RenderManager.DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, nVtxCount, 0, nIdxCount );
	if( hr == 0 )		
		g_chr_face += nIdxCount;
	else
	{
//		#ifdef GAMETYPE_TEST		
//			MessageBox( NULL, "DrawIndexedPrimitive Draw Error", "", MB_OK );
//		#endif
	}
	return hr;
}