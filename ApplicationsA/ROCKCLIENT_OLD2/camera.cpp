                  #include "RockPCH.h"

#include	<NETWORK\\Protocol.h>
#include	<Base\\D3DBase.h>
#include	"RockClient.H"
#include	"..\\quadlist.h"
#include	<OBJ\\ObjBase.H>
#include	<OBJ\\Object.H>
#include	<Map\\Field.h>
#include	"Camera.h"
#include	"PC.h"

#include "global.h"
#include "bravolibs\map\d3dfvf.h"
#include "bravolibs\\Sound\\Sound.h"

#include "rocklibs\\RockInterface\\Rui.h"
#include "rocklibs\\RockInterface\\Resource.h"
#include "CRenderManager.h"
   
extern		long				g_nowTime;
inline float Distance( float x , float z ) { return (float)sqrt((double)(x*x + z*z)); }
#ifdef _DEBUG
	#define		CAM_RAD_MAX		240.0f
#else 
	#define		CAM_RAD_MAX		240.0f
#endif 
#define		CAM_RAD_MIN		18.0f
#define		CAM_PIT_MAX		0.8f // 1.0f
#define		CAM_PIT_MIN		-0.8f //-1.45f ///-1.1f
#define		CAM_YAW_MAX		6.28f
#define		CAM_YAW_MIN		0.0f

#define		PLANE_EPSILON	5.0f// ����ü�� ��Ȯ�ϰ� ���Ե��� �ʴ���, �ణ�� ������ �־ ����ü�� ���Խ�Ű�� ���� ��


//................................................................................................................
/*
z-axis rotation:
[cos  -sin  0]
[sin   cos  0]
[  0     0  1]

x-axis rotation:
[1    0     0]
[0  cos  -sin]
[0  sin   cos]

y-axis rotation:
[ cos  0  sin]
[   0  1    0]
[-sin  0  cos]
*/

/// convert quaternion to euler(in radians)
/// order = (roll(y)->pitch(x)->yaw(z)) in ogl coordinate system
/// yaw(z-axis), pitch(x-axis), roll(y-axis)
/// from  http://www.euclideanspac...aternionToEuler/index.htm
/*
RotationMatrix = 
Rz * Rx * Ry = 
[c3c2-s3s1s2  -s3c1   s3s1c2]
[s3c2+c3s1s2   c3c1  -c3s1c2]
[      -c1s2     s1     c1c2]

where,
c1 = cos(theta_x), c2 = cos(theta_y), c3 = cos(theta_z)
s1 = sin(theta_x), s2 = sin(theta_y), s3 = sin(theta_z).

quaternion matrix = 
[1-2xx-2zz   2(xy-wz)   2(xz+wy)]
[ 2(xy+wz)  1-2xx-2zz   2(yz-wx)]
[ 2(xz-wy)   2(yz+wx)  1-2xx-2yy]

x-axis-rotation = asin(s1) = asin(2(yz+wx))
y-axis-rotation = atan(-(-c1s2), c1c2) = atan(-2(xz-wy), 1-2xx-2yy) = atan(2(wy-xz), xx+yy+zz+ww-2xx-2yy)
	   	   	   	   = atan(2(wy-xz), -xx-yy+zz+ww)
z-axis-rotation = atan(-s3c1, c3c1) = atan(-2(xy-wz), 1-2xx-2zz) = atan(2(wz-xy), xx+yy+zz+ww-2xx-2zz)
	   	   	   	   = atan(2(wz-xy), -xx+yy-zz+ww)
*/
    
void quat_2_euler_ogl (const D3DXQUATERNION& q, float& yaw, float& pitch, float& roll)
{
	   float sqw = q.w*q.w;
	   float sqx = q.x*q.x;
	   float sqy = q.y*q.y;
	   float sqz = q.z*q.z;
	   pitch = asinf(2.0f * (q.y*q.z + q.w*q.x)); // rotation about x-axis
	   roll = atan2f(2.0f * (q.w*q.y - q.x*q.z),(-sqx - sqy + sqz + sqw)); // rotation about y-axis
	   yaw = atan2f(2.0f * (q.w*q.z - q.x*q.y),(-sqx + sqy - sqz + sqw)); // rotation about z-axis
}

/// convert quaternion to euler(in radians)
/// order = (roll(z)->pitch(x)->yaw(y)) in d3d coordinate system
/// yaw(y-axis), pitch(x-axis), roll(z-axis)
/*
RotationMatrix = 
Ry * Rx * Rz = 
[ c2c3+s2s1s3  -c2s3+s2s1c3  s2c1]
[        c1s3          c1c3   -s1]
[-s2c3+c2s1s3   s2s3+c2s1c3  c2c1]

where,
c1 = cos(theta_x), c2 = cos(theta_y), c3 = cos(theta_z)
s1 = sin(theta_x), s2 = sin(theta_y), s3 = sin(theta_z).

quaternion matrix = 
[1-2xx-2zz   2(xy-wz)   2(xz+wy)]
[ 2(xy+wz)  1-2xx-2zz   2(yz-wx)]
[ 2(xz-wy)   2(yz+wx)  1-2xx-2yy]


x-axis-rotation = asin(s1) = asin(-2(yz-wx))
y-axis-rotation = atan(s2c1, c2c1) = atan(2(xz+wy), 1-2xx-2yy) = atan(2(xz+wy), xx+yy+zz+ww-2xx-2yy)
	   	   	   	   = atan(2(xz+wy), -xx-yy+zz+ww)
z-axis-rotation = atan(c1s3, c1c3) = atan(2(xy+wz), 1-2xx-2zz) = atan(2(xy+wz), xx+yy+zz+ww-2xx-2zz)
	   	   	   	   = atan(2(xy+wz), -xx+yy-zz+ww)
*/
void quat_2_euler_d3d (const D3DXQUATERNION& q, float& yaw, float& pitch, float& roll)
{
	   float sqw = q.w*q.w;
	   float sqx = q.x*q.x;
	   float sqy = q.y*q.y;
	   float sqz = q.z*q.z;
	   pitch = asinf(2.0f * (q.w*q.x - q.y*q.z)); // rotation about x-axis
	   yaw = atan2f(2.0f * (q.x*q.z + q.w*q.y),(-sqx - sqy + sqz + sqw)); // rotation about y-axis
	   roll = atan2f(2.0f * (q.x*q.y + q.w*q.z),(-sqx + sqy - sqz + sqw)); // rotation about z-axis
}

/*
convert euler angle(in radians) to quaternion
D3D coordinate system (left-hand):

  ^ y(yaw)
  |
  +--> z(roll)
 /
v x(pitch)

*/
D3DXQUATERNION & euler_2_quat_d3d(D3DXQUATERNION &q, const float yaw, const float pitch, const float roll)
{
	   D3DXQUATERNION qpitch( 1 , 0, 0 , pitch), qyaw( 0, 1, 0, yaw), qroll( 0, 0, 1, roll);

	   q = qyaw * qpitch * qroll;
	   return q;
}

/*
convert euler angle(in radians) to quaternion
OpenGL coordinate system (right-hand):

  ^ z(yaw)
  |
  +--> y(roll)
 /
v x(pitch)

*/
D3DXQUATERNION & euler_2_quat_ogl(D3DXQUATERNION &q, const float yaw, const float pitch, const float roll)
{
	   D3DXQUATERNION qpitch( 1, 0, 0 , pitch), qyaw( 0, 0, 1 , yaw), qroll( 0, 1, 0 , roll);

	   q = qyaw * qpitch * qroll;
	   return q;
}
//................................................................................................................





CBravoCamera::CBravoCamera()
{
	m_ISSkip		= FALSE;

#ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
	
	m_fFOV = 0.0f;
	m_fAspect = 0.0f;
	
#endif// #ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
	
	m_fRot			= 0.0f;
	m_fPos			= 0.0f;
	m_fRadian		= 0.01745f;
	m_fCameraRadius = INIT_RADIUS;
	m_fLastRadius = m_fCameraRadius;
	m_fCameraPitch	= 0.30f;
	m_fCameraYaw	= 0.005f;
	


//	m_bViewPoint	= false;
	m_unique		= -1;
	m_fDist			= -1.0f;
	m_bFog			= false;
	m_bProj			= false;
	m_bRotBySort	= false;
	m_fLife			= 0.0f;
	m_lPrevTime		= g_nowTime;
	m_byLifeFirst	= 0;

	m_fDelayLife		= 0.0f;
	m_lDelayPrevTime	= g_nowTime;

	m_fSpeed			= 0.0f;
	m_fAngularSpeed		= 0.0f;

	m_fRadiusVelocity	= 0.0f;
	m_fYawVelocity		= 0.0f;
	m_fPitchVelocity	= 0.0f;
	m_fNewPitch			= 0.0f;

	theIsDraw		= false;

#ifdef BOUND_BOX_DISPLAY
	m_ISBoundCamera = FALSE;
#endif

	//............................................................................................................
	// ī�޶� ��ũ by wxywxy
	//............................................................................................................
	m_State        = CAMERA_STOP;
	m_SelectAniNum = -1;

}


void CBravoCamera::Reset(void)
{

	m_fRot			= 0.0f;
	m_fPos			= 0.0f;
	m_fRadian		= 0.01745f;
	m_fCameraRadius = INIT_RADIUS;
	m_fLastRadius = m_fCameraRadius;
	m_fCameraPitch	= 0.30f;
	m_fCameraYaw	= 0.005f;
//	m_bViewPoint	= false;
	m_unique		= -1;
	m_fDist			= -1.0f;
	m_bFog			= false;
	m_bProj			= false;
	m_bRotBySort	= false;
	m_fLife			= 0.0f;
	m_lPrevTime		= g_nowTime;
	m_byLifeFirst	= 0;

	m_fDelayLife		= 0.0f;
	m_lDelayPrevTime	= g_nowTime;

	m_fSpeed			= 0.0f;
	m_fAngularSpeed		= 0.0f;

	m_fRadiusVelocity	= 0.0f;
	m_fYawVelocity		= 0.0f;
	m_fPitchVelocity	= 0.0f;
	m_fNewPitch			= 0.0f;
}

CBravoCamera::~CBravoCamera()
{
}

#ifdef DIRECT_VERSION_9_MJH
void CBravoCamera::Create( LPDIRECT3DDEVICE9 pDevice )//....sh
#else
void CBravoCamera::Create( LPDIRECT3DDEVICE8 pDevice )//....sh
#endif // DIRECT_VERSION_9_MJH
{
	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "Invalid device passed", NULL, MB_OK);
		return;
	}

	if ( m_pDevice == NULL )
	{
		m_pDevice	= pDevice;
	}
}


