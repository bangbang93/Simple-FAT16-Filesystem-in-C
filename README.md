Simple-FAT16-Filesystem-in-C
============================

A simple FAT16 filesystem in C (Please read Readme.txt)

This is a Simple File System using FAT12.

This file has 4 other files that do one of these tasks.

diskinfo: This will get the information of the disk from the boot sector.

dislist: This will list all the files and directory on the IMG.

diskget: This will get a file from the IMG and put it on the current linux root.


The format of the command line is:

"program name" + "IMG name" + "File desired"

I have included a mock disk-image (disk.IMA) for testing purposes.
