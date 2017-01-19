Target = function(x, y) {
  this.x = x;
  this.y = y;
  this.life = 11;
  this.size = 12;

  return this;
}
Target.prototype.render = (function(context) {
  function render() {
    var x = this.x - gCamera.x;
    var y = this.y - gCamera.y;

    // Only render if within the camera's bounds
    if (gCamera.isInView(this)) {
      context.beginPath();
      context.arc(x + 2, y + 2, 13, 0, Math.PI * 2);
      context.fillStyle = '#222';
      context.fill();
      context.closePath();

      context.beginPath();
      context.arc(x, y, this.size, 0, Math.PI * 2);
      context.fillStyle = 'rgb(' + Math.round(255 * (this.life / this.size)) + ',' + Math.round(255 * ((this.size - this.life) / this.size)) + ',' + 0x22 + ')';
      context.fill();
      context.closePath();
    }
  }
  return render;
})(gRenderer.context);
