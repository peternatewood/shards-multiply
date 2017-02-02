Bolt = function(source, type) {
  this.x = source.x + (PLAYER_RADIUS * Math.cos(source.rad));
  this.y = source.y + (PLAYER_RADIUS * Math.sin(source.rad));
  this.rad = source.rad;
  this.type = typeof type === 'number' ? type : 0;
  this.xVel = (Bolt.speed[this.type] * Math.cos(source.rad)) + source.xVel;
  this.yVel = (Bolt.speed[this.type] * Math.sin(source.rad)) + source.yVel;
  this.life = 4;
  this.dying = false;
  this.size = 14;

  return this;
}
Bolt.delay = 140;
Bolt.speed = [28, 9];
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
  var strokeStyle = '#8F8';
  var fillStyle = '#FFF';
  if (gCamera.isInView(this)) {
    if (this.life == 4) {
      switch (this.type) {
        case 0:
          path = [
            ['moveTo', 4 * Math.cos(this.rad), 4 * Math.sin(this.rad)],
            ['lineTo', 4 * Math.cos(this.rad + (Math.PI / 2)), 4 * Math.sin(this.rad + (Math.PI / 2))],
            ['lineTo', 8 * Math.cos(this.rad + Math.PI), 8 * Math.sin(this.rad + Math.PI)],
            ['lineTo', 4 * Math.cos(this.rad - (Math.PI / 2)), 4 * Math.sin(this.rad - (Math.PI / 2))],
          ];
          break;
        case 1:
          path = [
            ['moveTo', 8 * Math.cos(this.rad), 8 * Math.sin(this.rad)],
            ['lineTo', 4 * Math.cos(this.rad + (Math.PI / 2)), 4 * Math.sin(this.rad + (Math.PI / 2))],
            ['lineTo', 4 * Math.cos(this.rad + Math.PI), 4 * Math.sin(this.rad + Math.PI)],
            ['lineTo', 4 * Math.cos(this.rad - (Math.PI / 2)), 4 * Math.sin(this.rad - (Math.PI / 2))],
          ];
          fillStyle = '#D66';
          strokeStyle = '#D88';
          break;
      }
    }
    else {
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
    }

    renderPath(path, true, this.x - gCamera.x, this.y - gCamera.y);
    fill(fillStyle);
    stroke(strokeStyle, 2);
  }
  if (this.dying) this.life--;
}