#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoCamera::Init(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 eye, D3DXVECTOR3 lookat, D3DXVECTOR3 up)
#else
HRESULT	CBravoCamera::Init(LPDIRECT3DDEVICE8 pDevice, D3DXVECTOR3 eye, D3DXVECTOR3 lookat, D3DXVECTOR3 up)
#endif // DIRECT_VERSION_9_MJH
{
	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "Invalid device passed", NULL, MB_OK);
		return	E_FAIL;
	}

	if ( m_pDevice == NULL )
		m_pDevice	= pDevice;
	
	m_vEye		= eye;
	m_vLookat	= lookat;
	m_vUp		= up;

	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matPos);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	D3DXMatrixInverse( &m_matInvView , NULL , &m_matView );
	
	g_RenderManager.SetTransform(D3DTS_VIEW, &m_matView);
	m_matChrView = m_matView;

	D3DXMatrixPerspectiveLH( &m_matProj, D3DX_PI/4, 1.0f, 1.0f, 10000.0f );
	return	g_RenderManager.SetTransform(D3DTS_PROJECTION, &m_matProj);
}

#ifdef DIRECT_VERSION_9_MJH
HRESULT	CBravoCamera::Init(LPDIRECT3DDEVICE9 pDevice, DWORD tileX, DWORD tileY)
#else
HRESULT	CBravoCamera::Init(LPDIRECT3DDEVICE8 pDevice, DWORD tileX, DWORD tileY)
#endif // DIRECT_VERSION_9_MJH
{
	if ( pDevice == NULL )
	{
///		MessageBox(NULL, "Invalid device passed", NULL, MB_OK);
		return	E_FAIL;
	}
	if ( m_pDevice == NULL )
		m_pDevice	= pDevice;

//	m_vEye		= D3DXVECTOR3((float)tileX*(TILE_SIZE/2), g_Map.GetPcHeight()+55.0f, m_vEye.z+(TILE_SIZE/2)*13);
//	m_vLookat	= D3DXVECTOR3((float)tileX*(TILE_SIZE/2), g_Map.GetPcHeight(), (float)tileY*(TILE_SIZE/2));

	m_vEye		= D3DXVECTOR3((float)tileX*(TILE_SIZE/2), 55.0f, m_vEye.z+(TILE_SIZE/2)*13);
	m_vLookat	= D3DXVECTOR3((float)tileX*(TILE_SIZE/2), 0.0f, (float)tileY*(TILE_SIZE/2));
	m_vUp		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	if( g_RockClient.GetGameMode() == GMODE_SELECT )
	{// ĳ������ ���� ��ġ(���� ��ġ)
		m_fEye_x = (float)tileX*(TILE_SIZE/2);
		m_fEye_y = g_Pc.GetMapHeight((int)tileX, (int)tileY );
		m_fEye_z = (float)tileY*(TILE_SIZE/2);
	}
	else
	{
		MatchingViewPoint();
	}

//	m_FirstPosX = tileX;
//	m_FirstPosY = tileY;
	m_FirstPosHeight = g_Pc.GetMapHeight((int)tileX, (int)tileY );

	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matPos);

	D3DXMatrixIdentity(&m_matWorld);
	g_RenderManager.SetTransform(D3DTS_WORLD, &m_matWorld);

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vLookat, &m_vUp);
	g_RenderManager.SetTransform(D3DTS_VIEW, &m_matView);
	m_matChrView = m_matView;

	if ( g_Map.m_cSize <= 0 )
	{
		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, 1000.0f );
//		temp = 0;
	}
	else
	{
//		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, g_Map.m_cSize<<6 );
		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, 10000.0f );
//		temp = 1;
	}

	return	g_RenderManager.SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void	CBravoCamera::ReloadProjTM()
{
#ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)
	
	m_fAspect = (float)g_RockClient.m_d3dpp.BackBufferWidth / g_RockClient.m_d3dpp.BackBufferHeight;
	
	// D3DXToRadian(45) == D3DX_PI / 4.0f;
	
	if(m_fAspect < 4.0f / 3.0f)
		m_fFOV = D3DXToRadian(42);// (1280 * 1024) = (5 : 4) = 1.25f
	
	else if(m_fAspect == 4.0f / 3.0f)
		m_fFOV = D3DX_PI / 4.25f;// (800 * 600) = (1024 * 768) = (1152 * 864) = (1280 * 960) = (1600 * 1200) = (4 : 3) = 1.33f
	
	else if(m_fAspect == 15.0f / 3.0f)// 3 * 15 : 9 Wide Surround = 5.0f
		m_fFOV = D3DXToRadian(114);
	
	else if(m_fAspect == 15.0f / 4.0f)// 3 * 5 : 4 Surround = 3.75f
		m_fFOV = D3DXToRadian(98);
	
	else if(m_fAspect == 15.0f / 9.0f)// (1280 * 768) = (15 : 9) = 1.66f
		m_fFOV = D3DXToRadian(54);
	
	else if(m_fAspect == 16.0f / 3.0f)// 3 * 16 : 9 Wide Surround = 5.33f
		m_fFOV = D3DXToRadian(117);
	
	else if(m_fAspect == 16.0f / 4.0f)// 3 * 4 : 3 Surround = 4.0f
		m_fFOV = D3DXToRadian(102);
	
	else if(m_fAspect == 16.0f / 9.0f)// (1280 * 720) = (1600 * 900) = (1920 * 1080) = (16 : 9) = 1.77f
		m_fFOV = D3DXToRadian(57);
	
	else if(m_fAspect == 16.0f / 10.0f)// (1680 * 1050) = (1920 * 1200) = (16 : 10) = 1.6f
		m_fFOV = D3DXToRadian(52);
	
	else
		m_fFOV = D3DX_PI / 4.25f;
	
	D3DXMatrixPerspectiveFovLH( &m_matProj, m_fFOV, m_fAspect, 1.0f, 10000.0f );

#else
	
	//	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, g_Map.m_cSize<<6 );
	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4.25f, 1.333f, 1.0f, 10000.0f );
	
#endif// #ifdef HHM_CAMERA_FOV// ����� ��Ⱦ�� ���� �þ߰� ����(2009/04/13)

	g_RenderManager.SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void	CBravoCamera::SetCameraClear()
{
	bool bFind = true;
	if( g_Map.m_byMapType == MS_INDOOR )
	{		
		switch( g_Map.m_NowMapID )
		{
		case 4:
			bFind = true;
			break;
		default:
			bFind = false;
			g_Camera.SetCameraPos( 0.84f, 0.6f, 100.0f );
			break;
		}
	}
	if( bFind == true )
	{
		m_fCameraRadius = INIT_RADIUS;
		m_fLastRadius = m_fCameraRadius;
		m_fCameraPitch	= 0.30f;
		m_fCameraYaw	= 0.005f;
	
	}
}

void	CBravoCamera::MovCamera()
{
	float fTimeD	= (float)( SAFE_TIME_SUB( g_nowTime , m_lPrevTime) ) / 1000.0f;
	m_lPrevTime		= g_nowTime;

	if( m_fLife > 0.0f )
		m_fLife += fTimeD;
	else
		return;

	if( m_fLife >= 0.7f )
		m_fLife = 0.0f;

	if ( m_fCameraYaw < 0.0f )
		m_fCameraYaw  = 6.28f;
	else if ( m_fCameraYaw > 6.28f )
		m_fCameraYaw = 0.0f;
//	if( !m_bViewPoint )
//	{
		if( m_fCameraPitch > CAM_PIT_MAX )
			m_fCameraPitch = CAM_PIT_MAX;
		else if ( m_fCameraPitch < CAM_PIT_MIN )
			m_fCameraPitch = CAM_PIT_MIN;
//	}


	if( m_fCameraRadius > CAM_RAD_MAX )
		m_fCameraRadius = CAM_RAD_MAX;
	else if ( m_fCameraRadius < CAM_RAD_MIN )
		m_fCameraRadius = CAM_RAD_MIN;
	
	// �� ȸ��
	float fDeltaX = (float)(sin(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);
	float fDeltaZ = -(float)(cos(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);
	float fDeltaY = (float)(sin(m_fCameraPitch) * m_fCameraRadius);

	m_fDeltaX	= fDeltaX;
	m_fDeltaY	= fDeltaY;
	m_fDeltaZ	= fDeltaZ;

//	if( !m_bViewPoint )
//	{
		m_vEye      = D3DXVECTOR3( fDeltaX + m_fEye_x,
									fDeltaY + m_fEye_y+ 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f)+m_fCameraRadius,
									fDeltaZ + m_fEye_z);
		m_vLookat   = D3DXVECTOR3( m_fEye_x, m_fEye_y +12.0f + 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f), m_fEye_z);
//	}
	m_vUp       = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp );
	D3DXMatrixMultiply( &m_matView, &m_matPos, &m_matView );
	g_RenderManager.SetTransform( D3DTS_VIEW, &m_matView );
}

