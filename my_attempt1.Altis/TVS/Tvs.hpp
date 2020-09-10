#define Nux_tvs_Dialog				8560
#define Nux_Tvs_MainFrame_IDC 		8561
#define Nux_Tvs_Mousearea_IDC 		8562
#define Nux_Tvs_Optics_IDC 			8563
//#define Nux_Tvs_Crosshair_IDC		8564
#define Nux_Tvs_Flirstate_IDC		8565

class Nux_Dlg_Tvs
{
	idd = Nux_tvs_Dialog;
	moving = false;
	movingEnable = false;
	movingEnabled = false;
	onLoad="uiNamespace setVariable ['Nux_Tvs_Display', _this select 0]; Nux_tvs_mousepos = [0.5, 0.5];";
	controlsBackground[ ]={};
	objects[ ]={ };
	controls[ ]={Nux_Tvs_Mousearea,
				Nux_Tvs_MainFrame,
				Nux_Tvs_Optics,
				//Nux_Tvs_Crosshair,
				Nux_Tvs_Flirstate,
				};

	class Nux_Tvs_MainFrame
	{
		idc = Nux_Tvs_MainFrame_IDC;
		type = 0;
		style = 64;
		colorText[ ]={ 1,1,1,1 };
		colorBackground[ ]={ 1 ,1 ,1 ,1 };
		font = "PuristaSemibold";
		sizeEx = 0.05;
		lineSpacing = 0;
		x = 0.5 - (1.5/2);
		y = 0.5 - (1.2/2);
		h = 1.2;
		w = 1.5;
		text = "ATGM";
		border = 5;
		borderSize = 5;
		colorBorder[] = {1,1,1,1};
	};
	class Nux_Tvs_Mousearea
	{
		idc = Nux_Tvs_Mousearea_IDC;
		type = 0;
		style = 16;
		colorText[ ]={ 0,0,0,1 };
		colorBackground[ ]={ 0,0,0,0 };
		font = "PuristaSemibold";
		sizeEx = 0.05;
		lineSpacing = 0;
		x = safezoneXAbs;
		y = safezoneY;
		w = safezoneWAbs;
		h = safezoneH;
		text = "";
		onMouseMoving = "Nux_tvs_mousepos = [_this select 1,_this select 2 ];";
		onKeyDown = "_this call Nux_fnc_tvs_keypress";
	};
	class Nux_Tvs_Optics
	{
		idc = Nux_Tvs_Optics_IDC;
		type = 0;
		style = 48;
		colorText[ ]={ 1,1,1,1 };
		colorBackground[ ]={ 1,1,1,1 };
		font = "PuristaSemibold";
		sizeEx = 0.01;
		lineSpacing = 0;
		x = 0.5 - (1.45/2);
		y = 0.5 - (1/2);
		w = 1.45;
		h = 1;
		text = "TVS\pics\bomb_optic.paa";
	};/*
	class Nux_Tvs_Crosshair
	{
		idc = Nux_Tvs_Crosshair_IDC;
		type = 0;
		style = 48;
		colorText[ ]={ 1,0,0,1 };
		colorBackground[ ]={ 1,1,1,1 };
		font = "PuristaSemibold";
		sizeEx = 0.01;
		lineSpacing = 0;
		x = 0.5 - (0.1/2);
		y = 0.5 - (0.1/2);
		w = 0.1;
		h = 0.1;
		text = "TVS\pics\crosshair.paa";
	};*/
	class Nux_Tvs_Flirstate
	{
		idc = Nux_Tvs_Flirstate_IDC;
		type = 0;
		style = 0x00;
		colorText[ ]={ 1,1,1,1};
		colorBackground[ ]={ 0.5,0.5,0.5,0.1 };
		font = "PuristaSemibold";
		sizeEx = 0.025;
		lineSpacing = 0;
		x = -0.24;
		y = - 0.04;
		w = 0.1;
		h = 0.05;
		text = "Flir:off";
	};


};
