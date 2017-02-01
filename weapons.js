Bolt = function(x, y, rad, enemy) {
  this.x = x + (PLAYER_RADIUS * Math.cos(rad));
  this.y = y + (PLAYER_RADIUS * Math.sin(rad));
  this.rad = rad;
  this.xVel = 28 * Math.cos(this.rad);
  this.yVel = 28 * Math.sin(this.rad);
  this.life = 4;
  this.dying = false;
  this.size = 14;
  this.enemy = enemy ? true : false;

  return this;
}
Bolt.delay = 140;
Bolt.prototype.update = function() {
  if (! this.dying) {
    this.x += this.xVel;
    this.y += this.yVel;
  }
}
Bolt.prototype.collide = function(other) {
  var rayToCenter = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2));
  var collide = rayToCenter < this.size;

  if (!collide) {
    var lastX = this.x - this.xVel;
    var lastY = this.y - this.yVel;
    var lastRay = Math.sqrt(Math.pow(lastX - this.x, 2) + Math.pow(lastY - this.y, 2));
    collide = Math.sqrt(Math.pow(rayToCenter, 2) + Math.pow(lastRay)) < this.size;
  }

  if (collide) {
    this.x = other.x;
    this.y = other.y;
    this.dying = true;
    if (other.life > 1 && this.life == 4) other.life--;
  }

  return collide;
}
Bolt.prototype.render = function() {
  var path = [];
  if (gCamera.isInView(this)) {
    switch (this.life) {
      case 4:
        path = [
          ['moveTo', 4 * Math.cos(this.rad), 4 * Math.sin(this.rad)],
          ['lineTo', 4 * Math.cos(this.rad + (Math.PI / 2)), 4 * Math.sin(this.rad + (Math.PI / 2))],
          ['lineTo', 8 * Math.cos(this.rad + Math.PI), 8 * Math.sin(this.rad + Math.PI)],
          ['lineTo', 4 * Math.cos(this.rad - (Math.PI / 2)), 4 * Math.sin(this.rad - (Math.PI / 2))],
        ];
        break;
      case 3:
      case 2:
      case 1:
        path = [
          ['moveTo', 3 * (4 - this.life), 0],
          ['arc', 3 * (4 - this.life), 0, 1 * this.life, 0, Math.PI * 2],
          ['moveTo', 0, 3 * (4 - this.life)],
          ['arc', 0, 3 * (4 - this.life), 1 * this.life, 0, Math.PI * 2],
          ['moveTo', -3 * (4 - this.life), 0],
          ['arc', -3 * (4 - this.life), 0, 1 * this.life, 0, Math.PI * 2],
          ['moveTo', 0, -3 * (4 - this.life)],
          ['arc', 0, -3 * (4 - this.life), 1 * this.life, 0, Math.PI * 2],
        ];
        break;
    }

    renderPath(path, true, this.x - gCamera.x, this.y - gCamera.y);
    stroke('#F88', 2);
    fill('#FFF');
  }
  if (this.dying) this.life--;
}
