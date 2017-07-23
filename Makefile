################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../doc \
../doc/html \
../doc/datasheet \
../src \
../src/core/Ethernet \
../src/core/Dimmer \
../src/core/Log \
../src/core/SD \
../src/core/Watchdog \
../src/core/Serial/ \
../src/drivers/ \
../src/drivers/W5100 \
../src/modules \
../src/modules/io \
../src/modules/timer \
../src/modules/spi \
../src/modules/usart \
../src/core \
../src/services \
../src/services/EtheRGB \
../src/tools


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../src/core/Dimmer/Dimmer.c \
../src/core/Ethernet/Ethernet.c \
../src/core/Log/Log.c \
../src/core/Serial/Serial.c \
../src/core/Watchdog/Watchdog.c \
../src/drivers/W5100/W5100.c \
../src/main.c \
../src/modules/spi/spi_master.c \
../src/services/EtheRGB/EtheRGB.c \
../src/services/EtheRGB/EtheRGB_Command.c \
../src/services/EtheRGB/EtheRGB_Command_Commands.c \
../src/services/EtheRGB/EtheRGB_Command_Responses.c \
../src/services/EtheRGB/EtheRGB_Config.c \
../src/services/EtheRGB/EtheRGB_Dimmer.c \
../src/services/EtheRGB/EtheRGB_Ethernet.c \
../src/services/EtheRGB/EtheRGB_IO.c \
../src/services/EtheRGB/EtheRGB_Serial.c \
../src/services/EtheRGB/EtheRGB_StateMachine.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
src/core/Dimmer/Dimmer.o \
src/core/Ethernet/Ethernet.o \
src/core/Log/Log.o \
src/core/Serial/Serial.o \
src/core/Watchdog/Watchdog.o \
src/drivers/W5100/W5100.o \
src/main.o \
src/modules/spi/spi_master.o \
src/services/EtheRGB/EtheRGB.o \
src/services/EtheRGB/EtheRGB_Command.o \
src/services/EtheRGB/EtheRGB_Command_Commands.o \
src/services/EtheRGB/EtheRGB_Command_Responses.o \
src/services/EtheRGB/EtheRGB_Config.o \
src/services/EtheRGB/EtheRGB_Dimmer.o \
src/services/EtheRGB/EtheRGB_Ethernet.o \
src/services/EtheRGB/EtheRGB_IO.o \
src/services/EtheRGB/EtheRGB_Serial.o \
src/services/EtheRGB/EtheRGB_StateMachine.o

OBJS_AS_ARGS +=  \
src/core/Dimmer/Dimmer.o \
src/core/Ethernet/Ethernet.o \
src/core/Log/Log.o \
src/core/Serial/Serial.o \
src/core/Watchdog/Watchdog.o \
src/drivers/W5100/W5100.o \
src/main.o \
src/modules/spi/spi_master.o \
src/services/EtheRGB/EtheRGB.o \
src/services/EtheRGB/EtheRGB_Command.o \
src/services/EtheRGB/EtheRGB_Command_Commands.o \
src/services/EtheRGB/EtheRGB_Command_Responses.o \
src/services/EtheRGB/EtheRGB_Config.o \
src/services/EtheRGB/EtheRGB_Dimmer.o \
src/services/EtheRGB/EtheRGB_Ethernet.o \
src/services/EtheRGB/EtheRGB_IO.o \
src/services/EtheRGB/EtheRGB_Serial.o \
src/services/EtheRGB/EtheRGB_StateMachine.o

C_DEPS +=  \
src/core/Dimmer/Dimmer.d \
src/core/Ethernet/Ethernet.d \
src/core/Log/Log.d \
src/core/Serial/Serial.d \
src/core/Watchdog/Watchdog.d \
src/drivers/W5100/W5100.d \
src/main.d \
src/modules/spi/spi_master.d \
src/services/EtheRGB/EtheRGB.d \
src/services/EtheRGB/EtheRGB_Command.d \
src/services/EtheRGB/EtheRGB_Command_Commands.d \
src/services/EtheRGB/EtheRGB_Command_Responses.d \
src/services/EtheRGB/EtheRGB_Config.d \
src/services/EtheRGB/EtheRGB_Dimmer.d \
src/services/EtheRGB/EtheRGB_Ethernet.d \
src/services/EtheRGB/EtheRGB_IO.d \
src/services/EtheRGB/EtheRGB_Serial.d \
src/services/EtheRGB/EtheRGB_StateMachine.d

C_DEPS_AS_ARGS +=  \
src/core/Dimmer/Dimmer.d \
src/core/Ethernet/Ethernet.d \
src/core/Log/Log.d \
src/core/Serial/Serial.d \
src/core/Watchdog/Watchdog.d \
src/drivers/W5100/W5100.d \
src/main.d \
src/modules/spi/spi_master.d \
src/services/EtheRGB/EtheRGB.d \
src/services/EtheRGB/EtheRGB_Command.d \
src/services/EtheRGB/EtheRGB_Command_Commands.d \
src/services/EtheRGB/EtheRGB_Command_Responses.d \
src/services/EtheRGB/EtheRGB_Config.d \
src/services/EtheRGB/EtheRGB_Dimmer.d \
src/services/EtheRGB/EtheRGB_Ethernet.d \
src/services/EtheRGB/EtheRGB_IO.d \
src/services/EtheRGB/EtheRGB_Serial.d \
src/services/EtheRGB/EtheRGB_StateMachine.d

OUTPUT_FILE_PATH +=EtheRGB.elf

OUTPUT_FILE_PATH_AS_ARGS +=EtheRGB.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





































src/core/Dimmer/%.o: ../src/core/Dimmer/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/core/Ethernet/%.o: ../src/core/Ethernet/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/core/Log/%.o: ../src/core/Log/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/core/Serial/%.o: ../src/core/Serial/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/core/Watchdog/%.o: ../src/core/Watchdog/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/drivers/W5100/%.o: ../src/drivers/W5100/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/modules/spi/%.o: ../src/modules/spi/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

src/services/EtheRGB/%.o: ../src/services/EtheRGB/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DNDEBUG -DCONF_DEVICE_USENIC_W5100  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"../src/modules/usart/Serial"  -O3 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="EtheRGB.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega328p -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega328p"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "EtheRGB.elf" "EtheRGB.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "EtheRGB.elf" "EtheRGB.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "EtheRGB.elf" > "EtheRGB.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "EtheRGB.elf" "EtheRGB.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "EtheRGB.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "EtheRGB.elf" "EtheRGB.a" "EtheRGB.hex" "EtheRGB.lss" "EtheRGB.eep" "EtheRGB.map" "EtheRGB.srec" "EtheRGB.usersignatures"
	