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
        ":frame_counter",
        "//third_party:perlin_noise",
    ],
    data = ["fonts/freefont-20051206/FreeMonoBold.ttf"],
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
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        "grid",
    ],
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