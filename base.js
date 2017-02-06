var SCREEN_WIDTH = 0;
var SCREEN_HEIGHT = 0;
var PLAYER_RADIUS = 12;
var PLAYER_SPEED = 4;
var PLAYER_LIFE = 16;
var POWERUP_SIZE = 24;
var THRUSTER_SPEED = 8;
var CAMERA_SPEED = 64;
var FRICTION = 0.8;
var LEVELS = [
  { l: 0, t: 0, r: 704, b: 552 },
  { l: 0, t: 0, r: 2000, b: 2000 }
];
var MAX_SPECIALS = [3, 2, 4, 24, 3, 4];

var POWERUP_ICONS = [
  // Bolts
  [
    ['moveTo',  8, -6],
    ['lineTo', 12, -2],
    ['lineTo',  8,  6],
    ['lineTo',  4, -2],
    ['lineTo',  8, -6],

    ['moveTo',  -8, -6],
    ['lineTo',  -4, -2],
    ['lineTo',  -8,  6],
    ['lineTo', -12, -2],
    ['lineTo',  -8, -6],
  ],
  // Clone
  [
    ['moveTo', 0, -PLAYER_RADIUS],
    ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(Math.PI / -4), (PLAYER_RADIUS / 2) * Math.sin(Math.PI / -4)],
    ['lineTo', PLAYER_RADIUS, 0],
    ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(Math.PI * 1 / 4), (PLAYER_RADIUS * 1.25) * Math.sin(Math.PI * 1 / 4)],
    ['lineTo', 0, PLAYER_RADIUS / 2],
    ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(Math.PI * 3 / 4), (PLAYER_RADIUS * 1.25) * Math.sin(Math.PI * 3 / 4)],
    ['lineTo', -PLAYER_RADIUS, 0],
    ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(Math.PI * 3 / -4), (PLAYER_RADIUS / 2) * Math.sin(Math.PI * 3 / -4)],
  ],
  // Missile
  [
    ['moveTo',  0, -8],
    ['lineTo',  3,  3],
    ['lineTo',  3,  4],
    ['lineTo',  5,  4],
    ['lineTo',  5, 10],
    ['lineTo',  4, 10],
    ['lineTo',  1,  7],
    ['lineTo', -1,  7],
    ['lineTo', -4, 10],
    ['lineTo', -5, 10],
    ['lineTo', -5,  4],
    ['lineTo', -3,  4],
    ['lineTo', -3,  3],
  ],
  // Beam
  [
    ['moveTo', -12,  12],
    ['lineTo', -12,   6],
    ['lineTo',   6, -12],
    ['lineTo',  12,  -6],
    ['lineTo',  -6,  12]
  ],
  // Shield
  [
    ['moveTo', 4, 0],
    ['arc', 0, 0, 4, 0, 2 * Math.PI],

    ['moveTo', 9, 0],
    ['arc', 0, 0, 9, 0, 2 * Math.PI],

    ['moveTo', 14, 0],
    ['arc', 0, 0, 14, 0, 2 * Math.PI]
  ],
  // Armor
  [
    ['moveTo', 16 * Math.cos(3 * Math.PI / 4), 16 * Math.sin(3 * Math.PI / 4)],
    ['lineTo',  5 * Math.cos(5 * Math.PI / 4),  5 * Math.sin(5 * Math.PI / 4)],
    ['lineTo',  2 * Math.cos(5 * Math.PI / 4),  2 * Math.sin(5 * Math.PI / 4)],
    ['lineTo',  2 * Math.cos(5 * Math.PI / 4),  2 * Math.sin(5 * Math.PI / 4)],
    ['lineTo', 16 * Math.cos(7 * Math.PI / 4), 16 * Math.sin(7 * Math.PI / 4)],
    ['lineTo',  5 * Math.cos(Math.PI / 4),  5 * Math.sin(Math.PI / 4)],
    ['lineTo',  2 * Math.cos(Math.PI / 4),  2 * Math.sin(Math.PI / 4)]
  ]
];
var POWERUP_COLORS = [
  ['#7788CC', '#FFFF88',   '#88FF88', 2], // Bolts
  ['#77AA44', '#44998B',   '#FFDD77', 3], // Clone
  ['#CC5555', '#CCCCCC',   '#555555', 2], // Missile
  ['#552277', '#FFFFFF',   '#EEAA22', 2], // Beam
  ['#CC5511', '#11AA1155', '#22EE22', 1], // Shield
  ['#EEEE22', '#3377DD',   '#77AAFF', 1]  // Armor
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
  renderPath = function(pathPoints, close, xOff, yOff, rad) {
    var xOff = typeof xOff === 'number' ? xOff : 0;
    var yOff = typeof yOff === 'number' ? yOff : 0;
    context.beginPath();
    pathPoints.forEach(function(p) {
      if (typeof rad === 'number') {
        switch(p[0]) {
          case 'moveTo': context.moveTo(xOff + (p[1] * Math.cos(rad)) - (p[2] * Math.sin(rad)), yOff + (p[2] * Math.cos(rad)) + (p[1] * Math.sin(rad)));
            break;
          case 'lineTo': context.lineTo(xOff + (p[1] * Math.cos(rad)) - (p[2] * Math.sin(rad)), yOff + (p[2] * Math.cos(rad)) + (p[1] * Math.sin(rad)));
            break;
        }
      }
      else {
        switch(p[0]) {
          case 'moveTo' : context.moveTo(p[1] + xOff, p[2] + yOff);
            break;
          case 'lineTo' : context.lineTo(p[1] + xOff, p[2] + yOff);
            break;
          case 'arc'  : context.arc(p[1] + xOff, p[2] + yOff, p[3], p[4], p[5]);
            break;
        }
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
var Missile = function() { return this }
var AudioChannel = function() { return this }
var Powerup = function() { return this }
