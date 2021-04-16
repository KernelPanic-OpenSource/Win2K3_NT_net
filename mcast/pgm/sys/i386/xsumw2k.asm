        title  "Compute Checksum"

;/*++
;
; Copyright (c) 1992  Microsoft Corporation
;
; Module Name:
;
;    cksy.asm
;
; Abstract:
;
;    This module implements a function to compute the checksum of a buffer.
;
; Author:
;
;    David N. Cutler (davec) 27-Jan-1992
;
; Revision History:
;
;     Who         When        What
;     --------    --------    ----------------------------------------------
;     mikeab      01-22-94    Pentium optimization
;
; Environment:
;
;    Any mode.
;
; Revision History:
;
;--*/

LOOP_UNROLLING_BITS     equ     5
LOOP_UNROLLING          equ     (1 SHL LOOP_UNROLLING_BITS)

        .386
        .model  small,c

        assume cs:FLAT,ds:FLAT,es:FLAT,ss:FLAT
        assume fs:nothing,gs:nothing

        .xlist
        include callconv.inc
        include ks386.inc
        .list

        .code

;++
;
; ULONG
; tcpxsum(
;   IN ULONG cksum,
;   IN PUCHAR buf,
;   IN ULONG len
;   )
;
; Routine Description:
;
;    This function computes the checksum of the specified buffer.
;
; Arguments:
;
;    cksum - Suppiles the initial checksum value, in 16-bit form,
;            with the high word set to 0.
;
;    buf - Supplies a pointer to the buffer to the checksum buffer.
;
;    len - Supplies the length of the buffer in bytes.
;
; Return Value:
;
;    The computed checksum in 32-bit two-partial-accumulators form, added to
;    the initial checksum, is returned as the function value.
;
;--

cksum   equ     12                      ; stack offset to initial checksum
buf     equ     16                      ; stack offset to source address
len     equ     20                      ; stack offset to length in words

to_checksum_last_word:
        jmp     checksum_last_word

to_checksum_done:
        jmp     checksum_done

to_checksum_dword_loop_done:
        jmp     checksum_dword_loop_done

cPublicProc tcpxsum,3

        push    ebx                     ; save nonvolatile register
        push    esi                     ; save nonvolatile register

        mov     ecx,[esp + len]         ; get length in bytes
        sub     eax,eax                 ; clear computed checksum
        test    ecx,ecx                 ; any bytes to checksum at all?
        jz      short to_checksum_done  ; no bytes to checksum

;
; if the checksum buffer is not word aligned, then add the first byte of
; the buffer to the input checksum.
;

        mov     esi,[esp + buf]         ; get source address
        sub     edx,edx                 ; set up to load word into EDX below
        test    esi,1                   ; check if buffer word aligned
        jz      short checksum_word_aligned ; if zf, buffer word aligned
        mov     ah,[esi]                ; get first byte (we know we'll have
                                        ;  to swap at the end)
        inc     esi                     ; increment buffer address
        dec     ecx                     ; decrement number of bytes
        jz      short to_checksum_done  ; if zf set, no more bytes

;
; If the buffer is not an even number of of bytes, then initialize
; the computed checksum with the last byte of the buffer.
;

checksum_word_aligned:                  ;
        shr     ecx,1                   ; convert to word count
        jnc     short checksum_start    ; if nc, even number of bytes
        mov     al,[esi+ecx*2]          ; initialize the computed checksum
        jz      short to_checksum_done  ; if zf set, no more bytes

;
; Compute checksum in large blocks of dwords, with one partial word up front if
; necessary to get dword alignment, and another partial word at the end if
; needed.
;

;
; Compute checksum on the leading word, if that's necessary to get dword
; alignment.
;

checksum_start:                         ;
        test    esi,02h                 ; check if source dword aligned
        jz      short checksum_dword_aligned ; source is already dword aligned
        mov     dx,[esi]                ; get first word to checksum
        add     esi,2                   ; update source address
        add     eax,edx                 ; update partial checksum
                                        ;  (no carry is possible, because EAX
                                        ;  and EDX are both 16-bit values)
        dec     ecx                     ; count off this word (zero case gets
                                        ;  picked up below)

