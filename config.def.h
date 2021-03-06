/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx              = 3;            /* border pixel of windows */
static const int gappx                          = 18;           /* gaps between windows */
static const unsigned int snap                  = 32;           /* snap pixel */
static const unsigned int systraypinning        = 0;            /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing        = 0;            /* systray spacing */
static const int systraypinningfailfirst        = 1;            /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor */
static const int showsystray                    = 1;            /* 0 means no systray */
static const int swallowfloating                = 0;            /* 1 means swallow floating windows by default */
static const int showbar                        = 1;            /* 0 means no bar */
static const int topbar                         = 1;            /* 0 means bottom bar */
static const int viewontag                      = 1;            /* follow a window when moving to a tag */
static const int horizpadbar                    = 12;           /* horizontal padding for statusbar */
static const int vertpadbar                     = 12;           /* vertical padding for statusbar */
static const int vertpad                        = 18;           /* vertical padding of bar */
static const int sidepad                        = 18;           /* horizontal padding of bar */
static const char *fonts[]                      = { "monospace:size=16", "JoyPixels:pixelsize=16:antialias=true:autohint=true" };
static const char dmenufont[]                   = "monospace:size=16";
static char selbordercolor[]                    = "#000000";
static char col_black[]                         = "#000000";
static char col_white[]                         = "#ffffff";
static char col_normborder[]                    = "#282a36";
static char col_selborder[]                     = "#ffb86c";
static char col_normbg[]                        = "#282a36";
static char col_selbg[]                         = "#684a82";
static char col_normfg[]                        = "#ff79c6";
static char col_selfg[]                         = "#ffb86c";
static const unsigned int baralpha              = 208;
static const unsigned int borderalpha           = OPAQUE;
static const char *colors[][3]                  = {
    /*                    fg                bg         	    border          */
    [SchemeNorm] =      { col_white,        col_normbg,     col_normborder  },
    [SchemeSel]  =      { col_white,        col_black,      col_selborder   },
    [SchemeStatus] =    { col_normfg,       col_normbg,     col_black       },
    [SchemeTagsNorm] = 	{ col_normfg,       col_normbg,     col_black       },
    [SchemeTagsSel] =   { col_selfg,        col_selbg,      col_black       },
    [SchemeInfoNorm] =  { col_normfg,       col_black,      col_black       },
    [SchemeInfoSel] =   { col_selfg,        col_white,      col_black       },
};

