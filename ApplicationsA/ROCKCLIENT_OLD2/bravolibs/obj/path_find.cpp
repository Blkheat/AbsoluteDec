#include "RockPCH.h"

#include	<windows.h>
#include	<stdio.H>
#include	<malloc.h>
#include	<math.h>
#include	<mmsystem.h>
#include	"..\\quadlist.h"
#include	<Map\Field.h>
#include	"..\\Map.H"

int		MAP_XSIZE = 512;
int		MAP_YSIZE = 512;


struct	NODE 
{
    signed short f;
    signed short h;
    signed short g;
    signed short x, y;
    struct NODE *prev;
    struct NODE *Direct[8];
    struct NODE *next;
};

struct	NSTACK 
{
    struct NODE *NodePtr;
    struct NSTACK *next;
};

struct NODE   *OPEN   = NULL;
struct NODE   *CLOSED = NULL;
struct NSTACK *STACK  = NULL;

short initpath( void ) 
{
	if( OPEN == NULL )
	{
		OPEN	= ( struct NODE * )calloc( 1, sizeof( struct NODE ) );
		CLOSED	= ( struct NODE * )calloc( 1, sizeof( struct NODE ) );
		if( ( STACK = ( struct NSTACK * )calloc( 1, sizeof( struct NSTACK ) ) ) == NULL ) 
		{
			return( 0 );
		}
	}
    return( 1 );
}

void endpath( void ) 
{
	if( OPEN != NULL )
	{
		free( OPEN );
		OPEN = NULL;
	}
	if( CLOSED != NULL )
	{
		free( CLOSED );
		CLOSED = NULL;
	}
	if( STACK != NULL )
	{
		free( STACK );
		STACK  = NULL;
	}
}

struct NODE *isOPEN( short x, short y ) 
{
    struct NODE *tmp;
    tmp = OPEN->next;
    while( tmp != NULL ) 
	{
        if( tmp->x == x && tmp->y == y ) 
			return( tmp );
        tmp = tmp->next;
    }
    return( NULL );
}

struct NODE *isCLOSED( short x, short y ) 
{
    struct NODE *tmp;
    tmp = CLOSED->next;
    while( tmp != NULL ) 
	{
        if( tmp->x == x && tmp->y == y )
			return( tmp );
        tmp = tmp->next;
    }
    return( NULL );
}

void Insert_node( struct NODE *t ) 
{
    struct NODE *one, *two;
    if( OPEN->next == NULL ) 
	{
		OPEN->next = t;
	}
    else 
	{
        one = OPEN;
        two = OPEN->next;
        while( ( two != NULL ) && ( two->f < t->f ) ) 
		{
            one = two;
            two = two->next;
        }
        t->next = two;
        one->next = t;
    }
}

void push( struct NODE *Node ) 
{
    struct NSTACK *data;
    data = ( struct NSTACK * )calloc( 1, sizeof( struct NSTACK ) );
    data->NodePtr = Node;
    data->next = STACK->next;
    STACK->next = data;
}

struct NODE *pop( void ) 
{
    struct NODE *n;
    struct NSTACK *data;
    data = STACK->next;
    n = data->NodePtr;
    STACK->next = data->next;
    free( data );
    return( n );
}

void makeDown( struct NODE *t ) 
{
    int i;
    struct NODE *D, *F;
    for( i = 0; i < 8; i++ ) 
	{
        if( ( D = t->Direct[i] ) == NULL ) 
			break;
        if( D->g > t->g + 1 ) 
		{
                D->g = t->g + 1;
                D->f = D->g + D->h;
                D->prev = t;
                push( D );
        }
    }
    while( STACK->next != NULL ) 
	{
        F = pop( );
        for( i = 0; i < 8; i++ ) 
		{
            if( ( D = F->Direct[i] ) == NULL ) 
				break;
            if( D->g > F->g + 1 ) 
			{
                    D->g = F->g + 1;
                    D->f = D->g + D->h;
                    D->prev = F;
                    push( D );
            }
        }
    }
}
void makeChildSub( struct NODE *node, short x, short y, short dx, short dy ) 
{
    struct NODE *t, *n;
    short i, g = node->g + 1;
    if( ( t = isOPEN( x, y ) ) != NULL ) 
	{                               
        for( i = 0; i < 8; i++ ) if( node->Direct[i] == NULL ) 
			break;
        node->Direct[i] = t;                                            
        if( g < t->g ) 
		{                                                  
                t->g = g;
                t->f = g + t->h;
                t->prev = node;
        }
    } 
	else if( ( t = isCLOSED( x, y ) ) != NULL ) 
	{                       
        for( i = 0; i < 8; i++ ) if( node->Direct[i] == NULL ) 
			break;
        node->Direct[i] = t;
        if( g < t->g ) 
		{
                t->g = g;
                t->f = g + t->h;
                t->prev = node;
                makeDown( t );
        }
    } 
	else 
	{
        if( ( n = ( struct NODE * )calloc( 1, sizeof( struct NODE ) ) ) == NULL ) 
			return;
        n->g = x - dx;
        n->g = n->g < 0 ? -n->g : n->g;
        n->h = y - dy;
        n->h = n->h < 0 ? -n->h : n->h;
        n->h = n->g + n->h;
        n->g = g;
        n->f = g + n->h;
        n->x = x;
        n->y = y;
        n->prev = node;
        Insert_node( n );                                         
        for( i = 0; i < 4; i++ ) 
			if( node->Direct[i] == NULL ) 
				break;
        node->Direct[i] = n;
    }
}

