# Microsoft Developer Studio Project File - Name="RockClient" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RockClient - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RockClient.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RockClient.mak" CFG="RockClient - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RockClient - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RockClient - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RockClient/ApplicationsA/RockClient", BROAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RockClient - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".\\" /I "..\..\..\RockBase\Include" /I "..\..\Include" /I ".\bravolibs" /I ".\rocklibs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "USING_ROCKBASE_DLL" /D "STRICT" /Fr /FD /c
# SUBTRACT CPP /WX /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib dinput8.lib ddraw.lib d3dx9.lib d3dxof.lib dsound.lib dxerr9.lib d3dx9dt.lib winmm.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib shell32.lib advapi32.lib Strmiids.lib Quartz.lib Strmbase.lib version.lib oleaut32.lib ole32.lib uuid.lib imm32.lib /nologo /subsystem:windows /incremental:yes /pdb:"..\..\Bin\Client\RockClient.pdb" /map /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /out:"..\..\Bin\Client\RockClient.exe" /libpath:"..\..\Lib" /libpath:"..\..\..\RockBase\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RockClient - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\\" /I "..\..\..\RockBase\Include" /I "..\..\Include" /I ".\bravolibs" /I ".\rocklibs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "USING_ROCKBASE_DLL" /D "STRICT" /Fr /FD /GZ /c
# SUBTRACT CPP /WX /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib dinput8.lib ddraw.lib d3dx9.lib d3dxof.lib dsound.lib dxerr9.lib d3dx9dt.lib winmm.lib dxguid.lib winspool.lib shell32.lib Strmiids.lib Quartz.lib Strmbase.lib version.lib oleaut32.lib ole32.lib uuid.lib kernel32.lib user32.lib gdi32.lib advapi32.lib comdlg32.lib imm32.lib HsUserUtil.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /out:"..\..\Bin\Client\RockClientD.exe" /libpath:"..\..\Lib" /libpath:"..\..\..\RockBase\Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RockClient - Win32 Release"
# Name "RockClient - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\brother.cpp
# End Source File
# Begin Source File

SOURCE=.\CAction.cpp
# End Source File
# Begin Source File

SOURCE=.\CCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\CDynamicVertexBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\charitem.cpp
# End Source File
# Begin Source File

SOURCE=.\CLightState.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogo2DEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CNodeSort.cpp
# End Source File
# Begin Source File

SOURCE=.\CSceneManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetEnv.cpp
# End Source File
# Begin Source File

SOURCE=.\CSlangFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\CWebDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DelayActList.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\IExplore.c
# End Source File
# Begin Source File

SOURCE=.\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\mobject.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketCrypto.cpp
# End Source File
# Begin Source File

SOURCE=.\pc.cpp
# End Source File
# Begin Source File

SOURCE=.\Rockrsc.rc
# End Source File
# Begin Source File

SOURCE=.\UIDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\VFileSystem.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\brother.h
# End Source File
# Begin Source File

SOURCE=.\CAction.h
# End Source File
# Begin Source File

SOURCE=.\CCollision.h
# End Source File
# Begin Source File

SOURCE=.\CDynamicVertexBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CFunctor.h
# End Source File
# Begin Source File

SOURCE=.\charitem.h
# End Source File
# Begin Source File

SOURCE=.\CLightState.h
# End Source File
# Begin Source File

SOURCE=.\CLogo2DEffect.h
# End Source File
# Begin Source File

SOURCE=.\CNodeSort.h
# End Source File
# Begin Source File

SOURCE=.\CSceneManager.h
# End Source File
# Begin Source File

SOURCE=.\CSetEnv.h
# End Source File
# Begin Source File

SOURCE=.\CSlangFilter.h
# End Source File
# Begin Source File

SOURCE=.\CWebDialog.h
# End Source File
# Begin Source File

SOURCE=.\DelayActList.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\IExplore.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\mobject.h
# End Source File
# Begin Source File

SOURCE=.\NetworkMgr.h
# End Source File
# Begin Source File

SOURCE=.\PacketCrypto.h
# End Source File
# Begin Source File

SOURCE=.\pc.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\UIDataManager.h
# End Source File
# Begin Source File

