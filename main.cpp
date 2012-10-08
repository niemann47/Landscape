//
//  main.cpp
//  
//
//  Created by Nick Niemann on 12/13/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//




/////////////////////////////////////////////////////////
////////////  File I/O Meaning //////////////////////////
/*
 -object number (what kind of object)
 -if tree then after would indicate what type of tree
 -translation vector
 -scale vector
 -rotating vector
 -color number (index to array of colors)
*/
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


#include "objects/object.h"
#include <vector>

using namespace std;

int WINDOW_HEIGHT = 700;
int WINDOW_WIDTH = 700;

const char VERTEX_FILE[] = "shaders/vertShader.vert";
const char FRAGMENT_FILE[] = "shaders/fragShader.frag";
string SAVE_FILE_NAME;
char TREE1_OBJ_FILE[] = "models/Tree.obj";
char TREE2_OBJ_FILE[] = "models/tree4.obj";
char TREE3_OBJ_FILE[] = "models/tree3.obj";
char HOUSE_OBJ_FILE[] = "models/besthouse.obj";
char PLANT1_OBJ_FILE[] = "models/house plant3.obj";
char PLANT2_OBJ_FILE[] = "models/plant2.obj";
char PLANT3_OBJ_FILE[] = "models/plant3.obj";
char PLANT4_OBJ_FILE[] = "models/plant4.obj";
char PLANT5_OBJ_FILE[] = "models/plant5.obj";
char CAN_OBJ_FILE[] = "models/wateringcan.obj";
char GROUND_OBJ_FILE[] = "models/ground.obj";
char ROCK_OBJ_FILE[] = "models/rock.obj";
char SKYBOX_OBJ_FILE[] = "models/skybox.obj";
char PICKET_OBJ_FILE[] = "models/picketfence2.obj";
char BENCH_OBJ_FILE[] = "models/bench.obj";
char GRASS_TEXTURE_FILE[] = "textures/grass5.raw";
char CLOUD_TEXTURE_FILE[] = "textures/sky.raw";


const int TREE=1, PLANT = 2, ROCK = 3, CAN = 4, PICKET = 5, HOUSE = 6, BENCH = 7;

int objCycle = -1, greenColorCycle = -1, MAIN_MENU, OBJECT_MENU, 
    COLOR_MENU, TREE_MENU, PLANT_MENU, STEP_MENU, X_POSITION, Y_POSITION, colorIndex=0;

Object *tree[3], *plant[5], *can, *ground, *rock, *skybox, *picketfence, *house, *bench;

material selected, oldMaterial, currentColor, color[9], groundColor, skyboxColor;

GLfloat phi = 90.0, theta = 80; 
int radius= 10000.0;

GLfloat fovy = 45.0, aspect = GLfloat(WINDOW_WIDTH)/GLfloat(WINDOW_HEIGHT),
        zNear = -2000, zFar = 2000, STEP = 50;

GLfloat leftClip = -1000, rightClip = 1000, topClip = 1000, 
    bottomClip = -1000, nearClip = -50000, farClip = 50000;

mat4 projection, view;

bool OBJECT_UP = true, ROTATING = false, HAS_FILE = false, FOLLOW = true;


point4 eye(radius * sin(DegreesToRadians *theta) * cos( DegreesToRadians *phi ), 
           radius * cos( DegreesToRadians *theta ),
           radius * sin(DegreesToRadians *theta) * sin(DegreesToRadians *phi),
           1.0);
 

//point4 eye(radius * cos(phi), 1, radius * sin(phi), 1.0);
//point4 eye(50, 50, 50, 1);

point4 at(0.0,0.0,0.0,0.0);

std::vector<Object*> objectList;