;
; Checksum as many words as possible by processing a dword at a time.
;

checksum_dword_aligned:
        push    ecx                     ; so we can tell if there's a trailing
                                        ;  word later
        shr     ecx,1                   ; # of dwords to checksum
        jz      short to_checksum_last_word ; no dwords to checksum

        mov     edx,[esi]               ; preload the first dword
        add     esi,4                   ; point to the next dword
        dec     ecx                     ; count off the dword we just loaded
        jz      short to_checksum_dword_loop_done
                                        ; skip the loop if that was the only
                                        ;  dword
        mov     ebx,ecx                 ; EBX = # of dwords left to checksum
        add     ecx,LOOP_UNROLLING-1    ; round up loop count
        shr     ecx,LOOP_UNROLLING_BITS ; convert from word count to unrolled
                                        ;  loop count
        and     ebx,LOOP_UNROLLING-1    ; # of partial dwords to do in first
                                        ;  loop
        jz      short checksum_dword_loop ; special-case when no partial loop,
                                          ;  because fixup below doesn't work
                                          ;  in that case (carry flag is
                                          ;  cleared at this point, as required
                                          ;  at loop entry)
        lea     esi,[esi+ebx*4-(LOOP_UNROLLING*4)]
                                        ; adjust buffer pointer back to
                                        ;  compensate for hardwired displacement
                                        ;  at loop entry point
                                        ; ***doesn't change carry flag***
        jmp     loop_entry[ebx*4]       ; enter the loop to do the first,
                                        ; partial iteration, after which we can
                                        ; just do 64-word blocks
                                        ; ***doesn't change carry flag***

checksum_dword_loop:

DEFLAB  macro   pre,suf
pre&suf:
        endm

TEMP=0
        REPT    LOOP_UNROLLING
        deflab  loop_entry_,%TEMP
        adc     eax,edx
        mov     edx,[esi + TEMP]
TEMP=TEMP+4
        ENDM

checksum_dword_loop_end:

        lea     esi,[esi + LOOP_UNROLLING * 4]  ; update source address
                                        ; ***doesn't change carry flag***
        dec     ecx                     ; count off unrolled loop iteration
                                        ; ***doesn't change carry flag***
        jnz     checksum_dword_loop     ; do more blocks

checksum_dword_loop_done label proc
        adc     eax,edx                 ; finish dword checksum
        mov     edx,0                   ; prepare to load trailing word
        adc     eax,edx

;
; Compute checksum on the trailing word, if there is one.
; High word of EDX = 0 at this point
; Carry flag set iff there's a trailing word to do at this point
;

checksum_last_word label proc           ; "proc" so not scoped to function
        pop     ecx                     ; get back word count
        test    ecx,1                   ; is there a trailing word?
        jz      short checksum_done     ; no trailing word
        add     ax,[esi]                ; add in the trailing word
        adc     eax,0                   ;

checksum_done label proc                ; "proc" so not scoped to function
        mov     ecx,eax                 ; fold the checksum to 16 bits
        ror     ecx,16
        add     eax,ecx
        mov     ebx,[esp + buf]
        shr     eax,16
        test    ebx,1                   ; check if buffer word aligned
        jz      short checksum_combine  ; if zf set, buffer word aligned
        ror     ax,8                    ; byte aligned--swap bytes back
checksum_combine label proc             ; "proc" so not scoped to function
        add     ax,word ptr [esp + cksum] ; combine checksums
        pop     esi                     ; restore nonvolatile register
        adc     eax,0                   ;
        pop     ebx                     ; restore nonvolatile register
        stdRET  tcpxsum


REFLAB  macro   pre,suf
        dd      pre&suf
        endm

        align   4
loop_entry      label   dword
        dd      0
TEMP=LOOP_UNROLLING*4
        REPT    LOOP_UNROLLING-1
TEMP=TEMP-4
        reflab  loop_entry_,%TEMP
        ENDM






stdENDP tcpxsum



