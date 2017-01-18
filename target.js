Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  return this;
}
Target.prototype.render = (function(context) {
  function render() {
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
  return render;
})(gRenderer.context);
