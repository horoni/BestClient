// This file can be included several times.

#ifndef MACRO_CONFIG_INT
#error "The config macros must be defined"
// This helps IDEs properly syntax highlight the uses of the macro below.
#define MACRO_CONFIG_INT(Name, ScriptName, Def, Min, Max, Save, Desc)
#define MACRO_CONFIG_COL(Name, ScriptName, Def, Save, Desc)
#define MACRO_CONFIG_STR(Name, ScriptName, Len, Def, Save, Desc)
#endif

MACRO_CONFIG_INT(ShDbg, sh_dbg, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Sh Debug")

// <><><> ESP <><><><><>
MACRO_CONFIG_INT(ShEsp, sh_esp, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Esp enable")
MACRO_CONFIG_INT(ShEspHookFov, sh_esp_hook_fov, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Draw Hook FOV")
MACRO_CONFIG_INT(ShEspWeaponFov, sh_esp_weapon_fov, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Draw Weapon FOV")
MACRO_CONFIG_COL(ShEspHookFovCol, sh_esp_hook_fov_col, 4294901760, CFGFLAG_CLIENT | CFGFLAG_SAVE | CFGFLAG_COLALPHA, "Hook FOV Color")
MACRO_CONFIG_COL(ShEspWeaponFovCol, sh_esp_weapon_fov_col, 4294901760, CFGFLAG_CLIENT | CFGFLAG_SAVE | CFGFLAG_COLALPHA, "Weapon FOV Color")

// <><><> AimBot <><><><><>
MACRO_CONFIG_INT(ShAim, sh_aim, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Aimbot enable")
MACRO_CONFIG_INT(ShAimSilent, sh_aim_silent, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Silent aimbot")
MACRO_CONFIG_INT(ShAimForceFng, sh_aim_forcefng, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Force FNG Gamemode")

// <> Hook <>
MACRO_CONFIG_INT(ShAimHook, sh_aim_hook, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot Enable")
MACRO_CONFIG_INT(ShAimHookFov, sh_aim_hook_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot FOV")
MACRO_CONFIG_INT(ShAimHookEdge, sh_aim_hook_edge, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot Edge")
MACRO_CONFIG_INT(ShAimHookEdgeAccuracy, sh_aim_hook_edge_accuracy, 0, 0, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Edge accuracy")

// <> Hammer <>
MACRO_CONFIG_INT(ShAimHammer, sh_aim_hammer, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hammer Aimbot Enable")
MACRO_CONFIG_INT(ShAimHammerFov, sh_aim_hammer_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hammer Aimbot FOV")

// <> Gun <>
MACRO_CONFIG_INT(ShAimGun, sh_aim_gun, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Gun Aimbot Enable")
MACRO_CONFIG_INT(ShAimGunFov, sh_aim_gun_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Gun Aimbot FOV")

// <> Shotgun <>
MACRO_CONFIG_INT(ShAimShotgun, sh_aim_shotgun, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Shotgun Aimbot Enable")
MACRO_CONFIG_INT(ShAimShotgunFov, sh_aim_shotgun_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Shotgun Aimbot FOV")

// <> Grenade <>
MACRO_CONFIG_INT(ShAimGrenade, sh_aim_grenade, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Grenade Aimbot Enable")
MACRO_CONFIG_INT(ShAimGrenadeFov, sh_aim_grenade_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Grenade Aimbot FOV")

// <> Laser <>
MACRO_CONFIG_INT(ShAimLaser, sh_aim_laser, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Laser Aimbot Enable")
MACRO_CONFIG_INT(ShAimLaserFov, sh_aim_laser_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Laser Aimbot FOV")
MACRO_CONFIG_INT(ShAimLaserAuto, sh_aim_laser_auto, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Laser Auto Fire")
