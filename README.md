# GeforceNowAMD
Unlocks 240FPS option on AMD GPUs inside Geforce Now

## Features
- Allows you to use 240FPS on an AMD GPU
- Open network debug stats by pressing the [HOME] key

## How it works
Geforce NOW client has logic to check if you are running on an AMD GPU, if you are, it disables some options like the capability to run at 240 FPS. `GeforceNowAMD` simply disables that check.

## How to use
1. Download the latest release from the [Releases](https://github.com/ShufflePerson/GeforceNowAMD/releases)
2. Extract the zip
3. Run `launcher.exe`
4. Run Geforce Now

**Note:** Your Anti-Virus might delete one or more files, this is due to the project using injection which is the same technique malware uses. The code is quite small and you can look through it and even compile it yourself.

## Compiling it yourself

If you wish to compile it yourself, have cmake and Visual Studio 2022 installed. Then simply run these commands:

```
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

The built DLL will be inside `build/Release/GFNAmd.dll`

The injector can be built with just `g++`


## Support
If you encounter any issues feel free to reach out to me on my Discord: `_.shuffle._`