SOURCE=.\VFileSystem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\iconimg.ico
# End Source File
# End Group
# Begin Group "BravoLibrary"

# PROP Default_Filter ""
# Begin Group "obj"

# PROP Default_Filter ""
# Begin Group "etc."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\obj\etcstruct.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\findway.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\findway.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\Inject.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\Inject.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\objmath.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\objmath.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\path_find.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\WinLock.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\WinLock.h
# End Source File
# End Group
# Begin Group "file data"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\obj\animation.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\animation.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\model.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\model.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\texture.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\texturemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\texturemanager.h
# End Source File
# End Group
# Begin Group "vertex"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\obj\afterimage_vertex.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\line_vertex.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\object_vertex.h
# End Source File
# End Group
# Begin Group "character"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\obj\ActionSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\ActionSystem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CCharImage.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CCharImage.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CClassView.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CClassView.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CFaceExp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CFaceExp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character_animation.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character_animation.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character_item.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\character_item.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CharacterAI.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CharacterAI.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CHolyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CHolyManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CMantleItem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CMantleItem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CNPCImpactTable.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\CNPCImpactTable.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\EffItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\EffItemManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\nonplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\nonplayer.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\player.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\player.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\player_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\player_manager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\PlayerNPCDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\PlayerNPCDataManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\SkillSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\SkillSystem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\SourceDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\SourceDataManager.h
# End Source File
# End Group
# Begin Group "object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\obj\object.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\object.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\object_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\object_manager.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\bravolibs\obj\define.h
# End Source File
# End Group
# Begin Group "map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\map\d3dfvf.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\d3drenderobj.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\d3drenderobj.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\field.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\field.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldattr.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldgeomedit.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldgrass.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldmapping.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldmultimap.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldnpc.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldobj.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldparticle.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fieldwaterfall.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fileinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\fileinfo.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\grass.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\grass.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\grassnormal.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\grassnormal.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\gtexture.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\gtexture.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\planewatereffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\planewatereffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\superviseenv.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\water.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\water.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\waterfall.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\map\waterfall.h
# End Source File
# End Group
# Begin Group "effect"

