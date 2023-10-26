#include<GL/glew.h>
#include<GL/gl.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <SDL_mixer.h>
#include <assert.h>
#include <ctime>


union V3I{
  struct{
    int x;
    int y;
    int z;
  };
  struct{
    int r;
    int g;
    int b;

  };
  int e[3];
};

union V4{
  struct{
    float x;
    float y;
    float z;
    float w;


  };


  struct{
    float r;
    float g;
    float b;
    float a;

  };
  float e[4];
};



union V3{
  struct{
    float x;
    float y;
    float z;
  };

  struct{
    float r;
    float g;
    float b;

  };
  float e[3];
};

struct Coler{
char**names;
int count;
V3*kd;
};

struct Triangle{

V3 col;
int a;
int b;
int c;

 };

struct Model{

V3*v;
int vc;
Triangle*f;
int fc;

};



V4 v4m(V4 m,float two){

  m.x*=two;
  m.y*=two;
  m.z*=two;
  m.w*=two;

  return m;

}

V4 v4m(V4 m,V4 two){

  m.x*=two.x;
  m.y*=two.y;
  m.z*=two.z;
  m.w*=two.w;

  return m;

}

struct Matrix{
  float e[16];


};


Matrix operator*(Matrix b,Matrix a){
  Matrix r;
  r.e[0]=a.e[0]*b.e[0]+a.e[1]*b.e[4]+a.e[2]*b.e[8]+a.e[3]*b.e[12];
  r.e[1]=a.e[0]*b.e[1]+a.e[1]*b.e[5]+a.e[2]*b.e[9]+a.e[3]*b.e[13];
  r.e[2]=a.e[0]*b.e[2]+a.e[1]*b.e[6]+a.e[2]*b.e[10]+a.e[3]*b.e[14];
  r.e[3]=a.e[0]*b.e[3]+a.e[1]*b.e[7]+a.e[2]*b.e[11]+a.e[3]*b.e[15];

  r.e[4]=a.e[4]*b.e[0]+a.e[5]*b.e[4]+a.e[6]*b.e[8]+a.e[7]*b.e[12];
  r.e[5]=a.e[4]*b.e[1]+a.e[5]*b.e[5]+a.e[6]*b.e[9]+a.e[7]*b.e[13];
  r.e[6]=a.e[4]*b.e[2]+a.e[5]*b.e[6]+a.e[6]*b.e[10]+a.e[7]*b.e[14];
  r.e[7]=a.e[4]*b.e[3]+a.e[5]*b.e[7]+a.e[6]*b.e[11]+a.e[7]*b.e[15];

  r.e[8]=a.e[8]*b.e[0]+a.e[9]*b.e[4]+a.e[10]*b.e[8]+a.e[11]*b.e[12];
  r.e[9]=a.e[8]*b.e[1]+a.e[9]*b.e[5]+a.e[10]*b.e[9]+a.e[11]*b.e[13];
  r.e[10]=a.e[8]*b.e[2]+a.e[9]*b.e[6]+a.e[10]*b.e[10]+a.e[11]*b.e[14];
  r.e[11]=a.e[8]*b.e[3]+a.e[9]*b.e[7]+a.e[10]*b.e[11]+a.e[11]*b.e[15];

  r.e[12]=a.e[12]*b.e[0]+a.e[13]*b.e[4]+a.e[14]*b.e[8]+a.e[15]*b.e[12];
  r.e[13]=a.e[12]*b.e[1]+a.e[13]*b.e[5]+a.e[14]*b.e[9]+a.e[15]*b.e[13];
  r.e[14]=a.e[12]*b.e[2]+a.e[13]*b.e[6]+a.e[14]*b.e[10]+a.e[15]*b.e[14];
  r.e[15]=a.e[12]*b.e[3]+a.e[13]*b.e[7]+a.e[14]*b.e[11]+a.e[15]*b.e[15];

  return r;
}


enum Blocki {

  CLEAR,
  GRASS,
  SAND,
  WATER,
  STONE,
  LAVA,
  COBLESTONE,
  FURNACE,
  IORNORE,
  OAKLOG,
  NAPLANK


};



struct Block{

  Blocki type;

  float x;
  float y;
  float z;

};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
V3 normalize(V3 v){
float l=sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
v.x/=l;
v.y/=l;
v.z/=l;
return v;

}