void	CBravoCamera::CameraPosUpdate()
{
	if( m_fCameraYaw < CAM_YAW_MIN )
	{
		m_fCameraYaw  = CAM_YAW_MAX;
	}
	else if( m_fCameraYaw > CAM_YAW_MAX )
	{
		m_fCameraYaw = CAM_YAW_MIN;
	}	

	//X������ -45 ~ 45�� ���̷θ� ȸ�� ��Ŵ
	if( m_fCameraPitch > CAM_PIT_MAX )	
	{
		m_fCameraPitch = CAM_PIT_MAX;
	}
	else if( m_fCameraPitch < CAM_PIT_MIN )
	{
		m_fCameraPitch = CAM_PIT_MIN;
	}
	
	if( m_fCameraRadius > CAM_RAD_MAX )
	{
		m_fCameraRadius = CAM_RAD_MAX;
	}
	else if( m_fCameraRadius < CAM_RAD_MIN )
	{
		m_fCameraRadius = CAM_RAD_MIN;
	}
	

	// �� ȸ��
	float fDeltaX = (float)(sin(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);
	float fDeltaZ = -(float)(cos(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);
	float fDeltaY = (float)(sin(m_fCameraPitch) * m_fCameraRadius);

	

	m_fDeltaX	= fDeltaX;
	m_fDeltaY	= fDeltaY;
	m_fDeltaZ	= fDeltaZ;

	//���� ĳ���� 12.0�� ���̿��� �ݰ� m_fCameraRadius���� ��ȸ�� �ϴ� ī�޶�...
	m_vEye      = D3DXVECTOR3( fDeltaX + m_fEye_x,
//									fDeltaY + m_fEye_y + 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f)+m_fCameraRadius,
									fDeltaY + m_fEye_y + 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f)+12.0f,
									fDeltaZ + m_fEye_z);

	/// by sooree
	float terrain_height = g_Map.Get__TerrainHeightF( m_vEye.x, m_vEye.z );	/// �ϴ� ���� ���� ����	
	float terrain_height2 = g_Map.Get__MapHeightF( m_vEye.x, m_vEye.z );
	
	
	if( ( terrain_height2 > terrain_height && terrain_height2 - terrain_height < 32.0f ) )		
	{	/// �������� �ణ ���� �Ӽ� ���̰� ���� ���� �Ӽ� ���� ���� : ���̰� ũ�� �׳� ���� ���� ��������(ex. �ٸ�)		
		terrain_height = terrain_height2;
	}
/*	else if( ( g_Map.GetMapIdx() == 28 ) || ( g_Map.GetMapIdx() == 32 ) ) /// 28������ �׳� �Ӽ� ���̷�... �� ��ü�� ������Ʈ�� �Ǿ� �ִ�. 
	{
		terrain_height = terrain_height2;

		if( m_vEye.y <= 1280.0f )		// ī�޶� ������Ʈ�� �հ� ���δ�. �ϴ� �ӽ� ����...�� ��; 
		{
			m_vEye.y = 1280.0f;
			terrain_height = 1280.0f;
		}
	}
*/
	if( m_vEye.y <= terrain_height + 8.0f )
	{
		m_vEye.y = terrain_height + 8.0f;	
	}

	m_vLookat   = D3DXVECTOR3( m_fEye_x, m_fEye_y +12.0f + 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f), m_fEye_z);
	m_vUp       = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

//	m_fNewPitch	= (float)atan( (fDeltaY+m_fLastRadius-12.0f) / (cos(m_fCameraPitch) * m_fCameraRadius) );

	// inform D3D where we are looking from
	

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp );
	D3DXMatrixMultiply( &m_matView, &m_matPos, &m_matView );

	g_RenderManager.GetViewport(&m_ViewPort);

}

HRESULT	CBravoCamera::Update()
{
//*************************************************************/
//	m_fCameraYaw	ī�޶��� y�� ȸ����
//	m_fCameraPitch	ī�޶��� x,z�� ȸ���� (-1.57< m_fCameraPitch < 1.57
//	m_fCameraRadius	ī�޶��� �Ÿ���
//*************************************************************/

	MatchingViewPoint();
	
	CameraPosUpdate();
	g_RenderManager.SetTransform( D3DTS_VIEW, &m_matView );

	CalculateFrustum();

	//-----------------------------------------------------------------------------
	// ���������
	//-----------------------------------------------------------------------------
	D3DXMatrixInverse( &m_matInvView, NULL, &m_matView );

	//-----------------------------------------------------------------------------
	// �����忡�� ����� ȸ����
	//-----------------------------------------------------------------------------
	D3DXMatrixRotationY( &m_matRotY, -m_fCameraYaw );

	//-----------------------------------------------------------------------------
	// ���� ���� ���� ����
	//-----------------------------------------------------------------------------
	m_vDir = m_vLookat-m_vEye;

	//-----------------------------------------------------------------------------
	// ī�޶� Frustum
	//-----------------------------------------------------------------------------
	CalculateFrustum();

	//-----------------------------------------------------------------------------
	// �������� ����
	//-----------------------------------------------------------------------------	
	m_fNearDistance = m_fCameraRadius/2;

	if( m_fNearDistance > 100.0f )
		m_fNearDistance = 100.0f;

//	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, fNear, 4000.0f );
//	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, fNear, g_Map.m_cSize<<6 );

	D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4.25f, 1.333f, m_fNearDistance, 10000.0f );
	
	//D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, g_Map.m_cSize<<6 );

	/*
	if( m_fCameraRadius <= INIT_RADIUS )
	{
		m_bFog = true;
		//D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, 2000.0f );
	}
	else
	{
		m_bFog = false;
		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, g_Map.m_cSize<<6 );
	}
	*/
	/*
	//-----------------------------------------------------------------------------
	// ī�޶�� ĳ���ͰŸ��� ���� �������ǰ�
	//-----------------------------------------------------------------------------
	D3DXMATRIX			matCameraPos;
	D3DXMatrixIdentity(&matCameraPos);
	matCameraPos._41 = g_Pc.GetPlayer()->GetPosTM()._41 + m_vEye.x - m_vLookat.x;
	matCameraPos._42 = g_Pc.GetPlayer()->GetPosTM()._42 + m_vEye.y - m_vLookat.y;
	matCameraPos._43 = g_Pc.GetPlayer()->GetPosTM()._43 + m_vEye.z - m_vLookat.z;
	float	_x  = (matCameraPos._41-g_Pc.GetPosTM(-1)._41)*(matCameraPos._41-g_Pc.GetPosTM(-1)._41);
	float	_y 	= (matCameraPos._42-g_Pc.GetPosTM(-1)._42)*(matCameraPos._42-g_Pc.GetPosTM(-1)._42);
	float	_z 	= (matCameraPos._43-g_Pc.GetPosTM(-1)._43)*(matCameraPos._43-g_Pc.GetPosTM(-1)._43);
	float	fDistance = sqrt(_x+_y+_z);
	*/
	return	S_OK;
}

void	CBravoCamera::Delaytime()
{
	float fTimeD	= (float)( SAFE_TIME_SUB( g_nowTime , m_lDelayPrevTime ) ) / 1000.0f;
	m_lDelayPrevTime		= g_nowTime;

	if( m_fDelayLife != 0.0f )
	{
		m_fDelayLife -= fTimeD;

		if( m_fDelayLife <= 0 )
		{
			m_fLife = 0.01f;
			m_fDelayLife = 0.0f;
		}
	}
}


void	CBravoCamera::MatchingViewPoint()
{
	if( g_RockClient.GetGameMode() == GMODE_SELECT )
	{
		m_fEye_x = 4.0f;
		m_fEye_y = 0;
		m_fEye_z = 0;
	}
	else
	{
		m_fEye_x = g_Pc.GetPosEyeTM( m_unique )._41;
		m_fEye_y = g_Pc.GetPosEyeTM( m_unique )._42;
		m_fEye_z = g_Pc.GetPosEyeTM( m_unique )._43;
	}	
}

bool	CBravoCamera::SetZoom( float rad )
{
	if( rad > 0.0f )
	{
		if( m_fCameraRadius >= CAM_RAD_MAX )
			return false;
		else
		{
			if( m_fCameraRadius + rad >= CAM_RAD_MAX )
				rad = CAM_RAD_MAX - m_fCameraRadius;
		}
	}
	else
	{
		if( m_fCameraRadius <= CAM_RAD_MIN )
			return false;
		else
		{
			if( m_fCameraRadius + rad <= CAM_RAD_MIN )
				rad = m_fCameraRadius - CAM_RAD_MIN;
		}
	}

	// ���������� ��� ����ó��
	bool bFind = true;
	if( g_Map.m_byMapType == MS_INDOOR )
	{
		if( g_EtcTable.GetDungeonMap( g_Map.m_NowMapID ) == true )
		{
			if( rad > 0.0f && m_fCameraRadius >= 152.0f )
				bFind = false;
			else
				bFind = true;
		}
		else
		{
			bFind = false;
		}
	}
	if( bFind == true )
	{
		m_fCameraRadius += rad;
		m_fLastRadius = m_fCameraRadius;
	}

	return bFind;
}

bool	CBravoCamera::SetRotate( float Yaw, float Pitch, bool bYaw )
{
	if( !bYaw )
	{
		if( Pitch > 0.0f )
		{
			if( m_fCameraPitch >= CAM_PIT_MAX )
				return false;
			else
			{
				if( m_fCameraPitch + Pitch >= CAM_PIT_MAX )
					Pitch = CAM_PIT_MAX - m_fCameraPitch;
			}
		}
		else
		{
			if( m_fCameraPitch <= CAM_PIT_MIN )
				return false;
			else
			{
				if( m_fCameraPitch + Pitch <= CAM_PIT_MIN )
					Pitch = CAM_PIT_MIN - m_fCameraPitch;
			}
		}
	}

	// ���������� ��� ����ó��
	m_bRotBySort = true;
	if( g_Map.m_byMapType == MS_INDOOR )
	{
		if( g_EtcTable.GetDungeonMap( g_Map.m_NowMapID ) == true )
		{
			if( bYaw )
				m_bRotBySort = true;
			else
				m_bRotBySort = false;
		}
		else
		{
			m_bRotBySort = false;
		}
	}
	if( m_bRotBySort == true )
	{
		IsCamMapOut();

		if( bYaw )
			m_fCameraYaw	+= Yaw;
		else
			m_fCameraPitch	+= Pitch;
	}
	return m_bRotBySort;
}

