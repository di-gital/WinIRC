# WinIRC
## Description
**WinIRC** is a trivial IRC server created as a exercise to learn
the Windows API. The entire application, besides the Visual Studio
project file, is hand-written C and Resource Configuration. This 
project was developed in Microsoft Visual C++ 1.0 on Windows NT 3.1.
The program has no dependencies, and is statically compiled.

While this program is distributed in the hope that it will be useful,
WinIRC is not recommended for general use. Being alpha software, it
has not been thoroughly tested, and does not conform to the revelant
specifications or conventions expected of Internet-facing IRC servers.
Functions besides the very minimum for clients to join channels and
send messages within those channels. Moreover, it does not support
moderation or persistence to any degree.

## Use
Binaries for this program are located under `build/`. WinIRC
expects a `winirc.ini` file located in the same directory as the
executable. 

## Compatiblity
WinIRC is intended to be compatible with all environments supporting
a standard Win32 runtime with WinSock support. That is, all versions
of Windows NT, Windows 95/98/Me, and WINE. The program has **not**
been tested or compiled for Win32s. Similarly, this program has not
been compiled or tested on or for architectures besides 32-bit x86.
The program has been compiled for the Intel386, and accordingly is
compatible with all x86 processors supporting 32-bit protected mode.

Being developed on and for Windows NT 3.1, this project secondarily
serves as a demonstration of the forwards-compatibility of the
Windows API, working on systems developed between 1993 and the
present day.

## License
BSD 2-clause (FreeBSD license)
