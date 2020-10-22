#ifndef __DEFINE_WID_H__
#define __DEFINE_WID_H__
//-----------------------------------------------------------------------------
	
#define MAX_WND_AMOUNT					20000

//-----------------------------------------------------------------------------
#define WID_None						0
#define WID_DeskTop						1


// 로딩바
//-----------------------------------------------------------------------------
#define WID_LoadingWnd					10
#define WID_LoadingBG					11
#define WID_LoadingBar					12

///-- 메세지박스
#define WID_MessageBox					15

// 로그인 윈도우
//-----------------------------------------------------------------------------
#define WID_LoginWnd					100
#define WID_Login_UserNameEdit			101
#define WID_Login_PasswordEdit			102
#define WID_Login_OkBtn					103
#define WID_Login_CancelBtn				104

// 시스템메뉴 윈도우
//-----------------------------------------------------------------------------
#define WID_SystemMenuWnd				200
#define WID_System_CloseBtn				201//닫기 버튼
#define WID_System_LogOffBtn			202//로그 오프
#define WID_System_GameExitBtn			203//게임 종료
#define WID_System_ScreenShotBtn		204//스크린 샷
#define WID_System_OptionBtn			205//옵션
#define WID_System_CommonWordsBtn		206//상용어 편집



// 서버리스트 윈도우
//-----------------------------------------------------------------------------
#define WID_ServerListWnd				300						// 서버 리스트 창

#define WID_ServerBtn0					301						// 서버 선택 버튼 0
#define WID_ServerBtn1					302 					// 서버 선택 버튼 1
#define WID_ServerBtn2					303						// 서버 선택 버튼 2
#define WID_ServerBtn3					304						// 서버 선택 버튼 3
#define WID_ServerBtn4					315						// 서버 선택 버튼 4
#define WID_ServerBtn5					316						// 서버 선택 버튼 5
#define WID_ServerBtn6					317						// 서버 선택 버튼 6
#define WID_ServerBtn7					318						// 서버 선택 버튼 7

#define WID_ServerConnectBtn			305						// 접속 버튼
#define WID_ServerCancelBtn				306						// 취소 버튼
#define WID_ServerScrollBtn_S			307						// 서버월드 스크롤 버튼
#define WID_ServerScrollBtn_C			319						// 서버채널  스크롤 버튼

#define WID_ServerProgressBar0			308						// 서버 프로그레시브바 0				
#define WID_ServerProgressBar1			309						// 서버 프로그레시브바 1				
#define WID_ServerProgressBar2			310						// 서버 프로그레시브바 2				
#define WID_ServerProgressBar3			311						// 서버 프로그레시브바 3			
#define WID_ServerProgressBar4			312						// 서버 프로그레시브바 4				
#define WID_ServerProgressBar5			313						// 서버 프로그레시브바 5			
#define WID_ServerProgressBar6			314						// 서버 프로그레시브바 6			

// 메인 윈도우
//-----------------------------------------------------------------------------
#define WID_MainMenu_Wnd				400
#define WID_MainMenuLv_Wnd				401
#define WID_MainExp_Wnd					402
#define WID_MainButtons_Wnd				403
#define WID_MAIN_ENDU_WARN				404
#define WID_MainEventButtons_Wnd        405

#define WID_MainMenuLvBtn_Wnd			437						// 업 포인트 있을때 버튼

///#define WID_MainMenu_LVgage				402
#define WID_MainMenu_LVHighlight		406
#define WID_MainMenu_LVBtn				407

#define WID_MainMenu_UpBtn				408
#define WID_MainMenu_DownBtn			409

#define WID_MainMenu_QSlot0				410
#define WID_MainMenu_QSlot1				411
#define WID_MainMenu_QSlot2				412
#define WID_MainMenu_QSlot3				413
#define WID_MainMenu_QSlot4				414
#define WID_MainMenu_QSlot5				415
#define WID_MainMenu_QSlot6				416
#define WID_MainMenu_QSlot7				417
#define WID_MainMenu_QSlot8				418
#define WID_MainMenu_QSlot9				419

#define WID_MainMenu_StatWndBtn			430
#define WID_MainMenu_SkillWndBtn		431
#define WID_MainMenu_MessengerWndBtn	432
#define WID_MainMenu_InvenWndBtn		433
#define WID_MainMenu_QuestWndBtn		434
#define WID_MainMenu_SystemWndBtn		435	
#define WID_MainMenu_HelpWndBtn			436
#define WID_MainMenu_LogWndBtn			437
#define WID_MainMenu_RadioBroadBtn		438
#define WID_MainMenu_GuildWndBtn		439
#define WID_MainMenu_CashWndBtn			440
#define WID_MainMenu_EventWndBtn		441



// 미니맵 윈도우
//-----------------------------------------------------------------------------
#define WID_MiniMapWnd					500
#define WID_Map_CloseBtn				501
#define WID_MiniMapViewer				502

#define WID_LocalEntireMap				503					//지역 전체맵

#define WID_MiniMap_InfoText128_Name	510	
#define WID_MiniMap_InfoText128_Coord	511
#define WID_MiniMap_InfoText256_Name	512
#define WID_MiniMap_InfoText256_Coord	513
#define WID_MINIMAP_MENU	514


// 다른 유저 및 NPC 정보창
//----------------------------------------------------------------------------- 
#define WID_PlayerInfo_Wnd				600					// 다른 유저 및 NPC 정보창 
#define WID_PlayerInfo_LevelText		601					// 레벨 표시  
#define WID_PlayerInfo_NameText			602					// 이름 표시 
#define WID_PlayerInfo_HPGauge			603					// 체력바 
#define WID_PlayerInfo_SPMPGauge		604					// 스테이너, 마나 
#define WID_PlayerInfo_HPText			605					// HP 표시  
#define WID_PlayerInfo_SMPText			606					// SMP 표시

#define WID_PlayerInfo_PrevHPGauge      607					// 이전 체력바
#define WID_PlayerInfo_GlowHPGauge	    608					// 글로우 체력바
#define WID_PlayerInfo_GlowWnd		    609					// 글로우 윈도우 

#define WID_PlayerInfo_KeepupSkillImg0	610
#define WID_PlayerInfo_KeepupSkillImg1	611
#define WID_PlayerInfo_KeepupSkillImg2	612
#define WID_PlayerInfo_KeepupSkillImg3	613
#define WID_PlayerInfo_KeepupSkillImg4	614
#define WID_PlayerInfo_KeepupSkillImg5	615
#define WID_PlayerInfo_KeepupSkillImg6	616
#define WID_PlayerInfo_KeepupSkillImg7	617
#define WID_PlayerInfo_KeepupSkillImg8	618
#define WID_PlayerInfo_KeepupSkillImg9	619
#define WID_PlayerInfo_KeepupSkillImg10	700
#define WID_PlayerInfo_KeepupSkillImg11	701
#define WID_PlayerInfo_KeepupSkillImg12	702
#define WID_PlayerInfo_KeepupSkillImg13	703
#define WID_PlayerInfo_KeepupSkillImg14	704
#define WID_PlayerInfo_KeepupSkillImg15	705
#define WID_PlayerInfo_KeepupSkillImg16	706
#define WID_PlayerInfo_KeepupSkillImg17	707
#define WID_PlayerInfo_KeepupSkillImg18	708
#define WID_PlayerInfo_KeepupSkillImg19 709


#define WID_PlayerInfo_Level1Img		640	
#define WID_PlayerInfo_Level2Img		641
#define WID_PlayerInfo_Level3Img		642	
#define WID_PlayerInfo_Level4Img		643	
#define WID_PlayerInfo_Level5Img		644

#define WID_PlayerInfo_Warning			645		



// 다른 유저 및 NPC 정보창
//----------------------------------------------------------------------------- 
#define WID_NonPlayerInfo_Wnd			620					// 다른 유저 및 NPC 정보창 

#define WID_NonPlayerInfo_CharImg		621					// 캐릭터 이미지  
#define WID_NonPlayerInfo_LevelText		622					// 레벨 표시  
#define WID_NonPlayerInfo_NameText		623					// 이름 표시
#define WID_NonPlayerInfo_PrevHPGauge   637					// 이전 체력바
#define WID_NonPlayerInfo_HPGauge		624					// 체력바 
#define WID_NonPlayerInfo_SPMPGauge		625					// 스테이너, 마나 
#define WID_NonPlayerInfo_AddBtn		626					// Action Button 확대, 축소 

#define WID_NonPlayerInfo_PrevHPGauge   637					// 이전 체력바
#define WID_NonPlayerInfo_GlowHPGauge	638					// 글로우 체력바
#define WID_NonPlayerInfo_GlowWnd		639					// 글로우 윈도우