void initShaders() {
    
	//lighting variables
	program1.light_position = point4( 50.0, 500.0, -50.0, 0.0 );
	program1.light_ambient = color4( 0.6, 0.6, 0.6, 1.0 );
	program1.light_diffuse = color4(1.0, 1.0, 1.0, 1.0 );
	program1.light_specular = color4( 1.0, 1.0, 1.0, 1.0 );
    
    
	// Load shaders and use the objecting shader program
    program1.program = InitShader( VERTEX_FILE, FRAGMENT_FILE );
    glUseProgram( program1.program );
    
    //get locations in shaders
    program1.modelLocation = glGetUniformLocation( program1.program , "model" );
    
	program1.viewLocation = glGetUniformLocation( program1.program , "view" );
    
	program1.projectionLocation = glGetUniformLocation( program1.program , "projection" );
    
	program1.vertexLocation = glGetAttribLocation( program1.program , "vertex" );
    
	program1.normalLocation = glGetAttribLocation( program1.program , "normal" );
    
	program1.ambientLocation = glGetUniformLocation(program1.program, "AmbientProduct");
    
	program1.diffuseLocation = glGetUniformLocation(program1.program, "DiffuseProduct");
    
	program1.specularLocation = glGetUniformLocation(program1.program, "SpecularProduct");
    
	program1.lightLocation = glGetUniformLocation(program1.program, "LightPosition");
    
	program1.shininessLocation = glGetUniformLocation(program1.program, "Shininess");
    
	program1.vTextCoordLocation = glGetAttribLocation(program1.program, "vTexCoord");
    
	program1.textureLocation = glGetUniformLocation(program1.program, "texture");
    
    program1.texLocation = glGetUniformLocation(program1.program, "tex");
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
	glOrtho( leftClip , rightClip , bottomClip , topClip , nearClip , farClip );
    //gluPerspective(DegreesToRadians* fovy, aspect, zNear, zFar);
    
	gluLookAt( eye.x, eye.y , eye.z , at.x , at.y , at.z , 0.0 , 1.0 , 0.0 );
    
    
	glEnable(GL_LIGHTING);
    
	glEnable( GL_DEPTH_TEST );
    
    glEnable( GL_CULL_FACE );
    
    glEnable(GL_LINE_SMOOTH);
    
    glEnable(GL_POINT_SMOOTH);
    
    
    
    
    
    //clear the background to white
	glClearColor(1.0, 1.0 , 1.0 , 1.0 );
}


void updateViewMatrix() {
    
    
    eye.x = radius * cos( DegreesToRadians * phi )*sin( DegreesToRadians *theta );
    eye.y = radius * cos( DegreesToRadians *theta );
	eye.z = radius * sin(DegreesToRadians *phi)*sin(DegreesToRadians *theta);
    
	glMatrixMode( GL_MODELVIEW );
    
	glLoadIdentity();
    
	glOrtho( leftClip , rightClip , bottomClip , topClip , nearClip , farClip );
    //gluPerspective(DegreesToRadians* fovy, aspect, zNear, zFar);
    
    
	gluLookAt( eye.x , eye.y , eye.z, at.x , at.y , at.z , 0.0 , 1.0 , 0.0 );
    
    glutPostRedisplay();
    
}


void duplicateObject(point4 pos, Object* item) {
    
    //copy object
    Object* another = new Object(*item); 
    
    //set at the postition
    another->matTranslation[0].w = pos.x;
    another->matTranslation[1].w = pos.y;
    another->matTranslation[2].w = pos.z;
    
    //store object for later
	objectList.push_back( another );
    
    
    objCycle = objectList.size() - 1;
    
    if (FOLLOW) {
        at.x = objectList[objCycle]->matTranslation[0].w;
        at.y = objectList[objCycle]->matTranslation[1].w;
        at.z = objectList[objCycle]->matTranslation[2].w;
        
    }
    
    
    
    
    updateViewMatrix();
    
    
}

void displayCallback() {
    
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    //view = LookAt(eye, at, point4(0.0, 1.0, 0.0, 0.0));
    
    //glUniformMatrix4fv(program1.viewLocation, 1, GL_TRUE, view);
    
    //projection = Perspective(fovy, aspect, zNear, zFar);
    
    //glUniformMatrix4fv(program1.projectionLocation, 1, GL_TRUE, projection);
    
    //draw ground (grass)
    ground->draw(program1);
    
    skybox->draw(program1);
    
    //draw all the shapes
	for( unsigned int i = 0 ; i < objectList.size() ; i++ ) {
        
        
		objectList[i]->draw(program1);
    }
    
	//show the changes
	glutSwapBuffers();
    
}

void mouseButtonHandler(int button, int state, int x, int y) {
    
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //keep track of the values
        X_POSITION = x;
        Y_POSITION = y;
        //tell it that its rotating
        ROTATING = true;
    }
    
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        ROTATING = false;
    
}

void mouseDown(int x, int y) {
    
    if (ROTATING) {
        //get the difference of movement
        int diffX = X_POSITION - x, diffY = Y_POSITION - y;
        
        //keep track of new position
        X_POSITION = x;
        Y_POSITION = y;
        
        //change the angles
        phi -= diffX / 2;
        theta += diffY / 2;
        
        //keep degrees between (-360, 360)
        if (phi >=360)
            phi -= 360;
        else if(phi <= 360) 
            phi += 360;
        
        //only allow degress to be between (2.5, 87.5)
        if (theta >= 87.5) 
            theta = 87.5;
        else if(theta <= 2.5)
            theta = 2.5;
        
        //update the view
        updateViewMatrix();
    }
    
}

