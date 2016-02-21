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
   _ctx.pre_x = 0.;
   return _ctx;
}
this. change_init = function() { return this._ctx_type_0_init();}
this.change = function(_ctx,x){
   var v = (_ctx.pre_x != x);
   _ctx.pre_x = x;
   return v;
}


this._ctx_type_1_init = function(){
   var _ctx = {};
   _ctx.dlow = 0.;
   _ctx.dband = 0.;
   return _ctx;
}
this. svf_step_init = function() { return this._ctx_type_1_init();}
this.svf_step = function(_ctx,input,g,q,sel){
   var low = (_ctx.dlow + (g * _ctx.dband));
   var high = ((input - low) - (q * _ctx.dband));
   var band = ((g * high) + _ctx.dband);
   var notch = (high + low);
   _ctx.dband = this.clip(band,(- 1.),1.);
   _ctx.dlow = this.clip(low,(- 1.),1.);
   var output = ((sel == (0|0))?low:((sel == (1|0))?high:((sel == (2|0))?band:notch)));
   return output;
}


this._ctx_type_2_init = function(){
   var _ctx = {};
   _ctx.step = this._ctx_type_1_init();
   _ctx.g = 0.;
   _ctx._inst0 = this._ctx_type_0_init();
   return _ctx;
}
this. svf_init = function() { return this._ctx_type_2_init();}
this.svf = function(_ctx,input,fc,q,sel){
   fc = this.clip(fc,0.,1.);
   q = this.clip(q,0.,1.);
   var fix_q = (2. * (1. - q));
   if(this.change(_ctx._inst0,fc)){
      _ctx.g = (fc / 2.);
   }
   var x1 = this.svf_step(_ctx.step,input,_ctx.g,fix_q,sel);
   var x2 = this.svf_step(_ctx.step,input,_ctx.g,fix_q,sel);
   return ((x1 + x2) / 2.);
}

    if(this.process_init)  this.context =  this.process_init(); else this.context = {};
    if(this.default_)      this.default_(this.context);
    this.liveNoteOn        = function(note,velocity) { if(this.noteOn)        this.noteOn(this.context,note,velocity); };
    this.liveNoteOff       = function(note,velocity) { if(this.noteOff)       this.noteOff(this.context,note,velocity); };
    this.liveControlChange = function(note,velocity) { if(this.controlChange) this.controlChange(this.context,note,velocity); };
    this.liveProcess       = function(input)         { if(this.process)       return this.process(this.context,input); else return 0; };
    this.liveDefault       = function()              { if(this.default_)      return this.default_(this.context); };
}
