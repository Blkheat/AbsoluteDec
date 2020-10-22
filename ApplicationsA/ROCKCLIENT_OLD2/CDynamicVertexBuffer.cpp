#include "CDynamicVertexBuffer.h"

CDynamicVertexBuffer::CDynamicVertexBuffer()
{
	m_dwBase = 0;
	m_dwBatchBase = 0;
	m_pVB = NULL;
	m_dwBufferSize = 0;
	m_dwVertexSize = 0;
	m_dwNextBase = 0;
}

void CDynamicVertexBuffer::Release()
{
	if( m_pVB )
	{
		m_pVB->Release();
	}

	m_dwBase = 0;
	m_dwBatchBase = 0;
	m_pVB = NULL;
}

#ifdef DIRECT_VERSION_9_MJH
int CDynamicVertexBuffer::Create( LPDIRECT3DDEVICE9 device, int fvf, int fvfsize, int num, D3DPOOL pool )
#else
int CDynamicVertexBuffer::Create( LPDIRECT3DDEVICE8 device, int fvf, int fvfsize, int num, D3DPOOL pool )
#endif // DIRECT_VERSION_9_MJH
{
	HRESULT hr;
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( hr = device->CreateVertexBuffer( num * fvfsize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		fvf, pool, &m_pVB , NULL ) ) )
#else
	if( FAILED( hr = device->CreateVertexBuffer( num * fvfsize,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		fvf, pool, &m_pVB) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return 0;
	}

	m_dwVertexSize = fvfsize;
	m_dwBufferSize = num;

	return( 1 );
}

//--------------------------------------------------------------------------------------------------------

void *CDynamicVertexBuffer::Lock( DWORD num, DWORD *lockedIndex )
{
	void *pBuf;

	/*	too big to lock
	 */
	if( num > m_dwBufferSize )
	{
		return( NULL );
	}

	if( m_dwBase + num >= m_dwBufferSize )
	{
		m_dwBase = 0;
	}
#ifdef DIRECT_VERSION_9_MJH
	if( FAILED( m_pVB->Lock( m_dwBase * m_dwVertexSize, num * m_dwVertexSize,
							(void**)&pBuf,
							m_dwBase ? D3DLOCK_NOOVERWRITE|D3DLOCK_NOSYSLOCK :
							D3DLOCK_DISCARD|D3DLOCK_NOSYSLOCK ) ) )
#else
	if( FAILED( m_pVB->Lock( m_dwBase * m_dwVertexSize, num * m_dwVertexSize,
							(BYTE**)&pBuf,
							m_dwBase ? D3DLOCK_NOOVERWRITE|D3DLOCK_NOSYSLOCK :
							D3DLOCK_DISCARD|D3DLOCK_NOSYSLOCK ) ) )
#endif // DIRECT_VERSION_9_MJH
	{
		return( NULL );
	}

	m_dwBatchBase = m_dwBase;
	*lockedIndex = m_dwBase;
	m_dwNextBase = m_dwBase + num;

	return( pBuf );
}

//--------------------------------------------------------------------------------------------------------

void CDynamicVertexBuffer::Unlock()
{
	m_pVB->Unlock();
	m_dwBase = m_dwNextBase;
}

//--------------------------------------------------------------------------------------------------------

int CDynamicVertexBuffer::LeftVertexCount()
{
	return( m_dwBufferSize - m_dwBase );
}

void CDynamicVertexBuffer::ReStart()
{
	m_dwBase = 0;
}

//--------------------------------------------------------------------------------------------------------

void CDynamicVertexBuffer::BeginBatch()
{
	m_dwBatchBase = m_dwBase;
}

int CDynamicVertexBuffer::BatchVertexCount()
{
	return( m_dwBatchBase - m_dwBase );
}

void CDynamicVertexBuffer::EndBatch()
{
	m_dwBase = m_dwBatchBase;
}

