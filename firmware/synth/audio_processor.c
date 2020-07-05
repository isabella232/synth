#include "audio_processor.h"

#include <stdio.h>
#include <stdlib.h>

void AudioConnection_Init(AudioConnection* conn,
                          AudioProcessor* from, AudioProcessor* to,
                          uint8_t to_slot) {
  conn->from = from;
  conn->to = to;
  conn->to_slot = to_slot;
}

void AudioProcessor_Init(AudioProcessor* processor,
                         AudioProcessorFunction process_func) {
  processor->process_func = process_func;

  processor->num_ins = 0;
  processor->num_outs = 0;

  for (int i = 0; i < MAX_AUDIO_PROCESSOR_INOUTS; ++i) {
    processor->ins[i] = NULL;
    processor->outs[i] = NULL;
  }
}

void AudioProcessor_Free(AudioProcessor* processor) {
  for (int i = 0; i < MAX_AUDIO_PROCESSOR_INOUTS; ++i) {
    free(processor->outs[i]);
  }
}

void AudioProcessor_Step(AudioProcessor* processor) {
  // At this point all of our inputs have been populated.

  // Generate our new audio block.
  AudioBlock* output = processor->process_func();

  // Propagate the new block to all appropriate slots for all outputs.  For each
  // output we up the reference count on the block.
  for (int i = 0; i < processor->num_outs; ++i) {
    AudioBlock_Inc(output);
    AudioConnection* out_conn = processor->outs[i];
    out_conn->to->ins[out_conn->to_slot] = output;
  }

  // We're done using the new block ourselves.
  if (output != NULL) {
    AudioBlock_Dec(output);
  }

  // We're done with all of our inputs so we can decrement their reference
  // counters as well.
  for (int i = 0; i < processor->num_ins; ++i) {
    AudioBlock* in_block = processor->ins[i];
    if (in_block != output) {
      // It could be the case that a processor returns one of its input blocks
      // as its output.  In this case we've already decremented the reference
      // count for that block so we want to avoid doing so again.
      AudioBlock_Dec(in_block);
    }
  }
}

void AudioProcessor_ConnectTo(AudioProcessor* us, AudioProcessor* them) {
  if (us->num_outs + 1 == MAX_AUDIO_PROCESSOR_INOUTS) {
    fprintf(stderr, "No room for more outputs.\n");
    return;
  }

  if (us->num_ins + 1 == MAX_AUDIO_PROCESSOR_INOUTS) {
    fprintf(stderr, "No room for more inputs.\n");
    return;
  }

  AudioConnection* conn = malloc(sizeof(AudioConnection));
  AudioConnection_Init(conn, us, them, them->num_ins++);
  us->outs[us->num_outs++] = conn;
}
