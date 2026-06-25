#ifndef AUTOSAVE_H
#define AUTOSAVE_H

/* >0 when the in-memory tree has edits not yet written to the real db_file
   (clears only on an explicit save/reload, NOT on autosave to db_file~). */
int autosave_dirty_since_save (void);

/* mark the tree as matching db_file (on explicit save, or after a reload) */
void autosave_mark_saved (void);

#endif /* AUTOSAVE_H */
