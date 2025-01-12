// Memory management using flash and ram: https://www.ti.com/lit/an/spra958l/spra958l.pdf

// The user must define CLA_C in the project linker settings if using the
// CLA C compiler
#ifdef CLA_C

// Define a size for the CLA scratchpad area that will be used
// by the CLA compiler for local symbols and temps
// Also force references to the special symbols that mark the
// scratchpad are.

CLA_SCRATCHPAD_SIZE = 0x100;
--undef_sym=__cla_scratchpad_end
--undef_sym=__cla_scratchpad_start
#endif //CLA_C

MEMORY
{
PAGE 0 :
   /* BEGIN is used for the "boot to SARAM" bootloader mode   */

#ifdef _FLASH
   BEGIN           	: origin = 0x080000,   length = 0x000002
#else
   BEGIN           	: origin = 0x000000,   length = 0x000002
#endif

   RAMM0           	: origin = 0x000123,   length = 0x0002DD
   RAMD0           	: origin = 0x00B000,   length = 0x000800
   RAMD1            : origin = 0x00B800,   length = 0x000800

   RAMLS5           : origin = 0x00A800, length = 0x000800

#ifdef CPU1
   RAMGS_0           : origin = 0x012000,   length = 0x002000
#endif

#ifdef CPU2
   RAMGS_0          : origin = 0x01A000, length = 0x001FF8
//   RAMGS15_RSVD : origin = 0x01BFF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
#endif


#ifdef _FLASH

	#ifdef CPU1

	   FLASH           : origin = 0x080002,   length = 0x3FFE	/* on-chip Flash A-B*/
	#endif

	#ifdef CPU2

	   FLASH           : origin = 0x0BC000,   length = 0x3FF0	/* on-chip Flash M-N */
//   FLASHN_RSVD     : origin = 0x0BFFF0, length = 0x000010    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

	#endif

#endif


   RESET           	: origin = 0x3FFFC0,   length = 0x000002

PAGE 1 :

   BOOT_RSVD       : origin = 0x000002, length = 0x000121     /* Part of M0, BOOT rom will use this for stack */
   RAMM1           : origin = 0x000400, length = 0x0003F8
//   RAMM1_RSVD      : origin = 0x0007F8, length = 0x000008     /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   RAMLS0123          	: origin = 0x008000,   length = 0x2000
   RAMLS4      	    : origin = 0x00A000, length = 0x000800


#ifdef CPU1
   RAMGS_1           : origin = 0x00C000,   length = 0x003000
   RAMGS_2           : origin = 0x00F000,   length = 0x003000
#endif

#ifdef CPU2
   RAMGS_1           : origin = 0x014000,   length = 0x003FF8
//   RAMGS11_RSVD : origin = 0x017FF8, length = 0x000008    /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   RAMGS_2          : origin = 0x018000, length = 0x003000
#endif


   CANA_MSG_RAM     : origin = 0x049000,   length = 0x000800
   CANB_MSG_RAM     : origin = 0x04B000,   length = 0x000800

   CLA1_MSGRAMLOW   : origin = 0x001480,   length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x001500,   length = 0x000080


	#ifdef CPU1
	   FLASH_LOG           : origin = 0x084000,   length = 0x38000	/* on-chip Flash C-D-E-F-G*/
	#endif
}

