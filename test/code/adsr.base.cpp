#include "..h"

_ctx_type_0 _ctx_type_0_init(){
   _ctx_type_0 _ctx;
   _ctx.count = 0;
   return _ctx;
}

_ctx_type_0 each_init(){ return _ctx_type_0_init();}

uint8_t each(_ctx_type_0 &_ctx, int n){
   uint8_t ret = (_ctx.count == 0);
   _ctx.count = ((_ctx.count + 1) % n);
   return ret;
}

_ctx_type_1 _ctx_type_1_init(){
   _ctx_type_1 _ctx;
   _ctx.pre_x = 0;
   return _ctx;
}

_ctx_type_1 bchange_init(){ return _ctx_type_1_init();}

uint8_t bchange(_ctx_type_1 &_ctx, uint8_t x){
   uint8_t v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}

_ctx_type_2 _ctx_type_2_init(){
   _ctx_type_2 _ctx;
   _ctx.pre_x = 0.f;
   return _ctx;
}

_ctx_type_2 lpfilter_init(){ return _ctx_type_2_init();}

float lpfilter(_ctx_type_2 &_ctx, float x){
   float ret = ((x + _ctx.pre_x) / 2.f);
   _ctx.pre_x = x;
   return ret;
}

_ctx_type_3 _ctx_type_3_init(){
   _ctx_type_3 _ctx;
   _ctx.value = 0.f;
   _ctx.sustainLevel = 0.f;
   _ctx.state = 0;
   _ctx.releaseRate = 0.f;
   _ctx.gate = 0;
   _ctx.decayRate = 0.f;
   _ctx.attackRate = 0.f;
   _ctx._inst2 = _ctx_type_2_init();
   _ctx._inst1 = _ctx_type_1_init();
   _ctx._inst0 = _ctx_type_0_init();
   return _ctx;
}

_ctx_type_3 adsr_init(){ return _ctx_type_3_init();}

float adsr(_ctx_type_3 &_ctx, uint8_t gate, float attack, float decay, float sustain, float release){
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


