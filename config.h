/* modifier 0 means no modifier */
static char *useragent   = "Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2049.0 Safari/537.36";
static char *stylefile   = "~/.surf/style.css";
static char *scriptdir   = "~/.surf/script/";
static char *historyfile = "~/.surf/history.txt";

static Bool kioskmode       = FALSE; /* Ignore shortcuts */
static Bool showindicators  = TRUE;  /* Show indicators in window title */
static Bool zoomto96dpi     = TRUE;  /* Zoom pages to always emulate 96dpi */
static Bool runinfullscreen = FALSE; /* Run in fullscreen mode by default */

static guint defaultfontsize = 12;   /* Default font size */
static gfloat zoomlevel = 1.0;       /* Default zoom level */

/* Soup default features */
static char *cookiefile     = "~/.surf/cookies.txt";
static char *cookiepolicies = "Aa@"; /* A: accept all; a: accept nothing,
                                        @: accept no third party */
static char *cafile         = "/etc/ssl/certs/ca-certificates.crt";
static char *strictssl      = FALSE; /* Refuse untrusted SSL connections */
static time_t sessiontime   = 3600;

/* Webkit default features */
static Bool enablescrollbars = FALSE;
static Bool enablespatialbrowsing = TRUE;
static Bool enableplugins = TRUE;
static Bool enablescripts = TRUE;
static Bool enableinspector = TRUE;
static Bool loadimages = TRUE;
static Bool hidebackground  = FALSE;
static Bool allowgeolocation = TRUE;

#define SETPROP(p, q, r) { \
	.v = (char *[]){ "/bin/sh", "-c", \
		"prop=\"`xprop -id $3 $0 | cut -d '\"' -f 2 | xargs -I_ zenity --entry --text=$2 --entry-text=_`\" && " \
		"xprop -id $3 -f $1 8s -set $1 \"$prop\"", \
		p, q, r, winid, NULL \
	} \
}

/* DOWNLOAD(URI, referer) */
#define DOWNLOAD(d, r) { \
	.v = (char *[]){ "/bin/sh", "-c", \
	  "wget -P ~/tmp --load-cookies $3 --user-agent \"$1\" --referer \"$2\" \"$0\";" \
	  "sleep 60", \
	  d, useragent, r, cookiefile, NULL \
  } \
}

#define MODKEY GDK_CONTROL_MASK

/* hotkeys */
/*
 * If you use anything else but MODKEY and GDK_SHIFT_MASK, don't forget to
 * edit the CLEANMASK() macro.
 */
static Key keys[] = {
    /* modifier	            keyval      function    arg             Focus */
    { MODKEY|GDK_SHIFT_MASK,GDK_r,      reload,     { .b = TRUE } },
    { MODKEY,               GDK_r,      reload,     { .b = FALSE } },
    { MODKEY|GDK_SHIFT_MASK,GDK_p,      print,      { 0 } },

    { MODKEY,               GDK_p,      clipboard,  { .b = TRUE } },
    { MODKEY,               GDK_y,      clipboard,  { .b = FALSE } },

    { MODKEY|GDK_SHIFT_MASK,GDK_j,      zoom,       { .i = -1 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_k,      zoom,       { .i = +1 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_q,      zoom,       { .i = 0  } },
    { MODKEY,               GDK_minus,  zoom,       { .i = -1 } },
    { MODKEY,               GDK_plus,   zoom,       { .i = +1 } },

    { MODKEY,               GDK_l,      navigate,   { .i = +1 } },
    { MODKEY,               GDK_h,      navigate,   { .i = -1 } },

    { MODKEY,               GDK_j,      scroll_v,   { .i = +1 } },
    { MODKEY,               GDK_k,      scroll_v,   { .i = -1 } },
    { MODKEY,               GDK_b,      scroll_v,   { .i = -10000 } },
    { MODKEY,               GDK_space,  scroll_v,   { .i = +10000 } },
    { MODKEY,               GDK_i,      scroll_h,   { .i = +1 } },
    { MODKEY,               GDK_u,      scroll_h,   { .i = -1 } },

    { 0,                    GDK_F11,    fullscreen, { 0 } },
    { 0,                    GDK_Escape, stop,       { 0 } },
    { MODKEY,               GDK_o,      source,     { 0 } },
    { MODKEY|GDK_SHIFT_MASK,GDK_o,      inspector,  { 0 } },

    { MODKEY,               GDK_g,      spawn,      SETPROP("_SURF_URI", "_SURF_GO", "open") },
    { MODKEY,               GDK_slash,  spawn,      SETPROP("_SURF_FIND", "_SURF_FIND", "find") },

    { MODKEY,               GDK_n,      find,       { .b = TRUE } },
    { MODKEY|GDK_SHIFT_MASK,GDK_n,      find,       { .b = FALSE } },
};

