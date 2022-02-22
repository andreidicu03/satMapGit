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
[Barbat & Sebus](https://choosealicense.com/licenses/mit/)