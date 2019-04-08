#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define L_CORCHETE		301
#define R_CORCHETE		302
#define L_LLAVE			303
#define R_LLAVE			304
#define COMA			305
#define DOS_PUNTOS		306
#define LITERAL_CADENA	307
#define LITERAL_NUM		308
#define PR_TRUE			309
#define PR_FALSE		310
#define PR_NULL			311
#define STRING 			312
#define NUMBER			313
#define TAMBUFF 	5
#define TAMLEX 		50
#define TAMHASH 	101

typedef struct entrada{
	int compLex;
	char lexema[TAMLEX];	
	struct entrada *tipoDato; 	
} entrada;

typedef struct {
	int compLex;
	entrada *pe;
} token;

/* Prototipos */
void sigLex();
entrada* buscar(const char *clave);
void insertar(entrada e);
void initTabla();
void initTablaSimbolos();

