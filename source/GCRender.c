/* 
 * GCRender.c
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

#include "GCRender.h"

void render_shape_walk(model model,Mtx view, float xposition,float zposition,float yrotation,f32 upordown,f32 walk,GXColor light[],GXTexObj texture){
	f32 mx,my,mz,mu,mv;
	f32 xtran = -xposition;
	f32 ztran = -zposition;
	f32 ytran = -walk-0.25f;
	//f32 ytran will be used for vertical changes
	f32 rotation = 360.0f-yrotation;
	
	int numtris;
	Mtx mod;
	Mtx mrot;
	Mtx mview;
	guVector axis;
	lighting(view,light[0],light[1],light[2]);
	GX_SetTevOp(GX_TEVSTAGE0,GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD0,GX_TEXMAP0,GX_COLOR0A0);
	GX_LoadTexObj(&texture,GX_TEXMAP0);
	
	axis.x=1.0f;
	axis.y=0;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,upordown);
	guMtxConcat(mod,view,mview);
	
	axis.x=0;
	axis.y=1.0f;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,rotation);
	guMtxConcat(mview,mod,mview);
	
	guMtxApplyTrans(mview,mrot,xtran,ytran,ztran);
	GX_LoadPosMtxImm(mrot,GX_PNMTX0);
	
	numtris=model.numtris;
	for(int i=0;i<numtris;i++){
		GX_Begin(GX_TRIANGLES,GX_VTXFMT0,3);
		
		mx = model.triangles[i].vert[0].x;
		my = model.triangles[i].vert[0].y;
		mz = model.triangles[i].vert[0].z;
		mu = model.triangles[i].text[0].u;
		mv = model.triangles[i].text[0].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[1].x;
		my = model.triangles[i].vert[1].y;
		mz = model.triangles[i].vert[1].z;
		mu = model.triangles[i].text[1].u;
		mv = model.triangles[i].text[1].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[2].x;
		my = model.triangles[i].vert[2].y;
		mz = model.triangles[i].vert[2].z;
		mu = model.triangles[i].text[2].u;
		mv = model.triangles[i].text[2].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		GX_End();
	}
	return;
}

void render_shape(model model,Mtx view, float xposition,float zposition,float yrotation,f32 upordown,GXColor light[],GXTexObj texture){
	f32 mx,my,mz,mu,mv;
	f32 xtran = -xposition;
	f32 ztran = -zposition;
	f32 ytran = 0;
	//f32 ytran will be used for vertical changes
	f32 rotation = 360.0f-yrotation;
	
	int numtris;
	Mtx mod;
	Mtx mrot;
	Mtx mview;
	guVector axis;
	lighting(view,light[0],light[1],light[2]);
	GX_SetTevOp(GX_TEVSTAGE0,GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD0,GX_TEXMAP0,GX_COLOR0A0);
	GX_LoadTexObj(&texture,GX_TEXMAP0);
	
	axis.x=1.0f;
	axis.y=0;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,upordown);
	guMtxConcat(mod,view,mview);
	
	axis.x=0;
	axis.y=1.0f;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,rotation);
	guMtxConcat(mview,mod,mview);
	
	guMtxApplyTrans(mview,mrot,xtran,ytran,ztran);
	GX_LoadPosMtxImm(mrot,GX_PNMTX0);
	
	numtris=model.numtris;
	for(int i=0;i<numtris;i++){
		GX_Begin(GX_TRIANGLES,GX_VTXFMT0,3);
		
		mx = model.triangles[i].vert[0].x;
		my = model.triangles[i].vert[0].y;
		mz = model.triangles[i].vert[0].z;
		mu = model.triangles[i].text[0].u;
		mv = model.triangles[i].text[0].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[1].x;
		my = model.triangles[i].vert[1].y;
		mz = model.triangles[i].vert[1].z;
		mu = model.triangles[i].text[1].u;
		mv = model.triangles[i].text[1].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[2].x;
		my = model.triangles[i].vert[2].y;
		mz = model.triangles[i].vert[2].z;
		mu = model.triangles[i].text[2].u;
		mv = model.triangles[i].text[2].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		GX_End();
	}
	return;
}

void render_shape_walk_cull(model model,Mtx view, float xposition,float zposition,float yrotation,f32 upordown,f32 walk,GXColor light[],GXTexObj texture){
	GX_SetCullMode(GX_CULL_FRONT);
	f32 mx,my,mz,mu,mv;
	f32 xtran = -xposition;
	f32 ztran = -zposition;
	f32 ytran = -walk-0.25f;
	//f32 ytran will be used for vertical changes
	f32 rotation = 360.0f-yrotation;
	
	int numtris;
	Mtx mod;
	Mtx mrot;
	Mtx mview;
	guVector axis;
	lighting(view,light[0],light[1],light[2]);
	GX_SetTevOp(GX_TEVSTAGE0,GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD0,GX_TEXMAP0,GX_COLOR0A0);
	GX_LoadTexObj(&texture,GX_TEXMAP0);
	
	axis.x=1.0f;
	axis.y=0;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,upordown);
	guMtxConcat(mod,view,mview);
	
	axis.x=0;
	axis.y=1.0f;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,rotation);
	guMtxConcat(mview,mod,mview);
	
	guMtxApplyTrans(mview,mrot,xtran,ytran,ztran);
	GX_LoadPosMtxImm(mrot,GX_PNMTX0);
	
	numtris=model.numtris;
	for(int i=0;i<numtris;i++){
		GX_Begin(GX_TRIANGLES,GX_VTXFMT0,3);
		
		mx = model.triangles[i].vert[0].x;
		my = model.triangles[i].vert[0].y;
		mz = model.triangles[i].vert[0].z;
		mu = model.triangles[i].text[0].u;
		mv = model.triangles[i].text[0].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[1].x;
		my = model.triangles[i].vert[1].y;
		mz = model.triangles[i].vert[1].z;
		mu = model.triangles[i].text[1].u;
		mv = model.triangles[i].text[1].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[2].x;
		my = model.triangles[i].vert[2].y;
		mz = model.triangles[i].vert[2].z;
		mu = model.triangles[i].text[2].u;
		mv = model.triangles[i].text[2].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		GX_End();
	}
	GX_SetCullMode(GX_CULL_NONE);
	return;
}

void render_shape_cull(model model,Mtx view, float xposition,float zposition,float yrotation,f32 upordown,GXColor light[],GXTexObj texture){
	GX_SetCullMode(GX_CULL_FRONT);
	f32 mx,my,mz,mu,mv;
	f32 xtran = -xposition;
	f32 ztran = -zposition;
	f32 ytran = 0;
	//f32 ytran will be used for vertical changes
	f32 rotation = 360.0f-yrotation;
	
	int numtris;
	Mtx mod;
	Mtx mrot;
	Mtx mview;
	guVector axis;
	lighting(view,light[0],light[1],light[2]);
	GX_SetTevOp(GX_TEVSTAGE0,GX_MODULATE);
	GX_SetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD0,GX_TEXMAP0,GX_COLOR0A0);
	GX_LoadTexObj(&texture,GX_TEXMAP0);
	
	axis.x=1.0f;
	axis.y=0;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,upordown);
	guMtxConcat(mod,view,mview);
	
	axis.x=0;
	axis.y=1.0f;
	axis.z=0;
	guMtxIdentity(mod);
	guMtxRotAxisDeg(mod,&axis,rotation);
	guMtxConcat(mview,mod,mview);


	guMtxApplyTrans(mview,mrot,xtran,ytran,ztran);
	GX_LoadPosMtxImm(mrot,GX_PNMTX0);
	
	numtris=model.numtris;
	for(int i=0;i<numtris;i++){
		GX_Begin(GX_TRIANGLES,GX_VTXFMT0,3);
		
		mx = model.triangles[i].vert[0].x;
		my = model.triangles[i].vert[0].y;
		mz = model.triangles[i].vert[0].z;
		mu = model.triangles[i].text[0].u;
		mv = model.triangles[i].text[0].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[1].x;
		my = model.triangles[i].vert[1].y;
		mz = model.triangles[i].vert[1].z;
		mu = model.triangles[i].text[1].u;
		mv = model.triangles[i].text[1].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		
		mx = model.triangles[i].vert[2].x;
		my = model.triangles[i].vert[2].y;
		mz = model.triangles[i].vert[2].z;
		mu = model.triangles[i].text[2].u;
		mv = model.triangles[i].text[2].v;
		GX_Position3f32(mx,my,mz);
		GX_Normal3f32((f32)0,(f32)0,(f32)1);
		GX_TexCoord2f32(mu,-mv);
		GX_End();
	}
	GX_SetCullMode(GX_CULL_NONE);
	return;
}

void lighting(Mtx view,GXColor lit,GXColor amb,GXColor mat){
	guVector lipos;
	GXLightObj liobj;
	
	lipos.x=0;
	lipos.y=0;
	lipos.z=2.0f;
	
	guVecMultiply(view,&lipos,&lipos);
	
	GX_InitLightPos(&liobj,lipos.x,lipos.y,lipos.z);
	GX_InitLightColor(&liobj,lit);
	
	GX_SetNumChans(1);
	GX_SetChanCtrl(GX_COLOR0A0,GX_ENABLE,GX_SRC_REG,GX_SRC_REG,GX_LIGHT0,GX_DF_CLAMP,GX_AF_NONE);
	GX_SetChanAmbColor(GX_COLOR0A0,amb);
	GX_SetChanMatColor(GX_COLOR0A0,mat);
}