SECTIONS
{
#ifdef _FLASH
	   /* Allocate program areas: */
	   .cinit           : > FLASH      PAGE = 0, ALIGN(8)
	   .text            : > FLASH      PAGE = 0, ALIGN(8)
	   codestart        : > BEGIN       PAGE = 0, ALIGN(8)
	   .stack           : > RAMM1       PAGE = 1
	   .switch          : > FLASH      PAGE = 0, ALIGN(8)

	   /* Allocate uninitalized data sections: */

	#if defined(__TI_EABI__)
	   .init_array         : > FLASH,       PAGE = 0, ALIGN(8)
	   .bss                : > RAMGS_1,       PAGE = 1
	   .bss:output         : > RAMGS_1,       PAGE = 1
	   .data               : > RAMGS_1,       PAGE = 1
	   .sysmem             : > RAMGS_1,       PAGE = 1
	   .const              : > FLASH,       PAGE = 0, ALIGN(8)
	#else
	   .pinit              : > FLASH,       PAGE = 0, ALIGN(8)
	   .ebss               : > RAMGS_1,		 PAGE = 1
	   .esysmem            : > RAMGS_1,       PAGE = 1
	   .econst             : > FLASH   	 PAGE = 0, ALIGN(8)
	#endif

	   .reset           : > RESET,     PAGE = 0, TYPE = DSECT /* not used, */
#else
	   codestart        : > BEGIN,      PAGE = 0
	   .text            : >> RAMD0 | RAMD1 | RAMGS_0,    PAGE = 0
	   .cinit           : > RAMM0,      PAGE = 0
	   .switch          : > RAMM0,      PAGE = 0
	   .reset           : > RESET,      PAGE = 0, TYPE = DSECT /* not used, */
	   .stack           : > RAMM1,      PAGE = 1

	#if defined(__TI_EABI__)
	   .bss             : > RAMGS_1,    PAGE = 1
	   .bss:output      : > RAMGS_1,    PAGE = 1
	   .init_array      : > RAMM0,     PAGE = 0
	   .const           : > RAMGS_1,    PAGE = 1
	   .data            : > RAMGS_1,    PAGE = 1
	   .sysmem          : > RAMGS_1,    PAGE = 1
	#else
	   .pinit           : > RAMM0,     PAGE = 0
	   .ebss            : > RAMGS_1,    PAGE = 1
	   .econst          : > RAMLS4,    PAGE = 1
	   .esysmem         : > RAMGS_1,    PAGE = 1
	#endif
#endif

    /* CLA specific sections */
#ifdef _FLASH
   #if defined(__TI_EABI__)
   		Cla1Prog    : LOAD = FLASH,
                      RUN = RAMLS5,
                      LOAD_START(Cla1funcsLoadStart),
                      LOAD_END(Cla1funcsLoadEnd),
                      RUN_START(Cla1funcsRunStart),
                      LOAD_SIZE(Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(8)
   #else
      	Cla1Prog    : LOAD = FLASH,
                      RUN = RAMLS5,
                      LOAD_START(_Cla1funcsLoadStart),
                      LOAD_END(_Cla1funcsLoadEnd),
                      RUN_START(_Cla1funcsRunStart),
                      LOAD_SIZE(_Cla1funcsLoadSize),
                      PAGE = 0, ALIGN(8)
   #endif
#else
   Cla1Prog         : > RAMLS5, PAGE=0
#endif


   AFDData			: > RAMGS_1, PAGE=1
   CLAData		: > RAMLS0123, PAGE=1
   Cla1ToCpuMsgRAM  : > CLA1_MSGRAMLOW,   PAGE = 1
   CpuToCla1MsgRAM  : > CLA1_MSGRAMHIGH,  PAGE = 1

#ifdef CPU1
   log  : > FLASH_LOG,  PAGE = 1, ALIGN(8)
#endif

#ifdef _FLASH


	#ifdef __TI_COMPILER_VERSION__
	   #if __TI_COMPILER_VERSION__ >= 15009000
	        #if defined(__TI_EABI__)
			    .TI.ramfunc : {} LOAD = FLASH,
								 RUN = RAMD0,
	                	         LOAD_START(RamfuncsLoadStart),
	                 	         LOAD_SIZE(RamfuncsLoadSize),
	                 	         LOAD_END(RamfuncsLoadEnd),
	                  	         RUN_START(RamfuncsRunStart),
	                  	         RUN_SIZE(RamfuncsRunSize),
	                   	         RUN_END(RamfuncsRunEnd),
								 PAGE = 0, ALIGN(8)
			#else
				.TI.ramfunc : {} LOAD = FLASH,
								 RUN = RAMD0,
	                	         LOAD_START(_RamfuncsLoadStart),
	                 	         LOAD_SIZE(_RamfuncsLoadSize),
	                 	         LOAD_END(_RamfuncsLoadEnd),
	                  	         RUN_START(_RamfuncsRunStart),
	                  	         RUN_SIZE(_RamfuncsRunSize),
	                   	         RUN_END(_RamfuncsRunEnd),
								 PAGE = 0, ALIGN(8)
			#endif
	   #else
	   ramfuncs         : LOAD = FLASH,
	                      RUN = RAMD0,
	                      LOAD_START(_RamfuncsLoadStart),
	                      LOAD_SIZE(_RamfuncsLoadSize),
	                      LOAD_END(_RamfuncsLoadEnd),
	                      RUN_START(_RamfuncsRunStart),
	                      RUN_SIZE(_RamfuncsRunSize),
	                      RUN_END(_RamfuncsRunEnd),
	                      PAGE = 0, ALIGN(8)
	   #endif
	#endif
#else
	#ifdef __TI_COMPILER_VERSION__
	   #if __TI_COMPILER_VERSION__ >= 15009000
	    .TI.ramfunc : {} > RAMM0,      PAGE = 0
	   #else
	    ramfuncs    : > RAMM0      PAGE = 0
	   #endif
	#endif
#endif


#ifdef _FLASH
ramconsts: LOAD = FLASH,
			RUN = RAMGS_1, PAGE = 1
			LOAD_START(_ramconsts_loadstart),
			LOAD_SIZE(_ramconsts_loadsize),
			RUN_START(_ramconsts_runstart)
#endif

#ifdef _FLASH
   isrfunc            : LOAD = FLASH,
                         RUN = RAMGS_0,
                         LOAD_START(_isrfuncLoadStart),
                         LOAD_END(_isrfuncLoadEnd),
                         RUN_START(_isrfuncRunStart),
						 LOAD_SIZE(_isrfuncLoadSize),
						 PAGE = 0
#endif

#ifdef CLA_C
   // Must be allocated to memory the CLA has write access to
   #ifdef _FLASH
	      CLAscratch       :
	                     { *.obj(CLAscratch)
	                     . += CLA_SCRATCHPAD_SIZE;
	                     *.obj(CLAscratch_end) } > RAMLS0123,  PAGE = 1

	   .scratchpad      : > RAMLS0123,       PAGE = 1
	   .bss_cla		    : > RAMLS0123,       PAGE = 1
	   .const_cla	    :  LOAD = FLASH,
	                       RUN = RAMLS0123,
	                       RUN_START(_Cla1ConstRunStart),
	                       LOAD_START(_Cla1ConstLoadStart),
	                       LOAD_SIZE(_Cla1ConstLoadSize),
	                       PAGE = 1
   #else
	   CLAscratch       :
	                     { *.obj(CLAscratch)
	                     . += CLA_SCRATCHPAD_SIZE;
	                     *.obj(CLAscratch_end) } >  RAMLS0123,  PAGE = 1

	   .scratchpad      : > RAMLS0123,       PAGE = 1
	   .bss_cla		    : > RAMLS0123,       PAGE = 1
	   .const_cla	    : > RAMLS0123,       PAGE = 1
   #endif
#endif //CLA_C
}
// End of file.
