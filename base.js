var SCREEN_WIDTH = 0;
var SCREEN_HEIGHT = 0;
var PLAYER_RADIUS = 12;
var PLAYER_SPEED = 4;
var PLAYER_LIFE = 16;
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

var decToHex = function(dec) {
  return [Math.floor(dec / 16), Math.floor(dec % 16)].map(function(d) {
    if (d < 10) return d.toString();
    else return String.fromCharCode(d + 55);
  }).join('');
}

var renderPath, fill, stroke, fillRect, strokeRect, fillText, strokeText, createLinearGradient;
(function(context) {
  renderPath = function(pathPoints, close, xOff, yOff) {
    var xOff = typeof xOff === 'number' ? xOff : 0;
    var yOff = typeof yOff === 'number' ? yOff : 0;
    context.beginPath();
    pathPoints.forEach(function(p) {
      switch(p[0]) {
        case 'moveTo' : context.moveTo(p[1] + xOff, p[2] + yOff);
          break;
        case 'lineTo' : context.lineTo(p[1] + xOff, p[2] + yOff);
          break;
        case 'arc'  : context.arc(p[1] + xOff, p[2] + yOff, p[3], p[4], p[5]);
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
    context.fillRect(x, y, w, h);
  }
  strokeRect = function(x, y, w, h, stroke, lineWidth) {
    if (stroke) context.strokeStyle = stroke;
    if (lineWidth) context.lineWidth = lineWidth;
    context.strokeRect(x, y, w, h);
  }

  fillText = function(text, x, y, fill, options) {
    if (fill) context.fillStyle = fill;
    if (options) {
      for (var o in options) {
        if (options.hasOwnProperty(o) && context[o]) context[o] = options[o];
      }
    }
    context.fillText(text, x, y);
  }
  strokeText = function(text, x, y, stroke, lineWidth, options) {
    if (stroke) context.strokeStyle = stroke;
    if (lineWidth) context.lineWidth = lineWidth;
    if (options) {
      for (var o in options) {
        if (options.hasOwnProperty(o) && context[o]) context[o] = options[o];
      }
    }
    context.strokeText(text, x, y);
  }

  createLinearGradient = function(x1, y1, x2, y2, colorStops) {
    var grad = context.createLinearGradient(x1, y1, x2, y2);
    colorStops.forEach(function(s, i) {
      grad.addColorStop(i, s);
    });
    return grad;
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
