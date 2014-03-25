var Shape = Base.extend({
    constructor: function()
    {}
});

var Circle = Shape.extend({
  constructor: function(radius) {
    this.extend({
      getCircumference: function() {
        return 2 * Math.PI * radius;
      }
    });
  }
});
