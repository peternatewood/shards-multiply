Target = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.life = 11;
  this.size = 12;
  this.type = typeof type === 'string' ? type : 'circle';

  return this;
}
Target.prototype.render = function() {
  var x = this.x - gCamera.x;
  var y = this.y - gCamera.y;

  // Only render if within the camera's bounds
  if (gCamera.isInView(this)) {
    switch(this.type) {
      case 'circle':
        renderPath([['arc', x + 2, y + 2, 13, 0, Math.PI * 2]]);
        fill('#222');

        renderPath([['arc', x, y, this.size, 0, Math.PI * 2]]);
        fill('rgb(' + Math.round(255 * (this.life / this.size)) + ',' + Math.round(255 * ((this.size - this.life) / this.size)) + ',' + 0x22 + ')');
        break;
    }
  }
}

var SHARD_COLORS = [
  ['#37D', '#7AF'],
];

Shard = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.xVel = 0;
  this.yVel = 0;
  this.rad = 0;
  this.type = type;
  this.speed = Shard.speed(type);
  this.dying = false;

  return this;
}
Shard.speed = function(type) {
  switch (type) {
    case 0: return 3;
  }
}
Shard.prototype.collide = function(actor) {
  return Math.sqrt(Math.pow(actor.x - this.x, 2) + Math.pow(actor.y - this.y, 2)) < actor.size + 12;
}
Shard.prototype.update = function() {
  if (! this.dying) {
    var xAcc = 0;
    var yAcc = 0;
    this.xVel *= FRICTION;
    this.yVel *= FRICTION;
    switch (this.type) {
      case 0:
        this.rad = Math.atan2(gPlayer.y - this.y, gPlayer.x - this.x);
        xAcc = Math.cos(this.rad);
        yAcc = Math.sin(this.rad);
        break;
    }
    this.xVel += xAcc;
    this.yVel += yAcc;

    if (Math.abs(this.xVel) < 0.01) this.xVel = 0;
    if (Math.abs(this.yVel) < 0.01) this.yVel = 0;

    this.x = Math.min(Math.min(gCamera.x + SCREEN_WIDTH, gScene.bounds.r) - 20, Math.max(Math.max(gCamera.x, gScene.bounds.l) + 20, this.x + this.xVel * this.speed));
    this.y = Math.min(Math.min(gCamera.y + SCREEN_HEIGHT, gScene.bounds.b) - 20, Math.max(Math.max(gCamera.y, gScene.bounds.t) + 20, this.y + this.yVel * this.speed));
  }

  if (this.collide(gPlayer)) this.dying = true;
}
Shard.prototype.render = function() {
  var path = [];
  switch (this.type) {
    case 0:
      path = [
        ['moveTo', 16 * Math.cos(this.rad), 16 * Math.sin(this.rad)],
        ['lineTo', 5 * Math.cos(this.rad + (Math.PI / 2)), 5 * Math.sin(this.rad + (Math.PI / 2))],
        ['lineTo', 2 * Math.cos(this.rad + (Math.PI / 2)), 2 * Math.sin(this.rad + (Math.PI / 2))],
        ['lineTo', 2 * Math.cos(this.rad + (Math.PI / 2)), 2 * Math.sin(this.rad + (Math.PI / 2))],
        ['lineTo', 16 * Math.cos(this.rad + Math.PI), 16 * Math.sin(this.rad + Math.PI)],
        ['lineTo', 5 * Math.cos(this.rad - (Math.PI / 2)), 5 * Math.sin(this.rad - (Math.PI / 2))],
        ['lineTo', 2 * Math.cos(this.rad - (Math.PI / 2)), 2 * Math.sin(this.rad - (Math.PI / 2))],
      ];
      break;
  }
  renderPath(path, true, this.x - gCamera.x, this.y - gCamera.y);
  stroke(SHARD_COLORS[this.type][1], 3);
  fill(SHARD_COLORS[this.type][0]);
}
