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
    },
    playNote: function(chan, note, octave, start, duration) {
      var base;
      switch (note) {
        case 'c'  : base = 16.35; break;
        case 'db' : base = 17.32; break;
        case 'd'  : base = 18.35; break;
        case 'eb' : base = 19.45; break;
        case 'e'  : base = 20.60; break;
        case 'f'  : base = 21.83; break;
        case 'gb' : base = 23.12; break;
        case 'g'  : base = 24.50; break;
        case 'ab' : base = 25.96; break;
        case 'a'  : base = 27.50; break;
        case 'bb' : base = 29.14; break;
        case 'b'  : base = 30.87; break;
      }
      this['mChan' + chan].start(base * Math.pow(2, octave), start);
      this['mChan' + chan].stop(context.currentTime + start + duration);
    },
    playSong: function(song) {
      var start = 0;
      song.forEach(function(voice) {
        voice.forEach(function(note) {
          this.playNote(note[0], note[1], note[2], start, note[3]);
          start += note[3];
        }, this);
      }, this);
    }
  }
})(new AudioContext());
