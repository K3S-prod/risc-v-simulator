# Prepare testing binary

```bash
riscv64-unknown-linux-gnu-gcc ./fibbonacci.c -o ./fibbonacci_riscv -march=rv32g -mabi=ilp32d -nostdlib
```
## Disassembly example:
NB:
1. Files should be compiled with `-nostdlib` flag;
2. `_start` also should be defined (see [tests/fibbonacci.c](./fibbonacci.c)).

```asm

./elfs/fibbonacci_riscv:     file format elf32-littleriscv


Disassembly of section .text:

00010094 <_start>:
   10094:	fe010113          	addi	sp,sp,-32
   10098:	00812e23          	sw	s0,28(sp)
   1009c:	02010413          	addi	s0,sp,32
   100a0:	000107b7          	lui	a5,0x10
   100a4:	15078793          	addi	a5,a5,336 # 10150 <main>
   100a8:	fef42623          	sw	a5,-20(s0)
   100ac:	fec42783          	lw	a5,-20(s0)
   100b0:	000780e7          	jalr	a5
   100b4:	00100073          	ebreak
   100b8:	00000013          	nop
   100bc:	00078513          	mv	a0,a5
   100c0:	01c12403          	lw	s0,28(sp)
   100c4:	02010113          	addi	sp,sp,32
   100c8:	00008067          	ret

000100cc <fibbonaci>:
   100cc:	fe010113          	addi	sp,sp,-32
   100d0:	00112e23          	sw	ra,28(sp)
   100d4:	00812c23          	sw	s0,24(sp)
   100d8:	00912a23          	sw	s1,20(sp)
   100dc:	02010413          	addi	s0,sp,32
   100e0:	fea42623          	sw	a0,-20(s0)
   100e4:	fec42703          	lw	a4,-20(s0)
   100e8:	00100793          	li	a5,1
   100ec:	00f71663          	bne	a4,a5,100f8 <fibbonaci+0x2c>
   100f0:	00100793          	li	a5,1
   100f4:	0440006f          	j	10138 <fibbonaci+0x6c>
   100f8:	fec42703          	lw	a4,-20(s0)
   100fc:	00200793          	li	a5,2
   10100:	00f71663          	bne	a4,a5,1010c <fibbonaci+0x40>
   10104:	00100793          	li	a5,1
   10108:	0300006f          	j	10138 <fibbonaci+0x6c>
   1010c:	fec42783          	lw	a5,-20(s0)
   10110:	fff78793          	addi	a5,a5,-1
   10114:	00078513          	mv	a0,a5
   10118:	fb5ff0ef          	jal	ra,100cc <fibbonaci>
   1011c:	00050493          	mv	s1,a0
   10120:	fec42783          	lw	a5,-20(s0)
   10124:	ffe78793          	addi	a5,a5,-2
   10128:	00078513          	mv	a0,a5
   1012c:	fa1ff0ef          	jal	ra,100cc <fibbonaci>
   10130:	00050793          	mv	a5,a0
   10134:	00f487b3          	add	a5,s1,a5
   10138:	00078513          	mv	a0,a5
   1013c:	01c12083          	lw	ra,28(sp)
   10140:	01812403          	lw	s0,24(sp)
   10144:	01412483          	lw	s1,20(sp)
   10148:	02010113          	addi	sp,sp,32
   1014c:	00008067          	ret

00010150 <main>:
   10150:	fe010113          	addi	sp,sp,-32
   10154:	00112e23          	sw	ra,28(sp)
   10158:	00812c23          	sw	s0,24(sp)
   1015c:	02010413          	addi	s0,sp,32
   10160:	fe042623          	sw	zero,-20(s0)
   10164:	00b00513          	li	a0,11
   10168:	f65ff0ef          	jal	ra,100cc <fibbonaci>
   1016c:	fea42623          	sw	a0,-20(s0)
   10170:	00000793          	li	a5,0
   10174:	00078513          	mv	a0,a5
   10178:	01c12083          	lw	ra,28(sp)
   1017c:	01812403          	lw	s0,24(sp)
   10180:	02010113          	addi	sp,sp,32
   10184:	00008067          	ret

```