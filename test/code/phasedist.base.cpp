#include "..h"

_ctx_type_0 _ctx_type_0_init(){
   _ctx_type_0 _ctx;
   _ctx.x = 0.f;
   return _ctx;
}

_ctx_type_0 smooth_init(){ return _ctx_type_0_init();}

float smooth(_ctx_type_0 &_ctx, float input){
   _ctx.x = (_ctx.x + ((input - _ctx.x) * 0.005f));
   return _ctx.x;
}

_ctx_type_1 _ctx_type_1_init(){
   _ctx_type_1 _ctx;
   _ctx.pre_x = 0.f;
   return _ctx;
}

_ctx_type_1 change_init(){ return _ctx_type_1_init();}

uint8_t change(_ctx_type_1 &_ctx, float x){
   uint8_t v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

float pitchToRate(float d){
   return ((8.1758f * expf((0.0577623f * d))) / 44100.f);
}

_ctx_type_3 _ctx_type_3_init(){
   _ctx_type_3 _ctx;
   _ctx.rate = 0.f;
   _ctx.phase = 0.f;
   _ctx._inst0 = _ctx_type_1_init();
   return _ctx;
}

_ctx_type_3 phasor_init(){ return _ctx_type_3_init();}

float phasor(_ctx_type_3 &_ctx, float pitch, uint8_t reset){
   if(change(_ctx._inst0,pitch)){
      _ctx.rate = pitchToRate(pitch);
   }
   _ctx.phase = (reset?0.f:fmodf((_ctx.phase + _ctx.rate),1.f));
   return _ctx.phase;
}

_ctx_type_4 _ctx_type_4_init(){
   _ctx_type_4 _ctx;
   _ctx.volume = 0.f;
   _ctx.pre_phase1 = 0.f;
   _ctx.pitch = 0.f;
   _ctx.detune = 0.f;
   _ctx._inst3 = _ctx_type_0_init();
   _ctx._inst2 = _ctx_type_3_init();
   _ctx._inst1 = _ctx_type_0_init();
   _ctx._inst0 = _ctx_type_3_init();
   return _ctx;
}

_ctx_type_4 process_init(){ return _ctx_type_4_init();}

float process(_ctx_type_4 &_ctx, float input){
   float phase1 = phasor(_ctx._inst0,_ctx.pitch,0);
   float comp = (1.f - phase1);
   uint8_t reset = ((_ctx.pre_phase1 - phase1) > 0.5f);
   _ctx.pre_phase1 = phase1;
   float phase2 = phasor(_ctx._inst2,(_ctx.pitch + (smooth(_ctx._inst1,_ctx.detune) * 32.f)),reset);
   float sine = sinf(((2.f * 3.14159265359f) * phase2));
   return (smooth(_ctx._inst3,_ctx.volume) * (sine * comp));
}

_ctx_type_4 noteOn_init(){ return _ctx_type_4_init();}

void noteOn(_ctx_type_4 &_ctx, int note, int velocity){
   _ctx.pitch = ((float)note);
}

_ctx_type_4 noteOff_init(){ return _ctx_type_4_init();}

void noteOff(_ctx_type_4 &_ctx, int note){
}

_ctx_type_4 controlChange_init(){ return _ctx_type_4_init();}

void controlChange(_ctx_type_4 &_ctx, int control, int value){
   if(control == 30){
      _ctx.volume = (((float)value) / 127.f);
   }
   if(control == 31){
      _ctx.detune = (((float)value) / 127.f);
   }
}

_ctx_type_4 default_init(){ return _ctx_type_4_init();}

void default_(_ctx_type_4 &_ctx){
   _ctx.volume = 0.f;
   _ctx.pitch = 45.f;
   _ctx.detune = 0.f;
}


