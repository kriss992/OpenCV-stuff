#define Nux_tvs_Dialog						8560
#define Nux_Tvs_MainFrame_IDC 		8561
#define Nux_Tvs_Mousearea_IDC 		8562
#define Nux_Tvs_Optics_IDC 				8563
//#define Nux_Tvs_Crosshair_IDC			8564
#define Nux_Tvs_Flirstate_IDC			8565

Nux_fnc_tvs_start = {
	man_guidance = 0;
	man_guidance_type = 0;
	lock_guidance = 0;
	st1 = 0;
	st2 = 0;
	dir_a = 0;
	button = 0;
	x_move = 0;
	y_move = 0;
	private ["_unit", "_weapon", "_missile", "_validammo"];
	_unit = vehicle (_this select 0);
	_weapon = _this select 1;
	_missile = _this select 6;
	_validammo = _unit getvariable "Nux_tvs_ammo";

	_unit setVariable ["Nux_tvs_onf", 1];

	// just exit if its not a valid weapon.
	if ((_weapon == "CMFlareLauncher") or (_unit getvariable "Nux_tvs_onf" == 0) or (typeof(_missile) != _validammo)) exitwith {};

	createDialog "Nux_Dlg_Tvs";
	findDisplay 8560 displayCtrl 8561 ctrlMapCursor ["Arrow", "Rotate"];
	findDisplay 8560 displayCtrl 8562 ctrlMapCursor ["Arrow", "Rotate"];
	findDisplay 8560 displayCtrl 8563 ctrlMapCursor ["Arrow", "Rotate"];
	findDisplay 8560 displayCtrl 8564 ctrlMapCursor ["Arrow", "Rotate"];
	findDisplay 8560 displayCtrl 8565 ctrlMapCursor ["Arrow", "Rotate"];
	waituntil{(dialog)};
	// center the mouse cursor
	setMousePosition [0.5,0.5];
	// Switch to the missile.
	_missile switchCamera "Internal";
	_missile camSetFov 0.75;
	_missile camCommit 0;
	// set the state of thermal info..
	_thermalmode = "Flir:off";
	switch (_unit getvariable "Nux_tvs_thermal") do {
		case 0 : {false setCamUseTi 0; _thermalmode = "Flir:off";};
		case 1 : {true setCamUseTi 0; _thermalmode = "Flir:m0";};
		case 2 : {true setCamUseTi 1; _thermalmode = "Flir:m1";};
	};
	((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlSetText format["%1",_thermalmode];
	((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlCommit 0;

	missile = _missile;
	[_unit, _weapon, _missile] spawn Nux_fnc_tvs_misislecontrol;
};

// change thermal mode.
Nux_fnc_tvs_keypress = {
	private ["_state","_thermalmode"];
	switch (_this select 1) do
	{
		_thermalmode = _this select 1;
		podatak = str(_this select 0) + "***" + str(_this select 1);
		//hint podatak;
		//((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlSetText format["%1", (str (podatak))];
		//((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlCommit 0;

		case 1:	{
			closeDialog 0;
		};
		//30 = a
		case 30:
		{
			if(man_guidance == 0) then { man_guidance = 1; } else { man_guidance = 0; };
			setMousePosition [0.5,0.5];
		};
		// 31 = s
		case 31:
		{
			if(dir_a == 0) then { dir_a = 1; } else { dir_a = 0; };
		};
		//32 = d
		case 32:		{
			_ctrls = "";
			//_ctrls = allDisplays;
			for "_i" from -1 to 10000 do {
				//_ctrls = _ctrls + format ["IDC: %1, Class Name: %2\n", _i];
				with uiNamespace do {
					_ctrl = findDisplay 46 displayCtrl _i;
					if (!isNull _ctrl) then {
						_ctrls = _ctrls + format ["IDC: %1, Class Name: %2\n", _i, ctrlClassName _ctrl];
					};
				};
			};
			//hint _ctrls;
		};
		// 47 = v
		case 47:
		{
			_state = vehicle player getvariable "Nux_tvs_thermal";
			_state = _state + 1;
			if (_state > 2) then {_state = 0;};
			vehicle player setvariable ["Nux_tvs_thermal", _state];
			_thermalmode = "Flir:test";
			switch (_state) do {
				case 0 : {false setCamUseTi 0; _thermalmode = "Flir:off";};
				case 1 : {true setCamUseTi 0; _thermalmode = "Flir:m0";};
				case 2 : {true setCamUseTi 1; _thermalmode = "Flir:m1";};
			};
			((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlSetText format["%1",_thermalmode];
			((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlCommit 0;
		};
		// 33 = f
		case 33:
		{
			mx = Nux_tvs_mousepos select 0;
			my = Nux_tvs_mousepos select 1;
			PosTgt = screenToWorld [mx,my];
			objs = (screenToWorld [mx,my]) nearEntities 50;
			string = getCursorObjectParams;
			((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlSetText format["%1", (str (string select 0))];
			((uiNamespace getVariable 'Nux_Tvs_Display') displayCtrl Nux_Tvs_Flirstate_IDC) ctrlCommit 0;
			if(lock_guidance == 0) then {lock_guidance = 1; } else { lock_guidance = 0; };
		};
		// 57 = space
		case 57:
		{
		};
		// 17 = w
		case 17:
		{
			if(man_guidance_type == 0) then {man_guidance_type = 1;} else {man_guidance_type = 0;};
		};
		case 23: //gore o
		{	if(_this select 3) then	{	y_move = 1; } else { y_move = 1;	};};
		case 37: //dole k
		{ if(_this select 3) then	{	y_move = -1; } else {	y_move = -1;	};};
		case 36: //levo j
		{	if(_this select 3) then	{	x_move = -1; } else {	x_move = -1;	};};
		case 38: //desno l
		{	if(_this select 3) then	{	x_move = 1;	} else { x_move = 1;	};};
		// 17 = w, gore = 200, levo = 203, dole = 208, desno = 205
		/*case 23: //gore o
		{	if(_this select 3) then	{	y_move = 1; } else { y_move = 0.25;	};};
		case 37: //dole k
		{ if(_this select 3) then	{	y_move = -1; } else {	y_move = -0.25;	};};
		case 36: //levo j
		{	if(_this select 3) then	{	x_move = -1; } else {	x_move = -0.25;	};};
		case 38: //desno l
		{	if(_this select 3) then	{	x_move = 1;	} else { x_move = 0.25;	};};
		// 17 = w, gore = 200, levo = 203, dole = 208, desno = 205*/
	};
};
// control the missile by mouse
Nux_fnc_tvs_misislecontrol = {
	private ["_unit","_weapon","_missile","_mx","_my","_allDisps"];

	_unit = _this select 0;
	_weapon = _this select 1;
	_missile = _this select 2;
	dlgchk = true;

	while {alive player && alive _missile} do
	{
		//moduleName_keyDownEHId = findDisplay 46 displayAddEventHandler ["MouseMoving", "hint str _this;"];
		//findDisplay 46 displayAddEventHandler ["JoystickButton", hint "radim joystick"];
		if (not dialog) exitwith {dlgchk = false;};

		if(man_guidance == 1  && man_guidance_type == 1) then {
			_mx = Nux_tvs_mousepos select 0;
			_my = Nux_tvs_mousepos select 1;
			tdir_moment = getdir vehicle _missile;
			ypitch_t = _missile call BIS_fnc_getPitchBank;
			ypitch_moment = ypitch_t select 0;
			xmap = (_mx + 0.284314) * ((1 + 1)/(1.28309 + 0.284314)) - 1;
			xturn_m = tdir_moment + xmap;
			ymap = (_my - 1.0866) * ((1 + 1)/(- 0.0882353 - 1.0866)) - 1;
			ypitch_m = ypitch_moment + ymap;
			_missile setDir xturn_m;
			[_missile, ypitch_m, 0] call BIS_fnc_setPitchBank;
			sleep 0.01;
		};

		if(man_guidance == 1 && man_guidance_type == 0) then {
			tdir_moment = getdir vehicle _missile;
			ypitch_t = _missile call BIS_fnc_getPitchBank;
			ypitch_moment = ypitch_t select 0;
			miss_pos = getPos _missile;
			return_string = "my2nddll" callExtension str(miss_pos);
			//hint str(return_string);
			if (return_string != "***" && return_string != "" && return_string != "+++" && return_string != "---") then {
				return_array = return_string splitString ",";
				x_abs = return_array select 4;
				y_abs = return_array select 5;
				x_abs = parseNumber x_abs;
				y_abs =	parseNumber y_abs;
				/*xmap = return_array select 0;
				ymap = return_array select 1;
				xmap = parseNumber xmap;
				ymap =	parseNumber ymap;
				xturn_m = tdir_moment + xmap;
				ypitch_m = ypitch_moment + ymap;
				xmap = 0;
				ymap = 0;*/
				xturn_m = tdir_moment + x_abs;
				ypitch_m = ypitch_moment + y_abs;
				x_abs = 0;
				y_abs = 0;
			}else{
				xturn_m = tdir_momen;
				ypitch_m = ypitch_moment;
			};
			_missile setDir xturn_m;
			[_missile, ypitch_m, 0] call BIS_fnc_setPitchBank;
			sleep 0.01;
		};
		/*
		if(man_guidance == 1 && man_guidance_type == 1) then {
			_mx = Nux_tvs_mousepos select 0;
			_my = Nux_tvs_mousepos select 1;
			tdir_moment = getdir vehicle _missile;
			ypitch_t = _missile call BIS_fnc_getPitchBank;
			ypitch_moment = ypitch_t select 0;
			xturn_m = tdir_moment;
			ypitch_m = ypitch_moment;
			if(_mx < 0.4) then {xturn_m = tdir_moment - 0.25;};
			if(_mx > 0.6) then {xturn_m = tdir_moment + 0.25;};
			if(_mx < 0.1) then {xturn_m = tdir_moment - 0.75;};
			if(_mx > 0.9) then {xturn_m = tdir_moment + 0.75;};
			if(_my < 0.4) then {ypitch_m = ypitch_moment + 0.25;};
			if(_my > 0.6) then {ypitch_m = ypitch_moment - 0.25;};
			if(_my < 0.1) then {ypitch_m = ypitch_moment + 0.75;};
			if(_my > 0.9) then {ypitch_m = ypitch_moment - 0.75;};
			_missile setDir xturn_m;
			[_missile, ypitch_m, 0] call BIS_fnc_setPitchBank;
			sleep 0.01;
		};
		*/
		if(man_guidance == 0 && lock_guidance == 0) then { };
		if(man_guidance == 0 && lock_guidance == 1) then {
			if (count objs == 0) then {
				if(((PosTgt distance _missile) > 250) && (dir_a == 0)) then {
					if(st1 == 0) then {
						PosTgt1 = [(PosTgt select 0), (PosTgt select 1), ((PosTgt select 2) + 150)];
						st1 = 1;
					};
					ScrPos = worldToScreen PosTgt1;
				} else {
					PosTgt2 = [(PosTgt select 0), (PosTgt select 1), ((PosTgt select 2) + 1)];
					ScrPos = worldToScreen PosTgt2;
				};
				setMousePosition ScrPos;
				_mx = ScrPos select 0;
				_my = ScrPos select 1;
				tdir_moment = getdir vehicle _missile;
				ypitch_t = _missile call BIS_fnc_getPitchBank;
				ypitch_moment = ypitch_t select 0;
				xmap = (_mx + 0.284314) * ((20 + 20)/(1.28309 + 0.284314)) - 20;
				xturn_m = tdir_moment + xmap;
				ymap = (_my - 1.0866) * ((20 + 20)/(- 0.0882353 - 1.0866)) - 20;
				ypitch_m = ypitch_moment + ymap;
				_missile setDir xturn_m;
				[_missile, ypitch_m, 0] call BIS_fnc_setPitchBank;
			} else {
				ObjTgt = getPos (objs select 0);
				if(((ObjTgt distance _missile) > 250) && (dir_a == 0)) then {
					ObjTgt = [(ObjTgt select 0), (ObjTgt select 1), ((ObjTgt select 2) + 150)];
				} else {
					ObjTgt = [(ObjTgt select 0), (ObjTgt select 1), ((ObjTgt select 2) + 2)];
				};
				ScrPos = worldToScreen ObjTgt;
				setMousePosition ScrPos;
				_mx = ScrPos select 0;
				_my = ScrPos select 1;
				tdir_moment = getdir vehicle _missile;
				ypitch_t = _missile call BIS_fnc_getPitchBank;
				ypitch_moment = ypitch_t select 0;
				xmap = (_mx + 0.284314) * ((20 + 20)/(1.28309 + 0.284314)) - 20;
				xturn_m = tdir_moment + xmap;
				ymap = (_my - 1.0866) * ((20 + 20)/(- 0.0882353 - 1.0866)) - 20;
				ypitch_m = ypitch_moment + ymap;
				_missile setDir xturn_m;
				[_missile, ypitch_m, 0] call BIS_fnc_setPitchBank;
			};
			sleep 0.01;
		};
	};
	if (not alive _unit) then {dlgchk=false; closeDialog Nux_tvs_Dialog;};
	if (dlgchk) then {closeDialog Nux_tvs_Dialog;1 cuttext ["","black out", 0.0000001];2 cutRsc["RscNoise","PLAIN",0.0001];sleep 1.1;2 cutFadeOut 0;1 cutFadeOut 0;};
	vehicle _unit switchCamera "internal";
	vehicle _unit selectWeapon _weapon;
};
