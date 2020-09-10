class CfgPatches
{
	class Moj_Titans_by_Ja
	{
		units[]={};
		weapons[]={};
		requiredVersion=1;
		requiredAddons[]=
		{
			"A3_Weapons_F",
			"A3_Characters_F_BLUFOR"
		};
	};
};
class StaticTitan_MainTurret_template
{
	aggregateReflectors[]={};
	allowLauncherIn=0;
	allowLauncherOut=0;
	allowTabLock=1;
	animationSourceBody="mainTurret";
	animationSourceCamElev="camElev";
	animationSourceGun="mainGun";
	animationSourceHatch="hatchGunner";
	armorLights=0.40000001;
	body="mainTurret";
	canEject=0;
	canHideGunner=-1;
	canUseScanners=1;
	castGunnerShadow=1;
	commanding=1;
	disableSoundAttenuation=0;
	displayName="";
	dontCreateAI=0;
	ejectDeadGunner=1;
	forceHideGunner=0;
	forceNVG=0;
	gun="mainGun";
	gunBeg="Usti hlavne";
	gunEnd="Konec hlavne";
	gunnerAction="gunner_staticlauncher";
	gunnerCompartments="Compartment1";
	gunnerDoor="";
	gunnerFireAlsoInInternalCamera=1;
	gunnerForceOptics=0;
	gunnerGetInAction="";
	gunnerGetOutAction="";
	gunnerInAction="ManActTestDriver";
	gunnerLeftHandAnimName="OtocHlaven_shake";
	gunnerLeftLegAnimName="";
	gunnerName="Gunner";
	gunnerOpticsColor[]={0,0,0,1};
	gunnerOpticsEffect[]={};
	gunnerOpticsModel="\A3\Weapons_F_Beta\acc\reticle_titan.p3d";
	gunnerOpticsShowCursor=0;
	gunnerOutFireAlsoInInternalCamera=1;
	gunnerOutForceOptics=0;
	gunnerOutOpticsColor[]={0,0,0,1};
	gunnerOutOpticsEffect[]={};
	gunnerOutOpticsModel="";
	gunnerOutOpticsShowCursor=0;
	gunnerRightHandAnimName="OtocHlaven_shake";
	gunnerRightLegAnimName="";
	gunnerType="";
	gunnerUsesPilotView=0;
	hasGunner=1;
	hideWeaponsGunner=1;
	inGunnerMayFire=1;
	initCamElev=0;
	initElev=0;
	initOutElev=0;
	initOutTurn=0;
	initTurn=0;
	isCopilot=0;
	lockWhenDriverOut=0;
	lockWhenVehicleSpeed=-1;
	LODTurnedIn=-1;
	LODTurnedOut=-1;
	magazines[]=
	{
		"1Rnd_GAT_missiles",
		"1Rnd_GAT_missiles",
		"1Rnd_GAT_missiles",
		"1Rnd_GAT_missiles",
		"VTI_8Rnd_LG_scalpel"
	};
	maxCamElev=90;
	maxElev=70;
	maxHorizontalRotSpeed=1.2;
	maxOutElev=20;
	maxOutTurn=60;
	maxTurn=40;
	maxVerticalRotSpeed=1.2;
	memoryPointGun="usti hlavne";
	memoryPointGunnerOptics="look";
	memoryPointGunnerOutOptics="";
	memoryPointsGetInGunner="pos_gunner_dir";
	memoryPointsGetInGunnerDir="pos_gunner";
	memoryPointsGetInGunnerPrecise="";
	minCamElev=-90;
	minElev=-20;
	minOutElev=-4;
	minOutTurn=-60;
	minTurn=-40;
	missileBeg="spice rakety";
	missileEnd="konec rakety";
	optics=1;
	outGunnerMayFire=1;
	playerPosition=0;
	preciseGetInOut=0;
	primary=1;
	primaryGunner=1;
	primaryObserver=0;
	proxyIndex=1;
	proxyType="CPGunner";
	selectionFireAnim="";
	showAllTargets=0;
	showCrewAim=0;
	showHMD=0;
	slingLoadOperator=0;
	soundElevation[]=
	{
		"",
		0.00316228,
		1
	};
	soundServo[]=
	{
		"",
		0.00316228,
		1
	};
	stabilizedInAxes=3;
	startEngine=1;
	turretCanSee=0;
	turretFollowFreeLook=0;
	turretInfoType="RscOptics_titan";
	viewGunnerInExternal=0;
	viewGunnerShadow=1;
	viewGunnerShadowAmb=1;
	viewGunnerShadowDiff=1;
	weapons[]=
	{
        "missiles_SCALPEL",
		"missiles_titan_static"
	};
	class ViewOptics
	{
		initAngleX=0;
		minAngleX=-30;
		maxAngleX=30;
		initAngleY=0;
		minAngleY=-100;
		maxAngleY=100;
		initFov=0.083329998;
		minFov=0.041669998;
		maxFov=0.083329998;
		visionMode[]=
		{
			"Normal",
			"Ti"
		};
		thermalMode[]={0,1};
		minMoveX=0;
		maxMoveX=0;
		minMoveY=0;
		maxMoveY=0;
		minMoveZ=0;
		maxMoveZ=0;
	};
	class HitPoints
	{
		class HitGun
		{
			armor=0.30000001;
			material=-1;
			name="action";
			visual="autonomous_unhide";
			passThrough=0;
			radius=0.2;
		};
		class HitTurret
		{
			class DestructionEffects
			{
				class Smoke
				{
					simulation="particles";
					type="WeaponWreckSmoke";
					position="destructionEffect";
					intensity=1;
					interval=1;
					lifeTime=5;
				};
			};
			armor=0.30000001;
			material=-1;
			name="action";
			visual="autonomous_unhide";
			passThrough=0;
			radius=0.2;
		};
	};
	class Components
	{
		class VehicleSystemsDisplayManagerComponentLeft
		{
			class Components
			{
				class EmptyDisplay
				{
					componentType="EmptyDisplayComponent";
				};
				class MinimapDisplay
				{
					componentType="MinimapDisplayComponent";
					resource="RscCustomInfoMiniMap";
				};
				class UAVDisplay
				{
					componentType="UAVFeedDisplayComponent";
				};
                class MissileDisplay
                {
                    componentType = "TransportFeedDisplayComponent";
                    source = "Missile";
                };
            };
			componentType="VehicleSystemsDisplayManager";
			left=1;
			defaultDisplay="EmptyDisplay";
			x="(profilenamespace getvariable [""IGUI_GRID_CUSTOMINFOLEFT_X"",	(safezoneX + 0.5 * 			(			((safezoneW / safezoneH) min 1.2) / 40))])";
			y="(profilenamespace getvariable [""IGUI_GRID_CUSTOMINFOLEFT_Y"",	(safezoneY + safezoneH - 21 * 			(			(			((safezoneW / safezoneH) min 1.2) / 1.2) / 25))])";
		};
		class VehicleSystemsDisplayManagerComponentRight
		{
			class Components
			{
				class EmptyDisplay
				{
					componentType="EmptyDisplayComponent";
				};
				class MinimapDisplay
				{
					componentType="MinimapDisplayComponent";
					resource="RscCustomInfoMiniMap";
				};
				class UAVDisplay
				{
					componentType="UAVFeedDisplayComponent";
				};
                class MissileDisplay
                {
                    componentType = "TransportFeedDisplayComponent";
                    source = "Missile";
                };
			};
			componentType="VehicleSystemsDisplayManager";
			right=1;
			defaultDisplay="EmptyDisplay";
			x="(profilenamespace getvariable [""IGUI_GRID_CUSTOMINFORIGHT_X"",	((safezoneX + safezoneW) - (		(10 * 			(			((safezoneW / safezoneH) min 1.2) / 40)) + 0.5 * 			(			((safezoneW / safezoneH) min 1.2) / 40)))])";
			y="(profilenamespace getvariable [""IGUI_GRID_CUSTOMINFORIGHT_Y"",	(safezoneY + safezoneH - 21 * 			(			(			((safezoneW / safezoneH) min 1.2) / 1.2) / 25))])";
		};
	};
	class ViewGunner
	{
		initAngleX=5;
		minAngleX=-75;
		maxAngleX=85;
		initAngleY=0;
		minAngleY=-150;
		maxAngleY=150;
		minFov=0.25;
		maxFov=1.25;
		initFov=0.75;
		minMoveX=0;
		maxMoveX=0;
		minMoveY=0;
		maxMoveY=0;
		minMoveZ=0;
		maxMoveZ=0;
		continuous=0;
		speedZoomMaxSpeed=1e+010;
		speedZoomMaxFOV=0;
	};
	class TurretSpec
	{
		showHeadPhones=0;
	};
	class Reflectors
	{
	};
	class GunFire
	{
		access=0;
		cloudletDuration=0.2;
		cloudletAnimPeriod=1;
		cloudletSize=1;
		cloudletAlpha=1;
		cloudletGrowUp=0.2;
		cloudletFadeIn=0.0099999998;
		cloudletFadeOut=0.5;
		cloudletAccY=0;
		cloudletMinYSpeed=-100;
		cloudletMaxYSpeed=100;
		cloudletShape="cloudletFire";
		cloudletColor[]={1,1,1,0};
		interval=0.0099999998;
		size=3;
		sourceSize=0.5;
		timeToLive=0;
		initT=4500;
		deltaT=-3000;
		class Table
		{
			class T0
			{
				maxT=0;
				color[]={0.81999999,0.94999999,0.93000001,0};
			};
			class T1
			{
				maxT=200;
				color[]={0.75,0.76999998,0.89999998,0};
			};
			class T2
			{
				maxT=400;
				color[]={0.56,0.62,0.67000002,0};
			};
			class T3
			{
				maxT=600;
				color[]={0.38999999,0.46000001,0.47,0};
			};
			class T4
			{
				maxT=800;
				color[]={0.23999999,0.31,0.31,0};
			};
			class T5
			{
				maxT=1000;
				color[]={0.23,0.31,0.28999999,0};
			};
			class T6
			{
				maxT=1500;
				color[]={0.20999999,0.28999999,0.27000001,0};
			};
			class T7
			{
				maxT=2000;
				color[]={0.19,0.23,0.20999999,0};
			};
			class T8
			{
				maxT=2300;
				color[]={0.22,0.19,0.1,0};
			};
			class T9
			{
				maxT=2500;
				color[]={0.34999999,0.2,0.02,0};
			};
			class T10
			{
				maxT=2600;
				color[]={0.62,0.28999999,0.029999999,0};
			};
			class T11
			{
				maxT=2650;
				color[]={0.58999997,0.34999999,0.050000001,0};
			};
			class T12
			{
				maxT=2700;
				color[]={0.75,0.37,0.029999999,0};
			};
			class T13
			{
				maxT=2750;
				color[]={0.88,0.34,0.029999999,0};
			};
			class T14
			{
				maxT=2800;
				color[]={0.91000003,0.5,0.17,0};
			};
			class T15
			{
				maxT=2850;
				color[]={1,0.60000002,0.2,0};
			};
			class T16
			{
				maxT=2900;
				color[]={1,0.70999998,0.30000001,0};
			};
			class T17
			{
				maxT=2950;
				color[]={0.98000002,0.82999998,0.41,0};
			};
			class T18
			{
				maxT=3000;
				color[]={0.98000002,0.91000003,0.54000002,0};
			};
			class T19
			{
				maxT=3100;
				color[]={0.98000002,0.99000001,0.60000002,0};
			};
			class T20
			{
				maxT=3300;
				color[]={0.95999998,0.99000001,0.72000003,0};
			};
			class T21
			{
				maxT=3600;
				color[]={1,0.98000002,0.91000003,0};
			};
			class T22
			{
				maxT=4200;
				color[]={1,1,1,0};
			};
		};
	};
	class GunClouds
	{
		access=0;
		cloudletDuration=0.30000001;
		cloudletAnimPeriod=1;
		cloudletSize=1;
		cloudletAlpha=1;
		cloudletGrowUp=1;
		cloudletFadeIn=0.0099999998;
		cloudletFadeOut=1;
		cloudletAccY=0.40000001;
		cloudletMinYSpeed=0.2;
		cloudletMaxYSpeed=0.80000001;
		cloudletShape="cloudletClouds";
		cloudletColor[]={1,1,1,0};
		interval=0.050000001;
		size=3;
		sourceSize=0.5;
		timeToLive=0;
		initT=0;
		deltaT=0;
		class Table
		{
			class T0
			{
				maxT=0;
				color[]={1,1,1,0};
			};
		};
	};
	class MGunClouds
	{
		access=0;
		cloudletGrowUp=0.050000001;
		cloudletFadeIn=0;
		cloudletFadeOut=0.1;
		cloudletDuration=0.050000001;
		cloudletAnimPeriod=1;
		cloudletSize=1;
		cloudletAlpha=0.30000001;
		cloudletAccY=0;
		cloudletMinYSpeed=-100;
		cloudletMaxYSpeed=100;
		cloudletShape="cloudletClouds";
		cloudletColor[]={1,1,1,0};
		timeToLive=0;
		interval=0.02;
		size=0.30000001;
		sourceSize=0.02;
		initT=0;
		deltaT=0;
		class Table
		{
			class T0
			{
				maxT=0;
				color[]={1,1,1,0};
			};
		};
	};
	class Turrets
	{
	};
	class TurnIn
	{
		turnOffset=0;
	};
	class TurnOut
	{
		turnOffset=0;
	};
};
class CfgWeapons
{
	class launch_B_Titan_short_F;
	class missiles_titan_static;
	class missiles_SCALPEL;
	class VTI_launch_Titan_short_saclos_Sand_F: launch_B_Titan_short_F
	{
		magazines[]=
		{
			"VTI_Titan_AT_saclos",
			"Titan_AP"
		};
		displayName="Titan MPRL SACLOS (Sand)";
		hiddenSelectionsTextures[]=
		{
			"A3\Weapons_F_Beta\Launchers\Titan\Data\Launcher_CO.paa",
			"A3\Weapons_F_Beta\Launchers\Titan\Data\TubeM_CO.paa"
		};
		picture="\A3\Weapons_F_Beta\Launchers\Titan\Data\UI\gear_titan_short_CA.paa";
	};
	class VTI_launch_Titan_short_saclos_Tropic_F: VTI_launch_Titan_short_saclos_Sand_F
	{
		displayName="Titan MPRL SACLOS (Tropic)";
		hiddenSelectionsTextures[]=
		{
			"\A3\Weapons_F_Exp\Launchers\Titan\Data\launch_B_Titan_tna_F_01_co.paa",
			"\A3\Weapons_F_Exp\Launchers\Titan\Data\launch_B_Titan_short_tna_F_02_co.paa"
		};
		picture="\A3\Weapons_F_Exp\Launchers\Titan\Data\UI\launch_B_Titan_short_tna_F_X_CA.paa";
	};
	class VTI_launch_Titan_short_saclos_Olive_F: VTI_launch_Titan_short_saclos_Sand_F
	{
		displayName="Titan MPRL SACLOS (Olive)";
		hiddenSelectionsTextures[]=
		{
			"A3\Weapons_F_Beta\Launchers\Titan\Data\Launcher_INDP_CO.paa",
			"A3\Weapons_F_Beta\Launchers\Titan\Data\TubeM_INDP_CO.paa"
		};
		picture="\A3\Weapons_F_Beta\Launchers\Titan\Data\UI\gear_titan_short_CA.paa";
	};
	class VTI_launch_Titan_short_saclos_Coyote_F: VTI_launch_Titan_short_saclos_Sand_F
	{
		displayName="Titan MPRL SACLOS (Coyote)";
		hiddenSelectionsTextures[]=
		{
			"A3\Weapons_F_Beta\Launchers\Titan\Data\Launcher_OPFOR_CO.paa",
			"A3\Weapons_F_Beta\Launchers\Titan\Data\TubeM_OPFOR_CO.paa"
		};
		picture="\A3\Weapons_F_Beta\Launchers\Titan\Data\UI\gear_titan_short_CA.paa";
	};
	class VTI_launch_Titan_short_saclos_GreenHex_F: VTI_launch_Titan_short_saclos_Sand_F
	{
		displayName="Titan MPRL SACLOS (Green Hex)";
		hiddenSelectionsTextures[]=
		{
			"\A3\Weapons_F_Exp\Launchers\Titan\Data\launch_O_Titan_ghex_F_01_co.paa",
			"\A3\Weapons_F_Exp\Launchers\Titan\Data\launch_O_Titan_short_ghex_F_02_co.paa"
		};
		picture="\A3\Weapons_F_Exp\Launchers\Titan\Data\UI\launch_O_Titan_short_ghex_F_X_CA.paa";
	};
	class VTI_missiles_titan_static_saclos: missiles_titan_static
	{
		displayName="Titan Missile (SACLOS)";
		magazines[]=
		{
			"VTI_1Rnd_GAT_missiles_saclos"
		};
	};
    class VTI_missiles_SCALEPEL_static_saclos: missiles_titan_static
	{
		displayName="SCALPEL Missile (TVG)";
		magazines[]=
		{
			"VTI_8Rnd_LG_scalpel"
		};
	};
};
class CfgVehicles
{
	class B_static_AT_F;
	class B_T_Static_AT_F;
	class O_static_AT_F;
	class I_static_AT_F;
	class VTI_B_static_AT_saclos_F: B_static_AT_F
	{
		displayname="Static Titan Launcher (AT, KL) [NATO]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos"
				};
				weapons[]=
				{
					"VTI_missiles_titan_static_saclos"
				};
			};
		};
	};
	class VTI_B_T_static_AT_saclos_F: B_T_Static_AT_F
	{
		displayname="Static Titan Launcher (AT, KL) [NATO]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos"
				};
				weapons[]=
				{
					"VTI_missiles_titan_static_saclos"
				};
			};
		};
	};
	class VTI_O_static_AT_saclos_F: O_static_AT_F
	{
		displayname="Static Titan Launcher (AT, KL) [CSAT]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos"
				};
				weapons[]=
				{
					"VTI_missiles_titan_static_saclos"
				};
			};
		};
	};
	class VTI_I_static_AT_saclos_F: I_static_AT_F
	{
		displayname="Static Titan Launcher (AT, KL) [AAF]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos",
					"VTI_1Rnd_GAT_missiles_saclos"
				};
				weapons[]=
				{
					"VTI_missiles_titan_static_saclos"
				};
			};
		};
	};
		class VTI_B_static_AT_saclos_F_scalpel: B_static_AT_F
	{
		displayname="Static Titan Launcher (AT, Scalpel, KL) [NATO]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_8Rnd_LG_scalpel"
				};
				weapons[]=
				{
					"VTI_missiles_SCALEPEL_static_saclos"
				};
			};
		};
	};
	class VTI_B_T_static_AT_saclos_F_scalpel: B_T_Static_AT_F
	{
		displayname="Static Titan Launcher (AT, Scalpel, KL) [NATO]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_8Rnd_LG_scalpel"
				};
				weapons[]=
				{
					"VTI_missiles_SCALEPEL_static_saclos"
				};
			};
		};
	};
	class VTI_O_static_AT_saclos_F_scalpel: O_static_AT_F
	{
		displayname="Static Titan Launcher (AT, Scalpel, KL) [CSAT]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_8Rnd_LG_scalpel"
				};
				weapons[]=
				{
					"VTI_missiles_SCALEPEL_static_saclos"
				};
			};
		};
	};
	class VTI_I_static_AT_saclos_F_scalpel: I_static_AT_F
	{
		displayname="Static Titan Launcher (AT, Scalpel, KL) [AAF]";
		class Turrets
		{
			class MainTurret: StaticTitan_MainTurret_template
			{
				magazines[]=
				{
					"VTI_8Rnd_LG_scalpel"
				};
				weapons[]=
				{
					"VTI_missiles_SCALEPEL_static_saclos"
				};
			};
		};
	};
};
class CfgMagazines
{
    class Default;
	class CA_Magazine: Default
	{
		author="$STR_A3_Bohemia_Interactive";
		scope=1;
		value=1;
		displayName="";
		model="\A3\weapons_F\ammo\mag_univ.p3d";
		picture="";
		modelSpecial="";
		useAction=0;
		useActionTitle="";
		reloadAction="";
		ammo="";
		count=30;
		type=256;
		initSpeed=900;
		selectionFireAnim="zasleh";
		nameSound="magazine";
		maxLeadSpeed=25;
		weaponpoolavailable=1;
		mass=8;
	};
	class VehicleMagazine: CA_Magazine
	{
		author="$STR_A3_Bohemia_Interactive";
		type=0;
		reloadAction="";
		maxLeadSpeed=100;
		weaponPoolAvailable=0;
	};
	class Titan_AT;
	class 1Rnd_GAT_missiles;
	class VTI_Titan_AT_saclos: Titan_AT
	{
		ammo="MOJ_M_Titan_AT";
		descriptionShort="Type: Anti-Tank, KL<br />Rounds: 1<br />Used in: Titan MPRL Compact";
		displayName="Titan AT Missile (KL)";
		displayNameShort="AT";
	};
	class VTI_1Rnd_GAT_missiles_saclos: 1Rnd_GAT_missiles
	{
		ammo="MOJ_M_Titan_AT_static";
	};
    class VTI_8Rnd_LG_scalpel: VehicleMagazine
	{
		author="$STR_A3_Bohemia_Interactive";
		scope=2;
		displayName="$STR_A3_CfgMagazines_8Rnd_LG_scalpel0";
		displayNameShort="$STR_A3_CfgMagazines_8Rnd_LG_scalpel_dns";
		descriptionShort="$STR_A3_cfgMagazine_Scalpel_tooltip";
		ammo="MOJ_M_Scalpel_AT";
		initSpeed=0;
		count=8;
		nameSound="missiles";
		weight=400;
	};
};
class SensorTemplatePassiveRadar;
class SensorTemplateAntiRadiation;
class SensorTemplateActiveRadar;
class SensorTemplateIR;
class SensorTemplateVisual;
class SensorTemplateMan;
class SensorTemplateLaser;
class SensorTemplateNV;
class SensorTemplateDataLink;
class CfgAmmo
{
    class MissileCore;
	class MissileBase: MissileCore
	{
		manualControl=0;
		maneuvrability=20;
		maxSpeed=500;
		simulationStep=0.0099999998;
		airFriction=0.2;
		sideAirFriction=0.001;
		maxControlRange=18000;
		soundHit1[]=
		{
			"A3\Sounds_F\arsenal\weapons\Launchers\Titan\Explosion_titan_missile_01",
			2.5118864,
			1,
			2000
		};
		soundHit2[]=
		{
			"A3\Sounds_F\arsenal\weapons\Launchers\Titan\Explosion_titan_missile_02",
			2.5118864,
			1,
			2000
		};
		soundHit3[]=
		{
			"A3\Sounds_F\arsenal\weapons\Launchers\Titan\Explosion_titan_missile_03",
			2.5118864,
			1,
			2000
		};
		multiSoundHit[]=
		{
			"soundHit1",
			0.34,
			"soundHit2",
			0.33000001,
			"soundHit3",
			0.33000001
		};
		explosionSoundEffect="DefaultExplosion";
		soundFly[]=
		{
			"",
			1,
			1,
			400
		};
		soundEngine[]=
		{
			"",
			1,
			1,
			50
		};
		supersonicCrackNear[]=
		{
			"A3\Sounds_F\weapons\Explosion\supersonic_crack_close",
			0.39810717,
			1,
			20
		};
		supersonicCrackFar[]=
		{
			"A3\Sounds_F\weapons\Explosion\supersonic_crack_50meters",
			0.31622776,
			1,
			50
		};
		CraterEffects="ATMissileCrater";
		explosionEffects="ATMissileExplosion";
		muzzleEffect="BIS_fnc_effectFiredRocket";
		effectsMissile="missile4";
		deflecting=0;
		weaponLockSystem="2 + 16";
		cmImmunity=0.89999998;
		dangerRadiusHit=-1;
		suppressionRadiusHit=30;
		class HitEffects
		{
			hitWater="ImpactEffectsWaterRocket";
		};
		class Components;
		maverickWeaponIndexOffset=0;
		htMin=60;
		htMax=1800;
		afMax=200;
		mfMax=100;
		mFact=0;
		tBody=0;
	};

