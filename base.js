var SCREEN_WIDTH = 0;
var SCREEN_HEIGHT = 0;
var PLAYER_RADIUS = 12;
var PLAYER_SPEED = 4;
var THRUSTER_SPEED = 8;
var CAMERA_SPEED = 64;
var FRICTION = 0.8;
var LEVELS = [
  { l: 0, t: 0, r: 704, b: 552 },
  { l: 0, t: 0, r: 2000, b: 2000 }
];

var round = function(num, place) {
  if (isNaN(num)) return num;
  var place = place ? place : 2;
  var mod = Math.pow(10, place);
  return Math.round(num * mod) / mod;
}

var renderPath, fill, stroke, fillRect, strokeRect, fillText, strokeText;
(function(context) {
  renderPath = function(pathPoints, close) {
    context.beginPath();
    pathPoints.forEach(function(p) {
      switch(p[0]) {
        case 'moveTo' : context.moveTo(p[1], p[2]);
          break;
        case 'lineTo' : context.lineTo(p[1], p[2]);
          break;
        case 'arc'  : context.arc(p[1], p[2], p[3], p[4], p[5]);
          break;
      }
    });

    if (close) context.closePath();
  }

  fill = function(fill) {
    if (fill) context.fillStyle = fill;
    context.fill();
  }

  stroke = function(stroke, lineWidth) {
    if (stroke) context.strokeStyle = stroke;
    if (lineWidth) context.lineWidth = lineWidth;
    context.stroke();
  }

  fillRect = function(x, y, w, h, fill) {
    if (fill) context.fillStyle = fill;
    context.strokeRect(x, y, w, h);
  }

  strokeRect = function(x, y, w, h, stroke, lineWidth) {
    if (stroke) context.strokeStyle = stroke;
    if (lineWidth) context.lineWidth = lineWidth;
    context.strokeRect(x, y, w, h);
  }

  fillText = function(text, x, y, fill, font) {
    if (fill) context.fillStyle = fill;
    if (font) context.font = font;
    context.fillText(text, x, y);
  }
  strokeText = function(text, x, y, stroke, lineWidth, font) {
    if (stroke) context.strokeStyle = stroke;
    if (lineWidth) context.lineWidth = lineWidth;
    if (font) context.font = font;
    context.strokeText(text, x, y);
  }
}) (document.getElementById('canvas').getContext('2d'));

var gInput = {}
var gPlayer = {}
var gScene = {}
var gAudio = {}
var gRenderer = {}
var gCamera = {}

var Target = function() { return this }
var Bolt = function() { return this }
var AudioChannel = function() { return this }
