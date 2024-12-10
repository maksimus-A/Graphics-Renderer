#version 330 core
out vec4 fragColor;

in vec2 fragCoord;  // Interpolated UV coordinates from the vertex shader
uniform vec2 iResolution; // Resolution of the viewport
uniform float iTime;

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );

}


/* // Old color palette
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(-0.647, 0.248, 0.895);
    vec3 c = vec3(-3.142, 3.078, 1.460);
    vec3 d = vec3(-1.972, 0.032, 0.365);

    return a + b*cos( 6.28318*(c*t+d) );
    */

void main() {
    // Normalize the coordinates to [-1, 1] range
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);

    
    for (float i = 0.0; i < 2.0; i++) {

        uv = fract(uv * 1.5) - 0.5;

        // Calculate the distance from the current fragment to the line y = x
        float d = abs(uv.y - uv.x) / sqrt(2.0);
        // Calculate the distance from the current fragment to the line y = -x
        float d2 = abs(uv.x + uv.y) / sqrt(2.0);
        // Calculate the distance from the current fragment to the line y = 0
        float d3 = abs(uv.x) / sqrt(2.0);
        // Calculate the distance from the current fragment to the line x = 0
        float d4 = abs(uv.y) / sqrt(2.0);

        float lines[4] = float[4](d, d2, d3, d4);

        vec3 col = palette(length(uv0) + i*.4 + iTime*.4); // 
        float colorVal;
        for (int i = 0; i < 4; i++) {
            // d calcs
            if (i % 2 == 0) {
                lines[i] = sin(lines[i]*8. + iTime)/8.;
            }
            else {
                lines[i] = -sin(lines[i]*8. + iTime)/8.;
            }
            lines[i] = abs(lines[i]);

            lines[i] = pow(0.01 / lines[i], 1.2);

            // calculate final color value
            colorVal += lines[i];
        }
        col *= colorVal;
        //d = smoothstep(0.0, 0.1, d);

        finalColor += col * d;
    }
        
    

    fragColor = vec4(finalColor, 1.0);
}
/*
    // My augmented code for a different effect
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 6.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + iTime*.4);

        d = sin((d*8. + iTime) * cos(0.2*i))/8.;
        d = abs(d);

        d = pow(0.006 / d, 1.2);

        finalColor += col * d;
    }
        
    fragColor = vec4(finalColor, 1.0);
    */
/*
// Original code
void main() {
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 5.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + iTime*.4);

        d = sin(d*8. + iTime)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    fragColor = vec4(finalColor, 1.0);
} */
