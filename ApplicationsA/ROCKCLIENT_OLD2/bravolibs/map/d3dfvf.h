#ifndef __d3dfvf_h__
#define __d3dfvf_h__

typedef	struct		_FVF_P
{
	D3DXVECTOR3		v;
}FVF_P;
#define		D3DFVF_P	D3DFVF_XYZ

typedef	struct		_FVF_PD
{
	//float	x, y, z;
	D3DXVECTOR3	v;
	DWORD	color;
}FVF_PD;
#define		D3DFVF_PD	(D3DFVF_XYZ|D3DFVF_DIFFUSE)

typedef	struct		_FVF_PN
{
	float	x, y, z;
	float	nx, ny, nz;
}FVF_PN;
#define		D3DFVF_PN	(D3DFVF_XYZ|D3DFVF_NORMAL)

typedef	struct		_FVF_PT
{
	float	x, y, z;
	float	tu, tv;
}FVF_PT;
#define		D3DFVF_PT	(D3DFVF_XYZ|D3DFVF_TEX1)

typedef	struct		_FVF_PTD
{
	float	x, y, z;
	float	tu, tv;
	DWORD	color;
}FVF_PTD;
#define		D3DFVF_PTD	(D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE)

typedef	struct		_FVF_PNT
{
	float	x, y, z;
	float	nx, ny, nz;
	float	tu, tv;
}FVF_PNT;
#define		D3DFVF_PNT	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

typedef	struct		_FVF_PNT2D
{
	float	x, y, z;
	float	nx, ny, nz;
	float	tu1, tv1;
	float	tu2, tv2;
	DWORD	color;
}FVF_PNT2D;
#define		D3DFVF_PNT2D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2|D3DFVF_DIFFUSE)

typedef	struct		_FVF_SKY
{
	float	x, y, z;
//	float	nx, ny, nz;
	DWORD	color;
	float	tu, tv;
}FVF_SKY;
#define		D3DFVF_SKY	(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

typedef	struct	_FVF_PDT
{
	D3DXVECTOR3	v;
	DWORD		color;
	D3DXVECTOR2	t;
}FVF_PDT;

#define		D3DFVF_PDT		(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#endif	__d3dfvf_h__