#include "bigint.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 64
#define OVERFLOWMARK 0x7FFFFFFFFFFFFFFFL

static uint64_t bits[64] = 
{
	0x0000000000000001L, 0x0000000000000002L,
	0x0000000000000004L, 0x0000000000000008L,
	0x0000000000000010L, 0x0000000000000020L,
	0x0000000000000040L, 0x0000000000000080L,
	0x0000000000000100L, 0x0000000000000200L,
	0x0000000000000400L, 0x0000000000000800L,
	0x0000000000001000L, 0x0000000000002000L,
	0x0000000000004000L, 0x0000000000008000L,
	0x0000000000010000L, 0x0000000000020000L,
	0x0000000000040000L, 0x0000000000080000L,
	0x0000000000100000L, 0x0000000000200000L,
	0x0000000000400000L, 0x0000000000800000L,
	0x0000000001000000L, 0x0000000002000000L,
	0x0000000004000000L, 0x0000000008000000L,
	0x0000000010000000L, 0x0000000020000000L,
	0x0000000040000000L, 0x0000000080000000L,
	0x0000000100000000L, 0x0000000200000000L,
	0x0000000400000000L, 0x0000000800000000L,
	0x0000001000000000L, 0x0000002000000000L,
	0x0000004000000000L, 0x0000008000000000L,
	0x0000010000000000L, 0x0000020000000000L,
	0x0000040000000000L, 0x0000080000000000L,
	0x0000100000000000L, 0x0000200000000000L,
	0x0000400000000000L, 0x0000800000000000L,
	0x0001000000000000L, 0x0002000000000000L,
	0x0004000000000000L, 0x0008000000000000L,
	0x0010000000000000L, 0x0020000000000000L,
	0x0040000000000000L, 0x0080000000000000L,
	0x0100000000000000L, 0x0200000000000000L,
	0x0400000000000000L, 0x0800000000000000L,
	0x1000000000000000L, 0x2000000000000000L,
	0x4000000000000000L, 0x8000000000000000L
};


void init(BigInt *arg) {
	arg->usedDigits = 0;

	for (int i = 0; i<MAXBLOCKS; ++i) {
		arg->value[i] = 0L;
	}
}

BigInt *getBigInt() {
	BigInt *res = (BigInt*) malloc(sizeof(BigInt));
	
	init(res);

	return res;
}

void freeBigInt(BigInt *target) {
	free(target);
}

void copy(BigInt *dest, BigInt *src) {
	memcpy(dest, src, sizeof(BigInt));
}

void add(BigInt *target, BigInt *arg) {
	uint32_t blocks;

	if (target->usedDigits > arg->usedDigits) {
		blocks = target->usedDigits / 64 + 1;
	} else {
		blocks = arg->usedDigits / 64 + 1;
	}

	uint64_t carry = 0L;
	for (int i = 0; i<blocks; ++i) {
		target->value[i] += carry;

		if (target->value[i] == 0L && carry == 1L) {
			carry = 1L;
		} else {
			carry = 0L;
		}

		if (arg->value[i] > OVERFLOWMARK && target->value[i] > OVERFLOWMARK) {
			carry = 1L;
		}

		target->value[i] += arg->value[i];
	}

	if (carry == 1L) {
		target->usedDigits += 1;
	}
}

void shiftLeft(BigInt *target, uint64_t arg) {
	uint64_t antiarg = BLOCKSIZE - arg;

	uint64_t carry = 0L;
	uint64_t temp = 0L;

	int32_t blocks;
	blocks = target->usedDigits / 64 + 2;

	for (int32_t i = 0; i<blocks; ++i) {
		temp = target->value[i];
		target->value[i] <<= arg;
		target->value[i] += carry;
		carry = temp >> antiarg;
	}

	target->usedDigits += arg;
}

void shiftRight(BigInt *target, uint64_t arg) {
	uint64_t antiarg = BLOCKSIZE - arg;

	uint64_t carry = 0L;
	uint64_t temp = 0L;

	int32_t blocks;
	blocks = target->usedDigits / 64 + 2;

	for (int32_t i = blocks; i>=0; --i) {
		temp = target->value[i];
		target->value[i] >>= arg;
		target->value[i] += carry;
		carry = temp << antiarg;
	}

	target->usedDigits -= arg;
}

void setBit(BigInt *target, uint64_t index) {
	--index;

	if (index > target->usedDigits) {
		target->usedDigits = index;
	}
	
	uint64_t block = index / BLOCKSIZE;
	index %= BLOCKSIZE;

	target->value[block] |= bits[index];
}

void unsetBit(BigInt *target, uint64_t index) {
	--index;

	uint64_t block = index / BLOCKSIZE;
	index %= BLOCKSIZE;

	target->value[block] &= !bits[index];
}

void outputBinary(BigInt *target, uint64_t digits, char *buf) {
	uint64_t blocks = digits / BLOCKSIZE + 1;

	uint64_t cur;
	uint64_t index = 0;
	uint64_t block = 0;

	while (block < blocks) {
		cur = target->value[block];

		for (int i = 0; i<BLOCKSIZE; ++i) {
			if (cur & 1) {
				buf[digits-index-1] = '1';
			} else {
				buf[digits-index-1] = '0';
			}

			++index;
			cur >>= 1;

			if (index == digits) {
				break;
			}
		}

		++block;
	}

	buf[index] = '\0';
}