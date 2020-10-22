#ifndef __DEFINE_TID_H__
#define __DEFINE_TID_H__

#define	MAX_TEX_AMOUNT	5000

#define	MAX_IMG_AMOUNT	310

#define		TID_None	0
#define		TID_CLIENT	1
#define		TID_SlotHighLight	2
#define		TID_LoadingClient	3
#define		TID_LoadingBar	4
#define		TID_LOGIN_BACK	5
#define		TID_LOADING_WALL_M0		6
#define		TID_LOADING_WALL_R0		7
#define		TID_SPIcon_None	8
#define		TID_SPIcon_Sword	9
#define		TID_SPIcon_Tsword	10
#define		TID_SPIcon_Bow	11
#define		TID_SPIcon_FLMagic	12
#define		TID_SPIcon_WIMagic	13
#define		TID_SPIcon_ENMagic	14
#define		TID_SPIcon_HMagic	15
#define		TID_SPIcon_Crystal	16
#define		TID_SPIcon_Shield	17
#define		TID_SIcon_BackStap	18
#define		TID_SIcon_CircleSlash	19
#define		TID_SIcon_DefenseMotion	20
#define		TID_SIcon_FocusSlash	21
#define		TID_SIcon_FortitudeWill	22
#define		TID_SIcon_GlareSlash	23
#define		TID_SIcon_MtStrike	24
#define		TID_SIcon_Parry	25
#define		TID_SIcon_PathFind	26
#define		TID_SIcon_ShieldBash	27
#define		TID_SIcon_Sprint	28
#define		TID_SIcon_Stamp	29
#define		TID_SIcon_Sword_Wield	30
#define		TID_SIcon_Vanish	31
#define		TID_SIcon_WeaponBreak	32
//#define		TID_TSwordSkill	33 ///-- Not referenced in Source 
//#define		TID_FireBall	34 ///-- Not referenced in Source 
//#define		TID_IceSpear	35 ///-- Not referenced in Source 
//#define		TID_RockPress	36 ///-- Not referenced in Source 
//#define		TID_Lighting	37 ///-- Not referenced in Source 
//#define		TID_BubbleCrash	38 ///-- Not referenced in Source 
//#define		TID_WoodenSword	39 ///-- Not referenced in Source 
//#define		TID_Meditation	40 ///-- Not referenced in Source 
//#define		TID_Null2	41 ///-- Not referenced in Source 
//#define		TID_Null3	42 ///-- Not referenced in Source 
//#define		TID_RockShield	43 ///-- Not referenced in Source 
//#define		TID_BlessofRain	44 ///-- Not referenced in Source 
//#define		TID_Proima	45 ///-- Not referenced in Source 
//#define		TID_MagicBook	46 ///-- Not referenced in Source 
//#define		TID_MagicCloth	47 ///-- Not referenced in Source 
//#define		TID_MagicPotion	48 ///-- Not referenced in Source 
//#define		TID_Normal_Attack_M	49 ///-- Not referenced in Source 
//#define		TID_Normal_Attack_R	50 ///-- Not referenced in Source 
//#define		TID_HealArrow	51 ///-- Not referenced in Source 
//#define		TID_Counter	52 ///-- Not referenced in Source 
//#define		TID_NoryoArrow	53 ///-- Not referenced in Source 
//#define		TID_FootArrow	54 ///-- Not referenced in Source 
//#define		TID_Bow	55 ///-- Not referenced in Source 
//#define		TID_ArcherMind	56 ///-- Not referenced in Source 
//#define		TID_Focusing	57 ///-- Not referenced in Source 
//#define		TID_ShamanCloth	58 ///-- Not referenced in Source 
//#define		TID_Crystal	59 ///-- Not referenced in Source 
//#define		TID_FireArmor	60 ///-- Not referenced in Source 
//#define		TID_IceField	61 ///-- Not referenced in Source 
//#define		TID_DamonFoot	62 ///-- Not referenced in Source 
//#define		TID_Sync	63 ///-- Not referenced in Source 
//#define		TID_Possesion	64 ///-- Not referenced in Source 
//#define		TID_UnPosses	65 ///-- Not referenced in Source 
//#define		TID_FirePunch	66 ///-- Not referenced in Source 
//#define		TID_LightKick	67 ///-- Not referenced in Source 
//#define		TID_MDefend	68 ///-- Not referenced in Source 
//#define		TID_NoryoSlash	69 ///-- Not referenced in Source 
//#define		TID_MoaSlash	70 ///-- Not referenced in Source 
//#define		TID_GupsoSlash	71 ///-- Not referenced in Source 
//#define		TID_ArmCrash	72 ///-- Not referenced in Source 
//#define		TID_Signwave	73 ///-- Not referenced in Source 
//#define		TID_FastSword	74 ///-- Not referenced in Source 
//#define		TID_PowerSword	75 ///-- Not referenced in Source 
//#define		TID_Evasion	76 ///-- Not referenced in Source 
//#define		TID_Null1	77 ///-- Not referenced in Source 
//#define		TID_BlessofMillena	78 ///-- Not referenced in Source 
//#define		TID_FirstAid	79 ///-- Not referenced in Source 
//#define		TID_LightArmor	80 ///-- Not referenced in Source 
//#define		TID_Sword	81 ///-- Not referenced in Source 
//#define		TID_TwoHandSword	82 ///-- Not referenced in Source 
//#define		TID_SwordSkill	83 ///-- Not referenced in Source 
//#define		TID_TSwordSkill	84 ///-- Not referenced in Source 
//#define		TID_FireBall	85 ///-- Not referenced in Source 
//#define		TID_IceSpear	86 ///-- Not referenced in Source 
//#define		TID_RockPress	87 ///-- Not referenced in Source 
//#define		TID_Lighting	88 ///-- Not referenced in Source 
//#define		TID_BubbleCrash	89 ///-- Not referenced in Source 
//#define		TID_WoodenSword	90 ///-- Not referenced in Source 
//#define		TID_Meditation	91 ///-- Not referenced in Source 
//#define		TID_Null2	92 ///-- Not referenced in Source 
//#define		TID_Null3	93 ///-- Not referenced in Source 
//#define		TID_RockShield	94 ///-- Not referenced in Source 
//#define		TID_BlessofRain	95 ///-- Not referenced in Source 
//#define		TID_Proima	96 ///-- Not referenced in Source 
//#define		TID_MagicBook	97 ///-- Not referenced in Source 
//#define		TID_MagicCloth	98 ///-- Not referenced in Source 
//#define		TID_MagicPotion	99 ///-- Not referenced in Source 
//#define		TID_Normal_Attack_M	100 ///-- Not referenced in Source 
//#define		TID_Normal_Attack_R	101 ///-- Not referenced in Source 
//#define		TID_HealArrow	102 ///-- Not referenced in Source 
//#define		TID_Counter	103 ///-- Not referenced in Source 
//#define		TID_NoryoArrow	104 ///-- Not referenced in Source 
//#define		TID_FootArrow	105 ///-- Not referenced in Source 
//#define		TID_Bow	106 ///-- Not referenced in Source 
//#define		TID_ArcherMind	107 ///-- Not referenced in Source 
#define		TID_Focusing	108
//#define		TID_ShamanCloth	109 ///-- Not referenced in Source 
//#define		TID_Crystal	110 ///-- Not referenced in Source 
//#define		TID_FireArmor	111 ///-- Not referenced in Source 
//#define		TID_IceField	112 ///-- Not referenced in Source 
//#define		TID_DamonFoot	113 ///-- Not referenced in Source 
//#define		TID_Sync	114 ///-- Not referenced in Source 
//#define		TID_Possesion	115 ///-- Not referenced in Source 
//#define		TID_UnPosses	116 ///-- Not referenced in Source 
//#define		TID_FirePunch	117 ///-- Not referenced in Source 
//#define		TID_LightKick	118 ///-- Not referenced in Source 
//#define		TID_MDefend	119 ///-- Not referenced in Source 
#define		TID_ArmorDown_01_Millenas2_M	120
#define		TID_ArmorDown_01_Millenas_M	121
#define		TID_ArmorDown_01_Millenas2_F	122
#define		TID_ArmorDown_01_Millenas_F	123
#define		TID_ArmorDown_02_Twinible2_M	124
#define		TID_ArmorDown_02_Twinible_M	125
#define		TID_ArmorUp_01_Millenas2_M	126
#define		TID_ArmorUp_01_Millenas_M	127
#define		TID_ArmorUp_01_Millenas2_F	128
#define		TID_ArmorUp_01_Millenas_F	129
#define		TID_ArmorUp_02_Twinible2_M	130
#define		TID_ArmorUp_02_Twinible_M	131
#define		TID_ArmorDown_02_Twinible2_F	132
#define		TID_ArmorDown_02_Twinible_F	133
#define		TID_ArmorDown_03_Break2_M	134
#define		TID_ArmorDown_03_Break_M	135
#define		TID_ArmorDown_03_Break2_F	136
#define		TID_ArmorDown_03_Break_F	137
#define		TID_ArmorUp_02_Twinible2_F	138
#define		TID_ArmorUp_02_Twinible_F	139
#define		TID_ArmorUp_03_Break2_M	140
#define		TID_ArmorUp_03_Break_M	141
#define		TID_ArmorUp_03_Break2_F	142
#define		TID_ArmorUp_03_Break_F	143
#define		TID_ArmorDown_04_Tirus2_M	144
#define		TID_ArmorDown_04_Tirus_M	145
#define		TID_ArmorDown_04_Tirus2_F	146
#define		TID_ArmorDown_04_Tirus_F	147
#define		TID_Dummy1	148
#define		TID_Dummy2	149
#define		TID_ArmorUp_04_Tirus2_M	150
#define		TID_ArmorUp_04_Tirus_M	151
#define		TID_ArmorUp_04_Tirus2_F	152
#define		TID_ArmorUp_04_Tirus_F	153
#define		TID_Dummy3	154
#define		TID_Dummy4	155
//#define		TID_ArmorDown_01_Rainas2F	156 ///-- Not referenced in Source 
//#define		TID_ArmorDown_01_RainasF	157 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_Magician2M	158 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_MagicianM	159 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_Magician2F	160 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_MagicianF	161 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_Rainas2F	162 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_RainasF	163 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_Magician2M	164 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_MagicianM	165 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_Magician2F	166 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_MagicianF	167 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_Ilisia2M	168 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_IlisiaM	169 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_Ilisia2F	170 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_IlisiaF	171 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_Prominas2M	172 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_ProminasM	173 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_Ilisia2M	174 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_IlisiaM	175 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_Ilisia2F	176 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_IlisiaF	177 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_Prominas2M	178 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_ProminasM	179 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_Prominas2F	180 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_ProminasF	181 ///-- Not referenced in Source 
//#define		TID_ArmorDown_01_ShamanM	182 ///-- Not referenced in Source 
//#define		TID_ArmorDown_01_ShamanF	183 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_MedibleM	184 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_MedibleF	185 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_Prominas2F	186 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_ProminasF	187 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_ShamanM	188 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_ShamanF	189 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_MedibleM	190 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_MedibleF	191 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_TinpleM	192 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_TinpleF	193 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_SommonerM	194 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_SommonerF	195 ///-- Not referenced in Source 
//#define		TID_ArmorDown_01_PriestM	196 ///-- Not referenced in Source 
//#define		TID_ArmorDown_01_PriestF	197 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_TinpleM	198 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_TinpleF	199 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_SommonerM	200 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_SommonerF	201 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_PriestM	202 ///-- Not referenced in Source 
//#define		TID_ArmorUp_01_PriestF	203 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_GartM	204 ///-- Not referenced in Source 
//#define		TID_ArmorDown_02_GartF	205 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_BlackM	206 ///-- Not referenced in Source 
//#define		TID_ArmorDown_03_BlackF	207 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_AirM	208 ///-- Not referenced in Source 
//#define		TID_ArmorDown_04_AirF	209 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_GartM	210 ///-- Not referenced in Source 
//#define		TID_ArmorUp_02_GartF	211 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_BlackM	212 ///-- Not referenced in Source 
//#define		TID_ArmorUp_03_BlackF	213 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_AirM	214 ///-- Not referenced in Source 
//#define		TID_ArmorUp_04_AirF	215 ///-- Not referenced in Source 
#define		TID_Boots_01_Millenas2_M	216
#define		TID_Boots_01_Millenas_M	217
#define		TID_Boots_01_Millenas2_F	218
#define		TID_Boots_01_Millenas_F	219
#define		TID_Boots_02_Twinible2_M	220
#define		TID_Boots_02_Twinible_M	221
#define		TID_Boots_02_Twinible2_F	222
#define		TID_Boots_02_Twinible_F	223
#define		TID_Boots_03_Break2_M	224
#define		TID_Boots_03_Break_M	225
#define		TID_Boots_03_Break2_F	226
#define		TID_Boots_03_Break_F	227
#define		TID_Boots_04_Tirus2_M	228
#define		TID_Boots_04_Tirus_M	229
#define		TID_Boots_04_Tirus2_F	230
#define		TID_Boots_04_Tirus_F	231
//#define		TID_Boots_01_Rainas2M	232 ///-- Not referenced in Source 
//#define		TID_Boots_01_RainasM	233 ///-- Not referenced in Source 
//#define		TID_Boots_01_Rainas2F	234 ///-- Not referenced in Source 
//#define		TID_Boots_01_RainasF	235 ///-- Not referenced in Source 
//#define		TID_Boots_02_Magician2M	236 ///-- Not referenced in Source 
//#define		TID_Boots_02_MagicianM	237 ///-- Not referenced in Source 
//#define		TID_Boots_02_Magician2F	238 ///-- Not referenced in Source 
//#define		TID_Boots_02_MagicianF	239 ///-- Not referenced in Source 
//#define		TID_Boots_03_Ilisia2M	240 ///-- Not referenced in Source 
//#define		TID_Boots_03_IlisiaM	241 ///-- Not referenced in Source 
//#define		TID_Boots_03_Ilisia2F	242 ///-- Not referenced in Source 
//#define		TID_Boots_03_IlisiaF	243 ///-- Not referenced in Source 
//#define		TID_Boots_04_Prominas2M	244 ///-- Not referenced in Source 
//#define		TID_Boots_04_ProminasM	245 ///-- Not referenced in Source 
//#define		TID_Boots_04_Prominas2F	246 ///-- Not referenced in Source 
//#define		TID_Boots_04_ProminasF	247 ///-- Not referenced in Source 
//#define		TID_Boots_01_ShamanM	248 ///-- Not referenced in Source 
//#define		TID_Boots_01_ShamanF	249 ///-- Not referenced in Source 
//#define		TID_Boots_02_MedibleM	250 ///-- Not referenced in Source 
//#define		TID_Boots_02_MedibleF	251 ///-- Not referenced in Source 
//#define		TID_Boots_03_TinpleM	252 ///-- Not referenced in Source 
//#define		TID_Boots_03_TinpleF	253 ///-- Not referenced in Source 
//#define		TID_Boots_04_SummonerM	254 ///-- Not referenced in Source 
//#define		TID_Boots_04_SummonerF	255 ///-- Not referenced in Source 
//#define		TID_Boots_01_PriestM	256 ///-- Not referenced in Source 
//#define		TID_Boots_01_PriestF	257 ///-- Not referenced in Source 
//#define		TID_Boots_02_GartM	258 ///-- Not referenced in Source 
//#define		TID_Boots_02_GartF	259 ///-- Not referenced in Source 
//#define		TID_Boots_03_BlackM	260 ///-- Not referenced in Source 
//#define		TID_Boots_03_BlackF	261 ///-- Not referenced in Source 
//#define		TID_Boots_04_AirM	262 ///-- Not referenced in Source 
//#define		TID_Boots_04_AirF	263 ///-- Not referenced in Source 
#define		TID_Glove_01_Millenas2_M	264
#define		TID_Glove_01_Millenas_M	265
#define		TID_Glove_01_Millenas2_F	266
#define		TID_Glove_01_Millenas_F	267
#define		TID_Glove_02_Twinible2_M	268
#define		TID_Glove_02_Twinible_M	269
#define		TID_Glove_02_Twinible2_F	270
#define		TID_Glove_02_Twinible_F	271
#define		TID_Glove_03_Break2_M	272
#define		TID_Glove_03_Break_M	273
#define		TID_Glove_03_Break2_F	274
#define		TID_Glove_03_Break_F	275
#define		TID_Glove_04_Tirus2_M	276
#define		TID_Glove_04_Tirus_M	277
#define		TID_Glove_04_Tirus2_F	278
#define		TID_Glove_04_Tirus_F	279
//#define		TID_Glove_01_Rainas2M	280 ///-- Not referenced in Source 
//#define		TID_Glove_01_RainasM	281 ///-- Not referenced in Source 
//#define		TID_Glove_01_Rainas2F	282 ///-- Not referenced in Source 
//#define		TID_Glove_01_RainasF	283 ///-- Not referenced in Source 
//#define		TID_Glove_02_Magician2M	284 ///-- Not referenced in Source 
//#define		TID_Glove_02_MagicianM	285 ///-- Not referenced in Source 
//#define		TID_Glove_02_Magician2F	286 ///-- Not referenced in Source 
//#define		TID_Glove_02_MagicianF	287 ///-- Not referenced in Source 
//#define		TID_Glove_03_Ilisia2M	288 ///-- Not referenced in Source 
//#define		TID_Glove_03_IlisiaM	289 ///-- Not referenced in Source 
//#define		TID_Glove_03_Ilisia2F	290 ///-- Not referenced in Source 
//#define		TID_Glove_03_IlisiaF	291 ///-- Not referenced in Source 
//#define		TID_Glove_04_Prominas2M	292 ///-- Not referenced in Source 
//#define		TID_Glove_04_ProminasM	293 ///-- Not referenced in Source 
//#define		TID_Glove_04_Prominas2F	294 ///-- Not referenced in Source 
//#define		TID_Glove_04_ProminasF	295 ///-- Not referenced in Source 
//#define		TID_Glove_01_ShamanM	296 ///-- Not referenced in Source 
//#define		TID_Glove_01_ShamanF	297 ///-- Not referenced in Source 
//#define		TID_Glove_02_MedibleM	298 ///-- Not referenced in Source 
//#define		TID_Glove_02_MedibleF	299 ///-- Not referenced in Source 
//#define		TID_Glove_03_TinpleM	300 ///-- Not referenced in Source 
//#define		TID_Glove_03_TinpleF	301 ///-- Not referenced in Source 
//#define		TID_Glove_04_SummonerM	302 ///-- Not referenced in Source 
//#define		TID_Glove_04_SummonerF	303 ///-- Not referenced in Source 
//#define		TID_Glove_01_PriestM	304 ///-- Not referenced in Source 
//#define		TID_Glove_01_PriestF	305 ///-- Not referenced in Source 
//#define		TID_Glove_02_GartM	306 ///-- Not referenced in Source 
//#define		TID_Glove_02_GartF	307 ///-- Not referenced in Source 
//#define		TID_Glove_03_BlackM	308 ///-- Not referenced in Source 
//#define		TID_Glove_03_BlackF	309 ///-- Not referenced in Source 
//#define		TID_Glove_04_AirM	310 ///-- Not referenced in Source 
//#define		TID_Glove_04_AirF	311 ///-- Not referenced in Source 
#define		TID_Sword_00_Wonder	312
#define		TID_Sword_01_Millena	313
#define		TID_Sword_02_Gladius	314
#define		TID_Sword_03_Wonder_Edge	315
#define		TID_Sword_04_Pioneer	316
#define		TID_Sword_05_Twinible	317
#define		TID_Sword_06_Blitz	318
#define		TID_Sword_07_Still_Break	319
#define		TID_Sword_08_Brave	320
#define		TID_Sword_09_Elven	321
#define		TID_Sword_10_Ramu	322
#define		TID_Sword_11_Rose	323
#define		TID_Sword_12_Soul_Traker	324
#define		TID_TSword_01_Millenas	325
#define		TID_TSword_02_Craymore	326
#define		TID_TSword_03_Tworandal	327
#define		TID_TSword_04_Zweihunder	328
#define		TID_TSword_05_Twinible_Two	329
#define		TID_TSword_06_BlitzLord	330
#define		TID_TSword_07_Core_Breaker	331
#define		TID_TSword_08_Ramu_Two	332
#define		TID_TSword_09_Elven_Drake	333
#define		TID_TSword_10_ElFarseer	334
#define		TID_TSword_11_Fury_Fire	335
#define		TID_TSword_12_Abyss	336
#define		TID_Shield_01_Twinible	337
#define		TID_Shield_02_Blitz	338
#define		TID_Shield_03_Cubric	339
#define		TID_Shield_04_Mighty	340
#define		TID_Shield_05_Elven	341
#define		TID_Shield_06_Cubric_Silver	342
#define		TID_Shield_07_Spell	343
#define		TID_Shield_08_Royal	344
//#define		TID_Staff_01_Rain	345 ///-- Not referenced in Source 
//#define		TID_Staff_02_Red	346 ///-- Not referenced in Source 
//#define		TID_Staff_03_Black	347 ///-- Not referenced in Source 
//#define		TID_Staff_04_Air	348 ///-- Not referenced in Source 
//#define		TID_Staff_05_El	349 ///-- Not referenced in Source 
//#define		TID_Staff_06_Mars	350 ///-- Not referenced in Source 
//#define		TID_Staff_07_Ark	351 ///-- Not referenced in Source 
//#define		TID_Staff_08_Royal	352 ///-- Not referenced in Source 
//#define		TID_Staff_09_Reincarnate	353 ///-- Not referenced in Source 
//#define		TID_Staff_10_ElRod	354 ///-- Not referenced in Source 
//#define		TID_Staff_11_ElSihard	355 ///-- Not referenced in Source 
//#define		TID_Staff_12_Isyllord	356 ///-- Not referenced in Source 
//#define		TID_Book_00_Strange	357 ///-- Not referenced in Source 
//#define		TID_Book_01_Pri	358 ///-- Not referenced in Source 
//#define		TID_Book_02_Ses	359 ///-- Not referenced in Source 
//#define		TID_Book_03_Rant	360 ///-- Not referenced in Source 
//#define		TID_Book_04_Prit	361 ///-- Not referenced in Source 
//#define		TID_Book_05_Seas	362 ///-- Not referenced in Source 
//#define		TID_Book_06_Raart	363 ///-- Not referenced in Source 
//#define		TID_Book_07_Air	364 ///-- Not referenced in Source 
//#define		TID_Book_08_Black	365 ///-- Not referenced in Source 
//#define		TID_Book_09_Elemental	366 ///-- Not referenced in Source 
//#define		TID_Crystal_Black	367 ///-- Not referenced in Source 
//#define		TID_Crystal_Tybore	368 ///-- Not referenced in Source 
//#define		TID_Crystal_Eltron	369 ///-- Not referenced in Source 
//#define		TID_Crystal_Bakalyon	370 ///-- Not referenced in Source 
//#define		TID_NewItem_01_Axe	371 ///-- Not referenced in Source 
//#define		TID_NewItem_02_Blade	372 ///-- Not referenced in Source 
//#define		TID_NewItem_03_Stick	373 ///-- Not referenced in Source 
//#define		TID_NewItem_04_Weapon	374 ///-- Not referenced in Source 
//#define		TID_NewItem_05_Weapon	375 ///-- Not referenced in Source 
//#define		TID_NewItem_06_Weapon	376 ///-- Not referenced in Source 
//#define		TID_NewItem_07_Weapon	377 ///-- Not referenced in Source 
//#define		TID_NewItem_08_Weapon	378 ///-- Not referenced in Source 
//#define		TID_NewItem_09_Weapon	379 ///-- Not referenced in Source 
//#define		TID_NewItem_10_Weapon	380 ///-- Not referenced in Source 
//#define		TID_NewItem_11_Weapon	381 ///-- Not referenced in Source 
//#define		TID_Crystal_01_Weapon	382 ///-- Not referenced in Source 
//#define		TID_Crystal_02_Weapon	383 ///-- Not referenced in Source 
//#define		TID_Crystal_03_Weapon	384 ///-- Not referenced in Source 
//#define		TID_Crystal_04_Weapon	385 ///-- Not referenced in Source 
//#define		TID_Crystal_05_Weapon	386 ///-- Not referenced in Source 
//#define		TID_Bow_01	387 ///-- Not referenced in Source 
//#define		TID_Bow_02	388 ///-- Not referenced in Source 
//#define		TID_Bow_03	389 ///-- Not referenced in Source 
//#define		TID_Bow_04	390 ///-- Not referenced in Source 
//#define		TID_Bow_05	391 ///-- Not referenced in Source 
//#define		TID_Bow_06	392 ///-- Not referenced in Source 
//#define		TID_Bow_07	393 ///-- Not referenced in Source 
//#define		TID_Bow_08	394 ///-- Not referenced in Source 
//#define		TID_Sword_12	395 ///-- Not referenced in Source 
#define		TID_Iron_01_Helm	396
#define		TID_Iron_02_Helm	397
#define		TID_Iron_03_Helm	398
#define		TID_Iron_04_Helm	399
#define		TID_Iron_05_Helm	400
#define		TID_Iron_06_Helm	401
#define		TID_Iron_07_Helm	402
#define		TID_Iron_08_Helm	403
#define		TID_Feather_01_Accessory	404
//#define		TID_Feather_02_Accessory	405 ///-- Not referenced in Source 
//#define		TID_Feather_03_Accessory	406 ///-- Not referenced in Source 
//#define		TID_Feather_04_Accessory	407 ///-- Not referenced in Source 
//#define		TID_Trinket_01_Accessory	408 ///-- Not referenced in Source 
//#define		TID_Trinket_02_Accessory	409 ///-- Not referenced in Source 
//#define		TID_Trinket_03_Accessory	410 ///-- Not referenced in Source 
//#define		TID_Trinket_04_Accessory	411 ///-- Not referenced in Source 
//#define		TID_Trinket_05_Accessory	412 ///-- Not referenced in Source 
#define		TID_Healing_Potion01	413
#define		TID_Healing_Potion02	414
#define		TID_Mana_Potion01	415
#define		TID_Mana_Potion02	416
//#define		TID_Healing1	417 ///-- Not referenced in Source 
//#define		TID_Healing2	418 ///-- Not referenced in Source 
//#define		TID_Mana1	419 ///-- Not referenced in Source 
//#define		TID_Mana2	420 ///-- Not referenced in Source 
//#define		TID_MScroll	421 ///-- Not referenced in Source 
//#define		TID_Water	422 ///-- Not referenced in Source
#define		TID_EldIMG		1690