#define WID_NonPlayerInfo_KeepupSkillImg0	627
#define WID_NonPlayerInfo_KeepupSkillImg1	628
#define WID_NonPlayerInfo_KeepupSkillImg2	629
#define WID_NonPlayerInfo_KeepupSkillImg3	630
#define WID_NonPlayerInfo_KeepupSkillImg4	631
#define WID_NonPlayerInfo_KeepupSkillImg5	632
#define WID_NonPlayerInfo_KeepupSkillImg6	633
#define WID_NonPlayerInfo_KeepupSkillImg7	634
#define WID_NonPlayerInfo_KeepupSkillImg8	635
#define WID_NonPlayerInfo_KeepupSkillImg9	636
#define WID_NonPlayerInfo_KeepupSkillImg10	730
#define WID_NonPlayerInfo_KeepupSkillImg11	731
#define WID_NonPlayerInfo_KeepupSkillImg12	732
#define WID_NonPlayerInfo_KeepupSkillImg13	733
#define WID_NonPlayerInfo_KeepupSkillImg14	734
#define WID_NonPlayerInfo_KeepupSkillImg15	735
#define WID_NonPlayerInfo_KeepupSkillImg16	736
#define WID_NonPlayerInfo_KeepupSkillImg17	737
#define WID_NonPlayerInfo_KeepupSkillImg18	738
#define WID_NonPlayerInfo_KeepupSkillImg19	739


//#define WID_NonPlayerInfo_StatICONImg1	627					// 상태 아이콘 표시 
//#define WID_NonPlayerInfo_StatICONImg2	628				
//#define WID_NonPlayerInfo_StatICONImg3	629				
//#define WID_NonPlayerInfo_StatICONImg4	630				
//#define WID_NonPlayerInfo_StatICONImg5	631				
//#define WID_NonPlayerInfo_StatICONImg6	632				


#define WID_NonPlayerInfoAddWnd			650					

#define WID_NonPlayerInfoAdd_ChatBtn	651					// 대화 (귓속말, NPC 대화)
#define WID_NonPlayerInfoAdd_PartyBtn	652					// 파티 초대  
#define WID_NonPlayerInfoAdd_FriendBtn	653					// 친구 추가 
#define WID_NonPlayerInfoAdd_TradeBtn	654					// 거래 
#define WID_NonPlayerInfoAdd_ChaseBtn	655					// 따라다니기 
#define WID_NonPlayerInfoAdd_AttackBtn	656					// 공격  




// 콤보 게이지
//----------------------------------------------------------------------------- 
#define WID_ComboGauge_Wnd				800					// 콤보 게이지 기본 창
#define WID_ComboGauge_Level			801					// 숫자 레벨 표시 
#define WID_ComboGauge_Gauge 			802					// 게이지 표시 



// 프로이마 히트바
//-----------------------------------------------------------------------------
#define WID_ChantHitBar_Wnd				810					// 히트바 배경  
#define WID_ChantHitBar_TimeBar			811					// 타이밍 바 
#define WID_ChantHitBar_Complete		812					// 영창 종료 
#define WID_ChantHitBar_Point1			813					// 영창 포인트 1 
#define WID_ChantHitBar_Point2			814					// 영창 포인트 2 
#define WID_ChantHitBar_Point3			815					// 영창 포인트 3 
#define WID_ChantHitBar_Point4			816					// 영창 포인트 4 
#define WID_ChantHitBar_Point5			817					// 영창 포인트 5 


//딜레이바
#define WID_DelayBar_Wnd                820                 //딜레이바 배경
#define WID_DelayBar_TimeBar            821                 //딜레이 타이밍 바
#define WID_DelayBar_NotifyTex          822                 //알림 텍스트


//파티수락여부 윈도우
#define WID_Party_YesNo_Wnd				823					//파티 수락여부 배경
#define WID_Party_YesBtn				824                 //파티초대 수락버튼
#define WID_Party_NoBtn					825					//파티초대 거부버튼
#define WID_Party_NotifyTex1			826					//파티초대 알림 텍스트
#define WID_Party_NotifyTex2			827					
#define WID_Party_NotifyTex3			828					

// 타이밍 히트 게이지 
//-----------------------------------------------------------------------------



// 컨디션 게이지 
//-----------------------------------------------------------------------------




// 채팅입력 윈도우
//-----------------------------------------------------------------------------
#define WID_ChatBoxWnd					900
#define WID_ChatBox_MessageEdit			901//대화 입력	
#define WID_ChatBox_SizeBtn				902//채팅 스크롤 조절.... 
#define WID_ChatBox_ChatTypeBtn			903//채팅 타입...전체, 귓속말, 파티,....
#define WID_ChatBox_ChatTypeImg			904

// 채팅출력 윈도우
//-----------------------------------------------------------------------------
#define WID_ChatOutputWnd				910
#define WID_ChatOutput_Text				911
#define WID_ChatOutput_FUpImg			912	

// 채팅메세지 윈도우
//-----------------------------------------------------------------------------
#define WID_ChatMessageWnd				920
#define WID_ChatMessage_Text			921


// 시스템메세지 윈도우
//-----------------------------------------------------------------------------
#define WID_SysMsgOutputWnd				930
#define WID_SysMsgOutput_Text			931
#define WID_AdminMsgOutput_Text			932
#define WID_MapNameMsgOutput_Text		933



// 캐릭터 상태 윈도우
//-----------------------------------------------------------------------------
#define WID_StatWnd						1000
#define WID_Stat_CloseBtn				1001
#define WID_Stat_SIconImg				1002
#define WID_Stat_StatText				1003
#define WID_Stat_FameText				1004
#define WID_Stat_NameText				1005
#define WID_Stat_StateText				1006
#define WID_Stat_GenderText				1007
#define WID_Stat_GuildText				1008
#define WID_Stat_ProfessionText			1009

#define WID_Stat_HPText					1010
#define WID_Stat_HPGauge				1011
#define WID_Stat_MPText					1012
#define WID_Stat_MPGauge				1013
#define WID_Stat_SPText					1014
#define WID_Stat_SPGauge				1015

#define WID_Stat_SubClass_TEXT			1016
#define WID_Stat_SubClass_Level_TEXT	1017
#define WID_Stat_SubClass_EXP_TEXT		1018

#define WID_Stat_AttText				1020//파워
#define WID_Stat_DefText				1021//방어
#define WID_Stat_MagText				1022//교감
#define WID_Stat_RegText				1023//저항
#define WID_Stat_AgiText				1024//집중
#define WID_Stat_DexText				1025//반응
#define WID_Stat_ChaText				1026//매력
#define WID_Stat_LukText				1027//행운

#define WID_Stat_PhysicAttText			1030//물리공격력
#define WID_Stat_PhysicDefText			1031//물리방어력
#define WID_Stat_MagicDefText			1035//마법방어력

#define WID_Stat_MagicAttText2			1034//마법공격력
#define WID_Stat_MagicDefText2			1056//마법방어력
#define WID_Stat_PhysicDefText2			1057//물리방어력


#define WID_Stat_HitText				1032//명중
#define WID_Stat_EvasionText			1033//회피
#define WID_Stat_MagicUSpeedText		1036//시전속도
#define WID_Stat_MagicSRateText			1037//시전성공률

#define WID_Stat_LevelText				1040//레벨
//#define WID_Stat_Xprism					1041//엑스프리즘
#define WID_Stat_AbilityUpPointText		1042//능력업 포인트
#define WID_Stat_SkillUpPointText		1043//스킬업 포인트
#define WID_Stat_MoveSpeedText			1044//이동속도 포인트
#define WID_Stat_WeightText				1045//체중


#define WID_Stat_LVgage					1050
#define WID_Stat_LVHighlight			1054
#define WID_Stat_LVBtn					1055




// 능력업 처리 윈도우
//-----------------------------------------------------------------------------
#define WID_AbilityUpWnd				1100
#define WID_Aup_OkBtn					1101
#define WID_Aup_CancelBtn				1102
#define WID_Aup_CloseBtn				1103

#define WID_Aup_AttSpin					1105//파워
#define WID_Aup_DefSpin					1106//방어
#define WID_Aup_AgiSpin					1107//집중
#define WID_Aup_DexSpin					1108//반응
#define WID_Aup_MagSpin					1109//교감
#define WID_Aup_ResSpin					1110//저항

#define WID_Aup_PointText				1111//남은 능력업 포인트
#define WID_Aup_DistriBtn				1112//다시 배분

