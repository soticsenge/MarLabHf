/**
*****************************************************************************
** Kommunikációs mérés - accelerometer.h
** A gyorsulásérzékelö interfésze
*****************************************************************************
*/
#pragma once
#ifndef _ACCELEROMETER_H__
#define _ACCELEROMETER_H__

/** A gyorsulaserzekelo inicializalasa. */
void Accelerometer_Init();

/** Visszaadja az aktualis gyorsulaserzekelo vektort. Siker eseten a visszateresi ertek 0. */
int Accelerometer_GetTilt(int8_t *Xg, int8_t *Yg, int8_t *Zg);

#endif
