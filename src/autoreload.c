/*
 * autoreload.c -- re-read the notebook when it changes on disk
 *
 * Copyright (C) 2026
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/stat.h>

#include "tree.h"
#include "prefs.h"
#include "ui.h"
#include "ui_cli.h"
#include "autosave.h"
#include "autoreload.h"

static time_t last_mtime = 0;
static int initialized = 0;
static int reload_pending = 0;

/* mtime of the current db_file, or 0 if unavailable / tutorial (disabled). */
static time_t db_mtime (void)
{
	struct stat st;

	if (prefs.db_file[0] == (char) 255 || prefs.db_file[0] == 0)
		return 0;
	if (stat (prefs.db_file, &st) != 0)
		return 0;
	return st.st_mtime;
}

void autoreload_notify_saved (void)
{
	last_mtime = db_mtime ();
	initialized = 1;
	reload_pending = 0;
}

/* re-read db_file into a fresh tree; *posp gets the new position */
static int do_reload (Node **posp)
{
	/* revert frees the current tree via the given node, so hand it the root
	   (not wherever the cursor happens to be); then settle on the first node
	   so the redraw starts cleanly from the top of the fresh tree. */
	time_t loaded = db_mtime ();	/* mtime of the version we are about to read */
	*posp = node_root (*posp);
	*posp = docmd (*posp, "revert");
	*posp = node_top (node_root (*posp));
	autosave_mark_saved ();		/* fresh from disk == in sync */
	/* Baseline is what we loaded — not the post-import time. If the file
	   changed mid-reload, the next check sees a newer mtime and reloads again,
	   so a rapid change is never absorbed/skipped. */
	last_mtime = loaded;
	reload_pending = 0;
	cli_outfunf ("auto-reloaded %s\n", prefs.db_file);
	return 1;
}

int autoreload_check (Node **posp)
{
	time_t m;

	if (!prefs.autoreload)
		return 0;
	if (prefs.db_file[0] == (char) 255 || prefs.db_file[0] == 0)
		return 0;

	m = db_mtime ();


	if (!initialized) {			/* first call: just remember the baseline */
		last_mtime = m;
		initialized = 1;
		return 0;
	}

	/* In readonly mode there are no unsaved edits to protect, so always
	   reload; otherwise reload only when clean (defer while there are edits).
	   The readonly bypass also avoids a stuck view: re-importing on reload can
	   leave the dirty counter set, which would otherwise block every later
	   reload of a file you can't save. */
	if (m != last_mtime && m != 0) {	/* the file changed on disk */
		if (prefs.readonly || autosave_dirty_since_save () == 0)
			return do_reload (posp);
		reload_pending = 1;
	}

	/* a deferred reload becomes safe once edits have been saved away */
	if (reload_pending && (prefs.readonly || autosave_dirty_since_save () == 0))
		return do_reload (posp);

	return 0;
}