    class MOJ_M_Titan_AT: MissileBase
	{
		model="\A3\Weapons_F_beta\Launchers\titan\titan_missile_at_fly";
		hit=200;
		indirectHit=100;
		indirectHitRange=8;
		explosive=1;
		warheadName="HE";
		submunitionAmmo="ammo_Penetrator_Titan_AT";
		submunitionDirectionType="SubmunitionModelDirection";
		submunitionInitSpeed=1000;
		submunitionParentSpeedCoef=0;
		submunitionInitialOffset[]={0,0,-0.2};
		triggerOnImpact=1;
		deleteParentWhenTriggered=0;
		cost=500;
		aiAmmoUsageFlags="128 + 512";
		explosionSoundEffect="DefaultExplosion";
		effectsMissileInit="";
		muzzleEffect="";
		simulationStep=0.0020000001;
		initTime=0.25;
		trackOversteer=1.5;
		trackLead=0.89999998;
		timeToLive=22;
		maneuvrability=20;
		airFriction=0.085000001;
		sideAirFriction=1;
		maxSpeed=120;
		typicalSpeed=100;
		thrustTime=5;
		thrust=45;
		fuseDistance=50;
		effectsMissile="missile2";
		whistleDist=4;
		airLock=1;
		lockType=0;
		missileLockCone=4.5;
		missileKeepLockedCone=160;
		missileLockMaxDistance=5000;
		missileLockMinDistance=50;
		missileLockMaxSpeed=35;
		manualControl=0;
		laserLock=0;
        cameraViewAvailable=1;
		missileManualControlCone=45;
		maxControlRange=18000;
		weaponLockSystem="2 + 16";
		cmImmunity=0.40000001;
		flightProfiles[]=
		{
			"Direct",
			"TopDown"
		};
		class Direct
		{
		};
		class TopDown
		{
			ascendHeight=150;
			descendDistance=180;
			minDistance=180;
			ascendAngle=30;
		};
		class Components: Components
		{
			class SensorsManagerComponent
			{
				class Components
				{
					class IRSensorComponent: SensorTemplateIR
					{
						class AirTarget
						{
							minRange=500;
							maxRange=18000;
							objectDistanceLimitCoef=-1;
							viewDistanceLimitCoef=1;
						};
						class GroundTarget
						{
							minRange=500;
							maxRange=18000;
							objectDistanceLimitCoef=1;
							viewDistanceLimitCoef=1;
						};
						maxTrackableSpeed=35;
						angleRangeHorizontal=3.7;
						angleRangeVertical=2.3;
						maxTrackableATL=50;
					};
				};
			};
		};
		soundFly[]=
		{
			"A3\Sounds_F\arsenal\weapons\Launchers\Titan\Fly_Titan",
			0.63095737,
			1.5,
			300
		};
		class CamShakeExplode
		{
			power=11;
			duration=1.4;
			frequency=20;
			distance=91.329597;
		};
		class CamShakeHit
		{
			power=110;
			duration=0.60000002;
			frequency=20;
			distance=1;
		};
		class CamShakeFire
		{
			power=2.78316;
			duration=1.6;
			frequency=20;
			distance=61.967701;
		};
		class CamShakePlayerFire
		{
			power=3;
			duration=0.1;
			frequency=20;
			distance=1;
		};
	};
	class MOJ_M_Titan_AT_static: MOJ_M_Titan_AT
	{
		effectsMissileInit="RocketBackEffectsStaticRPG";
		initTime=0.1;
	};
    class MOJ_M_Scalpel_AT: MissileBase
	{
		hit=200;
		indirectHit=100;
		indirectHitRange=8;
		warheadName="TandemHEAT";
		submunitionAmmo="ammo_Penetrator_Scalpel";
		submunitionDirectionType="SubmunitionModelDirection";
		submunitionInitSpeed=1000;
		submunitionParentSpeedCoef=0;
		submunitionInitialOffset[]={0,0,-0.2};
		triggerOnImpact=1;
		deleteParentWhenTriggered=0;
		soundHit[]=
		{
			"A3\Sounds_F\weapons\Rockets\explosion_missile_02",
			0.89125091,
			1,
			1900
		};
		model="\A3\Weapons_F\Ammo\Missile_AT_03_fly_F";
		proxyShape="\A3\Weapons_F\Ammo\Missile_AT_03_F";
		CraterEffects="ATRocketCrater";
		explosionEffects="ATRocketExplosion";
		whistleDist=8;
		maneuvrability=15;
		simulationStep=0.0020000001;
		aiAmmoUsageFlags="128 + 512";
		irLock=0;
		nvLock=0;
		laserLock=0;
		cost=300;
        maxSpeed=120;
		typicalSpeed=100;
		timeToLive=28;
		airFriction=0.103;
		sideAirFriction=0.18000001;
		trackLead=1;
		trackOversteer=1.5;
		initTime=0;
		thrustTime=5;
		thrust=45;
		fuseDistance=500;
		muzzleEffect="BIS_fnc_effectFiredHeliRocket";
		class Hiteffects
		{
			hitWater="ImpactEffectsSmall";
		};
		cameraViewAvailable=1;
		airLock=0;
		missileLockCone=30;
		missileKeepLockedCone=60;
		missileLockMaxDistance=8000;
		missileLockMinDistance=250;
		missileLockMaxSpeed=55;
		weaponLockSystem="2 + 4 + 16";
		cmImmunity=0.44999999;
		manualControl=0;
		maxControlRange=18000;
		missileManualControlCone=120;
		soundFly[]=
		{
			"A3\Sounds_F\weapons\Rockets\rocket_fly_1",
			0.56234133,
			1.5,
			700
		};
		flightProfiles[]=
		{
			"TopDown"
		};
		class TopDown
		{
			ascendHeight=400;
			descendDistance=800;
			minDistance=800;
			ascendAngle=20;
		};
		class Components: Components
		{
			class SensorsManagerComponent
			{
				class Components
				{
					class IRSensorComponent: SensorTemplateIR
					{
						class AirTarget
						{
							minRange=500;
							maxRange=18000;
							objectDistanceLimitCoef=-1;
							viewDistanceLimitCoef=1;
						};
						class GroundTarget
						{
							minRange=500;
							maxRange=18000;
							objectDistanceLimitCoef=1;
							viewDistanceLimitCoef=1;
						};
						maxTrackableSpeed=55;
						angleRangeHorizontal=45;
						angleRangeVertical=35;
					};
					class LaserSensorComponent: SensorTemplateLaser
					{
						class AirTarget
						{
							minRange=6000;
							maxRange=18000;
							objectDistanceLimitCoef=-1;
							viewDistanceLimitCoef=-1;
						};
						class GroundTarget
						{
							minRange=6000;
							maxRange=18000;
							objectDistanceLimitCoef=-1;
							viewDistanceLimitCoef=-1;
						};
						maxTrackableSpeed=55;
						angleRangeHorizontal=90;
						angleRangeVertical=70;
					};
				};
			};
		};
	};
};
class cfgMods
{
	author="Moja malenkost";
	timepacked="1536490516";
};