void	CBravoCamera::IsCamMapOut()
{
	//-----------------------------------------------------------------------------
	//��¥ ī�޶� ��ġ.
	//-----------------------------------------------------------------------------
	D3DXVECTOR3	RealCamPos;

	RealCamPos.x = (float)(sin(m_fCameraYaw) * cos(m_fCameraPitch) * m_fLastRadius) + m_fEye_x;
	RealCamPos.y = (float)(sin(m_fCameraPitch) * m_fLastRadius) + m_fEye_y + 2.0f*(0.7f - m_fLife)*(float)sin(m_fLife*60.0f)+m_fLastRadius;
	RealCamPos.z = -(float)(cos(m_fCameraYaw) * cos(m_fCameraPitch) * m_fLastRadius) + m_fEye_z;

	int		RealCamPosx, RealCamPosz;		// Ÿ�� �ѹ��� ��� �ϱ� ���� ��.
	float	CamPosx, CamPosz;				// Ÿ�Ͼȿ��� � ��ǥ�� �ڸ� ��� �ֳ� ��� �ϱ� ���� ��.

	RealCamPosx =	(int)(RealCamPos.x/16.0f);
	RealCamPosz =	(int)(RealCamPos.z/16.0f);
	CamPosx		=	RealCamPos.x - RealCamPosx*16;
	CamPosz		=	RealCamPos.z - RealCamPosz*16;

	float delta = 1.0f;
	float dxdy = 1.0f;
	float dxdy2 = 1.0f;

	{
		int	res1 = 0;
		int res2 = 0;
		if( RealCamPosx >= (int)g_Map.GetMapSize() - 4)
			res1 = 1;
		else if( 4 >= RealCamPosx )
			res1 = 2;
		else
			res1 = 0;

		if( RealCamPosz >= (int)g_Map.GetMapSize() - 4)
			res2 = 1;
		else if( 4 >= RealCamPosz )
			res2 = 2;
		else
			res2 = 0;

		switch( res1 )
		{
		case 0:
			{
				switch( res2 )
				{
				case 0:
					delta = 1.0f;
					break;
				case 1:
					delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
					break;
				case 2:
					delta	= ( 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
					break;
				}
			}
			break;
		case 1:
			{
				switch( res2 )
				{
				case 0:
					delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
					break;
				case 1:
					{
						dxdy = (g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_z) / (g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_x);
						dxdy2 = ( RealCamPos.z - m_fEye_z ) / (RealCamPos.x - m_fEye_x);
						if( dxdy2 > dxdy )
						{
							delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
						}
						else
						{
							delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
						}
					}
					break;
				case 2:
					{
						dxdy = (16.0f*4.0f - m_fEye_z) / (g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_x);
						dxdy2 = ( RealCamPos.z - m_fEye_z ) / (RealCamPos.x - m_fEye_x);
						if( dxdy2 < dxdy )
						{
							delta	= ( 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
						}
						else
						{
							delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
						}
					}
					break;
				}
			}
			break;
		case 2:
			{
				switch( res2 )
				{
				case 0:
					{
						delta	= ( 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
					}
					break;
				case 1:
					{
						dxdy = (g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_z) / (16.0f*4.0f - m_fEye_x);
						dxdy2 = ( RealCamPos.z - m_fEye_z ) / (RealCamPos.x - m_fEye_x);
						if( dxdy2 < dxdy )
						{
							delta	= ( g_Map.GetMapSize()*16 - 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
						}
						else
						{
							delta	= ( 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
						}
					}
					break;
				case 2:
					{
						dxdy = (16.0f*4.0f - m_fEye_z) / (16.0f*4.0f - m_fEye_x);
						dxdy2 = ( RealCamPos.z - m_fEye_z ) / (RealCamPos.x - m_fEye_x);
						if( dxdy2 > dxdy )
						{
							delta	= ( 16.0f*4.0f - m_fEye_z ) / (RealCamPos.z - m_fEye_z);
						}
						else
						{
							delta	= ( 16.0f*4.0f - m_fEye_x ) / (RealCamPos.x - m_fEye_x);
						}
					}
					break;
				}
			}
			break;
		}
		m_fCameraRadius = m_fLastRadius * delta;
		if( m_fLastRadius < m_fCameraRadius )
			m_fCameraRadius = m_fLastRadius;
	}
}

bool	CBravoCamera::IsCamOk()
{
	if( m_fCameraPitch > CAM_PIT_MAX )
		return false;
	if ( m_fCameraPitch < CAM_PIT_MIN )
		return false;
	if( m_fCameraRadius > CAM_RAD_MAX )
		return false;
	if ( m_fCameraRadius < CAM_RAD_MIN )
		return false;

	return true;
}

void	CBravoCamera::CamSpeed( float fTimeD )
{
	m_fSpeed        = 3.0f*fTimeD;
	m_fAngularSpeed = 1.1f*fTimeD;

    // De-accelerate the camera movement (for smooth motion)
    m_fRadiusVelocity	*= 0.3f;
    m_fYawVelocity		*= 0.3f;
    m_fPitchVelocity	*= 0.3f;
/*
	m_fRadiusVelocity		+= m_fSpeed; // Move Forward
	m_fRadiusVelocity		-= m_fSpeed; // Move Backward
	m_fYawVelocity			+= m_fSpeed; // Turn Right
	m_fYawVelocity			-= m_fSpeed; // Turn Left
	m_fPitchVelocity		+= m_fSpeed; // Turn Down
	m_fPitchVelocity		-= m_fSpeed; // Turn Up
*/
}

void	CBravoCamera::UpdateMovCam()
{
	if( m_fRadiusVelocity >= 0.05f || m_fRadiusVelocity <= -0.05f )
	{
		bool bRes = false;
		bRes = SetZoom( m_fRadiusVelocity*m_fSpeed );
		if( bRes == true )
		{
			Update();
			g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
		}
	}
	if( m_fYawVelocity >= 0.005f || m_fYawVelocity <= -0.005f )
	{
		bool bRes = false;
		bRes = SetRotate( m_fYawVelocity*m_fAngularSpeed, -1.0f );
		if( bRes == true )
		{
			Update();
			g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
		}
	}
	if( m_fPitchVelocity >= 0.005f || m_fPitchVelocity <= -0.005f )
	{
		bool bRes = false;
		bRes = SetRotate( -1.0f, m_fPitchVelocity*m_fAngularSpeed, false );
		if( bRes == true )
		{
			Update();
			g_Map.UpdateTerrain( g_Pc.GetDestX(), g_Pc.GetDestY(), true );					
		}
	}
}

void	CBravoCamera::SetCameraPos(float Yaw, float Pitch, float Radius)
{
	m_fCameraYaw	 =	Yaw;
	m_fCameraPitch	 =	Pitch;
	m_fCameraRadius	 =	Radius;
	m_fLastRadius = m_fCameraRadius;
}

void	CBravoCamera::Transform()
{
	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp );
	D3DXMatrixMultiply(&m_matView, &m_matPos, &m_matView);
}

void	CBravoCamera::ProjUpdate()
{
	m_bProj = !m_bProj;
	if( m_bProj == true )
		//D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, 500.0f );
		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, (float)(g_Map.m_cSize<<6) );
	else
		D3DXMatrixPerspectiveFovLH( &m_matProj, D3DX_PI/4, 1.333f, 1.0f, (float)(g_Map.m_cSize<<6) );
}

/****************************************************************************************
	* Frustum
****************************************************************************************/
///-- ����� ����ȭ�ϴ� �Լ�
void	CBravoCamera::NormalizePlane(float Frustum[6][4], int side)
{
	float	magnitude	= (float)sqrt( Frustum[side][0] * Frustum[side][0] +
									   Frustum[side][1] * Frustum[side][1] +
									   Frustum[side][2] * Frustum[side][2] );

	Frustum[side][0]	/= magnitude;
	Frustum[side][1]	/= magnitude;
	Frustum[side][2]	/= magnitude;
	Frustum[side][3]	/= magnitude;
}

// �������� ���
void	CBravoCamera::CalculateFrustum()
{
	D3DXMATRIX	matClip, matProj, matWorld;

	// ī�޶�(view) * ��������(projection)����� �Է¹޾� 8���� ����� �����.
	matWorld	= m_matView;
	matProj		= m_matProj;

//#ifdef BOUND_BOX_DISPLAY
	
	D3DXMATRIX matInv;
	D3DXMATRIX matTrans;

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// theVtx�� �� ���������� ��谪�� �־�д�.
	theVtx[0].x = -1.0f;	theVtx[0].y = -1.0f;	theVtx[0].z = 0.0f;
	theVtx[1].x =  1.0f;	theVtx[1].y = -1.0f;	theVtx[1].z = 0.0f;
	theVtx[2].x =  1.0f;	theVtx[2].y = -1.0f;	theVtx[2].z = 1.0f;
	theVtx[3].x = -1.0f;	theVtx[3].y = -1.0f;	theVtx[3].z = 1.0f;
	theVtx[4].x = -1.0f;	theVtx[4].y =  1.0f;	theVtx[4].z = 0.0f;
	theVtx[5].x =  1.0f;	theVtx[5].y =  1.0f;	theVtx[5].z = 0.0f;
	theVtx[6].x =  1.0f;	theVtx[6].y =  1.0f;	theVtx[6].z = 1.0f;
	theVtx[7].x = -1.0f;	theVtx[7].y =  1.0f;	theVtx[7].z = 1.0f;
	
	// World��ǥ�� ������ؼ� View * Proj����� ���Ѵ�.
	D3DXMatrixMultiply( &matTrans , &m_matView , &m_matProj );

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInv, NULL , &matTrans );
	
	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for( int i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &theVtx[i], &theVtx[i], &matInv );

//#endif


	//D3DXPlaneFromPoints(&thePlane[0], theVtx + 4, theVtx + 7, theVtx + 6);// �� ���(top)
	//D3DXPlaneFromPoints(&thePlane[1], theVtx, theVtx + 1, theVtx + 2);// �� ���(bottom)
	//D3DXPlaneFromPoints(&thePlane[2], theVtx, theVtx + 4, theVtx + 5);// �� ���(near)
	//D3DXPlaneFromPoints(&thePlane[3], theVtx + 2, theVtx + 6, theVtx + 7);// �� ���(far)
	//D3DXPlaneFromPoints(&thePlane[4], theVtx, theVtx + 3, theVtx + 7);// �� ���(left)
	//D3DXPlaneFromPoints(&thePlane[5], theVtx + 1, theVtx + 5, theVtx + 6);// �� ���(right)

		
	// view * proj�� ����� ���Ѵ�.
	D3DXMatrixMultiply(&matClip, &matWorld, &matProj);

	m_Frustum[FRUSTUM_RIGHT][0]		= matClip._14 - matClip._11;			///-- a
	m_Frustum[FRUSTUM_RIGHT][1]		= matClip._24 - matClip._21;			///-- b  
	m_Frustum[FRUSTUM_RIGHT][2]		= matClip._34 - matClip._31;			///-- c
	m_Frustum[FRUSTUM_RIGHT][3]		= matClip._44 - matClip._41;			///-- d
	NormalizePlane(m_Frustum, FRUSTUM_RIGHT);		///-- ��� ����ȭ

	m_Frustum[FRUSTUM_LEFT][0] 		= matClip._14  + matClip._11;
	m_Frustum[FRUSTUM_LEFT][1] 		= matClip._24  + matClip._21;
	m_Frustum[FRUSTUM_LEFT][2] 		= matClip._34  + matClip._31;
	m_Frustum[FRUSTUM_LEFT][3] 		= matClip._44  + matClip._41;
	NormalizePlane(m_Frustum, FRUSTUM_LEFT);

	m_Frustum[FRUSTUM_BOTTOM][0] 	= matClip._14 + matClip._12;
	m_Frustum[FRUSTUM_BOTTOM][1] 	= matClip._24 + matClip._22;
	m_Frustum[FRUSTUM_BOTTOM][2] 	= matClip._34 + matClip._32;
	m_Frustum[FRUSTUM_BOTTOM][3] 	= matClip._44 + matClip._42;
	NormalizePlane(m_Frustum, FRUSTUM_BOTTOM);

	m_Frustum[FRUSTUM_TOP][0] 		= matClip._14  - matClip._12;
	m_Frustum[FRUSTUM_TOP][1] 		= matClip._24  - matClip._22;
	m_Frustum[FRUSTUM_TOP][2] 		= matClip._34  - matClip._32;
	m_Frustum[FRUSTUM_TOP][3] 		= matClip._44  - matClip._42;
	NormalizePlane(m_Frustum, FRUSTUM_TOP);

	m_Frustum[FRUSTUM_BACK][0] 		= matClip._14 - matClip._13;
	m_Frustum[FRUSTUM_BACK][1] 		= matClip._24 - matClip._23;
	m_Frustum[FRUSTUM_BACK][2] 		= matClip._34 - matClip._33;
	m_Frustum[FRUSTUM_BACK][3] 		= matClip._44 - matClip._43;
	NormalizePlane(m_Frustum, FRUSTUM_BACK);

	m_Frustum[FRUSTUM_FRONT][0] 	= matClip._14 + matClip._13;
	m_Frustum[FRUSTUM_FRONT][1] 	= matClip._24 + matClip._23;
	m_Frustum[FRUSTUM_FRONT][2] 	= matClip._34 + matClip._33;
	m_Frustum[FRUSTUM_FRONT][3] 	= matClip._44 + matClip._43;
	NormalizePlane(m_Frustum, FRUSTUM_FRONT);

	return;
}

bool	CBravoCamera::CubeInFrustum(float x, float y, float z, float size)
{
	for(int i = 0; i < 4; i++ )
	{
		if(m_Frustum[i][0] * (x - size) + m_Frustum[i][1] * (y - size) + m_Frustum[i][2] * (z - size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + size) + m_Frustum[i][1] * (y - size) + m_Frustum[i][2] * (z - size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x - size) + m_Frustum[i][1] * (y + size) + m_Frustum[i][2] * (z - size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + size) + m_Frustum[i][1] * (y + size) + m_Frustum[i][2] * (z - size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x - size) + m_Frustum[i][1] * (y - size) + m_Frustum[i][2] * (z + size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + size) + m_Frustum[i][1] * (y - size) + m_Frustum[i][2] * (z + size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x - size) + m_Frustum[i][1] * (y + size) + m_Frustum[i][2] * (z + size) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + size) + m_Frustum[i][1] * (y + size) + m_Frustum[i][2] * (z + size) + m_Frustum[i][3] > 0)
		   continue;
		// If we get here, it isn't in the frustum		
		return false;
	}

	return true; 
}

bool	CBravoCamera::CubeInFrustum(float x, float y, float z, float xsize, float ysize, float zsize)
{
	for(int i = 0; i < 6; i++ )
	{
		///--- �� ����ü�� �����ϴ� ����� �����İ� ���⼱ x , y , z�� �����̰� xsize, ysize, zsize�� �̿��� 8���� 
		///-- ����ü �������� ã�� ������� ����ü ��� �ٱ��ʿ� ������ return false				
		if((m_Frustum[i][0] * (x - xsize) + m_Frustum[i][1] * (y - ysize) + m_Frustum[i][2] * (z - zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y - ysize) + m_Frustum[i][2] * (z - zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x - xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z - zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z - zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x - xsize) + m_Frustum[i][1] * (y - ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y - ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x - xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3]) > 0)
		   continue;
		if((m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3]) > 0)
		   continue;
		// If we get here, it isn't in the frustum		
		return false;
	} 

	return true; 
}

