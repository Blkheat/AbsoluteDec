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
		///-- �������ϸ��� �����Ϸ��� DO_PROFILE �� Define
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

		void ProfileInit_( void );// �ʱ�ȭ. ProfileBegin_() ���� �������� ȣ���ؾ� �Ѵ�.

		void ProfileBegin_( char* name );
		void ProfileEnd_( char* name );// ����� ����, �м��ϰ� �θ�/�ڽ� ���踦 ������ ��������

		void ProfileDumpOutputToBuffer_();
		void StoreProfileInHistory( char* name, float percent );
		void GetProfileFromHistory( char* name, float* ave, float* min, float* max );

	#endif// #ifndef DO_PROFILE

#endif// #ifndef _PROFILE_H