Coler edcoler(const char*mtl){
  Coler cpp={};
  cpp.names=new char*[10];
  cpp.kd=new V3 [10];
  FILE*sus=fopen(mtl,"rb");
  printf("%s\n",mtl );
  fflush(stdout);
  assert(sus);
  while (!feof(sus)){
    char buffer[1024];
    fgets(buffer,1024,sus);
    if (buffer[0]=='#') {
      continue;
    }
    char*word=strtok(buffer," ");
    if (strcmp(word,"newmtl")==0) {
      char* name=strtok(NULL," \n");
      assert(cpp.count<10);
      cpp.names[cpp.count++]=new char[strlen(name)+1];
      strcpy(cpp.names[cpp.count-1],name);

    }
    if (strcmp(word,"Kd")==0) {
      char* x=strtok(NULL," ");
      char* y=strtok(NULL," ");
      char* z=strtok(NULL," ");
      V3 kd;
      kd.x=atof(x);
      kd.y=atof(y);
      kd.z=atof(z);
      assert(cpp.count);
      cpp.kd[cpp.count-1]=kd;

    }
  }
  return cpp;
}



Model lender(const char*obj){
Model m={};
Coler colerII={};
V3 kdII={0,1,0};
m.v=new V3[5268];
m.f=new Triangle[5268];
FILE*sus=fopen(obj,"rb");
printf("%s\n",obj );
fflush(stdout);
assert(sus);
while (!feof(sus)){
char buffer[1024];
fgets(buffer,1024,sus);
if (buffer[0]=='#') {
  continue;
  }
  char*word=strtok(buffer," \n");
  if (strcmp(word,"mtllib")==0) {
    char* mtl=strtok(NULL," \n");
    colerII=edcoler(mtl);
  }
  if (strcmp(word,"v")==0) {
    char* x=strtok(NULL," ");
    char* y=strtok(NULL," ");
    char* z=strtok(NULL," ");
    V3 v;
    v.x=atof(x);
    v.y=atof(y);
    v.z=atof(z);
    assert(m.vc<5268);
    m.v[m.vc++]=v;

  }
  if (strcmp(word,"usemtl")==0) {
    char*mat=strtok(NULL," \n");
    printf("%s\n",mat );
    for (int i=0;  i<colerII.count; i++) {
      printf("%s_\n",colerII.names[i]);
      if (strcmp(colerII.names[i],mat)==0) {
        kdII=colerII.kd[i];
        printf("colers are %f %f %f\n",kdII.r,kdII.g,kdII.b);
        fflush(stdout);
      }
    }
  }
  if (strcmp(word,"f")==0) {
    char*a=strtok(NULL," ");
    char*b=strtok(NULL," ");
    char*c=strtok(NULL," ");
    Triangle f;
    f.a=atoi(a)-1;
    f.b=atoi(b)-1;
    f.c=atoi(c)-1;
    f.col=kdII;
    assert(m.fc<5268);
    m.f[m.fc++]=f;

  }
 }
 return m;
}
//tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
void create_entity(Model m,V3 pos,float orit,Matrix mat){

  float cc=cosf(orit);
  float ss=sinf(orit);

  Matrix pr= {
   cc,0,ss,0,
    0,1,0,0,
  -ss,0,cc,0,
    0,0,0,1,
  };

Matrix pm={
1,0,0,0,
0,1,0,0,
0,0,1,0,
pos.x,pos.y,pos.z,1,
};

glLoadMatrixf((mat*pm*pr).e);

glBegin(GL_TRIANGLES);
for(int i=0;i<m.fc;i++){
Triangle f=m.f[i];
glColor3fv(f.col.e);
glVertex3fv(m.v[f.a].e);
glVertex3fv(m.v[f.b].e);
glVertex3fv(m.v[f.c].e);
}
glEnd();
}


GLuint load(const char*file){
  SDL_Surface*addpng=IMG_Load(file);
  GLuint tex;
  glGenTextures(1,&tex);
  glBindTexture(GL_TEXTURE_2D,tex);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,addpng->w,addpng->h,0,GL_RGBA,GL_UNSIGNED_BYTE,addpng->pixels);
  return tex;

}


void drawimage(float x,float y,GLuint image, float r,float size=0.05,float a=1)
{

  float tx0=0;
  float tx1=1;
  float c=cos(r);
  float s=sin(r);
  float m1[16]={
    c,s,0,0,
    -s,c,0,0,
    0,0,1,0,
    x,y,0,1,
  };
  glLoadMatrixf(m1);
  glBindTexture(GL_TEXTURE_2D,image);
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_TRIANGLES);
  glColor3f(1,1,1);
  glTexCoord2f(tx1,0);
  glVertex2f(+size,+size);
  glTexCoord2f(tx0,1);
  glVertex2f(-size,-size);
  glTexCoord2f(tx0,0);
  glVertex2f(-size,+size);
  glTexCoord2f(tx1,1);
  glVertex2f(+size,-size);
  glTexCoord2f(tx0,1);
  glVertex2f(-size,-size);
  glTexCoord2f(tx1,0);
  glVertex2f(+size,+size);
  glEnd();
  glDisable(GL_TEXTURE_2D);

}