bool CBravoCamera::PointInFrustum( float x, float y, float z )
{
	for(int i = 0; i < 6; i++ )
	{
		// Calculate the plane equation and check if the point is behind a side of the frustum
		if(m_Frustum[i][0] * x + m_Frustum[i][1] * y + m_Frustum[i][2] * z + m_Frustum[i][3] <= 0)
		{
			// The point was behind a side, so it ISN'T in the frustum
			return false;
		}
	}
	// The point was inside of the frustum (In front of ALL the sides of the frustum)
	return true;
}

bool	CBravoCamera::PlaneInFrustum(float x, float y, float xsize, float ysize)
{
	for(int i = 0; i < 5; i++ )
	{
		if(m_Frustum[i][0] * (x) + m_Frustum[i][2] * (y) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][2] * (y) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x) + m_Frustum[i][2] * (y) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][2] * (y) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x) + m_Frustum[i][2] * (y + ysize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][2] * (y + ysize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x) + m_Frustum[i][2] * (y + ysize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][2] * (y + ysize) + m_Frustum[i][3] > 0)
		   continue;
		// If we get here, it isn't in the frustum
		return false;
	}
	return	true;
}

bool	CBravoCamera::NodeInFrustum(float x, float y, float z, float xsize, float ysize, float zsize)
{
	for(int i = 0; i < 5; i++ )
	{
		if(m_Frustum[i][0] * (x) + m_Frustum[i][1] * (y) + m_Frustum[i][2] * (z) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y) + m_Frustum[i][2] * (z) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z) + m_Frustum[i][3] > 0)
		   continue;

		if(m_Frustum[i][0] * (x) + m_Frustum[i][1] * (y) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3] > 0)
		   continue;
		if(m_Frustum[i][0] * (x + xsize) + m_Frustum[i][1] * (y + ysize) + m_Frustum[i][2] * (z + zsize) + m_Frustum[i][3] > 0)
		   continue;
		// If we get here, it isn't in the frustum
		return false;
	}
	return true;
}

D3DXVECTOR3	CBravoCamera::From2Dto3D( int x, int y )
{
#ifdef DIRECT_VERSION_9_MJH
	D3DVIEWPORT9	vp;
#else
	D3DVIEWPORT8	vp;
#endif // DIRECT_VERSION_9_MJH
	g_RenderManager.GetViewport(&vp);
	
	D3DXVECTOR3	v;
	v.z = m_fCameraRadius;
	v.x = ( ( ( x*2.0f/g_RockClient.GetDisplayWidth() - 1 ) - m_matProj._31 ) / m_matProj._11 ) * v.z;
	v.y = ( ( -1.0f * ( y*2.0f/g_RockClient.GetDisplayHeight() - 1 ) - m_matProj._32 ) / m_matProj._22 ) * v.z;
	return v;
}

void		CBravoCamera::ClearCamThrough()
{
	for( int i = 0; i < 512; i++ )
	{
		CamThrough[i].x = -1;
		CamThrough[i].y = -1;
	}
}

