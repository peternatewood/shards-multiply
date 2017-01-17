Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  return this;
}
Target.prototype.collide = function(other) {
  var collision = false;
  var rayToCenter = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2));
  var collide = rayToCenter < other.size;

  if (collide) {
    var rad = Math.atan2(other.y - this.y, other.x - this.x);
    collision = { x: this.x, y: this.y }
  }
  else if (other.xVel || other.yVel) {
    var lastX = other.x - other.xVel;
    var lastY = other.y - other.yVel;
    var otherRay = Math.sqrt(Math.pow(other.x - lastX, 2) + Math.pow(other.y - lastY, 2));
    collide = Math.sqrt(Math.pow(rayToCenter, 2) + Math.pow(otherRay)) < other.size;

    if (collide) {
      var rad = Math.atan2(lastY - this.y, lastX - this.x);
      collision = { x: this.x, y: this.y }
    }
  }

  if (this.life > 1 && collide && other.life == 4) this.life--;

  return collision;
}
Target.prototype.render = function(context) {
  context.beginPath();
  context.arc(this.x + 2, this.y + 2, 13, 0, Math.PI * 2);
  context.fillStyle = '#222';
  context.fill();
  context.closePath();

  context.beginPath();
  context.arc(this.x, this.y, 12, 0, Math.PI * 2);
  context.fillStyle = 'rgb(' + Math.round(255 * (this.life / 12)) + ',' + Math.round(255 * ((12 - this.life) / 12)) + ',' + 0x22 + ')';
  context.fill();
  context.closePath();
}
