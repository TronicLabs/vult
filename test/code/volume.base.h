#ifndef ._H
#define ._H
#include <stdint.h>
#include <math.h>

typedef struct _ctx_type_0 {
   float x;
} _ctx_type_0;

typedef _ctx_type_0 smooth_type;

_ctx_type_0 _ctx_type_0_init();

_ctx_type_0 smooth_init();

float smooth(_ctx_type_0 &_ctx, float input);

typedef struct _ctx_type_1 {
   float volume;
   _ctx_type_0 _inst0;
} _ctx_type_1;

typedef _ctx_type_1 process_type;

_ctx_type_1 _ctx_type_1_init();

_ctx_type_1 process_init();

float process(_ctx_type_1 &_ctx, float input);

typedef _ctx_type_1 noteOn_type;

_ctx_type_1 noteOn_init();

void noteOn(_ctx_type_1 &_ctx, int note, int velocity);

typedef _ctx_type_1 noteOff_type;

_ctx_type_1 noteOff_init();

void noteOff(_ctx_type_1 &_ctx, int note);

typedef _ctx_type_1 controlChange_type;

_ctx_type_1 controlChange_init();

void controlChange(_ctx_type_1 &_ctx, int control, int value);

typedef _ctx_type_1 default_type;

_ctx_type_1 default_init();

void default_(_ctx_type_1 &_ctx);



#endif // ._H