void		CBravoCamera::SetCamThrough()
{
	ClearCamThrough();

	int playerx = g_Pc.GetDestX();
	int playery = g_Pc.GetDestY();

	D3DXVECTOR3 RealCamPos;
	RealCamPos.x = (float)(sin(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius) + m_fEye_x;
	RealCamPos.z = -(float)(cos(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius) + m_fEye_z;

	int		RealCamPosx, RealCamPosy;		// Ÿ�� �ѹ��� ��� �ϱ� ���� ��.

	RealCamPosx =	(int)(RealCamPos.x/8.0f);
	RealCamPosy =	(int)(RealCamPos.z/8.0f);

	int		dx, dy;
	dx = RealCamPosx-playerx;
	dy = RealCamPosy-playery;

	int count = 0;
	int i = 0;

	if( dx == 0 )
	{
		if( dy > 0 )
		{
			for( i = RealCamPosy; i > playery; i-- )
			{
				CamThrough[count].x = playerx;
				CamThrough[count].y = i;
				count++;
			}
		}
		else if( dy < 0 )
		{
			for( i = RealCamPosy; i < playery; i++ )
			{
				CamThrough[count].x = playerx;
				CamThrough[count].y = i;
				count++;
			}
		}
	}
	else if( dx > 0 )
	{
		if( dy > 0 )
		{
			if( dx >= dy )
			{
				for( i = RealCamPosx; i > playerx; i-- ) //2
				{
					int calculatey;
					if( IsOneTrue( ((float)dy/(float)dx)*(RealCamPosx-i)) && dx != dy )
					{
						calculatey = RealCamPosy - (int)(((float)dy/(float)dx)*(RealCamPosx-i)-0.1f);
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey-1;
						count++;
					}
					else
					{
						calculatey = RealCamPosy - (int)(((float)dy/(float)dx)*(RealCamPosx-i));
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
					}
				}
			}
			else
			{
				for( i = RealCamPosy; i > playery; i-- ) //1
				{
					int calculatex;
					if( IsOneTrue( ((float)dx/(float)dy)*(RealCamPosy-i)) )
					{
						calculatex = RealCamPosx - (int)(((float)dx/(float)dy)*(RealCamPosy-i)-0.1f);
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
						CamThrough[count].x = calculatex-1;
						CamThrough[count].y = i;
						count++;
					}
					else
					{
						calculatex = RealCamPosx - (int)(((float)dx/(float)dy)*(RealCamPosy-i));
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
					}
				}
			}
		}
		else if( dy < 0 )
		{
			if( dx >= abs(dy) )
			{
				for( i = RealCamPosx; i > playerx; i-- ) //3
				{
					int calculatey;
					if( IsOneTrue( ((float)abs(dy)/(float)dx)*(RealCamPosx-i)) && dx != abs(dy) )
					{
						calculatey = RealCamPosy + (int)(((float)abs(dy)/(float)dx)*(RealCamPosx-i)-0.1f);
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey+1;
						count++;
					}
					else
					{
						calculatey = RealCamPosy + (int)(((float)abs(dy)/(float)dx)*(RealCamPosx-i));
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
					}
				}
			}
			else
			{
				for( i = RealCamPosy; i < playery; i++ ) //4
				{
					int calculatex;
					if( IsOneTrue( ((float)dx/(float)dy)*(RealCamPosy-i)) )
					{
						calculatex = RealCamPosx + (int)(((float)dx/(float)abs(dy))*(RealCamPosy-i)-0.1f);
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
						CamThrough[count].x = calculatex-1;
						CamThrough[count].y = i;
						count++;
					}
					else
					{
						calculatex = RealCamPosx + (int)(((float)dx/(float)abs(dy))*(RealCamPosy-i));
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
					}
				}
			}
		}
		else if( dy == 0 )
		{
			for( i = RealCamPosx; i > playerx; i-- )
			{
				CamThrough[count].x = i;
				CamThrough[count].y = playery;
				count++;
			}
		}
	}
	else if( dx < 0 )
	{
		if( dy > 0 )
		{
			if( abs(dx) >= dy )
			{
				for( i = RealCamPosx; i < playerx; i++ ) //7
				{
					int calculatey;
					if( IsOneTrue( ((float)dy/(float)abs(dx))*(i-RealCamPosx)) && abs(dx) != dy )
					{
						calculatey = RealCamPosy - (int)(((float)dy/(float)abs(dx))*(i-RealCamPosx)-0.1f);
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey-1;
						count++;
					}
					else
					{
						calculatey = RealCamPosy - (int)(((float)dy/(float)abs(dx))*(i-RealCamPosx));
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
					}
				}
			}
			else
			{
				for( i = RealCamPosy; i > playery; i-- ) //8
				{
					int calculatex;
					if( IsOneTrue( ((float)abs(dx)/(float)dy)*(i-RealCamPosy)) )
					{
						calculatex = RealCamPosx - (int)(((float)abs(dx)/(float)dy)*(i-RealCamPosy)-0.1f);
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
						CamThrough[count].x = calculatex+1;
						CamThrough[count].y = i;
						count++;
					}
					else
					{
						calculatex = RealCamPosx - (int)(((float)abs(dx)/(float)dy)*(i-RealCamPosy));
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
					}
				}
			}
		}
		else if( dy < 0 )
		{
			if( abs(dx) >= abs(dy) )
			{
				for( i = RealCamPosx; i < playerx; i++ ) //6
				{
					int calculatey;
					if( IsOneTrue( ((float)abs(dy)/(float)abs(dx))*(i-RealCamPosx)) && abs(dx) != abs(dy) )
					{
						calculatey = RealCamPosy + (int)(((float)abs(dy)/(float)abs(dx))*(i-RealCamPosx)-0.1f);
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey+1;
						count++;
					}
					else
					{
						calculatey = RealCamPosy + (int)(((float)abs(dy)/(float)abs(dx))*(i-RealCamPosx));
						CamThrough[count].x = i;
						CamThrough[count].y = calculatey;
						count++;
					}
				}
			}
			else
			{
				for( i = RealCamPosy; i < playery; i++ ) //5
				{
					int calculatex;
					if( IsOneTrue( ((float)abs(dx)/(float)abs(dy))*(i-RealCamPosy)) )
					{
						calculatex = RealCamPosx + (int)(((float)abs(dx)/(float)abs(dy))*(i-RealCamPosy)-0.1f);
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
						CamThrough[count].x = calculatex+1;
						CamThrough[count].y = i;
						count++;
					}
					else
					{
						calculatex = RealCamPosx + (int)(((float)abs(dx)/(float)abs(dy))*(i-RealCamPosy));
						CamThrough[count].x = calculatex;
						CamThrough[count].y = i;
						count++;
					}
				}
			}
		}
		else if( dy == 0 )
		{
			for( i = RealCamPosx; i < playerx; i++ )
			{
				CamThrough[count].x = i;
				CamThrough[count].y = playery;
				count++;
			}
		}
	}
}

bool	CBravoCamera::IsOneTrue( float f )
{
	if( f < 1.01f && f > 0.99f )
		return true;

	return false;
}

int	CBravoCamera::PickCamThrough()
{
	SetCamThrough();

	D3DXVECTOR3 vector;

	D3DXVECTOR3	vPickRayDir;
	D3DXVECTOR3	vPickRayOrig;

	vector.x = (float)(sin(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);
	vector.y = (float)(sin(m_fCameraPitch) * m_fCameraRadius)+m_fCameraRadius-12.0f;
	vector.z = -(float)(cos(m_fCameraYaw) * cos(m_fCameraPitch) * m_fCameraRadius);

	D3DXVec3Normalize(&vPickRayDir, &vector);
	vPickRayDir *= -1.0f;
	vPickRayOrig.x = vector.x+m_fEye_x;
	vPickRayOrig.y = vector.y+m_fEye_y+12.0f;
	vPickRayOrig.z = vector.z+m_fEye_z;

	// Picking Start
	FLOAT	fBary1, fBary2;
	FLOAT	fDist;
	// 2 Faces == 1 Tile
	D3DXVECTOR3		v1[3];	// 1st face
	D3DXVECTOR3		v2[3];	// 2nd face

	D3DXVECTOR3 V1;
	D3DXVECTOR3 V2;

	m_fDist = -1.0f;
	m_iFrontBack	= -1;

	BOOL	frontback;

	for( int camt = 0; camt < 512; camt++ )
	{
		if( CamThrough[camt].x == -1 && CamThrough[camt].y == -1 )
			return -1;

		int camtx = CamThrough[camt].x/2;
		int camty = CamThrough[camt].y/2;

		v1[0].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].x;
		v1[0].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].y+4.0f;
		v1[0].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].z;
		v1[1].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[1].x;
		v1[1].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[1].y+4.0f;
		v1[1].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[1].z;
		v1[2].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].x;
		v1[2].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].y+4.0f;
		v1[2].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].z;

		v2[0].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].x;
		v2[0].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].y+4.0f;
		v2[0].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[2].z;
		v2[1].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[3].x;
		v2[1].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[3].y+4.0f;
		v2[1].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[3].z;
		v2[2].x	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].x;
		v2[2].y	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].y+4.0f;
		v2[2].z	= g_Map.m_pDisplayMap[camty][camtx].Pos[0].z;

		D3DXVECTOR3 pOut1;
		if( g_Map.IntersectTriangle2(vPickRayOrig, vPickRayDir, v1[0], v1[1], v1[2], &fDist, &fBary1, &fBary2, &frontback) )
		{
			m_fDist = fDist;
			if( m_fDist < 0.0f )
				m_fDist *= -1.0f;

			if( m_fDist <= m_fLastRadius )
			{
				if( frontback )
					m_iFrontBack = 0;
				else
					m_iFrontBack = 1;

				return m_iFrontBack;
			}
			else
				m_fDist = -1.0f;
		}
		else if( g_Map.IntersectTriangle2(vPickRayOrig, vPickRayDir, v2[0], v2[1], v2[2], &fDist, &fBary1, &fBary2, &frontback) )
		{
			m_fDist = fDist;
			if( m_fDist < 0.0f )
				m_fDist *= -1.0f;

			if( m_fDist <= m_fLastRadius )
			{
				if( frontback )
					m_iFrontBack = 0;
				else
					m_iFrontBack = 1;

				return m_iFrontBack;
			}
			else
				m_fDist = -1.0f;
		}

	}

	return -1;
}

/*
void CBravoCamera::IsDrawFrustum()
{
	theIsDraw != theIsDraw;

	return;
}

void CBravoCamera::DrawFrustum()
{	
	if( !theIsDraw )
	{
		return;
	}

	LPDIRECT3DVERTEXBUFFER8	m_pVB = NULL;
	if ( FAILED ( g_RenderManager.CreateVertexBuffer(4*sizeof(FVF_PD), D3DUSAGE_DYNAMIC, D3DFVF_PD, D3DPOOL_SYSTEMMEM, &m_pVB) ) )
	{
		return false;
	}

	for(int i = 0; i < 6; i++ )
	{
		FVF_PD	*	pVertices;
		if ( FAILED ( m_pVB->Lock(0,0,(BYTE**)&pVertices, 0) ) )
		{
			return false;
		}	
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(m_Frustum[i][0]x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(min.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,min.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,min.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);
		pVertices[m_nVtxCount].v		= D3DXVECTOR3(max.x,max.y,max.z);
		pVertices[m_nVtxCount++].color	= D3DCOLOR_XRGB(255,0,0);

		m_pVB->Unlock();
	
		D3DXMATRIX m_matPos;
		D3DXMatrixIdentity(&m_matPos);
		g_RenderManager.SetRenderState(D3DRS_LIGHTING, FALSE);
		g_RenderManager.SetTransform(D3DTS_WORLD, &matPos);
		g_RenderManager.SetTexture(0, NULL);
		g_RenderManager.SetStreamSource(0, m_pVB, sizeof(FVF_PD));
		g_RenderManager.SetFVF(D3DFVF_PD);
		g_RenderManager.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		g_RenderManager.SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	SAFE_RELEASE(m_pVB);
	return;
}
*/

//................................................................................................................
// ī�޶� �������� �׸���
//................................................................................................................
void CBravoCamera::DrawFrustumLine(void)
{
#if ( defined(BOUND_BOX_DISPLAY) || defined(HHM_DRAW_FRUSTUM_LINE) )

	WORD		index[] = { 0 , 1 ,
							0 , 4 ,
							4 , 5 ,
							5 , 1 ,
							4 , 7 ,
							3 , 2 , 
							2 , 6 , 
							7 , 6 ,
							0 , 3 ,
							4 , 7 ,
							1 , 2 ,
							5 , 6  };

	FVF_PD		vtx[8];

	for( int i = 0 ; i < 8 ; i++ )
	{
		vtx[i].v = theVtx[i];
	
		vtx[i].color = D3DCOLOR_XRGB( 255 , 0 , 0 );
	}


	g_RenderManager.SetTexture(0, NULL);
	
	g_RenderManager.SetFVF( D3DFVF_PD );
	g_RenderManager.SetFillMode( D3DFILL_WIREFRAME );
	//g_RenderManager.DrawIndexedPrimitiveUP( D3DPT_LINELIST , 0 , 8 , 12 , index , D3DFMT_INDEX16, vtx, sizeof( FVF_PD ) );
	g_RenderManager.DrawIndexedPrimitive( D3DPT_LINELIST , 0 , 8 , 0, 12 );
	g_RenderManager.SetFillMode( D3DFILL_SOLID );


	if( g_RockClient.GetGameMode() == GMODE_NORMAL )
	{
		vtx[ 1 ].v = D3DXVECTOR3( g_Pc.GetPlayer()->GetPosTM()._41 , 
									g_Pc.GetPlayer()->GetPosTM()._42 + ( g_Pc.GetPlayer()->GetBoundMaxPos().y - g_Pc.GetPlayer()->GetBoundMinPos().y ) * 0.5f ,
									g_Pc.GetPlayer()->GetPosTM()._43 );

		vtx[ 0 ].color = vtx[ 1 ].color = D3DCOLOR_XRGB( 0 , 255 , 255 );
		vtx[ 0 ].v = m_vEye;

		g_RenderManager.DrawPrimitiveUP( D3DPT_LINELIST , 1 , vtx, sizeof( FVF_PD ) );
	}

#endif	
}


