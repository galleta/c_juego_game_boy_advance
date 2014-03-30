/*****************************************************************************/
/*                                                                           */
/* Francisco Jesús Delgado Almirón                                           */
/* Amelia Armijo Salto                                                       */
/* Antonio Manuel Rodríguez Castro                                           */
/*                                                                           */
/*****************************************************************************/

#include "gba.h"
#include "gba_video.h"
#include "gba_bg.h"
#include "gba_dma.h"
#include "gba_keypad.h"
#include "gba_sprite.h"
#include "gba_irq.h"
#include "gba_bios.h"
#include "gba_rand.h"

/*****************************************************************************/

/* Incluimos el fichero de configuración del juego */
#include "configuracion.h"

/*****************************************************************************/

/*
 * Incluimos los arrays de los fondos y de sus paletas
 */
#include "fondo.raw.c"
#include "fondo.pal.c"
#include "fondo2.raw.c"
#include "fondo2.pal.c"

/*****************************************************************************/

/*
 * Incluimos los arrays de los sprites y de su paleta
 */
#include "luffy_patos.h"

/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Funciones de los patos                                                    */
/*                                                                           */
/*****************************************************************************/

/*
 * Función para mover un pato
 * @param p Sprite del pato
 * @param i Número del pato
 * @see gba_sprite_move
 * @see inicializarPato
 * @see gba_sprite_next_frame
 */
void moverPato(sprite_struct *p, u_short i)
{
	if( direccionpato[i] == 0 ) // pato a la izquierda
	{
		if( p->pos_x > 0 )
		{
			gba_sprite_move(p, -pixeles_mov, 0);
		}
		else
		{
			inicializarPato(p, i);
		}
	}
	else // pato a la derecha
	{
		if( p->pos_x < SCREEN_WIDTH )
		{
			gba_sprite_move(p, pixeles_mov, 0);
		}
		else
		{
			inicializarPato(p, i);
		}
	}
	gba_sprite_next_frame(p);
}

/*****************************************************************************/

/*
 * Función para mover todos los patos
 * @see esperar
 * @see moverPato
 */
void moverPatos()
{
	u_short conta;
	esperar();
    for(conta = 0; conta < CANTIDAD_PATOS; conta++)
    {
		moverPato(&patos[conta], direccionpato[conta]);
	}
}

/*****************************************************************************/

/*
 * Función para inicializar los datos de un pato
 * @param p Sprite del pato
 * @param i Número del pato
 * @see gba_rand
 * @see gba_sprite_set_pos
 * @see gba_sprite_flip_h
 */
void inicializarPato(sprite_struct *p, u_short i)
{	
	if( direccionpato[i] == 0 ) // el pato antes ha ido a la izquierda, luego lo dejo como si fuera a la derecha
	{
		gba_sprite_flip_h(p);
		direccionpato[i] = 1;
	}
	
	u_short alto = 80;
	alto = alto + (gba_rand()%30);
	if( gba_rand()%2 == 0 ) // pato a la izquierda
	{
		gba_sprite_set_pos(p, SCREEN_WIDTH - (int)((*p).width), alto);
		gba_sprite_flip_h(p);
		direccionpato[i] = 0;
	}
	else // pato a la derecha
	{
		gba_sprite_set_pos(p, 0, alto);
		direccionpato[i] = 1;
	}
}

/*****************************************************************************/

/*
 * Función para mostrar un pato
 * @param i Número del pato
 * @see gba_sprite_show
 */
void mostrarPato(u_short numeropato)
{
	gba_sprite_show(&patos[numeropato]);
}

/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Funciones para gestionar los fondos del juego                             */
/*                                                                           */
/*****************************************************************************/

/*
 * Habilita y muestra el fondo para jugar 
 * @see gba_bg_init_palette256
 * @see gba_bg_init_front_buffer
*/
void habilitarFondo()
{
  /* Inicializamos la paleta en la memoria de paleta del fondo */
  gba_bg_init_palette256((void*) fondo_Palette);

  /* Inicializamos la imagen de fondo */
  gba_bg_init_front_buffer((void*) fondo_Bitmap);
}

/*
 * Habilita y muestra el fondo para cuando se pierde la partida 
 * @see gba_bg_init_palette256
 * @see gba_bg_init_front_buffer
*/
void habilitarFondoDerrota()
{
  /* Inicializamos la paleta en la memoria de paleta del fondo */
  gba_bg_init_palette256((void*) fondo2_Palette);

  /* Inicializamos la imagen de fondo */
  gba_bg_init_front_buffer((void*) fondo2_Bitmap);
}

