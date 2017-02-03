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

    ['moveTo', PLAYER_RADIUS * Math.cos(this.rad), PLAYER_RADIUS * Math.sin(this.rad)],
    ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad + (Math.PI / 4)), (PLAYER_RADIUS / 2) * Math.sin(this.rad + (Math.PI / 4))],
    ['lineTo', PLAYER_RADIUS * Math.cos(this.rad + (Math.PI / 2)), PLAYER_RADIUS * Math.sin(this.rad + (Math.PI / 2))],
    ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(this.rad + (Math.PI * 3 / 4)), (PLAYER_RADIUS * 1.25) * Math.sin(this.rad + (Math.PI * 3 / 4))],
    ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad + Math.PI), (PLAYER_RADIUS / 2) * Math.sin(this.rad + Math.PI)],
    ['lineTo', (PLAYER_RADIUS * 1.25) * Math.cos(this.rad - (Math.PI * 3 / 4)), (PLAYER_RADIUS * 1.25) * Math.sin(this.rad - (Math.PI * 3 / 4))],
    ['lineTo', PLAYER_RADIUS * Math.cos(this.rad - (Math.PI / 2)), PLAYER_RADIUS * Math.sin(this.rad - (Math.PI / 2))],
    ['lineTo', (PLAYER_RADIUS / 2) * Math.cos(this.rad - (Math.PI / 4)), (PLAYER_RADIUS / 2) * Math.sin(this.rad - (Math.PI / 4))],
  ],
  [
    ['moveTo', -12, -8],
    ['lineTo',  12, -8],
    ['lineTo', -12, -6],
    ['lineTo', -12, -8],

    ['moveTo', -12, 0],
    ['lineTo',  12, 0],
    ['lineTo', -12, 2],
    ['lineTo', -12, 0],

    ['moveTo', -12,  8],
    ['lineTo',  12,  8],
    ['lineTo', -12, 10],
    ['lineTo', -12,  8],
  ],
  [
    ['moveTo', -6,  -8],
    ['lineTo', -3,  -8],
    ['lineTo', -3, -11],
    ['lineTo',  3, -11],
    ['lineTo',  3,  -8],
    ['lineTo',  6,  -8],
    ['lineTo',  6,  11],
    ['lineTo', -6,  11]
  ]
];
var POWERUP_COLORS = [
  ['#78C', '#FF8', '#8F8', 2],
  ['#7A4', '#49B', '#FD7', 3],
  ['#EE2', '#A31', '#F73', 1],
  ['#C55', '#6BC', '#356', 2]
];

var Powerup = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.type = type;
  this.size = POWERUP_SIZE;

  return this;
}
Powerup.prototype.render = function() {
  renderPath([
    ['moveTo', -POWERUP_SIZE, 6 - POWERUP_SIZE],
    ['lineTo', 6 - POWERUP_SIZE, -POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE - 6, -POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE, 6 - POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE, POWERUP_SIZE - 6],
    ['lineTo', POWERUP_SIZE - 6, POWERUP_SIZE],
    ['lineTo', 6 - POWERUP_SIZE, POWERUP_SIZE],
    ['lineTo', -POWERUP_SIZE, POWERUP_SIZE - 6],
  ], true, this.x - gCamera.x, this.y - gCamera.y);

  var strokeStyle, lineWidth, fillStyle, close;

  switch (this.type) {
    case 0:
    case 2:
      close = false;
      break;
    case 1:
    case 3:
      close = true;
      break;
  }
  fill(POWERUP_COLORS[this.type][0]);
  stroke('#222', 2);
  renderPath([
    ['moveTo', 5 - POWERUP_SIZE, 8 - POWERUP_SIZE],
    ['lineTo', 8 - POWERUP_SIZE, 5 - POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE - 8, 5 - POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE - 5, 8 - POWERUP_SIZE],
    ['lineTo', POWERUP_SIZE - 5, POWERUP_SIZE - 8],
    ['lineTo', POWERUP_SIZE - 8, POWERUP_SIZE - 5],
    ['lineTo', 8 - POWERUP_SIZE, POWERUP_SIZE - 5],
    ['lineTo', 5 - POWERUP_SIZE, POWERUP_SIZE - 8],
  ], true, this.x - gCamera.x, this.y - gCamera.y);
  stroke();

  renderPath(POWERUP_ICONS[this.type], close, this.x - gCamera.x, this.y - gCamera.y);
  fill(POWERUP_COLORS[this.type][1]);
  stroke(POWERUP_COLORS[this.type][2], POWERUP_COLORS[this.type][3]);
};
