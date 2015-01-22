#include "bitset.h"

//BITSET* alloc_bitset(uint32_t bit_count)
//{
//	BITSET* bitset = (BITSET*)malloc(sizeof(bitset));
//	bitset->bitset = malloc(bit_count / 8 + 1);
//	bitset->bit_count = bit_count;
//
//	return bitset;
//}
//
//void free_bitset(BITSET* bitset)
//{
//	free(bitset->bitset);
//	free(bitset);
//}
//
//bool get_bitset_entry(uint32_t bit, BITSET* bitset)
//{
//	return *()bitset->bitset[bit / 8] << bit % 8;
//}
//
//int set_bitset_entry(uint32_t bit, bool bit_value, BITSET* bitset)
//{
//	uint8_t value = *(uint8_t*)((void*)((uint32_t)bitset->bitset + (bit / 8)));
//
//	return 0;
//}
//
//void clear_bitset(BITSET* bitset)
//{
//	memset(bitset->bitset, 0, bitset->bit_count / 8 + 1);
//}