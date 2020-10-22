/* Copyright (C) Steve Rabin, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Steve Rabin, 2000"(GPG 1)
 */

#include "..\\RockPCH.h"
#include "profile.h"
#ifdef DO_PROFILE

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "custom_time.h"

// �־��� �ϳ��� �����ӿ���, ������ �������� ���õ��� ������ ���� ������ �䱸�Ѵ�.
typedef struct {
   bool bValid;						 //Whether this data is valid(�������� ��ȿ�� ����)
   unsigned int iProfileInstances;   //# of times ProfileBegin called(�ϳ��� ������ �ȿ��� ProfileBegin�� ȣ�� Ƚ��)
   int iOpenProfiles;				 //# of times ProfileBegin w/o ProfileEnd(�����⸸ �ϰ� ������ ���� ���õ��� ����)
   char szName[256];				 //Name of sample
   
   float fStartTime;				 //The current open profile start time
   float fAccumulator;				 //All samples this frame added together
   float fChildrenSampleTime;		 //Time taken by all children
   float fUseTime;
   
   float fStartMemory;
   float fmemoryALU;
   float fChildrenmemory;		 //Time taken by all children
   float fUseMem;
      
   unsigned int iNumParents;         //Number of profile parents
} ProfileSample;

// ���, �ִ�, �ּ� ���� ��� �����͸� ��´�
typedef struct {
   bool bValid;						 //Whether the data is valid
   char szName[256];				 //Name of the sample
   float fAve;						 //Average time per frame (percentage)
   float fMin;						 //Minimum time per frame (percentage)
   float fMax;					     //Maximum time per frame (percentage)
} ProfileSampleHistory;



#define NUM_PROFILE_SAMPLES 500

// �ܼ����� �ӵ��� ���ؼ� �迭�� �̸� �Ҵ��� �д�. �����͸� �̸� �Ҵ��� ���� ������ ������ �޸� �Ҵ�, ������ �ɸ�
// �ð��� �������ϸ� ���� �ð��� ������ ��ġ�� ������ ����� ��Ȯ���� ��������.
ProfileSample			g_samples[NUM_PROFILE_SAMPLES];
ProfileSampleHistory	g_history[NUM_PROFILE_SAMPLES];


float g_startProfile = 0.0f;
float g_endProfile = 0.0f;

static bool first = true;

// �ʱ�ȭ. ProfileBegin_() ���� �������� ȣ���ؾ� �Ѵ�.
void ProfileInit_( void )
{
   unsigned int i;

   for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) 
   {
      g_samples[i].bValid = false;
      g_history[i].bValid = false;
	  g_samples[i].iOpenProfiles =0;
	  g_samples[i].iProfileInstances =0;
   }

   g_startProfile = GetExactTime();
}

//
void ProfileBegin_( char* name )
{
   unsigned int i = 0;
   MEMORYSTATUS ms;
   GlobalMemoryStatus(&ms);

   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true ) 
   {
      if( strcmp( g_samples[i].szName, name ) == 0 )// ������ �̸��� ���� ������ �����ϴ��� ����
	  {// ���� �����Ѵٸ� ���� �����ӿ� ���� �� ������ �̹� ȣ��Ǿ���.
         g_samples[i].iOpenProfiles++;// Begin()���� ����, End()���� ����
         g_samples[i].iProfileInstances++;
         g_samples[i].fStartTime = GetExactTime();

		 g_samples[i].fStartMemory = (float)ms.dwAvailPhys;

         assert( g_samples[i].iOpenProfiles == 1 ); //max 1 open at once
         return;
       }
       i++;	
   }

   if( i >= NUM_PROFILE_SAMPLES ) {
      assert( !"Exceeded Max Available Profile Samples" );
      return;
   }

   strcpy( g_samples[i].szName, name);
   g_samples[i].bValid = true;
   g_samples[i].iOpenProfiles = 1;
   g_samples[i].iProfileInstances = 1;

   g_samples[i].fAccumulator = 0.0f;
   g_samples[i].fStartTime = GetExactTime();
   g_samples[i].fChildrenSampleTime = 0.0f;

   g_samples[i].fStartMemory = (float)ms.dwAvailPhys;   
   g_samples[i].fChildrenmemory= 0.0f;
   g_samples[i].fmemoryALU = 0.0f;
}

// ����� ����, �м��ϰ� �θ�/�ڽ� ���踦 ������ ��������
void ProfileEnd_( char* name )
{
   unsigned int i = 0;
   unsigned int numParents = 0;

   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true )
   {
      if( strcmp( g_samples[i].szName, name ) == 0 )// ���� ������ ã�Ƴ���
      { 
         unsigned int inner = 0;
         int parent = -1;
         float fEndTime = GetExactTime();
		 
		 MEMORYSTATUS ms;
		 GlobalMemoryStatus(&ms);

         g_samples[i].iOpenProfiles--;

         //Count all parents and find the immediate parent
         while( g_samples[inner].bValid == true ) {
            if( g_samples[inner].iOpenProfiles > 0 )
            {  //Found a parent (any open profiles are parents)
               numParents++;
               if( parent < 0 )
               {  //Replace invalid parent (index)
                  parent = inner;
               }
               else if( g_samples[inner].fStartTime >=
                        g_samples[parent].fStartTime )
               {  //Replace with more immediate parent
                  parent = inner;
               }
            }
            inner++;
         }

		

         //Remember the current number of parents of the sample
         g_samples[i].iNumParents = numParents;
		
         if( parent >= 0 )
         {  //Record this time in fChildrenSampleTime (add it in)
			 g_samples[parent].fChildrenSampleTime += fEndTime -
				 g_samples[i].fStartTime;	
			 g_samples[parent].fChildrenmemory += 
				 g_samples[i].fStartMemory - ms.dwAvailPhys;			 
         }
		 
         //Save sample time in accumulator
         g_samples[i].fAccumulator += fEndTime - g_samples[i].fStartTime;			 
		 g_samples[i].fmemoryALU += g_samples[i].fStartMemory - ms.dwAvailPhys ;
		 g_samples[i].fUseMem = g_samples[i].fStartMemory - ms.dwAvailPhys;
		 g_samples[i].fUseTime = fEndTime - g_samples[i].fStartTime;	
		 //g_samples[i].fmemory += ms.dwAvailPhys - ;	
		 
		 
         return;
      }
      i++;	
   }
}



