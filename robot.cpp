// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)
#pragma warning (disable : 4312)


#include <cstring>
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"



#include "mat.h"
#include <FL/gl.h>
#include <cstdlib>

using namespace std;

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1





// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.
enum RobotControls
{ 
    BASE_ROTATION=0, //base controls
    LOWER_ARM_TILT, UPPER_ARM_TILT, CLAW_ROTATION, LOWER_ARM_LENGTH, UPPER_ARM_LENGTH, //arms
    CANNON_TILT, CANNON_LENGTH, CANNON_SIZE, //cannon
    PARTICLE_COUNT, NUMCONTROLS,
};

void ground(float thickness);
void threads(float diameter, float thickness);
void rotation_base();
void lower_arm(float h);
void upper_arm(float h);
void claw(float h);
void y_box(float h);
void unit_box();
Mat4f glGetMatrix(GLenum pname);
Vec3f getWorldPoint(Mat4f matCamXforms);

// To make a RobotArm, we inherit off of ModelerView
class RobotArm : public ModelerView 
{
public:
    RobotArm(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) {}
    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createRobotArm(int x, int y, int w, int h, char *label)
{ 
    return new RobotArm(x,y,w,h,label); 
}

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))


// Utility function.  Use glGetMatrix(GL_MODELVIEW_MATRIX) to retrieve
//  the current ModelView matrix.
Mat4f glGetMatrix(GLenum pname)
{
    GLfloat m[16];
    glGetFloatv(pname, m);
    Mat4f matCam(m[0],  m[1],  m[2],  m[3],
                            m[4],  m[5],  m[6],  m[7],
                            m[8],  m[9],  m[10], m[11],
                            m[12], m[13], m[14], m[15] );

    // because the matrix GL returns is column major...
    return matCam.transpose();
}





// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out RobotArm
void RobotArm::draw()
{
	/* pick up the slider values */

    float r_b = VAL( BASE_ROTATION );
    float r_la = VAL( LOWER_ARM_TILT );
    float r_ua = VAL( UPPER_ARM_TILT );
    float l_la = VAL( LOWER_ARM_LENGTH );
    float l_ua = VAL( UPPER_ARM_LENGTH );
    float r_cw = VAL( CLAW_ROTATION );
    float r_cn = VAL( CANNON_TILT );
    float l_cn = VAL( CANNON_LENGTH );
    float d_cn = VAL( CANNON_SIZE );
	float pc = VAL( PARTICLE_COUNT );

    // This call takes care of a lot of the nasty projection 
    // matrix stuff
    ModelerView::draw();

    // Save the camera transform that was applied by
    // ModelerView::draw() above.
    // While we're at it, save an inverted copy of this matrix.  We'll
    // need it later.
    Mat4f matCam = glGetMatrix( GL_MODELVIEW_MATRIX );
    //Mat4f matCamInverse = matCam.inverse();



	static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

	// define the model

    // floor
	ground(-0.2);

    // threads of vehicle
    threads(0.8, 0.5);

    // skeleton of vehicle
    glTranslatef( 0.0, 0.8, 0.0 );
    glPushMatrix();
        glScalef(2.0f, 0.8f, 2.0f);
        unit_box();                         // platform on top of the threads
    glPopMatrix();

    // rotating surface of vehicle
    glPushMatrix();
        glTranslatef( 0.0, 0.8, 0.0 );
        glRotatef( r_b, 0.0, 1.0, 0.0 );		// turn the whole assembly around the y-axis.
        rotation_base();						// draw the rotation assembly unit
    glPopMatrix();

    // rotating arm -z
    glPushMatrix();

    glPopMatrix();

    // rotating arm +z
    glPushMatrix();

    glPopMatrix();

//    glTranslatef( 0.0, DEPRECATED, 0.0 );			// move to the top of the base
//	glPushMatrix();
//            glTranslatef( 0.5, DEPRECATED, 0.6 );
//	glPopMatrix();
//    glRotatef( r_la, 0.0, 0.0, 1.0 );		// rotate around the z-axis for the lower arm
//	glTranslatef( -0.1, 0.0, 0.4 );
//    lower_arm(l_la);							// draw the lower arm

//    glTranslatef( 0.0, l_la, 0.0 );			// move to the top of the lower arm
//    glRotatef( r_ua, 0.0, 0.0, 1.0 );		// rotate  around z-axis for the upper arm
//    upper_arm(l_ua);							// draw the upper arm

//    glTranslatef( 0.0, l_ua, 0.0 );
//    glRotatef( r_cw, 0.0, 0.0, 1.0 );
//	claw(1.0);




	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}