#define		TID_Sychy_Message	423
#define		TID_Maron_blood01	424
#define		TID_Maron_blood02	425
#define		TID_Healling_explanation	426
#define		TID_lupus_canine	427
#define		TID_meditation_proof	428
#define		TID_Maron_head	429
//#define		TID_Wood	430 ///-- Not referenced in Source 
//#define		TID_Garon_Head	431 ///-- Not referenced in Source 
//#define		TID_Ceru_Skin	432 ///-- Not referenced in Source 
//#define		TID_Alba	433 ///-- Not referenced in Source 
//#define		TID_RupasTeeth	434 ///-- Not referenced in Source 
//#define		TID_PupasTail	435 ///-- Not referenced in Source 
//#define		TID_ReBag	436 ///-- Not referenced in Source 
//#define		TID_Report2	437 ///-- Not referenced in Source 
//#define		TID_MaronSkin	438 ///-- Not referenced in Source 
//#define		TID_Paper3	439 ///-- Not referenced in Source 
#define		TID_Hart	440
#define		TID_Ohigen	441
#define		TID_Flora	442
#define		TID_Arbess	443
#define		TID_Tituruit	444
#define		TID_Strange_Old	445
#define		TID_Rui	446
#define		TID_ElRoo	447
#define		TID_Tiris	448
#define		TID_Nanaha	449
#define		TID_Reishana	450
#define		TID_Arbeit	451
#define		TID_NewbiFrominas	452
#define		TID_Inves	453
#define		TID_Sychy	454
#define		TID_Nas	455
#define		TID_VolTraja	456
#define		TID_Millena_Soldier	457
#define		TID_Mission_Soldier	458
#define		TID_NPC_Port18		906	
#define		TID_NPC_Port19		911
#define		TID_NPC_Port20		1716	
#define		TID_NPC_Port21		1717	
#define		TID_NPC_Port22		1718	
#define		TID_NPC_Port23		838	
#define		TID_NPC_Port24		839	
#define		TID_NPC_Port25		840	
#define		TID_NPC_Port26		841	
#define		TID_NPC_Port27		1971	
#define		TID_NPC_Port28		1972
#define		TID_rain_oldman		2193
#define		TID_rain_littleboy	2194
#define		TID_rain_littlegirl	2195
#define		TID_rain_man		2196
#define		TID_rain_girl		2197
#define		TID_millena_girl	2198
#define		TID_millena_man		2199
#define		TID_millena_littleboy	2200
#define		TID_millena_oldman		2201
#define		TID_millena_littlegirl	2202
#define		TID_millena_soldier2	2203
#define		TID_snow_man		2204	
#define		TID_rain_soldier2	2205	
	