# PROP Default_Filter ""
# Begin Group "Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\EffectID.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\effectsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\effectsystem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\MixEffectItemSword.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\MixEffectItemSword.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\particle.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\particle.h
# End Source File
# End Group
# Begin Group "봉인의성지 보스몹 이펙트들"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBaragosEff.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBaragosEff.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDogmaEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDogmaEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFJanadEff.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFJanadEff.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRugalSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRugalSkill.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFUrobosEff.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFUrobosEff.h
# End Source File
# End Group
# Begin Group "이펙트베이스"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectBase.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectBase.h
# End Source File
# End Group
# Begin Group "미정리 이펙트들"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\effect\animsequence.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\animsequence.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\billboard.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\billboard.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowDisplay.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowStuck.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowStuck.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CArrowWeapon.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CBillboardCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CBillboardCurve.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CBillboardLine.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CBillboardLine.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CCombo2DEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CCombo2DEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CComboEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CComboEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDamageEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDamageEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDamageNum.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDamageNum.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDropEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CDropEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAddEffectBase.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAddEffectBase.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAniWing0.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAniWing0.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFArrow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAvoid.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFAvoid.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBallUp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBallUp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBeeLineBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBeeLineBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigBubble.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigBubble.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigNpc0.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigNpc0.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigNpcSkill0.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigNpcSkill0.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigPress.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBigPress.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBizEyeBeam.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBizEyeBeam.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBonArcSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBonArcSkill.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBonWarSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBonWarSkill.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookCriticalAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookCriticalAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookUp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBookUp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowDoubleShot.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowDoubleShot.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowRain.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowRain.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowUp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBowUp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBubbleCrash.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBubbleCrash.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBunchStrength.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFBunchStrength.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCannonEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCannonEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCastStone.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCastStone.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCircleCrash.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCircleCrash.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCommonFlySill.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFCommonFlySill.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFConAniSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFConAniSkill.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDarkBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDarkBomb.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDarkness.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDarkness.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFDetect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEarthBind.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEarthBind.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEldaWeaponItem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEldaWeaponItem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEmblemFlag.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEmblemFlag.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEnergy.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFEnergy.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFameItem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFameItem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFameItemSet.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFameItemSet.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFastBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFastBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectModel.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectModel.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectTextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEffectTextureManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireArmor.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireArmor.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireBall.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireBall.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireRain.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireRain.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireSmack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireSmack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireStormSmash.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireStormSmash.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireWork.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFireWork.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootBowLink.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootBowLink.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootStrength.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFootStrength.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFPopoiAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFPopoiAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFruitAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFFruitAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGageTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGageTrans.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGatherCut.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGatherCut.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGlareBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGlareBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGloveDestruction.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGloveDestruction.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGolemEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGolemEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGTEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFGTEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHandPar.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHandPar.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHealBubble.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHealBubble.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHealRainBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHealRainBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHide.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHide.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHideSensor.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHideSensor.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHolyAppear.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHolyAppear.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHolyBomb.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHolyBomb.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHPSuction.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFHPSuction.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceBowRain.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceBowRain.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceField.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceField.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceRain.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceRain.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceRipBlow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceRipBlow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceShiled.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceShiled.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceSpear.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceSpear.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceSpire.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFIceSprie.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFKeepUpPump.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFKeepUpPump.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLandDamage.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLandDamage.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLightning.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLightning.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLightStorm.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFLightStorm.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMagicDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMagicDelete.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMomentHide.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMomentHide.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMouthFire.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFMouthFire.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNPCBubble.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNPCBubble.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNpcFireBall.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNpcFireBall.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNpcStoneAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFNpcStoneAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPossessed.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPossessed.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPossessedAppear.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPossessedAppear.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPriestStandAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFPriestStandAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRandomArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRandomArrow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRecoveryItem2.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRecoveryItem2.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRefreshBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRefreshBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRefreshSword.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRefreshSword.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRockPress.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRockPress.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRockShield.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRockShield.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRoundFire.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRoundFire.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRoundLight.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFRoundLight.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFScapmper.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFScapmper.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSniperBow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSniperBow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSonBrush.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSonBrush.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSpeacialItem.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSpeacialItem.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSpiritFireSmack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSpiritFireSmack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSPSuction.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSPSuction.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSrcLink.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSrcLink.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStickAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStickAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStickUp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStickUp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStoneRian.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStoneRian.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStrongSword.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFStrongSword.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSwordUp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFSWORDUP.H
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTigerAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTigerAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTripleDarkness.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTripleDarkness.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTripleFire.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTripleFire.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTyneeWave.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFTyneeWave.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWindBush.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWindBush.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWing0.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWing0.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWoodenSword.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWoodenSword.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWrath_Bomb.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWrath_Bomb.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWrathKniteAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CEFWrathKniteAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CExpDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CExpDisplay.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFade.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFade.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFadeLoop.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFadeLoop.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFixSizeBillboard.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFixSizeBillboard.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFloatSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFloatSequence.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFlyWeaponBase.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFlyWeaponBase.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFlyWeaponManager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CFlyWeaponManager.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CGlow.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CGlow.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\cloak.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\cloak.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CNPCStickAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CNPCStickAttack.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CRenderSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CRenderSurface.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CScreenBlur.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CScreenBlur.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CSpellEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CSpellEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CSpotEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CSpotEffect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CStringMap.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CStringToCode.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CStringToCode.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTail.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTail.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTargetSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTargetSurface.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTextCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTextCommand.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTextCommandW.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CTextCommandW.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CVAniModel.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CVAniModel.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CWeatherDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\CWeatherDrop.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\effectinsect.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\effectinsect.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\effectvertex.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\obj\objbase.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\tails.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\tails.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\effect\timer.h
# End Source File
# End Group
# End Group
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\base\d3dapp.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dapp.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dbase.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dfont.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dfont.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dres.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\base\dxutil.h
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bravolibs\network\protocol.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\bravolibs\sound\c3dsound.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\sound\c3dsound.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\sound\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\sound\sound.h
# End Source File
# Begin Source File