LOOP_UNROLLING_BITS_XMMI     equ     4
LOOP_UNROLLING_XMMI          equ     (1 SHL LOOP_UNROLLING_BITS_XMMI)

;VRSTEST EQU     0
ifdef VRSTEST
;
; Test tcpxsum_xmmi for correctness.
tcksum   equ     8[ebp]                   ; stack offset to initial checksum
tbuf     equ     12[ebp]                   ; stack offset to source address
tlen     equ     16[ebp]                  ; stack offset to length in words
        align
cPublicProc tcpxsum_xmmi,3
        ;int     3
        push    ebp
        mov     ebp, esp
        push    ebx
        push    esi
        mov     ebx, offset tcpxsum
        mov     esi, offset tcpxsum_xmmi1
        ; Get a "random" number
        .586p
        rdtsc
        .386p
        and     eax, 10H
        jz      old_then_new
        ; Swap which routine is called first
        push    ebx
        mov     ebx, esi
        pop     esi
old_then_new:
        ; Call the first routine
        push    tlen
        push    tbuf
        push    tcksum
        call    ebx
        ; Save the answer
        push    eax
        ; Call the second routine
        push    tlen
        push    tbuf
        push    tcksum
        call    esi
        ; Check the answer
        cmp     eax, [esp]
        jnz     different_xsum
        ; Same answer, we are done
        pop     eax
        pop     esi
        pop     ebx
        pop     ebp
        stdRET  tcpxsum_xmmi
        align
        ; Different answers, need to debug the problem
different_xsum:
        ; Get both checksums onto the stack
        push    eax
        ; ... and bugcheck
        ;EXTRNP  _KeBugCheck,1,IMPORT
        ;stdCall   _KeBugCheck, <0>
again:
        int     3
        jmp     again
stdENDP tcpxsum_xmmi
endif


;++
;
; ULONG
; tcpxsum_xmmi(
;   IN ULONG cksum,
;   IN PUCHAR buf,
;   IN ULONG len
;   )
;
; Routine Description:
;
;    This function computes the checksum of the specified buffer.
;    It uses Processor's prefetch instruction.
;
; Arguments:
;
;    cksum - Suppiles the initial checksum value, in 16-bit form,
;            with the high word set to 0.
;
;    buf - Supplies a pointer to the buffer to the checksum buffer.
;
;    len - Supplies the length of the buffer in bytes.
;
; Return Value:
;
;    The computed checksum in 32-bit two-partial-accumulators form, added to
;    the initial checksum, is returned as the function value.
;
;--

cksum   equ     12                      ; stack offset to initial checksum
buf     equ     16                      ; stack offset to source address
len     equ     20                      ; stack offset to length in words

to_checksum_last_word_xmmi:
        jmp     checksum_last_word_xmmi

to_checksum_done_xmmi:
        jmp     checksum_done_xmmi

to_checksum_dword_loop_done_xmmi:
        jmp     checksum_dword_loop_done_xmmi

ifdef VRSTEST
cPublicProc tcpxsum_xmmi1,3
else
cPublicProc tcpxsum_xmmi,3
endif

        push    ebx                     ; save nonvolatile register
        push    esi                     ; save nonvolatile register

        mov     ecx,[esp + len]         ; get length in bytes
        sub     eax,eax                 ; clear computed checksum
        test    ecx,ecx                 ; any bytes to checksum at all?
        jz      short to_checksum_done_xmmi  ; no bytes to checksum

;
; if the checksum buffer is not word aligned, then add the first byte of
; the buffer to the input checksum.
;

        mov     esi,[esp + buf]         ; get source address
        sub     edx,edx                 ; set up to load word into EDX below
        test    esi,1                   ; check if buffer word aligned
        jz      short checksum_word_aligned ; if zf, buffer word aligned
        mov     ah,[esi]                ; get first byte (we know we'll have
                                        ;  to swap at the end)
        inc     esi                     ; increment buffer address
        dec     ecx                     ; decrement number of bytes
        jz      short to_checksum_done_xmmi  ; if zf set, no more bytes

;
; If the buffer is not an even number of of bytes, then initialize
; the computed checksum with the last byte of the buffer.
;