#define		TID_MAP02_512	459 
#define		TID_MAP03_512	460 		
#define		TID_MAP04_512		461	
#define		TID_MAP05_256		462	
#define		TID_MAP06_512		463	
#define		TID_MAP07_512		464	
#define		TID_MAP08_512		465	
#define		TID_MAP09_512		466	
#define		TID_MAP10_512		467	
#define		TID_MAP11_512		468	
#define		TID_MAP12_512		469	
#define		TID_MAP13_512		895	
#define		TID_MAP14_512		896	
#define		TID_MAP15_512		897	
#define		TID_MAP16_512		1554
#define		TID_MAP17_512		1555
#define		TID_MAP18_512		1556
#define		TID_MAP19_512		1557
#define		TID_MAP20_512		1558
#define		TID_MAP21_512		1559
#define		TID_MAP22_512		1724
#define		TID_MAP23_512		1725
#define		TID_MAP24_128		1827
#define		TID_MAP25_256		1939
#define		TID_MAP26_256		1940
#define		TID_MAP27_256		1941
#define		TID_MAP28_256		1942
#define		TID_MAP29_256		1943
#define		TID_MAP30_512		1944
#define		TID_MAP33_256		1945
#define		TID_MAP34_256		1946
#define		TID_MAP35_256		1898
#define		TID_MAP36_256		1899
#define		TID_MAP41_512		2535
#define		TID_MAP42_512		2536
#define		TID_MAP43_512		2537
#define		TID_MAP44_256		2628
#define		TID_MAP45_512		2646
#define		TID_MAP46_512		2647
#define		TID_MAP47_512		2806
#define		TID_MAP48_512		2997


#define		TID_Direction0	472
//#define		TID_Direction1	473 ///-- Not referenced in Source 
//#define		TID_Direction2	474 ///-- Not referenced in Source 
//#define		TID_Direction3	475 ///-- Not referenced in Source 
//#define		TID_Direction4	476 ///-- Not referenced in Source 
//#define		TID_Direction5	477 ///-- Not referenced in Source 
//#define		TID_Direction6	478 ///-- Not referenced in Source 
//#define		TID_Direction7	479 ///-- Not referenced in Source 
//#define		TID_Direction8	480 ///-- Not referenced in Source 
//#define		TID_Direction9	481 ///-- Not referenced in Source 
//#define		TID_Direction10	482 ///-- Not referenced in Source 
//#define		TID_Direction11	483 ///-- Not referenced in Source 
//#define		TID_Direction12	484 ///-- Not referenced in Source 
//#define		TID_Direction13	485 ///-- Not referenced in Source 
//#define		TID_Direction14	486 ///-- Not referenced in Source 
//#define		TID_Direction15	487 ///-- Not referenced in Source 
#define		TID_HLP_North	488
#define		TID_HLP_NorthEast	489
#define		TID_HLP_East	490
#define		TID_HLP_SouthEast	491
#define		TID_HLP_South	492
#define		TID_HLP_SouthWest	493
#define		TID_HLP_West	494
#define		TID_HLP_NorthWest	495
#define		TID_DB_NL_North	496
#define		TID_DB_NL_NorthEast	497
#define		TID_DB_NL_East	498
#define		TID_DB_NL_SouthEast	499
#define		TID_DB_NL_South	500
#define		TID_DB_NL_SouthWest	501
#define		TID_DB_NL_West	502
#define		TID_DB_NL_NorthWest	503
#define		TID_DB_NL_Tail	504
#define		TID_ST_START	505
//#define		DT	506 ///-- Not referenced in Source 
//#define		DT	507 ///-- Not referenced in Source 
//#define		DT	508 ///-- Not referenced in Source 
//#define		DT	509 ///-- Not referenced in Source 
//#define		DT	510 ///-- Not referenced in Source 
//#define		DT	511 ///-- Not referenced in Source 
//#define		DT	512 ///-- Not referenced in Source 
//#define		DT	513 ///-- Not referenced in Source 
//#define		DT	514 ///-- Not referenced in Source 
//#define		DT	515 ///-- Not referenced in Source 
//#define		DT	516 ///-- Not referenced in Source 
//#define		DT	517 ///-- Not referenced in Source 
//#define		DT	518 ///-- Not referenced in Source 
//#define		DT	519 ///-- Not referenced in Source 
//#define		DT	520 ///-- Not referenced in Source 
//#define		DT	521 ///-- Not referenced in Source 
//#define		DT	522 ///-- Not referenced in Source 
//#define		DT	523 ///-- Not referenced in Source 
//#define		DT	524 ///-- Not referenced in Source 
//#define		DT	525 ///-- Not referenced in Source 
//#define		DT	526 ///-- Not referenced in Source 
//#define		DT	527 ///-- Not referenced in Source 
//#define		DT	528 ///-- Not referenced in Source 
//#define		DT	529 ///-- Not referenced in Source 
//#define		DT	530 ///-- Not referenced in Source 
//#define		DT	531 ///-- Not referenced in Source 
//#define		DT	532 ///-- Not referenced in Source 
//#define		DT	533 ///-- Not referenced in Source 
//#define		DT	534 ///-- Not referenced in Source 
//#define		DT	535 ///-- Not referenced in Source 
//#define		DT	536 ///-- Not referenced in Source 
//#define		DT	537 ///-- Not referenced in Source 
//#define		DT	538 ///-- Not referenced in Source 
//#define		DT	539 ///-- Not referenced in Source 
//#define		DT	540 ///-- Not referenced in Source 
//#define		DT	541 ///-- Not referenced in Source 
//#define		DT	542 ///-- Not referenced in Source 
//#define		DT	543 ///-- Not referenced in Source 
//#define		DT	544 ///-- Not referenced in Source 
//#define		DT	545 ///-- Not referenced in Source 
//#define		DT	546 ///-- Not referenced in Source 
//#define		DT	547 ///-- Not referenced in Source 
//#define		DT	548 ///-- Not referenced in Source 
//#define		DT	549 ///-- Not referenced in Source 
//#define		DT	550 ///-- Not referenced in Source 
//#define		DT	551 ///-- Not referenced in Source 
//#define		DT	552 ///-- Not referenced in Source 
//#define		DT	553 ///-- Not referenced in Source 
//#define		DT	554 ///-- Not referenced in Source 
//#define		DT	555 ///-- Not referenced in Source 
//#define		DT	556 ///-- Not referenced in Source 
//#define		DT	557 ///-- Not referenced in Source 
//#define		DT	558 ///-- Not referenced in Source 
//#define		DT	559 ///-- Not referenced in Source 
//#define		DT	560 ///-- Not referenced in Source 
#define		TID_Cursor_Normal	561
#define		TID_Cursor_HAND0	562
#define		TID_Cursor_ITEMDROP0	563
#define		TID_Cursor_MOUTH0	564
#define		TID_Cursor_SKILL0	565
#define		TID_Cursor_SKILL1	566
#define		TID_Cursor_SKILL2	567
#define		TID_Cursor_SWORD	568
#define		TID_Cursor_TXT	569
#define		TID_Cursor_TXT_X	570
//................................................................................................................
// 로그인창
//................................................................................................................
#define		TID_Login_Wnd				571
#define		TID_Login_OkBtn_Focus		572
#define		TID_Login_CanCelBtn_Focus	573
#define		TID_Login_OkBtn_Select		574
#define		TID_Login_CancelBtn_Select	575

/*#define		TID_MAINMENU_CON_PRISM	600 ///-- Not referenced in Source 
#define		TID_MainMenu_UpBtn_Select	601
#define		TID_MainMenu_DownBtn_Select	602
#define		TID_MainMenu_StatWnd_Focus	603
#define		TID_MainMenu_SkillWnd_Focus	604
#define		TID_MainMenu_MessengerWnd_Focus	605
#define		TID_MainMenu_InvenWnd_Focus	606
#define		TID_MainMenu_QuestWnd_Focus	607
#define		TID_MainMenu_SystemWnd_Focus	608
#define		TID_MainMenu_HelpWnd_Focus	609
#define		TID_MainMenu_StatWnd_Select	610
#define		TID_MainMenu_SkillWnd_Select	611
#define		TID_MainMenu_MessengerWnd_Select	612
#define		TID_MainMenu_InvenWnd_Select	613
#define		TID_MainMenu_QuestWnd_Select	614
#define		TID_MainMenu_SystemWnd_Select	615
#define		TID_MainMenu_HelpWnd_Select	616
#define		TID_MainMenu_LVgage	617
#define		TID_MainMenu_LVHighlight	618
#define		TID_MainMenu_QuestWnd_Highlight	619
#define		TID_MainMenu_StatWnd_Highlight		1701	
#define		TID_MainMenu_SkillWnd_Highlight		1702	
#define		TID_MainMenu_MessengerWnd_Highlight	1703	
#define		TID_MainMenu_InvenWnd_Highlight		1704	
#define		TID_MainMenu_QuestWnd_Highlight		619		
#define		TID_MainMenu_SystemWnd_Highlight	1705	
#define		TID_MainMenu_HelpWnd_Highlight		1706
*/	
#define		TID_Chat_Wnd	620
#define		TID_Chat_FrameBar	621
#define		TID_Chat_SizeBtn_Focus	622
#define		TID_Chat_SizeBtn_Select	623
#define		TID_Chat_ChatType_Whisper	624
#define		TID_Chat_ChatType_Friend	625
#define		TID_Chat_ChatType_Party	626
#define		TID_Chat_ChatType_Guild	627
#define		TID_Chat_MessageBG	628

#define     TID_PlayerInfo_Warning			635
#define		TID_ChantHitBar_Wnd	661
#define		TID_ChantHitBar_TimeBar	662
#define		TID_ChantHitBar_Point_Normal	663
#define		TID_ChantHitBar_Point_Over	664
#define		TID_ChantHitBar_Point_Down	665

#define		TID_Alive_Wnd	696
#define		TID_Alive_OkBtn_Normal	697
#define		TID_Alive_OKBtn_Focus	698
#define		TID_Alive_OkBtn_Select	699	
#define		TID_NpcChat_AskImg	757
#define		TID_NpcChat_Ask_NextBtn_Focus	758
#define		TID_NpcChat_Ask_NextBtn_Select	759
#define		TID_NpcChat_Ask_NextBtn_Twinkle	760
#define		TID_NpcChat_Ask_CloseBtn_Focus	761
#define		TID_NpcChat_Ask_CloseBtn_Select	762
#define		TID_NpcChat_AnswerBtn_Normal	763
#define		TID_NpcChat_AnswerBtn_Select	764
//................................................................................................................
// 서버 선택창
//................................................................................................................
#define		TID_Server_List_Select_Svr		816
#define		TID_Server_List_Select_Chn		817
#define		TID_C_SERVER_LWND			818	
#define		TID_C_SERVER_CWND			819	
#define		TID_C_SERVER_RWND			820	
#define		TID_Server_List_Profress_Bar	821

//................................................................................................................
// 레인 직업 미리 보기
//................................................................................................................
//................................................................................................................
#define		TID_MessageBox_WIN	823
#define		TID_MessageBox_OK_UP	824
#define		TID_MessageBox_OK_DN	825
#define		TID_MessageBox_CANCEL_UP	826
#define		TID_MessageBox_CANCEL_DN	827

//................................................................................................................
// 캐릭터 선택창 
//................................................................................................................
#define			TID_CharSel_Name_Img	829	
#define			TID_CharSel				830	
#define			TID_CharSel_OVR			831	
#define			TID_CharSel_DIS			832	
#define			TID_CharSel_CLK			833	
//................................................................................................................
// 로그인 셔터
//................................................................................................................
#define		TID_Shutter_Up				938
#define		TID_Shutter_Dn				939
//................................................................................................................
// 캐릭터 생성창
//................................................................................................................
#define		TID_CharCreate_Dlg				845	
#define		TID_CharCreate_Millena			846	
#define		TID_CharCreate_Millena_On		847	
#define		TID_CharCreate_Millena_Da		848	
#define		TID_CharCreate_Rain				849	
#define		TID_CharCreate_Rain_On			850	
#define		TID_CharCreate_Rain_Da			851	
#define		TID_CharCreate_Left				852	
#define		TID_CharCreate_Left_OVR			853	
#define		TID_CharCreate_Left_CLK			854	
#define		TID_CharCreate_Right			855	
#define		TID_CharCreate_Right_OVR		856	
#define		TID_CharCreate_Right_CLK		857	
#define		TID_CharCreate_MINUS			858	
#define		TID_CharCreate_MINUS_OVR		859	
#define		TID_CharCreate_MINUS_CLK		860	
#define		TID_CharCreate_PLUS				861	
#define		TID_CharCreate_PLUS_OVR			862	
#define		TID_CharCreate_PLUS_CLK			863	
#define		TID_CharCreate_BT				864	
#define		TID_CharCreate_BT_OVR			865	
#define		TID_CharCreate_BT_CLK			866	
#define		TID_CharCreate_BACK_SEX			867	
#define		TID_CharCreate_BACK_NATION		868	
#define 		TID_PreView_Image		936	
#define 		TID_Rain_Previwe		937	
#define 		TID_Millena_Previwe		572	
#define 		TID_prejob_sword		573	
#define 		TID_prejob_archer		574	
#define 		TID_prejob_mercynary	575	
#define 		TID_prejob_soul			822	
#define 		TID_prejob_knight		815	
#define 		TID_prejob_slayer		824	
#define 		TID_prejob_magician		825	
#define 		TID_prejob_shaman		826	
#define 		TID_prejob_holy			827	
#define 		TID_prejob_shamer		834	
#define 		TID_prejob_prom			835	
#define 		TID_prejob_prist		836	
#define 		TID_prejob_OVR			837 
#define			TID_prejob_sikongchanga		2904  
//................................................................................................................
// 직업 보기 도움말 뒷 배경
//................................................................................................................
#define		TID_Help_Image					940

