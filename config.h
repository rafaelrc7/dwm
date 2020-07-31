/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  		= 3;        /* border pixel of windows */
static unsigned int snap      		= 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 4;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 4;       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static int showbar            		= 1;        /* 0 means no bar */
static int topbar             		= 1;        /* 0 means bottom bar */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const char *fonts[]          = { "monospace:size=10", "Symbola:size=12", "Noto Color Emoji:size=12" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char urgbordercolor[]   		= "#ff0000";
static char urgfgcolor[]            = "#bbbbbb";
static char urgbgcolor[]            = "#222222";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
	   [SchemeUrg]  = { urgfgcolor,  urgbgcolor,  urgbordercolor  },
};
static unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
	[SchemeUrg]  = { OPAQUE, baralpha, borderalpha },
};

static const char *const autostart[] = {
	"picom", "-b", NULL,
	"redshift-gtk", NULL,
	"clipmenud", NULL,
	"lxpolkit", NULL,
	"conky", NULL,
	"updatewallpaper", NULL,
	"dwmblocks", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1:", "2:", "3:", "4:", "5:", "6:", "7:", "8:", "9:" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",             NULL,       NULL,       0,            1,           0,         0,        -1 },
	{ "firefox",          NULL,       NULL,       1 << 1,       0,           0,         0,        -1 },
	{ "St",               NULL,       NULL,       0,            0,           1,         1,        -1 },
	{ "Steam",            NULL,       NULL,       1 << 3,       0,           0,         0,        -1 },
	{ "discord",          NULL,       NULL,       1 << 2,       0,           0,         0,        -1 },
	{ "TelegramDesktop",  NULL,       NULL,       1 << 2,       0,           0,         0,        -1 },
	{ "Galculator",       NULL,       NULL,       0,            1,           0,         0,        -1 },
	{ "KeePassXC",        NULL,       NULL,       1 << 8,       0,           0,         0,        -1 },
	{ "Thunderbird",      NULL,       NULL,       1 << 8,       0,           0,         0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define SUPER Mod4Mask
#define ALTKEY Mod1Mask
	
#define MODKEY SUPER

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	{ MOD, XK_w,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define BROWSER "firefox"
#define FILEMNG "thunar"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-i", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };
static const char *filemanager[] = { FILEMNG, NULL };
static const char *browser[] = { BROWSER, NULL };
static const char *dmenunetwork[] = { "networkmanager_dmenu", NULL };
static const char *dmenuclip[] = { "clipmenu", NULL };
static const char *dmenuuni[] = { "unipicker", "--copy", "--command", "dmenu -i", NULL };
static const char *dmenuemoji[] = { "/home/rafael/.bin/dmenu-emoji", NULL };
static const char *printscreen[] = { "screengrab", "-r", NULL };
static const char *togglepicom[] = { "/home/rafael/.bin/toggle-picom", NULL };
static const char *togglemic[] = { "/home/rafael/.bin/toggle-mic", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "urgbgcolor",         STRING,  &urgbgcolor},
		{ "urgbordercolor",     STRING,  &urgbordercolor},
		{ "urgfgcolor",         STRING,  &urgfgcolor},
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
		{ "alpha",              ALPHA,   alphas},
};

#include "shiftview.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ALTKEY,                XK_Return, togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filemanager } },
	{ MODKEY,                       XK_q,      spawn,          {.v = browser } },
	{ MODKEY|ALTKEY,                XK_n,      spawn,          {.v = dmenunetwork } },
	{ MODKEY|ALTKEY,                XK_c,      spawn,          {.v = dmenuclip } },
	{ MODKEY|ALTKEY,                XK_u,      spawn,          {.v = dmenuuni } },
	{ MODKEY|ALTKEY,                XK_e,      spawn,          {.v = dmenuemoji } },
	{ MODKEY|ALTKEY,                XK_m,      spawn,          {.v = togglemic } },
	{ 0,                            XK_Print,  spawn,          {.v = printscreen } },
	{ ALTKEY|ShiftMask,             XK_F12,    spawn,          {.v = togglepicom } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ALTKEY,              	XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|ALTKEY,              	XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|ALTKEY|ShiftMask,    	XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|ALTKEY|ShiftMask,    	XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|ALTKEY|ControlMask,  	XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|ALTKEY|ControlMask,  	XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|ALTKEY,              	XK_0,      togglegaps,     {0} },
	{ MODKEY|ALTKEY|ShiftMask,    	XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } },
	{ MODKEY|ALTKEY,              	XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ALTKEY,              	XK_o,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,						XK_Right,  shiftview,	   {.i = +1}  },
	{ MODKEY,						XK_Left,   shiftview,	   {.i = -1}  },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

