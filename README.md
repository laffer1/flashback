flashback
=========

FlashBack is a file backup system for "Flash drives" and other media.
  
Authors:
Byron Heads
Lucas Holt
Chris Tubbs
John Markus

To compile, you must use GNU Make.  This will not work with BSD make.
Tested on Linux (Ubuntu, Fedora), Mac OS X, FreeBSD, and MidnightBSD.

Requires: sqlite3 and libarchive (both included with MidnightBSD 0.2)

FreeBSD includes libarchive with recent versions, you can install
sqlite3 from ports.

This code is licensed under a 2 clause BSD license, however it
currently uses code under other licenses.

urldecode.cpp contains public domain code from an O'Reilly book.

Get libarchive from:
http://people.freebsd.org/~kientzle/libarchive/