SOURCE=.\bravolibs\sound\sound_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\bravolibs\sound\sound_manager.h
# End Source File
# End Group
# End Group
# Begin Group "RockLibrary"

# PROP Default_Filter ""
# Begin Group "profile"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\profile\custom_time.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\profile\custom_time.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\profile\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\profile\profile.h
# End Source File
# Begin Source File

SOURCE=".\rocklibs\profile\프로파일러_작업내용.txt"
# End Source File
# End Group
# Begin Group "RockInterface"

# PROP Default_Filter ""
# Begin Group "Framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Chain.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Chain.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Container.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Container.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Define.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\DefineHID.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\DefineTID.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\DefineWID.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EmblemTexManager.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EmblemTexManager.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontasm.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontasm.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontg.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontg.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontman.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Fontman.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Input.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Input.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\InputDXDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\InputDXDevice.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessengerMemoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessengerMemoWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyBaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerEtcPrc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerEtcPrc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Render.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Render.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Resource.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Resource.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Rui.CPP
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Rui.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\TextOutputBase.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\TextOutputBase.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UiUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UiUtil.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Util.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\WndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\WndProc.h
# End Source File
# End Group
# Begin Group "BaseControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Button.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CheckButton.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CheckButton.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CImm.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CImm.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComboBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EditBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ImageAni.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ImageAni.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ImageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ImageBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ListBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RadioButton.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SlideBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SlideBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Spin.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Spin.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StateButton.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StateButton.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\TextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\TextBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UpGauge.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UpGauge.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Wnd.h
# End Source File
# End Group
# Begin Group "GameControl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Announcement.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Announcement.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BZoneIN2DEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BZoneIN2DEffect.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CArrowLineList.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CArrowLineList.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CBillBoardUIRender.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CBillBoardUIRender.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharIdRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharIdRenderer.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharImgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharImgButton.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatEditBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatMainProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatMainProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatOutput.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Combo2DTimingEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Combo2DTimingEffect.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComboStepBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComboStepBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComEffImgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComEffImgBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComEffProgBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ComEffProgBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPlayerPartyInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPlayerPartyInfo.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CReflection.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CReflection.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CSlotDelayProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CSlotDelayProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CTextOutList.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CTextOutList.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\DlgBalloon.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\DlgBalloon.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FlashFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FlashFrameWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FlashWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FlashWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrameTextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrameTextBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\HelpItemTip.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\HelpItemTip.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\HelpStringTip.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\HelpStringTip.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemGetButton.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemGetButton.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LimitLenTextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LimitLenTextBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LocalEntireMap.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LocalEntireMap.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniMap.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MLineEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MLineEditBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoMiniBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoMiniBar.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RITextBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RITextBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Slot.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Slot.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SysMsgOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SysMsgOutput.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Xprism.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\Xprism.h
# End Source File
# End Group
# Begin Group "WndProc"

