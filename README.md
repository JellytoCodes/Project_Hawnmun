Project Hawnmun

Unreal Engine 5.7과 C++ / Blueprint를 기반으로 제작한 3인칭 액션 RPG 포트폴리오 프로젝트입니다.
단순 전투 데모가 아니라 GAS 기반 전투, Behavior Tree 기반 AI, 보스 페이즈, 인벤토리/장비, 저장/로드, 던전 세션, UI, 시네마틱 연출을 하나의 플레이 가능한 루프로 통합하는 것을 목표로 했습니다.

프로젝트 정보

항목	내용
Engine	Unreal Engine 5.7
Language	C++ / Blueprint
IDE	Visual Studio 2022
Genre	Third-Person Action RPG
Role	UE5 Client Programming

개발 초점

이 프로젝트는 UE5 C++ 클라이언트 개발자로서 다음 역량을 보여주기 위해 제작했습니다.

* GAS 기반 전투 시스템을 실제 플레이 흐름에 통합
* Gameplay Tag를 이용한 입력, 상태, 보스 패턴, BGM 전환 관리
* Behavior Tree와 Gameplay Ability를 연결한 적 AI 구조 설계
* DungeonSubsystem을 통한 던전 세션, 체크포인트, 전리품 흐름 관리
* SaveGame 기반 진행 데이터 저장과 로드
* WidgetController 기반 UI 상태 관리
* Media Framework, Niagara, Chaos Cloth, Data Layer를 활용한 보스 연출 구성

핵심 기능

* Gameplay Ability System 기반 능력, 속성, Gameplay Effect, Gameplay Tag 처리
* 회피, 질주, 타겟 락, 투사체와 근접 충돌 기반 전투
* Behavior Tree와 Gameplay Ability를 연계한 적 AI
* 멧돼지와 구미호 보스의 페이즈 전환 및 전용 패턴
* 구미호 여우불, 포탈 브레스, 포탈 바이트, 환영 분신 패턴
* Motion Warping과 Root Motion을 이용한 이동 및 공격 보정
* 인벤토리, 장비, 아이템 획득과 던전 세션 전리품 관리
* 체크포인트 기반 리스폰과 저장 슬롯 기반 진행 데이터 저장
* WidgetController 기반 HUD, 인벤토리, 속성, 던전 맵, 설정 UI
* Media Framework 기반 오프닝, 조우, 페이즈 전환, 처치 시네마틱
* Gameplay Tag 기반 BGM 전환
* Niagara 이펙트와 Chaos Cloth 기반 캐릭터 연출
* World Partition Data Layer를 이용한 보스 페이즈 환경 전환

구현 완료 범위

* 타이틀에서 거점, 던전으로 이어지는 기본 게임 흐름
* 플레이어 이동, 전투, 회피, 질주, 타겟 락
* 일반 적과 보스 AI 전투
* 멧돼지 보스 조우 및 처치 흐름
* 구미호 보스 1페이즈, 2페이즈 전환, 처치 흐름
* 던전 체크포인트, 리스폰, 전리품 정산
* 저장 슬롯 기반 진행 데이터 저장과 로드
* 인벤토리, 장비, 속성, 던전 맵, 설정 UI
* 오프닝, 조우, 페이즈 전환, 처치 시네마틱 재생
* BGM 전환과 보스별 전투 음악

게임 흐름

TitleMap
  -> 새 게임 또는 세이브 데이터 로드
  -> HideoutMap
  -> 던전 선택
  -> Dungeon_01 / Dungeon_01_WP
  -> 체크포인트, 보스전, 전리품 정산
  -> HideoutMap

SandBoxMap은 전투, AI, 보스 패턴, UI 기능을 개별 검증하는 테스트 맵입니다.

주요 맵

맵	용도
TitleMap	기본 실행 맵과 타이틀 UI
OpeningMap	게임 오프닝 흐름
FirstStartMap	최초 시작 흐름
HideoutMap	플레이어 거점과 던전 진입
Dungeon_01	던전 플레이 맵
Dungeon_01_WP	World Partition 기반 던전 맵
SandBoxMap	전투 및 시스템 테스트 맵

기술 구성

GAS 기반 전투 구조

UHawnmunAbilitySystemComponent, UHawnmunAttributeSet, UHawnmunGameplayAbility를 중심으로 전투 상태를 관리합니다.

GameplayTag 입력
  -> Ability 활성화
  -> GameplayEffect Spec 생성
  -> ExecCalc_Damage에서 최종 피해 계산
  -> AttributeSet에서 체력, 스태미나, 사망, 페이즈 처리

주요 능력과 상태는 Gameplay Tag로 식별합니다.

* Ability.Rolling
* Ability.Sprint
* Ability.TargetLock
* State.Dead
* State.Rolling
* State.TargetLock
* Enemy.State.Boss.Phase2
* 보스 패턴, 시네마틱, BGM, 아이템 태그

적 AI와 보스

AHawnmunEnemy가 일반 적의 GAS 초기화, Behavior Tree 실행, 어그로, 사망, 전리품과 리셋 흐름을 담당합니다.

* AHawnmunWildBoar: 돌진과 페이즈 기반 보스 전투
* AHawnmunNineTailed: 2페이즈 전환, 로브 교체, 귀/꼬리 Niagara, Chaos Cloth, 환경 Data Layer 전환
* AHawnmunNineTailedIllusion: 환영 분신의 외형, 피격 판정과 패턴 피해 처리
* AFoxFireDropActor, APortalBreathActor, APortalBiteActor: 풀링 기반 구미호 패턴 액터

Behavior Tree Task는 Ability Tag를 사용해 GAS Ability를 실행하고 완료 상태를 AI 트리에 전달합니다.