// 능력업 처리 확인 윈도우
//-----------------------------------------------------------------------------
#define WID_AbilityUpConfirmWnd			1120
#define WID_AupCfm_OkBtn				1121
#define WID_AupCfm_CancelBtn			1122


// 부활 확인 윈도우 
//-----------------------------------------------------------------------------
#define WID_AliveWnd					1130
#define WID_Alive_AskImg				1131
#define WID_Alive_OkBtn					1132



// 소지품 윈도우(인벤토리)
//-----------------------------------------------------------------------------
#define WID_InventoryWnd				1200
#define WID_Inven_CloseBtn				1201
#define WID_Inven_MoneyBtn				1202
#define WID_Inven_MoneyText				1203
#define WID_Inven_NormalSBtn			1204	
#define WID_Inven_QuestSBtn				1205
#define WID_Inven_Scroll				1206

#define WID_Inven_BgSlot				1207

#define WID_Inven_WearSlot0				1210
#define WID_Inven_WearSlot1				1211
#define WID_Inven_WearSlot2				1212
#define WID_Inven_WearSlot3				1213
#define WID_Inven_WearSlot4				1214
#define WID_Inven_WearSlot5				1215
#define WID_Inven_WearSlot6				1216
#define WID_Inven_WearSlot7				1217
#define WID_Inven_WearSlot8				1218
#define WID_Inven_WearSlot9				1219

#define WID_Inven_ItemSlot0				1220
#define WID_Inven_ItemSlot1				1221
#define WID_Inven_ItemSlot2				1222
#define WID_Inven_ItemSlot3				1223
#define WID_Inven_ItemSlot4				1224

#define WID_Inven_ItemSlot5				1225
#define WID_Inven_ItemSlot6				1226
#define WID_Inven_ItemSlot7				1227
#define WID_Inven_ItemSlot8				1228
#define WID_Inven_ItemSlot9				1229

#define WID_Inven_ItemSlot10			1230
#define WID_Inven_ItemSlot11			1231
#define WID_Inven_ItemSlot12			1232
#define WID_Inven_ItemSlot13			1233
#define WID_Inven_ItemSlot14			1234

#define WID_Inven_ItemSlot15			1235
#define WID_Inven_ItemSlot16			1236
#define WID_Inven_ItemSlot17			1237
#define WID_Inven_ItemSlot18			1238
#define WID_Inven_ItemSlot19			1239

#define WID_Inven_ItemSlot20			1240
#define WID_Inven_ItemSlot21			1241
#define WID_Inven_ItemSlot22			1242
#define WID_Inven_ItemSlot23			1243
#define WID_Inven_ItemSlot24			1244

//말과장비 인벤 추가 
#define WID_Inven_WearSlot10				1245
#define WID_Inven_WearSlot11				1246
#define WID_Inven_WearSlot12				1247
#define WID_Inven_WearSlot13				1248

//스킨아이템 
#define WID_Inven_WearSlot14				1249
#define WID_Inven_WearSlot15				1250
#define WID_Inven_WearSlot16				1251

//-----------------------------------------------------------------------------
// 스킬 윈도우
//-----------------------------------------------------------------------------
#define WID_SkillWnd							1300
#define WID_Skill_CloseBtn						1301
#define WID_Skill_CombatSBtn					1302
#define WID_Skill_NonCombatSBtn					1303 
#define WID_Skill_SubClassSBtn  				1304	
#define WID_Skill_ActiveSBtn 					1305
#define WID_Skill_PassiveSBtn  					1306
#define WID_Skill_SkillPointText 				1307
#define WID_Skill_SkillPointNumText 			1308
#define WID_Skill_Scroll 						1309
#define WID_Skill_Slot0							1310 
#define WID_Skill_Slot1 						1311
#define WID_Skill_Slot2 						1312
#define WID_Skill_Slot3 						1313
#define WID_Skill_Slot4 						1314
#define WID_Skill_Slot5 						1315
#define WID_Skill_Slot6 						1316
#define WID_Skill_Slot7 						1317
#define WID_Skill_Slot8 						1318
#define WID_Skill_Slot9 						1319
#define WID_Skill_SkillLevelBtn0 				1340
#define WID_Skill_SkillLevelBtn1 				1341
#define WID_Skill_SkillLevelBtn2 				1342
#define WID_Skill_SkillLevelBtn3 				1343
#define WID_Skill_SkillLevelBtn4 				1344
#define WID_Skill_SkillLevelBtn5 				1345
#define WID_Skill_SkillLevelBtn6 				1346
#define WID_Skill_SkillLevelBtn7 				1347
#define WID_Skill_SkillLevelBtn8 				1348
#define WID_Skill_SkillLevelBtn9 				1349
#define WID_Skill_SkillLevelBar0 				1350
#define WID_Skill_SkillLevelBar1 				1351
#define WID_Skill_SkillLevelBar2 				1352
#define WID_Skill_SkillLevelBar3 				1353
#define WID_Skill_SkillLevelBar4 				1354
#define WID_Skill_SkillLevelBar5 				1355
#define WID_Skill_SkillLevelBar6 				1356
#define WID_Skill_SkillLevelBar7 				1357
#define WID_Skill_SkillLevelBar8 				1358
#define WID_Skill_SkillLevelBar9 				1359
#define WID_Skill_ShortCutSlot_Ctrl				1360
#define WID_Skill_ShortCutSlot_Shift			1361

// 스킬 레벨업 윈도우 
#define WID_SkillLevelUpWnd						1370
#define WID_SkillLevelUp_TitleText				1371
#define WID_SkillLevelUp_CloseBtn				1372
#define WID_SkillLevelUp_SkillImg				1373
#define WID_SkillLevelUp_LevelUpDescText		1374
#define WID_SkillLevelUp_OKBtn					1375
#define WID_SkillLevelUp_CancelBtn				1376
#define WID_SkillLevelUp_CurtSkillNameText		1378 
#define WID_SkillLevelUp_CurtSkillDescText		1379 
#define WID_SkillLevelUp_CurtSkillNeedItemText	1380 
#define WID_SkillLevelUp_CurtSkillLevelText		1381 
#define WID_SkillLevelUp_CurtSkillGaugeTypeText	1382 
#define WID_SkillLevelUp_CurtSkillGaugeValueText	1383
#define WID_SkillLevelUp_CurtSkillWastePointText	1384 
#define WID_SkillLevelUp_CurtSkillProimaTypeText	1385 
#define WID_SkillLevelUp_NextSkillNameText		1386 
#define WID_SkillLevelUp_NextSkillDescText		1387 
#define WID_SkillLevelUp_NextSkillNeedItemText	1388 
#define WID_SkillLevelUp_NextSkillLevelText		1389 
#define WID_SkillLevelUp_NextSkillGaugeTypeText	1390 
#define WID_SkillLevelUp_NextSkillGaugeValueText	1391
#define WID_SkillLevelUp_NextSkillWastePointText	1392 
#define WID_SkillLevelUp_NextSkillProimaTypeText	1393 




// 퀘스트 창
//-----------------------------------------------------------------------------
#define WID_Quest_Wnd					1500					// 퀘스트 창			
#define WID_Quest_Close_Btn				1501					// 닫기 버튼
#define WID_Quest_Cancel_Btn			1502					// 퀘스트 포기 버튼
#define WID_Quest_AllTab_Btn			1503					// 전체   버튼
#define WID_Quest_NormalTab_Btn			1504					// 일반   버튼
#define WID_Quest_StoryTab_Btn			1505					// 스토리 버튼
#define WID_Quest_ChangeTab_Btn			1506					// 전직   버튼
#define WID_Quest_ContextText			1507					// 퀘스트 내용 

#define WID_Quest_QuestName0_Txt		1510
#define WID_Quest_Client0_Txt			1511
#define WID_Quest_TimeLimit0_Txt		1512
#define WID_Quest_RecmdLevel0_Txt		1513
#define WID_Quest_QuestName1_Txt		1514
#define WID_Quest_Client1_Txt			1515
#define WID_Quest_TimeLimit1_Txt		1516
#define WID_Quest_RecmdLevel1_Txt		1517
#define WID_Quest_QuestName2_Txt		1518
#define WID_Quest_Client2_Txt			1519
#define WID_Quest_TimeLimit2_Txt		1520
#define WID_Quest_RecmdLevel2_Txt		1521
#define WID_Quest_QuestName3_Txt		1522
#define WID_Quest_Client3_Txt			1523
#define WID_Quest_TimeLimit3_Txt		1524
#define WID_Quest_RecmdLevel3_Txt		1525

#define WID_Quest_Field0_Btn			1530					// 퀘스트 리스트 버튼 0
#define WID_Quest_Field1_Btn			1531					// 퀘스트 리스트 버튼 1
#define WID_Quest_Field2_Btn			1532					// 퀘스트 리스트 버튼 2
#define WID_Quest_Field3_Btn			1533					// 퀘스트 리스트 버튼 3

