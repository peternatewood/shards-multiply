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
Bolt.prototype.render = (function(context) {
  function render() {
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
  return render;
})(gRenderer.context);
