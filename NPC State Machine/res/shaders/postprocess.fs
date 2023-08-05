#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;

out vec4 finalColor;

uniform float gameWidth;
uniform float gameHeight;

uniform int renderWidth;
uniform int renderHeight;

uniform float resolution;

uniform float time;

vec2 CRTCurveUV( vec2 uv, float str )
{
    uv = uv * 2.0 - 1.0;
    vec2 offset = ( str * abs( uv.yx ) ) / vec2( 6.0, 4.0 );
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void main()
{
    vec2 baseUV = fragTexCoord.xy;
    vec2 uv = CRTCurveUV( baseUV, 1.0 );
    
    vec3 color = texture( texture0, uv ).rgb;
    color *= colDiffuse.rgb;
    
    vec2 rgbUV = uv;
    float aspectRatio = gameWidth / gameHeight;
    float res = resolution * 1.0;
    rgbUV.x *= gameWidth * res;
    rgbUV.y *= gameHeight * res;
    rgbUV.x *= aspectRatio;
    vec3 rgbColor = color * texture(texture1, rgbUV).rgb * 4.0;
    color = mix(color, rgbColor, 0.2);

    uv = CRTCurveUV( baseUV, 1.0 );
    if ( uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0 )
    {
        color = vec3( 0.0, 0.0, 0.0 );
    }    
    float vignette = uv.x * uv.y * ( 1.0 - uv.x ) * ( 1.0 - uv.y );
    vignette = clamp( pow( 16.0 * vignette, 0.3 ), 0.0, 1.0 );
    color *= vignette * 1.1;

    float scanline 	= clamp( 0.95 + 0.05 * cos( 3.14 * ( uv.y + 0.008 * time ) * 240.0 * 1.0 ), 0.0, 1.0 );
    float grille 	= 0.85 + 0.15 * clamp( 1.5 * cos( 3.14 * uv.x * 640.0 * 1.0 ), 0.0, 1.0 );    
    color *= scanline * grille * 1.0;
    
	finalColor = vec4(color, 1.0);
    //finalColor = texture( texture0, fragTexCoord ) * colDiffuse;
}