void ProfileDumpOutputToBuffer_()
{
   unsigned int i = 0;
   unsigned int index = 0;   
   int line = 370;

   FILE		*f;

   if(first) f = fopen("Profile.log", "wt");
   else f = fopen("Profile.log", "at");

   g_endProfile = GetExactTime();
   
   while( i < NUM_PROFILE_SAMPLES && g_samples[i].bValid == true ) {		
      unsigned int indent = 0;
	  float sampleTime=0, percentTime=0, aveTime=0, minTime=0, maxTime=0, sammemory, usemem, usetime;	  
	  char name[256], indentedName[256];
      char sam[16], ave[16], min[16], max[16], num[16], mem[16];


      if( g_samples[i].iOpenProfiles < 0 ) {
         assert( !"ProfileEnd() called without a ProfileBegin()" );	
      }
      else if( g_samples[i].iOpenProfiles > 0 ) {
         assert( !"ProfileBegin() called without a ProfileEnd()" );
      }

      sampleTime = g_samples[i].fAccumulator - g_samples[i].fChildrenSampleTime;
	  sammemory = g_samples[i].fmemoryALU - g_samples[i].fChildrenmemory;
      percentTime = ( sampleTime / (g_endProfile - g_startProfile ) ) * 100.0f;
	  usemem = g_samples[i].fUseMem / 1048576.0f;
	  usetime = g_samples[i].fUseTime;

      aveTime = minTime = maxTime = percentTime;	  

      //Add new measurement into the history and get the ave, min, and max
      StoreProfileInHistory( g_samples[i].szName, percentTime );
      GetProfileFromHistory( g_samples[i].szName, &aveTime, &minTime, &maxTime );
 
      //Format the data	  
	  sprintf( sam, "%0.5f", sampleTime );
	  sprintf( mem, "%.6fMB", sammemory /1048576);
      sprintf( ave, "%.1f", aveTime );
      sprintf( min, "%.1f", minTime );
      sprintf( max, "%.1f", maxTime );
      sprintf( num, "%5d", g_samples[i].iProfileInstances );
	  
	  indentedName[0] = '\0';
      for( indent=0; indent < g_samples[i].iNumParents; indent++ ) {
         sprintf( name, "-----" );
         strcat( indentedName, name);
      }
	  strcat( indentedName, g_samples[i].szName);

	  fprintf(f, "Sec : %7s | Time : %3.3f | Mem : %s | Use :%3.3f | Ave : %s | Min : %s | Max : %s | Calls : %s | Name : %s\n", 
						sam, usetime, mem, usemem, ave, min, max, num, indentedName);

      i++;
	  line += 10;	  
   }

   {  //Reset samples for next frame
      unsigned int i;
      for( i=0; i<NUM_PROFILE_SAMPLES; i++ ) {
         g_samples[i].bValid = false;
      }
      g_startProfile = GetExactTime();
   }

   fputs("\n", f);
   first = false;

   fclose(f);
   ProfileInit_();
}




void StoreProfileInHistory( char* name, float percent )
{
   unsigned int i = 0;
   float oldRatio = 0.5f;
   float newRatio = 0.5f;

  while( i < NUM_PROFILE_SAMPLES && g_history[i].bValid == true ) {
      if( strcmp( g_history[i].szName, name ) == 0 )
      {  //Found the sample		  
		 g_history[i].fAve = (g_history[i].fAve*oldRatio) + (percent*newRatio);
  
		 if( percent < g_history[i].fMin ) {
            g_history[i].fMin = percent;
         }
         else {
            g_history[i].fMin = (g_history[i].fMin*oldRatio) + (percent*newRatio);
         }

         if( g_history[i].fMin < 0.0f ) {
            g_history[i].fMin = 0.0f;
		 }

         if( percent > g_history[i].fMax ) {
            g_history[i].fMax = percent;
         }
         else {
            g_history[i].fMax = (g_history[i].fMax*oldRatio) + (percent*newRatio);
         }
         return;
      }
      i++;
   }

   if( i < NUM_PROFILE_SAMPLES )
   {  //Add to history
      strcpy( g_history[i].szName, name);
      g_history[i].bValid = true;
      g_history[i].fAve = g_history[i].fMin = g_history[i].fMax = percent;
   }
   else {
      assert( !"Exceeded Max Available Profile Samples!");
   }
}

void GetProfileFromHistory( char* name, float* ave, float* min, float* max )
{
   unsigned int i = 0;
   while( i < NUM_PROFILE_SAMPLES && g_history[i].bValid == true ) {
      if( strcmp( g_history[i].szName, name ) == 0 )
	  {  //Found the sample
         *ave = g_history[i].fAve;
         *min = g_history[i].fMin;
         *max = g_history[i].fMax;
         return;
	  }
      i++;
   }	
   *ave = *min = *max = 0.0f;
   return;
}

#endif