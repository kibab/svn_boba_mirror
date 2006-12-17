;
; //Extend file extention
;
#ifdef NEWSGOLD

        THUMB
        RSEG    PATCH_EXT2_R4R7:CODE:ROOT(1)
	BLX	J_R4R7
        RSEG    PATCH_EXT2_R6R5:CODE:ROOT(1)
	BLX	J_R6R7
        RSEG    PATCH_EXT2_R5R1:CODE:ROOT(1)
	BLX	J_R5R1
	RSEG    PATCH_EXT2_R5R7:CODE:ROOT(1)
	BLX	J_R5R7
        RSEG    PATCH_EXT2_CLUST1:CODE:ROOT(1)
	MOVS	R0,R7
        RSEG    PATCH_EXT2_CLUST2:CODE:ROOT(1)
	MOVS	R0,R7

	RSEG	PATCH_EXT2_IMPL:CODE:ROOT(1)
	BLX	J_EXT2_REALLOC
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	RSEG	PATCH_EXT2_JUMPERS:CODE:NOROOT(2)
	ARM
J_R4R7
	LDR	PC,=I_R4R7
J_R6R7
	LDR	PC,=I_R6R7
J_R5R1
	LDR	PC,=I_R5R1
J_R5R7
	LDR	PC,=I_R5R7
J_EXT2_REALLOC
	EXTERN	EXT2_REALLOC
	LDR	PC,=EXT2_REALLOC

	RSEG	CODE:CODE:NOROOT(2)
	EXTERN	GET_EXT2_TABLE
I_R4R7
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R7,R0
	MOV	R4,#0
	LDMFD   SP!,{R0-R3, PC}
I_R6R7
	ADD	R5,R0,#0
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R6,R0
	LDMFD   SP!,{R0-R3, PC}
I_R5R1
	MOV	R1,#0
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R5,R0
	LDMFD   SP!,{R0-R3, PC}
I_R5R7
        STMFD   SP!,{R0-R3, LR}
	BL	GET_EXT2_TABLE
	MOV	R7,R0
	MOV	R5,#0
	LDMFD   SP!,{R0-R3, PC}
	

#else
        ARM
        RSEG    PATCH_EXT2_R2R3:CODE:ROOT
	BL      R2R3
        MOV     R1, #0
        NOP

        
        RSEG    PATCH_EXT2_R8R10:CODE:ROOT
        ARM
	BL      R8R10
        NOP
        
        
        EXTERN	EXT2_REALLOC
              
	RSEG	PATCH_EXT2_IMPL:CODE:ROOT
        ARM
        
        LDR     LR, [SP,#0x1C]
        MOV     R8, #0
        STMFD   SP!,{R0-R3,LR}
	BL 	EXT2_REALLOC
        MOV     R7, R0
        LDMFD   SP!,{R0-R3,LR}
	STR     R8,[R7]
	NOP
	NOP
	     
             
        RSEG	PATCH_EXT2_IMPL_2:CODE:ROOT
        ARM
        NOP
	NOP
        

	RSEG	CODE:CODE:NOROOT
        EXTERN	GET_EXT2_AREA
R2R3
        STMFD   SP!,{R0,LR}
	BL	GET_EXT2_AREA
        MOV     R3, R0
        ADD     R2, R0, #4
	LDMFD   SP!,{R0,PC}
        
R8R10
        STMFD   SP!,{R0-R3,LR}
	BL	GET_EXT2_AREA
        MOV     R10, R0 
        ADD     R8, R0, #4
	LDMFD   SP!,{R0-R3,PC}


#endif
	END
	