void keyboardHandler(unsigned char key , int x , int y ) {
    
    //excape or q
	if (key == 033 || key == 'q') {
		exit(EXIT_SUCCESS);
	}
    
    
    ////////camera//////////
    
    else if (key == 'a' ) {
        at.x -= 100.0;
        updateViewMatrix();
    }
    
    else if (key == 'd' ) {
        at.x +=100.0;
        updateViewMatrix();
    }
    
    else if (key == 'w') {
        at.z -= 100.0;
        
        updateViewMatrix();
    }
    
    else if (key == 's') {
        at.z += 100.0;
        updateViewMatrix();
    }
    
    else if (key == 'z') {
        
        if (rightClip > 75) {
            leftClip += 25;
            rightClip -= 25;
            topClip -= 25;
            bottomClip += 25;
        }
        
        updateViewMatrix();
    }
    
    else if (key == 'Z') {
        leftClip -= 25;
        rightClip += 25;
        topClip += 25;
        bottomClip -= 25;
        updateViewMatrix();
    }
    
    
    
    
    //follow object with cam f
    else if (key == 'f') {
        if (FOLLOW) {
            FOLLOW = false;
            
            
            
        }
        else {
            FOLLOW = true;
            
            if ( objCycle != 1) {
                at.x = objectList[objCycle]->matTranslation[0].w;
                at.y = objectList[objCycle]->matTranslation[1].w;
                at.z = objectList[objCycle]->matTranslation[2].w;
                updateViewMatrix();
            }
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    //delete
    else if (key == 127) {
        if (objCycle != -1) {
            //set the old color back
            //objectList[objCycle]->setColor( oldMaterial );
            
            //delete the item by overwriting
            vector<Object*>::iterator spot = objectList.begin();
            
            for (int i=0; i<objCycle; i++)
                spot++;
            
            objectList.erase(spot);
            
            objCycle = -1;
            
            glutPostRedisplay();
        }
    }
    
    
    //tab
    else if (key == 9) {
        if (objCycle != -1) {
            //set the old color back
            //objectList[objCycle]->setColor( oldMaterial );
            
            //set translation back
            objectList[objCycle]->matTranslation[1][3] = 0;
        }
        
        //increment cycle counter
        objCycle++;
        
        int size = objectList.size();
        
        //check if out of bounds
        if (objCycle>=size) {
            objCycle=0;
        }
        
        if (FOLLOW) {
            at.x = objectList[objCycle]->matTranslation[0].w;
            at.y = objectList[objCycle]->matTranslation[1].w;
            at.z = objectList[objCycle]->matTranslation[2].w;
            
        }
        
        
        
        
        //save old material
        oldMaterial = objectList[objCycle]->colorAttributes;
        
        //set object with highlighted color
        //objectList[objCycle]->setColor( selected );
        
        //redisplay
        updateViewMatrix();
    }
    
    
    //[
    else if (key == '[') {
        if (objCycle != -1) {
            //set the old color back
            //objectList[objCycle]->setColor( oldMaterial );
            
            //set translation back
            objectList[objCycle]->matTranslation[1][3] = 0;
        }
        
        //increment cycle counter
        objCycle--;
        
        int size = objectList.size();
        
        //check if out of bounds
        if (objCycle<=-1) {
            objCycle=size-1;
        }
        
        if (FOLLOW && size > 0) {
            at.x = objectList[objCycle]->matTranslation[0].w;
            at.y = objectList[objCycle]->matTranslation[1].w;
            at.z = objectList[objCycle]->matTranslation[2].w;
            
        }
        
        
        
        
        //save old material
        //oldMaterial = objectList[objCycle]->colorAttributes;
        
        //set object with highlighted color
        //objectList[objCycle]->setColor( selected );
        
        //redisplay
        updateViewMatrix();
    }
    
    
    
    //select object ]
    else if (key == ']') {
        if (objCycle != -1) {
            //set the old color back
            //objectList[objCycle]->setColor( oldMaterial );
            
            //set translation back
            objectList[objCycle]->matTranslation[1][3] = 0;
        }
        
        //increment cycle counter
        objCycle++;
        
        int size = objectList.size();
        
        //check if out of bounds
        if (objCycle>=size) {
            objCycle=0;
        }
        
        if (FOLLOW && size > 0) {
            at.x = objectList[objCycle]->matTranslation[0].w;
            at.y = objectList[objCycle]->matTranslation[1].w;
            at.z = objectList[objCycle]->matTranslation[2].w;
            
        }
        
        
        
        
        //save old material
        //oldMaterial = objectList[objCycle]->colorAttributes;
        
        //set object with highlighted color
        //objectList[objCycle]->setColor( selected );
        
        //redisplay
        updateViewMatrix();
    }
    
    
    //space
    else if (key == 32) {
        
        if (objCycle != -1) {
            //set the old color back
            //objectList[objCycle]->setColor( oldMaterial, colorIndex );
            
            //set translation back
            objectList[objCycle]->matTranslation[1][3] = 0;
            
            //deselect
            objCycle = -1;
            
            glutPostRedisplay();
        }
    }
    
    //rotate left ,
    else if (key == ',') { 
        if (objCycle != -1) {
            objectList[objCycle]->rotateY(-15);
            glutPostRedisplay();
        }
    }
    
    //rotate right .
    else if (key == '.') { 
        if (objCycle != -1) {
            objectList[objCycle]->rotateY(15);
            glutPostRedisplay();
        }
    }
    
    //-
    else if (key == '-') {
        if (objCycle != -1) {
            if (objectList[objCycle]->matScale[0].x > 0.3) {
                objectList[objCycle]->scale(-.2);
            }
            
            glutPostRedisplay();
        }
    }
    
    //=
    else if (key == '=') {
        if (objCycle != -1) {
            objectList[objCycle]->scale(.2);
            glutPostRedisplay();
        }
    }
    
    
    
    
    
    
    //t or T
    else if (key=='t' || key=='T') {
        
        //make tree set properties
        duplicateObject(point4(0.0, 0.0, 0.0, 0.0), tree[0]);
    }
    
    //p or P
    else if (key=='p' || key=='P') {
        
        //make plant set properties
        duplicateObject(point4(0.0, 0.0, 0.0, 0.0), plant[0]);
    }
    
    //c or C
    else if (key=='c' || key=='C') {
        
        //make can set properties
        duplicateObject(point4(0.0, 0.0, 0.0, 0.0), can);
    }
    
    //duplicate rock
    else if (key=='r' || key=='R') {
        
        //make rock set properties
        duplicateObject(point4(0.0, 0.0, 0.0, 0.0), rock);
    }
}

void specialHandler( int key , int x , int y ) {
	
	if( key == GLUT_KEY_UP ) {
        
        //move object up
        if (objCycle != -1) {
            objectList[objCycle]->matTranslation[2][3] -=STEP;
            
            if (FOLLOW) {
                at.z = objectList[objCycle]->matTranslation[2].w;
                
            }
            
            updateViewMatrix();
        }
    }
    
	if( key == GLUT_KEY_DOWN ) {
        
        //move object down
        if (objCycle != -1) {
            objectList[objCycle]->matTranslation[2][3] +=STEP;
            
            if (FOLLOW) {
                at.z = objectList[objCycle]->matTranslation[2].w;
                
            }
            
            updateViewMatrix();
        }
    }
    
	if( key == GLUT_KEY_LEFT ) {
    
        //move object left
        if (objCycle != -1) {
            objectList[objCycle]->matTranslation[0][3] -=STEP;
            
            if (FOLLOW) {
                at.x = objectList[objCycle]->matTranslation[0].w;
                
            }
            
            updateViewMatrix();
        }
    }
    
	if( key == GLUT_KEY_RIGHT ) {
        
        //move object right
        if (objCycle != -1) {
            objectList[objCycle]->matTranslation[0][3] +=STEP;
            
            if (FOLLOW) {
                at.x = objectList[objCycle]->matTranslation[0].w;                
            }
            
            updateViewMatrix();
        }
    }
}

void selectedTimer(int ignore) {
    
    if (objCycle != -1) {
        if (objectList[objCycle]->matTranslation[1][3] >= 6.0) {
            OBJECT_UP = false;
        }
        else if (objectList[objCycle]->matTranslation[1][3] <=0.0) {
            OBJECT_UP = true;
        }
        
        if (OBJECT_UP) {
            objectList[objCycle]->matTranslation[1][3] += 0.5;
        }
        
        else   
            objectList[objCycle]->matTranslation[1][3] -= 0.5;
        
        glutPostRedisplay();
    }
    
    glutTimerFunc(15, selectedTimer, 0);
}





void readVertices( char* filename , Object* object ) {
    
	objLoader* loader = new objLoader;
    
	loader->load( filename );
    
	object->setFaces( loader );
    
	delete loader;
    
}



void loadTexture(material &tex, char filename[], int size) {
    
    tex.texturized = true;
    tex.texSize = size;
    
    // allocate buffer
    tex.data = new unsigned char[ size * size * 3 ];
    
    
    //char filename[] = {"textures/grass.raw"};
    
    FILE * file;
    
    // open and read texture data
    file = fopen( filename, "rb" );
    fread( tex.data, size * size * 3, 1, file );
    fclose( file );
    
    
}


void loadObject(Object *&item, char file[], material col, int colIndex, point4 trans, GLfloat scalor) {
    readVertices( file , item );
    
    item->scale(scalor);
    
    
    
    item->matTranslation[0].w = trans.x;
    item->matTranslation[1].w = trans.y;
    item->matTranslation[2].w = trans.z;
    
    item->setColor( col, colIndex );
    
    item->init(program1);
    
	//objectList.push_back( item );
    
    
}

void initObjects() {
    
    //set selected color
	selected.material_ambient = color4(1.0, 0.0, 0.0, 1.0);
	selected.material_diffuse = color4(1.0, 0.0, 0.0, 1.0);
	selected.material_specular = color4(1.0, 1.0, 1.0, 1.0);
	selected.material_shininess = 100.0;
    
    //dark green color stuff
	color[0].material_ambient = color4(0.0, 0.1, 0.0, 1.0);
	color[0].material_diffuse = color4(0.0, 0.3, 0.0, 1.0);
	color[0].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[0].material_shininess = 100.0;
    
    //green color stuff
	color[1].material_ambient = color4(0.0, 0.3, 0.0, 1.0);
	color[1].material_diffuse = color4(0.0, 0.5, 0.0, 1.0);
	color[1].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[1].material_shininess = 100.0;
    
    //light green color stuff
	color[2].material_ambient = color4(0.0, 0.7, 0.0, 1.0);
	color[2].material_diffuse = color4(0.0, 0.9, 0.0, 1.0);
	color[2].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[2].material_shininess = 100.0;
    
    //yellow color stuff
	color[3].material_ambient = color4(0.5, 0.5, 0.0, 1.0);
	color[3].material_diffuse = color4(0.7, 0.7, 0.0, 1.0);
	color[3].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[3].material_shininess = 100.0;
    
    //orange color stuff
	color[4].material_ambient = color4(0.5, 0.1, 0.0, 1.0);
	color[4].material_diffuse = color4(0.7, 0.2, 0.0, 1.0);
	color[4].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[4].material_shininess = 100.0;
    
    //tree color stuff
	color[5].material_ambient = color4(0.4, 0.2, 0.0, 1.0);
	color[5].material_diffuse = color4(0.8, 0.4, 0.0, 1.0);
	color[5].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[5].material_shininess = 75.0;
    
    //brown color stuff
	color[6].material_ambient = color4(0.5, 0.3, 0.1, 1.0);
	color[6].material_diffuse = color4(0.7, 0.4, 0.2, 1.0);
	color[6].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[6].material_shininess = 100.0;
    
    //rock color stuff
	color[7].material_ambient = color4(0.2, 0.2, 0.2, 1.0);
	color[7].material_diffuse = color4(0.4, 0.4, 0.4, 1.0);
	color[7].material_specular = color4(0.8, 0.8, 0.8, 1.0);
	color[7].material_shininess = 150.0;
    
    //picketfence color
    color[8].material_ambient = color4(0.9, 0.9, 0.9, 1.0);
	color[8].material_diffuse = color4(0.9, 0.9, 0.9, 1.0);
	color[8].material_specular = color4(0.6, 0.6, 0.6, 1.0);
	color[8].material_shininess = 150.0;
    
    
    currentColor = color[0];
    
    
    //sky color stuff
	skyboxColor.material_ambient = color4(0.0, 0.6, 0.6, 1.0);
	skyboxColor.material_diffuse = color4(0.0, 0.8, 0.8, 1.0);
	skyboxColor.material_specular = color4(0.7, 0.7, 0.7, 1.0);
	skyboxColor.material_shininess = 150.0;
    loadTexture(skyboxColor, CLOUD_TEXTURE_FILE, 1200);
    
    //ground color stuff
	groundColor.material_ambient = color4(0.0, 0.4, 0.0, 1.0);
	groundColor.material_diffuse = color4(0.0, 0.8, 0.0, 1.0);
	groundColor.material_specular = color4(0.8, 0.8, 0.8, 1.0);
	groundColor.material_shininess = 100.0;
    loadTexture(groundColor, GRASS_TEXTURE_FILE, 1600);
    
    
    //parameter indicated that there is a texture
	ground = new Object();
    
    cout << "Loading ground...          ";
    cout.flush();
    
    loadObject(ground, GROUND_OBJ_FILE, groundColor, 0, vec4(0.0,0.0,0.0,0.0), 1000);
    
    
    
    cout << "DONE" << endl;
    
    
    
    
    
    
    //parameter indicated that there is a texture
	skybox = new Object();
    
    cout << "Loading sky...             ";
    cout.flush();
    
    loadObject(skybox, SKYBOX_OBJ_FILE, skyboxColor, 0, vec4(0.0,100.0,0.0,0.0), 100);
    
    cout << "DONE" << endl;
    
    
    
    
    
    
	//parameter indicated that there is a texture
	tree[0] = new Object(false, 1, 0);
    
    cout << "Loading tree1...           ";
    cout.flush();
    loadObject(tree[0], TREE1_OBJ_FILE, color[3], 3, vec4(200,0.0,0.0,0.0), 5);
    cout << "DONE" << endl;
    
    objectList.push_back( tree[0] );
    
    //keep track of old material (ie. first color pushed in)
    oldMaterial = color[3];
    
    
    
    //parameter indicated that there is a texture
	tree[1] = new Object(false, 1, 1);
    
    cout << "Loading tree2...           ";
    cout.flush();
    
    loadObject(tree[1], TREE2_OBJ_FILE, color[1], 1, vec4(200,0.0,500,0.0), 15);
    cout << "DONE" << endl;
    
    objectList.push_back( tree[1] );
    
    
    //parameter indicated that there is a texture
	tree[2] = new Object(false, 1, 2);
    
    cout << "Loading tree3...           ";
    cout.flush();
    
    loadObject(tree[2], TREE3_OBJ_FILE, color[2], 2, vec4(800,0.0, 200, 0.0), 10);
    cout << "DONE" << endl;
    
    objectList.push_back( tree[2] );
    
    
    
    
    
    
    plant[0] = new Object(false, 2, 0);
    
    cout << "Loading plant...           ";
    cout.flush();
    
    loadObject(plant[0], PLANT1_OBJ_FILE, color[2], 2, vec4(-200, 0.0, 0.0, 0.0), 0);
    cout << "DONE" << endl;
    objectList.push_back( plant[0] );
    
    
    plant[1] = new Object(false, 2, 1);
    
    cout << "Loading plant...           ";
    cout.flush();
    
    loadObject(plant[1], PLANT2_OBJ_FILE, color[2], 2, vec4(-500, 0.0, -800.0, 0.0), -0.6 );
    cout << "DONE" << endl;
    objectList.push_back( plant[1] );
    
    
    
    
    
    plant[2] = new Object(false, 2, 2);
    
    cout << "Loading plant...           ";
    cout.flush();
    
    loadObject(plant[2], PLANT3_OBJ_FILE, color[2], 2, vec4(980, 0.0, 800.0, 0.0), 1 );
    cout << "DONE" << endl;
    objectList.push_back( plant[2] );
    
    
    
    
    
    
    plant[3] = new Object(false, 2, 3);
    
    cout << "Loading plant...           ";
    cout.flush();
    
    loadObject(plant[3], PLANT4_OBJ_FILE, color[2], 2, vec4(250, 0.0, 725, 0.0), 1 );
    cout << "DONE" << endl;
    objectList.push_back( plant[3] );
    
    
    
    plant[4] = new Object(false, 2, 4);
    
    cout << "Loading plant...           ";
    cout.flush();
    
    loadObject(plant[4], PLANT5_OBJ_FILE, color[3], 3, vec4(-500, 0.0, 250, 0.0), 5 );
    cout << "DONE" << endl;
    objectList.push_back( plant[4] );
    
    
    
    //duplicateObject(point4(200, 0.0, 200, 0.0), plant[0]);
    

    rock = new Object(false, 3);
    
    cout << "Loading rock...            ";
    cout.flush();
    
    
    loadObject(rock, ROCK_OBJ_FILE, color[7], 7, vec4(100, 0.0, -200, 0.0), -0.7);
    
    cout << "DONE" << endl;
    
    objectList.push_back( rock );
    
    
    
    can = new Object(false, 4);
    
    cout << "Loading can...             ";
    cout.flush();
    
    loadObject(can, CAN_OBJ_FILE, color[7], 7, vec4(100, 0.0, -80, 0.0), 0.2);
    
    cout << "DONE" << endl;
    
    objectList.push_back( can );
    
    
    
    picketfence = new Object(false, 5);
    
    cout << "Loading picket...          ";
    cout.flush();
    
    loadObject(picketfence, PICKET_OBJ_FILE, color[8], 8, vec4(0.0,0.0,-80,0.0), 1);
    
    cout << "DONE" << endl;
    
    objectList.push_back( picketfence );
    
    
    //parameter indicated that there is a texture
	house = new Object(false, 6);
    
    cout << "Loading house...           ";
    cout.flush();
    
    loadObject(house, HOUSE_OBJ_FILE, color[8], 8, vec4(5000,0.0, 5000, 0.0), 10);
    cout << "DONE" << endl;
    
    objectList.push_back( house );
    
    
    
    bench = new Object(false, 7);
    
    cout << "Loading bench...           ";
    cout.flush();
    
    
    loadObject(bench, BENCH_OBJ_FILE, color[8], 8, vec4(1000, 0.0, -2000, 0.0), 5);
    
    cout << "DONE" << endl;
    
    objectList.push_back( bench );
    
}

void saveFile(string file) {
    ofstream fout;
    
    fout.open(file.c_str());
    
    //write objects here
    
    int size = objectList.size();
    
    //output the size
    fout << size << endl;
    
    for (int i=0; i<size; i++) {
        //type of object
        fout << objectList[i]->type << ' ';
        
        //if its a tree or plant output what kind
        if (objectList[i]->type == 1 || objectList[i]->type == 2) {
            fout << objectList[i]->treeIndex << ' ';
        }
        
        //translation. 2 floats (x z) y should be static
        fout << objectList[i]->matTranslation[0].w << ' ';
        fout << objectList[i]->matTranslation[2].w << ' ';
        
        //scale 1 float
        fout << objectList[i]->matScale[0].x << ' ';
        
        //rotation 1 float
        fout << objectList[i]->angle << ' ';
        
        //color
        fout << objectList[i]->materialColor << endl;
        
    }
    
    
    fout.close();
}

void loadFile(string file) {
    
    ifstream fin;
    
    int num, next;
    GLfloat floatNext;
    
    fin.open(file.c_str());
    
    if (fin.good()) {
        fin >> num;
        
        for (int i=0; i<num; i++) {
            fin >> next;
            
            if (next == TREE) {
                fin >> next;
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), tree[next]);
            }
            else if(next == PLANT) {
                fin >> next;
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), plant[next]);
            }
            else if(next == ROCK) {
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), rock);
            }
            else if(next == CAN) {
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), can);
            }
            else if(next == PICKET) {
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), picketfence);
            }
            else if(next == HOUSE) {
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), house);
            }
            else if(next == BENCH) {
                duplicateObject(point4(0.0, 0.0, 0.0, 0.0), bench);
            }
             
            //set the translation
            fin >> floatNext;
            objectList[objectList.size()-1]->matTranslation[0].w = floatNext;
            
            fin >> floatNext;
            objectList[objectList.size()-1]->matTranslation[2].w = floatNext;
            
            //set the scale
            fin >> floatNext;
            objectList[objectList.size()-1]->setScale(floatNext);
            
            //set the rotation
            fin >> floatNext;
            objectList[objectList.size()-1]->angle = floatNext;
            
            //set the color
            fin >> next;
            objectList[objectList.size()-1]->setColor(color[next], next);
            
            
        }
    }
    
    fin.close();
    
    glutPostRedisplay();
}

