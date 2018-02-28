#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/gcdemo_adc/ADC_Pipeline.c ../src/gcdemo_adc/ADC_SigmaDelta.c ../src/gcdemo_adc/pipeline.c ../src/main.c ../src/gcdemo_adc/lcd_display.c "../src/DEE Emulation 16-bit.c" ../src/FlashOperations.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o ${OBJECTDIR}/_ext/1439034970/pipeline.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1439034970/lcd_display.o "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o" ${OBJECTDIR}/_ext/1360937237/FlashOperations.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d ${OBJECTDIR}/_ext/1439034970/pipeline.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1439034970/lcd_display.o.d "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o.d" ${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o ${OBJECTDIR}/_ext/1439034970/pipeline.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1439034970/lcd_display.o ${OBJECTDIR}/_ext/1360937237/DEE\ Emulation\ 16-bit.o ${OBJECTDIR}/_ext/1360937237/FlashOperations.o

# Source Files
SOURCEFILES=../src/gcdemo_adc/ADC_Pipeline.c ../src/gcdemo_adc/ADC_SigmaDelta.c ../src/gcdemo_adc/pipeline.c ../src/main.c ../src/gcdemo_adc/lcd_display.c ../src/DEE Emulation 16-bit.c ../src/FlashOperations.s


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GC010
MP_LINKER_FILE_OPTION=,--script=p24FJ128GC010.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o: ../src/gcdemo_adc/ADC_Pipeline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/ADC_Pipeline.c  -o ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o: ../src/gcdemo_adc/ADC_SigmaDelta.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/ADC_SigmaDelta.c  -o ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/pipeline.o: ../src/gcdemo_adc/pipeline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/pipeline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/pipeline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/pipeline.c  -o ${OBJECTDIR}/_ext/1439034970/pipeline.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/pipeline.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/pipeline.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/lcd_display.o: ../src/gcdemo_adc/lcd_display.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/lcd_display.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/lcd_display.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/lcd_display.c  -o ${OBJECTDIR}/_ext/1439034970/lcd_display.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/lcd_display.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/lcd_display.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/DEE\ Emulation\ 16-bit.o: ../src/DEE\ Emulation\ 16-bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o".d 
	@${RM} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "../src/DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o: ../src/gcdemo_adc/ADC_Pipeline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/ADC_Pipeline.c  -o ${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/ADC_Pipeline.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o: ../src/gcdemo_adc/ADC_SigmaDelta.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/ADC_SigmaDelta.c  -o ${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/ADC_SigmaDelta.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/pipeline.o: ../src/gcdemo_adc/pipeline.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/pipeline.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/pipeline.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/pipeline.c  -o ${OBJECTDIR}/_ext/1439034970/pipeline.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/pipeline.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/pipeline.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/main.c  -o ${OBJECTDIR}/_ext/1360937237/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1439034970/lcd_display.o: ../src/gcdemo_adc/lcd_display.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1439034970" 
	@${RM} ${OBJECTDIR}/_ext/1439034970/lcd_display.o.d 
	@${RM} ${OBJECTDIR}/_ext/1439034970/lcd_display.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../src/gcdemo_adc/lcd_display.c  -o ${OBJECTDIR}/_ext/1439034970/lcd_display.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1439034970/lcd_display.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1439034970/lcd_display.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/DEE\ Emulation\ 16-bit.o: ../src/DEE\ Emulation\ 16-bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o".d 
	@${RM} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE)  "../src/DEE Emulation 16-bit.c"  -o "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o"  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -O0 -I"../src/gcdemo_adc" -I"../src/gcdemo_utilities" -I"../src/gcdemo_mtouch" -I"../src/gcdemo_usb" -I"../src/" -I"../Microchip/Include" -I"../Microchip/Include/USB" -I"../Microchip/Include/mTouchCap" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/DEE Emulation 16-bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/FlashOperations.o: ../src/FlashOperations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/FlashOperations.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/FlashOperations.s  -o ${OBJECTDIR}/_ext/1360937237/FlashOperations.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1360937237/FlashOperations.o: ../src/FlashOperations.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/FlashOperations.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../src/FlashOperations.s  -o ${OBJECTDIR}/_ext/1360937237/FlashOperations.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -Wa,-MD,"${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/FlashOperations.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../GCLibBuilder.X/dist/Build_All/production/GCLibBuilder.X.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\GCLibBuilder.X\dist\Build_All\production\GCLibBuilder.X.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--heap=2048,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../GCLibBuilder.X/dist/Build_All/production/GCLibBuilder.X.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    ..\GCLibBuilder.X\dist\Build_All\production\GCLibBuilder.X.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=2048,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/integrated_analog.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
