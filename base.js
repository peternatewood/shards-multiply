var SCREEN_WIDTH = 0;
var SCREEN_HEIGHT = 0;
var PLAYER_RADIUS = 12;
var PLAYER_SPEED = 4;
var FRICTION = 0.8;
var LEVELS = [
  { l: 48, t: 24, r: 752, b: 576 },
  { l: 0, t: 0, r: 2000, b: 2000 }
];

var gInput = {}
var gPlayer = {}
var gScene = {}
var gAudio = {}
var gRenderer = {}

var Target = function() { return this }
var Bolt = function() { return this }
var ChannelBase = function() { return this }
var EffectChannel = function() { return this }
var MusicChannel = function() { return this }
