function update() {
  gScene.update();
}

function step(timestamp) {
  gRenderer.render();
  // Only request next frame if user is still in this tab
  if (!start) var start = timestamp;
  var progress = timestamp - start;
  if (progress < 2000) window.requestAnimationFrame(step);
}

function init() {
  gRenderer.canvas = document.getElementById('canvas');
  gRenderer.context = gRenderer.canvas.getContext('2d');
  SCREEN_WIDTH = gRenderer.canvas.width;
  SCREEN_HEIGHT = gRenderer.canvas.height;

  gRenderer.context.font = '32px courier';
  gPlayer.x = (gRenderer.canvas.width / 2) - PLAYER_RADIUS;
  gPlayer.y = (gRenderer.canvas.height * 0.75) - PLAYER_RADIUS;

  // Key listeners only seem to work on the document
  document.addEventListener('keydown', handleKeyDown);
  document.addEventListener('keyup', handleKeyUp);
  // Set mouse listeners on the canvas
  gRenderer.canvas.addEventListener('mousemove', handleMouseMove);
  gRenderer.canvas.addEventListener('mousedown', handleMouseDown);
  gRenderer.canvas.addEventListener('mouseup', handleMouseUp);
  gRenderer.canvas.addEventListener('contextmenu', function(event) { event.preventDefault(); event.stopPropagation() });

  setInterval(update, 25);
  window.requestAnimationFrame(step);
}

init();
