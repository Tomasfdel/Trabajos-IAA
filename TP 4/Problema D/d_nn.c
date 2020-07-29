/*
k_nn.c : Clasificador K Nearest Neighbors para features continuos usando distancia máxima.
Formato de datos: c4.5
La clase a predecir tiene que ser un numero comenzando de 0: por ejemplo, para 3 clases, las clases deben ser 0,1,2
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define min(a,b) ((a) <= (b) ? (a) : (b))

//~ Estructura para almacenar un índice de un valor de entrenamiento y la distancia 
//~ que tenga al valor que se quiere clasificar. Usado como estructura auxiliar
//~ para ordenar los valores de entrenamiento en función de la distancia.
typedef struct _IndexAndDist{
	int index;
	float distance;
} IndexAndDist;

struct DATOS {
    char *filename;      /* nombre del archivo para leer los datos */
    int N_IN;             /* N_IN: dimensiones de entrada */
    int N_Class;        /*Total number of classes (outputs)*/    
    float maxDist;      // Distancia dentro de la que se buscan vecinos.
    int PTOT;           /* cantidad TOTAL de patrones en el archivo .data */
    int PR;             /* cantidad de patrones de ENTRENAMIENTO */
                        /* cantidad de patrones de VALIDACION: PTOT - PR*/
    int PTEST;          /* cantidad de patrones de TEST (archivo .test) */
    
    int SEED;       /* semilla para la funcion rand(). Los posibles valores son:*/
                    /* SEED: -1: No mezclar los patrones: usar los primeros PR para entrenar
                                 y el resto para validar.Toma la semilla del rand con el reloj.
                              0: Seleccionar semilla con el reloj, y mezclar los patrones.
                             >0: Usa el numero leido como semilla, y mezcla los patrones. */
    
    IndexAndDist *distances;  // Arreglo para ordenar los valores por distancia a uno específico.
    int *classCounters; // Arreglo de contadores de clases para los k-vecinos.

    /*matrices de datos*/
    float **data;                     /* train data */
    float **test;                     /* test  data */
    int *pred;                        /* salidas predichas ES UNA CLASE*/
    int *seq;      	       	      /* indice de acceso con la sequencia de presentacion de los patrones*/
};

int CONTROL;        /* nivel de verbosity */


/* -------------------------------------------------------------------------- */
/*define_matrix_datos: reserva espacio en memoria para todas las matrices de datos declaradas.
  Todas las dimensiones son leidas del archivo .kn en la funcion arquitec()  */
/* -------------------------------------------------------------------------- */
int define_matrix_datos(struct DATOS *datos){
  int i,max;
    
  datos->seq=(int *)calloc(datos->PTOT,sizeof(int));
  for(i=0;i<datos->PTOT;i++) datos->seq[i]=i;  /* inicializacion del indice de acceso a los datos */
  
  datos->data=(float **)calloc(datos->PTOT,sizeof(float *));
  if(datos->PTEST) datos->test=(float **)calloc(datos->PTEST,sizeof(float *));

  if(datos->PTOT>datos->PTEST) max=datos->PTOT;
  else max=datos->PTEST;
  datos->pred=(int *)calloc(max,sizeof(int));
  
  datos -> distances = (IndexAndDist *) calloc(datos -> PR, sizeof(IndexAndDist));
  datos -> classCounters = (int *) calloc(datos -> N_Class, sizeof(int));

  if(datos->seq==NULL||datos->data==NULL||(datos->PTEST&&datos->test==NULL)||
     datos->pred==NULL || datos -> distances == NULL || datos -> classCounters == NULL) 
		return 1;
  
  for(i=0;i<datos->PTOT;i++){
    datos->data[i]=(float *)calloc(datos->N_IN+1,sizeof(float));
	if(datos->data[i]==NULL) return 1;
  }
  for(i=0;i<datos->PTEST;i++){
    datos->test[i]=(float *)calloc(datos->N_IN+1,sizeof(float));
	if(datos->test[i]==NULL) return 1;
  }

  return 0;
}

/* ---------------------------------------------------------------------------------- */
/*arquitec: Lee el archivo .kn e inicializa el algoritmo en funcion de los valores leidos
  filename es el nombre del archivo .kn (sin la extension) */
