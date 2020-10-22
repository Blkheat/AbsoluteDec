/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"(GPG 1)
 */

#ifndef _PROFILE_H
#define _PROFILE_H

	///#define DO_PROFILE
	#ifdef _DEBUG
		///-- 프로파일링을 수행하려면 DO_PROFILE 를 Define
//		#define DO_PROFILE
	#endif


	#ifndef DO_PROFILE

		#define ProfileBegin(a) 
		#define ProfileEnd(a) 
		#define ProfileDumpOutputToBuffer() 
		#define ProfileInit()

	#else

		#define ProfileBegin(a) ProfileBegin_(a)
		#define ProfileEnd(a) ProfileEnd_(a)
		#define ProfileDumpOutputToBuffer() ProfileDumpOutputToBuffer_()
		#define ProfileInit() ProfileInit_()

		void ProfileInit_( void );// 초기화. ProfileBegin_() 최초 시작전에 호출해야 한다.

		void ProfileBegin_( char* name );
		void ProfileEnd_( char* name );// 결과를 누적, 분석하고 부모/자식 관계를 적절히 밝혀낸다

		void ProfileDumpOutputToBuffer_();
		void StoreProfileInHistory( char* name, float percent );
		void GetProfileFromHistory( char* name, float* ave, float* min, float* max );

	#endif// #ifndef DO_PROFILE

#endif// #ifndef _PROFILE_H