void draw_cube(float x, float y,float z,uint32_t i)
{



  glBegin(GL_TRIANGLES);

  glColor4ubv((uint8_t*)&i);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);

  i++;
  glColor4ubv((uint8_t*)&i);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);

  i++;
  glColor4ubv((uint8_t*)&i);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);

  i++;
  glColor4ubv((uint8_t*)&i);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);

  i++;
  glColor4ubv((uint8_t*)&i);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);

  i++;
  glColor4ubv((uint8_t*)&i);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glEnd();
}



void draw_block(float x, float y,float z,Blocki t)
{

  V4 c;

  glBegin(GL_TRIANGLES);
  if(t==GRASS)
  //glColor3f(0,1,0);
  c={0,1,0,1};
  else if (t==SAND) {
    //glColor3f(1,1,0);
    c={1,1,0,1};
  }
  else if (t==WATER) {
    //glColor4f(0,0,1,0.4);
    c={0,0,1,0.4};
  }
  else if(t==STONE){
    c={0.4,0.4,0.4,1};
  }
  else if(t==LAVA){
    c={1,0,0,1};
  }

  glColor4fv(v4m(c,{0.4,0.4,0.4,1}).e);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);

  glColor4fv(c.e);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);

  glColor4fv(v4m(c,{0.8,0.8,0.8,1}).e);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);

  glColor4fv(v4m(c,{0.8,0.8,0.8,1}).e);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);

  glColor4fv(v4m(c,{0.8,0.8,0.8,1}).e);
  glVertex3f(x-0.2,y+0.2,z+0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z+0.2);
  glVertex3f(x-0.2,y+0.2,z-0.2);
  glVertex3f(x-0.2,y-0.2,z-0.2);
  glVertex3f(x-0.2,y+0.2,z+0.2);

  glColor4fv(v4m(c,{0.8,0.8,0.8,1}).e);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z+0.2);
  glVertex3f(x+0.2,y+0.2,z-0.2);
  glVertex3f(x+0.2,y-0.2,z-0.2);
  glVertex3f(x+0.2,y+0.2,z+0.2);
  glEnd();
}

void genstru(Block*blocks,uint32_t&n,uint32_t maxb,float x,float z){
      blocks[n++]={COBLESTONE,0.4f+x,0.8f,0.0f+z};
      blocks[n++]={COBLESTONE,0.4f+x,0.4f,0.0f+z};
      blocks[n++]={COBLESTONE,-0.4f+x,0.4f,0.0f+z};
      blocks[n++]={COBLESTONE,-0.4f+x,0.8f,0.0f+z};
      blocks[n++]={NAPLANK,0.0f+x,1.2f,0.0f+z};
}


void gentree(Block*blocks,uint32_t&n,uint32_t maxb,float x,float z){
      blocks[n++]={OAKLOG,0+x,0.4f,0.0f+z};
      blocks[n++]={OAKLOG,0+x,0.8f,0.0f+z};
      blocks[n++]={OAKLOG,0+x,1.2f,0.0f+z};


}




//ssprweadwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww

Blocki cb(Block*blocks,uint32_t n,float x,float y,float z){

  for (size_t j= 0;j<n;j++) {
    Block bt=blocks[j];
    if(fabs(bt.x-x)<0.01&&fabs(bt.z-z)<0.01&&fabs(bt.y-y)<0.01){
      return bt.type;
    }
  }
return CLEAR;
}

int cubeindex(Block*blocks,uint32_t n,float x,float y,float z){

  for (size_t j= 0;j<n;j++) {
    Block bt=blocks[j];
    if(fabs(bt.x-x)<0.01&&fabs(bt.z-z)<0.01&&fabs(bt.y-y)<0.01){
      return j;
    }
  }
return -1;
}