# PROP Default_Filter ""
# Begin Group "Subjob"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EssenceWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\EssenceWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaMixRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaMixRWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEnchantRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEnchantRWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEnchantWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEnchantWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSelectPotionWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSelectPotionWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NpcChatWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NpcChatWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\QuestProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\QuestProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SpellStoneWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SpellStoneWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StrengthStoneWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StrengthStoneWndProc.h
# End Source File
# End Group
# Begin Group "로딩"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LoadingWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LoadingWndProc.h
# End Source File
# End Group
# Begin Group "캐릭터 선택"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharSelectWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharSelectWndProc.h
# End Source File
# End Group
# Begin Group "서버선택"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ServerListWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ServerListWndProc.h
# End Source File
# End Group
# Begin Group "로그인"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LoginWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LoginWndProc.h
# End Source File
# End Group
# Begin Group "인벤"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\InventoryWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\InventoryWndProc.h
# End Source File
# End Group
# Begin Group "캐릭터 생성"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharCreateWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharCreateWndProc.h
# End Source File
# End Group
# Begin Group "캐릭터 삭제"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharDelWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharDelWndProc.h
# End Source File
# End Group
# Begin Group "캐릭터 정보"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharStateWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CharStateWndProc.h
# End Source File
# End Group
# Begin Group "캐릭터 상태"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerInfoWnd.h
# End Source File
# End Group
# Begin Group "클래스내용 없음"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BankWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BankWndProc.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPartyInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPartyInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillInfoWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillOverWriteWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillOverWriteWnd.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillRegistWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillRegistWnd.h
# End Source File
# End Group
# Begin Group "로그인 셔터"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CLoginShutter.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CLoginShutter.h
# End Source File
# End Group
# Begin Group "미니게임"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniGame_Elda.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniGame_Elda.h
# End Source File
# End Group
# Begin Group "미니맵 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniMapWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MiniMapWndProc.h
# End Source File
# End Group
# Begin Group "부가 효과 메세지"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AddEffectMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AddEffectMsg.h
# End Source File
# End Group
# Begin Group "대전장"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BattleZoneWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\BattleZoneWndProc.h
# End Source File
# End Group
# Begin Group "히트바 윈도우"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChantHitBarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChantHitBarWnd.h
# End Source File
# End Group
# Begin Group "사교 동작 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CSocialActWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CSocialActWnd.h
# End Source File
# End Group
# Begin Group "교환창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ExchangeWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ExchangeWndProc.h
# End Source File
# End Group
# Begin Group "리그 멤버 정보 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LeagueInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\LeagueInfoWnd.h
# End Source File
# End Group
# Begin Group "구매 아이템 리스트 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PayItemWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PayItemWndProc.h
# End Source File
# End Group
# Begin Group "유저 정보 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerInfoAddWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PlayerInfoAddWnd.h
# End Source File
# End Group
# Begin Group "라디오 방송 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RadioBroadcast.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\RadioBroadcast.h
# End Source File
# End Group
# Begin Group "상점 거래 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StoreWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\StoreWndProc.h
# End Source File
# End Group
# Begin Group "UI 메세지 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UIMsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\UIMsgWnd.h
# End Source File
# End Group
# Begin Group "갯수 입력 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NumberInputWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NumberInputWnd.h
# End Source File
# End Group
# Begin Group "엘다스톤 색상 변경 결과 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CChangeEldaColorRWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CChangeEldaColorRWnd.h
# End Source File
# End Group
# Begin Group "엘다스톤 색상 변경 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CChangeEldaColorWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CChangeEldaColorWnd.h
# End Source File
# End Group
# Begin Group "엘다스톤 가공창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodelWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodelWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤 가공 성공"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodel_SWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodel_SWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤 가공 실패"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodel_FWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRemodel_FWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤과 아이템합성 선택"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSelectEldaMixWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSelectEldaMixWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤과 아이템합성"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaMixAWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaMixAWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤 가루변환 결과"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaDustRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaDustRWndProc.h
# End Source File
# End Group
# Begin Group "엘다스톤 가루변환"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaDust.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemEldaDust.h
# End Source File
# End Group
# Begin Group "개인상점 타이틀 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreTitleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreTitleWnd.h
# End Source File
# End Group
# Begin Group "개인상점 판매상품 등록 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSetWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSetWnd.h
# End Source File
# End Group
# Begin Group "개인상점 아이템 구입창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSetBuyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSetBuyWnd.h
# End Source File
# End Group
# Begin Group "개인상점 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSellWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreSellWnd.h
# End Source File
# End Group
# Begin Group "개인상점 구입창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreBuyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CUserStoreBuyWnd.h
# End Source File
# End Group
# Begin Group "스킬 습득창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillLearnProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillLearnProc.h
# End Source File
# End Group
# Begin Group "스킬 레벨업 처리창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillLevelUpWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillLevelUpWnd.h
# End Source File
# End Group
# Begin Group "스킬창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SkillWndProc.h
# End Source File
# End Group
# Begin Group "시스템 메세지창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SysMsgWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SysMsgWndProc.h
# End Source File
# End Group
# Begin Group "시스템 메뉴"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SystemMenuWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SystemMenuWndProc.h
# End Source File
# End Group
# Begin Group "시스템 출력창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SystemPrintWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\SystemPrintWnd.h
# End Source File
# End Group
# Begin Group "채팅창 슬러쉬 명령어 처리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CEditSlashProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CEditSlashProc.h
# End Source File
# End Group
# Begin Group "파티 팝업창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MPopupMenuWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MPopupMenuWnd.h
# End Source File
# End Group
# Begin Group "파티 멤버 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyMemberWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyMemberWnd.h
# End Source File
# End Group
# Begin Group "파티 팝업 메뉴 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyPopupWnd.h
# End Source File
# End Group
# Begin Group "파티 정보 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyInforWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PartyInforWnd.h
# End Source File
# End Group
# Begin Group "파티 수락 여부창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPAcceptYesNoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CPAcceptYesNoWnd.h
# End Source File
# End Group
# Begin Group "장신구 가공 결과창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CAcryEnchantRWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CAcryEnchantRWnd.h
# End Source File
# End Group
# Begin Group "장신구 가공창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CAcryEnchantWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CAcryEnchantWnd.h
# End Source File
# End Group
# Begin Group "아이템 강화 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemAccEnchant.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemAccEnchant.h
# End Source File
# End Group
# Begin Group "아이템 강화 결과 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemAccEnchantRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemAccEnchantRWndProc.h
# End Source File
# End Group
# Begin Group "아이템 가공 성공창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreate_SWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreate_SWndProc.h
# End Source File
# End Group
# Begin Group "아이템 가공 실패창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreate_FWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreate_FWndProc.h
# End Source File
# End Group
# Begin Group "아이템 가공창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreateWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemCreateWndProc.h
# End Source File
# End Group
# Begin Group "아이템 교환 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExchangeWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExchangeWndProc.h
# End Source File
# End Group
# Begin Group "아이템 추출창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExtractWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExtractWndProc.h
# End Source File
# End Group
# Begin Group "아이템 추출 결과창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExtractRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemExtractRWndProc.h
# End Source File
# End Group
# Begin Group "메인화면 메뉴 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MainMenuWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MainMenuWndProc.h
# End Source File
# End Group
# Begin Group "친구창 정보 설정"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MSettingTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MSettingTabWnd.h
# End Source File
# End Group
# Begin Group "친구창의 친구창 탭"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MFriendTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MFriendTabWnd.h
# End Source File
# End Group
# Begin Group "친구창의 차단탭"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MBlockTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MBlockTabWnd.h
# End Source File
# End Group
# Begin Group "친구창의 프론티어탭"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MGuildTabWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MGuildTabWnd.h
# End Source File
# End Group
# Begin Group "아이템 수리 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRepairWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemRepairWndProc.h
# End Source File
# End Group
# Begin Group "포션 제조 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSkillPotionWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSkillPotionWndProc.h
# End Source File
# End Group
# Begin Group "포션 제조 결과창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSkillPotionRWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ItemSkillPotionRWndProc.h
# End Source File
# End Group
# Begin Group "다른유저 및 NPC 메뉴창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoAddWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoAddWnd.h
# End Source File
# End Group
# Begin Group "다른유저 및 NPC정보창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\NonPlayerInfoWnd.h
# End Source File
# End Group
# Begin Group "프론티어 메인 정보 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrontierMainWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrontierMainWndProc.h
# End Source File
# End Group
# Begin Group "프론티어 생성창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrontierCreateWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\FrontierCreateWnd.h
# End Source File
# End Group
# Begin Group "능력업 처리창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AbilityUpWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AbilityUpWndProc.h
# End Source File
# End Group
# Begin Group "능력업 결과(확인/취소)창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AbilityUpConfirmWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AbilityUpConfirmWnd.h
# End Source File
# End Group
# Begin Group "펫 상태창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetInfoWnd.h
# End Source File
# End Group
# Begin Group "펫 인벤토리 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetInventoryWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetInventoryWnd.h
# End Source File
# End Group
# Begin Group "펫 메인창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetMainWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\PetMainWndProc.h
# End Source File
# End Group
# Begin Group "채팅대화 활성화/비활성화 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatPartWndList.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatPartWndList.h
# End Source File
# End Group
# Begin Group "채팅창"

