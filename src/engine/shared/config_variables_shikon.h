// This file can be included several times.

#ifndef MACRO_CONFIG_INT
#error "The config macros must be defined"
// This helps IDEs properly syntax highlight the uses of the macro below.
#define MACRO_CONFIG_INT(Name, ScriptName, Def, Min, Max, Save, Desc)
#define MACRO_CONFIG_COL(Name, ScriptName, Def, Save, Desc)
#define MACRO_CONFIG_STR(Name, ScriptName, Len, Def, Save, Desc)
#endif

MACRO_CONFIG_INT(ClShikonDbg, sh_dbg, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Sh Debug")
// <><><> ESP <><><><><>
MACRO_CONFIG_INT(ClEsp, sh_esp, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Esp enable")
MACRO_CONFIG_INT(ClEspFov, sh_esp_fov, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Draw FOV")

// <><><> AimBot <><><><><>
MACRO_CONFIG_INT(ClAimbot, sh_aimbot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Aimbot enable")
MACRO_CONFIG_INT(ClAimbotSilent, sh_aimbot_silent, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Silent aimbot")
MACRO_CONFIG_INT(ClAimbotHookVisible, sh_aimbot_hookvisible, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook visible")

// <> Hook <>
MACRO_CONFIG_INT(ClAimbotHook, sh_aimbot_hook, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotHookFov, sh_aimbot_hook_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot FOV")
MACRO_CONFIG_INT(ClAimbotHookEdge, sh_aimbot_hook_edge, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook Aimbot Edge")
MACRO_CONFIG_INT(ClAimbotHookEdgeAccuracy, sh_aimbot_hook_edge_accuracy, 0, 0, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Edge accuracy")

// <> Hammer <>
MACRO_CONFIG_INT(ClAimbotHammer, sh_aimbot_hammer, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hammer Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotHammerFov, sh_aimbot_hammer_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hammer Aimbot FOV")

// <> Gun <>
MACRO_CONFIG_INT(ClAimbotGun, sh_aimbot_gun, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Gun Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotGunFov, sh_aimbot_gun_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Gun Aimbot FOV")

// <> Shotgun <>
MACRO_CONFIG_INT(ClAimbotShotgun, sh_aimbot_shotgun, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Shotgun Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotShotgunFov, sh_aimbot_shotgun_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Shotgun Aimbot FOV")

// <> Grenade <>
MACRO_CONFIG_INT(ClAimbotGrenade, sh_aimbot_grenade, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Grenade Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotGrenadeFov, sh_aimbot_grenade_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Grenade Aimbot FOV")

// <> Laser <>
MACRO_CONFIG_INT(ClAimbotLaser, sh_aimbot_laser, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Laser Aimbot Enable")
MACRO_CONFIG_INT(ClAimbotLaserFov, sh_aimbot_laser_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Laser Aimbot FOV")

