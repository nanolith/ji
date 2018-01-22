The ji Text Editor
==================

This is a personal project of mine to create a text editor that corresponds to
my needs.

Largely, this is a vi / ex clone that does not attempt to have any deep
compatibility to vi / nvi / vim / neovim, etc.  I don't know if this project
will be useful for anyone else.  It's to scratch a particular itch that I have.

I won't even make the claim that this editor is superior to other editors.  It
is not.  You're probably better off using a different editor.  Since I spend
most of my time in front of a text editor every day, I want deep customizations
that fit *my* usage pattern.  This pattern is likely different than the patterns
of others.  In order to keep things sane, I am designing this editor to have a
rich plugin interface, so that I can focus on "vanilla" ji and "custom" ji.
Most of my particular itches are scratched by the "custom" ji, but "vanilla" ji
is meant to be more general-purpose, even if a bit bare bones.

ej, or how to crawl before walking
==================================

The first piece of this editor is an `ed` or `ex` clone.  While this clone is
vaguely similar to `ex`, it is built from the ground up to be better at
scripting.  Instead of simply writing a tool that reads from file handles for
commands, this tool is designed as an API that can be called directly from ji or
wrapped by the ej executable.  The majority of ej is found in libej.
Applications wishing to perform operations on files may wish to link to libej
for that purpose.

The following core concepts are required for understanding ej.  ej is a line
editor, much like ed or ex.  It sees the world in terms of buffers, which are
doubly linked lists of lines.  Each line is an array of UTF-8 characters.  Lines
can be appeneded, inserted, deleted, or changed.  In order to perform these
operations, a command must be invoked on the buffer.  A command is a pair of
functions.  The first function maps a buffer to another buffer with the given
changes applied.  The second function maps the buffer with the changes applied
back to the original buffer.  As such, each command has an "undo" function.

ej maintains a stack of commands executed on a buffer, so that unlimited undo
and redo can be performed.  Some buffer level commands, like writing a buffer to
a file, do not impact this stack.  This ensures that a buffer can have all of
its modifications undone, and it can be written to file.  Because this stack can
become quite large during a long editing session, ej provides the ability to map
this stack to a database backed by the filesystem.  When an ej buffer is created
through the API, the appropriate command stack can be selected.