void ground(float thickness)
{
	glDisable(GL_LIGHTING);
    glColor3f(0.6,0.4,0.2);
	glPushMatrix();
	glScalef(30,0,30);
    y_box(thickness);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void threads(float diameter, float thickness) {
    setDiffuseColor( 0.40, 0.60, 0.40 );
    setAmbientColor( 0.2, 0.2, 0.2 );
	glPushMatrix();
        glPushMatrix();
            glTranslatef(0.0, 0.0, 1.0);
            glScalef(2.0f, diameter, thickness);
            unit_box();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0, diameter / 2.0, 0.75);
            drawCylinder(thickness, diameter / 2.0, diameter / 2.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.0, diameter / 2.0, 0.75);
            drawCylinder(thickness, diameter / 2.0, diameter / 2.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, 0.0, -1.0);
            glScalef(2.0f, diameter, thickness);
            unit_box();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.0, diameter / 2.0, -1.25);
            drawCylinder(thickness, diameter / 2.0, diameter / 2.0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.0, diameter / 2.0, -1.25);
            drawCylinder(thickness, diameter / 2.0, diameter / 2.0);
        glPopMatrix();
	glPopMatrix();
}

void rotation_base() {
    glPushMatrix();

        // the rotating base
        setDiffuseColor( 0.85, 0.75, 0.25 );
        setAmbientColor( 0.95, 0.75, 0.25 );
		glPushMatrix();
            glScalef(1.50, 1.0f, 1.50);
            unit_box();
		glPopMatrix();

        glPushMatrix();
            glTranslatef(0.0, 1.0, 0.0);

            // the console
            setDiffuseColor( 0.15, 0.15, 0.65 );
            setAmbientColor( 0.15, 0.15, 0.65 );
            glPushMatrix();
                glScalef(0.75, 0.5, 0.75);
                unit_box();
            glPopMatrix();

            // the thin pipe
            setDiffuseColor( 0.65, 0.65, 0.65 );
            setAmbientColor( 0.65, 0.65, 0.65 );
            glPushMatrix();
                glTranslatef( 0.5, 0.0, 0.5 );
                glRotatef( -90.0, 1.0, 0.0, 0.0 );
                drawCylinder( 1.0, 0.05, 0.05 );
            glPopMatrix();

            // the thick pipe
            setDiffuseColor( 0.65, 0.65, 0.65 );
            setAmbientColor( 0.65, 0.65, 0.65 );
            glPushMatrix();
                glTranslatef( 0.375, 0.0, -0.6 );
                glRotatef( -90.0, 1.0, 0.0, 0.0 );
                drawCylinder( 0.35, 0.1, 0.1 );
            glPopMatrix();
        glPopMatrix();

        // the cannon
        setDiffuseColor( 0.4, 0.6, 0.4 );
        setAmbientColor( 0.2, 0.2, 0.2 );
        glPushMatrix();
            glTranslatef( -0.75, 0.5, 0.0 );
            glRotatef( -90.0, 0.0, 1.0, 0.0 );
            glPushMatrix();
                drawCylinder( 1.0, 0.25, 0.4 );
            glPopMatrix();
            glPushMatrix();
                glTranslatef( 0.0, 0.0, 1.0 );
                drawCylinder( 0.25, 0.5, 0.5 );
            glPopMatrix();
        glPopMatrix();
	glPopMatrix();
}
void cannon_head(float h){
    setDiffuseColor( 0.0, 0.0, 0.0 );
    setAmbientColor( 0.2, 0.2, 0.2 );
    glPushMatrix();
        glTranslatef( 0.0, h / 2.0, 0.65 );
        drawCylinder( 0.5, h * 1.1 , h * 1.1 );
    glPopMatrix();
}

void lower_arm(float h) {					// draw the lower arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	y_box(h);
}

void upper_arm(float h) {					// draw the upper arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
	glScalef( 1.0, 1.0, 0.7 );
	y_box(h);
	glPopMatrix();
}

void claw(float h) {
	setDiffuseColor( 0.25, 0.25, 0.85 );
	setAmbientColor( 0.25, 0.25, 0.85 );

	glBegin( GL_TRIANGLES );

	glNormal3d( 0.0, 0.0, 1.0);		// +z side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d(-0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);

	glNormal3d( 0.0, 0.0, -1.0);	// -z side
	glVertex3d( 0.5, 0.0, -0.5);
	glVertex3d(-0.5, 0.0, -0.5);
	glVertex3d( 0.5,   h, -0.5);

	glEnd();

	glBegin( GL_QUADS );

	glNormal3d( 1.0,  0.0,  0.0);	// +x side
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);
	glVertex3d( 0.5,   h,-0.5);

	glNormal3d( 0.0,-1.0, 0.0);		// -y side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0, 0.5);

	glEnd();
}