#define		TID_SkillLearn_Wnd	871
#define		TID_SkillLearn_Field_Focus	872
#define		TID_LogoutClient	879
#define		TID_LogoutBar	880
#define		TID_ArcherBar_Hit	881
#define		TID_ArcherBar_Hit_Check	882
#define		TID_ArcherBar_Else	883
#define		TID_ArcherBar_Bar	884
#define		TID_ArcherBar_PBar0	885
#define		TID_ArcherBar_PBar1	886
#define		TID_ArcherBar_NomalHit	2190	
#define		TID_ArcherBar_CritiHit	2191	

#define		TID_Tooltip2_Img	899
#define		TID_mini_npc	900
#define		TID_mini_party	901
#define		TID_mini_qstartglow	902
#define		TID_mini_qreportglow 903
//MONSTER_INFO.dds 추가정보

//PC_INFO.dds  추가정보
#define		TID_PlayerInfo_GlowWnd	908
#define		TID_PlayerInfo_PrevHP_Gauge	909
#define		TID_PlayerInfo_GlowHP_Gauge	910

///---------------------------------------------------------------------------
///-- Mini Game 관련
///---------------------------------------------------------------------------
#define		TID_ELD_BACKGRIUND_TOP	952
#define		TID_ELD_BACKGRIUND_BTM	953
#define		TID_ELD_BOARD	954
#define		TID_ELD_TIMEBAR4	955
#define		TID_ELD_EFF_SLIDE	956
#define		TID_ELD_EFF_HIT_LT	957
#define		TID_ELD_EFF_HIT_RB	958
#define		TID_ELD_EFF_HIT_LB	959
#define		TID_ELD_EFF_HIT_RT	960
#define		TID_ELD_SHADOW	961
#define		TID_ELD_TIMEBAR1	962
#define		TID_ABORT_CLK	963
#define		TID_ABORT_OVR	964
#define		TID_ELD_TIMEBAR2	965
#define		TID_Number_2	966
#define		TID_Number_3	967 
#define		TID_Number_4	968 
#define		TID_Number_5	969 
#define		TID_Number_6	970 
#define		TID_Number_7	971 
#define		TID_MSG_Good	972 
#define		TID_MSG_Combo	973 
#define		TID_MSG_Great	974 
#define		TID_MSG_Excellent	975 
#define		TID_MSG_Slendid	976 
#define		TID_MSG_Superb	977 
#define		TID_MSG_Miss	978 
#define		TID_MSG_READY	979 
#define		TID_MSG_GO	980 
#define		TID_MSG_TIMEOVER	981 
#define		TID_MSG_MISSION	982 
#define		TID_MSG_SUCCESS	983 
#define		TID_MSG_PERFECT	984 
#define		TID_ELD_SHADOWHIT	985
#define		TID_ELD_RB_1	1295
//#define		TID_ELD_RB_2	1296 ///-- Not referenced in Source 
//#define		TID_ELD_RB_3	1297 ///-- Not referenced in Source 
//#define		TID_ELD_RB_4	1298 ///-- Not referenced in Source 
//#define		TID_ELD_RB_5	1299 ///-- Not referenced in Source 
//#define		TID_ELD_RB_6	1300 ///-- Not referenced in Source 
//#define		TID_ELD_RB_7	1301 ///-- Not referenced in Source 
//#define		TID_ELD_RB_8	1302 ///-- Not referenced in Source 
//#define		TID_ELD_RB_9	1303 ///-- Not referenced in Source 
//#define		TID_ELD_RB_10	1304 ///-- Not referenced in Source 
//#define		TID_ELD_RB_11	1305 ///-- Not referenced in Source 
//#define		TID_ELD_RB_12	1306 ///-- Not referenced in Source 
#define		TID_ELD_LB_1	1307
//#define		TID_ELD_LB_2	1308 ///-- Not referenced in Source 
//#define		TID_ELD_LB_3	1309 ///-- Not referenced in Source 
//#define		TID_ELD_LB_4	1310 ///-- Not referenced in Source 
//#define		TID_ELD_LB_5	1311 ///-- Not referenced in Source 
//#define		TID_ELD_LB_6	1312 ///-- Not referenced in Source 
//#define		TID_ELD_LB_7	1313 ///-- Not referenced in Source 
//#define		TID_ELD_LB_8	1314 ///-- Not referenced in Source 
//#define		TID_ELD_LB_9	1315 ///-- Not referenced in Source 
//#define		TID_ELD_LB_10	1316 ///-- Not referenced in Source 
//#define		TID_ELD_LB_11	1317 ///-- Not referenced in Source 
//#define		TID_ELD_LB_12	1318 ///-- Not referenced in Source 
#define		TID_ELD_RT_1	1319
//#define		TID_ELD_RT_2	1320 ///-- Not referenced in Source 
//#define		TID_ELD_RT_3	1321 ///-- Not referenced in Source 
//#define		TID_ELD_RT_4	1322 ///-- Not referenced in Source 
//#define		TID_ELD_RT_5	1323 ///-- Not referenced in Source 
//#define		TID_ELD_RT_6	1324 ///-- Not referenced in Source 
//#define		TID_ELD_RT_7	1325 ///-- Not referenced in Source 
//#define		TID_ELD_RT_8	1326 ///-- Not referenced in Source 
//#define		TID_ELD_RT_9	1327 ///-- Not referenced in Source 
//#define		TID_ELD_RT_10	1328 ///-- Not referenced in Source 
//#define		TID_ELD_RT_11	1329 ///-- Not referenced in Source 
//#define		TID_ELD_RT_12	1330 ///-- Not referenced in Source 
#define		TID_ELD_LT_1	1331
//#define		TID_ELD_LT_2	1332 ///-- Not referenced in Source 
//#define		TID_ELD_LT_3	1333 ///-- Not referenced in Source 
//#define		TID_ELD_LT_4	1334 ///-- Not referenced in Source 
//#define		TID_ELD_LT_5	1335 ///-- Not referenced in Source 
//#define		TID_ELD_LT_6	1336 ///-- Not referenced in Source 
//#define		TID_ELD_LT_7	1337 ///-- Not referenced in Source 
//#define		TID_ELD_LT_8	1338 ///-- Not referenced in Source 
//#define		TID_ELD_LT_9	1339 ///-- Not referenced in Source 
//#define		TID_ELD_LT_10	1340 ///-- Not referenced in Source 
//#define		TID_ELD_LT_11	1341 ///-- Not referenced in Source 
//#define		TID_ELD_LT_12	1342 ///-- Not referenced in Source 
#define		TID_ELDEFF_RB_1		1345
#define		TID_ELDEFF_RB_2		1346
#define		TID_ELDEFF_RB_3		1347
#define		TID_ELDEFF_RB_4		1348
#define		TID_ELDEFF_RB_5		1349
#define		TID_ELDEFF_LB_1		1350
#define		TID_ELDEFF_LB_2		1351
#define		TID_ELDEFF_LB_3		1352
#define		TID_ELDEFF_LB_4		1353
#define		TID_ELDEFF_LB_5		1354
#define		TID_ELDEFF_RT_1		1355
#define		TID_ELDEFF_RT_2		1356
#define		TID_ELDEFF_RT_3		1357
#define		TID_ELDEFF_RT_4		1358
#define		TID_ELDEFF_RT_5		1359
#define		TID_ELDEFF_LT_1		1360
#define		TID_ELDEFF_LT_2		1361
#define		TID_ELDEFF_LT_3		1362
#define		TID_ELDEFF_LT_4		1363
#define		TID_ELDEFF_LT_5		1364
//------------------------------------------------------------
//파티정보창 , 파티멤버창 구성	  
//------------------------------------------------------------
#define		TID_PARTY_MEMBER_NORMAL				898	 
#define		TID_PARTY_MEMBER_SELECT				986	 
#define		TID_PARTY_MEMBER_WARNING			987  
#define		TID_PARTY_TITLE_WND					988	 
#define		TID_PARTY_GETITEM_HOWWND 			989  
#define		TID_PARTY_DOWN_WND					990  
#define		TID_PARTY_MEMBER_HPGAUGE			991	 
#define		TID_PARTY_MEMBER_SPMPGAUGE  		992  
#define		TID_PARTY_MEMBER_COMBO_LEVEL1		993	 
#define		TID_PARTY_MEMBER_COMBO_LEVEL2		994	 
#define		TID_PARTY_MEMBER_COMBO_LEVEL3		995	 
#define		TID_PARTY_MEMBER_COMBO_LEVEL4		996	 
#define		TID_PARTY_MEMBER_COMBO_LEVEL5		997	 
#define		TID_PARTY_MEMBER_COMBO_LEVEL6		998	 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL1		999	 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL2		1000 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL3		1001 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL4		1002 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL5		1003 
#define		TID_PARTY_MEMBER_COMBO_ENLEVEL6		1004 
#define		TID_PARTY_MEMBER_MASTER				1005 
#define		TID_PARTY_MEMBER_F1					1006 
#define		TID_PARTY_MEMBER_F2					1007 
#define		TID_PARTY_MEMBER_F3					1008 
#define		TID_PARTY_MEMBER_F4					1009 
#define		TID_PARTY_MEMBER_F5					1010 
#define		TID_PARTY_GETITEM_FREENOWSEL		1011 
#define		TID_PARTY_GETITEM_FREEDISABLESEL	1012 
#define		TID_PARTY_GETITEM_FREEENABLESEL		1013 
#define		TID_PARTY_GETITEM_SEQUENOWSEL		1014 
#define		TID_PARTY_GETITEM_SEQUEDISABLESEL	1961  
#define		TID_PARTY_GETITEM_SEQUEENABLESEL	1962  
#define		TID_PARTY_GETITEM_COUNTNOWSEL		1963  
#define		TID_PARTY_GETITEM_COUNTDISABLESEL	1964  
#define		TID_PARTY_GETITEM_COUNTENABLESEL	1965  
#define		TID_PARTY_MEMBER_LOGOFF				1966  
#define		TID_PARTY_MEMBER_NOSEE				1967  
#define		TID_PARTY_MEMBER_CHAR_SELECT		1968  
#define		TID_PARTY_MEMBER_BTN_NORMAL			1969  
#define		TID_PARTY_MEMBER_BTN_FOCUS			1970  

#define		TID_PARTY_LEAGUE_BTN_NORMAL			2475  
#define		TID_PARTY_LEAGUE_BTN_FOCUS			2476  	
#define		TID_PARTY_LEAGUE_BTN_CLICK			2477  

//------------------------------------------------------------
//메세지 창 구성요소
//------------------------------------------------------------
#define		TID_Com_Msg_WndBg1				1015

//------------------------------------------------------------
//메시지 창 - 파티용 타이틀, 에디트
//------------------------------------------------------------	
#define		TID_UIMsg_InputBox			1020
#define		TID_UIMsg_PRNameTitle		1021
#define		TID_UIMsg_POutTitle			1022
#define		TID_UIMsg_PInTitle			1023
#define		TID_UIMsg_PBreakTitle		1024
#define		TID_UIMsg_PCallTitle		1025
#define		TID_UIMsg_PGiveTitle		1026
#define		TID_UIMsg_PGoAwayTitle		1027   	

//-------------------------------------------------------------
//파티 - 팝업 메뉴창
//-------------------------------------------------------------
#define		TID_Party_PopupWnd		1028		
#define		TID_Party_TextFocus		1029

