// IMPORTS
#include <X11/XF86keysym.h>
#include "gaplessgrid.c"

// appearance
static const unsigned int borderpx = 1; // border pixel of windows
static const unsigned int snap = 32;	// snap pixel

// Bar
static const int showbar = 0;	  // 0 means no bar
static const int topbar = 1;	  // 0 means bottom bar
static const int horizpadbar = 2; // horizontal padding for statusbar
static const int vertpadbar = 2;  // vertical padding for statusbar

// Font
static const char *fonts[] = {"poppins:size=10:antialias=true:autohint=true"};
static const char dmenufont[] = "poppins:size=10:antialias=true:autohint=true";

// Color
static const char col_gray1[] = "#111111";
static const char col_gray2[] = "#222222";
static const char col_gray3[] = "#444444";
static const char col_gray6[] = "#777777";
static const char col_gray4[] = "#bbbbbb";
static const char col_gray5[] = "#aaaaaa";
static const char *colors[][3] = {
	//               fg         bg         border  
	[SchemeNorm] = {col_gray4, col_gray1, col_gray2},
	[SchemeSel] = {col_gray5, col_gray1, col_gray6},
};
typedef struct
{
	const char *name;
	const void *cmd;

} Sp;
const char *spcmd1[] = {"alacritty", "-t", "spt0", "-e", "pulsemixer", "--color", "0", NULL};
const char *spcmd2[] = {"alacritty", "-t", "spt1", NULL};
const char *spcmd3[] = {"alacritty", "-t", "spm","-e", "bash", "spotifydrun", NULL};

static Sp scratchpads[] = {
	// name          cmd
	{"spt0", spcmd1},
	{"spt1", spcmd2},
	{"spm", spcmd3},
};

// Auto Startups
static const char *const autostart[] = {
	"picom", "-m", "1.0", "-i", "1.0", "-e", "1.0", "--vsync", "--backend", "glx", "--no-fading-openclose", NULL,
	"slstatus", NULL,
	"discord", NULL,
	NULL // terminate
};

// tagging
// static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const char *tags[] = {"  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  "};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	// class      instance    title       tags mask     isfloating   monitor

	{"ffplay", NULL, NULL, 0, 1, -1},
	{"firefox", NULL, NULL, 1 << 1, 0, -1},
	{"qutebrowser", NULL, NULL, 1 << 1, 0, -1},
	{"Code", NULL, NULL, 1 << 2, 0, -1},
	{"mpv", NULL, NULL, 1 << 3, 0, -1},
	{"discord", NULL, NULL, 1 << 4, 0, -1},
	{"Pcmanfm", NULL, NULL, 1 << 6, 0, -1},
	{"Gimp", NULL, NULL, 1 << 7, 0, -1},
	{"Blender", NULL, NULL, 1 << 7, 0, -1},
	{"kdenlive", NULL, NULL, 1 << 7, 0, -1},
	{"Inkscape", NULL, NULL, 1 << 7, 0, -1},
	{"obs", NULL, NULL, 1 << 8, 0, -1},
	{"Steam", NULL, NULL, 1 << 7, 0, -1},
	{"jetbrains-studio", NULL, NULL, 0, 1, -1},
	
	//Chromium apps
	{NULL, "www.figma.com__files", NULL, 1 << 5, 0, -1},
	{NULL, "web.whatsapp.com", NULL, 1 << 6, 0, -1},

	// Scratch Pads
	{NULL, NULL, "spt0", SPTAG(0), 1, -1},
	{NULL, NULL, "spt1", SPTAG(1), 1, -1},
	{NULL, NULL, "spm", SPTAG(2), 1, -1},
};

// layout(s) 
static const float mfact = 0.55;  // factor of master area size [0.05..0.95]
static const int nmaster = 1;	  // number of clients in master area
static const int resizehints = 1; // 1 means respect size hints in tiled resizals

// Volume Control
static const char *upvol[] = {"volume", "-u", NULL};
static const char *downvol[] = {"volume", "-d", NULL};
static const char *mutevol[] = {"volume", "-m", NULL};

//Spotifyd volume
static const char *spotifydvolup[] = {"spotifydVol", "-u", NULL};
static const char *spotifydvoldwn[] = {"spotifydVol", "-d", NULL};
static const char *spotifydmute[] = {"spotifydVol", "-m", NULL};

// Brightness Control
static const char *brupcmd[] = {"brightness", "-u", NULL};
static const char *brdowncmd[] = {"brightness", "-d", NULL};

//Take Screenshot
static const char *printscreen[] = {"screenshot", "-s", NULL};
static const char *printwindow[] = {"screenshot", "-w", NULL};
static const char *printregion[] = {"screenshot", "-r", NULL};

//Date/Time/Battery show
static const char *dateandbattery[] = {"dateAndBattery", NULL};

static const Layout layouts[] = {
	// symbol     arrange function
	{"    ", gaplessgrid}, // first entry is default
	{"    ", tile},
	{"   ", monocle},
	{"   ", NULL}, // no layout function means floating behavior
};

// key definitions 
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

// helper for spawning shell commands in the pre dwm-5.0 fashion
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

// commands
static char dmenumon[2] = "0"; // component of dmenucmd, manipulated in spawn()
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray4, "-sb", col_gray3, "-sf", col_gray5, NULL};
static const char *termcmd[] = {"alacritty", NULL};
static const char *firefoxcmd[] = {"firefox", NULL};
static const char *gamemodecmd[] = {"gameMode", NULL};
static const char *searchcmd[] = {"search", NULL};

