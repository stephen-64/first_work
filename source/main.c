/* 
 * main.c The entry point to the game
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

#include "mud_tpl.h"
#include "mud.h"
#include "sky_tpl.h"
#include "sky.h"
#include "funcube_obj.h"
#include "funplane_obj.h"
#include "plane_obj.h"
#include "skybox_obj.h"
#include "house_obj.h"
#include "map_obj.h"

int main(){
	init_engine();
	//model cube = genmodel((void*)funcube_obj,funcube_obj_size);
	//model house = genmodel((void*)house_obj,house_obj_size);
	//model plane = genmodel((void*)funplane_obj,funplane_obj_size);
	model map = genmodel((void*)map_obj,map_obj_size);
	model skybox = genmodel((void*)skybox_obj,skybox_obj_size);
	
	TPLFile texfile;
	GXTexObj textureobj;
	TPL_OpenTPLFromMemory(&texfile,(void*)mud_tpl,mud_tpl_size);
	TPL_GetTexture(&texfile,mud,&textureobj);
	
	TPLFile skytexfile;
	GXTexObj skytextureobj;
	TPL_OpenTPLFromMemory(&skytexfile,(void*)sky_tpl,sky_tpl_size);
	TPL_GetTexture(&skytexfile,sky,&skytextureobj);

	while(1){
		/*
		Use this for a walking motion good for first person games
			preprender_walk();
			render_model_walk(skybox,skytextureobj);
			render_model_walk(cube,textureobj);
			render_model_walk(plane,textureobj);
		*/
		preprender();
		render_model(skybox,skytextureobj);
		//render_enviroment(cube,textureobj);
		render_enviroment(map,textureobj);
		//render_model(plane,textureobj);
		endrender();
	}
}