//................................................................................................................
// ī�޶� ��ũ ȭ�� �б� 
//................................................................................................................
int CBravoCamera::LoadWalk(char *pFile)
{
	ANI_WALK_DATA TempAniWalkData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined( LOAD_VF ) /////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FILE *fp = NULL;

	char TempStr[ 1024 ] = "";
	
	wsprintf( TempStr , "%s\\Etc\\%s" , g_RockClient.GetRootDir() , pFile );
	
	fp = fopen( pFile , "rb" );

	if( fp == NULL )
	{
		RLGS( "Error Load CBravoCamera::LoadWalk" );

		return FALSE;
	}
	
	float EndFrame;

	fread( &TempAniWalkData.fov		   , sizeof( float ) , 1 , fp );
	fread( &TempAniWalkData.near_plane , sizeof( float ) , 1 , fp );
	fread( &TempAniWalkData.far_plane  , sizeof( float ) , 1 , fp );
	fread( &EndFrame			       , sizeof( float ) , 1 , fp );
	fread( &TempAniWalkData.frame_rate , sizeof( float ) , 1 , fp );
	
	TempAniWalkData.end_frame = ( int ) EndFrame;

	D3DXQUATERNION quat( 0.0f , 0.0f , 0.0f , 1.0f );
	D3DXQUATERNION result_quat(0.0f,0.0f,0.0f,1.0f);


	for( int i = 0 ; i < TempAniWalkData.end_frame ; ++i )
	{
		ANI_TRANS_DATA	TempTransData;

		fread( &TempTransData.pos.x , sizeof( float ) , 1 , fp );
		fread( &TempTransData.pos.y , sizeof( float ) , 1 , fp );
		fread( &TempTransData.pos.z , sizeof( float ) , 1 , fp );
	
		fread( &quat.x , sizeof( float ) , 1 , fp );
		fread( &quat.y , sizeof( float ) , 1 , fp );
		fread( &quat.z , sizeof( float ) , 1 , fp );
		fread( &quat.w , sizeof( float ) , 1 , fp );

		TempTransData.quat = quat;
		
		TempAniWalkData.ani_list.push_back( TempTransData );
	}
	
	fclose( fp );
	
	m_List.push_back( TempAniWalkData );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else

	VFileHandle* pFH = NULL;

	pFH = g_VFEtc.OpenFile( pFile );

	if( pFH )
	{
		float EndFrame;
		
		pFH->Read( &TempAniWalkData.fov	       , sizeof( float ) );
		pFH->Read( &TempAniWalkData.near_plane , sizeof( float ) );
		pFH->Read( &TempAniWalkData.far_plane  , sizeof( float ) );
		pFH->Read( &EndFrame			       , sizeof( float ) );
		pFH->Read( &TempAniWalkData.frame_rate , sizeof( float ) );
		
		TempAniWalkData.end_frame = ( int ) EndFrame;
		
		D3DXQUATERNION quat( 0.0f , 0.0f , 0.0f , 1.0f );
		D3DXQUATERNION result_quat(0.0f,0.0f,0.0f,1.0f);
		
		
		for( int i = 0 ; i < TempAniWalkData.end_frame ; ++i )
		{
			ANI_TRANS_DATA	TempTransData;
			
			pFH->Read( &TempTransData.pos.x , sizeof( float ) );
			pFH->Read( &TempTransData.pos.y , sizeof( float ) );
			pFH->Read( &TempTransData.pos.z , sizeof( float ) );
			
			pFH->Read( &quat.x , sizeof( float ) );
			pFH->Read( &quat.y , sizeof( float ) );
			pFH->Read( &quat.z , sizeof( float ) );
			pFH->Read( &quat.w , sizeof( float ) );
			
			TempTransData.quat = quat;
			
			TempAniWalkData.ani_list.push_back( TempTransData );
		}
		
		m_List.push_back( TempAniWalkData );
	
		g_VFEtc.CloseFile( pFH );

		return TRUE;
	}

	RLGS( "Error Load CBravoCamera::LoadWalk" );

	return FALSE;
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

	return TRUE;
}

void CBravoCamera::LoadInvWalk()
{
	///-- �з��� ����â - ����â ī�޶� ��ũ �ݴ�� ��������
	ANI_WALK_DATA TempAniWalkData1, TempAniWalkData2;
	
	TempAniWalkData1.fov = m_List[2].fov;
	TempAniWalkData1.near_plane = m_List[2].near_plane;
	TempAniWalkData1.far_plane = m_List[2].far_plane;
	TempAniWalkData1.end_frame = m_List[2].end_frame;
	TempAniWalkData1.frame_rate = m_List[2].frame_rate;

	int nSize = m_List[2].ani_list.size();
	
	for(int i = 0 ; i < nSize ;  i++ )
	{
		ANI_TRANS_DATA	TempTransData;
		TempTransData.pos.x = m_List[2].ani_list[(nSize - 1) - i].pos.x;
		TempTransData.pos.y = m_List[2].ani_list[(nSize - 1) - i].pos.y;
		TempTransData.pos.z = m_List[2].ani_list[(nSize - 1) - i].pos.z;

		TempTransData.quat = m_List[2].ani_list[(nSize - 1) - i].quat;
		TempAniWalkData1.ani_list.push_back( TempTransData );		
	}

	m_List.push_back( TempAniWalkData1 );
	///-- ���� ����â - ����â ī�޶� ��ũ �ݴ�� ��������

	TempAniWalkData2.fov = m_List[4].fov;
	TempAniWalkData2.near_plane = m_List[4].near_plane;
	TempAniWalkData2.far_plane = m_List[4].far_plane;
	TempAniWalkData2.end_frame = m_List[4].end_frame;
	TempAniWalkData2.frame_rate = m_List[4].frame_rate;

	nSize = m_List[4].ani_list.size();

	for(int i = 0 ; i < nSize ;  i++ )
	{
		ANI_TRANS_DATA	TempTransData;
		TempTransData.pos.x = m_List[4].ani_list[(nSize - 1) - i].pos.x;
		TempTransData.pos.y = m_List[4].ani_list[(nSize - 1) - i].pos.y;
		TempTransData.pos.z = m_List[4].ani_list[(nSize - 1) - i].pos.z;

		TempTransData.quat = m_List[4].ani_list[(nSize - 1) - i].quat;
		TempAniWalkData2.ani_list.push_back( TempTransData );		
	}

	m_List.push_back( TempAniWalkData2 );

}