/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Funciones para gestionar los sprites                                      */
/*                                                                           */
/*****************************************************************************/

/*
 * Le hace un flip a los sprites de Luffy 
 * @see gba_sprite_flip_h
*/
void flip_luffy()
{
	gba_sprite_flip_h(&luffy);
	gba_sprite_flip_h(&luffyataque01);  
	gba_sprite_flip_h(&luffyataque02); 
	gba_sprite_flip_h(&luffysalto);  
}

/*****************************************************************************/

/*
 * Habilita los sprites 
 * @see gba_oam_init
 * @see gba_sprite_enable
 * @see gba_sprite_init_palette256
*/
void habilitadSprites()
{
  gba_oam_init();
  gba_sprite_enable();
  /* Copiamos la paleta de los sprites */
  gba_sprite_init_palette256((void*) luffy_patos_pal);
}

/*****************************************************************************/

/*
 * Desabilita los sprites 
 * @see gba_sprite_disable
*/
void desabilitarSprites()
{
	gba_sprite_disable();
}

/*****************************************************************************/

/*
 * Crea todos los sprites necesarios
 * @see gba_sprite_new 
 * @see gba_sprite_set_pos
 * @see gba_sprite_set_frame_seq
 * @see inicializarPato
*/
void crearTodosSprites()
{
  gba_sprite_new(&luffy, SPRITE_SIZE_32x32, SPRITE_COLOR256, (void*) luffy01);
  gba_sprite_new(&luffyataque01, SPRITE_SIZE_64x32, SPRITE_COLOR256, (void*) luffyAtaque101);
  gba_sprite_new(&luffyataque02, SPRITE_SIZE_64x64, SPRITE_COLOR256, (void*) luffyAtaque201);
  gba_sprite_new(&luffysalto, SPRITE_SIZE_32x32, SPRITE_COLOR256, (void*) luffySalto01);
  gba_sprite_new(&patomuerto, SPRITE_SIZE_32x16, SPRITE_COLOR256, (void*) patoAzul04);
  
  u_short i;
  for(i = 0; i < TOQUESPERDER; i++)
  {
	gba_sprite_new(&luffyvida[i], SPRITE_SIZE_16x16, SPRITE_COLOR256, (void*) luffyiconovida);
	gba_sprite_set_pos(&luffyvida[i], i*16, 0);
  }
  
  gba_sprite_set_pos(&luffy,120, 120);
  gba_sprite_set_frame_seq(&luffy, luffy_anim_seq, LUFFY_ANIM_LEN);
  gba_sprite_set_frame_seq(&luffyataque01, luffy_anim_seq_at_01, LUFFY_ANIM_LEN_AT_01);
  gba_sprite_set_frame_seq(&luffyataque02, luffy_anim_seq_at_02, LUFFY_ANIM_LEN_AT_02);
  gba_sprite_set_frame_seq(&luffysalto, luffy_anim_seq_salto, LUFFY_ANIM_LEN_SALTO);
  
  for(i = 0; i < CANTIDAD_PATOS; i++)
  {
	  gba_sprite_new(&patos[i], SPRITE_SIZE_32x16, SPRITE_COLOR256, (void*) patoAzul01);
	  gba_sprite_set_frame_seq(&patos[i], patos_anim_seq, PATOS_ANIM_LEN);
	  direccionpato[i] = 1; // El pato inicialmente va hacia la derecha
	  inicializarPato(&patos[i], i);
  }
}

/*****************************************************************************/

/*
 * Muestra el sprite de Luffy 
 * @see gba_sprite_show
*/
void mostrarLuffy()
{
	gba_sprite_show(&luffy);
}

/*****************************************************************************/

/*
 * Muestra todos los sprites 
 * @see mostrarLuffy
 * @see mostrarPato
*/
void mostrarSprites()
{
  mostrarLuffy();
  mostrarVidas();
  u_short i;
  for(i = 0; i < CANTIDAD_PATOS; i++)
  {
	  mostrarPato(i);
  }
}

/*****************************************************************************/

