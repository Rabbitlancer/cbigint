#include <bigint.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
	BigInt *val;

	val = getBigInt();
	setBit(val, 70);
	setBit(val, 22);

	char output[100];

	outputBinary(val, 90, output);
	printf("%s\n", output);

	setBit(val, 1);

	shiftLeft(val, 2);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	BigInt *val2 = getBigInt();
	copy(val2, val);
	setBit(val2, 64);

	shiftRight(val, 3);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	add(val, val2);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	add(val, val2);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	shiftRight(val, 1);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	unsetBit(val, 68);
	unsetBit(val, 3);
	outputBinary(val, 90, output);
	printf("%s\n", output);

	printf("%d\n", comp(val, val2));
	printf("%d\n", comp(val2, val));
	printf("%d\n", comp(val, val));

	freeBigInt(val);
	freeBigInt(val2);

	return 0;
}