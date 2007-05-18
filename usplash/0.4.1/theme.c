/* usplash
 *
 * theme.c - theme definition
 *
 * Copyright © 2006 David Härdeman <david@hardeman.nu>
 *   based on example-theme from usplash which is
 *   Copyright © 2006 Dennis Kaarsemaker <dennis@kaarsemaker.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <usplash-theme.h>
#include <usplash_backend.h>

void t_init(struct usplash_theme *theme);
void t_draw_progressbar(struct usplash_theme *theme, int percentage);
void t_clear_progressbar(struct usplash_theme *theme);
void t_clear_text(struct usplash_theme *theme);
void t_animate_step(struct usplash_theme* theme, int pulsating);

/* Auto-generated theme definitions */
#include "theme.h"

static struct private_hack *phack = NULL;

void
t_init(struct usplash_theme *theme)
{
	int x, y;

	/* START  - HACKS R US */
	phack = (struct private_hack *)theme->font;
	theme->font = NULL;
	/* FINISH - HACKS R US */
	usplash_getdimensions(&x, &y);
	theme->progressbar_x = (x - theme->pixmap->width)/2 + theme->progressbar_x;
	theme->progressbar_y = (y - theme->pixmap->height)/2 + theme->progressbar_y;
}

void
t_draw_progressbar(struct usplash_theme *theme, int percentage)
{
	int w = (phack->progress_bg->width * percentage / 100);
	static int last_w = 0;

	if (w != 0 && w == last_w)
		return;

	/* Check if we're changing mode between startup and shutdown */
	if (last_w >= 0 && w < 0) {
		usplash_put(theme->progressbar_x,
			    theme->progressbar_y,
			    phack->progress_fg);
		last_w = 0;
	} else if (last_w < 0 && w > 0) {
		usplash_put(theme->progressbar_x,
			    theme->progressbar_y,
			    phack->progress_bg);
		last_w = 0;
	}

	/* Draw only the part which has changed since last time */
	if (percentage == 0) {
		usplash_put(theme->progressbar_x,
			    theme->progressbar_y,
			    phack->progress_bg);
	} else if (percentage < 0) {
		usplash_put_part(theme->progressbar_x - last_w,
				 theme->progressbar_y,
				 -w + last_w,
				 phack->progress_bg->height,
				 phack->progress_bg,
				 -last_w, 0);
	} else if (percentage > 0) {
		usplash_put_part(theme->progressbar_x + last_w,
				 theme->progressbar_y,
				 w - last_w,
				 phack->progress_fg->height,
				 phack->progress_fg,
				 last_w, 0);
	}

	last_w = w;
}

void
t_clear_progressbar(struct usplash_theme *theme)
{
	t_draw_progressbar(theme, 0);
}

void
t_clear_text(struct usplash_theme *theme)
{
	int x, y, x1, y1, x2, y2;
	usplash_getdimensions(&x, &y);

	/* Draw outer border */
	x1 = (x - theme->pixmap->width)/2  + theme->text_x - TEXT_EXTRA_PX;
	y1 = (y - theme->pixmap->height)/2 + theme->text_y - TEXT_EXTRA_PX;
	x2 = x1 + theme->text_width + 2 * TEXT_EXTRA_PX;
	y2 = y1 + theme->text_height + 2 * TEXT_EXTRA_PX;
	usplash_clear(x1, y1, x2, y2, theme->text_foreground);

	/* Draw main text area with TEXT_BORDER_PX pixels padding */
	x1 += TEXT_BORDER_PX;
	y1 += TEXT_BORDER_PX;
	x2 -= TEXT_BORDER_PX;
	y2 -= TEXT_BORDER_PX;
	usplash_clear(x1, y1, x2, y2, theme->text_background);
}

void
t_animate_step(struct usplash_theme* theme, int pulsating)
{
	static int pulsate_step = 0;
	static int pulse_width = 70;
	static int step_width = 1;
	static int prev_step = 0;
	static int reset = 1;
	int num_steps = (phack->progress_fg->width - pulse_width)/step_width;
	int x1;

	if (!pulsating) {
		reset = 1;
		return;
	}

	if (reset) {
		/* Reset pulse */
		t_clear_progressbar(theme);
		reset = 0;
		pulsate_step = 0;
		usplash_put_part(theme->progressbar_x,
				 theme->progressbar_y,
				 pulse_width,
				 phack->progress_fg->height,
				 phack->progress_fg,
				 0, 0);
	} else {
		/* Remove from left hand side of pulse */
		x1 = step_width * prev_step;
		usplash_put_part(theme->progressbar_x + x1,
				 theme->progressbar_y,
				 step_width,
				 phack->progress_bg->height,
				 phack->progress_bg,
				 x1, 0);
	}

	/* Add to right hand side of pulse */
	x1 = step_width * pulsate_step + pulse_width;
	usplash_put_part(theme->progressbar_x + x1,
			 theme->progressbar_y,
			 step_width,
			 phack->progress_fg->height,
			 phack->progress_fg,
			 x1, 0);

	/* Check for wraparound */
	prev_step = pulsate_step;
	pulsate_step++;
	if (pulsate_step >= num_steps)
		reset = 1;
}

