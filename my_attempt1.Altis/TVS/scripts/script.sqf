// Howto use: simply put this in the init of the unit. altho you need to know the ammotype.
// TVS= [this,"Bo_GBU12_LGB_MI10"] execvm "TVS\scripts\init.sqf"


private ["_unit","_ammo"];
_unit = vehicle (_this select 0);
_ammo = _this select 1;
// This could be put into main init file.
call compileFinal preprocessfilelinenumbers "TVS\scripts\tvg.sqf";

_unit setvariable ["Nux_tvs_thermal",0];
_unit setVariable ["Nux_tvs_onf",0];
_unit setVariable ["Nux_tvs_ammo",_ammo];
_unit addEventHandler ["FIRED", {if ((player == gunner (_this select 0)) or ((player == driver (_this select 0))
  and not (isPlayer gunner (_this select 0)))) then {
    _this spawn Nux_fnc_tvs_start;
    };}];