static const unsigned int alphas[][3]           = {
    /*                    fg            bg          border      */
    [SchemeStatus] =    { OPAQUE,       baralpha,   borderalpha },
    [SchemeTagsNorm] =  { OPAQUE,       baralpha,   borderalpha },
    [SchemeTagsSel] =   { OPAQUE,       baralpha,   borderalpha },
    [SchemeInfoNorm] =  { 0,            0,          0           },
    [SchemeInfoSel] =   { 0,            0,          0           },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class        instance    title               tags mask   isfloating  isterminal  noswallow   monitor */
    { "Gimp",       NULL,       NULL,               0,          1,          0,          0,          -1 },
    { "Firefox",    NULL,       NULL,               1 << 8,     0,          0,          -1,         -1 },
    { NULL,         NULL,       "scratchpad",       0,          1,          0,          0,          -1 },
    { "St",         NULL,       NULL,               0,          0,          1,          0,          -1 },
    { NULL,         NULL,       "Event Tester",     0,          0,          0,          1,          -1 }, /* xev */
    { NULL,         NULL,       "Arcolinux Logout", 0,          1,          0,          0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.6; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol   arrange function */
    { "[]=",    tile },
    { "><>",    NULL }, /* no layout function means floating behavior */
    { "[M]",    monocle },
    { "|M|",    centeredmaster },
    { ">M>",    centeredfloatingmaster },
    { "TTT",    bstack },
    { "===",    bstackhoriz },
    { "D",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]             = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_normbg, "-nf", col_normfg, "-sb", col_selbg, "-sf", col_selfg, "-nhb", col_normbg, "-nhf", col_selfg, "-shb", col_normbg, "-shf", col_normfg, "-h", "30", "-g", "3", "-l", "5", NULL };
static const char *dmenuemojicmd[]  = { "dmenuunicode" , NULL};
static const char *termcmd[]        = { "st", NULL };
static const char scratchpadname[]  = "scratchpad";
static const char *scratchpadcmd[]  = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *layoutmenu_cmd   = "layoutmenu.sh";
static const char *lockscreencmd[]  = { "betterlockscreen", "-l", "blur", NULL};

#include "selfrestart.c"

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key                     function                    argument */

    // launchers
    { MODKEY,                       XK_p,                   spawn,                      {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_p,                   spawn,                      {.v = dmenuemojicmd} },
    { MODKEY,                       XK_Return,              spawn,                      {.v = termcmd } },
    { MODKEY,                       XK_grave,               togglescratch,              {.v = scratchpadcmd } },
    { 0|Mod1Mask|ControlMask,       XK_s,                   spawn,                      SHCMD("spotify") },
    { 0|Mod1Mask|ControlMask,       XK_e,                   spawn,                      SHCMD("pcmanfm") },
    { 0|Mod1Mask|ControlMask,       XK_f,                   spawn,                      SHCMD("firefox") },
    { 0|Mod1Mask|ControlMask,       XK_c,                   spawn,                      SHCMD("google-chrome-stable") },
    { 0|Mod1Mask|ControlMask,       XK_m,                   spawn,                      SHCMD("xfce4-settings-manager") },
    { 0|Mod1Mask|ControlMask,       XK_u,                   spawn,                      SHCMD("pavucontrol") },
    // { 0|Mod1Mask|ControlMask,       XK_k,                   spawn,                      SHCMD("xev | awk -F'[ )]+' '/^KeyPress/ { a[NR+2] } NR in a { printf "%-3s %s\n", $5, $8 }'") },
    { 0|ShiftMask|ControlMask,      XK_Escape,              spawn,                      SHCMD("urxvt 'htop task manager' -e htop") },
    { MODKEY|ShiftMask,             XK_c,                   killclient,                 {0} },
    { MODKEY,                       XK_q,                   killclient,                 {0} },

    // layout
    { MODKEY,                       XK_b,                   togglebar,                  {0} },
    { MODKEY,                       XK_j,                   focusstack,                 {.i = +1 } },
    { MODKEY,                       XK_k,                   focusstack,                 {.i = -1 } },
    { MODKEY,                       XK_bracketleft,         incnmaster,                 {.i = -1 } },
    { MODKEY,                       XK_bracketright,        incnmaster,                 {.i = +1 } },
    { MODKEY,                       XK_h,                   setmfact,                   {.f = -0.05} },
    { MODKEY,                       XK_l,                   setmfact,                   {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_j,                   movestack,                  {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,                   movestack,                  {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Return,              zoom,                       {0} },
    { MODKEY,                       XK_Tab,                 view,                       {0} },
    { MODKEY,                       XK_t,                   setlayout,                  {.v = &layouts[0]} },
    { MODKEY,                       XK_m,                   setlayout,                  {.v = &layouts[2]} },
    { MODKEY,                       XK_o,                   setlayout,                  {.v = &layouts[3]} },
    { MODKEY|ShiftMask,             XK_o,                   setlayout,                  {.v = &layouts[4]} },
    { MODKEY,                       XK_u,                   setlayout,                  {.v = &layouts[5]} },
    { MODKEY|ShiftMask,             XK_u,                   setlayout,                  {.v = &layouts[6]} },
    { MODKEY|ShiftMask,             XK_t,                   setlayout,                  {.v = &layouts[7]} },
    { MODKEY,                       XK_f,                   togglefullscreen,           {0} },
    { MODKEY|ShiftMask,             XK_f,                   togglefakefullscreen,       {0} },
    // { MODKEY,                       XK_space,               setlayout,                  {0} },
    { MODKEY|ShiftMask,             XK_space,               togglefloating,             {0} },
    { MODKEY,                       XK_minus,               setgaps,                    {.i = -10 } },
    { MODKEY,                       XK_equal,               setgaps,                    {.i = +10 } },
    { MODKEY|ShiftMask,             XK_equal,               setgaps,                    {.i = 0 } },

    // monitors
    { MODKEY,                       XK_comma,               focusmon,                   {.i = +1 } },
    { MODKEY,                       XK_period,              focusmon,                   {.i = -1 } },
    { MODKEY|ShiftMask,             XK_comma,               tagmon,                     {.i = +1 } },
    { MODKEY|ShiftMask,             XK_period,              tagmon,                     {.i = -1 } },

    // system
    { MODKEY|ShiftMask,             XK_l,                   spawn,                      {.v = lockscreencmd } },
    { MODKEY,                       XK_x,                   spawn,                      SHCMD("arcolinux-logout") },
    { MODKEY|ShiftMask,             XK_r,                   self_restart,               {0} },
    { MODKEY|ShiftMask,             XK_q,                   quit,                       {0} },
    { MODKEY|ControlMask,           XK_minus,               spawn,                      SHCMD("pamixer --allow-boost -d 5") },
    { MODKEY|ControlMask|ShiftMask, XK_minus,               spawn,                      SHCMD("pamixer --allow-boost -d 15") },
    { MODKEY|ControlMask,           XK_equal,               spawn,                      SHCMD("pamixer --allow-boost -i 5") },
    { MODKEY|ControlMask|ShiftMask, XK_equal,               spawn,                      SHCMD("pamixer --allow-boost -i 15") },
    { MODKEY|ControlMask,           XK_0,                   spawn,                      SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.PlayPause") },
    { MODKEY|ControlMask,           XK_bracketleft,         spawn,                      SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Previous") },
    { MODKEY|ControlMask,           XK_bracketright,        spawn,                      SHCMD("dbus-send --print-reply --dest=org.mpris.MediaPlayer2.spotify /org/mpris/MediaPlayer2 org.mpris.MediaPlayer2.Player.Next") },

    // screenshots
    { MODKEY|ShiftMask,             XK_s,                   spawn,                      SHCMD("maim -s -u | xclip -selection clipboard -t image/png -i") }, /* Area to clipboard */

    // tags
    { MODKEY,                       XK_0,                   view,                       {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,                   tag,                        {.ui = ~0 } },
    TAGKEYS(                        XK_1,                                               0)
    TAGKEYS(                        XK_2,                                               1)
    TAGKEYS(                        XK_3,                                               2)
    TAGKEYS(                        XK_4,                                               3)
    TAGKEYS(                        XK_5,                                               4)
    TAGKEYS(                        XK_6,                                               5)
    TAGKEYS(                        XK_7,                                               6)
    TAGKEYS(                        XK_8,                                               7)
    TAGKEYS(                        XK_9,                                               8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
    { ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
