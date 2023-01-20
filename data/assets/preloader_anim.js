var alphaEffect = false;
var alphaStart = 1;
var scaleFactor = 2.8 // by volume with 8 circs at 16px rad
var initialScale = 0.6;
var finalScale = initialScale*scaleFactor;
var offset = 1/10;
var travel = 50;

if(alphaEffect) {
  alphaStart = 0;
}

TweenMax.set('svg', {
  visibility: 'visible'
});

TweenMax.set('.ellipse', { transformOrigin: "50% 50%" });

var tl = new TimelineMax({ repeat: -1 });

  tl.fromTo('.ellipses', 3.5, { x: -(travel) }, { x: travel, ease: Linear.easeNone })
    .staggerFromTo('.ellipse', 2.5, { x: -350, autoAlpha: alphaStart }, { x: 350, autoAlpha: 1, ease: SlowMo.ease.config(0.25, 1.0, false), transformOrigin: "50% 50%"}, offset, "-=3.4")
    .staggerFromTo('.ellipse', 1.75, { scale: initialScale }, { scale: finalScale, ease: SlowMo.ease.config(0, 1, true), yoyo: true}, offset, "-=3.0")

tl.eventCallback("onUpdate", function() {
});