#define WID_Quest_List_Scroll			1540					// 퀘스트 리스트 스크롤
#define WID_Quest_Context_Scroll		1541					// 퀘스트 내요   스크롤



// 스킬 습득창
//-----------------------------------------------------------------------------
#define WID_SkillLearn_Wnd					2500				// 스킬 습득창			
#define WID_SkillLearn_Close_Btn			2501				// 닫기 버튼
#define WID_SkillLearn_Learn_Btn			2502				// 스킬 습득 버튼
#define WID_SkillLearn_ContextText			2503				// 퀘스트 내용 
#define WID_SkillLearn_ClassText			2504				// 퀘스트 내용 

#define WID_SkillLearn_SkillLearnName1_Txt	2510
#define WID_SkillLearn_NeedSkill11_Txt		2511
#define WID_SkillLearn_NeedSkill21_Txt		2512

#define WID_SkillLearn_SkillLearnName2_Txt	2513
#define WID_SkillLearn_NeedSkill12_Txt		2514
#define WID_SkillLearn_NeedSkill22_Txt		2515

#define WID_SkillLearn_SkillLearnName3_Txt	2516
#define WID_SkillLearn_NeedSkill13_Txt		2517
#define WID_SkillLearn_NeedSkill23_Txt		2518

#define WID_SkillLearn_SkillLearnName4_Txt	2519
#define WID_SkillLearn_NeedSkill14_Txt		2520
#define WID_SkillLearn_NeedSkill24_Txt		2521

#define WID_SkillLearn_LernCost_1_Txt		2522
#define WID_SkillLearn_LernCost_2_Txt		2523
#define WID_SkillLearn_LernCost_3_Txt		2524
#define WID_SkillLearn_LernCost_4_Txt		2525

#define WID_SkillLearn_Field0_Btn			2530					// 스킬 습득 리스트 버튼 0
#define WID_SkillLearn_Field1_Btn			2531					// 스킬 습득 리스트 버튼 1
#define WID_SkillLearn_Field2_Btn			2532					// 스킬 습득 리스트 버튼 2
#define WID_SkillLearn_Field3_Btn			2533					// 스킬 습득 리스트 버튼 3

#define WID_SkillLearn_List_Scroll			2540					// 스킬 습득 리스트 스크롤
#define WID_SkillLearn_Context_Scroll		2541					// 스킬 습득 내용 스크롤





// NPC 대화창 
//-----------------------------------------------------------------------------
#define WID_NpcChatWnd					1600
#define WID_NpcChat_PictureImg			1601
#define WID_NpcChat_AskImg				1602
#define WID_NpcChat_Ask_NextBtn			1603
#define WID_NpcChat_Ask_CloseBtn		1604
#define WID_NpcChat_AnswerBtn1			1605
#define WID_NpcChat_AnswerBtn2			1606
#define WID_NpcChat_AnswerBtn3			1607
#define WID_NpcChat_AnswerBtn4			1608
#define WID_NpcChat_AnswerBtn5			1609
#define WID_NpcChat_AnswerBtn6			1610
#define WID_NpcChat_AnswerBtn7			1611
#define WID_NpcChat_AnswerBtn8			1612
#define WID_NpcChat_AnswerBtn9			1613
#define WID_NpcChat_AnswerBtn10			1614
#define WID_NpcChat_AskText				1615	
#define WID_NpcChat_NameText			1616	
#define WID_NpcChat_LevelText			1617



// 상점 거래창
//-----------------------------------------------------------------------------
#define WID_StoreWnd					1800
#define WID_Store_CloseBtn				1801
#define WID_Store_Item1SBtn				1802	
#define WID_Store_Item2SBtn				1803
//#define WID_Store_HighItemSBtn			1804
#define WID_Store_Scroll				1805
//#define WID_Store_ArmorItemScroll
//#define WID_Store_HighItemScroll

#define WID_Store_BgSlot				1806

#define WID_Store_ItemSlot0				1810
#define WID_Store_ItemSlot1				1811
#define WID_Store_ItemSlot2				1812
#define WID_Store_ItemSlot3				1813
#define WID_Store_ItemSlot4				1814

#define WID_Store_ItemSlot5				1815
#define WID_Store_ItemSlot6				1816
#define WID_Store_ItemSlot7				1817
#define WID_Store_ItemSlot8				1818
#define WID_Store_ItemSlot9				1819

#define WID_Store_ItemSlot10			1820
#define WID_Store_ItemSlot11			1821
#define WID_Store_ItemSlot12			1822
#define WID_Store_ItemSlot13			1823
#define WID_Store_ItemSlot14			1824

#define WID_Store_ItemSlot15			1825
#define WID_Store_ItemSlot16			1826
#define WID_Store_ItemSlot17			1827
#define WID_Store_ItemSlot18			1828
#define WID_Store_ItemSlot19			1829

#define WID_Store_ItemSlot20			1830
#define WID_Store_ItemSlot21			1831
#define WID_Store_ItemSlot22			1832
#define WID_Store_ItemSlot23			1833
#define WID_Store_ItemSlot24			1834


// 아이템 구입창
//-----------------------------------------------------------------------------
#define WID_ItemBuyWnd					1850
#define WID_ItemBuy_IconImg				1851
#define WID_ItemBuy_PriceText			1852
#define WID_ItemBuy_OkBtn				1853
#define WID_ItemBuy_CancelBtn			1854

// 아이템 판매창
//-----------------------------------------------------------------------------
#define WID_ItemSellWnd					1860
#define WID_ItemSell_IconImg			1861
#define WID_ItemSell_PriceText			1862
#define WID_ItemSell_OkBtn				1863
#define WID_ItemSell_CancelBtn			1864

// 은행 거래창
//-----------------------------------------------------------------------------
#define WID_BankWnd						1900
#define WID_Bank_CloseBtn				1901
#define WID_Bank_NormalSBtn				1902
#define WID_Bank_TreasureSBtn			1903
#define WID_Bank_Scroll					1904
#define WID_Bank_DepositBtn				1905
#define WID_Bank_WithdrawalBtn			1906
//#define WID_Bank_TreasureScroll

// 돈찾기
//-----------------------------------------------------------------------------
#define WID_WithdrawalWnd				1950
#define WID_Withdrawal_MoneyText		1951
#define WID_Withdrawal_OkBtn			1952
#define WID_Withdrawal_CancelBtn		1953

// 돈맡기기
//-----------------------------------------------------------------------------
#define WID_DepositWnd					1960
#define WID_Deposit_MoneyText			1961
#define WID_Deposit_OkBtn				1962
#define WID_Deposit_CancelBtn			1963


// 숫자 입력창
//-----------------------------------------------------------------------------
#define WID_NumberInput_Wnd				1970
#define WID_NumberInput_Edit			1971
#define WID_NumberInput_OkBtn			1972
#define WID_NumberInput_CancelBtn		1973


// 엘드 입력창
//-----------------------------------------------------------------------------
#define WID_MoneyThrow_Wnd				1980
#define WID_MoneyThrow_InfoText			1981
#define WID_MoneyThrow_Edit				1982
#define WID_MoneyThrow_OkBtn			1983
#define WID_MoneyThrow_CancelBtn		1984


//-----------------------------------------------------------------------------
// 메세지 창
//-----------------------------------------------------------------------------
#define WID_MessageWnd					2000
#define WID_Message_OK_Btn				2001
#define WID_Message_Cancel_Btn			2002
#define WID_Message_Context_Btn		    2003
#define WID_Message_ImageWnd			2004						// 메세지 이미지

//------------------------------------------------------------------------------
// 캐릭터 선택 / 생성 창
//------------------------------------------------------------------------------
#define WID_CharSelect_Wnd				2100					// 캐릭터 선택창

#define WID_CharSelect_Name_BoxImg		2101					// 캐릭터 선택 이름

#define WID_CharSelect_BoxImg           2128					// 캐릭터 선택 배경

#define WID_CharSelect_Delete_Btn		2103					// 캐릭터 선택 오른쪽 버튼

#define WID_CharSelect_Start_Btn		2104					// 게임 시작 버튼

#define WID_CharSelect_Server_Btn0		2105					// 서버 선택 
#define WID_CharSelect_Server_Btn1		2131

#define WID_CharSelect_Create_Btn		2106					// 생성 버튼 

#define WID_CharCreate_Rot_Left_Btn		2107					// 캐릭터 생성 회전 ( 왼쪽   ) 버튼
#define WID_CharCreate_Rot_Right_Btn	2108					// 캐릭터 생성 회전 ( 오른쪽 ) 버튼