/* ---------------------------------------------------------------------------------- */
int arquitec(char *filename,struct DATOS *datos){
  FILE *b;
  char filepat[100];
  /*bandera de error*/
  int error;

  time_t t;

  /*Paso 1:leer el archivo con la configuracion*/
  sprintf(filepat,"%s.kn",filename);
  b=fopen(filepat,"r");
  error=(b==NULL);
  if(error){
    printf("Error al abrir el archivo de parametros\n");
    return 1;
  }

  /* Dimensiones */
  fscanf(b,"%d",&datos->N_IN);
  fscanf(b,"%d",&datos->N_Class);
  
  //~ Distancia en la que se buscan vecinos
  fscanf(b,"%f",&datos->maxDist);

  /* Archivo de patrones: datos para train y para validacion */
  fscanf(b,"%d",&datos->PTOT);
  fscanf(b,"%d",&datos->PR);
  fscanf(b,"%d",&datos->PTEST);

  /* Semilla para la funcion rand()*/
  fscanf(b,"%d",&datos->SEED);

  /* Nivel de verbosity*/
  fscanf(b,"%d",&CONTROL);

  fclose(b);

  /*Paso 2: Definir matrices para datos (e inicializarlos)*/
  error=define_matrix_datos(datos);
  if(error){
    printf("Error en la definicion de matrices de datos\n");
    return 1;
  }
  
  /* Chequear semilla para la funcion rand() */
  if(datos->SEED==0) datos->SEED=time(&t);

  /*Imprimir control por pantalla*/
  printf("\nK Nearest Neighbors usando Distancia:\nCantidad de entradas:%d",datos->N_IN);
  printf("\nCantidad de clases:%d",datos->N_Class);
  printf("\nDistancia en la que se buscan vecinos:%f",datos->maxDist);
  printf("\nArchivo de patrones: %s",filename);
  printf("\nCantidad total de patrones: %d",datos->PTOT);
  printf("\nCantidad de patrones de entrenamiento: %d",datos->PR);
  printf("\nCantidad de patrones de validacion: %d",datos->PTOT-datos->PR);
  printf("\nCantidad de patrones de test: %d",datos->PTEST);
  printf("\nSemilla para la funcion rand(): %d",datos->SEED); 

  return 0;
}
/* -------------------------------------------------------------------------------------- */
/*read_data: lee los datos de los archivos de entrenamiento(.data) y test(.test)
  filenamepasado es el nombre de los archivos (sin extension)
  La cantidad de datos y la estructura de los archivos fue leida en la funcion arquitec()
  Los registros en el archivo pueden estar separados por blancos ( o tab ) o por comas    */
/* -------------------------------------------------------------------------------------- */
int read_data(char *filenamepasado,struct DATOS *datos){

  FILE *fpat;
  char filepat[100];
  float valor;
  int i,k,separador;
  /*bandera de error*/
  int error;

  sprintf(filepat,"%s.data",filenamepasado);
  fpat=fopen(filepat,"r");
  error=(fpat==NULL);
  if(error){
    printf("Error al abrir el archivo de datos\n");
    return 1;
  }

  datos->filename=filenamepasado;
  
  if(CONTROL>1) printf("\n\nDatos de entrenamiento:");

  for(k=0;k<datos->PTOT;k++){
	 if(CONTROL>1) printf("\nP%d:\t",k);
 	 for(i=0;i<datos->N_IN+1;i++){
	   fscanf(fpat,"%f",&valor);
	   datos->data[k][i]=valor;
	   if(CONTROL>1) printf("%f\t",datos->data[k][i]);
	   separador=getc(fpat);
	   if(separador!=',') ungetc(separador,fpat);
  	 }
  }
  fclose(fpat);

  if(!datos->PTEST) return 0;

  sprintf(filepat,"%s.test",filenamepasado);
  fpat=fopen(filepat,"r");
  error=(fpat==NULL);
  if(error){
    printf("Error al abrir el archivo de test\n");
    return 1;
  }

  if(CONTROL>1) printf("\n\nDatos de test:");

  for(k=0;k<datos->PTEST;k++){
	 if(CONTROL>1) printf("\nP%d:\t",k);
         for(i=0;i<datos->N_IN+1;i++){
	   fscanf(fpat,"%f",&valor);
	   datos->test[k][i]=valor;
	   if(CONTROL>1) printf("%f\t",datos->test[k][i]);
	   separador=getc(fpat);
	   if(separador!=',') ungetc(separador,fpat);
         }
  }
  fclose(fpat);

  return 0;

}