/*
 * Refresca todos los sprites en la pantalla 
 * @see gba_sprite_update
 * @see gba_oam_update
*/
void refrescarSprites()
{
	gba_sprite_update(&luffy);
	gba_sprite_update(&luffyataque01);
	gba_sprite_update(&luffyataque02);
	gba_sprite_update(&luffysalto);
	gba_sprite_update(&patomuerto);
	u_short i;
	for(i = 0; i < TOQUESPERDER - toques; i++)
	{
		gba_sprite_update(&luffyvida[i]);
	}
	for(i = 0; i < CANTIDAD_PATOS; i++)
	{
		gba_sprite_update(&patos[i]);
	}
	gba_oam_update();
}

/*****************************************************************************/

/*
 * Esconde las vidas que quedan en el juego
 * @see gba_sprite_hide 
*/
void esconderVidas()
{
	u_short i;
	for(i = 0; i < TOQUESPERDER; i++)
	{
		gba_sprite_hide(&luffyvida[i]);
	}
}

/*****************************************************************************/

/*
 * Muestra las vidas que quedan en el juego
 * @see gba_sprite_show 
*/
void mostrarVidas()
{
	u_short i;
	for(i = 0; i <= TOQUESPERDER - toques; i++)
	{
		gba_sprite_show(&luffyvida[i]);
	}
}

/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Funciones para gestionar las colisiones                                   */
/*                                                                           */
/*****************************************************************************/

/*
 * Gestiona las colisiones del juego, devolviendo qué tipo de colisión ha ocurrido
 * Si devuelve 0 no ha habido colisiones
 * Si devuelve 2 ha habido una colisión entre luffy normal o saltando y un pato -> PIERDES SI HAS LLEGADO AL TOPE DE TOQUES
 * Si devuelve 1 ha habido una colisión entre luffy atacado y un pato -> SIGUES JUGANDO 
 * @see gba_sprite_overlap
 * @see gba_sprite_hide
 * @see gba_sprite_set_pos
 * @see inicializarPato
 * @see mostrarPato
*/
int detectarColisiones()
{
	u_short i;
	for(i = 0; i < CANTIDAD_PATOS; i++)
	{
		if( !luffydesabilitado && (gba_sprite_overlap(&luffy, &patos[i]) == TRUE || gba_sprite_overlap(&luffysalta, &patos[i]) == TRUE) )
		{
			toques++;
			esconderVidas();
			mostrarVidas();
			if( toques == TOQUESPERDER )
			{
				return 2;
			}
			else
			{
				gba_sprite_hide(&patos[i]);
				gba_sprite_set_pos(&patos[i], 0, 0);
				inicializarPato(&patos[i], i);
				mostrarPato(i);
				return 0;
			}
		}
		if( luffydesabilitado && (gba_sprite_overlap(&luffyataque01, &patos[i]) == TRUE || gba_sprite_overlap(&luffyataque02, &patos[i]) == TRUE) )
		{
			gba_sprite_hide(&patos[i]);
			gba_sprite_set_pos(&patos[i], 0, 0);
			inicializarPato(&patos[i], i);
			mostrarPato(i);
			return 1;
		}
	}
	return 0;
}

/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Funciones para controlar a Luffy                                          */
/*                                                                           */
/*****************************************************************************/

/*
 * Comprueba el estado de Luffy y lo muestra en consecuencia 
 * @see gba_sprite_hide
 * @see gba_sprite_show
*/
void estadoLuffy()
{
	// Si Luffy no está atacando oculto el sprite del ataque y muestro el de estarse quieto
	if( !luffyataque )
	{
		gba_sprite_hide(&luffyataque01);
		gba_sprite_hide(&luffyataque02);
		gba_sprite_show(&luffy);
		luffydesabilitado = 0;
	}
	// Si Luffy no está saltando oculto el sprite del salto y muestro el de estarse quieto
	if( !luffysalta )
	{
		gba_sprite_hide(&luffysalto);
		gba_sprite_show(&luffy);
		luffydesabilitado = 0;
	}
}

/*****************************************************************************/

