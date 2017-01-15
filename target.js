Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  return this;
}
Target.prototype.collide = function(other) {
  var collide = Math.sqrt(Math.pow(other.x - this.x, 2) + Math.pow(other.y - this.y, 2)) < other.size;
  if (this.life > 1 && collide && other.life == 4) this.life--;
  if (collide) {
    var rad = Math.atan2(other.y - this.y, other.x - this.x);
    return { x: this.x + ((other.size + 1) * Math.cos(rad)), y: this.y + ((other.size + 1) * Math.sin(rad)) }
  }
  else return false;
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
