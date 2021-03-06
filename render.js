gRenderer = {
  frame: 0,
  render: function() {
    this.context.fillStyle = '#000';
    this.context.fillRect(0, 0, this.context.canvas.width, this.context.canvas.height);

    switch (gScene.scene) {
      case 'title':
        gScene.renderTitle();
        break;
      case 'hangar':
      case 'game':
        gScene.renderBack();
        if (gPlayer.life > 0) gPlayer.render();
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
    // this.context.font = '16px monospace';
    // this.context.textAlign = 'left';
    // this.context.fillStyle = '#FFF';
    // this.context.fillText('gCamera: ' + gCamera.x + ', ' + gCamera.y + ' vel: ' + gCamera.xVel + ', ' + gCamera.yVel, 12, 12);
    // this.context.fillText('gPlayer: ' + gPlayer.x + ', ' + gPlayer.y + ' vel: ' + gPlayer.xVel + ', ' + gPlayer.yVel, 12, 36);

    this.frame = (this.frame + 1) % 240;
  }
}
