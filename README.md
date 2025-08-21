# STFixes-metamod
A stripped down build of CS2Fixes with only surf/bhop related fixes (movement unlocker, botnavignore, water floor jump fix, trigger_push fix, trigger_gravity fix)

## Installation

- Install [Metamod](https://cs2.poggu.me/metamod/installation/)
- Build the plugin using the [compilation instructions below](https://github.com/SharpTimer/STFixes-metamod/tree/main?tab=readme-ov-file#instructions) or download the [latest release](https://github.com/SharpTimer/STFixes-metamod/releases/latest)
- Extract the package contents into `game/csgo` on your server

## Configuration
```cs2f_movement_unlocker_enable	0``` - keep this 0 if you are using ServerMovementUnlocker standalone, if not, set to 1
```cs2f_use_old_push 				    0``` - set this to 1 for maps with broken trigger_push

## Compilation

### Requirements

- [Metamod:Source](https://www.sourcemm.net/downloads.php/?branch=master) (build 1219 or higher)
- [AMBuild](https://wiki.alliedmods.net/Ambuild)

### Instructions

Follow the instructions below to compile CS2Fixes.

```bash

git clone https://github.com/SharpTimer/STFixes-metamod && cd STFixes-metamod
git submodule update --init --recursive

export MMSOURCE112=/path/to/metamod/
export HL2SDKCS2=/path/to/sdk/submodule

mkdir build && cd build
python3 ../configure.py --enable-optimize --sdks cs2
ambuild
```

Copy the contents of package/ to your server's csgo/ directory.
