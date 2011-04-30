Warning
=======

*CTNETSTRINGS IS EXPERIMENTAL SOFTWARE - DON'T USE IT IN PRODUCTION.*

Introduction
------------

This project is aimed to provide a simple tnetstrings implementation on
top of my C Data Structures library. This library should be compatible
with the reference at http://tnetstrings.org/, and by extension, with
plain old netstrings (http://cr.yp.to/proto/netstrings.txt). If you find
a valid tnetstring that fails then it is a bug and you should report it
(or fork and provide patches, if you would be so kind).

This library requires to be linked against the C Data Structures library
(included in the lib/cds directory).

Compiling
---------

Just run `make`. 
