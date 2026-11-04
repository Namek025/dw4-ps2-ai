/* Replacement for memmove at 0x127148 (mods/, "Cargas: memmove de a 4 bytes").
   mips-linux-gnu-as -march=r5900 -EL tools/memmove_fast.s -o x.o; objcopy -O binary -j .text x.o x.bin */
    .set noreorder
    .set noat
    .text
/* memmove(dst a0, src a1, n a2) -> dst. Word copies when dst, src and n are 4-byte aligned. */
memmove:
    move    $2, $4
    or      $8, $4, $5
    or      $8, $8, $6
    andi    $8, $8, 3
    sltu    $9, $5, $4
    beqz    $9, fwd
    addu    $10, $5, $6
    sltu    $9, $4, $10
    beqz    $9, fwd
    addu    $7, $4, $6
    bnez    $8, bback
    nop
wback:
    beqz    $6, done
    nop
    addiu   $10, $10, -4
    lw      $11, 0($10)
    addiu   $7, $7, -4
    addiu   $6, $6, -4
    sw      $11, 0($7)
    b       wback
    nop
bback:
    beqz    $6, done
    nop
    addiu   $10, $10, -1
    lbu     $11, 0($10)
    addiu   $7, $7, -1
    addiu   $6, $6, -1
    sb      $11, 0($7)
    b       bback
    nop
fwd:
    bnez    $8, bfwd
    move    $7, $4
wfwd:
    beqz    $6, done
    nop
    lw      $11, 0($5)
    addiu   $5, $5, 4
    addiu   $6, $6, -4
    sw      $11, 0($7)
    b       wfwd
    addiu   $7, $7, 4
bfwd:
    beqz    $6, done
    nop
    lbu     $11, 0($5)
    addiu   $5, $5, 1
    addiu   $6, $6, -1
    sb      $11, 0($7)
    b       bfwd
    addiu   $7, $7, 1
done:
    jr      $31
    nop
