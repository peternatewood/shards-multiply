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

var SHARD_LIFE = 14;
var SHARD_COLORS = [
  ['#3377DD', '#77AAFF'],
  ['#DD3355', '#FF7799']
];

Shard = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.xVel = 0;
  this.yVel = 0;
  this.rad = 0;
  this.type = type;
  this.speed = Shard.speed(type);
  this.life = SHARD_LIFE;
  this.size = 20;
  this.projectiles = [];
  this.timeToFire = 0;

  return this;
}
Shard.fireDelay = 50;
Shard.speed = function(type) {
  switch (type) {
    case 0: return 3;
    case 1: return 2;
  }
}
Shard.prototype.collide = function(actor) {
  return Math.sqrt(Math.pow(actor.x - this.x, 2) + Math.pow(actor.y - this.y, 2)) < actor.size + 12;
}
Shard.prototype.update = function() {
  if (this.life < SHARD_LIFE) this.life--;
  else {
    var xAcc = 0;
    var yAcc = 0;
    this.xVel *= FRICTION;
    this.yVel *= FRICTION;
    this.rad = Math.atan2(gPlayer.y - this.y, gPlayer.x - this.x);
    switch (this.type) {
      case 0:
        xAcc = Math.cos(this.rad);
        yAcc = Math.sin(this.rad);
        break;
      case 1:
        if (Math.sqrt(Math.pow(gPlayer.x - this.x, 2) + Math.pow(gPlayer.y - this.y, 2)) > gPlayer.size * 10) {
          xAcc = Math.cos(this.rad);
          yAcc = Math.sin(this.rad);
        }
        if (gPlayer.life == PLAYER_LIFE) {
          if (this.timeToFire == 0) {
            this.projectiles.push(new Bolt(this, 1));
            this.timeToFire = Shard.fireDelay;
          }
          this.timeToFire--;
        }
        break;
    }
    this.xVel += xAcc;
    this.yVel += yAcc;

    if (Math.abs(this.xVel) < 0.01) this.xVel = 0;
    if (Math.abs(this.yVel) < 0.01) this.yVel = 0;

    this.x = Math.min(Math.min(gCamera.x + SCREEN_WIDTH, gScene.bounds.r) - 20, Math.max(Math.max(gCamera.x, gScene.bounds.l) + 20, this.x + this.xVel * this.speed));
    this.y = Math.min(Math.min(gCamera.y + SCREEN_HEIGHT, gScene.bounds.b) - 20, Math.max(Math.max(gCamera.y, gScene.bounds.t) + 20, this.y + this.yVel * this.speed));

    if (this.life == SHARD_LIFE && gPlayer.life == PLAYER_LIFE && this.collide(gPlayer)) {
      gPlayer.life--;
      this.life--;
    }
  }

  var liveProjectiles = [];
  this.projectiles.forEach(function(p) {
    var collision = false;
    if (p.collide(gPlayer) && gPlayer.life == PLAYER_LIFE) gPlayer.life--;
    else {
      for (var i in gScene.targets) {
        collision = p.collide(gScene.targets[i]);
        if (collision) break;
      }
    }
    if (!collision && gScene.bounds.collide(p)) p.dying = true;
    p.update();
    if (p.life > 0) liveProjectiles.push(p);
  });
  this.projectiles = liveProjectiles;
}
Shard.prototype.render = function() {
  this.projectiles.forEach(function(p) { p.render() });
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
    case 1:
      path = [
        ['moveTo', 14 * Math.cos(this.rad), 14 * Math.sin(this.rad)],
        ['lineTo', 7 * Math.cos(this.rad + (Math.PI * 2 / 3)), 7 * Math.sin(this.rad + (Math.PI * 2 / 3))],
        ['lineTo', 14 * Math.cos(this.rad + Math.PI), 14 * Math.sin(this.rad + Math.PI)],
        ['lineTo', 7 * Math.cos(this.rad - (Math.PI * 2 / 3)), 7 * Math.sin(this.rad - (Math.PI * 2 / 3))],
      ];
      break;
  }

  var opacity = decToHex(255 * (this.life / SHARD_LIFE));
  renderPath(path, true, this.x - gCamera.x, this.y - gCamera.y);
  stroke(SHARD_COLORS[this.type][1] + opacity, 3);
  fill(SHARD_COLORS[this.type][0] + opacity);

  if (this.life < SHARD_LIFE) {
    var radius = Math.log10(Math.max(1, 10 * (1 - (this.life / SHARD_LIFE)))) * SHARD_LIFE;
    renderPath([
      ['moveTo', 0, radius],
      ['arc', 0, 0, radius, 0, 2 * Math.PI],
      ['moveTo', 3, 7 + (2 * radius / 3)],
      ['arc', 3, 7, 2 * radius / 3, 0, 2 * Math.PI],
      ['moveTo', -12, -3 + (2 * radius / 3)],
      ['arc', -12, -3, 2 * radius / 3, 0, 2 * Math.PI],
      ['moveTo', 10, -6 + (2 * radius / 3)],
      ['arc', 10, -6, 2 * radius / 3, 0, 2 * Math.PI],
    ], true, this.x - gCamera.x, this.y - gCamera.y);
    fill(this.life < SHARD_LIFE / 2 ? '#CCCCCC' + decToHex(255 * (this.life / (SHARD_LIFE / 2))) : '#EEE');
  }
}
