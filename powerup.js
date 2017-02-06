var Powerup = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.type = type;
  this.size = POWERUP_SIZE;
  this.frame = 0;

  return this;
}
Powerup.lifetime = 18;
Powerup.types = ['bolt', 'clone', 'missile', 'beam', 'shield', 'armor'];
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
  return this.frame == 0 && gPlayer.x >= this.x - POWERUP_SIZE && gPlayer.y >= this.y - POWERUP_SIZE && gPlayer.x <= this.x + POWERUP_SIZE && gPlayer.y <= this.y + POWERUP_SIZE;
}
Powerup.prototype.getType = function() {
  return Powerup.types[this.type];
}
Powerup.prototype.render = function() {
  var strokeStyle, lineWidth, fillStyle;
  var opacity = this.frame > 0 && this.frame < Powerup.lifetime ? decToHex(255 * ((Powerup.lifetime - this.frame) / Powerup.lifetime)) : 'FF';

  renderPath(Powerup.outerPath, true, this.x - gCamera.x, this.y - gCamera.y);
  fill(POWERUP_COLORS[this.type][0] + opacity);
  stroke('#222222' + opacity, 2);
  renderPath(Powerup.innerPath, true, this.x - gCamera.x, this.y - gCamera.y);
  stroke();

  if (this.frame === 0) {
    renderPath(POWERUP_ICONS[this.type], true, this.x - gCamera.x, this.y - gCamera.y);
    fill(POWERUP_COLORS[this.type][1]);
    stroke(POWERUP_COLORS[this.type][2], POWERUP_COLORS[this.type][3]);
  }
  else this.frame++;
};
