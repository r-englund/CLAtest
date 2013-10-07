uniform sampler2D inport_;
uniform float alpha_;
uniform vec2 dimension_;

void main() {
    vec2 texCoordsM = vec2(gl_FragCoord.x, gl_FragCoord.y) * dimension_;
    vec3 colorM		= texture2D(inport_, texCoordsM).rgb;
        
    vec2 texCoordsR		= vec2(gl_FragCoord.x + 1.0	, gl_FragCoord.y)		* dimension_;
	vec2 texCoordsL		= vec2(gl_FragCoord.x - 1.0	, gl_FragCoord.y)		* dimension_;
    vec2 texCoordsU		= vec2(gl_FragCoord.x		, gl_FragCoord.y + 1.0)	* dimension_;
    vec2 texCoordsD		= vec2(gl_FragCoord.x		, gl_FragCoord.y - 1.0)	* dimension_;
    vec2 texCoordsUL	= vec2(gl_FragCoord.x + 1.0, gl_FragCoord.y - 1.0)	* dimension_;
	vec2 texCoordsUR	= vec2(gl_FragCoord.x + 1.0, gl_FragCoord.y + 1.0)	* dimension_;
    vec2 texCoordsDL	= vec2(gl_FragCoord.x - 1.0, gl_FragCoord.y - 1.0)	* dimension_;
    vec2 texCoordsDR	= vec2(gl_FragCoord.x - 1.0, gl_FragCoord.y + 1.0)	* dimension_;

    vec3 colorR  = texture2D(inport_, texCoordsR).rgb;
    vec3 colorL  = texture2D(inport_, texCoordsL).rgb;
    vec3 colorU  = texture2D(inport_, texCoordsU).rgb;
    vec3 colorD  = texture2D(inport_, texCoordsD).rgb;
    vec3 colorUL = texture2D(inport_, texCoordsUL).rgb;
    vec3 colorUR = texture2D(inport_, texCoordsUR).rgb;
    vec3 colorDL = texture2D(inport_, texCoordsDL).rgb;
    vec3 colorDR = texture2D(inport_, texCoordsDR).rgb;

	float valR	 = (colorR.r + colorR.g + colorR.b)/3.0;
	float valL	 = (colorL.r + colorL.g + colorL.b)/3.0;
	float valU	 = (colorU.r + colorU.g + colorU.b)/3.0;
	float valD	 = (colorD.r + colorD.g + colorD.b)/3.0;
	float valUL	 = (colorUL.r + colorUL.g + colorUL.b)/3.0;
	float valUR	 = (colorUR.r + colorUR.g + colorUR.b)/3.0;
	float valDL	 = (colorDL.r + colorDL.g + colorDL.b)/3.0;
	float valDR	 = (colorDR.r + colorDR.g + colorDR.b)/3.0;

    float valX	= valUR + 2.0*valR + valDR - valUL - 2.0*valL - valUR; 
    float valY	= valUL + 2.0*valU + valUR - valDL - 2.0*valD - valDR;

	float val	= length(vec2(valX, valY));
    
    vec4 color = vec4(vec3(val) * alpha_ + colorM * (1 - alpha_), 1.0);
        
    FragData0 = color;
}