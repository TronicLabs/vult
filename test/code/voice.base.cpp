#include "..h"

float minFixed(){
   return 1.52588e-05f;
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

_ctx_type_2 _ctx_type_2_init(){
   _ctx_type_2 _ctx;
   _ctx.pre_x = 0;
   return _ctx;
}

_ctx_type_2 bchange_init(){ return _ctx_type_2_init();}

uint8_t bchange(_ctx_type_2 &_ctx, uint8_t x){
   uint8_t v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

_ctx_type_3 _ctx_type_3_init(){
   _ctx_type_3 _ctx;
   _ctx.pre_x = 0;
   return _ctx;
}

_ctx_type_3 edge_init(){ return _ctx_type_3_init();}

uint8_t edge(_ctx_type_3 &_ctx, uint8_t x){
   uint8_t v = ((_ctx.pre_x != x) && (_ctx.pre_x == 1));
   _ctx.pre_x = x;
   return v;
}

_ctx_type_4 _ctx_type_4_init(){
   _ctx_type_4 _ctx;
   _ctx.count = 0;
   return _ctx;
}

_ctx_type_4 each_init(){ return _ctx_type_4_init();}

uint8_t each(_ctx_type_4 &_ctx, int n){
   uint8_t ret = (_ctx.count == 0);
   _ctx.count = ((_ctx.count + 1) % n);
   return ret;
}

uint8_t near_zero(float x){
   return (fabsf(x) < 0.02f);
}

_ctx_type_6 _ctx_type_6_init(){
   _ctx_type_6 _ctx;
   _ctx.y1 = 0.f;
   _ctx.x1 = 0.f;
   return _ctx;
}

_ctx_type_6 dcblock_init(){ return _ctx_type_6_init();}

float dcblock(_ctx_type_6 &_ctx, float x0){
   float y0 = ((x0 - _ctx.x1) + (_ctx.y1 * 0.995f));
   _ctx.x1 = x0;
   _ctx.y1 = y0;
   return y0;
}

_ctx_type_7 _ctx_type_7_init(){
   _ctx_type_7 _ctx;
   _ctx.pre_x = 0.f;
   return _ctx;
}

_ctx_type_7 lpfilter_init(){ return _ctx_type_7_init();}

float lpfilter(_ctx_type_7 &_ctx, float x){
   float ret = ((x + _ctx.pre_x) / 2.f);
   _ctx.pre_x = x;
   return ret;
}

float pitchToRate(float d){
   return ((8.1758f * expf((0.0577623f * d))) / 44100.f);
}

float pulse_train(float m, float phase){
   float pi_phase = (phase * 3.14159265359f);
   float denominator1 = sinf(pi_phase);
   float tmp1 = 0.f;
   if(near_zero(denominator1)){
      tmp1 = 1.f;
   }
   else
   {
      tmp1 = sinf((m * pi_phase));
      tmp1 = (tmp1 / (m * denominator1));
   }
   return tmp1;
}

_ctx_type_10 _ctx_type_10_init(){
   _ctx_type_10 _ctx;
   _ctx.state_triang = 0.f;
   _ctx.state_saw = 0.f;
   _ctx.state_pulse = 0.f;
   _ctx.rate = 0.f;
   _ctx.phase = 0.f;
   _ctx.output = 0.f;
   _ctx.m = 0.f;
   _ctx._inst1 = _ctx_type_6_init();
   _ctx._inst0 = _ctx_type_1_init();
   return _ctx;
}

_ctx_type_10 osc_init(){ return _ctx_type_10_init();}

float osc(_ctx_type_10 &_ctx, float pitch, float pw, float wave){
   float fixed_pitch = 0.f;
   if(wave < (2.f / 3.f)){
      fixed_pitch = pitch;
   }
   else
   {
      fixed_pitch = (pitch + 12.f);
   }
   if(change(_ctx._inst0,fixed_pitch)){
      _ctx.rate = pitchToRate(fixed_pitch);
      float p = (1.f / _ctx.rate);
      float maxHarmonics = floorf((p / 2.f));
      _ctx.m = ((2.f * maxHarmonics) + 1.f);
   }
   float shift05 = (0.5f + (pw * 0.49f));
   float shift = (_ctx.phase + shift05);
   if(shift > 1.f){
      shift = (shift - 1.f);
   }
   float tmp1 = pulse_train(_ctx.m,_ctx.phase);
   float tmp2 = pulse_train(_ctx.m,shift);
   _ctx.phase = (_ctx.phase + _ctx.rate);
   if(_ctx.phase > 1.f){
      _ctx.phase = (_ctx.phase - 1.f);
   }
   _ctx.state_pulse = clip_float((((_ctx.state_pulse * 0.9995f) + tmp1) - tmp2),(- 1.f),1.f);
   _ctx.state_saw = clip_float(((_ctx.state_saw * 0.9995f) + ((((tmp1 + tmp2) - (2.f * _ctx.rate)) / shift05) / 2.f)),(- 1.f),1.f);
   _ctx.state_triang = clip_float(((_ctx.state_triang * 0.9995f) + ((2.f * _ctx.state_pulse) * _ctx.rate)),(- 1.f),1.f);
   if(wave < (1.f / 3.f)){
      _ctx.output = _ctx.state_pulse;
   }
   else
   {
      if(wave < (2.f / 3.f)){
         _ctx.output = (2.f * _ctx.state_saw);
      }
      else
      {
         _ctx.output = ((2.f * _ctx.state_triang) * (1.f + pw));
      }
   }
   _ctx.output = dcblock(_ctx._inst1,_ctx.output);
   return clip_float((_ctx.output / 4.f),(- 1.f),1.f);
}

_ctx_type_11 _ctx_type_11_init(){
   _ctx_type_11 _ctx;
   _ctx.dlow = 0.f;
   _ctx.dband = 0.f;
   return _ctx;
}

_ctx_type_11 svf_step_init(){ return _ctx_type_11_init();}

float svf_step(_ctx_type_11 &_ctx, float input, float g, float q, int sel){
   float low = (_ctx.dlow + (g * _ctx.dband));
   float high = ((input - low) - (q * _ctx.dband));
   float band = ((g * high) + _ctx.dband);
   float notch = (high + low);
   _ctx.dband = clip_float(band,(- 1.f),1.f);
   _ctx.dlow = clip_float(low,(- 1.f),1.f);
   float output = ((sel == 0)?low:((sel == 1)?high:((sel == 2)?band:notch)));
   return output;
}

_ctx_type_12 _ctx_type_12_init(){
   _ctx_type_12 _ctx;
   _ctx.step = _ctx_type_11_init();
   _ctx.g = 0.f;
   _ctx._inst0 = _ctx_type_1_init();
   return _ctx;
}

_ctx_type_12 svf_init(){ return _ctx_type_12_init();}

float svf(_ctx_type_12 &_ctx, float input, float fc, float q, int sel){
   fc = clip_float(fc,0.f,1.f);
   q = clip_float(q,0.f,1.f);
   float fix_q = (2.f * (1.f - q));
   if(change(_ctx._inst0,fc)){
      _ctx.g = (fc / 2.f);
   }
   float x1 = svf_step(_ctx.step,input,_ctx.g,fix_q,sel);
   float x2 = svf_step(_ctx.step,input,_ctx.g,fix_q,sel);
   return ((x1 + x2) / 2.f);
}

_ctx_type_13 _ctx_type_13_init(){
   _ctx_type_13 _ctx;
   _ctx.value = 0.f;
   _ctx.sustainLevel = 0.f;
   _ctx.state = 0;
   _ctx.releaseRate = 0.f;
   _ctx.gate = 0;
   _ctx.decayRate = 0.f;
   _ctx.attackRate = 0.f;
   _ctx._inst2 = _ctx_type_7_init();
   _ctx._inst1 = _ctx_type_2_init();
   _ctx._inst0 = _ctx_type_4_init();
   return _ctx;
}

_ctx_type_13 adsr_init(){ return _ctx_type_13_init();}

float adsr(_ctx_type_13 &_ctx, uint8_t gate, float attack, float decay, float sustain, float release){
   int IDLE = 0;
   int ATTACK = 1;
   int DECAY = 2;
   int SUSTAIN = 3;
   int RELEASE = 4;
   if(each(_ctx._inst0,32)){
      _ctx.attackRate = ((1.f / 44100.f) * (1.f / (attack + 0.1f)));
      float inv_sustain = (1.f - sustain);
      _ctx.decayRate = ((inv_sustain / 44100.f) * (inv_sustain / (decay + 0.1f)));
      _ctx.releaseRate = ((0.5f / 44100.f) * (0.5f / (release + 0.1f)));
   }
   _ctx.sustainLevel = sustain;
   uint8_t trig = 0;
   float rate = 0.f;
   trig = bchange(_ctx._inst1,_ctx.gate);
   uint8_t up = (trig && _ctx.gate);
   uint8_t down = (trig && (! _ctx.gate));
   if(_ctx.state == IDLE){
      if(up){
         _ctx.state = ATTACK;
      }
   }
   else
   {
      if(_ctx.state == ATTACK){
         if(_ctx.value >= 1.f){
            _ctx.state = DECAY;
         }
         if(down){
            _ctx.state = RELEASE;
         }
         rate = _ctx.attackRate;
      }
      else
      {
         if(_ctx.state == DECAY){
            if(_ctx.value <= _ctx.sustainLevel){
               _ctx.state = SUSTAIN;
            }
            if(down){
               _ctx.state = RELEASE;
            }
            rate = (- _ctx.decayRate);
         }
         else
         {
            if(_ctx.state == SUSTAIN){
               if(down){
                  _ctx.state = RELEASE;
               }
               rate = 0.f;
               _ctx.value = _ctx.sustainLevel;
            }
            else
            {
               if(_ctx.state == RELEASE){
                  if(_ctx.value <= 0.f){
                     _ctx.state = IDLE;
                  }
                  if(up){
                     _ctx.state = ATTACK;
                  }
                  rate = (- _ctx.releaseRate);
               }
            }
         }
      }
   }
   _ctx.value = clip_float((lpfilter(_ctx._inst2,rate) + _ctx.value),0.f,1.f);
   return _ctx.value;
}

_ctx_type_14 _ctx_type_14_init(){
   _ctx_type_14 _ctx;
   _ctx.phase = 0.f;
   _ctx._inst1 = _ctx_type_4_init();
   _ctx._inst0 = _ctx_type_3_init();
   return _ctx;
}

_ctx_type_14 lfo_init(){ return _ctx_type_14_init();}

float lfo(_ctx_type_14 &_ctx, float f, uint8_t gate){
   float rate = (((f * 100.f) * minFixed()) + minFixed());
   if(edge(_ctx._inst0,gate)){
      _ctx.phase = 0.f;
   }
   if(each(_ctx._inst1,4)){
      _ctx.phase = (_ctx.phase + rate);
   }
   if(_ctx.phase > 1.f){
      _ctx.phase = (_ctx.phase - 1.f);
   }
   return (sinf(((_ctx.phase * 2.f) * 3.14159265359f)) + 0.5f);
}

_ctx_type_15 _ctx_type_15_init(){
   _ctx_type_15 _ctx;
   _ctx.pre = 0;
   _ctx.n4 = 0;
   _ctx.n3 = 0;
   _ctx.n2 = 0;
   _ctx.n1 = 0;
   _ctx.count = 0;
   return _ctx;
}

_ctx_type_15 noteOn_init(){ return _ctx_type_15_init();}

int noteOn(_ctx_type_15 &_ctx, int n){
   if(_ctx.count == 0){
      _ctx.n1 = n;
      _ctx.pre = n;
   }
   else
   {
      if(_ctx.count == 1){
         _ctx.n2 = n;
         _ctx.pre = n;
      }
      else
      {
         if(_ctx.count == 2){
            _ctx.n3 = n;
            _ctx.pre = n;
         }
         else
         {
            if(_ctx.count == 3){
               _ctx.n4 = n;
               _ctx.pre = n;
            }
         }
      }
   }
   if(_ctx.count <= 4){
      _ctx.count = (_ctx.count + 1);
   }
   return _ctx.pre;
}

_ctx_type_15 noteOff_init(){ return _ctx_type_15_init();}

int noteOff(_ctx_type_15 &_ctx, int n){
   uint8_t found = 0;
   if(n == _ctx.n1){
      int _tmp_0 = _ctx.n2;
      int _tmp_1 = _ctx.n3;
      int _tmp_2 = _ctx.n4;
      _ctx.n1 = _tmp_0;
      _ctx.n2 = _tmp_1;
      _ctx.n3 = _tmp_2;
      found = 1;
   }
   else
   {
      if(n == _ctx.n2){
         int _tmp_0 = _ctx.n3;
         int _tmp_1 = _ctx.n4;
         _ctx.n2 = _tmp_0;
         _ctx.n3 = _tmp_1;
         found = 1;
      }
      else
      {
         if(n == _ctx.n3){
            _ctx.n3 = _ctx.n4;
            found = 1;
         }
         else
         {
            if(n == _ctx.n4){
               found = 1;
            }
         }
      }
   }
   if(found && (_ctx.count > 0)){
      _ctx.count = (_ctx.count - 1);
   }
   if(_ctx.count == 1){
      _ctx.pre = _ctx.n1;
   }
   if(_ctx.count == 2){
      _ctx.pre = _ctx.n2;
   }
   if(_ctx.count == 3){
      _ctx.pre = _ctx.n3;
   }
   if(_ctx.count == 4){
      _ctx.pre = _ctx.n4;
   }
   return _ctx.pre;
}

_ctx_type_15 isGateOn_init(){ return _ctx_type_15_init();}

uint8_t isGateOn(_ctx_type_15 &_ctx){
   return (_ctx.count > 0);
}

_ctx_type_16 _ctx_type_16_init(){
   _ctx_type_16 _ctx;
   _ctx.param9 = 0.f;
   _ctx.param8 = 0.f;
   _ctx.param7 = 0.f;
   _ctx.param6 = 0.f;
   _ctx.param5 = 0.f;
   _ctx.param4 = 0.f;
   _ctx.param3 = 0.f;
   _ctx.param2 = 0.f;
   _ctx.param16 = 0.f;
   _ctx.param15 = 0.f;
   _ctx.param14 = 0.f;
   _ctx.param13 = 0.f;
   _ctx.param12 = 0.f;
   _ctx.param11 = 0.f;
   _ctx.param10 = 0.f;
   _ctx.param1 = 0.f;
   _ctx.monoin = _ctx_type_15_init();
   _ctx._inst4 = _ctx_type_12_init();
   _ctx._inst3 = _ctx_type_13_init();
   _ctx._inst2 = _ctx_type_13_init();
   _ctx._inst1 = _ctx_type_10_init();
   _ctx._inst0 = _ctx_type_14_init();
   return _ctx;
}

_ctx_type_16 process_init(){ return _ctx_type_16_init();}

float process(_ctx_type_16 &_ctx, float i){
   uint8_t gate = isGateOn(_ctx.monoin);
   float lfo1 = (lfo(_ctx._inst0,_ctx.param5,gate) * _ctx.param4);
   float x = osc(_ctx._inst1,_ctx.param1,(_ctx.param2 + lfo1),_ctx.param3);
   float amp_env = adsr(_ctx._inst2,gate,_ctx.param9,_ctx.param10,_ctx.param11,_ctx.param12);
   float flt_env = adsr(_ctx._inst3,gate,_ctx.param13,_ctx.param14,_ctx.param15,_ctx.param16);
   float cut_mod = (_ctx.param6 + (_ctx.param8 * flt_env));
   float output = svf(_ctx._inst4,x,cut_mod,_ctx.param7,0);
   return (output / 2.f);
}

_ctx_type_16 process_noteOn_init(){ return _ctx_type_16_init();}

void process_noteOn(_ctx_type_16 &_ctx, int n){
   noteOn(_ctx.monoin,n);
}

_ctx_type_16 process_noteOff_init(){ return _ctx_type_16_init();}

void process_noteOff(_ctx_type_16 &_ctx, int n){
   noteOff(_ctx.monoin,n);
}