/*
 * Comprueba si se ha pulsado alguna tecla y ejecuta la acción correspondiente 
 * @see gba_keypad_pressed
 * @see gba_sprite_hide
 * @see gba_sprite_set_pos
 * @see gba_sprite_move
 * @see gba_sprite_show
 * @see gba_sprite_show
 * @see gba_sprite_next_frame
 * @see gba_sprite_next_frame2
*/
void comprobarTeclas()
{
	// Luffy salta
	// Salí de casa con la sonrisa puesta, hoy me he levantado contengo de verdad ... yo digo saltaaaaaaaaaaaaaaaaaaa
	if (gba_keypad_pressed(KEYPAD_UP))
	{
		if( !luffysalta )
		{
			luffysalta = TRUE;
			gba_sprite_hide(&luffy);
			luffydesabilitado = 1;
			gba_sprite_set_pos(&luffysalto, luffy.pos_x, luffy.pos_y);
			gba_sprite_move(&luffysalto, 0, -pixeles_mov);
			gba_sprite_show(&luffysalto);						
			esperar();
			gba_sprite_move(&luffysalto, 0, -pixeles_mov);	
			gba_sprite_next_frame2(&luffysalto,1);				
			esperar();
			gba_sprite_move(&luffysalto, 0, -pixeles_mov);	
			gba_sprite_next_frame2(&luffysalto,2);				
			esperar();
			gba_sprite_move(&luffysalto, 0, -pixeles_mov);	
			gba_sprite_next_frame2(&luffysalto,1);				
			esperar();	
			gba_sprite_next_frame2(&luffysalto,3);				
			esperar();	
		}
		luffysalta = FALSE;
	}
	
	// Movimiento a la derecha de Luffy
	if (gba_keypad_pressed(KEYPAD_RIGHT))
	{
		luffyataque = FALSE; // Luffy no ataca
		if (luffy_flipped == FALSE)
		{
			flip_luffy();
			luffy_flipped = TRUE;
		}
		// Aquí muevo a Luffy hacia adelante
		if( luffy.pos_x < ( SCREEN_WIDTH - (int)(luffy.width) ) )
		{
			gba_sprite_move(&luffy, pixeles_mov, 0);
		}
		esperar();
		// Ahora me muevo entre los frames 1-8 del sprite que son lo que tienen el movimiento
		siguientepasoLuffy = (luffy.frame_index+1)%8;
		// si da un 0 pongo un 1, ya que el 0 es el frame de estar quieto
		if ( siguientepasoLuffy == 0 ) 
			siguientepasoLuffy = 1;
		gba_sprite_next_frame2(&luffy, siguientepasoLuffy);
	}
	
	// Movimiento a la izquierda de Luffy
	if (gba_keypad_pressed(KEYPAD_LEFT))
	{
		luffyataque = FALSE; // Luffy no ataca
		if (luffy_flipped == TRUE)
		{
			flip_luffy();  
			luffy_flipped = FALSE;
		}
		// Aquí muevo a Luffy hacia adelante
		if( luffy.pos_x > 0 )
		{
			gba_sprite_move(&luffy, -pixeles_mov, 0);
		}
		esperar();
		// Ahora me muevo entre los frames 1-8 del sprite que son lo que tienen el movimiento
		siguientepasoLuffy = (luffy.frame_index+1)%8;
		if ( siguientepasoLuffy == 0 )
			siguientepasoLuffy = 1;
		gba_sprite_next_frame2(&luffy, siguientepasoLuffy);
	}
	
	// Ataque Gomu Gomu no Gatoringu
	if( gba_keypad_pressed(KEYPAD_A) )
	{
		luffyataque = TRUE; // Luffy ataca
		gba_sprite_next_frame2(&luffy, 8);
		esperar();
		gba_sprite_hide(&luffy);
		luffydesabilitado = 1;
		gba_sprite_set_pos(&luffyataque01, luffy.pos_x, luffy.pos_y);
		gba_sprite_show(&luffyataque01);
		gba_sprite_next_frame(&luffyataque01);
		// hacer ataque
		while( gba_keypad_getstatus() == 0 )
		{
			gba_sprite_next_frame(&luffyataque01);
		}
	}
	
	// Ataque Gomu Gomu no Fuusen
	if( gba_keypad_pressed(KEYPAD_B) )
	{
		luffyataque = TRUE; // Luffy ataca
		gba_sprite_next_frame2(&luffy, 10);
		esperar();
		gba_sprite_hide(&luffy);
		luffydesabilitado = 1;
		gba_sprite_set_pos(&luffyataque02, luffy.pos_x, luffy.pos_y);
		gba_sprite_move(&luffyataque02, 0, -32);
		gba_sprite_show(&luffyataque02);
		esperar();
		gba_sprite_next_frame2(&luffyataque02,1);
		esperar();
		gba_sprite_next_frame2(&luffyataque02,2);
		esperar();
		gba_sprite_next_frame2(&luffyataque02,1);
		esperar();
		gba_sprite_next_frame2(&luffyataque02,0);
		esperar();
	}
	
	// Si no hay pulsada ninguna tecla de la game boy Luffy se queda quieto
	if ( !gba_keypad_pressed(KEYPAD_RIGHT | KEYPAD_B | KEYPAD_SELECT | KEYPAD_START | KEYPAD_RIGHT | KEYPAD_LEFT | KEYPAD_UP | KEYPAD_DOWN | KEYPAD_R | KEYPAD_L) )
	{
		luffyataque = FALSE; // Luffy no ataca
		luffysalta = FALSE; // Luffy no salta
		gba_sprite_next_frame2(&luffy, 0); // Muestro el frame de Luffy quieto
	}
}

