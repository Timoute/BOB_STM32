#include <stdint.h>
#include "gpio.h"
#include "xl320.h"

/*! \var static volatile uint16_t pos
 *  \brief Position actuelle du moteur en demi-pas.
 */
static volatile uint16_t pos = 0;

/*! \var static volatile int half_step_counter
 *  \brief Compteur pour le demi-pas du moteur.
 */
static volatile int half_step_counter = 0;

/*! \fn void motor_set(uint16_t degrees)
 *  \brief Définit la position du moteur en degrés.
 *  \param degrees Angle en degrés à définir.
 */
void motor_set (uint16_t degrees){
	pos = (degrees * 2 * 200 * 32) / 360; //x2 car set et reset
}

/*! \fn void motor_dir(int dir,int n, int ang)
 *  \brief Définit la direction et l'angle de rotation du moteur.
 *  \param dir Direction de rotation (1 pour sens horaire, 0 pour sens anti-horaire).
 *  \param n Numéro du moteur.
 *  \param ang Angle de rotation en degrés.
 */
void motor_dir(int dir,int n, int ang){

	switch(n){
	case 5:
//		if(dir == 1){
//			HAL_GPIO_WritePin(Stepper_Dir_4_GPIO_Port,Stepper_Dir_4_Pin,GPIO_PIN_SET);
//		}
//		else if(dir == 0){
//			HAL_GPIO_WritePin(Stepper_Dir_4_GPIO_Port,Stepper_Dir_4_Pin,GPIO_PIN_RESET);
//		}
		break;

	case 4:
		if(dir == 1){
			HAL_GPIO_WritePin(Stepper_Dir_5_GPIO_Port,Stepper_Dir_5_Pin,GPIO_PIN_SET);
		}
		else if(dir == 0){
			HAL_GPIO_WritePin(Stepper_Dir_5_GPIO_Port,Stepper_Dir_5_Pin,GPIO_PIN_RESET);
		}
		break;

	case 6:
		if(dir == 1){
			HAL_GPIO_WritePin(Stepper_Dir_6_GPIO_Port,Stepper_Dir_6_Pin,GPIO_PIN_SET);
		}
		else if(dir == 0){
			HAL_GPIO_WritePin(Stepper_Dir_6_GPIO_Port,Stepper_Dir_6_Pin,GPIO_PIN_RESET);
		}
		break;
	}
	motor_set(ang);
}


/*! \fn void motor_reset(void)
 *  \brief Réinitialise le compteur de demi-pas du moteur.
 */
void motor_reset(void){
	half_step_counter = 0;
}


/*! \fn void motor_irq_cb(int numMot, int dir)
 *  \brief Fonction de rappel d'interruption pour le moteur.
 *  \param numMot Numéro du moteur.
 *  \param dir Direction de rotation du moteur.
 */
void motor_irq_cb(int numMot, int dir){
	switch(numMot){
	case 5:
		if(half_step_counter <= pos){
			if (half_step_counter % 2 == 0){
				HAL_GPIO_WritePin(Setpper_Rot_4_GPIO_Port,Setpper_Rot_4_Pin,GPIO_PIN_SET);

				half_step_counter++;
			}
			else if(half_step_counter % 2 == 1){
				HAL_GPIO_WritePin(Setpper_Rot_4_GPIO_Port,Setpper_Rot_4_Pin,GPIO_PIN_RESET);


				half_step_counter++;
			}
		}
		break;
	case 4:
		//if(HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Top_GPIO_Port,In_Setpper_Capt_Pos_Top_Pin) == 1 || HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Bot_GPIO_Port,In_Setpper_Capt_Pos_Bot_Pin) == 1){
		if(HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Top_GPIO_Port,In_Setpper_Capt_Pos_Top_Pin) == 1 || dir == 1){ // s'arrete grace au capteur de fin de course.
			if(half_step_counter <= pos){
				if (half_step_counter % 2 == 0){
					HAL_GPIO_WritePin(Setpper_Rot_5_GPIO_Port,Setpper_Rot_5_Pin,GPIO_PIN_SET);
					half_step_counter++;
				}
				else if(half_step_counter % 2 == 1){
					HAL_GPIO_WritePin(Setpper_Rot_5_GPIO_Port,Setpper_Rot_5_Pin,GPIO_PIN_RESET);
					half_step_counter++;
				}
			}
		}
		else if(HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Top_GPIO_Port,In_Setpper_Capt_Pos_Top_Pin) == 0 && dir == 0){
			pos = 0;
		}
		break;
	case 6:
		if(half_step_counter <= pos){
			if (half_step_counter % 2 == 0){
				HAL_GPIO_WritePin(Setpper_Rot_6_GPIO_Port,Setpper_Rot_6_Pin,GPIO_PIN_SET);
				half_step_counter++;
			}
			else if(half_step_counter % 2 == 1){
				HAL_GPIO_WritePin(Setpper_Rot_6_GPIO_Port,Setpper_Rot_6_Pin,GPIO_PIN_RESET);
				half_step_counter++;
			}
		}
		break;
	}


}
