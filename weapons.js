Bolt = function(x, y, rad) {
  this.x = x + (PLAYER_RADIUS * Math.cos(rad));
  this.y = y + (PLAYER_RADIUS * Math.sin(rad));
  this.rad = rad;
  this.xVel = 28 * Math.cos(this.rad);
  this.yVel = 28 * Math.sin(this.rad);
  this.life = 4;
  this.dying = false;
  this.size = 14;

  return this;
}
Bolt.delay = 140;
Bolt.prototype.update = function(collision) {
  if (collision) {
    this.dying = true;
    if (collision instanceof Object) {
      this.x = collision.x;
      this.y = collision.y;
    }
  }

  if (! this.dying) {
    this.x += this.xVel;
    this.y += this.yVel;
  }
}
Bolt.prototype.collide = function(other) {
  var collision = false;
  var rayToCenter = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2));
  var collide = rayToCenter < this.size;

  if (collide) {
    var rad = Math.atan2(other.y - this.y, other.x - this.x);
    collision = { x: this.x, y: this.y }
  }
  else if (other.xVel || other.yVel) {
    var lastX = other.x - other.xVel;
    var lastY = other.y - other.yVel;
    var otherRay = Math.sqrt(Math.pow(other.x - lastX, 2) + Math.pow(other.y - lastY, 2));
    collide = Math.sqrt(Math.pow(rayToCenter, 2) + Math.pow(otherRay)) < this.size;

    if (collide) {
      var rad = Math.atan2(lastY - this.y, lastX - this.x);
      collision = { x: this.x, y: this.y }
    }
  }

  if (other.life > 1 && collide && this.life == 4) other.life--;

  return collision;
}
Bolt.prototype.render = function(context) {
  context.beginPath();
  switch (this.life) {
    case 4:
      context.moveTo(this.x + (4 * Math.cos(this.rad)), this.y + (4 * Math.sin(this.rad)));
      context.lineTo(this.x + (4 * Math.cos(this.rad + (Math.PI / 2))), this.y + (4 * Math.sin(this.rad + (Math.PI / 2))));
      context.lineTo(this.x + (8 * Math.cos(this.rad + Math.PI)), this.y + (8 * Math.sin(this.rad + Math.PI)));
      context.lineTo(this.x + (4 * Math.cos(this.rad - (Math.PI / 2))), this.y + (4 * Math.sin(this.rad - (Math.PI / 2))));
      break;
    case 3:
    case 2:
    case 1:
      context.moveTo(this.x + (3 * (4 - this.life)), this.y);
      context.arc(this.x + (3 * (4 - this.life)), this.y, 1 * this.life, 0, Math.PI * 2);
      context.moveTo(this.x, this.y - (3 * (4 - this.life)));
      context.arc(this.x, this.y - (3 * (4 - this.life)), 1 * this.life, 0, Math.PI * 2);
      context.moveTo(this.x - (3 * (4 - this.life)), this.y);
      context.arc(this.x - (3 * (4 - this.life)), this.y, 1 * this.life, 0, Math.PI * 2);
      context.moveTo(this.x, this.y + (3 * (4 - this.life)));
      context.arc(this.x, this.y + (3 * (4 - this.life)), 1 * this.life, 0, Math.PI * 2);
      break;
  }
  context.closePath();

  context.fillStyle = '#FFF';
  context.strokeStyle = '#F88';
  context.lineWidth = 2;

  context.stroke();
  context.fill();
  if (this.dying) this.life--;
}