int main(int argc, char *argv[]) {

    int seed=time(0);
    //seed=1684798100;
    srand(seed);
      printf("seed is:%d\n",seed );
  Block blocks[100000]={


    //{GRASS,0.4,0,0.4},
    //{SAND,0,0,0},
    //{SAND,0.4,0,0},
    //{SAND,0,0,-0.4},
    //{WATER,0.4,0,-0.4},


  };
  int wid=10;
  int depth=5;
  uint32_t n=0;
  uint32_t maxb=sizeof(blocks)/sizeof(blocks[0]);
  for (int i=0; i<wid*(wid*depth); i++) {
    int bhl=0-i/(wid*wid);
    int bx=i%wid;
    int bz=i/wid%wid;
    printf("helo:%d\n",bhl );
    Block block={};
    if(bhl==0){
    switch(rand()%3){
      case 0:
      block.type=GRASS;
      break;
      case 1:
      block.type=SAND;
      break;
      case 2:
      block.type=WATER;

    }

    }
    else if (bhl<0&&bhl>-depth+1&&bx<wid-1&&bz<wid-1&&bx>0&&bz>0) {
      switch(rand()%3){
        case 0:
        block.type=STONE;
        break;
        case 1:
        block.type=LAVA;
        break;
        case 2:
        block.type=IORNORE;

    }
    }
    else{
      switch(rand()%2){
case 0:
block.type=STONE;
break;
case 1:
block.type=IORNORE;
}
    }

    block.x=i%wid*0.4;
    block.z=i/wid%wid*0.4;
    block.y=bhl*0.4;
    blocks[n++]=block;
  }
 for (size_t i=0; i < 25; i++) {
   if(rand()%5==0){

     for (int j = 0; j < rand()%11; j++) {
       Block block={};
       block.type=STONE;
       block.x=i%5*0.4;
       block.z=i/5%5*0.4;
       block.y=j*0.4+0.4;
         blocks[n++]=block;
     }

   }
 }

 genstru(blocks,n,maxb,3.2,rand()%9*0.4);
 gentree(blocks,n,maxb,2.4,rand()%5*0.4);


   Blocki bp=GRASS;

  float cubespinx=0.2;
  float cubespiny=0;
  float cubey=0;
  float cubex=0;
  float cubez=0;
 int hack=0;

 int bc=0;

int slot2=0;
int slot3=0;
int slot4=0;

int prorit=0;

int babybackribs=0;


int m4=0;

 int ix=0;
 int iy=0;
 int inon=0;

  float ex=1.3;
  float ey=0.4;;
  float ez=1.3;
  float er=0;

float pickr=-0.3;

 int hp=900;
 int zhp=900;

int gg=0;

float px2=0;
float py2=0;
float pz2=0;
float pr=0;
int php=30;
int game=0;

  float px=0.8;
  float py=0;
  float pz=0.8;

int dead=0;
  //0.8,2.1,0.8

  float orit=0;

  int fall=0;

  int fall2=0;

int eat=0;

  int yumy=0;
    int wa=0;
    int la=0;

  int mousex=0;
  int mousey=0;

  int mouseleft=0;
  int mouseright=0;

  float picx=-0.8;
  float picy=-0.9;

  int c1=0;
  int cc2=0;
  int c3=0;
  int lol=0;

int eaty=0;

  //float pickr=0;


  int gr=1;
  SDL_Init(SDL_INIT_VIDEO);



  // SDL_WINDOW_FULLSCREEN|


  int world_X = 600;
  int world_Y = 600;

  SDL_Window*window=SDL_CreateWindow("bildnbreak",0,0,world_X,world_Y,SDL_WINDOW_OPENGL);
  SDL_GL_CreateContext(window);
  glewExperimental=GL_TRUE;
  glewInit();
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


  GLuint colortex;
  glGenTextures(1,&colortex);
  glBindTexture(GL_TEXTURE_2D,colortex);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,world_X,world_Y,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);

  GLuint depthtex;
  glGenTextures(1,&depthtex);
  glBindTexture(GL_TEXTURE_2D,depthtex);
  glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,world_X,world_Y,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,NULL);


  GLuint framebuffer;
  glGenFramebuffers(1,&framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colortex,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthtex,0);
  Model pig=lender("pepa.obj");
  Model wood2=lender("plank2.obj");
  Model gra=lender("grass.obj");
  Model log=lender("wood.obj");
  Model ss=lender("sand.obj");
  Model ss2=lender("stone.obj");
  Model f=lender("f.obj");
  Model io=lender("io.obj");
  Model cob=lender("coblestone2.obj");
  Model player=lender("player1.obj");
  Model eek=lender("zombie.obj");
  GLuint pic1=load("silver-picaxe.png");
  GLuint in=load("inv.png");
  GLuint block2=load("block4.png");
  GLuint ri=load("rawi.png");
  GLuint ii=load("ii.png");
  GLuint wp=load("planks.png");
  GLuint nwp=load("np.png");
  GLuint item=load("oakwood.png");
  GLuint buc=load("y.png");
  GLuint meat=load("daedpepa.png");
  GLuint meaty=load("dinerbone.png");
  GLuint q=load("start.png");




  Mix_OpenAudio(44000, MIX_DEFAULT_FORMAT, 2, 4096);

  Mix_Chunk *k = Mix_LoadWAV("zombie_pain.wav");
  assert(k);

  Mix_Chunk *ow = Mix_LoadWAV("ow.wav");
  assert(ow);


    Mix_Chunk *yum = Mix_LoadWAV("pig.mp3");
    assert(yum);
    Mix_Chunk *nom = Mix_LoadWAV("nyom.mp3");
    assert(nom);

    Mix_Music *ah = Mix_LoadMUS("music.mp3");
    assert(ah);





    Mix_PlayMusic(ah, -1);
    //Mix_Volume(-5,1);
    Mix_VolumeMusic(20);

  while(gr==1){


    SDL_Event event;

    while(SDL_PollEvent(&event)){
      if(event.type==SDL_QUIT)
      return 0;


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_ESCAPE)
      return 0;




      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_RIGHT)
      cubespinx+=0.1;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_LEFT)
      cubespinx-=0.1;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_DOWN)
      cubespiny-=0.05;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_UP)
      cubespiny+=0.05;



      if (event.type==SDL_MOUSEMOTION) {
        mousex=event.motion.x;
        mousey=world_Y-event.motion.y;
      }



            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_i){
              inon=!inon;
            }


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_h){
        hack=1;
      }


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_c){
      hack=0;
      }


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_2){
        lol=1;
      }

      if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_2)
      lol=0;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_b){
        mouseright=1;
      }

      if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_b)
      mouseright=0;


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_e){
        eat=1;
      }

      if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_e)
      eat=0;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_a){
        px-=cosf(orit-M_PI/2)*0.05;
        pz-=sinf(orit-M_PI/2)*0.05;
        printf("%f,%f,%f\n",px,py,pz );
        fflush(stdout);
      }


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_d){
        px+=cosf(orit-M_PI/2)*0.05;
        pz+=sinf(orit-M_PI/2)*0.05;
      }

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_s)  {
        px-=cosf(orit)*0.05;
        pz-=sinf(orit)*0.05;
      }

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_w){
        px+=cosf(orit)*0.05;
        pz+=sinf(orit)*0.05;
      }
      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_e){
        eaty=1;
      }
      if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_e){
        eaty=0;
      }

            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_x&&zhp>0){
              Mix_PlayChannel(-1,k,0);
              zhp-=5;
            }

            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_k&&dead==0){
              Mix_PlayChannel(-1,yum,0);
              php-=5;
            }


            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_r){
            px=0.8;
            py=0;;
            pz=0.8;
            }


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_SPACE)
      py+=0.5;

      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_LSHIFT)
      py-=0.05;

      if (event.type==SDL_MOUSEBUTTONDOWN) {
        mouseleft=1;
        gg=1;
        mousex=event.motion.x;
        mousey=world_Y-event.motion.y;
      }

      if (event.type==SDL_MOUSEBUTTONUP) {
        mouseleft=0;
        gg=0;



      }

            if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_m)
            m4=1;


            if(event.type==SDL_KEYUP&&event.key.keysym.sym==SDLK_m)
            m4=0;


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_1)
      bp=GRASS;


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_2)
      bp=SAND;


      if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_3)
      bp=STONE;



          if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_4)
          bp=FURNACE;

          if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_5)
          bp=NAPLANK;

          if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_6)
          bp=WATER;
          if(event.type==SDL_KEYDOWN&&event.key.keysym.sym==SDLK_7)
          bp=LAVA;



}
if (game==1) {


if(inon&&m4&&slot4>0){
c1+=1;
slot4-=1;
m4=0;
}
if(inon==1&&m4==1&&c1>0&&slot3>0){
cc2+=1;
c1-=1;
slot3-=1;
m4=0;
}
if(inon==1&&lol==1&&slot3>=3){
c3+=1;
slot3-=3;
lol=0;
}

if(yumy>0&&eaty==1){
yumy-=1;
hp+=10;
Mix_PlayChannel(-1,nom,0);
}
if(php<=0&&dead==0){
dead=1;
babybackribs+=1;
}
if(eat==1&&babybackribs>0){
babybackribs-=1;
}
if (mouseleft==1) {
  pickr=-1.5;

}
if(mouseleft==0){
pickr+=0.07;
if (pickr> 0.3) {
  pickr=0.3;
}
}



    prorit+=4;
    if (prorit>=523) {
      prorit=0;
      pr=rand()%360/(M_PI*2);
    }
    int in_solid=0;
    int in_fluid=0;
    float solid_y=NAN;
    px2+=cosf(pr)*0.02;
    pz2+=sinf(pr)*0.02;
    py2-=0.01;
  //  py-=0.01;
    ey-=0.05;
    cubex=px;
    cubey=py;
    cubez=pz;
    V3 fakp={px-ex,py-ey,pz-ez};
    fakp=normalize(fakp);
    er=atan2(fakp.z,fakp.x);
    ex+=fakp.x*0.0001;
    ey+=fakp.y*0.0001;
    ez+=fakp.z*0.0001;
    if(px-0.4<ex+0.03&&px+0.4>ex-0.03&&py-0.8<ey+0.06&&py+0.8>ey-0.06&&pz-0.4<ez+0.03&&pz+0.4>ez-0.03){
    Mix_PlayChannel(-1,ow,0);
    hp-=5;
    }
    if(hp<=0){
       ex=1.3;
       ey=0.4;
       ez=1.3;
       px=0.8;
       py=0;
       pz=0.8;
       hp=90;
       zhp=80;


    }


    if(zhp<=0)
    ex=7777777;
//
    orit=cubespinx;

    //if (mouseleft==1) {
    //
    //}


    Matrix camera={
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      -cubex,-cubey,-cubez,1,
    };


    float c2=cos(cubespiny);
    float s2=sin(cubespiny);

    Matrix m2 ={
      1,0,0,0,
      0,c2,s2,0,
      0,-s2,c2,0,
      0,0,0,1,
    };

    float c=cos(-cubespinx+M_PI/2);
    float s=sin(-cubespinx+M_PI/2);

    Matrix m={
      c,0,s,0,
      0,1,0,0,
      -s,0,c,0,
      0,0,0,1,
    };
    Matrix afatecs={
      1,0,0,0,
      0,1,0,0,
      0,0,0.2,0,
      0,0,0,1,
    };




    Matrix m3=afatecs*m2*m*camera;

    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glDisable(GL_BLEND);
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadMatrixf(m3.e);


//oooooooonnnnnnnnnnnnnnnnnnnaaaaaaaaaaaaaaaaoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo




     fall++;
     if(fall>100)
     fall=0;

     fall2++;
     if(fall2>2)
     fall2=0;

     for(uint32_t i=0;i<n;i++){
          Block bt=blocks[i];

          if(bt.type==LAVA){



           if (cb(blocks,n,bt.x-0.4,bt.y,bt.z)==WATER||
               cb(blocks,n,bt.x+0.4,bt.y,bt.z)==WATER||
               cb(blocks,n,bt.x,bt.y+0.4,bt.z)==WATER||
               cb(blocks,n,bt.x,bt.y-0.4,bt.z)==WATER||
               cb(blocks,n,bt.x,bt.y,bt.z+0.4)==WATER||
               cb(blocks,n,bt.x,bt.y,bt.z-0.4)==WATER){
              blocks[i].type=COBLESTONE;
            }

       }
     }
uint32_t n2=n;
     for(uint32_t i=0;i<n2;i++){
          Block bt=blocks[i];


          if(bt.type==SAND&&fall2==0&&n<maxb){


            Blocki wob=cb(blocks,n,bt.x,bt.y-0.4,bt.z);
            if(wob==CLEAR){
              blocks[i].y-=0.1;

}
else if(wob==WATER||wob==LAVA){
  blocks[i].y-=0.1;
  int index=cubeindex(blocks,n,bt.x,bt.y-0.4,bt.z);
  if (index!=-1) {
    blocks[index]=blocks[--n];
  }

}

}
          if((bt.type==WATER||bt.type==LAVA)&&fall==0&&n<maxb){
            /*int chek=0;
            for (size_t j= 0;j<n2;j++) {
              Block bt2=blocks[j];
              if(bt.x==bt2.x&&bt.z==bt2.z&&fabs(bt.y-0.4-bt2.y)<0.01){
                chek=1;
                 break;
              }
            }      */
            Blocki wob=cb(blocks,n,bt.x,bt.y-0.4,bt.z);
            if(wob==CLEAR){
              blocks[n++]={bt.type,bt.x,bt.y-0.4f,bt.z};

            }
           if (wob!=CLEAR&&wob!=WATER&&wob!=LAVA) {
           if (cb(blocks,n,bt.x-0.4,bt.y,bt.z)==CLEAR) {
              blocks[n++]={bt.type,bt.x-0.4f,bt.y,bt.z};
            }
            if (cb(blocks,n,bt.x,bt.y,bt.z-0.4)==CLEAR) {
               blocks[n++]={bt.type,bt.x,bt.y,bt.z-0.4f};
             }

          if (cb(blocks,n,bt.x+0.4,bt.y,bt.z)==CLEAR) {
             blocks[n++]={bt.type,bt.x+0.4f,bt.y,bt.z};
           }
           if (cb(blocks,n,bt.x,bt.y,bt.z+0.4)==CLEAR) {
              blocks[n++]={bt.type,bt.x,bt.y,bt.z+0.4f};
            }
         }
          }



      }
     for(uint32_t i=0;i<n;i++){
       Block bt=blocks[i];
       if (px2-0.2<bt.x+0.2&&py2-0.4<bt.y+0.2&&pz2-0.2<bt.z+0.2&&px2+0.2>bt.x-0.1&&py2+0.4>bt.y-0.2&&pz2+0.2>bt.z-0.2) {
         if(bt.type!=WATER&&bt.type!=LAVA)
         py2+=0.02;
         if(bt.type==LAVA){
           php-=7;
           Mix_PlayChannel(-1,yum,0);
          }
         }
       }
     for(uint32_t i=0;i<n;i++){
       Block bt=blocks[i];
      //if (px-0.1<bt.x+0.2&&py-0.25<bt.y+0.2&&pz-0.1<bt.z+0.2&&px+0.1>bt.x-0.2&&py+0.25>bt.y-0.2&&pz+0.1>bt.z-0.2) {
         if (px-0.10 < bt.x+0.2 &&
              py-0.25 < bt.y+0.2 &&
              pz-0.10 < bt.z+0.2 &&
              px+0.10 > bt.x-0.2 &&
              py+0.25 > bt.y-0.2 &&
              pz+0.10 > bt.z-0.2)
            {
              if (bt.type == WATER)
                {
                  in_fluid = 1;
                }
              else
                {
                  if (!in_solid)
                    solid_y = bt.y;
                  else if (bt.y > solid_y)
                    solid_y = bt.y;
                  in_solid = 1;
                }
            }
      //   if(bt.type!=WATER&&bt.type!=LAVA)
    //   py+=0.02;
      //   if(bt.type==LAVA){
        //   hp-=7;
          // Mix_PlayChannel(-1,ow,0);
      //   }

  // }
       draw_cube(bt.x,bt.y,bt.z,i*6);

     }
     if (in_solid)
             {
               py += 0.01;
               float max_y = solid_y + 0.4 - 0.0001;
               if (py >= max_y)
                 py = max_y;
             }
           else if (in_fluid)
             {
              // if (input_jump)
                // py+=0.01;
              // else
                 py-=0.008;
             }
           else
             {
               py-=0.01;
             }

     for(uint32_t i=0;i<n;i++){
       Block bt=blocks[i];
       if (ex-0.2<bt.x+0.2&&ey-0.4<bt.y+0.2&&ez-0.2<bt.z+0.2&&ex+0.2>bt.x-0.1&&ey+0.4>bt.y-0.2&&ez+0.2>bt.z-0.2) {
         if(bt.type!=WATER&&bt.type!=LAVA)
         ey+=0.02;
         if(bt.type==LAVA){
           zhp-=7;
           Mix_PlayChannel(-1,k,0);
          }
         }
       }
     uint32_t hover;
     glReadPixels(mousex,mousey,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&hover);
     if ((mouseleft||mouseright||hack||m4)&&hover<n*6) {
       uint32_t index=hover/6;
       uint32_t side=hover%6;
      printf("%x - %u(%u) - %d,%d\n", hover,index,side,mousex,mousey);
      fflush(stdout);
      Block bt=blocks[index];
      if (mouseleft&&bt.type!=LAVA&&bt.type!=WATER) {
        bc+=1;
        mouseleft=0;


        if (blocks[index].type==IORNORE) {
          slot2+=1;
        }
        if (blocks[index].type==OAKLOG) {
          slot4+=1;
        }
        blocks[index]=blocks[--n];
      }

      if (m4&&blocks[index].type==FURNACE&&slot2) {
        m4=0;

        slot3+=1;
        slot2-=1;
      }
      if (m4&&blocks[index].type==FURNACE&&babybackribs) {
        m4=0;

     babybackribs-=1;
     yumy+=1;
      }
      if ((bt.type==LAVA||bt.type==WATER)&&c3>0&&mouseleft) {
        blocks[index]=blocks[--n];
        if (bt.type==WATER) {
          wa+=1;
        }
        if (bt.type==LAVA) {
          la+=1;
        }
      }

      if ((mouseright||hack)&&bc>0&&n<maxb) {
        mouseright=0;
        if (bp!=NAPLANK) {
          bc-=1;
        }
        else if (bp==NAPLANK) {
          c3-=1;
        }



        Block b;
        b.x=blocks[index].x;
        b.y=blocks[index].y;
        b.z=blocks[index].z;




        if (side==0) {
          b.z+=0.4;
        }


        if (side==1) {
          b.z-=0.4;
        }
        if (side==2) {
          b.y-=0.4;
        }


        if (side==3) {
          b.y+=0.4;
        }
        if (side==4) {
          b.x-=0.4;
        }


        if (side==5) {
          b.x+=0.4;
        }




        b.type=bp;

        if(bp==NAPLANK&&cc2>0){
        blocks[n++]=b;
        c2-=1;
      }
      else if(bp==WATER&&wa>0){
       blocks[n++]=b;
       wa-=1;
     }
     else if(bp==LAVA&&wa>0){
      blocks[n++]=b;
      la-=1;
    }
      else if (bp!=NAPLANK&&bp!=WATER&&bp!=LAVA) {
        b.type=bp;
        blocks[n++]=b;
      }

      }


    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glEnable(GL_BLEND);
    glClearColor(0.5,0.5,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    //float c=cos(cubespinx);
    //float s=sin(cubespinx);

    //Matrix m={
    //c,s,0,0,
    //-s,c,0,0,
    //0,0,1,0,
    //0,0,0,1,
    //};










    for(uint32_t i=0;i<n;i++){
      Block bt=blocks[i];


      if( bt.type==COBLESTONE){
   create_entity(cob,{bt.x,bt.y,bt.z},0,m3);
      }
      else if(bt.type==GRASS){
      create_entity(gra,{bt.x,bt.y,bt.z},0,m3);
    }
    else if(bt.type==FURNACE){
    create_entity(f,{bt.x,bt.y,bt.z},0,m3);
  }
  else if(bt.type==IORNORE){
  create_entity(io,{bt.x,bt.y,bt.z},0,m3);

}
else if(bt.type==SAND){
create_entity(ss,{bt.x,bt.y,bt.z},0,m3);
}
else if(bt.type==STONE){
create_entity(ss2,{bt.x,bt.y,bt.z},0,m3);
}

else if(bt.type==OAKLOG){
create_entity(log,{bt.x,bt.y,bt.z},0,m3);
}
else if(bt.type==NAPLANK){
create_entity(wood2,{bt.x,bt.y,bt.z},0,m3);
}
      else {
        glLoadMatrixf(m3.e);
        draw_block(bt.x,bt.y,bt.z,bt.type);
      }
    }

    if(dead==0){
    create_entity(pig,{px2,py2,pz2},pr,m3);
    }
    create_entity(player,{px,py,pz},orit,m3);
    create_entity(eek,{ex,ey,ez},er,m3);

    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);


    drawimage(picx,picy,pic1,pickr,0.9);


    if(inon){
    drawimage(ix,iy,in,0,1);
  }
  if(bc>0&&inon==1){
  drawimage(-0.3,-0.35,block2,0,0.1);
}
if(slot2>0&&inon==1){
drawimage(-0.0,-0.45,ri,0,0.3);
}
if(slot3>0&&inon==1){
drawimage(0.3,-0.45,ii,0,0.3);
}
if(slot4>0&&inon==1){
drawimage(0.6,-0.45,item,0,0.5);
}
if(c1>0&&inon==1){
drawimage(0.6,0.50,wp,0,0.5);
}
if(cc2>0&&inon==1){
drawimage(0.3,0.50,nwp,0,0.3);
}
if(c3>0&&inon==1){
drawimage(0.3,0.100,buc,0,0.3);
}
if(babybackribs>0&&inon==1){
drawimage(0.3,0.40,meat,0,0.3);
}
if(yumy>0&&inon==1){
drawimage(0.3,0.40,meaty,0,0.3);
}

glEnable(GL_DEPTH_TEST);
}
else if (game==0) {
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glEnable(GL_BLEND);
  glClearColor(0.5,0.5,1,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  drawimage(0,0,q,0,1);
  if(gg)
  game=1;
}


    SDL_GL_SwapWindow(window);
  }
  return 0;
}
