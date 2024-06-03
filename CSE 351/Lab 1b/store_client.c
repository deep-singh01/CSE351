/*
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s): Deepesh Singh  
 * NetID(s): dsingh01
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  // TODO: implement this function
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      // refill sections in aisles
      int id = get_id(&aisles[i], j);
      unsigned short curr_items = num_items(&aisles[i], j);
      add_items(&aisles[i], j, stockroom[id]);
      // remove items added to sections in aisles
      stockroom[id] -= (num_items(&aisles[i], j) - curr_items); 
    }
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  // TODO: implement this function
  int items_removed = 0;

  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      // if section id matches id given
      if (id == get_id(&aisles[i], j)) {
	// remove items from sections to fulfill order
        unsigned short curr_items = num_items(&aisles[i], j);
        remove_items(&aisles[i], j, num);
	num -= (curr_items - num_items(&aisles[i], j));
	// add num of items removed to counter
        items_removed += (curr_items - num_items(&aisles[i], j));
      }
    }
  }

  // if order not completed
  if (num > 0) {
    // if items in stockroom > num of items remaining in order
    if (stockroom[id] > num) {
      items_removed += num;
      stockroom[id] -= num;
    } else { // items in stockroom <= num of items remaining in order 
      items_removed += stockroom[id];
      stockroom[id] = 0;
    }
  }
  return items_removed;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  // TODO: implement this function
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      // checks if section has no items and if section id matches id given
      if ((num_items(&aisles[i], j) == 0) && (id == get_id(&aisles[i], j))) {
       return (unsigned short*) &aisles[i] + j;
      }
    }
  }
  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  // TODO: implement this function
  int most_items = -1;
  unsigned short* sec;
  for (int i = 0; i < NUM_AISLES; i++) {
    for (int j = 0; j < SECTIONS_PER_AISLE; j++) {
      // check if num of items in section > previously set section with most items
      if (num_items(&aisles[i], j) > most_items) {
        most_items = num_items(&aisles[i], j);
	sec = (unsigned short*) &aisles[i] + j;
      }
    }
  }
  return sec;
}