void clear() {
    objCycle = -1;
    objectList.clear();
    glutPostRedisplay();
}


void colorMenu( int value ) {
    
    currentColor = color[value];
    colorIndex = value;
    
    if (objCycle != -1) {
        objectList[objCycle]->setColor(currentColor, colorIndex);
    }
}

void objectMenu(int value) {
    
    switch (value) {
        case 2:
            
            //make plant set properties
            duplicateObject(point4(0.0, 0.0, 0.0, 0.0), rock);
            
            objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
            break;
            
        case 3:
            //make plant set properties
            duplicateObject(point4(0.0, 0.0, 0.0, 0.0), picketfence);
            
            objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
            break;
            
        case 4:
            //make plant set properties
            duplicateObject(point4(0.0, 0.0, 0.0, 0.0), bench);
            
            objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
            break;
            
        case 5:
            //make plant set properties
            duplicateObject(point4(0.0, 0.0, 0.0, 0.0), can);
            
            objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
            break;
    }
}

void treeMenu ( int value ) {
    
    
    duplicateObject(point4(0.0, 0.0, 0.0, 0.0), tree[value]);
    
    objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
}

void plantMenu ( int value ) {
    
    
    duplicateObject(point4(0.0, 0.0, 0.0, 0.0), plant[value]);
    
    objectList[objectList.size()-1]->setColor(currentColor, colorIndex);
}

