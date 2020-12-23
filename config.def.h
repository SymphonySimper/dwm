
// IMPORTS
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 32;	/* snap pixel */

/* Bar */
static const int showbar = 0;	  /* 0 means no bar */
static const int topbar = 1;	  /* 0 means bottom bar */
static const int horizpadbar = 2; /* horizontal padding for statusbar */
static const int vertpadbar = 2;  /* vertical padding for statusbar */

/* Font */
static const char *fonts[] = {"poppins:size=10"};
static const char dmenufont[] = "poppins:size=10";

/* Color */
static const char col_gray1[] = "#111111";
static const char col_gray2[] = "#222222";
static const char col_gray3[] = "#444444";
static const char col_gray6[] = "#777777";
static const char col_gray4[] = "#bbbbbb";
static const char col_gray5[] = "#eeeeee";
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = {col_gray4, col_gray1, col_gray2},
	[SchemeSel] = {col_gray5, col_gray1, col_gray6},
};

/* Auto Startups */
static const char *const autostart[] = {
	"picom","-m","1.0","-i","1.0","-e","1.0", "--no-fading-openclose", NULL,
	"nitrogen", "--restore", NULL,
	"wmname", "LG3D", NULL,
	"slstatus", NULL,
	"firefox", NULL,
	"discord", NULL,
	"spotify", NULL,
	NULL /* terminate */
};

/* tagging */
/*static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};*/

static const char *tags[] = { "  ","  ","  ", "  ", "  ", "  ", "  ", "  ", "  "};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	
	{"firefox", NULL, NULL, 1 << 1, 0, -1},
	{"Code" , NULL, NULL, 1 << 2, 0, -1},
	{"vlc", NULL, NULL, 1 << 3 , 0, -1},
	{"discord", NULL, NULL, 1 << 4, 0, -1},
	{"Spotify", NULL, NULL, 1 << 5, 0, -1},
	{"Pcmanfm", NULL, NULL, 1 << 6, 0, -1},
	{"Gimp", NULL, NULL, 1 << 7, 0, -1},
};

/* layout(s) */
static const float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;	  /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */

// Volume Control
static const char *upvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "+2%", NULL};
static const char *downvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "-2%", NULL};
static const char *mutevol[] = {"/usr/bin/pactl", "set-sink-mute", "0", "toggle", NULL};

// Brightness Control
static const char *brupcmd[] = {"xbacklight", "-inc", "2", NULL};
static const char *brdowncmd[] = {"xbacklight", "-dec", "2", NULL};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"    ", tile}, /* first entry is default */
	{"   ", monocle},
	{"   ", NULL}, /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray4, "-sb", col_gray3, "-sf", col_gray5, NULL};
static const char *termcmd[] = {"alacritty", NULL};
static const char *firefoxcmd[] = {"firefox", NULL};
static const char *cmdprintscreen[] = {"scrot", "-z", "-q", "100", "/home/s2b/Pictures/screenshots/%Y-%m-%d-%s_$wx$h.jpg", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */

	/* Opens Dmenu */
	{MODKEY, XK_p, spawn, {.v = dmenucmd}},

	/* Opens Terminal */
	{MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},

	/* Open Firefox */
	{MODKEY | ShiftMask, XK_f, spawn, {.v = firefoxcmd}},

	/* Show/Hide Bar */
	{MODKEY, XK_b, togglebar, {0}},

	/* Change Focus */
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},

	/* Vertical Split */
	{MODKEY, XK_i, incnmaster, {.i = +1}},

	/* Horizontal Split */
	{MODKEY, XK_d, incnmaster, {.i = -1}},

	/* Increase Size of the window */
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},

	/* IDK what it does */
	{MODKEY, XK_Return, zoom, {0}},

	/* Switches between tag 1 and 2 */
	{MODKEY, XK_Tab, view, {0}},

	/* Kills window on focus */
	{MODKEY | ShiftMask, XK_c, killclient, {0}},

	/* Layout Stack */
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},

	/* Layout Monocule */
	{MODKEY, XK_m, setlayout, {.v = &layouts[1]}},

	/* Layout Floating */
	{MODKEY, XK_f, setlayout, {.v = &layouts[2]}},

	/* Cycles between Stack and Monocule layout */
	{MODKEY, XK_space, setlayout, {0}},

	/* No Idea what it does */
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},

	/* Switch Tab */
	{MODKEY, XK_0, view, {.ui = ~0}},

	/* Moves winodw on focus to a different tag */
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

	/* Cycle stack */
	{MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
	{MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},

	/* Volume */
	{MODKEY, XK_F2, spawn, {.v = downvol}},
	{MODKEY, XK_F3, spawn, {.v = upvol}},
	{MODKEY, XK_F4, spawn, {.v = mutevol}},

	/* Brightness */
	{MODKEY, XK_F5, spawn, {.v = brupcmd}},
	{MODKEY, XK_F6, spawn, {.v = brdowncmd}},

	/* Take Screenshot using scrot */
	{0, XK_Print, spawn, {.v = cmdprintscreen}},
	/* Tag Keys */
	TAGKEYS(XK_1, 0)
		TAGKEYS(XK_2, 1)
			TAGKEYS(XK_3, 2)
				TAGKEYS(XK_4, 3)
					TAGKEYS(XK_5, 4)
						TAGKEYS(XK_6, 5)
							TAGKEYS(XK_7, 6)
								TAGKEYS(XK_8, 7)
									TAGKEYS(XK_9, 8)

	/* Restart and Quit dwm */
	{MODKEY | ShiftMask, XK_q, quit, {0}},
	{MODKEY | ShiftMask, XK_r, quit, {1}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
