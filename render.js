gRenderer = {
  frame: 0,
  render: function() {
    this.context.fillStyle = '#000';
    this.context.fillRect(0, 0, canvas.width, canvas.height);

    switch (gScene.scene) {
      case 'title':
        gScene.renderTitle(this.context);
        break;
      case 'hangar':
      case 'game':
        gScene.renderBack(this.context);
        gPlayer.render(this.context);
        gScene.renderFore(this.context);
        gPlayer.projectiles.forEach(function(p) { p.render(this.context) }.bind(this));
        if (gScene.inTransition) gScene.renderTransition(this.context);
        break;
      case 'levelTitle':
        gScene.renderLevelTitle(this.context);
        break;
    }

    if (!gScene.inTransition) gInput.render(this.context);
    // Debug display
    // this.context.font = '16px monospace';
    // this.context.fillStyle = '#FFF';
    // this.context.fillText('Mouse: ' + gInput.mouseX + ', ' + gInput.mouseY + ' Player rad: ' + gPlayer.rad, 12, 12);
    this.frame = (this.frame + 1) % 60;
  }
}
