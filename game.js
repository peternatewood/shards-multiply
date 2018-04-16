function step(timestamp) {
  gScene.update();
  gRenderer.render();
  // Only request next frame if user is still in this tab
  if (!start) var start = timestamp;
  var progress = timestamp - start;
  if (progress < 2000) window.requestAnimationFrame(step);
}

function init() {
  var canvas = document.getElementById('canvas');
  gRenderer.context = canvas.getContext('2d');
  SCREEN_WIDTH = canvas.width;
  SCREEN_HEIGHT = canvas.height;

  gPlayer.x = gScene.bounds.l + ((gScene.bounds.r - gScene.bounds.l) / 2);
  gPlayer.y = gScene.bounds.b - 135;

  var xOff = (canvas.width / 2) - ((LEVELS[0].r - LEVELS[0].l) / 2);
  var yOff = (canvas.height / 2) - ((LEVELS[0].b - LEVELS[0].t) / 2);
  LEVELS[0].l += xOff;
  LEVELS[0].t += yOff;
  LEVELS[0].r += xOff;
  LEVELS[0].b += yOff;
  gScene.setBounds();

  // Key listeners only seem to work on the document
  document.addEventListener('keydown', handleKeyDown);
  document.addEventListener('keyup', handleKeyUp);
  // Set mouse listeners on the canvas
  canvas.addEventListener('mousemove', handleMouseMove);
  canvas.addEventListener('mousedown', handleMouseDown);
  canvas.addEventListener('mouseup', handleMouseUp);
  canvas.addEventListener('mouseleave', handleMouseLeave);
  canvas.addEventListener('contextmenu', function(event) { event.preventDefault(); event.stopPropagation() });

  window.requestAnimationFrame(step);
}

init();
