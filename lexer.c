/*
    -Analizador Léxico
    -Tarea 1
    -Integrantes: 
    	Derlis Javier Salinas Notario
		Félix Rafael Gómez Meaurio    
*/

#include "lexer.h"
int consumir;			// 1: devolver el sgte componente lexico, 0: debe devolver el actual
char cad[5*TAMLEX];		// mensajes de error
token t;				// recibe componentes del Analizador Léxico
FILE *archivo;			// Fuente.txt
char buff[2*TAMBUFF];	// Buffer
char id[TAMLEX];		
int delantero=-1;		
int fin=0;				
int numLinea=1;			

void error(const char* mensaje)
{
	printf("Linea %d. Error Léxico: %s.\n",numLinea,mensaje);	
}

void sigLex()
{
	int i=0;
	char c=0;
	int acepto=0;
	int estado=0;
	char msg[41];
	entrada e;

	while((c=fgetc(archivo))!=EOF)
	{		
		if (c==' ' || c=='\t')
			continue;	//elimina espacios en blanco
		else if(c=='\n')
		{
			numLinea++; //incrementa el numero de linea
			continue;
		}
		else if (isdigit(c))
		{
				//es número
				i=0;
				acepto=0;
				id[i]=c;
				estado=0;
				while(!acepto)
				{
					switch(estado){
					case 0: 
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=0;
						}
						else if(c=='.'){
							id[++i]=c;
							estado=1;
						}
						else if(tolower(c)=='e'){  //pasa a minúscula e
							id[++i]=c;
							estado=3;
						}
						else{
							estado=6;
						}
						break;
					case 1://viene un punto, debe seguir un digito u otro punto(caso especial)
						c=fgetc(archivo);						
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(c=='.')
						{
							i--;
							fseek(archivo,-1,SEEK_CUR);
							estado=6;
						}
						else{
							sprintf(msg,"No se esperaba el caracter '%c'",c); //cualquier otro caracter
							estado=-1;
						}
						break;
					case 2://la fraccion decimal
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=2;
						}
						else if(tolower(c)=='e')
						{
							id[++i]=c;
							estado=3;
						}
						else
							estado=6;
						break;
					case 3://viene una e, debe seguir +/- o digitos
						c=fgetc(archivo);
						if (c=='+' || c=='-')
						{
							id[++i]=c;
							estado=4;
						}
						else if(isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba el caracter'%c'",c);
							estado=-1;
						}
						break;
					case 4://debe venir al menos un digito
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							sprintf(msg,"No se esperaba el caracter '%c'",c);
							estado=-1;
						}
						break;
					case 5://digitos del exponente
						c=fgetc(archivo);
						if (isdigit(c))
						{
							id[++i]=c;
							estado=5;
						}
						else{
							estado=6;
						}break;
					case 6://estado de aceptación
						if (c!=EOF)
							ungetc(c,archivo);
						else
							c=0;
						id[++i]='\0';
						acepto=1;
						t.pe=buscar(id);
						if (t.pe->compLex==-1)
						{
							strcpy(e.lexema,id);
							e.compLex=LITERAL_NUM;
							insertar(e);
							t.pe=buscar(id);
						}
						t.compLex=LITERAL_NUM;
						break;
					case -1:
						if (c==EOF)
							error("No se esperaba el final de archivo");
						else
							error(msg);
						exit(1);
					}
				}
			break;
		}
		else if (isalpha(c))
		{
			//es un identificador o palabra reservada
			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
				if (i>=TAMLEX)
					error("Tamaño de buffer excedido");
			}while(isalpha(c) || isdigit(c));
			id[i]='\0';
			if (c!=EOF)
				ungetc(c,archivo);
			else
				c=0;
			t.pe=buscar(id);
			t.compLex=t.pe->compLex;
			if (t.pe->compLex==-1)
			{
				strcpy(e.lexema,id);
				e.compLex=LITERAL_CADENA;
				insertar(e);
				t.pe=buscar(id);
				t.compLex=LITERAL_CADENA;
			}
			break;
		}
		else if (c=='{')
		{
			t.compLex=L_LLAVE;
			t.pe=buscar("{");
			break;
		}
		else if (c=='}')
		{
			t.compLex=R_LLAVE;
			t.pe=buscar("}");
			break;
		}
		else if (c=='[')
		{
			t.compLex=L_CORCHETE;
			t.pe=buscar("[");
			break;
		}
		else if (c==']')
		{
			t.compLex=R_CORCHETE;
			t.pe=buscar("]");
			break;
		}
		else if (c==':')
		{
			c=fgetc(archivo);
			if (c==':'){
				t.compLex=DOS_PUNTOS;
				t.pe=buscar(":");
			}
			break;
		}
		else if (c=='\'')
		{//caracter o cadena de caracteres
			i=0;
			id[i]=c;
			i++;
			do{
				c=fgetc(archivo);
				if (c=='\'')
				{
					c=fgetc(archivo);
					if (c=='\'')
					{
						id[i]=c;
						i++;
						id[i]=c;
						i++;
					}
					else
					{
						id[i]='\'';
						i++;
						break;
					}
				}
				else if(c==EOF)
				{
					error("No se esperaba el final del archivo");
				}
				else{
					id[i]=c;
					i++;
				}
			}while(isascii(c));
			id[i]='\0';
			if (c!=EOF)
				ungetc(c,archivo);
			else
				c=0;
			t.pe=buscar(id);
			t.compLex=t.pe->compLex;
			if (t.pe->compLex==-1)
			{
				e.compLex=LITERAL_CADENA;
				insertar(e);
				t.pe=buscar(id);
				t.compLex=e.compLex;
			}
			break;
		}
		else if (c=='\"')
		{
			t.pe=buscar("\"");
			break;
		}
		else if (c==',')
		{
			t.compLex=COMA;
			t.pe=buscar(",");
			break;
		}
		else if (c!=EOF)
		{
			sprintf(msg,"No se esperaba '%c'",c);
			error(msg);
		}
	}
	if (c==EOF)
	{
		t.compLex=EOF;
		sprintf(e.lexema,"EOF");
		t.pe=&e;
	}
	
}

void imprimir(int compLex)
{
	switch(compLex)
	{
		case 301:
			printf("L_CORCHETE");
			break;
		case 302:
			printf("R_CORCHETE");
			break;
		case 303:
			printf("L_LLAVE");
			break;
		case 304:
			printf("R_LLAVE");
			break;
		case 305:
			printf("COMA");
			break;
		case 306:
			printf("DOS_PUNTOS");
			break;
		case 307:
			printf("LITERAL_CADENA");
			break;
		case 308:
			printf("LITERAL_NUM");
			break;
		case 309:
			printf("PR_TRUE");
			break;
		case 310:
			printf("PR_FALSE");
			break;
		case 311:
			printf("PR_NULL");
			break;
		case 312:
			printf("STRING");
			break;
		case 313:
			printf("NUMBER");
			break;
	}
}

int main(int argc,char* args[])
{
	initTabla();
	initTablaSimbolos();
	int contadorLineas = 1;
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("No se ha encontrado el archivo.\n");
			exit(1);
		}
		while (t.compLex!=EOF){
			sigLex();
			imprimir(t.compLex);
			printf("\n");
		}
		fclose(archivo);
	}
	return 0;
}

