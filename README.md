# Ktrayshortcut

Assign global shortcuts for toggling X11 applications using KDE components to manage them.

## Warning notice

This is my first Qt/KDE application and there's a long time since the last time I
programmed in C++ so don't expect a well organized application code or any code
conventions. Comments, reviews, suggestions and pull requests are mostly welcomed.

# Known bugs

This is in a very early stage and is not suitable for production usage yet.

Here's a list of known bugs:

- The application icon is not correctly picked when running from qtcreator (it doesn't
seem to happen when running it directly);
- The original window position may not be restored for some applications like gVim;

# Missing features

- storing and restoring global shortcuts by application name

# TODO

There's a lot to organize in the code internals. I just wanted to get an idea on the
minimum requirements to get it kind of working.

