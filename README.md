# Tetris
A tetris clone written using C++, SDL, and my framework utilityKnife.

## Screenshots ##
<img width="33%" alt="Early game - with a T-spin" src="https://github.com/user-attachments/assets/750c855c-387b-4d15-85b0-a2d3938a658a" /> <img width="33%" alt="Mid-game" src="https://github.com/user-attachments/assets/eb751d37-ce00-4901-84e9-61294a6d1cc5" /> <img width="33%" alt="Game over" src="https://github.com/user-attachments/assets/661ba5f1-1235-411e-aeec-8584ed6b0fea" />

## Controls ## 
* Move left, right - Left Arrow, Right Arrow
* Rotate clockwise - Up Arrow, X
* Rotate counterclockwise - Left Ctrl, Z
* Soft drop - Down Arrow
* Hard drop - Spacebar
* Hold piece - C
* Pause - ESC
* Resume - Enter

## Build instructions ##

### Linux ###

You need: 
* git (2.13+ recommended)
* ninja (recommend for building with)
* CMake (3.27+)
* GCC 10.1+ (must support C++20 for \<numbers> and std::ranges)

```
git clone --recurse-submodules -j8 https://github.com/danelbro/tetris.git tetris
cd tetris
./config.sh [Debug/Release]
./build.sh [Debug/Release]
./run.sh [Debug/Release]
```

### Windows ###

To play, you can grab a release for the binary - just unzip and run tetris.exe, ensuring that the .exe is in the same folder as the 'data' directory.

Otherwise:

```
git clone --recurse-submodules -j8 https://github.com/danelbro/tetris.git tetris
```

If you use Visual Studio with CMake support, you should then be able to open the folder within Visual Studio to configure, build and run it. You'll need at least Visual Studio 2019 16.10.

I recommend the following CMake options (these are baked into the .config.sh script):

```
BUILD_SHARED_LIBS=OFF
SDLTTF_VENDORED=ON	// requires that the repository is cloned recursively - otherwise you'll need SDL and SDL_ttf installed on your system
SDL_TEST_LIBRARY=OFF
```

Using MinGW you should be able to build by following the Linux instructions. It won't build with Cygwin as it is no longer supported by SDL. 