# PROP Default_Filter ""
# Begin Group "메세지 박스"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessageBox.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessageWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessageWndProc.h
# End Source File
# End Group
# Begin Group "채팅 메세지 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatBoxWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ChatBoxWndProc.h
# End Source File
# End Group
# Begin Group "친구창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MCommunityWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MCommunityWnd.h
# End Source File
# End Group
# Begin Group "메신저 알림창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenNotifyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenNotifyWnd.h
# End Source File
# End Group
# Begin Group "메신저 메세지 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MMsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MMsgWnd.h
# End Source File
# End Group
# Begin Group "멀티채팅 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenMultiChatWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenMultiChatWnd.h
# End Source File
# End Group
# Begin Group "메신저 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenOneChatWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\MessenOneChatWnd.h
# End Source File
# End Group
# End Group
# Begin Group "콜로니 아이템 미리보기 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColItemPreViewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColItemPreViewWnd.h
# End Source File
# End Group
# Begin Group "콜리니 공성/수성 병기 맵배치"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColMapPositionWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColMapPositionWnd.h
# End Source File
# End Group
# Begin Group "콜로니 지역 정보창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColRegionInfoWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColRegionInfoWnd.h
# End Source File
# End Group
# Begin Group "콜로니 지역 안내 리스트창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColRegionListWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CColRegionListWnd.h
# End Source File
# End Group
# Begin Group "보관함 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\WarehouseWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\WarehouseWndProc.h
# End Source File
# End Group
# Begin Group "옵션창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\OptionWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\OptionWndProc.h
# End Source File
# End Group
# Begin Group "웹상점"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CWebShopWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CWebShopWnd.h
# End Source File
# End Group
# Begin Group "로그창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CLogWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CLogWnd.h
# End Source File
# End Group
# Begin Group "캐릭터 부활창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AliveWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\AliveWnd.h
# End Source File
# End Group
# Begin Group "가이드 창(모험백서)"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CGuideWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CGuideWnd.h
# End Source File
# End Group
# Begin Group "아처 컨디션 게이지 바"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CArcherBar.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CArcherBar.h
# End Source File
# End Group
# Begin Group "딜레이 바"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CDelayBarWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CDelayBarWnd.h
# End Source File
# End Group
# Begin Group "길드창고"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\GuildInventoryWndProc.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\GuildInventoryWndProc.h
# End Source File
# End Group
# Begin Group "길드초대"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ProposeJoinFrontier.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\ProposeJoinFrontier.h
# End Source File
# End Group
# End Group
# Begin Group "UI창들 생성"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CompntCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockInterface\CompntCommon.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\rocklibs\ExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\GlobalDefine.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\HHMWorkDefine.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\PickHeightField.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\PickHeightField.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockPCH.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockPCH.h
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockWater.cpp
# End Source File
# Begin Source File