#define WID_CharCreate_Millena_Btn		2109					// 밀레나 선택 버튼
#define WID_CharCreate_Rain_Btn			2110					// 레  인 선택 버튼

#define WID_CharCreate_MALE_Btn			2111					// 남자 선택
#define WID_CharCreate_FEMALE_Btn		2112					// 여자 선택

#define WID_CharCreate_Hair_Left_Btn	2113					// 헤어 왼  쪽 버튼
#define WID_CharCreate_Hair_Right_Btn	2114					// 헤어 오른쪽 버튼

#define WID_CharCreate_Face_Left_Btn	2115					// 얼굴 왼  쪽 버튼
#define WID_CharCreate_Face_Right_Btn	2116					// 얼굴 오른쪽 버튼

#define WID_CharCreate_Create_Btn		2117					// 생성 버튼
#define WID_CharCreate_Cancel_Btn		2118					// 취소 버튼

#define WID_CharCreate_Help				2130					// 도움말

#define WID_CharSelect_Name_TextWnd		2121					// 캐릭터 이름 에디트 박스
#define WID_CharSelect_Level_TextWnd	2122					// 캐릭터 레벨 에디트 박스

#define WID_CharCreate_Dlg				2123					// 캐릭터 생성 창

#define WID_CharCreate_Hair_TextWnd		2124					// 캐릭터 생성 창 머리 타입
#define WID_CharCreate_Face_TextWnd		2125					// 캐릭터 생성 창 머리 타입

#define WID_CharSelect_Create_BoxImg    2126					// 캐릭터 선택 생성 배경

#define WID_CharSelect_Class_TextWnd	2127					// 직업 텍스트 박스

#define WID_CharCreate_Name_EditBox		2129

#define WID_ClassView_Millena_Image		2102					// 밀레나 클래스 미리보기 배경
#define WID_ClassView_Rain_Image		2119					// 레  인 클래스 미리보기 배경

#define WID_CharSelect_exit_Btn 	    2120                    // 게임종료

#define WID_CharSelect0					2132					// 캐릭터 선택창 0
#define WID_CharSelect1					2133					// 캐릭터 선택창 1
#define WID_CharSelect2					2134					// 캐릭터 선택창 2
#define WID_CharSelect3					2135					// 캐릭터 선택창 3
#define WID_CharSelect4					2136					// 캐릭터 선택창 4
#define WID_CharacterZoom				2137					// 캐릭터 줌
#define WID_CharacterClassView			2138					// 캐릭터 클래스 미리 보기
#define WID_CharacterClassViewReturn	2139					// 캐릭터 클래스 미리 보기 뒤로

//................................................................................................................
// 클래스 미리보기 버튼
//................................................................................................................
#define WID_ClassView_SwordmanBtn		2140
#define WID_ClassView_ArcherBtn			2141
#define WID_ClassView_MagicianBtn		2142
#define WID_ClassView_ShamanBtn			2143
#define WID_ClassView_KnightBtn			2144
#define WID_ClassView_MercenaryBtn		2145
#define WID_ClassView_SlayerBtn			2146	
#define WID_ClassView_Soul_BreakerBtn	2147
#define WID_ClassView_ProminasBtn		2148
#define WID_ClassView_PriestBtn			2149
#define WID_ClassView_Holy_AvengerBtn	2150
#define WID_ClassView_SummonerBtn		2151
#define WID_ClassView_HelpText		    2152
#define WID_ClassView_HelpScrollBar		2153
#define WID_Help_Image					2154
//................................................................................................................
#define WID_ArcherBar_Wnd				3300

//------------------------------------------------------------------------------
// 아이템 교환창 
//------------------------------------------------------------------------------
#define WID_Exchange_Wnd				4200	

#define WID_Exchange_CloseBtn			4201		

#define WID_Exchange_MineEldText		4202		
#define WID_Exchange_MineEldBtn			4203		

#define WID_Exchange_YoursEldText		4204		

#define WID_Exchange_UploadBtn			4205	
#define WID_Exchange_OKBtn				4206		
#define WID_Exchange_CancelBtn			4207		

#define WID_Exchange_MineItemSlot0		4208		
#define WID_Exchange_MineItemSlot1		4209		
#define WID_Exchange_MineItemSlot2		4210		
#define WID_Exchange_MineItemSlot3		4211		
#define WID_Exchange_MineItemSlot4		4212		
#define WID_Exchange_MineItemSlot5		4213		
#define WID_Exchange_MineItemSlot6		4214		
#define WID_Exchange_MineItemSlot7		4215		
#define WID_Exchange_MineItemSlot8		4216  
#define WID_Exchange_MineItemSlot9		4217		

#define WID_Exchange_YoursItemSlot0		4218		
#define WID_Exchange_YoursItemSlot1		4219		
#define WID_Exchange_YoursItemSlot2		4220		
#define WID_Exchange_YoursItemSlot3		4221		
#define WID_Exchange_YoursItemSlot4		4222		
#define WID_Exchange_YoursItemSlot5		4223		
#define WID_Exchange_YoursItemSlot6		4224		
#define WID_Exchange_YoursItemSlot7		4225		
#define WID_Exchange_YoursItemSlot8		4226  
#define WID_Exchange_YoursItemSlot9		4227		


//------------------------------------------------------------------------------
// 보관 창고창 
//------------------------------------------------------------------------------
#define WID_Warehouse_Wnd				4250	

#define WID_Warehouse_CloseBtn			4251		

#define WID_Warehouse_Box1StateBtn		4252		
#define WID_Warehouse_Box2StateBtn		4253		
#define WID_Warehouse_Box3StateBtn		4254		
#define WID_Warehouse_Box4StateBtn		4255		
#define WID_Warehouse_Box5StateBtn		4256

#define WID_Warehouse_DrawOutEldText	4257		
#define WID_Warehouse_DrawOutEldBtn		4258		

#define WID_Warehouse_ItemSlot0			4259 
#define WID_Warehouse_ItemSlot1			4260 
#define WID_Warehouse_ItemSlot2			4261 
#define WID_Warehouse_ItemSlot3			4262 
#define WID_Warehouse_ItemSlot4			4263 
#define WID_Warehouse_ItemSlot5			4264 
#define WID_Warehouse_ItemSlot6			4265 
#define WID_Warehouse_ItemSlot7			4266 
#define WID_Warehouse_ItemSlot8			4267 
#define WID_Warehouse_ItemSlot9			4268 
#define WID_Warehouse_ItemSlot10		4269 
#define WID_Warehouse_ItemSlot11		4270 
#define WID_Warehouse_ItemSlot12		4271 
#define WID_Warehouse_ItemSlot13		4272 
#define WID_Warehouse_ItemSlot14		4273 
#define WID_Warehouse_ItemSlot15		4274 
#define WID_Warehouse_ItemSlot16		4275 
#define WID_Warehouse_ItemSlot17		4276 
#define WID_Warehouse_ItemSlot18		4277 
#define WID_Warehouse_ItemSlot19		4278 
#define WID_Warehouse_ItemSlot20		4279 
#define WID_Warehouse_ItemSlot21		4280 
#define WID_Warehouse_ItemSlot22		4281 
#define WID_Warehouse_ItemSlot23		4282 
#define WID_Warehouse_ItemSlot24		4283 

//------------------------------------------------------------------------------
// 길드 창고창 
//------------------------------------------------------------------------------
#define WID_GuildInven_Wnd				4300	

#define WID_GuildInven_CloseBtn			4301		

#define WID_GuildInven_Box1StateBtn		4302		
#define WID_GuildInven_Box2StateBtn		4303		

#define WID_GuildInven_DrawOutEldText	4304		
#define WID_GuildInven_DrawOutEldBtn	4305		

#define WID_GuildInven_ItemSlot0		4306 
#define WID_GuildInven_ItemSlot1		4307 
#define WID_GuildInven_ItemSlot2		4308 
#define WID_GuildInven_ItemSlot3		4309 
#define WID_GuildInven_ItemSlot4		4310 
#define WID_GuildInven_ItemSlot5		4311 
#define WID_GuildInven_ItemSlot6		4312 
#define WID_GuildInven_ItemSlot7		4313 
#define WID_GuildInven_ItemSlot8		4314 
#define WID_GuildInven_ItemSlot9		4315 
#define WID_GuildInven_ItemSlot10		4316 
#define WID_GuildInven_ItemSlot11		4317 
#define WID_GuildInven_ItemSlot12		4318 
#define WID_GuildInven_ItemSlot13		4319 
#define WID_GuildInven_ItemSlot14		4320 
#define WID_GuildInven_ItemSlot15		4321 
#define WID_GuildInven_ItemSlot16		4322 
#define WID_GuildInven_ItemSlot17		4323 
#define WID_GuildInven_ItemSlot18		4324 
#define WID_GuildInven_ItemSlot19		4325 
#define WID_GuildInven_ItemSlot20		4326 
#define WID_GuildInven_ItemSlot21		4327 
#define WID_GuildInven_ItemSlot22		4328 
#define WID_GuildInven_ItemSlot23		4329 
#define WID_GuildInven_ItemSlot24		4330 