checksum_word_aligned:                  ;
        shr     ecx,1                   ; convert to word count
        jnc     short checksum_start    ; if nc, even number of bytes
        mov     al,[esi+ecx*2]          ; initialize the computed checksum
        jz      short to_checksum_done_xmmi  ; if zf set, no more bytes

;
; Compute checksum in large blocks of dwords, with one partial word up front if
; necessary to get dword alignment, and another partial word at the end if
; needed.
;

;
; Compute checksum on the leading word, if that's necessary to get dword
; alignment.
;

checksum_start:                         ;
        test    esi,02h                 ; check if source dword aligned
        jz      short checksum_dword_aligned ; source is already dword aligned
        mov     dx,[esi]                ; get first word to checksum
        add     esi,2                   ; update source address
        add     eax,edx                 ; update partial checksum
                                        ;  (no carry is possible, because EAX
                                        ;  and EDX are both 16-bit values)
        dec     ecx                     ; count off this word (zero case gets
                                        ;  picked up below)

;
; Checksum as many words as possible by processing a dword at a time.
;

checksum_dword_aligned:
        push    ecx                     ; so we can tell if there's a trailing
                                        ;  word later
        shr     ecx,1                   ; # of dwords to checksum
        jz      short to_checksum_last_word_xmmi ; no dwords to checksum

        mov     edx,[esi]               ; preload the first dword
        add     esi,4                   ; point to the next dword
        dec     ecx                     ; count off the dword we just loaded
        jz      short to_checksum_dword_loop_done_xmmi
                                        ; skip the loop if that was the only
                                        ;  dword
        mov     ebx,ecx                 ; EBX = # of dwords left to checksum
        add     ecx,LOOP_UNROLLING_XMMI-1    ; round up loop count
        shr     ecx,LOOP_UNROLLING_BITS_XMMI ; convert from word count to unrolled
                                        ;  loop count
        and     ebx,LOOP_UNROLLING_XMMI-1    ; # of partial dwords to do in first
                                        ;  loop
        jz      short checksum_dword_loop ; special-case when no partial loop,
                                          ;  because fixup below doesn't work
                                          ;  in that case (carry flag is
                                          ;  cleared at this point, as required
                                          ;  at loop entry)
        lea     esi,[esi+ebx*4-(LOOP_UNROLLING_XMMI*4)]
                                        ; adjust buffer pointer back to
                                        ;  compensate for hardwired displacement
                                        ;  at loop entry point
                                        ; ***doesn't change carry flag***
        jmp     xmmi_loop_entry[ebx*4]  ; enter the loop to do the first,
                                        ; partial iteration, after which we can
                                        ; just do 64-word blocks
                                        ; ***doesn't change carry flag***

checksum_dword_loop:
        ; prefetch the 32-byte cache line from [esi+0]
        db 0fH
        db 18H
        db 46H
        db 00H

        ; prefetch the 32-byte cache line from [esi+20h]
        db 0fH
        db 18H
        db 46H
        db 20H

        ; prefetch the 32-byte cache line from [esi+40h]
        db 0fH
        db 18H
        db 46H
        db 40H

        ; prefetch the 32-byte cache line from [esi+60h]
        db 0fH
        db 18H
        db 46H
        db 60H

DEFLAB  macro   pre,suf
pre&suf:
        endm

TEMP=0
        REPT    LOOP_UNROLLING_XMMI
        deflab  xmmi_loop_entry_,%TEMP
        adc     eax,edx
        mov     edx,[esi + TEMP]
TEMP=TEMP+4
        ENDM

checksum_dword_loop_end:

        lea     esi,[esi + LOOP_UNROLLING_XMMI * 4]  ; update source address
                                        ; ***doesn't change carry flag***
        dec     ecx                     ; count off unrolled loop iteration
                                        ; ***doesn't change carry flag***
        jnz     checksum_dword_loop     ; do more blocks

checksum_dword_loop_done_xmmi label proc
        adc     eax,edx                 ; finish dword checksum
        mov     edx,0                   ; prepare to load trailing word
        adc     eax,edx

