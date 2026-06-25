#ifndef AUTORELOAD_H
#define AUTORELOAD_H

#include "tree.h"

/* If the notebook file changed on disk since we last read/wrote it AND there
   are no unsaved edits, re-read it (via the `revert` command) so an externally
   synced change shows up live. *posp is updated to the new tree position.
   Returns 1 if a reload happened. No-op unless prefs.autoreload is set. */
int autoreload_check (Node **posp);

/* Record that we just wrote db_file ourselves, so our own save is not mistaken
   for an external change on the next check. */
void autoreload_notify_saved (void);

#endif /* AUTORELOAD_H */