/* ------------------------------------------------------------ */
/* shuffle: mezcla el vector seq al azar.
   El vector seq es un indice para acceder a los patrones.
   Los patrones mezclados van desde seq[0] hasta seq[hasta-1]
   Esto permite separar la parte de validacion de la de train   */
/* ------------------------------------------------------------ */
void shuffle(int hasta,struct DATOS *datos){
   float x;
   int tmp;
   int top,select;

   top=hasta-1;
   while (top > 0) {
	x = (float)rand();
	x /= (RAND_MAX+1.0);
	x *= (top+1);
	select = (int)x;
	tmp = datos->seq[top];
	datos->seq[top] = datos->seq[select];
	datos->seq[select] = tmp;
	top --;
   }
  if(CONTROL>3) {printf("End shuffle\n");fflush(NULL);}
}


//~ mapDistances: Llena el arreglo de distancias con el índice y la distancia a
//~ input de cada valor de entrenamiento. Se aprovecha que la función cuadrática
//~ es creciente, por lo que ordenamos por las distancias cuadradas en lugar de su raíz.
void mapDistances(struct DATOS *datos, float *input){
	int i, attribute;
	for(i = 0; i < datos -> PR; i++){
		datos -> distances[i].index = datos -> seq[i];
		datos -> distances[i].distance = 0;
		for(attribute = 0; attribute < datos -> N_IN; attribute++)
			datos -> distances[i].distance += pow(datos -> data[datos -> seq[i]][attribute] - input[attribute], 2);
		
	}
}

//~ compareDistances: Devuelve un orden entre la distancia de dos IndexAndDist.
int compareDistances(const void *_value1, const void *_value2){
	IndexAndDist value1 = * (IndexAndDist *) _value1;
	IndexAndDist value2 = * (IndexAndDist *) _value2;
	
	if(value1.distance < value2.distance)
		return -1;
	if(value1.distance == value2.distance)
		return 0;
	
	return 1;
}

//~ mostPopularClass: Determina la clase más frecuente entre vecinos dentro de cierta distancia.
//~ Se ignora el primer elemento en caso de estar analizando el error de entrenamiento.
//~ En caso empate, se van quitando vecinos desde el más lejano al más cercano hasta 
//~ que haya un ganador.
int mostPopularClass(struct DATOS *datos, int ignore_first){
	int i, start, end, class;
	
	if(ignore_first){
		start = 1;
		end = 2;
	}
	else{
		start = 0;
		end = 1;
	}
	
	//~ Buscamos cuál es el indice máximo en el que tendremos que contar los vecinos, 
	//~ mientras tengamos valores disponibles y los vecinos tengan una distancia menor a la máxima.
	for(; end < datos -> PR && datos -> distances[end].distance <= datos -> maxDist; end++); 
	
	//~ Primero reiniciamos los contadores de las clases.
	for(class = 0; class < datos -> N_Class; class++)
		datos -> classCounters[class] = 0;
	
	//~ Luego vemos cuántos valores pertenecen a cada una.
	for(i = start; i < end; i++){
		class = datos -> data[datos -> distances[i].index][datos -> N_IN];
		datos -> classCounters[class]++;
	}
	
	//~ Y vemos cuál es la clase con más miembros. Mientras haya un empate iremos eliminando miembros
	//~ hasta que una clase sea la elegida.
	int maxAmount, maxIndex, tied = 1, reduction = 0;
	while(tied){
		//~ Suponemos que la primer clase es la que más miembros tiene.
		maxAmount = datos -> classCounters[0];
		maxIndex = 0;
		tied = 0;
		
		//~ Revisamos el resto de las clases para ver si alguien tiene más miembros, o si hay un empate.
		for(class = 1; class < datos -> N_Class; class++){
			if(datos -> classCounters[class] == maxAmount)
				tied = 1;
			if(datos -> classCounters[class] > maxAmount){
				maxAmount = datos -> classCounters[class];
				maxIndex = class;
				tied = 0;
			}
		}
		
		//~ Si terminamos la revisión con un empate, eliminamos de los valores seleccionados al vecino más lejano.
		if(tied){
			reduction++;
			class = datos -> data[datos -> distances[end - reduction].index][datos -> N_IN];
			datos -> classCounters[class]--;
		}
	}
	
	return maxIndex;
}

/* ------------------------------------------------------------------------------ */
/*output: calcula la clase de un vector de entrada *input mediante un voto
  entre sus vecinos dentro de cierta distancia.                                   */