;
; Compute checksum on the trailing word, if there is one.
; High word of EDX = 0 at this point
; Carry flag set iff there's a trailing word to do at this point
;

checksum_last_word_xmmi label proc      ; "proc" so not scoped to function
        pop     ecx                     ; get back word count
        test    ecx,1                   ; is there a trailing word?
        jz      short checksum_done_xmmi; no trailing word
        add     ax,[esi]                ; add in the trailing word
        adc     eax,0                   ;

checksum_done_xmmi label proc           ; "proc" so not scoped to function
        mov     ecx,eax                 ; fold the checksum to 16 bits
        ror     ecx,16
        add     eax,ecx
        mov     ebx,[esp + buf]
        shr     eax,16
        test    ebx,1                   ; check if buffer word aligned
        jz      short checksum_combine_xmmi  ; if zf set, buffer word aligned
        ror     ax,8                    ; byte aligned--swap bytes back
checksum_combine_xmmi label proc        ; "proc" so not scoped to function
        add     ax,word ptr [esp + cksum] ; combine checksums
        pop     esi                     ; restore nonvolatile register
        adc     eax,0                   ;
        pop     ebx                     ; restore nonvolatile register
        stdRET  tcpxsum


REFLAB  macro   pre,suf
        dd      pre&suf
        endm

        align   4
xmmi_loop_entry      label   dword
        dd      0
TEMP=LOOP_UNROLLING_XMMI*4
        REPT    LOOP_UNROLLING_XMMI-1
TEMP=TEMP-4
        reflab  xmmi_loop_entry_,%TEMP
        ENDM

ifdef VRSTEST
stdENDP tcpxsum_xmmi1
else
stdENDP tcpxsum_xmmi
endif




;++
;
; PSINGLE_LIST_ENTRY
; FASTCALL
; InterlockedFlushSList (
;    IN PSINGLE_LIST_ENTRY ListHead
;    )
;
; Routine Description:
;
;    This function removes the entire list from a sequenced singly
;    linked list so that access to the list is synchronized in an MP system.
;    If there are no entries in the list, then a value of NULL is returned.
;    Otherwise, the address of the entry at the top of the list is removed
;    and returned as the function value and the list header is set to point
;    to NULL.
;
;    N.B. The cmpxchg8b instruction is only supported on some processors.
;         If the host processor does not support this instruction, then
;         then following code is patched to contain a jump to the normal
;         pop entry code which has a compatible calling sequence and data
;         structure.
;
; Arguments:
;
;    (ecx) = ListHead - Supplies a pointer to the sequenced listhead from
;         which the list is to be flushed.
;
; Return Value:
;
;    The address of the entire current list, or NULL if the list is
;    empty.
;
;--

cPublicProc InterlockedFlushSList, 1



;
; Save nonvolatile registers and read the listhead sequence number followed
; by the listhead next link.
;
; N.B. These two dwords MUST be read exactly in this order.
;


        push    ecx

        push    ebx                     ; save nonvolatile registers
        push    ebp                     ;
        mov     ecx, [esp+16]
        mov     ebp, ecx                ; save listhead address
        mov     edx, [ebp] + 4          ; get current sequence number
        mov     eax, [ebp] + 0          ; get current next link

;
; N.B. The following code is the retry code should the compare
;      part of the compare exchange operation fail
;
; If the list is empty, then there is nothing that can be removed.
;

Efls10: or      eax, eax                ; check if list is empty
        jz      short Efls20            ; if z set, list is empty
        mov     ecx, 0	                ; clear sequence number and depth
        mov     ebx, 0                  ; clear successor entry pointer

.586
ifndef NT_UP

   lock cmpxchg8b qword ptr [ebp]       ; compare and exchange

else

        cmpxchg8b qword ptr [ebp]       ; compare and exchange

endif
.386

        jnz     short Efls10            ; if z clear, exchange failed

;
; Restore nonvolatile registers and return result.
;



Efls20: pop     ebp                     ; restore nonvolatile registers
        pop     ebx                     ;
        pop     ecx

        stdRET    InterlockedFlushSList

stdENDP InterlockedFlushSList







        end
