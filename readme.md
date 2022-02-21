# SatMap

SatMap is a QT application for dealing with satellites.

## Installation

Install the [QMake](https://doc.qt.io/qt-5/qmake-manual.html) build utility.

### Debian

For Debian based distros it is located in the apt repository.

```bash
sudo apt install qtbase5-dev # core dependencies
sudo apt install libmarble-dev # 3d map
sudo apt install qt5-qmake # lmao
```

### Arch

For Arch it is in pacman repository

```
sudo pacman -Syuu qt5-base
```

# All

Clone the repository and Create a build directory.

```bash
git clone https://github.com/BarbatBoss03/satMapGit.git
mkdir build
cd build
qmake
```

## Usage

```python
import foobar

# returns 'words'
foobar.pluralize('word')

# returns 'geese'
foobar.pluralize('goose')

# returns 'phenomenon'
foobar.singularize('phenomena')
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[Barbat & Sebus](https://choosealicense.com/licenses/mit/)