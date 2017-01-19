gRenderer = {
  frame: 0,
  canvas: document.getElementById('canvas'),
  context: this.canvas.getContext('2d'),
  render: function() {
    this.context.fillStyle = '#000';
    this.context.fillRect(0, 0, canvas.width, canvas.height);

    switch (gScene.scene) {
      case 'title':
        gScene.renderTitle();
        break;
      case 'hangar':
      case 'game':
        gScene.renderBack();
        gPlayer.render();
        gScene.renderFore();
        gPlayer.projectiles.forEach(function(p) { p.render() }.bind(this));
        if (gScene.inTransition) gScene.renderTransition();
        break;
      case 'levelTitle':
        gScene.renderLevelTitle();
        break;
    }

    if (!gScene.inTransition) gInput.render();
    // Debug display
    this.context.font = '16px monospace';
    this.context.textAlign = 'left';
    this.context.fillStyle = '#FFF';
    this.context.fillText('gCamera: ' + gCamera.x + ', ' + gCamera.y + ' vel: ' + gCamera.xVel + ', ' + gCamera.yVel, 12, 12);
    this.context.fillText('gPlayer: ' + gPlayer.x + ', ' + gPlayer.y + ' vel: ' + gPlayer.xVel + ', ' + gPlayer.yVel, 12, 36);

    this.frame = (this.frame + 1) % 60;
  }
}