///-- 3 차 Close 추가 Item / Skill
#define		TID_merge_eldda_gemstone1	1060 
#define		TID_merge_eldda_gemstone2	1061 
#define		TID_merge_eldda_gemstone3	1062 
#define		TID_merge_eldda_gemstone4	1063 
#define		TID_merge_eldda_gemstone5	1064 
#define		TID_merge_eldda_gemstone6	1065 
#define		TID_merge_eldda_gemstone7	1066 
#define		TID_merge_eldda_gemstone8	1067 
#define		TID_merge_eldda_gemstone9	1068 
#define		TID_merge_eldda_gemstone10	1069 
#define		TID_merge_eldda_green1	1070 
#define		TID_merge_eldda_green2	1071 
#define		TID_merge_eldda_green3	1072 
#define		TID_merge_eldda_green4	1073 
#define		TID_merge_eldda_green5	1074 
#define		TID_merge_eldda_green6	1075 
#define		TID_merge_eldda_green7	1076 
#define		TID_merge_eldda_green8	1077 
#define		TID_merge_eldda_green9	1078 
#define		TID_merge_eldda_green10	1079 
#define		TID_merge_eldda_blue1	1080 
#define		TID_merge_eldda_blue2	1081 
#define		TID_merge_eldda_blue3	1082 
#define		TID_merge_eldda_blue4	1083 
#define		TID_merge_eldda_blue5	1084 
#define		TID_merge_eldda_blue6	1085 
#define		TID_merge_eldda_blue7	1086 
#define		TID_merge_eldda_blue8	1087 
#define		TID_merge_eldda_blue9	1088 
#define		TID_merge_eldda_blue10	1089 
#define		TID_merge_eldda_yellow1	1090 
#define		TID_merge_eldda_yellow2	1091 
#define		TID_merge_eldda_yellow3	1092 
#define		TID_merge_eldda_yellow4	1093 
#define		TID_merge_eldda_yellow5	1094 
#define		TID_merge_eldda_yellow6	1095 
#define		TID_merge_eldda_yellow7	1096 
#define		TID_merge_eldda_yellow8	1097 
#define		TID_merge_eldda_yellow9	1098 
#define		TID_merge_eldda_yellow10	1099 
#define		TID_merge_eldda_red1	1100 
#define		TID_merge_eldda_red2	1101 
#define		TID_merge_eldda_red3	1102 
#define		TID_merge_eldda_red4	1103 
#define		TID_merge_eldda_red5	1104 
#define		TID_merge_eldda_red6	1105 
#define		TID_merge_eldda_red7	1106 
#define		TID_merge_eldda_red8	1107 
#define		TID_merge_eldda_red9	1108 
#define		TID_merge_eldda_red10	1109 
#define		TID_merge_eldda_violet1	1110 
#define		TID_merge_eldda_violet2	1111 
#define		TID_merge_eldda_violet3	1112 
#define		TID_merge_eldda_violet4	1113 
#define		TID_merge_eldda_violet5	1114 
#define		TID_merge_eldda_violet6	1115 
#define		TID_merge_eldda_violet7	1116 
#define		TID_merge_eldda_violet8	1117 
#define		TID_merge_eldda_violet9	1118 
#define		TID_merge_eldda_violet10	1119 
#define		TID_merge_eldda_purple1	1120 
#define		TID_merge_eldda_purple2	1121 
#define		TID_merge_eldda_purple3	1122 
#define		TID_merge_eldda_purple4	1123 
#define		TID_merge_eldda_purple5	1124 
#define		TID_merge_eldda_purple6	1125 
#define		TID_merge_eldda_purple7	1126 
#define		TID_merge_eldda_purple8	1127 
#define		TID_merge_eldda_purple9	1128 
#define		TID_merge_eldda_purple10	1129 
#define		TID_merge_eldda_indigo1	1130 
#define		TID_merge_eldda_indigo2	1131 
#define		TID_merge_eldda_indigo3	1132 
#define		TID_merge_eldda_indigo4	1133 
#define		TID_merge_eldda_indigo5	1134 
#define		TID_merge_eldda_indigo6	1135 
#define		TID_merge_eldda_indigo7	1136 
#define		TID_merge_eldda_indigo8	1137 
#define		TID_merge_eldda_indigo9	1138 
#define		TID_merge_eldda_indigo10	1139 
#define		TID_merge_eldda_silver1	1140 
#define		TID_merge_eldda_silver2	1141 
#define		TID_merge_eldda_silver3	1142 
#define		TID_merge_eldda_silver4	1143 
#define		TID_merge_eldda_silver5	1144 
#define		TID_merge_eldda_silver6	1145 
#define		TID_merge_eldda_silver7	1146 
#define		TID_merge_eldda_silver8	1147 
#define		TID_merge_eldda_silver9	1148 
#define		TID_merge_eldda_silver10	1149 
#define		TID_merge_eldda_lightgreen1	1150 
#define		TID_merge_eldda_lightgreen2	1151 
#define		TID_merge_eldda_lightgreen3	1152 
#define		TID_merge_eldda_lightgreen4	1153 
#define		TID_merge_eldda_lightgreen5	1154 
#define		TID_merge_eldda_lightgreen6	1155 
#define		TID_merge_eldda_lightgreen7	1156 
#define		TID_merge_eldda_lightgreen8	1157 
#define		TID_merge_eldda_lightgreen9	1158 
#define		TID_merge_eldda_lightgreen10	1159 
#define		TID_merge_eldda_orange1	1160 
#define		TID_merge_eldda_orange2	1161 
#define		TID_merge_eldda_orange3	1162 
#define		TID_merge_eldda_orange4	1163 
#define		TID_merge_eldda_orange5	1164 
#define		TID_merge_eldda_orange6	1165 
#define		TID_merge_eldda_orange7	1166 
#define		TID_merge_eldda_orange8	1167 
#define		TID_merge_eldda_orange9	1168 
#define		TID_merge_eldda_orange10	1169 
#define		TID_item_quest_instant08	1170 
#define		TID_item_quest_instant09	1171 
#define		TID_item_quest_instant10	1172 
#define		TID_item_quest_instant11	1173 
#define		TID_item_quest_instant12	1174 
#define		TID_item_quest_instant13	1175 
#define		TID_item_quest_instant14	1176 
#define		TID_item_quest_instant15	1177 
#define		TID_item_quest_instant16	1178 
#define		TID_item_quest_instant17	1179 
#define		TID_item_quest_instant18	1180 
#define		TID_item_quest_instant19	1181 
#define		TID_item_quest_instant20	1182 
#define		TID_item_quest_instant21	1183 
#define		TID_item_quest_instant22	1184 
#define		TID_item_quest_instant23	1185 
#define		TID_item_quest_instant24	1186 
#define		TID_item_quest_instant25	1187 
#define		TID_item_quest_instant26	1188 
#define		TID_item_quest_instant27	1189 
#define		TID_item_quest_instant28	1190 
#define		TID_item_quest_instant29	1191 
#define		TID_item_quest_instant30	1192 
#define		TID_merge_item_shortclaw	1193 
#define		TID_merge_item_shortteeth	1194 
#define		TID_merge_item_greenskin	1195 
#define		TID_merge_item_blueskin	1196 
#define		TID_merge_item_kiragashorn	1197 
#define		TID_merge_item_garonteeth	1198 
#define		TID_merge_item_oldearring	1199 
#define		TID_merge_item_woodstick	1200 
#define		TID_merge_item_badfur	1201 
#define		TID_merge_item_brokenpeice	1202 
#define		TID_merge_item_kerupusteeth	1203 
#define		TID_merge_item_shorttail	1204 
#define		TID_merge_item_brokenfruit	1205 
#define		TID_merge_item_fur	1206 
#define		TID_merge_item_kerupusmolar	1207 
#define		TID_merge_item_karoncanine	1208 
#define		TID_merge_item_greentail	1209 
#define		TID_merge_item_redclaw	1210 
#define		TID_merge_item_karonearring	1211 
#define		TID_merge_item_karonchain	1212 
#define		TID_merge_item_redjewel	1213 
#define		TID_merge_item_postosskin	1214 
#define		TID_merge_item_postosclaw	1215 
#define		TID_merge_item_postostail	1216 
#define		TID_merge_item_postosteeth	1217 
#define		TID_merge_item_postoshorn	1218 
#define		TID_merge_item_postostoughskin	1219 
#define		TID_merge_item_kaironhorn	1220 
#define		TID_merge_item_kaironhoof	1221 
#define		TID_merge_item_kaironcrystal	1222 
#define		TID_merge_item_herokironhorn	1223 
#define		TID_merge_item_strongchop	1224 
#define		TID_merge_item_volacskin	1225 
#define		TID_merge_item_bigbone	1226 
#define		TID_merge_item_projection	1227 
#define		TID_merge_item_bigclaw	1228 
#define		TID_merge_item_picturebone	1229 
#define		TID_merge_item_pictureskin	1230 
#define		TID_merge_item_thonetail	1231 
#define		TID_merge_item_volachorn	1232 
#define		TID_merge_item_madidalinleg	1233 
#define		TID_merge_item_whitebeard	1234 
#define		TID_merge_item_fungustail1	1235 
#define		TID_merge_item_fungustail2	1236 
#define		TID_merge_item_redpattern	1237 
#define		TID_merge_item_maronhair	1238 
#define		TID_merge_item_patternskin	1239 
#define		TID_merge_item_hardstick	1240 
#define		TID_merge_item_chainadorn	1241 
#define		TID_merge_item_veigasfoot	1242 
#define		TID_merge_item_veigasbill	1243 
#define		TID_merge_item_bluefeature	1244 
#define		TID_merge_item_brownfeature	1245 
#define		TID_merge_item_veigastail	1246 
#define		TID_merge_item_veigashorn	1247 
#define		TID_merge_item_firetail	1248 
#define		TID_merge_item_redfeather	1249 
#define		TID_merge_item_rockskin	1250 
#define		TID_merge_item_rockprojection	1251 
#define		TID_merge_item_nosering	1252 
#define		TID_merge_item_magaronhoof	1253 
#define		TID_merge_item_magaronknee	1254 
#define		TID_merge_item_woodbord	1255 
#define		TID_merge_item_whitefur	1256 
#define		TID_merge_item_urakclaw	1257 
#define		TID_merge_item_urakallskin	1258 
#define		TID_merge_item_bluewhitefur	1259 
#define		TID_merge_item_whitefur2	1260 
#define		TID_merge_item_hardurakclaw	1261 
#define		TID_merge_item_urakheart	1262 
#define		TID_merge_item_verytoughfur	1263 
#define		TID_merge_item_strongmolar	1264 
#define		TID_merge_item_picoredbill	1265 
#define		TID_merge_item_whitepico	1266 
#define		TID_merge_item_blackpico	1267 
#define		TID_merge_item_picotail	1268 
#define		TID_merge_item_picobeard	1269 
#define		TID_merge_item_picoshapclaw	1270 
#define		TID_merge_item_redpicotail	1271 
#define		TID_merge_item_picoredcrown	1272 
#define		TID_merge_item_lightegg	1273 
#define		TID_shield_attack	1274 
#define		TID_foot_press	1275 
#define		TID_provocation	1276 
#define		TID_pressure	1277 
#define		TID_guard	1278 
#define		TID_heavy_amor	1279 
#define		TID_shield	1280 
#define		TID_spirits_attack	1281 
#define		TID_great_attack	1282 
#define		TID_blood_control	1283 
#define		TID_fortitude_will	1284 
#define		TID_medium_amor	1285 
#define		TID_heal	1286 
#define		TID_party_heal	1287 
#define		TID_resurrection	1288 
#define		TID_dispel	1289 
#define		TID_mana_light	1290 
#define		TID_eia_blessing	1291 
#define		TID_proima_baptism	1292 
#define		TID_priest_clothes	1293 
#define		TID_priest_staff	1294
#define		TID_U_LOADING_10		571
#define		TID_U_LOADING_17		581
#define		TID_U_LOADING_20		656
#define		TID_U_LOADING_21		657
#define		TID_U_LOADING_24		744
#define		TID_U_LOADING_25_26		745
#define		TID_LOADING_WALL_M1		738
#define		TID_LOADING_WALL_M2		739
#define		TID_LOADING_WALL_M3		740
#define		TID_LOADING_WALL_M4		741
#define		TID_LOADING_WALL_M5		742
#define		TID_LOADING_WALL_M6		3554
#define		TID_LOADING_WALL_R1		743
#define		TID_LOADING_WALL_R2		667
#define		TID_LOADING_WALL_R3		1431
#define		TID_LOADING_WALL_R4		2635
#define		TID_LOADING_WALL_R5		2636
#define		TID_LOADING_WALL_R6		3555
#define		TID_MINIGAMETEMP_2		1432
#define		TID_MINIGAMETEMP_3		1433
	
///---------------------------------------------------------------------------
///-- 공용 Window ( Elda관련 )
#define		TID_CTRL_BTN_WINMIN_DIS		1695
#define		TID_CTRL_BTN_WINMIN_OVR		1696
#define		TID_CTRL_BTN_WINMIN_CLK		1697
#define		TID_CTRL_BTN_WINMAX_DIS		1698
#define		TID_CTRL_BTN_WINMAX_OVR		1699
#define		TID_CTRL_BTN_WINMAX_CLK		1700

///----------------------------------------------------------------------------
///-- 커뮤니티 윈도우
///----------------------------------------------------------------------------			
#define		TID_MessengerBgUI				1454
#define		TID_MPopupMenuBgUI				1455
#define		TID_MPopupMenuFocus				1456
#define		TID_MTAB_FRIEND					1457
#define		TID_MTAB_FRIEND_ON				1458
#define		TID_MTAB_GUILD					1459
#define		TID_MTAB_GUILD_ON				1460
#define		TID_MTAB_BLOCK					1461
#define		TID_MTAB_BLOCK_ON				1462
#define		TID_MTAB_SETTING				1463
#define		TID_MTAB_SETTING_ON				1464
#define		TID_MBACK_FRIEND				1465
#define		TID_MBACK_BLOCK					1466
#define		TID_MBACK_GUILD					1467
#define		TID_MBACK_SETTING				1468
#define		TID_MStrangerIcon				1469
#define		TID_MMagicianIcon				1470
#define		TID_MProminasIcon				1471
#define		TID_MPriestIcon					1472
#define		TID_MHoly_AvengerIcon			1473
#define		TID_MShamanIcon					1474
#define		TID_MPsycheIcon					1475
#define		TID_MWandererIcon				1476
#define		TID_MSwordmanIcon				1477
#define		TID_MKnightIcon					1478
#define		TID_MMercenaryIcon				1479
#define		TID_MSlayerIcon					1480
#define		TID_MArcherIcon					1481
#define		TID_MSniperIcon					1482
#define		TID_MFriendSelFocus				1483
#define		TID_MBlockSelFocus				1484
#define		TID_MFNameFieldBtnFocus			1485
#define		TID_MFAreaFieldBtnFocus			1486
#define		TID_MFChanelFieldBtnFocus		1487
#define		TID_MFLevelFieldBtnFocus		1488
#define		TID_MFJobFieldBtnFocus			1489
#define		TID_MTEMP_BACK					1490
	
///----------------------------------------------------------------------------
///-- 메신져 채팅 윈도우
///----------------------------------------------------------------------------		
#define		TID_MCUpTitleBgImg			1516
#define		TID_MCDownControlBgImg		1517
#define		TID_MCOutChatMinBgImg		1518
#define		TID_MCOutChatMidBgImg		1519
#define		TID_MCOutChatMaxBgImg		1520
#define		TID_MCChatSendBtnFocus		1521
#define		TID_MCChatSendBtnClick		1522
#define		TID_MCUpScrollBtnClick		1523
#define		TID_MCDownScrollBtnClick	1524
#define		TID_MCThumbScrollBtn		1525
#define		TID_MCCloseBtnFocus			1526
#define		TID_MCCloseBtnClick			1527
#define		TID_MCSizeAddBtnNone		1528
#define		TID_MCSizeAddBtnFocus		1529
#define		TID_MCSizeAddBtnClick		1530
#define		TID_MCSizeSubBtnNone 	 	1531
#define		TID_MCSizeSubBtnFocus		1532
#define		TID_MCSizeSubBtnClick		1533
#define		TID_MCMemViewBtnNomal		1534
#define		TID_MCMemViewBtnFocus		1535
#define		TID_MCMemViewBtnClick		1536
#define		TID_MCMemListBgImg			1537
#define		TID_MCIDTitleBgImg			1538

///----------------------------------------------------------------------------
///-- 메신져 쪽지 윈도우
///----------------------------------------------------------------------------	
#define		TID_MessenMemoBgUI			1539	
#define		TID_MMConfirmBtnFocus		1540	
#define		TID_MMConfirmBtnClick		1541    
#define		TID_MMUpScrollBtnClick		1542	
#define		TID_MMDownScrollBtnClick	1543	
#define		TID_MMThumbScrollBtn		1544	
#define		TID_MMScrollHideImg			1545	
#define		TID_MMCloseBtnFocus			1546	
#define		TID_MMCloseBtnClick			1547	

///----------------------------------------------------------------------------
///-- 메신져 알림 윈도우
///----------------------------------------------------------------------------
#define		TID_MessenNotifyBgUI		1548	
#define		TID_MNotifyFlashImg			1549	
#define		TID_MNotifyCloseBtnFocus	1550	
#define		TID_MNotifyCloseBtnClick	1551	
#define		TID_MNotifyMouseOver		1552	
#define		TID_MNotifyMouseClick		1553


