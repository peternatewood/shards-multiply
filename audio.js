gAudio = (function(context) {
  ChannelBase = function() {
    this.gain = context.createGain();
    this.gain.gain.value = 0;
    this.gain.connect(context.destination);

    return this;
  }
  ChannelBase.prototype.start = function() {
    var now = context.currentTime;
    this.gain.gain.linearRampToValueAtTime(0, now);
    this.gain.gain.linearRampToValueAtTime(0.1, now + 0.01);
    this.osc = context.createOscillator();
    this.osc.connect(this.gain);
    this.osc.start();
  }
  ChannelBase.prototype.stop = function(delay) {
    this.osc.stop(delay);
  }

  EffectChannel.prototype = new ChannelBase();
  EffectChannel.prototype.startBolt = function() {
    this.start();
    var now = context.currentTime;
    this.osc.frequency.linearRampToValueAtTime(1200, now);
    this.osc.frequency.linearRampToValueAtTime(400, now + 0.1);
    this.gain.gain.linearRampToValueAtTime(0.1, now + 0.1);
    this.gain.gain.linearRampToValueAtTime(0, now + 0.11);
    this.stop(now + 0.11);
  }

  MusicChannel.prototype = new ChannelBase();

  return {
    // context: context,
    eChan1: new EffectChannel()
  }
})(new AudioContext());
