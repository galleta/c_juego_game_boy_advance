/*****************************************************************************/
/*                                                                           */
/* Configuración básica del juego                                            */
/*                                                                           */
/*****************************************************************************/

/*
 * Sólo incluimos este archivo una vez
 */
#ifndef __CONFIGURACION__
#define __CONFIGURACION__

/*****************************************************************************/

/*
 * Constantes referentes a los sprites
 */
#define LUFFY_ANIM_LEN         12
const short luffy_anim_seq[LUFFY_ANIM_LEN] = {0,1,2,3,4,5,6,7,8,9,10,11};
/*
 * En el frame 0 del sprite está la imagen de Luffy quieto
 * Entre los frames 1 y 8 están las imágenes de Luffy andando 
 * En el frame 9 está la primera imagen del ataque Gomu Gomu no Gatoringu
 * En el frame 10 está la primera imagen del ataque Gomu Gomo no Fuusen y en el 11 la última
*/
#define LUFFY_ANIM_LEN_AT_01   4
const short luffy_anim_seq_at_01[LUFFY_ANIM_LEN_AT_01] = {0,1,2,3};
/*
 * Ataque Gomu Gomu no Gatoringu
*/
#define LUFFY_ANIM_LEN_AT_02   5
const short luffy_anim_seq_at_02[LUFFY_ANIM_LEN_AT_02] = {0,1,2,3,4};
/*
 * Ataque Gomu Gomu no Fuusen
*/
#define LUFFY_ANIM_LEN_SALTO   4
const short luffy_anim_seq_salto[LUFFY_ANIM_LEN_SALTO] = {0,1,2,3};
/*
 * Luffy saltando
*/

#define CANTIDAD_PATOS	3
#define PATOS_ANIM_LEN   3
const short patos_anim_seq[PATOS_ANIM_LEN] = {0,1,2};

/*****************************************************************************/

/*
 * Variables globales
 */
u_short pixeles_mov = 5, cantidadcolisiones = 0, luffydesabilitado = 0, patosmatados = 0, toques = 0;
#define TOQUESPERDER 5
sprite_struct luffy, luffyataque01, luffyataque02, luffysalto, luffyvida[TOQUESPERDER];
sprite_struct patos[CANTIDAD_PATOS], patomuerto;
u_short direccionpato[CANTIDAD_PATOS];
u_short siguientepasoLuffy, luffyataque, luffysalta;
u_short luffy_flipped;
u_short colision = 0;

/*****************************************************************************/

#endif