#define 	TID_Combo_barBg				1719	
#define 	TID_Combo_barGauge			1720	
#define		TID_Combo_barSBg			1721

///----------------------------------------------------------------------------
///-- 채팅창 리소스
///----------------------------------------------------------------------------
#define		TID_CHATINPUT_LWND		1726
#define		TID_CHATINPUT_CWND		1727
#define		TID_CHATINPUT_RWND		1728
#define		TID_CHATUPFRAME_LWND	1729
#define		TID_CHATUPFRAME_CWND	1730
#define		TID_CHATUPFRAME_RWND	1731
#define		TID_CFILTAB_NORMAL		1732
#define		TID_CFILTAB_SELECT		1733
#define		TID_NORMALCHAT_NORMAL	1734
#define		TID_NORMALCHAT_FOCUS	1735
#define		TID_NORMALCHAT_CLICK	1736
#define		TID_PARTYCHAT_NORMAL	1737
#define		TID_PARTYCHAT_FOCUS		1738
#define		TID_PARTYCHAT_CLICK		1739
#define		TID_GUILDCHAT_NORMAL	1740
#define		TID_GUILDCHAT_FOCUS		1741
#define		TID_GUILDCHAT_CLICK		1742
#define		TID_WHISCHAT_NORMAL		1743
#define		TID_WHISCHAT_FOCUS		1744
#define		TID_WHISCHAT_CLICK		1745
#define		TID_CSIZEBTN_NORMAL		1746
#define		TID_CSIZEBTN_FOCUS		1747
#define		TID_CSIZEBTN_CLICK		1748
#define		TID_CHAT_TITLEPATN		1749
// 사교동작 관련 추가 
#define			TID_CHANNELC_DISABLE	1992	
#define			TID_CHANNELC_NORMAL		1993
#define			TID_CHANNELC_FOCUS		1994
#define			TID_CHANNELC_CLICK		1995	
#define			TID_SERVERC_NORMAL		2783
#define			TID_SERVERC_FOCUS		2784
#define			TID_SERVERC_CLICK		2785
#define			TID_AREA_BRODCAST_DISABLE	3495
#define			TID_AREA_BRODCAST_NORMAL	3496
#define			TID_AREA_BRODCAST_FOCUS		3497
#define			TID_AREA_BRODCAST_CLICK		3498
#define			TID_SOCIALACT_BG_L1						1996
#define			TID_SOCIALACT_BG_C1						1997
#define			TID_SOCIALACT_BG_R1						1998
#define			TID_SOCIALACT_BG_L2						1999
#define			TID_SOCIALACT_BG_C2						2000
#define			TID_SOCIALACT_BG_R2						2001
#define			TID_SOCIALACT_BG_L3						2002
#define			TID_SOCIALACT_BG_C3						2003
#define			TID_SOCIALACT_BG_R3						2004
#define			TID_SOCIALACT_PREV_NORMAL				2005
#define			TID_SOCIALACT_PREV_FOCUS				2006
#define			TID_SOCIALACT_PREV_CLICK				2007
#define			TID_SOCIALACT_NEXT_NORMAL				2008
#define			TID_SOCIALACT_NEXT_FOCUS				2009
#define			TID_SOCIALACT_NEXT_CLICK				2010
#define			TID_SOCIALACT_BTN_FOCUS					2011
#define			TID_SOCIALACT_BTN_CLICK					2012

//==============================================================================

///---------------------------------------------------------------------------
///-- Common Control
///---------------------------------------------------------------------------
#define			TID_NPCCHAT_WND_L1		647	
#define			TID_NPCCHAT_WND_C1		648	
#define			TID_NPCCHAT_WND_R1		649	
#define			TID_NPCCHAT_WND_L2		650	
#define			TID_NPCCHAT_WND_C2		651	
#define			TID_NPCCHAT_WND_R2		652	
#define			TID_NPCCHAT_WND_L3		653	
#define			TID_NPCCHAT_WND_C3		654	
#define			TID_NPCCHAT_WND_R3		655	
#define			TID_NPCCHAT_CONT_WND_L1	770	
#define			TID_NPCCHAT_CONT_WND_C1	771	
#define			TID_NPCCHAT_CONT_WND_R1	772	
#define			TID_NPCCHAT_CONT_WND_L2	773	
#define			TID_NPCCHAT_CONT_WND_C2	774	
#define			TID_NPCCHAT_CONT_WND_R2	775	
#define			TID_NPCCHAT_CONT_WND_L3	776	
#define			TID_NPCCHAT_CONT_WND_C3	777	
#define			TID_NPCCHAT_CONT_WND_R3	778	
#define			TID_NPCCHAT_NAME_BOX	582	
#define			TID_NPCCHAT_NEXT_BT		583	
#define			TID_NPCCHAT_NEXT_OVR	642	
#define			TID_NPCCHAT_NEXT_CLK	643	
#define			TID_NPCCHAT_CLOSE_BT	644	
#define			TID_NPCCHAT_CLOSE_OVR	645	
#define			TID_NPCCHAT_CLOSE_CLK	646	
#define		TID_CTRL_BTN58    			584		
#define		TID_CTRL_BTN58_OVR			585		
#define		TID_CTRL_BTN58_CLK 			586		
#define		TID_CTRL_BTN58_DIS			587		
#define		TID_CTRL_BTN72				588		
#define		TID_CTRL_BTN72_OVR			589		
#define		TID_CTRL_BTN72_CLK			590		
#define		TID_CTRL_BTN72_DIS			591		
#define		TID_CTRL_BTN87				592		
#define		TID_CTRL_BTN87_OVR			593		
#define		TID_CTRL_BTN87_CLK			594		
#define			TID_CTRL_BTN87_DIS			595	
#define			TID_CTRL_BTN175				874		
#define			TID_CTRL_BTN175_OVR			875		
#define			TID_CTRL_BTN175_CLK			876		
#define			TID_CTRL_BTN175_DIS			877		
#define			TID_CTRL_BTNMIN				596		
#define			TID_CTRL_BTNMIN_OVR			597		
#define			TID_CTRL_BTNMIN_CLK			598		
#define			TID_CTRL_BTNMIN_DIS			599		
#define			TID_CTRL_BTNBOX				600		
#define			TID_CTRL_BTNBOX_OVR			601		
#define			TID_CTRL_BTNBOX_CLK			602		
#define			TID_CTRL_BTNBOX_DIS			603		
#define			TID_CTRL_BTN_X				604		
#define			TID_CTRL_BTN_X_OVR			605		
#define			TID_CTRL_BTN_X_CLK			606		
#define			TID_CTRL_BTN_X_DIS			607		
#define			TID_CTRL_SCR1_UBT			608		
#define			TID_CTRL_SCR1_UBT_OVR		609		
#define			TID_CTRL_SCR1_UBT_CLK		610		
#define			TID_CTRL_SCR1_UBT_DIS		611		
#define			TID_CTRL_SCR1_DBT			612		
#define			TID_CTRL_SCR1_DBT_OVR		613		
#define			TID_CTRL_SCR1_DBT_CLK		614		
#define			TID_CTRL_SCR1_DBT_DIS		615		
#define			TID_CTRL_SCR2_UBT			616		
#define			TID_CTRL_SCR2_UBT_OVR		617		
#define			TID_CTRL_SCR2_UBT_CLK		618		
#define			TID_CTRL_SCR2_UBT_DIS		619		
#define			TID_CTRL_SCR2_DBT			470		
#define			TID_CTRL_SCR2_DBT_OVR		471		
#define			TID_CTRL_SCR2_DBT_CLK		629		
#define			TID_CTRL_SCR2_DBT_DIS		630		
#define			TID_CTRL_SCR3_UBT			631		
#define			TID_CTRL_SCR3_UBT_OVR		632		
#define			TID_CTRL_SCR3_UBT_CLK		636		
#define			TID_CTRL_SCR3_UBT_DIS		637		
#define			TID_CTRL_SCR3_DBT			638		
#define			TID_CTRL_SCR3_DBT_OVR		639		
#define			TID_CTRL_SCR3_DBT_CLK		640		
#define			TID_CTRL_SCR3_DBT_DIS		641		
#define			TID_CTRL_SCR_BAR			669		
#define			TID_CTRL_SCR_BAR_CLK		670 	
#define			TID_CTRL_SCR_BAR_OVR		671		
#define			TID_CTRL_SCR_BAR_DIS		672		
#define			TID_CTRL_RAD_CHK1			673		
#define			TID_CTRL_RAD_CHK2			674		
#define			TID_CTRL_RAD_DIS			675		
#define			TID_CTRL_SCR_BG				676		
#define			TID_CTRL_OUTLINE_L1			677		
#define			TID_CTRL_OUTLINE_C1			678		
#define			TID_CTRL_OUTLINE_R1			679		
#define			TID_CTRL_OUTLINE_L2			680		
#define			TID_CTRL_OUTLINE_C2			681		
#define			TID_CTRL_OUTLINE_R2			682		
#define			TID_CTRL_OUTLINE_L3			683		
#define			TID_CTRL_OUTLINE_C3			684		
#define			TID_CTRL_OUTLINE_R3			685		
#define			TID_CTRL_SKILL_UPBT			686		
#define			TID_CTRL_SKILL_UPBT_OVR		687		
#define			TID_CTRL_SKILL_UPBT_CLK		688		
#define			TID_CTRL_SKILL_UPBT_TWI		689		
#define			TID_CTRL_SKILL_UPBT_DIS		690		
#define			TID_CTRL_WND_L1				700		
#define			TID_CTRL_WND_C1				701		
#define			TID_CTRL_WND_R1				702		
#define			TID_CTRL_WND_L2				703		
#define			TID_CTRL_WND_C2				704		
#define			TID_CTRL_WND_R2				705		
#define			TID_CTRL_WND_L3				706		
#define			TID_CTRL_WND_C3				707		
#define			TID_CTRL_WND_R3				708
#define			TID_CTRL_WND_NL1			2326
#define			TID_CTRL_WND_NC1			2327
#define			TID_CTRL_WND_NR1			2328		
#define			TID_CTRL_WND_NL2			2329
#define			TID_CTRL_WND_NC2			2330
#define			TID_CTRL_WND_NR2			2331		
#define			TID_CTRL_WND_NL3			2332
#define			TID_CTRL_WND_NC3			2333
#define			TID_CTRL_WND_NR3			2334
#define			TID_CTRL_WND_TITLE_BACK		878	
#define			TID_CTRL_ICON_STATUS		887	
#define			TID_CTRL_ICON_INVEN			888	
#define			TID_CTRL_ICON_LOG		889		
#define			TID_CTRL_ICON_QUEST		890		
#define			TID_CTRL_ICON_SYSTEM	891		
#define			TID_CTRL_ICON_FRIEND	929		
#define			TID_CTRL_ICON_SKILL		930		
#define			TID_CTRL_ICON_HELP		931	
#define			TID_CTRL_ICON_OPT			813		
#define			TID_CTRL_ICON_ALRAM			814	
#define			TID_CTRL_ICON_NONE			812
#define			TID_CTRL_ICON_RADIO_BROAD	3530
#define			TID_CTRL_CONTBACK_L1	1491	
#define			TID_CTRL_CONTBACK_C1	1492	
#define			TID_CTRL_CONTBACK_R1	1493	
#define			TID_CTRL_CONTBACK_L2	1494	
#define			TID_CTRL_CONTBACK_C2	1495	
#define			TID_CTRL_CONTBACK_R2	1496	
#define			TID_CTRL_CONTBACK_L3	1497	
#define			TID_CTRL_CONTBACK_C3	1498	
#define			TID_CTRL_CONTBACK_R3	1499	
#define			TID_CTRL_LINEDOT		1015	
#define			TID_CTRL_BACK			1343		
#define			TID_CTRL_TAB38				709		
#define			TID_CTRL_TAB38_OVR			710		
#define			TID_CTRL_TAB38_CLK			711		
#define			TID_CTRL_TAB38_DIS			712		
#define			TID_CTRL_TAB78				713		
#define			TID_CTRL_TAB78_OVR			714		
#define			TID_CTRL_TAB78_CLK			715		
#define			TID_CTRL_TAB78_DIS			716		
#define			TID_CTRL_SLOT_SUNKEN		717		
#define			TID_CTRL_SLOT_SWOLLEN		718		
#define			TID_CTRL_BTN_ELD			719		
#define			TID_CTRL_BTN_ELD_OVR		720		
#define			TID_CTRL_BTN_ELD_CLK		721		
#define			TID_CTRL_BTN_ELD_DIS		722		
#define			TID_CTRL_IMG_ELDTEXT90		723		
#define			TID_CTRL_IMG_ELDTEXT35		724		
#define			TID_CTRL_IMG_ELDTEXT132		725		
#define			TID_CTRL_NUM_SLASH			726		
#define			TID_CTRL_NUM0				727		
#define			TID_CTRL_NUM1				728		
#define			TID_CTRL_NUM2				729		
#define			TID_CTRL_NUM3				730		
#define			TID_CTRL_NUM4				731		
#define			TID_CTRL_NUM5				732		
#define			TID_CTRL_NUM6				733		
#define			TID_CTRL_NUM7				734		
#define			TID_CTRL_NUM8				735		
#define			TID_CTRL_NUM9				736	
#define			TID_CTRL_INFINITE			932	
#define			TID_CTRL_SLOT_BIG			933
#define			TID_CTRL_TEXT_OVER			934		
#define			TID_PVP_GATE_BACK 576
#define			TID_PVP_BTN_L	578
#define			TID_PVP_BTN_R	579
#define			TID_PVP_TITLE	577
#define			TID_PVP_SHD		580
#define			TID_PVP_TITLE_white		2961
#define			TID_PVP_BTN_M			2962

