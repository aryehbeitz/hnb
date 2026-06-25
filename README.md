# hnb — Hierarchical NoteBook (auto-reload fork)

A fork of [hnb](http://hnb.sourceforge.net/) (Hierarchical NoteBook), the
curses-based outliner by Øyvind Kolås, with features layered on top of the
classic 1.9.18 release.

## What this fork adds

- **Auto-reload** — when the open notebook file changes on disk (e.g. synced
  in from another machine), hnb re-reads it and redraws live, instead of
  showing a stale in-memory copy until you quit and reopen. Reload is deferred
  while you have unsaved edits, so nothing you've typed is lost. Toggle with the
  `autoreload` setting in `~/.hnbrc`.

## Provenance & license

- Based on upstream **hnb 1.9.18** (© Øyvind Kolås, et al.).
- Includes the **Debian** packaging patch series (modern-compiler fixes:
  callback signatures, format-security, implicit pointer conversions, ncursesw).
- Plus a couple of macOS build fixes (portable `printf` for `hnbrc.inc`,
  forcing the generated `init_subsystems.o` into the link).

Distributed under the **GNU General Public License v2.0 or later** — same as
upstream. See `COPYING`.

## Build

```sh
# Linux: needs libncursesw-dev
make
sudo make install            # /usr/local/bin/hnb

# macOS (Homebrew ncurses):
make CC=clang \
  CPPFLAGS="-I$(brew --prefix ncurses)/include -I$(brew --prefix ncurses)/include/ncursesw" \
  LDFLAGS="-L$(brew --prefix ncurses)/lib"
```

## Usage

```sh
hnb [database]      # default database: ~/.hnb
hnb -t              # load the tutorial
```
Arrows navigate; `Enter` adds a sibling; `Tab` indents; `Space` toggles to-do.