void y_box(float h) {

	glBegin( GL_QUADS );

	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
    glVertex3d( 0.25,0.0, 0.25);
    glVertex3d( 0.25,0.0,-0.25);
    glVertex3d( 0.25,  h,-0.25);
    glVertex3d( 0.25,  h, 0.25);

	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
    glVertex3d( 0.25,0.0,-0.25);
    glVertex3d(-0.25,0.0,-0.25);
    glVertex3d(-0.25,  h,-0.25);
    glVertex3d( 0.25,  h,-0.25);

	glNormal3d(-1.0, 0.0, 0.0);			// -x side
    glVertex3d(-0.25,0.0,-0.25);
    glVertex3d(-0.25,0.0, 0.25);
    glVertex3d(-0.25,  h, 0.25);
    glVertex3d(-0.25,  h,-0.25);

	glNormal3d( 0.0, 0.0, 1.0);			// +z side
    glVertex3d(-0.25,0.0, 0.25);
    glVertex3d( 0.25,0.0, 0.25);
    glVertex3d( 0.25,  h, 0.25);
    glVertex3d(-0.25,  h, 0.25);

    glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
    glVertex3d( 0.25,  h, 0.25);
    glVertex3d( 0.25,  h,-0.25);
    glVertex3d(-0.25,  h,-0.25);
    glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
    glVertex3d( 0.25,0.0, 0.25);
    glVertex3d(-0.25,0.0, 0.25);
    glVertex3d(-0.25,0.0,-0.25);
    glVertex3d( 0.25,0.0,-0.25);

    glEnd();
}

void unit_box() { //origin in the center of -y face

    glBegin( GL_QUADS );

    glNormal3d( 1.0 ,0.0, 0.0);			// +x side
    glVertex3d( 0.5,0.0, 0.5);
    glVertex3d( 0.5,0.0,-0.5);
    glVertex3d( 0.5,1.0,-0.5);
    glVertex3d( 0.5,1.0, 0.5);

    glNormal3d( 0.0 ,0.0, -1.0);		// -z side
    glVertex3d( 0.5,0.0,-0.5);
    glVertex3d(-0.5,0.0,-0.5);
    glVertex3d(-0.5,1.0,-0.5);
    glVertex3d( 0.5,1.0,-0.5);

    glNormal3d(-1.0, 0.0, 0.0);			// -x side
    glVertex3d(-0.5,0.0,-0.5);
    glVertex3d(-0.5,0.0, 0.5);
    glVertex3d(-0.5,1.0, 0.5);
    glVertex3d(-0.5,1.0,-0.5);

    glNormal3d( 0.0, 0.0, 1.0);			// +z side
    glVertex3d(-0.5,0.0, 0.5);
    glVertex3d( 0.5,0.0, 0.5);
    glVertex3d( 0.5,1.0, 0.5);
    glVertex3d(-0.5,1.0, 0.5);

    glNormal3d( 0.0,1.0, 0.0);			// top (+y)
    glVertex3d( 0.5,1.0, 0.5);
    glVertex3d( 0.5,1.0,-0.5);
    glVertex3d(-0.5,1.0,-0.5);
    glVertex3d(-0.5,1.0, 0.5);

    glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
    glVertex3d( 0.5,0.0, 0.5);
    glVertex3d(-0.5,0.0, 0.5);
    glVertex3d(-0.5,0.0,-0.5);
    glVertex3d( 0.5,0.0,-0.5);

    glEnd();
}

int main()
{
    ModelerControl controls[NUMCONTROLS ];

    controls[BASE_ROTATION] = ModelerControl("base rotation (r_b)", -180.0, 180.0, 0.1, 0.0 );
    controls[LOWER_ARM_TILT] = ModelerControl("lower arm tilt (r_la)", 15.0, 95.0, 0.1, 55.0 );
    controls[UPPER_ARM_TILT] = ModelerControl("upper arm tilt (r_ua)", 0.0, 135.0, 0.1, 30.0 );
    controls[CLAW_ROTATION] = ModelerControl("claw rotation (r_cw)", -30.0, 180.0, 0.1, 0.0 );
    controls[LOWER_ARM_LENGTH] = ModelerControl("lower arm length (l_la)", 1, 10.0, 0.1, 3.0 );
    controls[UPPER_ARM_LENGTH] = ModelerControl("upper arm length (l_ua)", 1, 10.0, 0.1, 2.5 );
    controls[CANNON_TILT] = ModelerControl("cannon tilt (r_cn)", 0.0, 5.0, 0.1, 5.0 );
    controls[CANNON_LENGTH] = ModelerControl("cannon length (l_cn)", 0.0, 5.0, 0.1, 5.0 );
    controls[CANNON_SIZE] = ModelerControl("cannon diameter (d_cn)", 0.0, 5.0, 0.1, 5.0 );
    controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0 );
    

	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

    ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
