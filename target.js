Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  return this;
}
Target.prototype.render = (function(context) {
  function render() {
    var x = this.x - gCamera.x;
    var y = this.y - gCamera.y;

    // Only render if within the camera's bounds
    if (x + 12 > 0 && x - 12 < SCREEN_WIDTH && y + 12 > 0 && y - 12 < SCREEN_HEIGHT) {
      context.beginPath();
      context.arc(x + 2, y + 2, 13, 0, Math.PI * 2);
      context.fillStyle = '#222';
      context.fill();
      context.closePath();

      context.beginPath();
      context.arc(x, y, 12, 0, Math.PI * 2);
      context.fillStyle = 'rgb(' + Math.round(255 * (this.life / 12)) + ',' + Math.round(255 * ((12 - this.life) / 12)) + ',' + 0x22 + ')';
      context.fill();
      context.closePath();
    }
  }
  return render;
})(gRenderer.context);
