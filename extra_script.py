Import("env")

env.Append(
    LINKFLAGS=[
        # "-mfloat-abi=hard",
        # "-mfloat-abi=softfp",
        # "-mfpu=fpv4-sp-d16"
    ],

    CXXFLAGS=[
        # "-mfloat-abi=hard",
        # "-mfloat-abi=softfp",
        # "-mfpu=fpv4-sp-d16"
    ]
)