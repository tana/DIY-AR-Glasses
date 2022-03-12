// Fragment shader for lens distortion compensation

// The lens distorts a point on the display (x + x_center, y + y_center)
// into a point on the virtual screen (x' + x_center, y' + y_center) .
// The distortion is written as
//   x' = x (1 + k1 r^2)
//	 y' = y (1 + k1 r^2)
// where r = sqrt(x^2 + y^2) .
// Reference: https://docs.opencv.org/4.x/d9/d0c/group__calib3d.html

#version 100

precision mediump float;

// For meaning of Raylib's predefined variable names,
// see https://github.com/raysan5/raylib/wiki/raylib-generic-uber-shader-and-custom-shaders
// and https://github.com/raysan5/raylib/wiki/raylib-default-shader .

uniform sampler2D texture0;
uniform float k1;

const vec2 center = vec2(0.5, 0.5);

varying vec2 fragTexCoord;

void main()
{
  vec2 displayPoint = fragTexCoord - center;
  float rSquared = displayPoint.x * displayPoint.x + displayPoint.y * displayPoint.y;
  vec2 lookupPoint = displayPoint * vec2(1.0 + k1 * rSquared);
  
  vec2 coord = lookupPoint + center;
  if (0.0 <= coord.x && coord.x <= 1.0 && 0.0 <= coord.y && coord.y <= 1.0) {
    gl_FragColor = texture2D(texture0, coord);
  } else {  // Outside area is filled black, instead of clamping or repeating
    gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
  }
}
