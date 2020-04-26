/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_MEMORY_H_
#define _CRT_MEMORY_H_

#include "CrtTypes.h"
#include <assert.h>

// Memory Defines
#define CrtNew( c )	new c
#define CrtDelete( p ) if ( p )	{delete p ; p = NULL;}
#define CrtNewData( c, n ) new c[n]
#define CrtDeleteData( p ) if ( p )	{delete [] p ; p = NULL;}

void CrtPrint( const CrtChar * str, ... );

inline void  CrtAssert(CrtChar * string, bool f )
{
	if ( !f)
	{
		CrtPrint("Assertion %s Failed \n", string );
		assert( false ); 
	}
}


#if 0


//////////////////////////////////////////////
// String Allocation Memory Pool 
extern CrtChar * memPool;
extern CrtChar * memPoolTop; 
extern CrtInt32 	memPoolSize; 
extern CrtInt32  memAllocated;
extern CrtInt32  memDeallocated;
extern CrtInt32  memInUse;

//#define CRT_USE_MEMPOOL 

inline void * CrtPoolInit( CrtUInt32 poolSize )
{
#ifdef CRT_USE_MEMPOOL
	if (! memPool)
	{

		memPoolSize = poolSize; 

		CrtPrint("-----------------------------------\n");
		CrtPrint("initialing CrtRender memory pool %d \n", memPoolSize ); 
		CrtPrint("-----------------------------------\n");

		
        memPool = (CrtChar *)malloc( poolSize);

		if ( memPool )
			CrtPrint("memPool initialized %p \n", memPool ); 
        else
			CrtPrint("memPool Allocation Failed %p \n", memPool ); 

		memPoolTop = memPool; 
	}
	#endif 
	return NULL;
}

#ifdef CRT_USE_MEMPOOL
	// Pad pointer x to the next multiple of argument pad.
static inline void* CrtPadPtr(const void* p,unsigned int pad)
{
	long int x = (long int)p;
	x = (x + pad - 1)/pad*pad;
	return (void*)x;
}

#endif 


inline void * CrtAllocator( CrtInt32 len )
{

	#ifdef CRT_USE_MEMPOOL
	
    if ( memPoolTop + len < memPool + memPoolSize )
	{
		// I know redundante but just to be sure 
		void * mem = CrtPadPtr( memPoolTop, 16 );

		memPoolTop += len;
		memPoolTop = (CrtChar *)CrtPadPtr( memPoolTop, 16 ); 

		memAllocated += len; 

		//CrtPrint(" Alloc Pos %p Allocateded %d of %d \n",
		//	   memPoolTop, memPoolTop - memPool, memPoolSize ); 
		return ( mem ); 	
	}
	else
	{
		CrtPrint(" Out of memory in TiXMLString MemoryPool \n" ); 
		CrtPrint(" %d Currently Allocated to the MemoryPool \n", memPoolSize ); fflush( stdout ); 
		return NULL;
	}

	#else 
	
    void * p = malloc ( len );
	if (p)
	{
		memAllocated += len; 
		memInUse += len; 
		if ( len > 1000 )
		{
		//	CrtPrint(" ALLOCATION OF %d \n", len ); 
		}
		return p; 
	}
	else
	{
		CrtPrint(" ALLOCATION OF %d FAILED \n", len ); 
	}
	return NULL;

	#endif 
	//return (CrtChar *)memalign( 4,len); 
}; 

inline void  CrtDeallocator( void * v )
{
	if( v )
	{
		#ifdef CRT_USE_MEMPOOL

		#else
        //memDeallocated += size; 
		//memInUse -= size; 
		free(v);
		#endif 
	}
}

inline void  CrtPoolDestroy()
{
	if ( memPool )
	{
		free( memPool ); 
	}
}

inline void  CrtPoolState()
{
//	CrtPrint(" Alloc Pos %p Allocateded %d of %d \n",
//			   memPoolTop, memPoolTop - memPool, memPoolSize ); 
	CrtPrint(" Mem in use %d Allocated %fmbs \n", memInUse, (CrtFloat)(memAllocated)/1024.f/1024.f ); 
}
//////////////////////////////////////////////////////


// This macro is kinda ugly but it gets done what I want 
//	which is get memory from the memory pool and call 
//	the constructor of that class.  The alernative was overloading
//	new and delete but since I aready overload new and delete with
//	CrtNew and CrtNewData this was a natural change
	
//#define CrtNew( c ) 	&(*(( c *)CrtAllocator( sizeof(c) ))=c())//( c *)malloc( sizeof(c) ) 
//#define CrtDelete( c) 	CrtDeallocator(c ); c = NULL;  
//#define CrtNewData( c, n ) ( c *)CrtAllocator( sizeof(c)*n )
//#define CrtDeleteData( c )  CrtDeallocator( c ); c = NULL; 


// make my own new that takes file/line on input
void * ::operator new (CrtUInt32 uiSize) //;;, int iLine, char *szFile)
{
   void *ptr = memAllocator(uiSize);
   memset(ptr,0,uiSize);
   return ptr;
}

// substitute the new operator to my own new
#define new         ::new ((int)__LINE__,(char*)__FILE__)

#endif

#endif // _CRT_MEMORY_H_

