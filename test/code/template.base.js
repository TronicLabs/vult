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
this.process = function(_ctx,input){
   return input;
}
this.noteOn = function(_ctx,note,velocity){
}
this.noteOff = function(_ctx,note){
}
this.controlChange = function(_ctx,control,value){
}
this.default_ = function(){
}

    if(this.process_init)  this.context =  this.process_init(); else this.context = {};
    if(this.default_)      this.default_(this.context);
    this.liveNoteOn        = function(note,velocity) { if(this.noteOn)        this.noteOn(this.context,note,velocity); };
    this.liveNoteOff       = function(note,velocity) { if(this.noteOff)       this.noteOff(this.context,note,velocity); };
    this.liveControlChange = function(note,velocity) { if(this.controlChange) this.controlChange(this.context,note,velocity); };
    this.liveProcess       = function(input)         { if(this.process)       return this.process(this.context,input); else return 0; };
    this.liveDefault       = function()              { if(this.default_)      return this.default_(this.context); };
}
