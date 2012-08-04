## libggpk

---

libggpk is a C++ library for extracting information from the ggpk archive used in the game Path of Exile by Grinding Gear Games.  Path of Exile is an excellent free-to-play action RPG available at http://www.pathofexile.com/ and you should definitely check it out.

*This software DOES NOT modify the game or the data used by the game in any way.  It is intended for educational use only.*

### Dependencies

libggpk uses the Qt4 library to read large files (64-bit file offsets) in a cross-platform way.  The Qt library is available at http://qt.nokia.com/downloads

### Usage

Most of what you probably want to accomplish can be done by using the 'ggpk' utility included in tools/ggpk -- this is a command line program that can extract files from the archive, list all files contained in the archive, or print information about the files contained in the archive.

### License

libggpk is licensed under the MIT license, the text of which is located within the LICENSE file that should be included with this source distribution.