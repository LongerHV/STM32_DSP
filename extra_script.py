Import("env")

env.Append(
    ASFLAGS=[
        "-mfpu=fpv5-d16",
        "-mfloat-abi=hard"
    ],

    CCFLAGS=[
        "-mfpu=fpv5-d16",
        "-mfloat-abi=hard"
    ],

    LINKFLAGS=[
        "-mfpu=fpv5-d16",
        "-mfloat-abi=hard",
        "-Wl,-T${PROJECT_DIR}/STM32H743ZI_FLASH.ld"
    ]
)