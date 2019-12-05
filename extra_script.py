Import("env")

env.Append(
    LINKFLAGS=[
        # "-mfloat-abi=hard",
        "-mfloat-abi=softfp",
        "-mfpu=fpv4-sp-d16"
    ],

    CCFLAGS=[
        # "-mfloat-abi=hard",
        "-mfloat-abi=softfp",
        "-mfpu=fpv4-sp-d16"
    ]
)