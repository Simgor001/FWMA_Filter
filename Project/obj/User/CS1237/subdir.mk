################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/15"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/CS1237/CS1237.c \
../User/CS1237/CS1237_bsp.c 

OBJS += \
./User/CS1237/CS1237.o \
./User/CS1237/CS1237_bsp.o 

C_DEPS += \
./User/CS1237/CS1237.d \
./User/CS1237/CS1237_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
User/CS1237/%.o: ../User/CS1237/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\Project\Sail-DMM\CS1237\Debug" -I"D:\Project\Sail-DMM\CS1237\Core" -I"D:\Project\Sail-DMM\CS1237\User" -I"D:\Project\Sail-DMM\CS1237\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

