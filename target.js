Target = function(x, y, type) {
  this.x = x;
  this.y = y;
  this.life = 11;
  this.size = 12;
  this.type = typeof type === 'string' ? type : 'circle';

  return this;
}
Target.prototype.render = function() {
  var x = this.x - gCamera.x;
  var y = this.y - gCamera.y;

  // Only render if within the camera's bounds
  if (gCamera.isInView(this)) {
    switch(this.type) {
      case 'circle':
        renderPath([['arc', x + 2, y + 2, 13, 0, Math.PI * 2]]);
        fill('#222');

        renderPath([['arc', x, y, this.size, 0, Math.PI * 2]]);
        fill('rgb(' + Math.round(255 * (this.life / this.size)) + ',' + Math.round(255 * ((this.size - this.life) / this.size)) + ',' + 0x22 + ')');
        break;
    }
  }
}
