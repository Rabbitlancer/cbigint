#pragma once

/**
 *	@file bigint.h
 *	@author Rabbitlancer
 *	@date 23 July 2017
 *
 *	Unsigned big integer API.
 *	This includes the BigInt struct, as well as its
 *	handling functions. Note that every function uses
 *	BigInt pointers, and most operations modify its
 *	first argument.
 */

#include <inttypes.h>

#define MAXBLOCKS 64

/**
 *	@struct BigInt
 *	The unsigned big integer type.
 *	BigInt uses at most 64 "blocks" of data, which are
 *	represented by 64-bit integers. 
 *	@see bigint.h
 */
typedef struct {
	uint64_t value[MAXBLOCKS]; /**< Actual data held by BigInt. */
	uint16_t usedDigits; /**< Amount of binary digits used by BigInt. Note that this value may exceed the real amount of digits used, but may never be less. In other words, no data loss is guaranteed. */
} BigInt;

/**
 *	@fn init(BigInt *arg)
 *	Initializes BigInt arg with zeroes.
 *	It is called automatically within getBigInt(), but
 *	you may also call it to clear the value of your
 *	BigInt.
 *	@see getBigInt()
 */
extern void			init(BigInt *arg);

/**
 *	@fn getBigInt()
 *	Returns a pointer to a single BigInt.
 *	BigInt returned is initialized with zeroes.
 *	Basically, getBigInt() is a malloc wrapper followed
 *	by an init() call, so you can easily write your
 *	own BigInt resource allocator via other memory
 *	allocation functions.
 *	@see init()
 *	@see freeBigInt()
 *	@return A pointer to zero-initialized memory block for use as a BigInt.
 */
extern BigInt	   *getBigInt();

/**
 *	@fn freeBigInt(BigInt *target)
 *	Frees memory taken by BigInt target.
 *	freeBigInt() is nothing much but a free() wrapper,
 *	really. Nothing to see here.
 *	@see getBigInt()
 */
extern void			freeBigInt(BigInt *target);

/**
 *	@fn copy(BigInt *dest, BigInt *src)
 *	Copies BigInt src into dest.
 *	copy() is pretty much a memcpy() wrapper.
 */
extern void			copy(BigInt *dest, BigInt *src);

/**
 *	@fn add(BigInt *target, BigInt *arg)
 *	Adds arg to target.
 *	Remember that the function modifies the actual
 *	target! All BigInts are passed by reference.
 */
extern void			add(BigInt *target, BigInt *arg);

/**
 *	@fn shiftLeft(BigInt *target, uint64_t arg)
 *	Performs an arithmetical left shift by arg positions.
 *	The shift only works within 64 bits; it is not
 *	designed to handle larger shifts. 
 *	@see shiftRight()
 */
extern void			shiftLeft(BigInt *target, uint64_t arg);

/**
 *	@fn shiftRight(BigInt *target, uint64_t arg)
 *	Performs an arithmetical right shift by arg positions.
 *	The shift only works within 64 bits; it is not
 *	designed to handle larger shifts.
 *	@see shiftLeft()
 */
extern void			shiftRight(BigInt *target, uint64_t arg);

/**
 *	@fn setBit(BigInt *target, uint64_t index)
 *	Sets a bit number index to 1.
 *	index is a human-readable position, starting with 1.
 *	@see unsetBit()
 */
extern void			setBit(BigInt *target, uint64_t index);

/**
 *	@fn unsetBit(BigInt *target, uint64_t index)
 *	Sets a bit number index to 0.
 *	index is a human-readable position, starting with 1.
 *	unsetBit() also ignores usedDigits variable, so use
 *	with care.
 *	@see setBit()
 */
extern void			unsetBit(BigInt *target, uint64_t index);

/**
 *	@fn outputBinary(BigInt *target, uint64_t digits, char *buf)
 *	Writes digits least significant bits from target into C string buf.	
 *	This function does not perform any memory checks.
 *	Make sure that buf is at least digits+1 bytes long,
 *	or the behavior of this function may be hazardous.
 *	However, it will never output more than usedDigits.
 */
extern void			outputBinary(BigInt *target, uint64_t digits, char *buf);