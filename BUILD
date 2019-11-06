cc_library(
    name = "person_type",
    hdrs = ["PersonType.h"],
)

cc_library(
    name = "person",
    srcs = ["Person.cpp"],
    hdrs = ["Person.h"],
    deps = [
        ":person_type",
    ]
)

cc_library(
    name = "frame_counter",
    srcs = ["FrameCounter.cpp"],
    hdrs = ["FrameCounter.h"],
    copts = [
        "-I/usr/include/SDL2",
    ],
    linkopts = [
        "-lSDL2",
        "-D_REENTRANT",
        "-ldl"
    ],
)

cc_library(
    name = "grid",
    srcs = ["Grid.cpp"],
    hdrs = ["Grid.h"],
    deps = [
        ":person",
        ":person_type",
        "//third_party:perlin_noise",
    ],
)

cc_library(
    name = "sdl_grid",
    srcs = ["SDLGrid.cpp"],
    hdrs = ["SDLGrid.h"],
    deps = [
        ":grid",
    ],
    copts = [
        "-I/usr/include/SDL2",
    ],
    linkopts = [
        "-lSDL2",
        "-D_REENTRANT",
        "-ldl"
    ],
)

cc_binary(
    name = "live",
    srcs = ["live.cpp"],
    deps = [
        ":sdl_grid",
        ":frame_counter",
    ],
    data = ["fonts/freefont-20051206/FreeMonoBold.ttf"],
    copts = [
        "-I/usr/include/SDL2",
    ],
    linkopts = [
        "-lSDL2",
        "-lSDL2_ttf",
        "-D_REENTRANT",
        "-ldl"
    ],
)

cc_library(
    name = "cv_grid",
    srcs = ["CVGrid.cpp"],
    hdrs = ["CVGrid.h"],
    deps = [
        ":grid",
    ],
    copts = [
        "-I/usr/local/include/opencv4",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lopencv_imgproc",
        "-lopencv_core",
    ],
)

cc_binary(
    name = "video",
    srcs = ["video.cpp"],
    deps = [
        "cv_grid",
    ],
    copts = [
        "-I/usr/local/include/opencv4",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lopencv_core",
        "-lopencv_imgproc",
        "-lopencv_highgui",
        "-lopencv_video",
        "-lopencv_videoio",
    ],
)
