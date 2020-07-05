#ifndef __AUDIO_PROCESSOR_H__
#define __AUDIO_PROCESSOR_H__

#include <stdint.h>

#include "audio_block.h"

#define MAX_AUDIO_PROCESSOR_INOUTS 8

struct AudioProcessor;

typedef struct AudioConnection {
  // The processors that are connected.
  struct AudioProcessor* from;
  struct AudioProcessor* to;

  // For the to processor, which input slot should we write to.
  uint8_t to_slot;
} AudioConnection;

void AudioConnection_Init(AudioConnection* conn,
                          struct AudioProcessor* from, struct AudioProcessor* to,
                          uint8_t to_slot);


typedef AudioBlock* (*AudioProcessorFunction)();

typedef struct AudioProcessor {
  AudioProcessorFunction process_func;

  // How many ins and outs this processor currently has.
  uint8_t num_ins, num_outs;

  // A list of input audio blocks.
  AudioBlock* ins[MAX_AUDIO_PROCESSOR_INOUTS];

  // A list of output audio connections.
  AudioConnection* outs[MAX_AUDIO_PROCESSOR_INOUTS];
} AudioProcessor;

void AudioProcessor_Init(AudioProcessor* processor,
                         AudioProcessorFunction process_func);
void AudioProcessor_Free(AudioProcessor* processor);

// Instructs the audio processor to generate a block's worth of audio and
// propagate that audio to all downstream processors.
void AudioProcessor_Step(AudioProcessor* processor);

// Connects the specified audio processors together.
void AudioProcessor_ConnectTo(AudioProcessor* us, AudioProcessor* them);

#endif
