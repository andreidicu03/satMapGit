# SatMap

SatMap is a [QT](https://wiki.qt.io/About_Qt) application for dealing with satellites.

## Pre-Installation

### Debian Dependencies

```bash
sudo apt install build-essential
sudo apt install qtbase5-dev
sudo apt install libmarble-dev
```

### Arch Dependencies

```bash
sudo pacman -Syu qt5-base
sudo pacman -Syu marble-common
```

## Building

Build using [QMake](https://doc.qt.io/qt-5/qmake-manual.html).
...Installation Flags...

```bash
git clone https://github.com/BarbatBoss03/satMapGit.git
mkdir build; cd build
qmake ../satMap.pro
make #&& sudo make install
```

## Usage

```bash

./satMap

```

## Algorithm

IAU1980 Nutation & IAU 1976 Precession Models

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
MIT License

Copyright (c) 2024 Andrei Dicu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