SOURCE=.\rocklibs\RockWater.h
# End Source File
# End Group
# Begin Group "Squence"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Squence\CChangeClass.cpp
# End Source File
# Begin Source File

SOURCE=.\Squence\CChangeClass.h
# End Source File
# Begin Source File

SOURCE=.\Squence\CRotDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Squence\CRotDisplay.h
# End Source File
# Begin Source File

SOURCE=.\Squence\CSquenceBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Squence\CSquenceBase.h
# End Source File
# Begin Source File

SOURCE=.\Squence\CSquenceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Squence\CSquenceManager.h
# End Source File
# Begin Source File

SOURCE=.\Squence\CThreeWayDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\Squence\CThreeWayDisplay.h
# End Source File
# End Group
# Begin Group "미사용"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CIMEddk.h
# End Source File
# Begin Source File

SOURCE=.\npc.cpp
# End Source File
# Begin Source File

SOURCE=.\npc.h
# End Source File
# Begin Source File

SOURCE=.\RockChksum.cpp
# End Source File
# Begin Source File

SOURCE=.\RockChksum.h
# End Source File
# End Group
# Begin Group "카메라"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# End Group
# Begin Group "지형"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCloud.cpp
# End Source File
# Begin Source File

SOURCE=.\CCloud.h
# End Source File
# Begin Source File

SOURCE=.\defineMap_ID.h
# End Source File
# Begin Source File

SOURCE=.\landpatch.cpp
# End Source File
# Begin Source File

SOURCE=.\landpatch.h
# End Source File
# Begin Source File

