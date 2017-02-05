var POWERUP_ICONS = [
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
  [
    ['moveTo',  0, -8],
    ['lineTo',  3,  3],
    ['lineTo',  3,  4],
    ['lineTo',  5,  4],
    ['lineTo',  5,  10],
    ['lineTo',  4,  10],
    ['lineTo',  1,  7],
    ['lineTo', -1,  7],
    ['lineTo', -4,  10],
    ['lineTo', -5,  10],
    ['lineTo', -5,  4],
    ['lineTo', -3,  4],
    ['lineTo', -3,  3],
  ],
  [
    ['moveTo', -12,  12],
    ['lineTo', -12,   6],
    ['lineTo',   6, -12],
    ['lineTo',  12,  -6],
    ['lineTo', -6,   12]
  ],
  [
    ['moveTo', 4, 0],
    ['arc', 0, 0, 4, 0, 2 * Math.PI],

    ['moveTo', 9, 0],
    ['arc', 0, 0, 9, 0, 2 * Math.PI],

    ['moveTo', 14, 0],
    ['arc', 0, 0, 14, 0, 2 * Math.PI]
  ],
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
  ['#78C', '#FF8', '#8F8', 2],
  ['#7A4', '#49B', '#FD7', 3],
  ['#C55', '#CCC', '#555', 2],
  ['#527', '#FFF', '#EA2', 2],
  ['#C51', '#1A15', '#2E2', 1],
  ['#EE2', '#37D', '#7AF', 1]
];

var Powerup = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.type = type;
  this.size = POWERUP_SIZE;
  this.frame = 0;

  return this;
}
Powerup.types = ['bolt', 'clone', 'speed', 'battery'];
Powerup.outerPath = [
  ['moveTo', -POWERUP_SIZE, 6 - POWERUP_SIZE],
  ['lineTo', 6 - POWERUP_SIZE, -POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE - 6, -POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE, 6 - POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE, POWERUP_SIZE - 6],
  ['lineTo', POWERUP_SIZE - 6, POWERUP_SIZE],
  ['lineTo', 6 - POWERUP_SIZE, POWERUP_SIZE],
  ['lineTo', -POWERUP_SIZE, POWERUP_SIZE - 6],
];
Powerup.innerPath = [
  ['moveTo', 5 - POWERUP_SIZE, 8 - POWERUP_SIZE],
  ['lineTo', 8 - POWERUP_SIZE, 5 - POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE - 8, 5 - POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE - 5, 8 - POWERUP_SIZE],
  ['lineTo', POWERUP_SIZE - 5, POWERUP_SIZE - 8],
  ['lineTo', POWERUP_SIZE - 8, POWERUP_SIZE - 5],
  ['lineTo', 8 - POWERUP_SIZE, POWERUP_SIZE - 5],
  ['lineTo', 5 - POWERUP_SIZE, POWERUP_SIZE - 8],
];
Powerup.prototype.collide = function() {
  return gPlayer.x >= this.x - POWERUP_SIZE && gPlayer.y >= this.y - POWERUP_SIZE && gPlayer.x <= this.x + POWERUP_SIZE && gPlayer.y <= this.y + POWERUP_SIZE;
}
Powerup.prototype.getType = function() {
  return Powerup.types[this.type];
}
Powerup.prototype.render = function() {
  renderPath(Powerup.outerPath, true, this.x - gCamera.x, this.y - gCamera.y);

  var strokeStyle, lineWidth, fillStyle;
  fill(POWERUP_COLORS[this.type][0]);
  stroke('#222', 2);
  renderPath(Powerup.innerPath, true, this.x - gCamera.x, this.y - gCamera.y);
  stroke();

  renderPath(POWERUP_ICONS[this.type], true, this.x - gCamera.x, this.y - gCamera.y);
  fill(POWERUP_COLORS[this.type][1]);
  stroke(POWERUP_COLORS[this.type][2], POWERUP_COLORS[this.type][3]);
};
