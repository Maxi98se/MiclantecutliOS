;[org 0x7c00]
[bits 16]

KERNEL_OFFSET equ 0x9000
KERNEL_SIZE equ 15
STACK_POINTER equ 0x80000

; Boot

mov bx, boot_msg
call println

mov [BOOT_DRIVE], dl

mov dl, [BOOT_DRIVE]
mov bx, KERNEL_OFFSET

mov ah, 0x02
mov al, KERNEL_SIZE
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02

int 0x13

cmp al, KERNEL_SIZE
jne disk_error

; Read Memory

begine820:
  pusha

  mov di, 0x7e04
  xor ebx, ebx
  xor bp, bp
  mov edx, 0x534d4150
  xor eax, eax
  mov eax, 0xe820
  mov ecx, 24
  
  int 0x15
  mov edx, 0x534d4150

  test ebx, ebx
  je note820

  jmp aftere820

contiuee820:
  mov eax, 0xe820
  mov ecx, 24

  int 0x15
  jc ende820
  mov edx, 0x534d4150

aftere820:
  jcxz skipe820
  inc bp
  add di, 24

skipe820:
  test ebx, ebx
  jne contiuee820

ende820:
  mov [0x7e00], bp
  clc
  jmp endofe820

note820:
  call mem_error
  jmp $

endofe820:
  popa

; Enter Protected Mode

cli

in al, 0x92
or al, 2
out 0x92, al

lgdt [gdt_descriptor]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp codeseg:StartProtectedMode

; Global Descriptor Table (GDT)

gdt_null:
  dd 0
  dd 0

gdt_code:
  dw 0xffff
  dw 0x0000
  db 0x00
  db 10011010b
  db 11001111b
  db 0x00

gdt_data:
  dw 0xffff
  dw 0x0000
  db 0x00
  db 10010010b
  db 11001111b
  db 0x00

gdt_end:

gdt_descriptor:
  gdt_size:
    dw gdt_end - gdt_null - 1
    dd gdt_null

codeseg equ gdt_code - gdt_null
dataseg equ gdt_data - gdt_null

; Functions

disk_error:
  mov bx, disk_error_msg
  call println
  ret

mem_error:
  mov bx, mem_error_msg
  call println
  ret

; Print

print:
  mov ah, 0x0e  ;text mode

  mov al, [bx]  ;load character into register

  cmp al, 0x00
  jz printend

  int 0x10      ;call write interrupt
  inc bx
  jmp print

printend:

  ret
  
println:            ; adds at the end of the print a newline symbol
  call print
  mov al, 0x0A
  int 0x10
  mov al, 0x0D
  int 0x10
  ret

clear:
  mov ah, 0x0F
  int 0x10
  mov ah, 0x00
  int 0x10
  ret

; Strings

boot_msg:
  db 'Loading Kernel...', 0

disk_error_msg:
  db 'Error Reading Disk', 0

mem_error_msg:
  db 'Error rading Memory', 0

; Var

BOOT_DRIVE:
  db 0x00


[bits 32]

StartProtectedMode:

  mov ax, dataseg
  mov     ss, ax
  mov     ds, ax
  mov     es, ax
  mov     fs, ax
  mov     gs, ax

  MOV  ESP, STACK_POINTER

  call KERNEL_OFFSET

jmp $


times 510-($-$$) db 0

dw 0xaa55
