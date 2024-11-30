#include "Personaje.h"

Personaje::Personaje(int _coorX, int _coorY, int _AnchoFotog,int _AltoFotog)
{
    coorX = _coorX;
    coorY = _coorY;
    AnchoFotog = _AnchoFotog;
    AltoFotog = _AltoFotog;
}


void Personaje::animacionMov(int columna, int fila)
{
    coorX = AnchoFotog*cont;
    coorY = fila;
    sprite = imgSprites.copy(coorX,coorY,AnchoFotog,AltoFotog);
    setPixmap(sprite);
    cont++;
    if(cont==columna){
        cont = 0;
    }
}