///---------------------------------------------------------------------------
///-- 아이템 강화 창
///---------------------------------------------------------------------------
#define WID_SUB_ENCHANT					5000
#define WID_SUB_ENCHANT_BTN_OK			5001
#define WID_SUB_ENCHANT_BTN_ABT			5002
#define WID_SUB_ENCHANT_SLOT_ITEM		5003
#define WID_SUB_ENCHANT_SLOT_ELDA		5004
#define WID_SUB_ENCHANT_BTN_CLOSE		5005
#define WID_SUB_ENCHANT_TEXT_LEVEL		5006
#define WID_SUB_ENCHANT_TEXT1			5007
#define WID_SUB_ENCHANT_TEXT2			5008		
#define WID_SUB_ENCHANT_TEXT3			5009

#define WID_SUB_ENCHANT_RESULT			5010
#define WID_SUB_ENCHANT_RESULT_BTN_OK	5011			
#define WID_SUB_ENCHANT_RESULT_SLOT_R	5012
#define WID_SUB_ENCHANT_RESULT_CLOSE	5013
#define WID_SUB_ENCHANT_RESULT_TEXT1	5014
#define WID_SUB_ENCHANT_RESULT_TEXT2	5015
#define WID_SUB_ENCHANT_RESULT_TEXT3	5016
#define WID_SUB_ENCHANT_RESULT_TEXT4	5017
#define WID_SUB_ENCHANT_RESULT_IMG_X	5018
#define WID_SUB_ENCHANT_RESULT_IMG_SUCC	5019
#define WID_SUB_ENCHANT_RESULT_IMG_FAIL	5020


///---------------------------------------------------------------------------
///-- 엘다스톤 추출 창
///---------------------------------------------------------------------------
#define WID_SUB_EXTRACT					5021
#define WID_SUB_EXTRACT_BT_EXT			5022
#define WID_SUB_EXTRACT_BT_ABT			5023
#define WID_SUB_EXTRACT_SLOT_ITEM		5024		
#define WID_SUB_EXTRACT_SLOT_TARGET		5025
#define WID_SUB_EXTRACT_BT_CLOSE		5026
#define WID_SUB_EXTRACT_TEXT1			5027

#define WID_SUB_EXTRACT_RESULT			5030
#define WID_SUB_EXTRACT_RESULT_BT_OK	5031
#define WID_SUB_EXTRACT_RESULT_SLOT		5032
#define WID_SUB_EXTRACT_RESULT_BT_CLOSE	5033
#define WID_SUB_EXTRACT_RESULT_TEXT1	5034

///---------------------------------------------------------------------------
///-- 가공 창
///---------------------------------------------------------------------------
#define WID_SUB_REMODEL					5040
#define WID_SUB_REMODEL_BT_REM			5041
#define WID_SUB_REMODEL_BT_ABT			5042
#define WID_SUB_REMODEL_SLOT_ORIN		5043
#define WID_SUB_REMODEL_SLOT_OP1		5044	
#define WID_SUB_REMODEL_SLOT_OP2		5045
#define WID_SUB_REMODEL_SLOT_OP3		5046
#define WID_SUB_REMODEL_BT_CLOSE		5047
#define WID_SUB_REMODEL_TEXT1			5048
#define WID_SUB_REMODEL_TEXT2			5049

#define WID_SUB_REMODEL_SUC				5061
#define WID_SUB_REMODEL_SUC_BT_OK		5062
#define WID_SUB_REMODEL_SUC_SLOT		5063
#define WID_SUB_REMODEL_SUC_BT_CLOSE	5064	
#define WID_SUB_REMODEL_SUC_TEXT1		5065

#define WID_SUB_REMODEL_FA				5070
#define WID_SUB_REMODEL_FA_BT_OK		5071
#define WID_SUB_REMODEL_FA_SLOT_1		5072
#define WID_SUB_REMODEL_FA_SLOT_2		5073
#define WID_SUB_REMODEL_FA_SLOT_3		5074
#define WID_SUB_REMODEL_FA_BT_CLOSE		5075
#define WID_SUB_REMODEL_FA_TEXT1		5076
#define WID_SUB_REMODEL_FA_TEXT2		5077
#define WID_SUB_REMODEL_FA_TEXT3		5078
#define WID_SUB_REMODEL_FA_IMG_X1		5079
#define WID_SUB_REMODEL_FA_IMG_X2		5080
#define WID_SUB_REMODEL_FA_IMG_X3		5081

//-----------------------------------------------------------------------
//UI메시지 창
//-----------------------------------------------------------------------
#define WID_UIMsgWndAdd					5290
#define WID_UIMsgOkBtnAdd				5291
#define WID_UIMsgCancleBtnAdd			5292
#define WID_UIMsgLongInAddEdit			5293
#define WID_UIMsgLongInAddEdit2			5294

#define WID_UIMsgWnd					5300
#define WID_UIMsgTitleImg				5301

#define WID_UIMsgOkBtn					5302
#define WID_UIMsgCancleBtn				5303

#define WID_UIMsgInEdit					5304
#define WID_UIMsgNotifyText1			5305
#define	WID_UIMsgNotifyText2			5306
#define	WID_UIMsgNotifyText3			5307

//-----------------------------------------------------------------------
//파티 정보창
//----------------------------------------------------------------------
#define	WID_PartyInforWnd				5308
#define	WID_PartyBreakBtn				5309
#define WID_PartyMinBtn					5310
#define WID_PartyName					5311
#define WID_PartyLevel					5312
#define WID_PartyGetItemFreeBtn			5313
#define WID_PartyGetItemSequenceBtn		5314
#define WID_PartyGetItemCountryBtn		5315
#define	WID_PartyTagetBtn				5316

//-----------------------------------------------------------------------
//파티 멤버창
//-----------------------------------------------------------------------
#define WID_PartyMemberWnd1				5336
#define WID_PartyMemberWnd2				5337
#define WID_PartyMemberWnd3				5338
#define WID_PartyMemberWnd4				5339
#define WID_PartyMemberWnd5				5340
#define WID_PartyMemberWnd6				5341

#define WID_PartyMemCharImgBtn1			5342
#define WID_PartyMemCharImgBtn2			5343
#define WID_PartyMemCharImgBtn3			5344
#define WID_PartyMemCharImgBtn4			5345
#define WID_PartyMemCharImgBtn5			5346
#define WID_PartyMemCharImgBtn6			5347

#define WID_PartyMemLevel1				5348
#define WID_PartyMemLevel2				5349
#define WID_PartyMemLevel3				5350
#define WID_PartyMemLevel4				5351
#define WID_PartyMemLevel5				5352
#define WID_PartyMemLevel6				5353

#define WID_PartyMemGoBtn1				5354
#define WID_PartyMemGoBtn2				5355
#define WID_PartyMemGoBtn3				5356
#define WID_PartyMemGoBtn4				5357
#define WID_PartyMemGoBtn5				5358
#define WID_PartyMemGoBtn6				5359

#define WID_PartyMemHPGauge1			5360
#define WID_PartyMemHPGauge2			5361
#define WID_PartyMemHPGauge3			5362
#define WID_PartyMemHPGauge4			5363
#define WID_PartyMemHPGauge5			5364
#define WID_PartyMemHPGauge6			5365

#define WID_PartyMemSPMPGauge1			5366
#define WID_PartyMemSPMPGauge2			5367
#define WID_PartyMemSPMPGauge3			5368
#define WID_PartyMemSPMPGauge4			5369
#define WID_PartyMemSPMPGauge5			5370
#define WID_PartyMemSPMPGauge6			5371

#define WID_PartyMemComImg11			5372
#define WID_PartyMemComImg12			5373
#define WID_PartyMemComImg13			5374
#define WID_PartyMemComImg14			5375
#define WID_PartyMemComImg15			5376
#define WID_PartyMemComImg16			5377

#define WID_PartyMemComImg21			5378
#define WID_PartyMemComImg22			5379
#define WID_PartyMemComImg23			5380
#define WID_PartyMemComImg24			5381
#define WID_PartyMemComImg25			5382
#define WID_PartyMemComImg26			5383

