#include "bigint.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 64
#define OVERFLOWMARK 0x7FFFFFFFFFFFFFFFL
#define ALLBITS 0xFFFFFFFFFFFFFFFFL

static const uint64_t bits[64] = 
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

static const uint64_t inverseBits[64] =
{
	0xFFFFFFFFFFFFFFFEL, 0xFFFFFFFFFFFFFFFDL,
	0xFFFFFFFFFFFFFFFBL, 0xFFFFFFFFFFFFFFF8L,
	0xFFFFFFFFFFFFFFEFL, 0xFFFFFFFFFFFFFFDFL,
	0xFFFFFFFFFFFFFFBFL, 0xFFFFFFFFFFFFFF8FL,
	0xFFFFFFFFFFFFFEFFL, 0xFFFFFFFFFFFFFDFFL,
	0xFFFFFFFFFFFFFBFFL, 0xFFFFFFFFFFFFF8FFL,
	0xFFFFFFFFFFFFEFFFL, 0xFFFFFFFFFFFFDFFFL,
	0xFFFFFFFFFFFFBFFFL, 0xFFFFFFFFFFFF8FFFL,
	0xFFFFFFFFFFFEFFFFL, 0xFFFFFFFFFFFDFFFFL,
	0xFFFFFFFFFFFBFFFFL, 0xFFFFFFFFFFF8FFFFL,
	0xFFFFFFFFFFEFFFFFL, 0xFFFFFFFFFFDFFFFFL,
	0xFFFFFFFFFFBFFFFFL, 0xFFFFFFFFFF8FFFFFL,
	0xFFFFFFFFFEFFFFFFL, 0xFFFFFFFFFDFFFFFFL,
	0xFFFFFFFFFBFFFFFFL, 0xFFFFFFFFF8FFFFFFL,
	0xFFFFFFFFEFFFFFFFL, 0xFFFFFFFFDFFFFFFFL,
	0xFFFFFFFFBFFFFFFFL, 0xFFFFFFFF8FFFFFFFL,
	0xFFFFFFFEFFFFFFFFL, 0xFFFFFFFDFFFFFFFFL,
	0xFFFFFFFBFFFFFFFFL, 0xFFFFFFF8FFFFFFFFL,
	0xFFFFFFEFFFFFFFFFL, 0xFFFFFFDFFFFFFFFFL,
	0xFFFFFFBFFFFFFFFFL, 0xFFFFFF8FFFFFFFFFL,
	0xFFFFFEFFFFFFFFFFL, 0xFFFFFDFFFFFFFFFFL,
	0xFFFFFBFFFFFFFFFFL, 0xFFFFF8FFFFFFFFFFL,
	0xFFFFEFFFFFFFFFFFL, 0xFFFFDFFFFFFFFFFFL,
	0xFFFFBFFFFFFFFFFFL, 0xFFFF8FFFFFFFFFFFL,
	0xFFFEFFFFFFFFFFFFL, 0xFFFDFFFFFFFFFFFFL,
	0xFFFBFFFFFFFFFFFFL, 0xFFF8FFFFFFFFFFFFL,
	0xFFEFFFFFFFFFFFFFL, 0xFFDFFFFFFFFFFFFFL,
	0xFFBFFFFFFFFFFFFFL, 0xFF8FFFFFFFFFFFFFL,
	0xFEFFFFFFFFFFFFFFL, 0xFDFFFFFFFFFFFFFFL,
	0xFBFFFFFFFFFFFFFFL, 0xF8FFFFFFFFFFFFFFL,
	0xEFFFFFFFFFFFFFFFL, 0xDFFFFFFFFFFFFFFFL,
	0xBFFFFFFFFFFFFFFFL, 0x8FFFFFFFFFFFFFFFL
};

