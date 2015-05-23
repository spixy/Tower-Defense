#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

/* Made by Krisztián Szabó, edited by Spixy */
void main(){

	vec2 light_pos = mouse;
  
	float radius = 800.0;
  
	float intensity = 0.01;
  
	float dist = distance(gl_FragCoord.xy, light_pos);
	
	vec3 light_color = vec3(1, 0, 0);

	float alpha = 0.1 / (dist*intensity);
	
	vec4 final_color = vec4(light_color, 1.0)*vec4(alpha, alpha, alpha, 1.0);
	
	gl_FragColor = final_color;
	
	/* If you want to apply radius to the effect comment out the gl_FragColor line and remove comments from below: */
	
	/*if(dist <= radius)
  {
		gl_FragColor = final_color;
	}*/
	
}