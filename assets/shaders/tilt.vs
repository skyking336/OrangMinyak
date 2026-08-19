#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;
uniform mat4 mvp;

uniform vec2 tilt;   // x = pitch (rotate around X axis), y = yaw (rotate around Y axis)
uniform vec2 origin; // Center of the quad in world space

out vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;

    // Shift vertex relative to its origin
    vec3 localPos = vertexPosition - vec3(origin, 0.0);

    float cx = cos(tilt.x);
    float sx = sin(tilt.x);
    float cy = cos(tilt.y);
    float sy = sin(tilt.y);
    
    mat4 rotX = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cx, -sx, 0.0,
        0.0, sx,  cx, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
    
    mat4 rotY = mat4(
        cy, 0.0, sy, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sy, 0.0, cy, 1.0,
        0.0, 0.0, 0.0, 1.0
    );

    vec4 rotatedPos = rotX * rotY * vec4(localPos, 1.0);

    // Manual pseudo-perspective
    // The Z axis moves the vertex towards or away from the camera.
    // We scale the X and Y based on Z to fake a perspective camera.
    float distanceToCamera = 1000.0; // Tweak this for more/less extreme perspective
    float perspective = distanceToCamera / (distanceToCamera - rotatedPos.z); //the z vector pointing us is positive
    rotatedPos.xy *= perspective;

    // Shift back to world space
    vec3 finalPos = rotatedPos.xyz + vec3(origin, 0.0);

    // Calculate final vertex position for OpenGL
    gl_Position = mvp * vec4(finalPos, 1.0);
    
    // FLATTEN Z: We are in a 2D Orthographic projection which has very tight clipping planes.
    // Since we only want the *illusion* of 3D via X/Y distortion, we force Z back to 0 
    // to prevent the GPU from clipping the parts of the card that rotated "too deep".
    gl_Position.z = 0.0;
}
