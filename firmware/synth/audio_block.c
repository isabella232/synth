#include "audio_block.h"

#include <stdlib.h>

void AudioBlock_Init(AudioBlock* block) {
  block->ref = 1;
}

void AudioBlock_Inc(AudioBlock* block) {
  block->ref++;
}

void AudioBlock_Dec(AudioBlock* block) {
  block->ref--;

  if (block->ref == 0) {
    free(block);
  }
}

AudioBlock* AudioBlockPool_Allocate() {
  AudioBlock* block = malloc(sizeof(AudioBlock));
  AudioBlock_Init(block);
  return block;
}