/*****************************************************************************/
/*                                                                           */
/* Funciones para gestionar el juego                                         */
/*                                                                           */
/*****************************************************************************/

/*
 * Funcion que espera un ciclo de tiempo 
*/
void esperar()
{
	int i;
	for(i = 0; i < 15000; i++);
}

/*****************************************************************************/

/*
 * Manejador para las interrupciones IRQ_VBLANK
 * @see estadoLuffy
 * @see comprobarTeclas
 * @see refrescarSprites
 * @see moverPatos
 * @see detectarColisiones
 * @see desabilitarSprites
 * @see habilitarFondoDerrota
 * @see gba_sound_ds_play
 */
void manejador_vblank()
{
	estadoLuffy();	
	comprobarTeclas();
	refrescarSprites();
	moverPatos();
	
	switch( detectarColisiones() )
	{
		case 0: // nada
			break;
		case 1: // se mata un pato
			patosmatados++;
			break;
		case 2: // pierdes
			cantidadcolisiones++;
			/*if( cantidadcolisiones == 1 )
			{
				gba_sound_ds_play(SOUND_DSA, (void*) golpe, golpelen, golperate);
			}*/
			desabilitarSprites();
			habilitarFondoDerrota();
			break;
	}
}

/*****************************************************************************/

/*
 * Inicializa las variables del juego 
*/
void inicializarVariablesNecesarias()
{
  luffy_flipped = TRUE; // Luffy esta 'flipped' en principio
  luffysalta = FALSE; // Luffy no salta en principio
  luffyataque = FALSE; // Luffy no está atacando en principio
  
  u_short i;
  for(i = 0; i < CANTIDAD_PATOS; i++)
  {
	  direccionpato[i] = 1;
  }
}

/*****************************************************************************/

/*
 * Habilita la interrupción VBLANK 
 * @see gba_irq_enable
 * @see gba_video_enable_int
*/
void habilitarVBLANK()
{
  /* Habilitamos la interrupción VBLANK */
  gba_irq_enable(IRQ_VBLANK, manejador_vblank);
  
  /* Indicamos al controlador de video que genere la interrupción VBLANK */
  gba_video_enable_int(VIDEO_INT_VBLANK);
}

/*****************************************************************************/

/*
 * Habilita el sonido 
 * @see gba_sound_ds_set_output
 * @see gba_sound_ds_set_volume
*/
void habilitarSonido()
{
  /* Sacamos el sonido por las dos líneas de salida */  
  //gba_sound_ds_set_output(SOUND_DSA, SOUND_DS_OUTPUT_TO_BOTH);
  //gba_sound_ds_set_output(SOUND_DSB, SOUND_DS_OUTPUT_TO_BOTH);

  /* Fijamos el volumen al 100% */
  //gba_sound_ds_set_volume(SOUND_DSA, SOUND_DS_OUTPUT_RATIO_100);
  //gba_sound_ds_set_volume(SOUND_DSB, SOUND_DS_OUTPUT_RATIO_100);
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main(void)
{  	
  gba_srand(0);
  
  /* Fijamos el modo de video y el fondo de pantalla */
  gba_bg_set_mode(BG_MODE4);
  gba_bg_show(BG2);

  /* Habilitamos el fondo */
  habilitarFondo();

  /* Inicializo las variables */
  inicializarVariablesNecesarias();

  /* Habilitamos, creamos y mostramos los sprites */
  habilitadSprites();
  crearTodosSprites();
  mostrarSprites();
  
  /* Habilitamos la interrupción VBLANK */
  habilitarVBLANK();

  /* El procesador está siempre en bajo consumo */
  while(1)
  {
    gba_halt();
  }
  
}

/*****************************************************************************/