#define WID_PartyMemComImg31			5384
#define WID_PartyMemComImg32			5385
#define WID_PartyMemComImg33			5386
#define WID_PartyMemComImg34			5387
#define WID_PartyMemComImg35			5388
#define WID_PartyMemComImg36			5389

#define WID_PartyMemComImg41			5390
#define WID_PartyMemComImg42			5391
#define WID_PartyMemComImg43			5392
#define WID_PartyMemComImg44			5393
#define WID_PartyMemComImg45			5394
#define WID_PartyMemComImg46			5395

#define WID_PartyMemComImg51			5396
#define WID_PartyMemComImg52			5397
#define WID_PartyMemComImg53			5398
#define WID_PartyMemComImg54			5399
#define WID_PartyMemComImg55			5400
#define WID_PartyMemComImg56			5401

#define WID_PartyMemMasterImg1			5402
#define WID_PartyMemMasterImg2			5403
#define WID_PartyMemMasterImg3			5404
#define WID_PartyMemMasterImg4			5405
#define WID_PartyMemMasterImg5			5406
#define WID_PartyMemMasterImg6			5407

#define WID_PartyMemMyselfImg1			5408
#define WID_PartyMemMyselfImg2			5409
#define WID_PartyMemMyselfImg3			5410
#define WID_PartyMemMyselfImg4			5411
#define WID_PartyMemMyselfImg5			5412
#define WID_PartyMemMyselfImg6			5413

//파티 최소화 프레임 윈도우
#define WID_Party_Min_Wnd				5414
#define	WID_PartyMBreakBtn				5415
#define WID_PartyMaxBtn					5416

//파티 팝업 윈도우
#define WID_Party_PopupWnd				5417
#define WID_Party_PopupName				5418
#define WID_Party_PopupTextBtn1			5419
#define WID_Party_PopupTextBtn2			5420
#define WID_Party_PopupTextBtn3			5421
#define WID_Party_PopupTextBtn4			5422


///---------------------------------------------------------------------------
///-- 수리창
#define WID_REPAIR_WND					5500

#define WID_REPAIR_BT_CLOSE				5501
#define WID_REPAIR_BT_ALLITEM			5502
#define WID_REPAIR_BT_REPAIR			5503
#define WID_REPAIR_BT_TEXT				5504
#define WID_REPAIR_BT_SCROLL			5505

#define WID_REPAIR_ItemSlot0				5510
#define WID_REPAIR_ItemSlot1				5511
#define WID_REPAIR_ItemSlot2				5512
#define WID_REPAIR_ItemSlot3				5513
#define WID_REPAIR_ItemSlot4				5514	
#define WID_REPAIR_ItemSlot5				5515
#define WID_REPAIR_ItemSlot6				5516
#define WID_REPAIR_ItemSlot7				5517
#define WID_REPAIR_ItemSlot8				5518
#define WID_REPAIR_ItemSlot9				5519

#define WID_REPAIR_ItemSlot10			5520
#define WID_REPAIR_ItemSlot11			5521
#define WID_REPAIR_ItemSlot12			5522
#define WID_REPAIR_ItemSlot13			5523
#define WID_REPAIR_ItemSlot14			5524

///---------------------------------------------------------------------------
///-- 아이템 제작 Wnd
#define WID_ITEMCREATE_WND				5600
#define WID_ITEMCREATE_BT_X				5601
#define WID_ITEMCREATE_BT_TYPE			5602	
#define WID_ITEMCREATE_BT_CREATE		5603
#define WID_ITEMCREATE_BT_CANCEL		5604	
#define WID_ITEMCREATE_SLOT_ELDA		5605
#define WID_ITEMCREATE_SLOT_1			5606
#define WID_ITEMCREATE_SLOT_2			5607
#define WID_ITEMCREATE_SLOT_3			5608
#define WID_ITEMCREATE_TEXT_TYPE		5609
#define WID_ITEMCREATE_SLOT_TARGET		5610
#define WID_ITEMCREATE_IMG_TYPE			5611

///-- 아이템 제작 결과 Wnd
#define WID_ITEMCREATES_WND			5620
#define WID_ITEMCREATES_BT_OK		5621
#define WID_ITEMCREATES_BT_X		5622
#define WID_ITEMCREATES_TEXT		5623
#define WID_ITEMCREATES_SLOT		5624

#define WID_ITEMCREATEF_WND			5630
#define WID_ITEMCREATEF_BT_OK		5631
#define WID_ITEMCREATEF_BT_X		5632
#define WID_ITEMCREATEF_TEXT		5633
#define WID_ITEMCREATEF_SLOT1		5634
#define WID_ITEMCREATEF_SLOT2		5635
#define WID_ITEMCREATEF_SLOT3		5636
#define WID_ITEMCREATEF_SLOT4		5637
#define WID_ITEMCREATEF_ICON_1		5638
#define WID_ITEMCREATEF_ICON_2		5639
#define WID_ITEMCREATEF_ICON_3		5640
#define WID_ITEMCREATEF_ICON_4		5641

//------------------------------------------------------------------------------
// 유료인벤확장 
//------------------------------------------------------------------------------
#define WID_PayInventory_Wnd			5525	

#define WID_PayInven_CloseBtn			5526		

#define WID_PayInven_Box1StateBtn		5527		
#define WID_PayInven_Box2StateBtn		5528		
#define WID_PayInven_Box3StateBtn		5529		
#define WID_PayInven_Box4StateBtn		5530		
#define WID_PayInven_Box5StateBtn		5531

#define WID_PayInven_ItemSlot0			5532 
#define WID_PayInven_ItemSlot1			5533 
#define WID_PayInven_ItemSlot2			5534 
#define WID_PayInven_ItemSlot3			5535 
#define WID_PayInven_ItemSlot4			5536 
#define WID_PayInven_ItemSlot5			5537 
#define WID_PayInven_ItemSlot6			5538 
#define WID_PayInven_ItemSlot7			5539 
#define WID_PayInven_ItemSlot8			5540 
#define WID_PayInven_ItemSlot9			5541 
#define WID_PayInven_ItemSlot10	  	    5542 
#define WID_PayInven_ItemSlot11			5543 
#define WID_PayInven_ItemSlot12			5544 
#define WID_PayInven_ItemSlot13			5545 
#define WID_PayInven_ItemSlot14			5546 
#define WID_PayInven_ItemSlot15			5547 
#define WID_PayInven_ItemSlot16			5548 
#define WID_PayInven_ItemSlot17			5549 
#define WID_PayInven_ItemSlot18			5550 
#define WID_PayInven_ItemSlot19			5551 
#define WID_PayInven_ItemSlot20			5552 
#define WID_PayInven_ItemSlot21			5553 
#define WID_PayInven_ItemSlot22			5554 
#define WID_PayInven_ItemSlot23			5555 
#define WID_PayInven_ItemSlot24			5556 


//커뮤니티 윈도우--------------------------------------------------------------
#define WID_MCommunityBgUI				5700
#define WID_MStateTitleImg				5701
#define WID_MFriendTabActBtn			5702
#define	WID_MGuildTabActBtn				5703
#define WID_MBlockTabActBtn				5704
#define WID_MSettingTabActBtn			5705
#define WID_MScrollBar					5706
#define WID_MCloseBtn					5707
#define WID_MSubBtnBgImg				5708
#define WID_MAddBtn						5709
#define WID_MSubBtn						5710


//커뮤니티 윈도우 - 친구탭
#define	WID_MFriendTabWnd				5715
#define	WID_MFNameFieldBtn				5716
#define WID_MFAreaFieldBtn				5717
#define WID_MFChanelFieldBtn			5718
#define WID_MFLevelFieldBtn				5719
#define WID_MFJobFieldBtn				5720

#define WID_MFriendSelBtn1				5721
#define WID_MFriendSelBtn2				5722
#define WID_MFriendSelBtn3				5723
#define WID_MFriendSelBtn4				5724
#define WID_MFriendSelBtn5				5725
#define WID_MFriendSelBtn6				5726

//커뮤니티 윈도우 - 길드탭
#define WID_MGuildTabWnd				5727
#define WID_MGTextBox1					5728
#define WID_MGTextBox2					5729

//커뮤니티 윈도우 - 차단탭
#define WID_MBlockTabWnd				5730
#define WID_MBNameFieldTxt1				5731
#define WID_MBNameFieldTxt2				5732

