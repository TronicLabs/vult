function vultProcess(){
    this.clip = function(x,low,high) { return x<low?low:(x>high?high:x); };
    this.not  = function(x)          { return x==0?1:0; };
    this.real = function(x)          { return x; };
    this.int  = function(x)          { return x|0; };
    this.sin  = function(x)          { return Math.sin(x); };
    this.cos  = function(x)          { return Math.cos(x); };
    this.abs  = function(x)          { return Math.abs(x); };
    this.exp  = function(x)          { return Math.exp(x); };
    this.floor= function(x)          { return Math.floor(x); };
    this.tan  = function(x)          { return Math.tan(x); };
    this.tanh = function(x)          { return Math.tanh(x); };
    this.sqrt = function(x)          { return x; };
    this.process_init = null;
    this.default_ = null;


this._ctx_type_0_init = function(){
   var _ctx = {};
   _ctx.count = ((0|0)|0);
   return _ctx;
}
this. each_init = function() { return this._ctx_type_0_init();}
this.each = function(_ctx,n){
   var ret = (_ctx.count == (0|0));
   _ctx.count = (((_ctx.count + (1|0)) % n)|0);
   return ret;
}


this._ctx_type_1_init = function(){
   var _ctx = {};
   _ctx.pre_x = false;
   return _ctx;
}
this. bchange_init = function() { return this._ctx_type_1_init();}
this.bchange = function(_ctx,x){
   var v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}


this._ctx_type_2_init = function(){
   var _ctx = {};
   _ctx.pre_x = 0.;
   return _ctx;
}
this. lpfilter_init = function() { return this._ctx_type_2_init();}
this.lpfilter = function(_ctx,x){
   var ret = ((x + _ctx.pre_x) / 2.);
   _ctx.pre_x = x;
   return ret;
}


this._ctx_type_3_init = function(){
   var _ctx = {};
   _ctx.value = 0.;
   _ctx.sustainLevel = 0.;
   _ctx.state = ((0|0)|0);
   _ctx.releaseRate = 0.;
   _ctx.gate = false;
   _ctx.decayRate = 0.;
   _ctx.attackRate = 0.;
   _ctx._inst2 = this._ctx_type_2_init();
   _ctx._inst1 = this._ctx_type_1_init();
   _ctx._inst0 = this._ctx_type_0_init();
   return _ctx;
}
this. adsr_init = function() { return this._ctx_type_3_init();}
this.adsr = function(_ctx,gate,attack,decay,sustain,release){
   var IDLE = ((0|0)|0);
   var ATTACK = ((1|0)|0);
   var DECAY = ((2|0)|0);
   var SUSTAIN = ((3|0)|0);
   var RELEASE = ((4|0)|0);
   if(this.each(_ctx._inst0,(32|0))){
      _ctx.attackRate = ((1. / 44100.) * (1. / (attack + 0.1)));
      var inv_sustain = (1. - sustain);
      _ctx.decayRate = ((inv_sustain / 44100.) * (inv_sustain / (decay + 0.1)));
      _ctx.releaseRate = ((0.5 / 44100.) * (0.5 / (release + 0.1)));
   }
   _ctx.sustainLevel = sustain;
   var trig = false;
   var rate = 0.;
   trig = this.bchange(_ctx._inst1,_ctx.gate);
   var up = (trig && _ctx.gate);
   var down = (trig && this.not(_ctx.gate));
   if((_ctx.state == IDLE)){
      if(up){
         _ctx.state = (ATTACK|0);
      }
   }
   else
   {
      if((_ctx.state == ATTACK)){
         if((_ctx.value >= 1.)){
            _ctx.state = (DECAY|0);
         }
         if(down){
            _ctx.state = (RELEASE|0);
         }
         rate = _ctx.attackRate;
      }
      else
      {
         if((_ctx.state == DECAY)){
            if((_ctx.value <= _ctx.sustainLevel)){
               _ctx.state = (SUSTAIN|0);
            }
            if(down){
               _ctx.state = (RELEASE|0);
            }
            rate = (- _ctx.decayRate);
         }
         else
         {
            if((_ctx.state == SUSTAIN)){
               if(down){
                  _ctx.state = (RELEASE|0);
               }
               rate = 0.;
               _ctx.value = _ctx.sustainLevel;
            }
            else
            {
               if((_ctx.state == RELEASE)){
                  if((_ctx.value <= 0.)){
                     _ctx.state = (IDLE|0);
                  }
                  if(up){
                     _ctx.state = (ATTACK|0);
                  }
                  rate = (- _ctx.releaseRate);
               }
            }
         }
      }
   }
   _ctx.value = this.clip((this.lpfilter(_ctx._inst2,rate) + _ctx.value),0.,1.);
   return _ctx.value;
}

    if(this.process_init)  this.context =  this.process_init(); else this.context = {};
    if(this.default_)      this.default_(this.context);
    this.liveNoteOn        = function(note,velocity) { if(this.noteOn)        this.noteOn(this.context,note,velocity); };
    this.liveNoteOff       = function(note,velocity) { if(this.noteOff)       this.noteOff(this.context,note,velocity); };
    this.liveControlChange = function(note,velocity) { if(this.controlChange) this.controlChange(this.context,note,velocity); };
    this.liveProcess       = function(input)         { if(this.process)       return this.process(this.context,input); else return 0; };
    this.liveDefault       = function()              { if(this.default_)      return this.default_(this.context); };
}
