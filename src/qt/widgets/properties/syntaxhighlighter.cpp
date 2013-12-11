/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/syntaxhighlighter.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/util/filedirectory.h>

#include <QTextDocument>
#include <QTextBlock>



namespace {
	enum GLSLSyntaxThins{//rename
		None = 0,
		Type,
		Comment,
		Qualifiers,
		BuiltinVars,
		BuiltinFunc,
		Preprocessor,
		String,
		SIZE_OF_GLSL_SYNTAX_ENUM
	};
}

static const char* glsl_types[] = {"float","vec2","vec3","vec4",
									"int","ivec2","ivec3","ivec4",
									"bool","bvec2","bvec3","bvec4",
									"mat2","mat3","mat4",
									"void",
									"sampler1D","sampler2D","sampler3D",
									"samplerCube",
									"sampler1DShadow","sampler2DShadow",0};

static const char* glsl_qualifiers[] = {"struct","uniform","attribute","varying","const","in","out","inout","const",0};
static const char* glsl_builtins_var[] = {"gl_ModelViewMatrix","gl_ModelViewProjectionMatrix","gl_ProjectionMatrix","gl_TextureMatrix",
	"gl_ModelViewMatrixInverse","gl_ModelViewProjectionMatrixInverse","gl_ProjectionMatrixInverse","gl_TextureMatrixInverse",
	"gl_ModelViewMatrixTranspose","gl_ModelViewProjectionMatrixTranspose","gl_ProjectionMatrixTranspose","gl_TextureMatrixTranspose",
	"gl_ModelViewMatrixInverseTranspose","gl_ModelViewProjectionMatrixInverseTranspose","gl_ProjectionMatrixInverseTranspose","gl_TextureMatrixInverseTranspose"
	"gl_NormalMatrix","gl_NormalScale","gl_DepthRangeParameters","gl_DepthRangeParameters","gl_DepthRange"
	"gl_FogParameters","gl_Fog","gl_LightSourceParameters","gl_LightSource",
	"gl_LightModelParameters","gl_LightModel","gl_LightModelProducts","gl_FrontLightModelProduct","gl_BackLightModelProduct"
	"gl_LightProducts"," gl_FrontLightProduct"," gl_BackLightProduct","gl_MaterialParameters","gl_FrontMaterial"
	"gl_BackMaterial","gl_PointParameters","gl_Point","gl_TextureEnvColor",
	"gl_ClipPlane","gl_EyePlaneS","gl_EyePlaneT",
	"gl_EyePlaneR","gl_EyePlaneQ","gl_ObjectPlaneS","gl_ObjectPlaneT","gl_ObjectPlaneR","gl_ObjectPlaneQ",
	"gl_Position","gl_PointSize","gl_ClipVertex",
	"gl_Vertex","gl_Normal","gl_Color",
	"gl_SecondaryColor","gl_MultiTexCoord0","gl_MultiTexCoord1",
	"gl_MultiTexCoord2","gl_MultiTexCoord3","gl_MultiTexCoord4",
	"gl_MultiTexCoord5","gl_MultiTexCoord6","gl_MultiTexCoord7",
	"gl_FogCoord","gl_FrontColor","gl_BackColor",
	"gl_FrontSecondaryColor","gl_BackSecondaryColor","gl_TexCoord",
	"gl_FogFragCoord","gl_FragData","gl_FragDepth",
	"gl_FragColor","gl_FragCoord","gl_FrontFacing",
	"gl_MaxVertexUniformComponents","gl_MaxFragmentUniformComponents","gl_MaxVertexAttribs",
	"gl_MaxVaryingFloats","gl_MaxDrawBuffers ","gl_MaxTextureCoords",
	"gl_MaxTextureUnits","gl_MaxTextureImageUnits","gl_MaxVertexTextureImageUnits",
	"gl_MaxCombinedTextureImageUnits","gl_MaxLights","gl_MaxClipPlanes",
	0};
static const char* glsl_builtins_func[] = {	"sin","cos","tab","asin",
	"acos","atan","radians","degrees",
	"pow","exp","log","exp2",
	"log2","sqrt","inversesqrt",
	"abs","ceil","clamp","floor",
	"fract","max","min","mix",
	"mod","sign","smoothstep","step",
	"matrixCompMult","aa","aa",
	"ftransform","cross","distance","dot",
	"faceforward","length","normalize",
	"reflect","refract","dFdx","dFdy",
	"fwidth","all","any","equal",
	"greaterThan","greaterThanEqual",
	"lessThan","lessThanEqual","not",
	"notEqual","texture1D","texture1DProj",
	"texture1DProj","texture2D","texture2DProj",
	"texture2DProj","texture3D","texture3DProj",
	"textureCube","shadow1D","shadow2D",
	"shadow1DProj","shadow2DProj","texture1DLod",
	"texture1DProjLod","texture1DProjLod",
	"texture2DLod","texture2DProjLod",
	"texture2DProjLod","texture3DProjLod",
	"textureCubeLod","shadow1DLod","shadow2DLod",
	"shadow1DProjLod","shadow2DProjLod",
	"noise1","noise2","noise3","noise4",
	0};
//static const char* glsl_preprocessor[] = {"#","#define","#include","#if","#ifdef","#ifdef","#else","#elif","#endif","#error","#pragma","#line","__LINE__","__FILE__","__VERSION__",0};