#define			TID_C_WND_ELDA_S_LWND			770		
#define			TID_C_WND_ELDA_S_CWND			771		
#define			TID_C_WND_ELDA_S_RWND			772		
#define			TID_C_WND_ELDA_M_LWND			773		
#define			TID_C_WND_ELDA_M_CWND			774		
#define			TID_C_WND_ELDA_M_RWND			775		
#define			TID_C_WND_ELDA_L_LWND			776		
#define			TID_C_WND_ELDA_L_CWND			777		
#define			TID_C_WND_ELDA_L_RWND			778		
#define			TID_C_WND_ELDA_SB_ARROW			779		
#define			TID_C_WND_ELDA_SB_LV			780		
#define			TID_C_WND_ELDA_SB_SLOTBT		781			
#define			TID_C_WND_ELDA_SB_SLOTBT_CLK	782		
#define			TID_C_WND_ELDA_SB_PLUS			783		
#define			TID_C_WND_ELDA_SB_X				784		
#define			TID_C_WND_OPT_LWND			744		
#define			TID_C_WND_OPT_CWND			745		
#define			TID_C_WND_OPT_RWND			746		
#define			TID_C_WND_OPT_SB_BAR		747		
#define			TID_C_WND_OPT_TICK			748		
#define			TID_C_WND_CLOG_LWND			749		
#define			TID_C_WND_CLOG_CWND			750		
#define			TID_C_WND_CLOG_RWND			751		
#define			TID_C_WND_CLOG_SB_BAR		752	    
#define			TID_C_WND_CLOG_LV_UP_BTN	753		
#define			TID_C_WND_CLOG_LV_ON_BTN	754		
#define			TID_C_WND_CLOG_LV_DN_BTN	755		
#define			TID_C_WND_CLOG_SUB_UP_BTN	756		
#define			TID_C_WND_CLOG_SUB_ON_BTN	799 	
#define			TID_C_WND_CLOG_SUB_DN_BTN	828 	
#define			TID_C_WND_CLOG_SELECT		905 	
#define			TID_C_WND_STATE_LWND			800	
#define			TID_C_WND_STATE_CWND			801	
#define			TID_C_WND_STATE_RWND			802	
#define			TID_C_WND_ABILITY_UP_LWND		803	
#define			TID_C_WND_ABILITY_UP_CWND		804	
#define			TID_C_WND_ABILITY_UP_RWND		805	
#define			TID_C_WND_STATE_SB_EXP_BG		806	
#define			TID_C_WND_ABUPBT				785	
#define			TID_C_WND_ABUPBT_OVR			786	
#define			TID_CTRL_FLAG_RAIN				787
#define			TID_CTRL_FLAG_MILLENA			788
#define			TID_CTRL_BLACKBACK_L1			2478 
#define			TID_CTRL_BLUEBACK_L1			2487 
#define			TID_CTRL_YSELECT_L1				2496	
#define			TID_CTRL_REDBACK_L1				2505	
#define			TID_C_WND_STATE_SB_EXPBAR		807	
#define			TID_C_WND_STATE_SB_HP_BG		808	
#define			TID_C_WND_STATE_SB_HPBAR		809	
#define			TID_C_WND_STATE_SB_SPBAR		810	
#define			TID_C_WND_STATE_SB_BAR_TW		811	
#define			TID_C_WND_QUEST_LWND			812	
#define			TID_C_WND_QUEST_CWND			813	
#define			TID_C_WND_QUEST_RWND			814	
#define			TID_C_WND_QBLACK_L1				785	
#define			TID_C_WND_QBLACK_C1				786	
#define			TID_C_WND_QBLACK_R1				787	
#define			TID_C_WND_QBLACK_L2				788	
#define			TID_C_WND_QBLACK_C2				789	
#define			TID_C_WND_QBLACK_R2				790	
#define			TID_C_WND_QBLACK_L3				791	
#define			TID_C_WND_QBLACK_C3				792	
#define			TID_C_WND_QBLACK_R3				793	
#define			TID_C_WND_QUEST_SEL				794	
#define			TID_C_WND_SYSMENU_LWND			842	
#define			TID_C_WND_SYSMENU_CWND			843	
#define			TID_C_WND_SYSMENU_RWND			844	
#define			TID_C_WND_SYSMENU_BT			869	
#define			TID_C_WND_SYSMENU_BT_OVR		870	
#define			TID_C_WND_SYSMENU_BT_CLK		873	
#define			TID_BZ_SELECTZONE_LWND				691	
#define			TID_BZ_SELECTZONE_CWND				692	
#define			TID_BZ_SELECTZONE_RWND				693	
#define			TID_BZ_SELECTZONE_BTN_NRL			694	
#define			TID_BZ_SELECTZONE_BTN_CLK			695	
#define			TID_BZ_SELECTZONE_BTNFOCUS_IMG		737	
#define			TID_BZ_SELECTGRADE_LWND				765	
#define			TID_BZ_SELECTGRADE_CWND				766	
#define			TID_BZ_SELECTGRADE_RWND				767 
#define			TID_BZ_SELECTGRADE_BTN_NRL			768 
#define			TID_BZ_SELECTGRADE_BTN_CLK			769 
#define			TID_BZ_SELECTGRADE_BTN_OVR			795 
#define			TID_BZ_SELECTGRADE_CLASS_IMG		796 
#define			TID_BZ_SELECTGRADE_PERCENTBG_IMG	797 
#define			TID_BZ_SELECTGRADE_PERCENTBAR_IMG	798 
#define			TID_C_WND_SMALL_L			642	
#define		    TID_C_WND_SMALL_C			643	
#define		    TID_C_WND_SMALL_R			644	
#define			TID_C_WND_WHOUSE_L			645	
#define			TID_C_WND_WHOUSE_C			646	
#define		    TID_C_WND_WHOUSE_R			647	
#define			TID_C_WND_REPAIR_L			648	
#define		    TID_C_WND_REPAIR_C			649	
#define		    TID_C_WND_REPAIR_R			650	
#define			TID_C_WND_STORE_L			651	
#define		    TID_C_WND_STORE_C			652	
#define		    TID_C_WND_STORE_R			653	
#define			TID_C_WND_INVEN_L			654	
#define		    TID_C_WND_INVEN_C			655	
#define		    TID_C_WND_INVEN_R			656	
#define			TID_C_WND_INVEN_LINE		657	
#define			TID_C_BGSLOT_NULL			658	
#define			TID_C_BGSLOT_WEAR_MILLENA	659	
#define			TID_C_BGSLOT_WEAR_RAIN		660	
#define			TID_C_BGSLOT_INVEN			666	
#define			TID_C_BGSLOT_REPAIR			668	
#define			TID_GAMERESULT_TOTEM_SCORE 2933




#define			TID_MAIN_CHRINFO		912		
#define			TID_MAIN_COMBO_UP_EFF	913		
#define			TID_MAIN_GAUGE_HP		915		
#define			TID_MAIN_GAUGE_MP		916		
#define			TID_MAIN_COMBO_STEP		917	
#define			TID_MAIN_COMBO_LEVEL1	914
#define			TID_MAIN_COMBO_LEVEL2		633	
#define			TID_MAIN_COMBO_LEVEL3		634 
#define			TID_MAIN_COMBO_LEVEL4		1722
#define			TID_MAIN_COMBO_LEVEL5		892	
#define			TID_MAIN_COMBO_LEVEL6		893	
#define			TID_MAIN_COMBO_ENLEVEL1		894	
#define			TID_MAIN_COMBO_ENLEVEL2		1900
#define			TID_MAIN_COMBO_ENLEVEL3		1901
#define			TID_MAIN_COMBO_ENLEVEL4		1902
#define			TID_MAIN_COMBO_ENLEVEL5		1903
#define			TID_MAIN_COMBO_ENLEVEL6		1904
#define			TID_MAIN_COMBO_LEVEL_EFF	1905
#define			TID_MAIN_COMBO_STEP_EFF		1906
#define			TID_MAIN_PC_INFO		918		
#define			TID_MAIN_PC_INFO_BOSS	1822
#define			TID_NonPlayerInfo_GlowWnd	904	
#define			TID_MAIN_PC_GAUGE_HP		919	
#define			TID_NonPCInfo_PrevHP_Gauge	1907	
#define			TID_NonPCInfo_GlowHP_Gauge	1908
#define			TID_MAIN_PC_GAUGE_MP		1909
#define			TID_MAIN_COMBO_TRANSIMG		1910
	
#define			TID_MAIN_MENUBT			920		
#define			TID_MAIN_MENUBT_OVR		921		
#define			TID_MAIN_MENUBT_CLK		922		
#define			TID_MAIN_MENU			923		
#define			TID_MAIN_WHISP			924		
#define			TID_MAIN_WHISP_OVR		925		
#define			TID_MAIN_WHISP_CLK		926		
#define			TID_MAIN_PARTY			927		
#define			TID_MAIN_PARTY_OVR		928		
#define			TID_MAIN_PARTY_CLK		950		
#define			TID_MAIN_FRIEND			951		
#define			TID_MAIN_FRIEND_OVR		1016	
#define			TID_MAIN_FRIEND_CLK		1017	
#define			TID_MAIN_FOLLOW 		1018	
#define			TID_MAIN_FOLLOW_OVR		1019	
#define			TID_MAIN_FOLLOW_CLK		1030	
#define			TID_MAIN_ATTACK 		1031	
#define			TID_MAIN_ATTACK_OVR		1032	
#define			TID_MAIN_ATTACK_CLK		1033	
#define			TID_MAIN_MAP_S			1034	
#define			TID_MAIN_MAP_L			1035	
#define	        TID_MAIN_MAPS_BT_DIS	1036	
#define	        TID_MAIN_MAPS_BT		1037	
#define	        TID_MAIN_MAPS_BT_OVR	1038	
#define	        TID_MAIN_MAPS_BT_CLK	1039	
#define			TID_MAIN_MAPL_BT_DIS	1040	
#define	        TID_MAIN_MAPL_BT		1041	
#define	        TID_MAIN_MAPL_BT_OVR	1042	
#define	        TID_MAIN_MAPL_BT_CLK	1043	
#define	        TID_MAIN_MAPF_BT_DIS	1044	
#define	        TID_MAIN_MAPF_BT		1045	
#define	        TID_MAIN_MAPF_BT_OVR	1046	
#define	        TID_MAIN_MAPF_BT_CLK	1047	
#define			TID_MAIN_SKILL			1048	
#define			TID_MAIN_NUMBER			1049	
#define			TID_MAIN_SKILL_SLOT		1050
#define			TID_MAIN_BUTTON_BG		1911
#define			TID_OPT_CASHOP_BT		2335   
#define			TID_OPT_CASHOP_BT_OVR	2336	
#define			TID_OPT_CASHOP_BT_CLK	2337
#define			TID_OPT_FRONTIER_BT		2538
#define			TID_OPT_FRONTIER_BT_OVR	2539	
#define			TID_OPT_FRONTIER_BT_CLK	2516	
#define			TID_OPT_LOG_BT			1051	
#define			TID_OPT_LOG_BT_OVR		1052	
#define			TID_OPT_LOG_BT_CLK		1053	
#define			TID_OPT_STATE_BT		1054	
#define			TID_OPT_STATE_BT_OVR	1055	
#define			TID_OPT_STATE_BT_CLK	1056	
#define			TID_OPT_SKILL_BT		1057	
#define			TID_OPT_SKILL_BT_OVR	1058	
#define			TID_OPT_SKILL_BT_CLK	1059	
#define			TID_OPT_PARTY_BT		1397	
#define			TID_OPT_PARTY_BT_OVR	1398	
#define			TID_OPT_PARTY_BT_CLK	1434	
#define			TID_OPT_INVEN_BT		1435	
#define			TID_OPT_INVEN_BT_OVR	1436	
#define			TID_OPT_INVEN_BT_CLK	1437	
#define			TID_OPT_QUEST_BT		1438	
#define			TID_OPT_QUEST_BT_OVR	1439	
#define			TID_OPT_QUEST_BT_CLK	1440	
#define			TID_OPT_SYSTEM_BT		1441	
#define			TID_OPT_SYSTEM_BT_OVR	1442	
#define			TID_OPT_SYSTEM_BT_CLK	1443	
#define			TID_OPT_HELP_BT			1444	
#define			TID_OPT_HELP_BT_OVR		1445	
#define			TID_OPT_HELP_BT_CLK		1446	
#define			TID_OPT_RADIO_BROAD_BT		3531	
#define			TID_OPT_RADIO_BROAD_BT_OVE  3532	
#define			TID_OPT_RADIO_BROAD_BT_CLK	3533	
#define			TID_MAIN_EXCH			1447	
#define			TID_MAIN_EXCH_OVR		1448	
#define			TID_MAIN_EXCH_CLK		1449	
#define			TID_MAIN_EXP_BG			907 	
#define			TID_MAIN_EXP_GAUGE		941
#define			TID_CTRL_BTN62    			2919
#define			TID_CTRL_BTN62_OVR			2920
#define			TID_CTRL_BTN62_CLK 			2921
#define			TID_CTRL_BTN62_DIS			2922

//내정보 메뉴

#define			TID_MYMAIN_MENU 			2180
#define			TID_MYMAIN_SHOP				2181
#define			TID_MYMAIN_SHOP_OVR			2182
#define			TID_MYMAIN_SHOP_CLK			2183	
#define			TID_MYMAIN_SOACTION			2184
#define			TID_MYMAIN_SOACTION_OVR		2185
#define			TID_MYMAIN_SOACTION_CLK		2186
#define			TID_MYMAIN_PVP				2187
#define			TID_MYMAIN_PVP_OVR			2188
#define			TID_MYMAIN_PVP_CLK			2189

#define			TID_NORMAL_FRAME_L1			2517
#define			TID_SMAIN_PARTY				2526
#define			TID_SMAIN_PARTY_OVR			2527
#define			TID_SMAIN_PARTY_CLK			2528
#define			TID_SMAIN_LEAGUE			2529
#define			TID_SMAIN_LEAGUE_OVR		2530
#define			TID_SMAIN_LEAGUE_CLK		2531
#define			TID_SMAIN_FRONTIER			2532
#define			TID_SMAIN_FRONTIER_OVR		2533
#define			TID_SMAIN_FRONTIER_CLK		2534

#define			TID_POPC_ITEMBTN			2338	
#define			TID_POPC_ITEMBTN_OVR		2339	
#define			TID_POPC_ITEMBTN_CLK		2340 	
 	
