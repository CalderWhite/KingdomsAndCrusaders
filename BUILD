cc_library(
    name = "position",
    hdrs = ["Position.h"],
)

cc_library(
    name = "person_type",
    hdrs = ["PersonType.h"],
)

cc_library(
    name = "person",
    srcs = ["Person.cpp"],
    hdrs = ["Person.h"],
    deps = [
        ":position",
        ":person_type",
    ]
)

cc_library(
    name = "grid",
    srcs = ["Grid.cpp"],
    hdrs = ["Grid.h"],
    deps = [
        ":person",
        ":position",
        ":person_type",
    ],
    copts = [
        "-I/usr/include/SDL2",
    ],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
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
        "-lSDL2_image",
        "-D_REENTRANT",
        "-ldl"
    ],
)