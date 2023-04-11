#ifndef __MOTOR_H
#define __MOTOR_H

/* Include des librairies utilisées */

#include <stdio.h>
#include <stdint.h>


/* Déclaration des fonctions */

void motor_set (uint16_t degrees);
void motor_irq_cb(int numMot, int dir);
void motor_reset(void);
void motor_test(void);
void motor_dir(int dir,int n, int ang);

#endif /* __MOTOR_H */
