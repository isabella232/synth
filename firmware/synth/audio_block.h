#ifndef __AUDIO_BLOCK_H__
#define __AUDIO_BLOCK_H__

#include <stdint.h>
#include "defines.h"

typedef struct AudioBlock {
  // Audio data with sequential (non-interleaved) channels.
  float data[AUDIO_BLOCK_CHANNELS][AUDIO_BLOCK_SAMPLES];

  // How many references there are to this block of data.
  uint8_t ref;
} AudioBlock;

void AudioBlock_Init(AudioBlock* block);

// Increment (and decrement) the reference count for the audio block.  If the
// reference count reaches zero the block is deallocated.
void AudioBlock_Inc(AudioBlock* block);
void AudioBlock_Dec(AudioBlock* block);


typedef struct AudioBlockPool {
  // TODO(gtaubman): Actually implement a block pool.  For now blocks are
  // malloc'd every time.
} AudioBlockPool;

// Returns a new audio block.  The data within the audio block is uninitialized.
AudioBlock* AudioBlockPool_Allocate();

#endif
