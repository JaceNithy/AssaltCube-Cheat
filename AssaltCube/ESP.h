#pragma once

#include <gl\glut.h>
#include "Main.h"

class ESP
{
private:

	bool m_on;

	void line(GLfloat width, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat r, GLfloat g, GLfloat b){
		//Grab the current line width to prevent clashes
		GLfloat glTemp[1];
		glGetFloatv(GL_LINE_WIDTH, glTemp);

		//Set our attributes
		glColor3f(r, g, b);
		glLineWidth(width);

		//Draw our line
		glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();

		//Restore the line width
		glLineWidth(glTemp[0]);
	}

	void BoxLine(GLfloat width, GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLfloat r, GLfloat g, GLfloat b){
		//Grab the current line width to prevent clashes
		GLfloat glTemp[1];
		glGetFloatv(GL_LINE_WIDTH, glTemp);

		//Set our attributes
		glColor3f(r, g, b);
		glLineWidth(width);

		//Draw out box
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + xLen, y);
		glVertex2f(x + xLen, y + yLen);
		glVertex2f(x, y + yLen);
		glEnd();

		//Restore the line width
		glLineWidth(glTemp[0]);
	}

	void Box(GLfloat x, GLfloat y, GLfloat xLen, GLfloat yLen, GLfloat r, GLfloat g, GLfloat b){
		//Set our attributes
		glColor3f(r, g, b);

		//Draw out box
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + xLen, y);
		glVertex2f(x + xLen, y + yLen);
		glVertex2f(x, y + yLen);
		glEnd();
	}


public:

	ESP() : m_on(false) { }

	bool State() { return m_on; }
	void Toggle() { m_on = !m_on; }


	void Draw() {


		for (int i = 0; i < game.GetNumberOfEnemies() ; i++){

			GLfloat tmpColors[3] = { 1.0f, 0.0f, 0.0f }; //Red for all


			Vector3 enemyPos = Vector3(pEnemyList->v_enemyList[i]->v_Position.fX, pEnemyList->v_enemyList[i]->v_Position.fY, pEnemyList->v_enemyList[i]->v_Position.fZ);

			Vector3 vecScreen;


			
			if (game.WorldToScreen(enemyPos, &vecScreen))
			{

				glPushMatrix();

				// Make sure we're drawing in an area of 800x600 (Or whatever resolution the player has)
				glViewport(0, 0, game.iLenX(), game.iLenY());
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0, game.iLenX(), game.iLenY(), 0, -1, 1);
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();


				//3D distance for resizing the ESP
				float dist = game.Get3dDistance(pLocalPlayer->v_Position, pEnemyList->v_enemyList[i]->v_Position);


				//glLineWidth(1.0);

				if (!pEnemyList->v_enemyList[i]->isDead())
				{

					// Draw the healthbar
					float porcentage = (float)pEnemyList->v_enemyList[i]->GetHealth() / 100.0f;
					Box(vecScreen.fX - game.iLenX() / (dist * 2),
						vecScreen.fY,
						porcentage * (game.iLenX() / dist),
						game.iLenX() / (dist * 8),
						0, 0.75, 0);

					Box(vecScreen.fX - game.iLenX() / (dist * 2),
						vecScreen.fY,
						game.iLenX() / dist,
						game.iLenX() / (dist * 8),
						0.5, 0.1, 0.1);


					line(1,
						game.iLenX() / 2,
						game.iLenY(),
						vecScreen.fX,
						vecScreen.fY,
						0.0f, 0.0f, 1.0f);

					//Make our line all pretty
					glEnable(GL_LINE_SMOOTH);

					//Proporção da tela em função da altura (largura/altura)
					float propScrY = game.iLenX() / game.iLenY();

					BoxLine(1,													//Line width
						vecScreen.fX - game.iLenX() / (dist * 2),				//X position
						vecScreen.fY - (game.iLenX() / dist) * 3 * propScrY,	//Y position
						game.iLenX() / dist,									//Box xLen
						(game.iLenX() / dist) * 3 * propScrY,					//Box yLen
						tmpColors[0], tmpColors[1], tmpColors[2]);				//Colors

					glDisable(GL_LINE_SMOOTH);

				}



				glPopMatrix();
			}
		}
	}

};
