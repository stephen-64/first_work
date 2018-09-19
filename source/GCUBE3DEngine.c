/* 
 * GCUBE3DEngine.c
 *
 * Copyright (c) 2018 Stephen Pollett, All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation and/or
 *    other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "GCUBE3DEngine.h"
#include "GCRender.h"

static void *framebuffer[2] = {NULL,NULL};
GXRModeObj *mode;

f32 xrotation;
f32 yrotation;
f32 xspd;
f32 yspd;

f32 walk = 0;
f32 walkangle = 0;

f32 upordown = 0.0f;

float xposition,zposition;
f32 zdpt=0.0f;



static GXColor Lights[] = {
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff},
		{0xff,0xff,0xff,0xff}
	};
	
f32 yscale;
u32 xfrbfheight;
Mtx view,mv,mr;
Mtx44 perspective;
u32 fb=0;
GXColor bkg={0,0,0,0xff};
f32 width;
f32 height;
void *fifo = NULL;
guVector cam = {0.0F,0.0F,0.0F},
		 up = {0.0F,1.0F,0.0F},
		 look = {0.0F,0.0F,-1.0F};

void init_engine(){
	//SETTING UP CPU & GPU
	VIDEO_Init();
	mode = VIDEO_GetPreferredMode(NULL);
	PAD_Init();
	framebuffer[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mode));
	framebuffer[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(mode));
	VIDEO_Configure(mode);
	VIDEO_SetNextFramebuffer(framebuffer[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(mode->viTVMode&VI_NON_INTERLACE){
		VIDEO_WaitVSync();
	}
	fifo=memalign(32,DEFAULT_FIFO_SIZE);
	memset(fifo,0,DEFAULT_FIFO_SIZE);
	width = mode->viWidth;
	height=mode->viHeight;
	GX_Init(fifo,DEFAULT_FIFO_SIZE);
	GX_SetCopyClear(bkg,0x00ffffff);
	GX_SetViewport(0,0,mode->fbWidth,mode->efbHeight,0,1);
	yscale=GX_GetYScaleFactor(mode->efbHeight,mode->xfbHeight);
	xfrbfheight=GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,mode->fbWidth,mode->efbHeight);
	GX_SetDispCopySrc(0,0,mode->fbWidth,mode->efbHeight);
	GX_SetDispCopyDst(mode->fbWidth,xfrbfheight);
	GX_SetCopyFilter(mode->aa,mode->sample_pattern,GX_TRUE,mode->vfilter);
	GX_SetFieldMode(mode->field_rendering,((mode->viHeight==2*mode->xfbHeight)?GX_ENABLE:GX_DISABLE));
	if(mode->aa){
		GX_SetPixelFmt(GX_PF_RGB565_Z16,GX_ZC_LINEAR);
	}else{
		GX_SetPixelFmt(GX_PF_RGB8_Z24,GX_ZC_LINEAR);
	}
	GX_SetCullMode(GX_CULL_NONE);
	GX_CopyDisp(framebuffer[fb],GX_TRUE);
	GX_SetDispCopyGamma(GX_GM_1_0);
	GX_InvVtxCache();
	GX_ClearVtxDesc();
	GX_SetVtxDesc(GX_VA_POS,GX_DIRECT);
	GX_SetVtxDesc(GX_VA_NRM,GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0,GX_DIRECT);
	GX_SetVtxAttrFmt(GX_VTXFMT0,GX_VA_POS,GX_POS_XYZ,GX_F32,0);
	GX_SetVtxAttrFmt(GX_VTXFMT0,GX_VA_NRM,GX_NRM_XYZ,GX_F32,0);
	GX_SetVtxAttrFmt(GX_VTXFMT0,GX_VA_TEX0,GX_TEX_ST,GX_F32,0);
	GX_SetTexCoordGen(GX_TEXCOORD0,GX_TG_MTX3x4,GX_TG_TEX0,GX_IDENTITY);
	guLightPerspective(mv,45,(f32)width/height,1.05F,1.0F,0.0F,0.0F);
	guMtxTrans(mr,0.0F,0.0F,-1.0F);
	guMtxConcat(mv,mr,mv);
	GX_LoadTexMtxImm(mv,GX_TEXMTX0,GX_MTX3x4);
	guLookAt(view,&cam,&up,&look);
	guPerspective(perspective,45,(f32)width/height,0.1F,300.0F);
	GX_LoadProjectionMtx(perspective,GX_PERSPECTIVE);
}

void preprender(){
			//Managing the users input
		PAD_ScanPads();
		
		s8 stick = PAD_SubStickX(0);
		if((stick <-8)||(stick>8)){
			yrotation -=(float)stick/50.f;
			xrotation +=(float)stick/50.f;
		}
		
		stick=PAD_StickY(0);
		if(stick>50){
			xposition -= (float)sin(DegToRad(yrotation))*0.05f;
			zposition -= (float)cos(DegToRad(yrotation))*0.05f;
			//if(walkangle >= 359.0f){
			//	walkangle = 0.0f;
			//}else{
			//	walkangle += 10;
			//}
			//walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		if(stick<-50){
			xposition += (float)sin(DegToRad(yrotation))*0.05f;
			zposition += (float)cos(DegToRad(yrotation))*0.05f;
			//if (walkangle <= 1.0f){
			//	walkangle = 359.0f;
			//}
			//else {
			//	walkangle -= 10;
			//}
			//walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		
		stick=PAD_StickX(0);
		if(stick>50){
			xposition += (float)cos(DegToRad(xrotation))*0.05f;
			zposition += (float)sin(DegToRad(xrotation))*0.05f;
			//if(walkangle >= 359.0f){
			//	walkangle = 0.0f;
			//}else{
			//	walkangle += 10;
			//}
			//walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		if(stick<-50){
			xposition -= (float)cos(DegToRad(xrotation))*0.05f;
			zposition -= (float)sin(DegToRad(xrotation))*0.05f;
			//if (walkangle <= 1.0f){
			//	walkangle = 359.0f;
			//}
			//else {
			//	walkangle -= 10;
			//}
			//walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		
		stick = PAD_SubStickY(0);
		if(((stick >8)||(stick<-8)) && ((90>=upordown) && (upordown >=-90))){
			zdpt -= ((f32)stick*0.01f);
			upordown -= ((f32)stick*0.01f);
			if(upordown >90){
				upordown = 90.0F;
			}
			if(upordown < -90){
				upordown = -90.0F;
			}
		}
		
		GX_SetViewport(0,0,mode->fbWidth,mode->efbHeight,0,1);
}

void preprender_walk(){
			//Managing the users input
		PAD_ScanPads();
		
		s8 stick = PAD_SubStickX(0);
		if((stick <-8)||(stick>8)){
			yrotation -=(float)stick/50.f;
			xrotation +=(float)stick/50.f;
		}
		
		stick=PAD_StickY(0);
		if(stick>50){
			xposition -= (float)sin(DegToRad(yrotation))*0.05f;
			zposition -= (float)cos(DegToRad(yrotation))*0.05f;
			if(walkangle >= 359.0f){
				walkangle = 0.0f;
			}else{
				walkangle += 10;
			}
			walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		if(stick<-50){
			xposition += (float)sin(DegToRad(yrotation))*0.05f;
			zposition += (float)cos(DegToRad(yrotation))*0.05f;
			if (walkangle <= 1.0f){
				walkangle = 359.0f;
			}
			else {
				walkangle -= 10;
			}
			walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		
		stick=PAD_StickX(0);
		if(stick>50){
			xposition += (float)cos(DegToRad(xrotation))*0.05f;
			zposition += (float)sin(DegToRad(xrotation))*0.05f;
			if(walkangle >= 359.0f){
				walkangle = 0.0f;
			}else{
				walkangle += 10;
			}
			walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		if(stick<-50){
			xposition -= (float)cos(DegToRad(xrotation))*0.05f;
			zposition -= (float)sin(DegToRad(xrotation))*0.05f;
			if (walkangle <= 1.0f){
				walkangle = 359.0f;
			}
			else {
				walkangle -= 10;
			}
			walk = (float)sin(DegToRad(walkangle))/20.0f;
		}
		
		stick = PAD_SubStickY(0);
		if(((stick >8)||(stick<-8)) && ((90>=upordown) && (upordown >=-90))){
			zdpt -= ((f32)stick*0.01f);
			upordown -= ((f32)stick*0.01f);
			if(upordown >90){
				upordown = 90.0F;
			}
			if(upordown < -90){
				upordown = -90.0F;
			}
		}
		
		GX_SetViewport(0,0,mode->fbWidth,mode->efbHeight,0,1);
}

void preprender_stat(){
	GX_SetViewport(0,0,mode->fbWidth,mode->efbHeight,0,1);
}

void endrender(){
	GX_SetZMode(GX_TRUE,GX_LEQUAL,GX_TRUE);
	GX_SetColorUpdate(GX_TRUE);
	GX_CopyDisp(framebuffer[fb],GX_TRUE);
	GX_DrawDone();
	fb^=1;
	VIDEO_SetNextFramebuffer(framebuffer[fb]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
}

void render_model_walk(model model,GXTexObj texture){
	render_shape_walk(model,view,xposition,zposition,yrotation,upordown,walk,Lights,texture);
}

void render_model(model model,GXTexObj texture){
	render_shape(model,view,xposition,zposition,yrotation,upordown,Lights,texture);
}

void render_enviroment(model model,GXTexObj texture){
	render_shape_cull(model,view,xposition,zposition,yrotation,upordown,Lights,texture);
}

void render_enviroment_walk(model model,GXTexObj texture){
	render_shape_walk_cull(model,view,xposition,zposition,yrotation,upordown,walk,Lights,texture);
}