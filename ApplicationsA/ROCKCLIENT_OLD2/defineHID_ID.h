#define      HID_ID                                        0        ///-- 게임 홈페이지에서 등록한 \아이디(계정)를 입력하세요.
#define      HID_PASS                                      1        ///-- 게임 홈페이지에서 등록한 \아이디(계정)의 암호를 입력하세요.
#define      HID_LOGIN                                     2        ///-- 입력하신 아이디로 게임에 접속합니다.
#define      HID_EXIT                                      3        ///-- 게임을 종료하고, \윈도우로 나갑니다.
#define      HID_SERVER                                    11       ///-- 각 서버에 대한 간략한 정보, 오픈 날짜, 인구 수등등…
#define      HID_TRAFFIC                                   12       ///-- 해당 서버의 인구밀도를 나타냅니다. \혼잡도가 낮은 서버일수록 쾌적한 \플레이를 즐기실 수 있습니다.
#define      HID_CONNECT                                   13       ///-- 선택하신 서버에 접속을 시도합니다.
#define      HID_BACK                                      14       ///-- 메인 타이틀 화면으로 돌아갑니다.
#define      HID_CLASSPREVIEW_BTN                          21       ///-- 캐릭터가
#define      HID_CHARACTER                                 51       ///-- 캐릭터의 정보창을 엽니다. \단축키 'V'
#define      HID_SKILL                                     52       ///-- 스킬 창을 엽니다. \단축키 'S'
#define      HID_FRIENDS                                   53       ///-- 친구 창을 엽니다. \단축키 'F'
#define      HID_INVENTORY                                 54       ///-- 소지품 창을 엽니다. \단축키 'I'
#define      HID_QUEST                                     55       ///-- 퀘스트 창을 엽니다. \단축키 'Q'
#define      HID_SYSTEM                                    56       ///-- 시스템 메뉴를 호출합니다. \단축키 'CTRL+S'
#define      HID_HELP                                      57       ///-- 모험백서를 호출합니다. \단축키 'CTRL+H'
#define      HID_LOGWND                                    58       ///-- 콤보 스킬 로그창을 엽니다. \단축키 'L'
#define      HD_CASH_SHOP                                  59       ///-- 캐쉬 아이템 샵창을 엽니다. \단축키 'CTRL+C'
#define      HID_GUILD                                     60       ///-- 프론티어 창을 엽니다. \단축키 'CTRL+F'
#define      HD_S_MAGIC_ATTACK                             61       ///-- 마법공격
#define      HD_S_PHYSICAL_ATTACK                          62       ///-- 물리공격
#define      HD_S_ATTACK_SPEED                             63       ///-- 공격속도
#define      HD_S_MOVE_SPEED                               64       ///-- 이동속도
#define      HD_S_PHYSICAL_DEFENSE                         65       ///-- 물리방어
#define      HD_S_MAGIC_DEFENSE                            66       ///-- 마법방어 
#define      HD_S_STATE                                    71       ///-- 캐릭터의 현재 상태를 나타냅니다.
#define      HD_S_FAME                                     72       ///-- 명성이 높으면, 기본적으로 보다 \고급 퀘스트를 수행할 수 있습니다. \그 밖에도 여러가지 특전이 있습니다.
#define      HD_S_NAME                                     73       ///-- 캐릭터의 이름입니다.
#define      HD_S_NATION                                   74       ///-- 캐릭터가 소속된 국가입니다. \소속된 국가에 따라서 전직 가능한 \직업이 달라집니다.
#define      HD_S_GUILD                                    75       ///-- 캐릭터가 현재 소속된 길드입니다. \누르시면 길드 창을 열 수 있습니다.
#define      HD_S_GENDER                                   76       ///-- 성별은 겉모습 외에는 \차이가 없습니다.
#define      HD_S_JOB                                      77       ///-- 캐릭터의 현재 직업입니다. \직업에 따라서 배울 수 있는 \스킬이 다릅니다.
#define      HD_S_HP                                       78       ///-- HP는 캐릭터의 생명력을 나타냅니다. \HP가 0이 되면, 캐릭터는 죽습니다.
#define      HD_S_HPGRAPH                                  79       ///-- 캐릭터의 현재 HP와 최대 HP입니다.
#define      HD_S_MP                                       80       ///-- MP는 캐릭터의 정신력을 나타냅니다.
#define      HD_S_MPGRAPH                                  81       ///-- 캐릭터의 현재 MP와 최대 MP입니다.
#define      HD_S_SP                                       82       ///-- SP는 캐릭터의 기력을 나타냅니다. 
#define      HD_S_SPGRAPH                                  83       ///-- 현재 SP와 최대 SP를 나타냅니다.
#define      HD_S_POWER                                    84       ///-- 물리적인 힘을 나타냅니다. \물리 공격력에 영향을 줍니다.
#define      HD_S_DEFEND                                   85       ///-- 생명력을 나타냅니다.  \최대 HP에 영향을 줍니다.
#define      HD_S_SYMPATHY                                 86       ///-- 마법의 힘의 근원과의 교감능력을 나타냅니다\마법 공격력에 영향을 줍니다.
#define      HD_S_RESIST                                   87       ///-- 지능을 나타냅니다. \최대 MP에 영향을 줍니다.
#define      HD_S_FOCUS                                    88       ///-- 스태미너를 나타냅니다. \최대 SP에 영향을 줍니다.
#define      HD_S_REACTION                                 89       ///-- 민첩성을 나타냅니다. \여러가지 공격의 성공, 회피에 \영향을 줍니다.
#define      HD_S_CHARM                                    90       ///-- 타인에 대한 매력의 척도입니다.
#define      HD_S_LUCK                                     91       ///-- 그날의 행운수치입니다. \매일 바뀝니다.
#define      HD_S_EXPRISM                                  92       ///-- [%치, 현재 경험치 / 필요 경험치] \빛나고 있을 때, 클릭하시면 \능력치를 향상시킬 수 있습니다.
#define      HD_S_ATTPOINT                                 93       ///-- 캐릭터의 6대 기본 능력치 \(힘,체력,교감,지능,지구력,민첩)를 \향상시키는데 필요합니다.
#define      HD_S_SKPOINT                                  94       ///-- 새로운 스킬을 습득하거나, \스킬의 숙련도를 향상시키는데 필요합니다.
#define      HD_S_MOVESPEED                                95       ///-- 캐릭터의 이동속도를 나타냅니다.
#define      HD_S_WEIGHT                                   96       ///-- 캐릭터의 무게,장비의 무게를 합산된 무게입니다. 
#define      HD_I_POP_CASH_ITEM                            101      ///-- 캐쉬 아이템을 인벤창으로 가져옵니다.
#define      HD_BATTLE_SKILL                               121      ///-- 밀레나의 전투 기술 \리스트를 표시합니다.
#define      HD_MAGIC_SKILL                                122      ///-- 레인의 마법 리스트를 표시합니다.
#define      HD_NORMAL_SKILL                               123      ///-- 비전투 기술 리스트를 표시합니다.
#define      HD_SKILL_ICON                                 124      ///-- 클릭하시고, 퀵슬록에 놓으시면 \등록하실 수 있습니다.
#define      HD_SKILL_NAME                                 125      ///-- 클릭하시면, 스킬에 대한 상세 정보와 \스킬트리를 보실 수 있습니다.
#define      HD_SKILL_ATTRIBUTE_ICON                       126      ///-- 해당 스킬 속성에 대한 설명
#define      HD_ENTRY                                      127      ///-- 콤보 중에 연계기로 사용할 스킬을 \단축키에 등록합니다.
#define      HD_SKILL_LEVELUP                              128      ///-- 스킬의 숙련도를 향상시킵니다. \스킬업 포인트가 필요합니다.
#define      HD_NORMAL_CHAT                                131      ///-- 일반적인 채팅 메시지입니다.
#define      HD_WHISPER                                    132      ///-- 귓속말을 보내려는 상대의 \이름을 입력하세요.
#define      HD_FREINDS_CHAT                               133      ///-- 친구들에게만 전해지는 귓속말입니다. \친구목록은 친구창에서 편집하실 수 있습니다.
#define      HD_PARTY_CHT                                  134      ///-- 현재 파티원들에게만 전해지는 메시지입니다.
#define      HD_GUILD_CHAT                                 135      ///-- 프론티어원들에게만 전해지는 메시지입니다.
#define      HD_SIZE_ARROW                                 136      ///-- 채팅창의 사이즈를 조절합니다.
#define      HD_EQ_SLOT                                    137      ///-- 확장 퀵슬롯
#define      HD_PARTY_RNAME                                181      ///-- 우측클릭으로 파티이름을 \변경할수 있습니다.
#define      HD_PARTY_LEVEL                                182      ///-- 파티의 평균 레벨입니다.
#define      HD_PARTY_GOAWAY                               183      ///-- 파티원을 강퇴합니다.
#define      HD_PARTY_IN                                   184      ///-- 파티원을 추가합니다.
#define      HD_ITEM_FREE                                  185      ///-- 아이템을 무작위로 가집니다.
#define      HD_ITEM_SEQ                                   186      ///-- 아이템을 순서대로 가집니다.
#define      HD_ITEM_CON                                   187      ///-- 아이템을 국가룰로 가집니다. \현재 지원하지 않습니다.
#define      HD_ITEM_VIEW                                  188      ///-- 아이템 루팅메뉴.
#define      HD_PBREAKBTN                                  189      ///-- 파티탈퇴
#define      HD_PMIN                                       190      ///-- 최소화
#define      HD_PMAX                                       191      ///-- 원래대로
#define      HD_VIEW_LMEM                                  192      ///-- 리그전체 멤버보기
#define      HD_LEAGUE_RENAME                              193      ///-- 우측클릭으로 리그이름을 \변경할수 있습니다.
#define      HD_LEAGUE_IN                                  194      ///-- 리그원을 추가합니다.
#define      HD_LPBREAKBTN                                 195      ///-- 리그탈퇴
#define      HD_F_FRIEND                                   201      ///-- 친구창
#define      HD_F_GUILD                                    202      ///-- 프론티어창
#define      HD_F_BLOCK                                    203      ///-- 차단창
#define      HD_F_SETTING                                  204      ///-- 설정창
#define      HD_UP_POWER                                   211      ///-- 캐릭터의 물리적인 힘을 나타냅니다. \물리 공격력과 필살기의 위력에 영향을 줍니다.
#define      HD_UP_DEFEND                                  212      ///-- 캐릭터의 체력을 나타냅니다. \최대 HP와 물리, 마법 방어력에 영향을 줍니다.
#define      HD_UP_SYMPATHY                                213      ///-- 마법의 힘의 근원과의 교감능력을 나타냅니다. \마법 공격력과 마법의 위력에 영향을 줍니다.
#define      HD_UP_RESIST                                  214      ///-- 캐릭터의 지능을 나타냅니다. \최대 MP와 마법 방어력에 영향을 줍니다.
#define      HD_UP_FOCUS                                   215      ///-- 캐릭터의 지구력을 나타냅니다. \최대 SP와 물리 방어력에 영향을 줍니다.
#define      HD_UP_REACTION                                216      ///-- 캐릭터의 민첩성을 나타냅니다. \모든 공격의 명중과 회피에 영향을 줍니다.
#define      HD_UP_REALLOCATION                            217      ///-- 능력치에 배분했었던 능력업 \포인트를 모두 되돌립니다.
#define      HD_LOGOFF                                     221      ///-- 로그인 화면(게임 시작화면)으로 \돌아갑니다.
#define      HD_QUIT                                       222      ///-- 게임을 종료하고, 윈도우로 \돌아갑니다.
#define      HD_SCREEN_SHOT                                223      ///-- 지금 화면을 시공찬가실행 \폴더에 저장합니다. \단축키 'F12'
#define      HD_OPTION                                     224      ///-- 여러가지 설정을 변경하실 수 \있습니다.
#define      HD_CLOSE_WINDOWS                              225      ///-- 창을닫습니다.
#define      HD_GOOUT                                      226      ///-- 관람모드에서 밖으로 나갑니다.
#define      HD_ELD                                        241      ///-- 버튼을 클릭하시면, 거래할 돈의 \금액을 조절하실 수 있습니다.
#define      HD_GIVE_ITEM                                  242      ///-- 상대방에게 줄 아이템을 \이곳으로 끌어다 놓으십시오.
#define      HD_TAKE_ITEM                                  243      ///-- 상대방에게서 받을 아이템을 \확인하십시오.
#define      HD_CONFIRM                                    244      ///-- 거래를 실행합니다. 아이템을 이동한 \직후에 곧바로 누르실 수 없습니다.
#define      HD_CANCEL                                     245      ///-- 거래를 즉시 취소합니다.
#define      HD_MWHISPER                                   251      ///-- 귓속말
#define      HD_MPARTY                                     252      ///-- 초대하기
#define      HD_MFRIEND                                    253      ///-- 친구초대
#define      HD_MEXCHANGE                                  254      ///-- 교환하기
#define      HD_MCHASE                                     255      ///-- 따라가기
#define      HD_MATTACK                                    256      ///-- 공격하기
#define      HD_SPARTY                                     267      ///-- 파티초대
#define      HD_SLEAGUE                                    268      ///-- 리그초대
#define      HD_SFRONTIER                                  269      ///-- 프론티어초대
#define      HD_MCBREAKBTN                                 261      ///-- 대화종료
#define      HD_MCMIN                                      262      ///-- 최소화
#define      HD_MCMAX                                      263      ///-- 원래대로
#define      HD_MNORMAL_CHAT                               271      ///-- 일반채팅
#define      HD_MGUILD_CHAT                                272      ///-- 프론티어채팅
#define      HD_MPARTY_CHAT                                273      ///-- 파티/리그채팅
#define      HD_MWHISPER_CHAT                              274      ///-- 귓속말
#define      HD_MSOCIAL_ACT_BTN                            275      ///-- 사교동작
#define      HD_MTCHANNELC_BTN                             276      ///-- 채널채팅
#define      HD_MTSETVERC_BTN                              277      ///-- 서버채팅
#define      HID_ACT_REQUEST                               281      ///-- /간청
#define      HID_ACT_BOW                                   282      ///-- /경례
#define      HID_ACT_RUDENESS                              283      ///-- /메롱
#define      HID_ACT_LIE                                   284      ///-- /눕기
#define      HID_ACT_CLAP                                  285      ///-- /박수
#define      HID_ACT_HANDGREETING                          286      ///-- /안녕
#define      HID_ACT_SHY                                   287      ///-- /수줍
#define      HID_ACT_CRY                                   288      ///-- /엉엉
#define      HID_ACT_COMPLIMENTS                           289      ///-- /인사
#define      HID_ACT_THERE                                 290      ///-- /저기
#define      HID_ACT_DANCE                                 291      ///-- /춤
#define      HID_ACT_KISS                                  292      ///-- /키스
#define      HID_ACT_HAHA                                  293      ///-- /하하
#define      HID_ACT_THANK                                 294      ///-- /감사
#define      HID_ACT_ATTACK                                295      ///-- /공격
#define      HID_ACT_NOD                                   296      ///-- /응
#define      HID_ACT_NO                                    297      ///-- /아니
#define      HID_ACT_WAIT                                  298      ///-- /대기
#define      HID_ACT_SITDOWN                               299      ///-- /앉기
#define      HID_ACT_HORSE_FOREFOOT                        300      ///-- /앞발
#define      HD_MMY_SHOP                                   400      ///-- 개인상점
#define      HD_MSO_ACT                                    401      ///-- 사교동작
#define      HD_MENU                                       402      ///-- 메뉴
#define      HD_PVPZONE                                    403      ///-- 대전장
#define      HID_PET_INFO_BTN                              450      ///-- Pet 정보창. 단축키 'P'
#define      HID_PET_CALLOFF_BTN                           451      ///-- 소환해제
#define      HID_PET_RECALL_BTN                            452      ///-- 소환
#define      HID_PET_INVEN_BTN                             453      ///-- Pet 인벤토리
#define      HID_PET_EMOTION_BTN                           454      ///-- Pet 감정표현
#define      HID_PET_CREAT_BTN                             455      ///-- Pet 생성
#define      HID_PET_CANCEL_BTN                            456      ///-- Pet 취소
#define      HID_PET_NAME_CREATE_ED                        457      ///-- Pet 이름을 입력해 주세요 .
#define      HD_S_PET_HP                                   458      ///-- HP는 펫의 생명력을 나타냅니다. \HP가 0이 되면, 펫은 죽습니다.
#define      HD_S_PET_CP                                   459      ///-- CP는 펫의 정신력을 나타냅니다.
#define      HID_TOTEMSCORE                                470      ///-- 밀레나 와 레인 의 토템보유 개수 를 나타냅니다 
#define      HID_WEAR_RIDING_SLOT                          471      ///-- 라이딩 슬롯
#define      HID_MINIGAME_STOP                             472      ///-- 미니게임을 중단하면 아이템이 사라지게 되오니 주의 하십시요. 
#define      HID_WINDOWS_MODE                              473      ///-- 적용을 하실려면 게임을 다시 실행해 주세요.
#define      HID_CHARSTAT_STR                              481      ///-- 최대스텟 은 255 까지 입니다.
#define      HID_SHADOW                                    490      ///-- 설정을 고화질로 갈수록 그래픽은 향상 되지만 게임 성능은 낮아 집니다. 
#define      HID_TEXTURE_SET                               491      ///-- 설정을 상으로 갈 수록 그래픽은 향상 되지만 게임 성능은 낮아 집니다.
#define      HID_FRAME_SET                                 492      ///-- 설정을 끄기로 설정하면 그래픽이 향상 되지만 게임 성능은 낮아 집니다. 
#define      HID_EFFECT_SET                                493      ///-- 설정을 일반으로 설정하면 그래픽이 향상 되지만 게임 성능은 낮아 집니다. 
#define      HID_DISTANTVIEW_SET                           495      ///-- 원경 지형을 끄면 게임 성능이 보다 향상 되지만 원거리에 있는 지형이 표시 되지 않습니다.
#define      HID_WEATHER_SET                               496      ///-- 날씨 표현을 끄면 게임 성능이 보다 향상 되지만 날씨 효과가 나타나지 않습니다. 
#define      HID_LIGHT_SCRREN_SET                          497      ///-- 화면에 밝음과 어두움을 조정 할 수 있습니다. 최대로 갈 수록 화면이 밝아 집니다. 
#define      HID_BLUR__SET                                 498      ///-- 보다 선명한 화면을 설정을 할수 있습니다. 
#define      HID_BGROUND_SET                               499      ///-- 배경 시야를 최대로 갈 수록 보다 시야가 넓어 집니다.
#define      HID_EFFECT_SOUND_SET                          500      ///-- 이펙트 사운드 크기를 조절 할 수 있습니다.
#define      HID_BG_SOUND_SET                              501      ///-- 배경 사운드 크기를 조절 할수 있다.
#define      HID_SHOW_ID_SET                               502      ///-- 캐릭터 아이디 표시 설정을 할수 있다.
#define      HID_HELPTIP_SET                               503      ///-- 게임내 도움말 표시를 설정 할수 있다.
#define      HID_SKILL_EFFECT_MES_SET                      504      ///-- 게임내 스킬 효력 메시지 표시를 설정 할수 있다.
#define      HID_RESOLUTION_SET                            505      ///-- 해상도 변경은 게임 재실행하면 적용됩니다.
#define      HID_TIWAN30EVENT                              506      ///-- 출석 이벤트 ( 하루에 한번만 참여가능합니다 )