int isTile( short x, short y ) 
{
	// 타일값이 0보다 작거나 ~ 맥스값보다 크니?
	if( x < 0 || y < 0 || x >= MAP_XSIZE || y >= MAP_YSIZE )		
		return 0;
	// 갈수있는곳이니?
	if( g_Map.m_Block[x][y].GetClear() == false || g_Map.m_pAttrMap[y][x].byAttr == MATTR_OBJ )
		return 0;
	return 1;
}

void makeChild( struct NODE *n, short dx, short dy )
{
    short x, y;

	// 오른쪽
    if ( isTile( x=( short )n->x+1,y=( short )n->y ) )
        makeChildSub( n,x,y,dx,dy );
	// 오른쪽위
    if ( isTile( x=( long )n->x+1,y=( long )n->y-1 ) )
       makeChildSub( n,x,y,dx,dy );    
	// 오른쪽아래
    if ( isTile( x=( long )n->x+1,y=( long )n->y+1 ) )
        makeChildSub( n,x,y,dx,dy );
	// 왼쪽
    if ( isTile( x=( short )n->x-1,y=( short )n->y ) )
        makeChildSub( n,x,y,dx,dy );
	// 왼쪽위
    if ( isTile( x=( long )n->x-1,y=( long )n->y-1 ) )
        makeChildSub( n,x,y,dx,dy );
	// 왼쪽아래
    if ( isTile( x=( long )n->x-1,y=( long )n->y+1 ) )
        makeChildSub( n,x,y,dx,dy );
    // 위
    if ( isTile( x=( short )n->x,y=( short )n->y-1 ) )
        makeChildSub( n,x,y,dx,dy );        
    // 아래
    if ( isTile( x=( short )n->x,y=( short )n->y+1 ) )
        makeChildSub( n,x,y,dx,dy );    
}

#define		FIND_PATH_MAX_TIME	1000
long		check_time = 0;


struct NODE *FindPath( short sx, short sy, short dx, short dy ) 
{
	check_time = (long)SAFE_TIME_ADD( timeGetTime() , FIND_PATH_MAX_TIME );

	int maxtry;
	struct NODE *s, *best;
    s = ( struct NODE * )calloc( 1, sizeof( struct NODE ) );        
    maxtry = abs( dx - sx ) * 2 + abs( dy - sy ) * 2 + 1;
    s->g = 0;
    s->h = abs( dx - sx ) + abs( dy - sy );
    s->f = s->h;
    s->x = sx;
    s->y = sy;
    OPEN->next = s;

    while( 1 ) 
	{
		if( SAFE_TIME_COMPARE( check_time , <= , timeGetTime() ) )
		{
//			#ifdef GAMETYPE_TEST			
//				MessageBox( NULL, "여기서 멈춤1", "", MB_OK );			
//			#endif
			return NULL;
		}
        best = OPEN->next;
        if( best == NULL ) 
			return( NULL );
        if( best->g >= maxtry ) 
			return( best );
        if( best->x == dx && best->y == dy ) 
			return( best );
        OPEN->next = best->next; 
        best->next = CLOSED->next;   
        CLOSED->next = best;
        makeChild( best, dx, dy );   
    }
}

bool GotoPath( short s_x, short s_y, short e_x, short e_y, short *d_x, short *d_y ) 
{	
    struct NODE *best, *f, *t;
    OPEN->prev = OPEN->next = CLOSED->prev = CLOSED->next = NULL;
    STACK->NodePtr = NULL;
    STACK->next = NULL;
	
    best = FindPath( s_x, s_y, e_x, e_y );
	if( best == NULL )
		return false;	

	bool bFind = false;
	check_time = (long)SAFE_TIME_ADD( timeGetTime() , FIND_PATH_MAX_TIME );
    if( best != NULL ) 
	{
        while( best->g > 1 ) 		
		{
			if( SAFE_TIME_COMPARE( check_time , <= , timeGetTime() ) )
			{
//				#ifdef GAMETYPE_TEST
//					MessageBox( NULL, "여기서 멈춤2", "", MB_OK );
//				#endif
				return false;
			}	
			best = best->prev;			
		}
        (*d_x) = best->x;
        (*d_y) = best->y;
		bFind = true;
    } 

	//-----------------------------------------------------------------------------
	// 메모리 죽이기
	//-----------------------------------------------------------------------------		
    f = OPEN->next;
	check_time = (long)SAFE_TIME_ADD( timeGetTime() , FIND_PATH_MAX_TIME );
    while( f != NULL ) 
	{		
		if( SAFE_TIME_COMPARE( check_time , <= , timeGetTime() ) )
		{
//			#ifdef GAMETYPE_TEST
//				MessageBox( NULL, "여기서 멈춤3", "", MB_OK );
//			#endif
			return false;
		}				

        t = f;
        f = f->next;
        free( t );
    }
	

    f = CLOSED->next;
	check_time = (long)SAFE_TIME_ADD( timeGetTime() , FIND_PATH_MAX_TIME );
    while( f != NULL ) 
	{
		if( SAFE_TIME_COMPARE( check_time , <= , timeGetTime() ) )
		{
//			#ifdef GAMETYPE_TEST
//				MessageBox( NULL, "여기서 멈춤4", "", MB_OK );
//			#endif
			return false;
		}	

        t = f;
        f = f->next;
        free( t );
    }
    return bFind;
}