void stepMenu(int value) {
    
    switch (value) {
        case 0:
            STEP = 5;
            break;
            
        case 1:
            STEP = 50;
            break;
            
        case 2:
            STEP = 500;
            break;
    }
    
}

void menu( int value ) {
    
    switch (value) {
        //zoom in
        case 1:
            leftClip += 25;
            rightClip -= 25;
            topClip -= 25;
            bottomClip += 25;
            updateViewMatrix();
            break;
            
        //zoom out
        case 2: 
            leftClip -= 25;
            rightClip += 25;
            topClip += 25;
            bottomClip -= 25;
            updateViewMatrix();
            break;
            
        case 3:
            if (objCycle != -1) {
                
                //delete the item
                vector<Object*>::iterator spot = objectList.begin();
                
                for (int i=0; i<objCycle; i++)
                    spot++;
                
                objectList.erase(spot);
                
                objCycle = -1;
                
                glutPostRedisplay();
            }            
            break;
            
        //clear
        case 4:
            clear();
            break;
            
        //load saved file
        case 5:
            cout << "Enter filename: ";
            cin >> SAVE_FILE_NAME;
            HAS_FILE = true;
            
            clear();
            
            //load the file here
            loadFile(SAVE_FILE_NAME);
            break;
            
        //save file
        case 6:
            //check if there is a filename already
            if (!HAS_FILE) {
                cout << "Enter filename: ";
                cin >> SAVE_FILE_NAME;
                HAS_FILE = true;
            }
            //save to file
            saveFile(SAVE_FILE_NAME);
            break;
            
        //save file as
        case 7:
            //get the filename
            cout << "Enter filename: ";
            cin >> SAVE_FILE_NAME;
            HAS_FILE = true;
            //save to file
            saveFile(SAVE_FILE_NAME);
            break;
            
        case 9:
            exit(EXIT_SUCCESS);
            break;
    }
    
}


