################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/15"}
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/FWMA_Filter/FWMA_Filter.c 

OBJS += \
./User/FWMA_Filter/FWMA_Filter.o 

C_DEPS += \
./User/FWMA_Filter/FWMA_Filter.d 


# Each subdirectory must supply rules for building sources it contributes
User/FWMA_Filter/%.o: ../User/FWMA_Filter/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\Project\Sail-DMM\CS1237\Debug" -I"D:\Project\Sail-DMM\CS1237\Core" -I"D:\Project\Sail-DMM\CS1237\User" -I"D:\Project\Sail-DMM\CS1237\Peripheral\inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