#define WID_MBlockSelBtn1				5733
#define WID_MBlockSelBtn2				5734
#define WID_MBlockSelBtn3				5735
#define WID_MBlockSelBtn4				5736
#define WID_MBlockSelBtn5				5737
#define WID_MBlockSelBtn6				5738
#define WID_MBlockSelBtn7				5739
#define WID_MBlockSelBtn8				5740
#define WID_MBlockSelBtn9				5741
#define WID_MBlockSelBtn10				5742
#define WID_MBlockSelBtn11				5743
#define WID_MBlockSelBtn12				5744
#define WID_MBlockSelBtn13				5745
#define WID_MBlockSelBtn14				5746
#define WID_MBlockSelBtn15				5747
#define WID_MBlockSelBtn16				5748
#define WID_MBlockSelBtn17				5749
#define WID_MBlockSelBtn18				5750
#define WID_MBlockSelBtn19				5751
#define WID_MBlockSelBtn20				5752

//커뮤니티 윈도우 - 설정탭
#define WID_MSettingTabWnd				5757
#define WID_MSOpenSettingTxt			5758
#define WID_MSLocalTxt					5759
#define WID_MSLocalCheckBox				5760
#define WID_MSLevelTxt					5761
#define WID_MSLevelCheckBox				5762
#define WID_MSJobTxt					5763
#define WID_MSJobCheckBox				5764
#define WID_MSStateSettingTxt			5765
#define WID_MSOnlineTxt					5766
#define WID_MSOnlineRadioBtn			5767
#define WID_MSOfflineTxt				5768
#define WID_MSOfflineRadioBtn			5769
#define WID_MSRestTxt					5770
#define WID_MSRestRadioBtn				5771
#define WID_MSMeatTxt					5772
#define WID_MSMeatRadioBtn				5773


#define WID_GuildAWordTxt				5774
#define WID_GuildAWordInputBtn			5775
#define WID_GuildAWordSaveBtn			5776
#define WID_GuildAWordTxtBox			5777
#define WID_MSSetCompleteBtn			5778


//커뮤니티 윈도우 - 확인메시지윈도우
#define WID_MMsgWnd						5779
#define WID_MMsgOkBtn					5780
#define WID_MMsgCancleBtn				5781
#define WID_MMsgInEdit					5782

//커뮤니티 윈도우 - 팝업창 윈도우
#define WID_MPopupWnd					5783
#define WID_MPopupBtn1					5784
#define WID_MPopupBtn2					5785
#define WID_MPopupBtn3					5786
#define WID_MPopupBtn4					5787

//메신저 쪽지 윈도우
#define WID_MessenMemoWnd				5788
#define WID_MMCloseBtn					5789
#define WID_MMTitleImgBox				5790
#define WID_MMTransStateTxt				5791
#define WID_MMInterDestTxt				5792
#define WID_MMInMLineEdit				5793
#define WID_MMOutputTxt					5794
#define WID_MMScrollHideImgBox			5795			
#define WID_MMConfirmBtn				5796

#define WID_OptWnd						5797
#define WID_OptWnd_ShdowOffBtn			5798
#define WID_OptWnd_ShdowLowBtn			5799
#define WID_OptWnd_ShdowHighBtn			5800
#define WID_OptWnd_GlowSlideBar			5801
#define WID_OptWnd_ObjViewSlideBar		5802
#define WID_OptWnd_FarLandOffBtn		5803
#define WID_OptWnd_FarLandOnBtn			5804
#define WID_OptWnd_EffectSoundSlideBar	5805
#define WID_OptWnd_BGMSoundSlideBar		5806
#define WID_OptWnd_OKBtn				5807
#define WID_OptWnd_CancelBtn			5808


//채팅창
#define WID_ChatActFrame				5900
#define WID_ChatFilterTabFrame			5901
#define WID_ChatPrintFrame				5902
#define WID_ChatInputFrame				5903
#define WID_ChatStateMenuFrame			5904
#define WID_ChatNonActFrame				5905
#define WID_ChatIDPopupFrame			5907

//시스템 출력창
#define WID_SystemPrintFrame			5906
#define WID_AddEffectMsgFrame			5908
#define WID_ITEMCREATE_WND_F		6000

//==============================================================================
// PVP 배틀 존 
//==============================================================================
// 공식전, 비공식전 		
#define WID_BZ_SELECTZONE_WND				6100 
#define WID_BZ_SELECTZONE_CLOSE_BTN			6101 
#define WID_BZ_SELECTZONE_NATION_BTN		6102 
#define WID_BZ_SELECTZONE_FREE_BTN			6103 
#define WID_BZ_SELECTZONE_BTNFOCUS_IMG		6104 

#define WID_BZ_SELECTGRADE_WND				6110 
#define WID_BZ_SELECTGRADE_CLOSE_BTN		6111
#define WID_BZ_SELECTGRADE_21_30_BTN		6112 
#define WID_BZ_SELECTGRADE_31_40_BTN		6113 
#define WID_BZ_SELECTGRADE_41_50_BTN		6114 
#define WID_BZ_SELECTGRADE_51_65_BTN		6115 
#define WID_BZ_SELECTGRADE_66_80_BTN		6116 
#define WID_BZ_SELECTGRADE_81_100_BTN		6117 
#define WID_BZ_SELECTGRADE_21_30_PGBAR		6118 
#define WID_BZ_SELECTGRADE_31_40_PGBAR		6119 
#define WID_BZ_SELECTGRADE_41_50_PGBAR		6120 
#define WID_BZ_SELECTGRADE_51_65_PGBAR		6121 
#define WID_BZ_SELECTGRADE_66_80_PGBAR		6122 
#define WID_BZ_SELECTGRADE_81_100_PGBAR		6123 
#define WID_BZ_ROOMLIST_WND					6124 
#define WID_BZ_CREATEROOM_WND					6125 
#define WID_BZ_WAITINGROOM_WND					6126 
#define WID_BZ_GAMERESULT_WND					6127
//==============================================================================

#define WID_PAYITEM_WND					6140

//==============================================================================
// 프론티어 관련 WID
//==============================================================================
#define WID_FRONTIERMAIN_WND					6150 

#define WID_CREATURE_INFO_WND                   6151
#define WID_MEMBER_INFO_WND                     6152
#define WID_NOTICE_INFO_WND                     6153

#define WID_FRONTIER_BTN_WND                    6154
#define WID_PROPOSE_FRONTIER_WND                6155
#define WID_PROPOSE_FRONTIER_ELDTEXT	        6156		
#define WID_PROPOSE_FRONTIER_ELDBTN		        6157
//////////////////////////////////////////////////////////////////////////
//펫 관련 WID 
//////////////////////////////////////////////////////////////////////////

#define WID_PETINFO_WND				6200
#define WID_PETMAIN_WND				6201
#define WID_PETCREATE_WND		6202
#define WID_PET_CALLOFF_WND		6203

//////////////////////////////////////////////////////////////////////////





//말탭 
#define WID_Inven_HorseSBtn			6250


#define WID_ACCESSORY_ENCHANT			6280
#define WID_ACCESSOR_ENCHANT_RESULT		6281

#define WID_TOTEMSCORE_WND				6290
#define	WID_FAME_INDUNTIME_WND			6291

//확장 퀵슬롯 
#define WID_MainMenu_EQSlot0				6300
#define WID_MainMenu_EQSlot1				6301
#define WID_MainMenu_EQSlot2				6302
#define WID_MainMenu_EQSlot3				6303
#define WID_MainMenu_EQSlot4				6304
#define WID_MainMenu_EQSlot5				6305
#define WID_MainMenu_EQSlot6				6306
#define WID_MainMenu_EQSlot7				6307
#define WID_MainMenu_EQSlot8				6308
#define WID_MainMenu_EQSlot9				6309
#define WID_MainMenu_EQSlot10				6310
#define WID_MainMenu_EQSlot11				6311
#define WID_MainMenu_EQSlot12				6312
#define WID_MainMenu_EQSlot13				6313
#define WID_MainMenu_EQSlot14				6314
#define WID_MainMenu_EQSlot15				6315
#define WID_MainMenu_EQSlot16				6316
#define WID_MainMenu_EQSlot17				6317
#define WID_MainMenu_EQSlot18				6318
#define WID_MainMenu_EQSlot19				6319

#define WID_FrontierCreateScroll            6320



// 현재 6250 까지 사용 6251 부터 사용 
///-- WID_None 일 경우 WID 는 MAX 에서 1씩 감소되며 자동 설정 됩니다.
///-- 자동 WID 와 #define 된 WID 의 충돌을 방지하기 위해 AUTOLIMIT 를 둡니다.
///-- WID 의 최고 값 +1 로 (혹은 그 이상으로) 항상 설정해 주시기 바랍니다.
#define WID_AUTO_LIMIT					8900




//----------------------------------------------------------------------
#endif  __DEFINE_WID_H__