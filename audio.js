gAudio = (function(context) {
  AudioChannel = function() {
    this.gain = context.createGain();
    this.gain.gain.value = 0;
    this.gain.connect(context.destination);

    return this;
  }
  AudioChannel.prototype.start = function(freq, delay) {
    var start = context.currentTime;
    if (typeof delay === 'number') start += delay;
    this.gain.gain.linearRampToValueAtTime(0, start);
    this.gain.gain.linearRampToValueAtTime(0.1, start + 0.01);
    this.osc = context.createOscillator();
    this.osc.connect(this.gain);
    if (freq > 0) this.osc.frequency.value = freq;
    this.osc.start(start);
  }
  AudioChannel.prototype.stop = function(delay) {
    this.osc.stop(delay);
  }

  return {
    eChan1: new AudioChannel(),
    mChan1: new AudioChannel(),
    startBolt: function() {
      var now = context.currentTime;
      this.eChan1.start(now);
      this.eChan1.osc.frequency.linearRampToValueAtTime(1200, now);
      this.eChan1.osc.frequency.linearRampToValueAtTime(400, now + 0.1);
      this.eChan1.gain.gain.linearRampToValueAtTime(0.1, now + 0.1);
      this.eChan1.gain.gain.linearRampToValueAtTime(0, now + 0.11);
      this.eChan1.stop(now + 0.11);
    }
  }
})(new AudioContext());
