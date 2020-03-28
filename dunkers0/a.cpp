//DUNKERS 0
//ENJOY!
#if defined(linux) || defined(_WIN32)
#include <GL/glut.h>    /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h>  /*Для Mac OS*/
#endif
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#define height 720
#define width 1366
#define BLUE 0.3, 0.8, 0.8

void reshape(int w, int h)
{
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}
int jmp=0,sd=0,jmp1=0,sd1=0;
float pl1r=1.0,pl1g=0,pl1b=0,pl2r=0,pl2g=1.0,pl2b=0;
void display()
{
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        glColor3f(0.7,0.7,0.7);
        glVertex2i(0, 150);
        glVertex2i(0, 0);
        glVertex2i(width, 0);
        glVertex2i(width, 150);
        glEnd();
        glBegin(GL_QUADS);//pl 1
        glColor3f(pl1r,pl1g,pl1b);
        glVertex2i(0+sd, 150+jmp);
        glVertex2i(0+sd, 300+jmp);
        glVertex2i(100+sd, 300+jmp);
        glVertex2i(100+sd, 150+jmp);
        glEnd();
        glBegin(GL_QUADS);//pl 2
        glColor3f(pl2r,pl2g,pl2b);
        glVertex2i(width-200+sd1, 150+jmp1);
        glVertex2i(width-200+sd1, 300+jmp1);
        glVertex2i(width-100+sd1, 300+jmp1);
        glVertex2i(width-100+sd1, 150+jmp1);
        glEnd();
        glBegin(GL_QUADS);//basket 2
        glColor3f(0.0,0.0,1.0);
        glVertex2i(0, 375);
        glVertex2i(0, 425);
        glVertex2i(50, 425);
        glVertex2i(50, 375);
        glBegin(GL_QUADS);//basket 1
        glColor3f(0.0,0.0,1.0);
        glVertex2i(width-100, 375);
        glVertex2i(width-100, 425);
        glVertex2i(width-150, 425);
        glVertex2i(width-150, 375);
        glEnd();
        glutSwapBuffers();
}
int dlr[2]={0,0},evh[2]={0,0},count[2]={0,0};
void up(int num){
	if(num==1){
		if(!jmp) dlr[0]=1;
	}
	else{
		if(!jmp1) dlr[1]=1;
	}
}
void side(int x,int num){
	if(num ==1){if(((sd>0) || (x>0))&&((sd<width-200)||(x<0))) /*sd+=x*10;*/evh[0]=x;}
	else if(((sd1<0)||(x<0))&&((x>0)||(width-200+sd1>0))) /*sd1+=x*10;*/evh[1]=x;
	glutPostRedisplay(); 
}
int checkWin(){
	if((sd>width-200)&&(jmp>50)) return 1;
	if((width-200+sd1<0)&&(jmp1>50)) return 2;
	return 0;
}
int nosave=0;
void pwon(int k){
	if(nosave==0){
		int fd = open("rez",O_WRONLY);
		lseek(fd,0,SEEK_END);
		if(fd<0){ std::cout<<"cannot open file!\n";exit(1);}
		char buf[15];
		if(k==1)strcpy(buf,"Player 1 Won!\n");
		else strcpy(buf,"Player 2 Won!\n");
		if(write(fd,buf,14)<0){ std::cout<<"error writing file!";exit(2);}
		close(fd);
	}
}
void timef(int value){
	if(!dlr[0]){
		if(jmp>0){ jmp-=10; glutPostRedisplay(); }
	}
	else{
		if(jmp < 90){ jmp+=20; glutPostRedisplay(); }
		else dlr[0]=0;
	}
	if(!dlr[1]){
		if(jmp1>0){ jmp1-=10; glutPostRedisplay(); }
	}
	else{
		if(jmp1 < 90){ jmp1+=20; glutPostRedisplay(); }
		else dlr[1]=0;
	}
	if(evh[0]){
		if(count[0]<4){ count[0]++;sd+=evh[0]*5; glutPostRedisplay(); }
		else{ evh[0]=0;count[0]=0;}
	}
	if(evh[1]){
		if(count[1]<4){ count[1]++;sd1+=evh[1]*5; glutPostRedisplay(); }
		else{ evh[1]=0;count[1]=0;}
	}
	switch(checkWin()){
		case 0:break;
l:		case 1:std::cout<<"Player 1 won!\n";pwon(1);exit(0);break;
		case 2:std::cout<<"Player 2 won!\n";pwon(2);exit(0);break;
	}
	glutTimerFunc (40, timef, 0);
}
void chgclr(float & i){
	if(i>0.9) i = 0.0;
	else i+=0.1;
}
void processNormalKeys(unsigned char key, int x, int y) {
	switch(key){
		case 27: exit(0);//esc
		case 87:case 119: up(1); break;//w
		case 65:case 97: side(-1,1);break;//a
		case 68:case 100: side(1,1);break;//d
		case 73:case 105: up(2); break;//i
		case 74:case 106: side(-1,2);break;//j
		case 76:case 108: side(1,2);break;//l
		case 90:case 122: chgclr(pl1r);break;//z
		case 88:case 120: chgclr(pl1g);break;//x
		case 67:case 99: chgclr(pl1b);break;//c
		case 66:case 98: chgclr(pl2r);break;//b
		case 78:case 110: chgclr(pl2g);break;//n
		case 77:case 109: chgclr(pl2b);break;//m
	}
	glutPostRedisplay(); 
}

int main (int argc, char * argv[])
{
	if(argc>1) nosave=1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(width,height);
	glutCreateWindow("Dunkers 0");
	glutKeyboardFunc(processNormalKeys);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutTimerFunc (40, timef, 0);
//	if(argc>1) {pwon(argv[1][0]-'0');exit(0);} //FOR TESTING 
	glutMainLoop();
	return 0;
}

