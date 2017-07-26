#include <bigint.h>

#include <stdio.h>
#include <time.h>	

int main(int argc, char *argv[]) {
	BigInt *val, *val2;
	clock_t start, end;	
	int count;

	val = getBigInt();
	val2 = getBigInt();
	setBit(val2, 1);
	setBit(val, 80);

	printf("Testing 1B additions... now\n");

	count = 0;
	start = clock(); 
	while (count<1000000000) {
		add(val, val2);
		++count;
	} 
	
	end = clock(); 
	printf("Took %fs to complete.\n", (double) (end-start)/CLOCKS_PER_SEC);

	freeBigInt(val);
	freeBigInt(val2);

	return 0;
}