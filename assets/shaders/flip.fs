#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D headsTexture;
uniform sampler2D tailsTexture;
uniform float flipAngle; // The exact same angle passed to the vertex shader

void main()
{
    vec2 uv = fragTexCoord;
    
    // If the cosine of the angle is positive, the front is facing the camera
    if (cos(flipAngle) > 0.0) 
    {
        finalColor = texture(headsTexture, uv);
    }
    else 
    {
        // The coin has flipped past 90 degrees, so we are looking at the back!
        // We mirror the Y-axis coordinate so the tails texture isn't drawn upside down
        uv.y = 1.0 - uv.y; 
        
        finalColor = texture(tailsTexture, uv);
    }
}