/* ------------------------------------------------------------------------------ */
int output(struct DATOS *datos,float *input, int ignore_first){
	mapDistances(datos, input);
	qsort(datos -> distances, datos -> PR, sizeof(IndexAndDist), compareDistances);  
	return mostPopularClass(datos, ignore_first);
}
/* ------------------------------------------------------------------------------ */
/*evaluar: calcula las clases predichas para un conjunto de datos
  la matriz S tiene que tener el formato adecuado ( definido en arquitec() )
  pat_ini y pat_fin son los extremos a tomar en la matriz
  usar_seq define si se accede a los datos directamente o a travez del indice seq
  los resultados (las propagaciones) se guardan en la matriz seq                  */
/* ------------------------------------------------------------------------------ */
float evaluar(struct DATOS *datos,float **S,int pat_ini,int pat_fin,int usar_seq, int ignore_first){

  float mse=0.0;
  int nu;
  int patron;
  
  for (patron=pat_ini; patron < pat_fin; patron ++) {

   /*nu tiene el numero del patron que se va a presentar*/
    if(usar_seq) nu = datos->seq[patron];
    else         nu = patron;
    
    /*clase para el patron nu*/
    datos->pred[nu]=output(datos,S[nu], ignore_first);

    /*actualizar error*/
    if(S[nu][datos->N_IN]!=(float)datos->pred[nu]) mse+=1.;
  }
    

  mse /= ( (float)(pat_fin-pat_ini));

  if(CONTROL>3) {printf("End prop\n");fflush(NULL);}

  return mse;
}

/* --------------------------------------------------------------------------------------- */
/*clasificar: Elige los valores de entrenamiento
              Clasifica los valores de entrenamiento, validación y test
              Calcula porcentaje de error en entrenamiento, validación y test                             
              Escribe las clasificaciones resultantes a un archivo                         */
/* --------------------------------------------------------------------------------------- */
int clasificar(struct DATOS *datos){

  int i,k;
  int error;
  float train_error, valid_error,test_error;
  FILE *fpredic;
  char filepat[100];

  /*efectuar shuffle inicial de los datos de entrenamiento si SEED != -1*/
  if(datos->SEED>-1){
    srand((unsigned)datos->SEED);    
    shuffle(datos->PTOT,datos);
  }


  train_error = evaluar(datos,datos->data,0,datos->PR,1, 1);
  /* calcular error de validacion */
  if(datos->PR==datos->PTOT) valid_error=0;
  else         valid_error=evaluar(datos,datos->data,datos->PR,datos->PTOT,1, 0);
  /*calcular error de test (si hay)*/
  if (datos->PTEST>0) test_error =evaluar(datos,datos->test,0,datos->PTEST,0, 0);
  else         test_error =0.;
  /*mostrar errores*/
  printf("\nFin del entrenamiento.\n\n");
  printf("Errores:\n");
  printf("Entrenamiento:%f%%\nValidacion:%f%%\nTest:%f%%\n",train_error*100., valid_error*100.,test_error*100.);
  if(CONTROL) fflush(NULL);

  /* archivo de predicciones */
  sprintf(filepat,"%s.predic",datos->filename);
  fpredic=fopen(filepat,"w");
  error=(fpredic==NULL);
  if(error){
    printf("Error al abrir archivo para guardar predicciones\n");
    return 1;
  }
  for(k=0; k < datos->PTEST ; k++){
    for( i = 0 ;i< datos->N_IN;i++) fprintf(fpredic,"%f\t",datos->test[k][i]);
    fprintf(fpredic,"%d\n",datos->pred[k]);
  }
  fclose(fpredic);

  return 0;
}

/* ----------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------- */
int main(int argc, char **argv){

  /*bandera de error*/
  int error;
 
  if(argc!=2){
    printf("Modo de uso: nb <filename>\ndonde filename es el nombre del archivo (sin extension)\n");
    return 0;
  }

  struct DATOS datos;
  
  /* defino la estructura*/
  error=arquitec(argv[1],&datos);
  if(error){
    printf("Error en la definicion del clasificador\n");
    return 1;
  }

  /* leo los datos */
  error=read_data(argv[1],&datos);                 
  if(error){
    printf("Error en la lectura de datos\n");
    return 1;
  }

  /* clasifico los datos*/
  error=clasificar(&datos);                  
  if(error){
    printf("Error en el entrenamiento\n");
    return 1;
  }

  return 0;

}
/* ----------------------------------------------------------------------------------------------------- */

