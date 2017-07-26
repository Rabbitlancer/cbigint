#include <bigint.h>

#include <stdio.h>

#define PRINT(x) 	outputBinary(x, 75, output);\
					printf("%s\n", output); \
					printf("%d\n", x->usedDigits);

int main(int argc, char *argv[]) {
	BigInt *val;

	val = getBigInt();
	setBit(val, 70);
	setBit(val, 22);

	char output[100];

	PRINT(val)

	setBit(val, 1);

	shiftLeft(val, 2);
	PRINT(val)

	BigInt *val2 = getBigInt();
	copy(val2, val);
	setBit(val2, 64);

	shiftRight(val, 3);
	PRINT(val)

	add(val, val2);
	PRINT(val)

	add(val, val2);
	PRINT(val)

	shiftRight(val, 1);
	PRINT(val)

	unsetBit(val, 68);
	unsetBit(val, 3);
	PRINT(val)

	printf("%d\n", comp(val, val2));
	printf("%d\n", comp(val2, val));
	printf("%d\n", comp(val, val));

	PRINT(val)

	unsetBit(val, val->usedDigits+1);
	PRINT(val)

	printf("%d\n", testBit(val,72));

	BigInt *oftest = getBigInt();
	setBit(oftest, 64);
	add(oftest, oftest);
	PRINT(oftest)
	freeBigInt(oftest);

	freeBigInt(val);
	freeBigInt(val2);

	return 0;
}