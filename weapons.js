var Bolt = function(x, y, rad) {
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
},
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
