# DotVillage

Mini city building game based on the Scratch game by BIG-RED-BUTTON

Original: https://scratch.mit.edu/projects/2269094/

Gif:

![](Data/README/gif.gif)
## Building and Running

### Linux

Requires conan w/ bincrafters and cmake.

```sh
python3 -m pip install conan
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan remote update bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

To build, at the root of the project:

```sh
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release
```
