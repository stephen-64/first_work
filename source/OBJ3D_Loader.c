/* 
 * OBJ3D_Loader.c
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

#include "OBJ3D_Loader.h"

static void readline(FILE* file,char* line){
	do{
		fgets(line,255,file);
	}while((line[0]=='/')||(line[0] == '\n'));
	return;
}

model genmodel(void* file,size_t size){

	model newmod;
	FILE* model_file;
	char modcon[255];
	int tricount=0;
	int rawvert=0;
	int rawtext=0;
	model_file = fmemopen( file,size,"rb");
	while(!feof(model_file)){
		readline(model_file,modcon);
		if(modcon[0] =='f'){
			tricount++;
		}
		if(modcon[0] == 'v' && modcon[1] == ' '){
			rawvert++;
		}
		if(modcon[0] == 'v' && modcon[1] == 't'){
			rawtext++;
		}
	}
	
	fseek(model_file,0,SEEK_SET);
	pnts pt[3];
	xyzpoint vert[rawvert+1];
	txtpnt txt[rawtext+1];
	newmod.numtris = tricount;
	newmod.triangles = (tris*)malloc(sizeof(tris)*tricount);
	tricount=0;
	rawvert=0;
	rawtext=0;
	while(!feof(model_file)){
		readline(model_file,modcon);
		if(modcon[0] == 'v' && modcon[1] == ' '){
			sscanf(modcon, "v %f %f %f",&(vert[rawvert].x),&(vert[rawvert].y),&(vert[rawvert].z));
			rawvert++;
		}
		if(modcon[0]=='v' && modcon[1]=='t'){
			sscanf(modcon,"vt %f %f", &(txt[rawtext].u),&(txt[rawtext].v));
			rawtext++;
		}
		if(modcon[0]=='f'){
			sscanf(modcon,"f %d/%d %d/%d %d/%d", &(pt[0].v),&(pt[0].t), &(pt[1].v),&(pt[1].t), &(pt[2].v),&(pt[2].t));
			newmod.triangles[tricount].vert[0].x = vert[pt[0].v-1].x;
			newmod.triangles[tricount].vert[0].y = vert[pt[0].v-1].y;
			newmod.triangles[tricount].vert[0].z = vert[pt[0].v-1].z;
			newmod.triangles[tricount].text[0].u = txt[pt[0].t-1].u;
			newmod.triangles[tricount].text[0].v = txt[pt[0].t-1].v;
			
			newmod.triangles[tricount].vert[1].x = vert[pt[1].v-1].x;
			newmod.triangles[tricount].vert[1].y = vert[pt[1].v-1].y;
			newmod.triangles[tricount].vert[1].z = vert[pt[1].v-1].z;
			newmod.triangles[tricount].text[1].u = txt[pt[1].t-1].u;
			newmod.triangles[tricount].text[1].v = txt[pt[1].t-1].v;
			
			newmod.triangles[tricount].vert[2].x = vert[pt[2].v-1].x;
			newmod.triangles[tricount].vert[2].y = vert[pt[2].v-1].y;
			newmod.triangles[tricount].vert[2].z = vert[pt[2].v-1].z;
			newmod.triangles[tricount].text[2].u = txt[pt[2].t-1].u;
			newmod.triangles[tricount].text[2].v = txt[pt[2].t-1].v;
			tricount++;
		}
	}
	fclose(model_file);
	return newmod;
}