namespace inviwo {

SyntaxHighligther::SyntaxHighligther(QTextDocument* parent,Syntax syntax):QSyntaxHighlighter(parent),syntax_(syntax){

}

void SyntaxHighligther::setSyntax(Syntax syntax){
	syntax_ = syntax;
	loadConfig();
}

void SyntaxHighligther::highlightBlock(const QString& text){
	setFormat(0,text.size() , formats_[None]);

	int index,prevIndex,startIndex = 0;
	int i;

	if(previousBlockState()!=0 && previousBlockState()!=-1){//we are in comment
		index = text.indexOf("*/");
		if(index!=-1){
			startIndex = index +2;
			setFormat(0,index+2,formats_[Comment]);
		}else{ //whole line is comment
			setFormat(0,text.size() , formats_[Comment]);
			setCurrentBlockState(1);
			return;
		}
	}


	i = -1;
	const char **arr = glsl_types;
	while(arr[++i]){
		int count = text.count(arr[i]);
		index = 0;
		prevIndex = -1;
		for(int a = 0;a<count;a++){
			prevIndex = index = text.indexOf(arr[i],prevIndex+1);
			if(index<startIndex) //in comment
				continue;
			int lastIndex = index + strlen(arr[i]);
			if(index>0 && text.at(index-1).isLetterOrNumber())
				continue;
			if(lastIndex < text.size() && text.at(lastIndex).isLetterOrNumber())
				continue;
			setFormat(index,strlen(arr[i]) , formats_[Type]);	
		}	
	}
	i = -1;
	arr = glsl_qualifiers;
	while(arr[++i]){
		int count = text.count(arr[i]);
		index = 0;
		prevIndex = -1;
		for(int a = 0;a<count;a++){
			prevIndex = index = text.indexOf(arr[i],prevIndex+1);
			if(index<startIndex) //in comment
				continue;
			int lastIndex = index + strlen(arr[i]);
			if(index>0 && text.at(index-1).isLetterOrNumber())
				continue;
			if(lastIndex < text.size() && text.at(lastIndex).isLetterOrNumber())
				continue;
			setFormat(index,strlen(arr[i]) , formats_[Qualifiers]);	
		}	
	}

	i = -1;
	arr = glsl_builtins_var;
	while(arr[++i]){
		int count = text.count(arr[i]);
		index = 0;
		prevIndex = -1;
		for(int a = 0;a<count;a++){
			prevIndex = index = text.indexOf(arr[i],prevIndex+1);
			if(index<startIndex) //in comment
				continue;
			int lastIndex = index + strlen(arr[i]);
			if(index>0 && text.at(index-1).isLetterOrNumber())
				continue;
			if(lastIndex < text.size() && text.at(lastIndex).isLetterOrNumber())
				continue;
			setFormat(index,strlen(arr[i]) , formats_[BuiltinVars]);	
		}	
	}

	i = -1;
	arr = glsl_builtins_func;
	while(arr[++i]){
		int count = text.count(arr[i]);
		index = 0;
		prevIndex = -1;
		for(int a = 0;a<count;a++){
			prevIndex = index = text.indexOf(arr[i],prevIndex+1);
			if(index<startIndex) //in comment
				continue;
			int lastIndex = index + strlen(arr[i]);
			if(index>0 && text.at(index-1).isLetterOrNumber())
				continue;
			if(lastIndex < text.size() && text.at(lastIndex).isLetterOrNumber())
				continue;
			setFormat(index,strlen(arr[i]) , formats_[BuiltinFunc]);	
		}	
	}

	if(text.startsWith("#")){
		setFormat(0,text.size() , formats_[Preprocessor]);	
	}
	

	index = text.indexOf("//");
	int commentStart = index;
	if(index!=-1){
		setFormat(index,text.size() - index , formats_[Comment]);	
	}


	index = text.indexOf("/*");
	bool inComment =  false;
	while(index!=-1){
		int end = text.indexOf("*/",index);
		if(end == -1){
			if(index>commentStart)
				inComment = true;
			end = text.size();
		}
		setFormat(index,end-index+2 , formats_[Comment]);	
		index = text.indexOf("/*",index+1);
	}
	if(inComment){
		setCurrentBlockState(1);
	}else{
		setCurrentBlockState(0);
	}

		
}


void SyntaxHighligther::loadConfig(){
	switch (syntax_)
	{
	case GLSL:
		loadGLSLConfig();
	case None:
	default:
		//TODO reset config
		break;
	}
}


void SyntaxHighligther::loadGLSLConfig(){
	int numFormats = SIZE_OF_GLSL_SYNTAX_ENUM;
	QColor textColor;
	QColor bgColor;
	textColor.setNamedColor("#999999");
	bgColor.setNamedColor("#4d4d4d");
	formats_ = new QTextCharFormat[numFormats];
	for(int i = 0;i<numFormats;i++){ //set defualed values
		formats_[i].setForeground(textColor);
		formats_[i].setBackground(bgColor);
	}

	formats_[Type].setForeground(QColor("#0000ff"));
	formats_[Comment].setForeground(QColor("#008000"));
	formats_[Qualifiers].setForeground(QColor("#0000ff"));
	formats_[BuiltinVars].setForeground(QColor("#008080"));
	formats_[BuiltinFunc].setForeground(QColor("#008080"));
	formats_[Preprocessor].setForeground(QColor("#808080"));

	
}

} // namespace