void initWindow(int argc, char **argv) {
    
    
	glutInit(&argc, argv);
    
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    
	glutInitWindowPosition(0, 0);
    
	glutCreateWindow("Landscape Design");
    
    
    
	glutDisplayFunc( displayCallback );
    
    
    
	//glutSetCursor( GL_NONE );
    
    
	glutMouseFunc(mouseButtonHandler);
    
    glutMotionFunc( mouseDown );
    
	//glutPassiveMotionFunc(mouseMoveHandler);
    
	//glewInit();
    
	glutKeyboardFunc( keyboardHandler );
    
	glutSpecialFunc( specialHandler );
    
	//glutSpecialUpFunc( specialUpHandler );
    
    
    glutTimerFunc( 15, selectedTimer, 0);
    
    
    
    //sub menu color
    COLOR_MENU = glutCreateMenu( colorMenu );
    glutAddMenuEntry("Dark Green", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Light Green", 2);
    glutAddMenuEntry("Yellow", 3);
    glutAddMenuEntry("Red", 4);
    glutAddMenuEntry("Orange", 5);
    glutAddMenuEntry("Brown", 6);
    glutAddMenuEntry("Gray", 7);
    glutAddMenuEntry("White", 8);
    
    
    //sub menu tree
    TREE_MENU = glutCreateMenu( treeMenu );
    glutAddMenuEntry("Oak", 0);
    glutAddMenuEntry("Pine", 1);
    glutAddMenuEntry("Palm", 2);
    
    //sub menu plant
    PLANT_MENU = glutCreateMenu( plantMenu );
    glutAddMenuEntry("plant1", 0);
    glutAddMenuEntry("plant2", 1);
    glutAddMenuEntry("plant3", 2);
    glutAddMenuEntry("plant4", 3);
    glutAddMenuEntry("Sunflowers", 4);
    
    //sub menu object
    OBJECT_MENU = glutCreateMenu( objectMenu );
    glutAddSubMenu("Tree", TREE_MENU);
    glutAddSubMenu("Plant", PLANT_MENU);
    glutAddMenuEntry("Rock", 2);
    glutAddMenuEntry("Picket Fence", 3);
    glutAddMenuEntry("Bench", 4);
    glutAddMenuEntry("Watering Can", 5);
    
    //sub menu step
    STEP_MENU = glutCreateMenu(stepMenu);
    glutAddMenuEntry("Small", 0);
    glutAddMenuEntry("Medium", 1);
    glutAddMenuEntry("Large", 2);
    
    
    //menu
    MAIN_MENU = glutCreateMenu( menu );
    glutAddSubMenu("Place Object", OBJECT_MENU );
    glutAddSubMenu("Color", COLOR_MENU);
    glutAddMenuEntry("Zoom In (z)", 1);
    glutAddMenuEntry("Zoom Out (Z)", 2);
    glutAddSubMenu("Set Translation Step", STEP_MENU);
    glutAddMenuEntry("Delete Item", 3);
    glutAddMenuEntry("Clear", 4);
    glutAddMenuEntry("Load", 5);
    glutAddMenuEntry("Save", 6);
    glutAddMenuEntry("Save As", 7);
    glutAddMenuEntry("Quit", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    
}


int main( int argc, char **argv ) {
    
	srand( time( NULL ) );
    
	initWindow(argc, argv);
    
	//initSound();
    
	initShaders();
    
	//initView();
    
	initObjects();
    
	//initMenu();
    
	glutMainLoop();
    
	return 0;
}