int CBravoCamera::SetAnimation(int DataNum)
{
	if( m_State != CAMERA_STOP )
		return FALSE;

	if( m_List.size() <= DataNum )
		return FALSE;

	m_State        = CAMERA_ANIMATION;
	m_PreTime      = timeGetTime();
	m_SelectAniNum = DataNum;

	m_Ani_Frame    = 0;

	switch( DataNum )	
	{
		case 0:
			//ī�޶� 1 ������ �ѱ��� �Ҹ� NO~
			#ifndef LOGINPAGE_BGIMG_CHANGE
				g_Sound.Play( 259 , NULL );
			#endif
			break;
		case 1:
			
			g_Sound.Play( 260 , NULL );
			break;
		case 2:
			
			g_Sound.Play( 261 , NULL );
			break;
		case 6:
			
			g_Sound.Play( 262 , NULL );
			break;
		case 5:
			
			g_Sound.Play( 263 , NULL );
			break;
		case 4:
			
			g_Sound.Play( 264 , NULL );
			break;
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CBravoCamera::SetStartFrame(int DataNum,LPDIRECT3DDEVICE9 pDevice)
#else
int CBravoCamera::SetStartFrame(int DataNum,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_List.size() <= DataNum )
		return FALSE;

	D3DXMATRIX matRot;
	D3DXMATRIX matTrans;
	D3DXMATRIX matMux;

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3(   0.0f,  0.0f,  0.0f );
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(   0.0f, -1.0f,  0.0f );
	D3DXVECTOR3 vUpVec    = D3DXVECTOR3(   0.0f,  0.0f,  1.0f );

	D3DXMatrixIdentity( &matMux );

	D3DXQUATERNION quat;
	
	quat.x = m_List[ DataNum ].ani_list[ 0 ].quat.x;
	quat.y = m_List[ DataNum ].ani_list[ 0 ].quat.y;
	quat.z = m_List[ DataNum ].ani_list[ 0 ].quat.z;
	quat.w = m_List[ DataNum ].ani_list[ 0 ].quat.w;
	
	
	D3DXQuaternionNormalize( &quat , &quat );

	D3DXMatrixRotationQuaternion( &matRot   , &quat );

	D3DXMatrixTranslation(        &matTrans , m_List[ DataNum ].ani_list[ 0 ].pos.x , 
								   	          m_List[ DataNum ].ani_list[ 0 ].pos.y ,
										      m_List[ DataNum ].ani_list[ 0 ].pos.z     );

	D3DXMatrixMultiply( &matMux , &matRot , &matTrans );

    D3DXVec3TransformCoord( &vFromPt   , &vFromPt   , &matMux );
    D3DXVec3TransformCoord( &vLookatPt , &vLookatPt , &matMux );
    D3DXVec3TransformCoord( &vUpVec    , &vUpVec    , &matRot );

    D3DXVec3Normalize( &vUpVec , &vUpVec );

	SetWalk( vFromPt , vLookatPt , vUpVec , quat );

	g_Map.UpdateTerrain( m_vEye.x / CLIENT_TILESIZE , m_vEye.z / CLIENT_TILESIZE , true );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CBravoCamera::SetEndFrame(int DataNum,LPDIRECT3DDEVICE9 pDevice)
#else
int CBravoCamera::SetEndFrame(int DataNum,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_List.size() <= DataNum )
		return FALSE;

	int key = ( m_List[ DataNum ].end_frame - 1 );

	D3DXMATRIX matRot;
	D3DXMATRIX matTrans;
	D3DXMATRIX matMux;

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3(   0.0f,  0.0f,  0.0f );
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(   0.0f, -1.0f,  0.0f );
	D3DXVECTOR3 vUpVec    = D3DXVECTOR3(   0.0f,  0.0f,  1.0f );

	D3DXMatrixIdentity( &matMux );

	D3DXQUATERNION quat;
	
	quat.x = m_List[ DataNum ].ani_list[ key ].quat.x;
	quat.y = m_List[ DataNum ].ani_list[ key ].quat.y;
	quat.z = m_List[ DataNum ].ani_list[ key ].quat.z;
	quat.w = m_List[ DataNum ].ani_list[ key ].quat.w;
	
	
	D3DXQuaternionNormalize( &quat , &quat );

	D3DXMatrixRotationQuaternion( &matRot   , &quat );

	D3DXMatrixTranslation(        &matTrans , m_List[ DataNum ].ani_list[ key ].pos.x , 
								   	          m_List[ DataNum ].ani_list[ key ].pos.y ,
										      m_List[ DataNum ].ani_list[ key ].pos.z     );

	D3DXMatrixMultiply( &matMux , &matRot , &matTrans );

    D3DXVec3TransformCoord( &vFromPt   , &vFromPt   , &matMux );
    D3DXVec3TransformCoord( &vLookatPt , &vLookatPt , &matMux );
    D3DXVec3TransformCoord( &vUpVec    , &vUpVec    , &matRot );

    D3DXVec3Normalize( &vUpVec , &vUpVec );

	SetWalk( vFromPt , vLookatPt , vUpVec , quat );

	g_Map.UpdateTerrain( m_vEye.x / CLIENT_TILESIZE , m_vEye.z / CLIENT_TILESIZE , true );

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CBravoCamera::SetFrame(int DataNum,int StartFrame,float Range,LPDIRECT3DDEVICE9 pDevice)
#else
int CBravoCamera::SetFrame(int DataNum,int StartFrame,float Range,LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{  
#ifdef LOGINPAGE_BGIMG_CHANGE
	if( DataNum == 0 )
	{
		StartFrame = m_List[ DataNum ].end_frame - 1;
		nRui->EnableBGround( FALSE );
		Resource->DestroyDynamic( TID_LOGIN_BACK );
	}
#endif


	if( StartFrame >= ( m_List[ DataNum ].end_frame - 1 ) )
	{
		m_State = CAMERA_STOP;
		
		int EndFrame = m_List[ DataNum ].end_frame - 1;

		D3DXMATRIX matRot;
		D3DXMATRIX matTrans;
		D3DXMATRIX matMux;
		
		D3DXMATRIX  matView;
		
		D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f,  0.0f,  0.0f );
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, -1.0f,  0.0f );
		D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f,  0.0f,  1.0f );
		
		D3DXMatrixIdentity( &matMux );
		
		D3DXQUATERNION quat;
		
		quat.x = m_List[ DataNum ].ani_list[ EndFrame ].quat.x;
		quat.y = m_List[ DataNum ].ani_list[ EndFrame ].quat.y;
		quat.z = m_List[ DataNum ].ani_list[ EndFrame ].quat.z;
		quat.w = m_List[ DataNum ].ani_list[ EndFrame ].quat.w;
		
		D3DXQuaternionNormalize( &quat , &quat );
		
		D3DXMatrixRotationQuaternion( &matRot   , &quat );
		
		D3DXMatrixTranslation( &matTrans , m_List[ DataNum ].ani_list[ EndFrame ].pos.x , 
										   m_List[ DataNum ].ani_list[ EndFrame ].pos.y ,
										   m_List[ DataNum ].ani_list[ EndFrame ].pos.z     );
		
		D3DXMatrixMultiply( &matMux , &matRot , &matTrans );
		
		D3DXVec3TransformCoord( &vFromPt   , &vFromPt   , &matMux );
		D3DXVec3TransformCoord( &vLookatPt , &vLookatPt , &matMux );
		D3DXVec3TransformCoord( &vUpVec    , &vUpVec    , &matRot );
		
		D3DXVec3Normalize( &vUpVec , &vUpVec );
		
		SetWalk( vFromPt , vLookatPt , vUpVec , quat );

		m_NextFunc();						// ���� �̺�Ʈ ����

		g_RockClient.m_SquenceManager.ResetWaitFlag( SWT_CAMERA );

		g_Map.UpdateTerrain( vFromPt.x / CLIENT_TILESIZE , vFromPt.z / CLIENT_TILESIZE , true );
	}
	else
	{
		int NextFrame = StartFrame + 1;

		D3DXMATRIX matRot;
		D3DXMATRIX matTrans;
		D3DXMATRIX matMux;
		
		D3DXMATRIX  matView;
		
		D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f,  0.0f,  0.0f );
		D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, -1.0f,  0.0f );
		D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f,  0.0f,  1.0f );
		
		D3DXMatrixIdentity( &matMux );
		
		D3DXQUATERNION quat;
		D3DXQUATERNION quat1;
		D3DXQUATERNION TempQuat;

		quat.x = m_List[ DataNum ].ani_list[ StartFrame ].quat.x;
		quat.y = m_List[ DataNum ].ani_list[ StartFrame ].quat.y;
		quat.z = m_List[ DataNum ].ani_list[ StartFrame ].quat.z;
		quat.w = m_List[ DataNum ].ani_list[ StartFrame ].quat.w;
		
		quat1.x = m_List[ DataNum ].ani_list[ NextFrame ].quat.x;
		quat1.y = m_List[ DataNum ].ani_list[ NextFrame ].quat.y;
		quat1.z = m_List[ DataNum ].ani_list[ NextFrame ].quat.z;
		quat1.w = m_List[ DataNum ].ani_list[ NextFrame ].quat.w;

		D3DXQuaternionNormalize( &quat  , &quat );
		D3DXQuaternionNormalize( &quat1 , &quat1 );
		
		D3DXQuaternionSlerp( &TempQuat,
							 &quat,
							 &quat1,
							 Range );

		D3DXMatrixRotationQuaternion( &matRot, &TempQuat );
		
		D3DXVECTOR3 vTempTrans;
		D3DXVec3Lerp( &vTempTrans , &m_List[ DataNum ].ani_list[ StartFrame ].pos , 
									&m_List[ DataNum ].ani_list[ NextFrame  ].pos , 
									Range );
		
		D3DXMatrixTranslation( &matTrans , vTempTrans.x , 
										   vTempTrans.y ,
										   vTempTrans.z  );
		
		D3DXMatrixMultiply( &matMux , &matRot , &matTrans );
		
		D3DXVec3TransformCoord( &vFromPt   , &vFromPt   , &matMux );
		D3DXVec3TransformCoord( &vLookatPt , &vLookatPt , &matMux );
		D3DXVec3TransformCoord( &vUpVec    , &vUpVec    , &matRot );
		
		D3DXVec3Normalize( &vUpVec , &vUpVec );
		
		SetWalk( vFromPt , vLookatPt , vUpVec , TempQuat );
	}

	return TRUE;
}

#ifdef DIRECT_VERSION_9_MJH
int CBravoCamera::UpdateWalk(LPDIRECT3DDEVICE9 pDevice)
#else
int CBravoCamera::UpdateWalk(LPDIRECT3DDEVICE8 pDevice)
#endif // DIRECT_VERSION_9_MJH
{
	if( m_State != CAMERA_ANIMATION )
		return FALSE;

	if( m_SelectAniNum == -1 )
		return FALSE;

	DWORD NowTime = timeGetTime();
	
	if( NowTime > m_PreTime + m_List[ m_SelectAniNum ].frame_rate )
	{
		int add_frame = (int)(( NowTime - m_PreTime ) / m_List[ m_SelectAniNum ].frame_rate ); 	

		m_Ani_Frame += add_frame;

		DWORD RangeTime = ( NowTime - m_PreTime ) % ( ( int ) m_List[ m_SelectAniNum ].frame_rate );

		float Range = 0.0f;

		if( RangeTime != 0 )
		{
			Range = ( float ) RangeTime / ( float ) m_List[ m_SelectAniNum ].frame_rate;
		}

		SetFrame( m_SelectAniNum , m_Ani_Frame , Range , pDevice );
		m_PreTime = timeGetTime();	

		m_ISSkip  = TRUE;
	}
	
	if( m_ISSkip )
	{
		g_Map.UpdateTerrain( m_vEye.x / CLIENT_TILESIZE , m_vEye.z / CLIENT_TILESIZE , true );
		m_bRotBySort = TRUE;
	}

	m_ISSkip = !m_ISSkip;

	return TRUE;
}

int CCamera::SetWalk(D3DXVECTOR3 vEye,D3DXVECTOR3 vLookAt,D3DXVECTOR3 vUp,D3DXQUATERNION quat)
{
	//............................................................................................................
	// ��ȯ
	//............................................................................................................
	Init( m_pDevice , vEye , vLookAt , vUp );
	
	float fYaw,fPitch,fRoll;
	
	quat_2_euler_d3d( quat , fYaw , fPitch , fRoll );
	
	SetCameraPos( -fYaw , fPitch , fRoll );
	//............................................................................................................
	
	D3DXMatrixPerspectiveFovLH( &m_matProj , D3DX_PI/4.25f, 1.333f, 1.0f, 10000.0f );
	//D3DXMatrixPerspectiveFovLH( &m_matProj , D3DX_PI/4.0f, 1.333f, 1.0f, 10000.0f );
	g_RenderManager.SetTransform(D3DTS_PROJECTION, &m_matProj);

	CalculateFrustum();

	return TRUE;
}

void CCamera::SetSaveVlue(CAMERA_SAVE_VALUE Data)
{
	m_fCameraYaw    = Data.CameraYaw;
	m_fCameraPitch  = Data.CameraPitch;
	m_fCameraRadius = Data.CameraRadius;

	m_matView       = Data.matView;
	m_matProj       = Data.matProj;

	m_vEye			= Data.vEye;
	m_vLookat		= Data.vLookAt;	
	m_vUp			= Data.vUp;	


	Update();

	g_Map.UpdateTerrain( m_vEye.x / CLIENT_TILESIZE , m_vEye.z / CLIENT_TILESIZE , true );
}

CAMERA_SAVE_VALUE CCamera::GetSaveVlue(void)
{
	CAMERA_SAVE_VALUE TempValue;

	TempValue.CameraYaw    = m_fCameraYaw;    
	TempValue.CameraPitch  = m_fCameraPitch;
	TempValue.CameraRadius = m_fCameraRadius;
	TempValue.matView      = m_matView;       
	TempValue.matProj      = m_matProj;       
	
	TempValue.vEye		   = m_vEye;
	TempValue.vLookAt	   = m_vLookat;
	TempValue.vUp		   = m_vUp;	

	return TempValue;
}

int CCamera::SetCamera(D3DXVECTOR3 vEye,D3DXVECTOR3 vLookAt,D3DXVECTOR3 vUp,BOOL ISTerrainUpdate)
{
	D3DXMatrixLookAtLH( &m_matView, &vEye, &vLookAt, &vUp );
	
	m_vEye	  = vEye;
	m_vLookat = vLookAt;	
	m_vUp     = vUp;	

	g_RenderManager.SetTransform(D3DTS_PROJECTION, &m_matProj);
	g_RenderManager.SetTransform(D3DTS_VIEW      , &m_matView);

	D3DXMatrixInverse( &m_matInvView , NULL , &m_matView );

	CalculateFrustum();

	if( ISTerrainUpdate )
		g_Map.UpdateTerrain( vEye.x / CLIENT_TILESIZE , vEye.z / CLIENT_TILESIZE , true );

	return FALSE;
}

void CCamera::SetLoginCamera(void)
{
	float fYaw = g_Pc.GetPlayer()->m_radian;

	float fAngle       = D3DXToDegree( fYaw );
	float fCameraAngle = D3DXToDegree( g_Camera.m_fCameraYaw ); 

	if( fAngle >= 360.0f )
	{
		fAngle -= 360.0f;
	}

	float fSub = fAngle - fCameraAngle;

	g_Camera.SetCameraPos( fYaw , 2.0f , 55.0f );
	g_Camera.Update();
}
