// This file can be included several times.

#ifndef MACRO_CONFIG_INT
#error "The config macros must be defined"
// This helps IDEs properly syntax highlight the uses of the macro below.
#define MACRO_CONFIG_INT(Name, ScriptName, Def, Min, Max, Save, Desc)
#define MACRO_CONFIG_COL(Name, ScriptName, Def, Save, Desc)
#define MACRO_CONFIG_STR(Name, ScriptName, Len, Def, Save, Desc)
#endif

MACRO_CONFIG_INT(ClEspEnable, sh_esp, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Esp enable")
MACRO_CONFIG_INT(ClEspFov, sh_esp_fov, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Draw FOV")
MACRO_CONFIG_INT(ClAimbotEnable, sh_aimbot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Aimbot enable")
MACRO_CONFIG_INT(ClAimbotFov, sh_aimbot_fov, 1, 1, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Aimbot FOV")
MACRO_CONFIG_INT(ClAimbotSilent, sh_aimbot_silent, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Silent aimbot")
MACRO_CONFIG_INT(ClAimbotHookVisible, sh_aimbot_hookvisible, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Hook visible")
MACRO_CONFIG_INT(ClAimbotEdge, sh_aimbot_edge, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Edge aimbot")
MACRO_CONFIG_INT(ClAimbotEdgeAccuracy, sh_aimbot_edge_accuracy, 0, 0, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Edge accuracy")