#define			TID_C_SKILL_SLOT_A			942		
#define			TID_C_SKILL_SLOT_P			943		
#define			TID_C_SKILL_SLOT_J			944		
#define			TID_C_SKILL_DIA_A			945     
#define			TID_C_SKILL_DIA_P			946		
#define			TID_C_SKILL_DIA_J			947		
#define			TID_C_SKILL_GAUGE_BAR		948     
#define			TID_C_SKILL_GAUGE_MASTER	949	 	
#define			TID_C_SKILL_NACT_TAB_A		1450    
#define			TID_C_SKILL_ACT_TAB_A		1451    
#define			TID_C_SKILL_NACT_TAB_P		1452	
#define			TID_C_SKILL_ACT_TAB_P		1453    
#define			TID_C_SKILL_NACT_TAB_J		1692    
#define			TID_C_SKILL_ACT_TAB_J		1693 
   
#define			TID_C_SKILL_LUPBTN_IMG1		1833    
#define			TID_C_SKILL_LUPBTN_IMG2		1834    
#define			TID_C_SKILL_ARROW_IMG		1835

#define			TID_C_WND_SKILLUP_L		579	
#define			TID_C_WND_SKILLUP_C		580	
#define			TID_C_WND_SKILLUP_R		581	
#define			TID_C_WND_SKILLUP_AR	582	
#define			TID_C_WND_SKILLUP_SLOT	583	
#define			TID_C_WND_EXCH_LWND		576 
#define		    TID_C_WND_EXCH_CWND		577	
#define			TID_C_WND_EXCH_RWND		578	
#define			TID_NonPlayerInfoMiniBar_Gauge_Back 1809
#define			TID_NonPlayerInfoMiniBar_HP_Gauge 1810
#define	    	TID_PVP_WND_BG							1806
#define	    	TID_ROOMLIST_IMG						1807
#define			TID_WAITINGCHARLIST_IMG					1808
#define			TID_MAPSELECT_IMG						1694
#define			TID_CHATWND_IMG							1695
#define			TID_USERLIST_IMG						1696
#define			TID_ROOMNAME_IMG						1697
#define			TID_GAMEMODE_IMG						1698
#define			TID_OPTION_CREATEROOM_IMG				1699
#define			TID_OPTION_WAITINGROOM_IMG				1700
#define			TID_GAMEREADY_START_BTN_DISABLE			1701
#define			TID_GAMEREADY_START_BTN_NORMAL			1702
#define			TID_GAMEREADY_START_BTN_FOCUS			1703
#define			TID_GAMEREADY_START_BTN_SELECT			1704
#define			TID_GAMEREADY_TEAM_BTN_DISABLE			1705
#define			TID_GAMEREADY_TEAM_BTN_NORMAL			1706
#define			TID_GAMEREADY_TEAM_BTN_FOCUS			1714
#define			TID_GAMEREADY_TEAM_BTN_SELECT			1715
#define			TID_GAMEREADY_GOOUT_BTN_DISABLE			1772
#define			TID_GAMEREADY_GOOUT_BTN_NORMAL			1773
#define			TID_GAMEREADY_GOOUT_BTN_FOCUS			1774
#define			TID_GAMEREADY_GOOUT_BTN_SELECT			1775
#define		  	TID_ROOMLIST_BTN_DISABLE				1776
#define			TID_ROOMLIST_BTN_NORMAL					1777
#define			TID_ROOMLIST_BTN_FOCUS					1778
#define			TID_ROOMLIST_BTN_SELECT					1779
#define			TID_CHATWND_BTN_DISABLE					1780
#define			TID_CHATWND_BTN_NORMAL					1781
#define			TID_CHATWND_BTN_FOCUS					1782
#define			TID_CHATWND_BTN_SELECT					1783
#define			TID_OPTION_SHORT_RBTN_DISABLE			1784
#define			TID_OPTION_SHORT_RBTN_NORMAL			1785
#define			TID_OPTION_SHORT_RBTN_FOCUS				1786
#define			TID_OPTION_SHORT_RBTN_SELECT			1787
#define			TID_OPTION_LONG_RBTN_DISABLE			1788
#define			TID_OPTION_LONG_RBTN_NORMAL				1789
#define			TID_OPTION_LONG_RBTN_FOCUS				1790
#define			TID_OPTION_LONG_RBTN_SELECT				1791
#define			TID_CREATEROOM_BG_LWND					1792
#define			TID_CREATEROOM_BG_CWND					1793
#define			TID_CREATEROOM_BG_RWND					1794
#define			TID_GAMEREADY_CHARSLOT_IMG				1795
#define			TID_GAMEREADY_CHARSLOT_READY_IMG		1796
#define			TID_GAMEREADY_STATELAMP_IMG_NOTHING		1797
#define			TID_GAMEREADY_STATELAMP_IMG_NORMAL		1798
#define			TID_GAMEREADY_STATELAMP_IMG_READY		1799
#define			TID_KICK_BTN_DISABLE					1800
#define			TID_KICK_BTN_NORMAL						1801
#define			TID_KICK_BTN_FOCUS						1802
#define			TID_KICK_BTN_SELECT						1803
#define			TID_COMBOBOX_LIST_IMG_TOP_LEFT			1804
#define			TID_COMBOBOX_LIST_IMG_TOP_CENTER		1805
#define			TID_COMBOBOX_LIST_IMG_TOP_RIGHT			1750
#define			TID_COMBOBOX_LIST_IMG_CENTER_LEFT		1751
#define			TID_COMBOBOX_LIST_IMG_CENTER_CENTER		1752
#define			TID_COMBOBOX_LIST_IMG_CENTER_RIGHT		1753
#define			TID_COMBOBOX_LIST_IMG_BOTTOM_LEFT		1754
#define			TID_COMBOBOX_LIST_IMG_BOTTOM_CENTER 	1755
#define			TID_COMBOBOX_LIST_IMG_BOTTOM_RIGHT		1756
#define			TID_COMBOBOX_LIST_IMG_SELECT			1757
#define			TID_LISTBTN_SELECT						1758
#define			TID_PASSWORD_ROOM_IMG					1759
#define			TID_NATIONFLAG_RAIN_IMG					1760
#define			TID_NATIONFLAG_MILLENA_IMG				1761
#define			TID_TEAMSTATE_RED_IMG					1762
#define			TID_TEAMSTATE_GREEN_IMG					1763
#define			TID_CREATEROOM_OK_CANCEL_BTN_DISABLE	1764
#define			TID_CREATEROOM_OK_CANCEL_BTN_NORMAL		1765
#define			TID_CREATEROOM_OK_CANCEL_BTN_FOCUS		1766
#define			TID_CREATEROOM_OK_CANCEL_BTN_SELECT		1767
#define			TID_OPTION_UNLIMIT_BTN_DISABLE			1768
#define			TID_OPTION_UNLIMIT_BTN_NORMAL			1769
#define			TID_OPTION_UNLIMIT_BTN_FOCUS			1770
#define			TID_OPTION_UNLIMIT_BTN_SELECT			1771
#define			TID_ROOM_CHAT_BLOCKBOX					4046
#define			TID_PVP_TEAMMARK_NO1					1811	
#define			TID_PVP_TEAMMARK_NO2					1812	
#define			TID_PVP_TEAMMARK_MILLENA				1813	
#define			TID_PVP_TEAMMARK_RAIN					1814

#define			TID_Combo_KeyZ							1815	
#define			TID_Combo_KeyX							1816	
#define			TID_Combo_KeyC							1817	
#define			TID_Combo_SKeyZ							1818	
#define			TID_Combo_SKeyX							1819    
#define			TID_Combo_SKeyC							1820    
#define			TID_Combo_KeyBg							1821	

#define			TID_GAMERESULT_WND_BG_L1				1500		
#define			TID_GAMERESULT_WND_BG_C1				1501		
#define			TID_GAMERESULT_WND_BG_R1				1502		
#define			TID_GAMERESULT_WND_BG_L2				1503		
#define			TID_GAMERESULT_WND_BG_C2				1504		
#define			TID_GAMERESULT_WND_BG_R2				1505		
#define			TID_GAMERESULT_WND_BG_L3				1506		
#define			TID_GAMERESULT_WND_BG_C3				1507		
#define			TID_GAMERESULT_WND_BG_R3				1508	
#define			TID_GAMERESULT_WIN_IMG					1824	
#define			TID_GAMERESULT_LOST_IMG					1825
#define			TID_GAMERESULT_DRAW_IMG					2235		
#define			TID_BLCK_BG								3646

#define			TID_GAMEREADY_MOVING_IMG				1828		
#define			TID_GAMEREADY_NORMAL_IMG				1829		
#define			TID_GAMESTART_MINIMIZE_IMG				1830		
#define			TID_GAMESTART_GLOW_IMG					1831		
#define			TID_GAMESTART_NORMAL_IMG				1832

// 사교동작 이모티콘 
#define			TID_ACT_REQUEST							1973	
#define			TID_ACT_BOW								1974	
#define			TID_ACT_RUDENESS						1975	
#define			TID_ACT_SHY								1976	
#define			TID_ACT_CRY								1977	
#define			TID_ACT_COMPLIMENTS						1978	
#define			TID_ACT_THANK							1979	
#define			TID_ACT_ATTACK							1980	
#define			TID_ACT_NOD								1981	
#define			TID_ACT_LIE								1982	
#define			TID_ACT_CLAP							1983	
#define			TID_ACT_HANDGREETING 					1984	
#define			TID_ACT_THERE							1985	
#define			TID_ACT_KISS							1986	
#define			TID_ACT_HAHA							1987	
#define			TID_ACT_NO								1988	
#define			TID_ACT_WAIT							1989	
#define			TID_ACT_SITDOWN							1990	
#define			TID_ACT_DANCE							1991

#define			TID_Cursor_Shop							2013
#define			TID_DB_SP_Icon							2023

//내정보 업데이트 

#define			TID_C_BGSLOT_INVEN_ONE				2820	
#define			TID_C_BGSLOT_PET_FACE				2821		
#define			TID_C_BGSLOT_WEAR_ONE				2877
// 이건 임시로 잠시 쓴다 .. ㅠㅠ 나중 삭제
#define			TID_C_BGSLOT_WEAR_MILLENA2	   2878
#define			TID_C_BGSLOT_WEAR_RAIN2		2883


#define			TID_MAIN_PETINFO			2822	
#define			TID_petInfo_Warning			2823	
#define			TID_PetInfo_GlowWnd			2824	
#define			TID_Pet_hp_red			        2825	
#define			TID_Pet_cp_green			2826
#define			TID_icon_boxbarR			2827	
#define			TID_icon_store			        2828	
#define			TID_icon_store_bright			2829
#define			TID_icon_store_dark			2830	
#define			TID_icon_emotion			2831	
#define			TID_icon_emotion_bright			2832	
#define 		TID_icon_emotion_dark			2833	
#define			TID_icon_worldwar			2834		
#define			TID_icon_worldwar_bright		2835
#define			TID_icon_worldwar_dark			2836
#define			TID_icon_pet			        2837		
#define			TID_icon_pet_bright			2838		
#define			TID_icon_pet_dark			2839		
#define			TID_icon_boxbarS			2840		
#define			TID_icon_pet_cancel			2841		
#define			TID_icon_pet_cancel_bright		2842
#define			TID_icon_pet_cancel_dark		2843
#define			TID_icon_pet_bag			2844		
#define			TID_icon_pet_bag_bright		        2845
#define			TID_icon_pet_bag_dark		        2846
#define			TID_icon_pet_emotion			2847	
#define			TID_icon_pet_emotion_bright		2848
#define			TID_icon_pet_emotion_dark		2849

//시공 찬가 
#define			TID_logo_sikongchanga			2882
#define			TID_logo_deco						2884
#define			TID_pvp_sikongchanga 			2891			
//중국 로고 
#define			TID_china_logo1					3380
#define			TID_china_pvp_logo				3381
#define			TID_china_preview_logo			3382
#define			TID_china_pvp		         	3461
//일본 로고
#define			TID_japan_logo1				3550
#define			TID_japan_pvp_logo			3551
#define			TID_japan_preview_logo		3552
#define			TID_japan_pvp_logo2			3553

//라이딩 
#define			TID_ride_blank			2892
#define			TID_ride_kicking		2893
#define			TID_C_riding_inven2		2925
#define			TID_C_riding_inven		2926
#define			TID_C_riding_inven_Top			3540	
#define			TID_C_riding_inven_Middle		3541			
#define			TID_C_riding_inven_Bottom		3542	
#define			TID_C_skin_suit					3543	
#define			TID_C_skin_helmet				3544		
#define			TID_C_skin_wing					3545	

#define			TID_skillwindow_button        3128    
#define			TID_skillwindow_button_ovr    3129    
#define			TID_skillwindow_button_clk    3130    
#define			TID_skillwindow_f			3131	
#define			TID_skill_exp				3132	
#define			TID_skill_percent			3133

///-- 중국 PK관련 이미지

#define			TID_icon_Chaos					3500   
#define			TID_icon_PKMode					3501
#define			TID_icon_PKMode_bright			3502
#define			TID_icon_PKMode_dark			3503

///-- PVP맵 토템 관련 아이콘 이미지
#define			TID_totem_icen_m		3537	
#define			TID_totem_icen_l		3538
#define			TID_totem_icen_n		3539

/// 2d 스프라이트 
#define			TID_ELDA_EFFECT_01		3683
#define 		TID_ELDA_EFFECT_BOMB_01		3694


/// 색깔별 엘다스톤 이미지
#define			TID_merge_eldda_green11	        3185
#define			TID_merge_eldda_blue11		    3186
#define			TID_merge_eldda_yellow11		3187
#define			TID_merge_eldda_red11		    3188
#define			TID_merge_eldda_violet11		3189
#define			TID_merge_eldda_purple11		3190
#define			TID_merge_eldda_indigo11		3191
#define			TID_merge_eldda_silver11		3192
#define			TID_merge_eldda_lightgreen11	3193
#define			TID_merge_eldda_orange11		3194

/// 봉인의 성지 추가된 로딩화면
#define			TID_LOADING_EVENT				4025
#define			TID_LOADING_SEAL_M1				4026
#define			TID_LOADING_SEAL_M2				4027
#define			TID_LOADING_SEAL_M3				4028


//  4028 까지 사용중인 TID 임...4029  부터 사용가능...!!!
// 282 까지 IMGID 사용중임...283 부터 사용가능...!!!
//이건 라이딩 작업 완료시 삭제해야한다 .. 
//이번 버젼의 인벤 현재 밀레나 용도로만 사용하고 있으며 잠시 레인 디파인을 사용한다 .. 
// TID_C_BGSLOT_WEAR_MILLENA2 <- 나중 에 삭제 

		
#endif __DEFINE_TID_H__
		
		 
		
		





	
