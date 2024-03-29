# KingdomsAndCrusaders [demo](https://calderwhite.github.io/KingdomsAndCrusaders/index.html)

<!-- Sadly enough, github markdown doesn't allow me to set the css property image-rendering: pixelate.
     Because of this I am forced to scale up the gifs I use in the md files. Otherwise I could just let the browser do that
     and not have to worry about it fuzzing all the pixels. !-->
<image src="https://github.com/CalderWhite/KingdomsAndCrusaders/raw/storage/gh_preview_600x600.gif?raw=true" />

This repository was originally dedicated to the processing version of this software for my Grade 12 Computer Science class.
The assignment was to simulate anything of your choice using a basic ruleset. A pdf of the design document for the assignment called `design_doc.pdf` describes in detail how the simulation works. If you are looking to understand the algorithms
a bit more, I highly recommend exploring the `processing` directory, as its code is well commented. However, it is not efficient or extensible. For this reason a year after I had written the original project I returned and wrote it in c++ to achieve more complex simulations as well as the ability to generate videos of the simulations.

There are 2 main files, `live.cpp` and `video.cpp`. `live` is 100% cross platform and has been tested on 64 bit windows 10 and ubuntu 18.04. It makes use [SDL](https://www.libsdl.org/) for graphics, and can achieve great speeds in real time. `video` has only been tested in linux environments but should preform on windows.

## Building

[Bazel](https://bazel.build/) was used for this project. There are 2 main targets you need to worry about: `live` and `video`. For `live`, you can run `bazel run live --compilation_mode=opt`. For video, to generate the binary call `bazel build video --compilation_mode=opt` and then call the binary in whichever directory you want it to place the generated video in via `<repository root>/bazel-bin/video`.

To run `live` you must have the dynamic SDL libaries installed on your system. `video` has no third party requirements.

## File Descriptions

| Class/Header/Main File | Purpose                                                                                                                                                                                                                                                                                                                                                                                                          |
|------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| Person                 | This object holds a handful of properties such as the person's type and direction. It could have almost been a struct, but there are some methods that make it friendlier to interact with.                                                                                                                                                                                                                      |
| PersonType             | Defines the enum for the PersonType. This is the essence of the entire simulation. People are either Breeders, Sailors or Settlers. The purpose of each are outlined in the design document.                                                                                                                                                                                                                     |
| Grid                   | This class holds the majority of the logic within the codebase. All algorithms integral to the simulation are here. The Grid manages the terrain as well as the people in the simulation, along with the responsibility generating the terrain and updating the people. It should be noted that the perlin noise algorithms used to generate the terrain are in a seperate class in the `third_party` directory. |
| SDLGrid                | This is based upon the Grid class via inheritance, and only has a couple of its own functions to manage rectangle objects and draw them on command. This allows the actual simulation code to be completely segregated from any large dependancies such as SDL.                                                                                                                                        |
| VideoGrid                 | In a similar fashion to SDLGrid, VideoGrid makes use of inheritance of Grid to access all the member variables. The use of VideoGrid is to render frames in conjunction with the third party library `gifenc`. The videos are gifs and the global pallette is defined inside `VideoGrid.cpp` along with the code that writes directly to a frame buffer.                                                                                                                                                                                                                                                                              |
| FrameCounter           | A concise class to average fps over n number of iterations in SDL. This has no relevance to the actual simulation or CVGrid. It is simply an auxiliary class to `live`                                                                                                                                                                                                                                           |
| live                   | This plays the simulation live in a popup window via SDL. There are many variables that can be tweaked in this program to provide interesting results. However, in order to understand what to tweak some exploration of the original processing code and/or the design document may be necessary.                                                                                                               |
| video                  | Generates a gif of a simulation.                                                                                                                                                                                                                                                            |



## Good settings

`./bazel-bin/video /tmp/out.gif 200 200 5 8 2`

`./bazel-bin/video /tmp/out.gif 600 300 7 8 2 3000`