Behavior Tree Task
  -> Ability Tag 선택
  -> AbilitySystemComponent를 통해 Ability 실행
  -> Ability 종료 시 Task 완료 처리
  -> 다음 AI 패턴으로 전환

던전과 저장

UHawnmunDungeonSubsystem은 맵 전환 이후에도 유지되어야 하는 던전 세션 상태를 관리합니다.

* 현재 던전 태그
* 세션 전리품 가방
* 체크포인트 Transform
* 던전 클리어 상태
* 던전 세션 시작 및 종료

AHawnmunGameModeBase는 레벨 이동, 저장과 로드, BGM, 어그로 적 초기화, 리스폰과 던전 결과 처리를 담당합니다.

던전 진입
  -> DungeonSubsystem 세션 시작
  -> 체크포인트 갱신
  -> 보스 처치 또는 플레이어 사망
  -> 전리품 정산
  -> HideoutMap 복귀

UI와 시네마틱

AHawnmunHUD가 Widget과 WidgetController를 생성하고 표시 상태를 관리합니다.

HUD
  -> OverlayWidgetController
  -> AttributeWidgetController
  -> InventoryWidgetController
  -> DungeonMapWidgetController
  -> SaveSlotWidgetController
  -> SettingsWidgetController
  -> CinematicWidgetController

시네마틱 재생 중에는 플레이 입력과 BGM을 중단하고, 영상 종료 후 전달받은 적의 AI 또는 보스 페이즈 흐름을 재개합니다.

프로젝트 구조

Project_Hawnmun/
|-- Config/                         # 엔진, 렌더링, 입력, 패키징 설정
|-- Content/                        # Blueprint와 Unreal 에셋
|   |-- Assets/                     # 캐릭터, 적, 이펙트, 사운드 에셋
|   |-- Blueprints/                 # 게임 로직 Blueprint
|   |-- Cinematic/                  # 시네마틱 관련 에셋
|   |-- Data/                       # DataAsset와 DataTable
|   |-- Maps/                       # 게임 및 테스트 맵
|   |-- Materials/                  # 머티리얼과 머티리얼 인스턴스
|   |-- Movies/                     # Media Framework 영상 리소스
|   |-- PCG/                        # PCG 관련 에셋
|   `-- Images/                     # UI 이미지 리소스
|-- Source/
|   |-- ProjectH/
|   |   |-- Public/
|   |   |   |-- AbilitySystem/      # GAS, Ability, Attribute, Effect 계산
|   |   |   |-- Actors/             # 투사체, 패턴, 상호작용 액터
|   |   |   |-- AI/                 # Behavior Tree Task와 Service
|   |   |   |-- AnimInstance/       # 플레이어와 적 AnimInstance
|   |   |   |-- Characters/         # 플레이어, 일반 적, 보스
|   |   |   |-- Components/         # 인벤토리와 장비
|   |   |   |-- Controllers/        # Player, AI, Camera Controller
|   |   |   |-- GameMode/           # GameMode, SaveGame, Subsystem
|   |   |   |-- HUD/                # HUD, Widget, WidgetController
|   |   |   |-- Input/              # Enhanced Input 연동
|   |   |   |-- Interfaces/         # Player, Enemy, Combat 계약
|   |   |   `-- Types/              # 공용 Enum과 Struct
|   |   `-- Private/                # Public 구조에 대응하는 구현
|   |-- ProjectH.Target.cs
|   `-- ProjectHEditor.Target.cs
|-- ProjectH.uproject
`-- README.md

주요 소스 진입점

파일	역할
HawnmunGameplayTags	프로젝트 전역 Gameplay Tag 선언과 등록
HawnmunFunctionLibrary	피해 적용과 범위 탐색 등 공용 게임 함수
HawnmunAssetManager	프로젝트 Asset Manager 초기화
HawnmunCharacterBase	GAS 캐릭터 공통 기반과 전투 충돌
HawnmunPlayer	플레이어 상태, 상호작용, 루팅, 입력 연동
HawnmunEnemy	적 AI, 사망, 어그로, 전리품 공통 처리
HawnmunGameModeBase	저장, 레벨 이동, BGM, 리스폰, 던전 결과
HawnmunDungeonSubsystem	맵 전환을 유지하는 던전 세션 데이터
HawnmunHUD	전체 게임 Widget과 Controller 관리

주요 모듈과 플러그인

런타임 모듈은 다음 기능을 사용합니다.

GameplayAbilities
GameplayTasks
GameplayTags
EnhancedInput
AIModule
Niagara
MotionWarping
UMG
MediaAssets
MetasoundEngine
ChaosClothAssetEngine

프로젝트에서는 다음 플러그인을 사용합니다.

* GameplayAbilities
* MotionWarping
* PCG
* Chaos Cloth Asset Editor
* Movie Render Pipeline
* Async Loading Screen

구현 시 고려 사항

* C++ 네이티브 컴포넌트 구조를 변경한 뒤 기존 Blueprint에 반영되지 않으면 Blueprint 상속 데이터가 남아 있을 수 있습니다. 이 경우 컴포넌트 Reset, Reparent 또는 Blueprint 재생성을 검토합니다.
* 필수 DataAsset, Ability Class, 패턴 풀 Class는 Blueprint Defaults에서 할당해야 합니다.
* 장기 수명 객체나 UGameInstanceSubsystem Delegate에 UObject를 연결할 때는 수명 차이를 고려해 weak binding 또는 DelegateHandle 해제를 사용합니다.
* 체크포인트가 없는 맵의 Retry는 기본 PlayerStart 리스폰 경로가 필요합니다.
* Chaos Cloth, Niagara, 시네마틱 Media Source 변경 후에는 실제 게임 맵과 패키징 환경에서 재검증합니다.

라이선스

Copyright (c) 2026 Project Hawnmun. All rights reserved.