static Key keys[] = {
	// modifier                     key        function        argument

	// Opens Dmenu
	{MODKEY, XK_p, spawn, {.v = dmenucmd}},

	// Opens Terminal
	{MODKEY | ShiftMask, XK_Return, spawn, {.v = termcmd}},

	// Open Firefox 
	{MODKEY | ShiftMask, XK_f, spawn, {.v = firefoxcmd}},

	// Show/Hide Bar
	{MODKEY | ShiftMask, XK_b, togglebar, {0}},

	// Change Focus
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},

	// Vertical Split
	{MODKEY, XK_i, incnmaster, {.i = +1}},

	// Horizontal Split
	{MODKEY, XK_d, incnmaster, {.i = -1}},

	// Increase Size of the window
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},

	// IDK what it does
	{MODKEY, XK_Return, zoom, {0}},

	// Switches between tag 1 and 2 
	{MODKEY, XK_Tab, view, {0}},

	// Kills window on focus
	{MODKEY | ShiftMask, XK_c, killclient, {0}},

	// Layout Grid
	{MODKEY, XK_g, setlayout, {.v = &layouts[0]}},

	// Layout Stack 
	{MODKEY, XK_t, setlayout, {.v = &layouts[1]}},

	// Layout Monocule
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},

	// Layout Floating
	{MODKEY, XK_f, setlayout, {.v = &layouts[3]}},

	// Cycles between Stack and Monocule layout
	{MODKEY, XK_space, setlayout, {0}},

	// No Idea what it does 
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},

	// Switch Tab
	{MODKEY, XK_0, view, {.ui = ~0}},

	// Moves winodw on focus to a different tag
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},

	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},

	// Cycle stack
	{MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
	{MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},

	// Volume 
	{MODKEY, XK_F2, spawn, {.v = downvol}},
	{MODKEY, XK_F3, spawn, {.v = upvol}},
	{MODKEY, XK_F4, spawn, {.v = mutevol}},
	
	//Spotifyd Volume
	{MODKEY | ShiftMask, XK_F2, spawn, {.v = spotifydvoldwn}},
	{MODKEY | ShiftMask, XK_F3, spawn, {.v = spotifydvolup}},
	{MODKEY | ShiftMask, XK_F4, spawn, {.v = spotifydmute}},

	// Brightness
	{MODKEY, XK_F5, spawn, {.v = brupcmd}},
	{MODKEY, XK_F6, spawn, {.v = brdowncmd}},

	// Take Screenshot using maim
	{0, XK_Print, spawn, {.v = printscreen}},
	{MODKEY | ShiftMask, XK_a, spawn, {.v = printwindow}},
	{MODKEY | ShiftMask, XK_s, spawn, {.v = printregion}},

	// Run scripts
	{MODKEY | ShiftMask, XK_p, spawn, {.v = gamemodecmd}},
	{MODKEY, XK_s, spawn, {.v = searchcmd}},
	{MODKEY, XK_b, spawn, {.v = dateandbattery}},

	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)

	// Move and resize with keyboard
	{MODKEY, XK_Down, moveresize, {.v = "0x 25y 0w 0h"}},
	{MODKEY, XK_Up, moveresize, {.v = "0x -25y 0w 0h"}},
	{MODKEY, XK_Right, moveresize, {.v = "25x 0y 0w 0h"}},
	{MODKEY, XK_Left, moveresize, {.v = "-25x 0y 0w 0h"}},
	{MODKEY | ShiftMask, XK_Down, moveresize, {.v = "0x 0y 0w 25h"}},
	{MODKEY | ShiftMask, XK_Up, moveresize, {.v = "0x 0y 0w -25h"}},
	{MODKEY | ShiftMask, XK_Right, moveresize, {.v = "0x 0y 25w 0h"}},
	{MODKEY | ShiftMask, XK_Left, moveresize, {.v = "0x 0y -25w 0h"}},
	{MODKEY | ControlMask, XK_Up, moveresizeedge, {.v = "t"}},
	{MODKEY | ControlMask, XK_Down, moveresizeedge, {.v = "b"}},
	{MODKEY | ControlMask, XK_Left, moveresizeedge, {.v = "l"}},
	{MODKEY | ControlMask, XK_Right, moveresizeedge, {.v = "r"}},
	{MODKEY | ControlMask | ShiftMask, XK_Up, moveresizeedge, {.v = "T"}},
	{MODKEY | ControlMask | ShiftMask, XK_Down, moveresizeedge, {.v = "B"}},
	{MODKEY | ControlMask | ShiftMask, XK_Left, moveresizeedge, {.v = "L"}},
	{MODKEY | ControlMask | ShiftMask, XK_Right, moveresizeedge, {.v = "R"}},

	{MODKEY | ControlMask, XK_m, togglefloating, {0}},

	// scratchpads 
	{MODKEY, XK_y, togglescratch, {.ui = 0}},
	{MODKEY, XK_u, togglescratch, {.ui = 1}},
	{MODKEY, XK_x, togglescratch, {.ui = 2}},

	// Restart and Quit dwm
	{MODKEY | ShiftMask, XK_q, quit, {0}},
	{MODKEY | ShiftMask, XK_r, quit, {1}},
};

// button definitions
// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static Button buttons[] = {
	// click                event mask      button          function        argument
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