static const uint64_t bitMasks[64] =
{
	0xFFFFFFFFFFFFFFFEL, 0xFFFFFFFFFFFFFFFCL,
	0xFFFFFFFFFFFFFFF8L, 0xFFFFFFFFFFFFFFF0L,
	0xFFFFFFFFFFFFFFE0L, 0xFFFFFFFFFFFFFFC0L,
	0xFFFFFFFFFFFFFF80L, 0xFFFFFFFFFFFFFF00L,
	0xFFFFFFFFFFFFFE00L, 0xFFFFFFFFFFFFFC00L,
	0xFFFFFFFFFFFFF800L, 0xFFFFFFFFFFFFF000L,
	0xFFFFFFFFFFFFE000L, 0xFFFFFFFFFFFFC000L,
	0xFFFFFFFFFFFF8000L, 0xFFFFFFFFFFFF0000L,
	0xFFFFFFFFFFFE0000L, 0xFFFFFFFFFFFC0000L,
	0xFFFFFFFFFFF80000L, 0xFFFFFFFFFFF00000L,
	0xFFFFFFFFFFE00000L, 0xFFFFFFFFFFC00000L,
	0xFFFFFFFFFF800000L, 0xFFFFFFFFFF000000L,
	0xFFFFFFFFFE000000L, 0xFFFFFFFFFC000000L,
	0xFFFFFFFFF8000000L, 0xFFFFFFFFF0000000L,
	0xFFFFFFFFE0000000L, 0xFFFFFFFFC0000000L,
	0xFFFFFFFF80000000L, 0xFFFFFFFF00000000L,
	0xFFFFFFFE00000000L, 0xFFFFFFFC00000000L,
	0xFFFFFFF800000000L, 0xFFFFFFF000000000L,
	0xFFFFFFE000000000L, 0xFFFFFFC000000000L,
	0xFFFFFF8000000000L, 0xFFFFFF0000000000L,
	0xFFFFFE0000000000L, 0xFFFFFC0000000000L,
	0xFFFFF80000000000L, 0xFFFFF00000000000L,
	0xFFFFE00000000000L, 0xFFFFC00000000000L,
	0xFFFF800000000000L, 0xFFFF000000000000L,
	0xFFFE000000000000L, 0xFFFC000000000000L,
	0xFFF8000000000000L, 0xFFF0000000000000L,
	0xFFE0000000000000L, 0xFFC0000000000000L,
	0xFF80000000000000L, 0xFF00000000000000L,
	0xFE00000000000000L, 0xFC00000000000000L,
	0xF800000000000000L, 0xF000000000000000L,
	0xE000000000000000L, 0xC000000000000000L,
	0x8000000000000000L, 0x0000000000000000L
};

void init(BigInt * const arg) {
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

void freeBigInt(BigInt * const target) {
	free(target);
}

void copy(BigInt * const dest, const BigInt *src) {
	memcpy(dest, src, sizeof(BigInt));
}

void add(BigInt * const target, const BigInt *arg) {
	uint32_t blocks;

	uint16_t mostDigits;
	if (target->usedDigits > arg->usedDigits) {
		mostDigits = target->usedDigits;
	} else {
		mostDigits = arg->usedDigits;
	}

	blocks = mostDigits / BLOCKSIZE + 1;

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

	if (carry) {
		target->value[blocks] += carry;
	} else if (target->value[blocks-1] & bitMasks[mostDigits % BLOCKSIZE]) {
		carry = 1L;
	}

	target->usedDigits = mostDigits + (uint16_t) carry;
}

int comp(const BigInt *target, const BigInt *arg) {
	if (target->usedDigits > arg->usedDigits) {
		return 1;
	}

	if (target->usedDigits < arg->usedDigits) {
		return -1;
	}

	for (int32_t block = target->usedDigits / BLOCKSIZE;
				 block >= 0;
			   --block) {
		if (target->value[block] > arg->value[block]) return 1;
		if (target->value[block] < arg->value[block]) return -1;		
	}

	return 0;
}

void shiftLeft(BigInt * const target, const uint64_t arg) {
	uint64_t antiarg = BLOCKSIZE - arg;

	uint64_t carry = 0L;
	uint64_t temp = 0L;

	int32_t blocks;
	blocks = target->usedDigits / BLOCKSIZE + 2;

	for (int32_t i = 0; i<blocks; ++i) {
		temp = target->value[i];
		target->value[i] <<= arg;
		target->value[i] += carry;
		carry = temp >> antiarg;
	}

	target->usedDigits += arg;
}

void shiftRight(BigInt * const target, const uint64_t arg) {
	uint64_t antiarg = BLOCKSIZE - arg;

	uint64_t carry = 0L;
	uint64_t temp = 0L;

	int32_t blocks;
	blocks = target->usedDigits / BLOCKSIZE + 2;

	for (int32_t i = blocks; i>=0; --i) {
		temp = target->value[i];
		target->value[i] >>= arg;
		target->value[i] += carry;
		carry = temp << antiarg;
	}

	target->usedDigits -= arg;
}

void setBit(BigInt * const target, uint64_t index) {
	--index;

	if (index > target->usedDigits) {
		target->usedDigits = index;
	}
	
	uint64_t block = index / BLOCKSIZE;
	index %= BLOCKSIZE;

	target->value[block] |= bits[index];
}

void unsetBit(BigInt * const target, uint64_t index) {
	--index;

	uint8_t reuseFlag = 0;
	if (index == target->usedDigits) {
		reuseFlag = 1;
	}

	uint64_t block = index / BLOCKSIZE;
	index %= BLOCKSIZE;

	target->value[block] &= inverseBits[index];

	if (reuseFlag) {
		uint64_t cur = target->value[block];

		while (cur - bits[index] > OVERFLOWMARK) {
			if (index) {
				--index;
			} else if (block) {
				index = 63;
				--block;
				cur = target->value[block];
			} else {
				break;
			}
		}

		target->usedDigits = BLOCKSIZE*block + index;
	}
}

extern int testBit(BigInt * const target, uint64_t index) {
	--index;
	
	uint64_t block = index / BLOCKSIZE;
	index %= BLOCKSIZE;

	int res = target->value[block] & bits[index];
	res >>= index;

	return res;
}

void outputBinary(const BigInt *target, const uint64_t digits, char * const buf) {
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