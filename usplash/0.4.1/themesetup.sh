#!/bin/sh

#
# Sanity checks
#
if [ ! -e theme.in ]; then
	exit 1
fi

if [ $# -lt 1 ]; then
	exit 1
fi

#
# Helper functions
#
struct_name() {
	name=${1%.*}
	echo "pixmap_$name"
}

#
# Begin processing
#

echo "/*"
echo " * Part 1 - Generic header part"
echo " */"
cat theme.in
echo



echo "/*"
echo " * Part 2 - Forward theme struct declarations"
echo " */"
first=yes
for size in $@; do
	if [ $first = "yes" ]; then
		THEMENAME="usplash_theme"
		first=no
	else
		THEMENAME="usplash_theme_$size"
	fi
	echo "struct usplash_theme $THEMENAME;"
done
echo



echo "/*"
echo " * Step 3 - Actual theme struct defs"
echo " */"
first=yes
while [ 1 ]; do
	size="$1"
	if [ -z "$size" ]; then
		exit 0
	fi

	if [ $first = "yes" ]; then
		THEMENAME="usplash_theme"
		first=no
	else
		THEMENAME="usplash_theme_$size"
	fi

	if [ -z "$2" ]; then
		THEMENEXT="NULL"
	else
		THEMENEXT="&usplash_theme_$2"
	fi
	shift

	PGFG_PIX="progressfg_$size.png"
	PGFG_STRUCT=$(struct_name "$PGFG_PIX")
	PGBG_PIX="progressbg_$size.png"
	PGBG_STRUCT=$(struct_name "$PGBG_PIX")
	BG_PIX="background_$size.png"
	BG_STRUCT=$(struct_name "$BG_PIX")

	PROGRESS_WIDTH=$(   identify "$PGFG_PIX" | cut -d' ' -f3 | cut -dx -f1)
	PROGRESS_HEIGHT=$(  identify "$PGFG_PIX" | cut -d' ' -f3 | cut -dx -f2)
	WIDTH=${size%x*}
	HEIGHT=${size#*x}

	if [ $(( $WIDTH / 16 )) -eq $(( $HEIGHT / 9 )) ]; then
		RATIO="USPLASH_16_9"
	else
		RATIO="USPLASH_4_3"
	fi

	cat <<EOF
/* Theme definition for $size */
extern struct usplash_pixmap $PGFG_STRUCT;
extern struct usplash_pixmap $PGBG_STRUCT;
extern struct usplash_pixmap $BG_STRUCT;

struct private_hack phack_$size = {
	.progress_fg = &$PGFG_STRUCT,
	.progress_bg = &$PGBG_STRUCT
};

struct usplash_theme $THEMENAME = {
	.version = THEME_VERSION,
	.next = $THEMENEXT,
	.ratio = $RATIO,

	/* Background and font */
	.pixmap = &$BG_STRUCT,
	.font = (struct usplash_font *)&phack_$size,

	/* Palette indexes */
	.background             = PALETTE_BG,
	.progressbar_background = PALETTE_PROGRESS_FG,
	.progressbar_foreground = PALETTE_PROGRESS_BG,
	.text_background        = PALETTE_TEXT_BG,
	.text_foreground        = PALETTE_TEXT_FG,
	.text_success           = PALETTE_TEXT_SUCCESS,
	.text_failure           = PALETTE_TEXT_FAILURE,

	/* Progress bar position and size in pixels */
	.progressbar_x      = $WIDTH/2 - $PROGRESS_WIDTH/2,
	.progressbar_y      = $HEIGHT/2 - $PROGRESS_HEIGHT/2,
	.progressbar_width  = $PROGRESS_WIDTH,
	.progressbar_height = $PROGRESS_HEIGHT,

	/* Text box position and size in pixels */
	.text_x      = TEXT_MARGIN,
	.text_y      = $HEIGHT/2 + $PROGRESS_HEIGHT/2 + TEXT_MARGIN,
	.text_width  = $WIDTH - 2 * TEXT_MARGIN,
	.text_height = $HEIGHT/2 - $PROGRESS_HEIGHT/2 - 2*TEXT_MARGIN,

	/* Text details */
	.line_height  = 15,
	.line_length  = 32,
	.status_width = 25,

	/* Functions */
	.init = t_init,
	.clear_progressbar = t_clear_progressbar,
	.draw_progressbar = t_draw_progressbar,
	.clear_text = t_clear_text,
	.animate_step = t_animate_step,
};

EOF
done

exit 0