SOURCE=.\landscape.cpp
# End Source File
# Begin Source File

SOURCE=.\landscape.h
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\MapDataManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MapDataManager.h
# End Source File
# Begin Source File

SOURCE=.\quadlist.cpp
# End Source File
# Begin Source File

SOURCE=.\quadlist.h
# End Source File
# Begin Source File

SOURCE=.\skybox.cpp
# End Source File
# Begin Source File

SOURCE=.\skybox.h
# End Source File
# End Group
# Begin Group "스레드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AThread.cpp
# End Source File
# Begin Source File

SOURCE=.\AThread.h
# End Source File
# Begin Source File

SOURCE=.\CItemTexLoadThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CItemTexLoadThread.h
# End Source File
# Begin Source File

SOURCE=.\CLoadingBarThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CLoadingBarThread.h
# End Source File
# Begin Source File

SOURCE=.\CLogoDisplayThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CLogoDisplayThread.h
# End Source File
# Begin Source File

SOURCE=.\CNetProcThread.cpp
# End Source File
# Begin Source File

SOURCE=.\CNetProcThread.h
# End Source File
# End Group
# Begin Group "핵쉴드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HShield.h
# End Source File
# Begin Source File

SOURCE=.\HsUserUtil.h
# End Source File
# End Group
# Begin Group "게임가드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NPGameLib.h
# End Source File
# End Group
# Begin Group "App"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\display.cpp
# End Source File
# Begin Source File

SOURCE=.\Jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\RockClient.cpp
# End Source File
# Begin Source File

SOURCE=.\RockClient.h
# End Source File
# Begin Source File

SOURCE=.\RockClientNet.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# End Group
# Begin Group "시간"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\GTimer.h
# End Source File
# Begin Source File

SOURCE=.\ListTime.cpp
# End Source File
# Begin Source File

SOURCE=.\ListTime.h
# End Source File
# Begin Source File

SOURCE=.\UserTime.cpp
# End Source File
# Begin Source File

SOURCE=.\UserTime.h
# End Source File
# End Group
# Begin Group "Dlg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GMCmdDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GMCmdDlg.h
# End Source File
# Begin Source File

SOURCE=.\PacketTransLogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketTransLogDlg.h
# End Source File
# End Group
# Begin Group "스트링"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\defineHID_ID.h
# End Source File
# Begin Source File

SOURCE=.\DefineSTR_ID.h
# End Source File
# Begin Source File

SOURCE=.\StringManager.cpp
# End Source File
# Begin Source File

SOURCE=.\StringManager.h
# End Source File
# End Group
# Begin Group "렌더"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\CRenderer.h
# End Source File
# Begin Source File

SOURCE=.\CRenderManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CRenderManager.h
# End Source File
# Begin Source File

SOURCE=.\CRenderState.cpp
# End Source File
# Begin Source File

SOURCE=.\CRenderState.h
# End Source File
# Begin Source File

SOURCE=.\CTextureState.cpp
# End Source File
# Begin Source File

SOURCE=.\CTextureState.h
# End Source File
# End Group
# Begin Group "STL 직접구현"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CIndexList.cpp
# End Source File
# Begin Source File

SOURCE=.\CIndexList.h
# End Source File
# End Group
# Begin Group "게임이펙트"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\effect.cpp
# End Source File
# Begin Source File

SOURCE=.\effect.h
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\ORockDocs\기획팀관리\테이블파일\Etc\Interface\CBT.txt"
# End Source File
# Begin Source File

SOURCE="C:\RockDev\Microsoft DX90SDK Update\Include\dmdls.h"
# End Source File
# Begin Source File

SOURCE=..\..\..\ORockDocs\ETC\gm_help.txt
# End Source File
# Begin Source File

SOURCE="..\..\..\ORockDocs\기획팀관리\테이블파일\Etc\Interface\HelpTip.txt"
# End Source File
# Begin Source File

SOURCE=.\SeiyaOnline.exe.mainfest
# End Source File
# Begin Source File

SOURCE="..\..\..\ORockDocs\기획팀관리\테이블파일\Etc\Interface\TID.txt"
# End Source File
# Begin Source File

SOURCE=.\work.txt
# End Source File
# End Target
# End Project
