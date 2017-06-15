uniform float GTime;
uniform float Time;

in vec3 DrawNormal;
in vec3 DrawPosition;

void main( void )
{
  /*
  vec3 p = DrawPosition;
  float r = p.x * p.x + p.y * p.y + p.z * p.z;
  if (r > 114 + 100 * abs(sin(GTime)))
    discard;
  if (gl_FragCoord.x > 100 + 400 * sin(GTime))
    discard;
  if (DrawPosition.y > 5)
    discard;
  */
  vec3 L = normalize(vec3(1 + 15 * sin(8 * Time), 5, 3));
  float nl = dot(DrawNormal, L);
  vec3 color = vec3(0, 0, 0);
  if (nl > 0)
    color = nl * vec3(1, 1, 1);
  
  float t = sin(DrawPosition.x + Time);
  if (t > 0.8)
    color += pow((t - 0.8) / 0.2, 30) * vec3(0, 1, 0);
  t = sin(DrawPosition.y + Time);
  if (t > 0.8)
    color += pow((t - 0.8) / 0.2, 30) * vec3(0, 1, 0);
  t = sin(DrawPosition.z + Time);
  if (t > 0.8)
    color += pow((t - 0.8) / 0.2, 30) * vec3(0, 1, 0);


  gl_FragColor = vec4(color + DrawNormal / 2, 1);
}