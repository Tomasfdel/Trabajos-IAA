/*
nb_n.c : Clasificador Naive Bayes usando la aproximacion de funciones normales para features continuos
Formato de datos: c4.5
La clase a predecir tiene que ser un numero comenzando de 0: por ejemplo, para 3 clases, las clases deben ser 0,1,2

PMG - Ultima revision: 20/05/2019
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define LOW 1.e-14                 /*Minimo valor posible para una probabilidad*/
#define PI  3.141592653
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))

/*defino una estructura para el clasificador naive bayes*/
struct NB {
    int N_IN;           /*Total numbre of inputs*/
    int N_Class;        /*Total number of classes (outputs)*/
    int N_Bins;         // Cantidad de bins para usar en el histograma.

    int SEED;           /* semilla para la funcion rand(). Los posibles valores son:*/
                    /* SEED: -1: No mezclar los patrones: usar los primeros PR para entrenar
                                 y el resto para validar.Toma la semilla del rand con el reloj.
                              0: Seleccionar semilla con el reloj, y mezclar los patrones.
                             >0: Usa el numero leido como semilla, y mezcla los patrones. */

    /* MATRICES DEL CLASIFICADOR */
    float *featMin; // Arreglo para los valores mínimos de cada atributo.
    float *featMax; // Arreglo para los valores máximos de cada atributo.
	float ***featProb; // Matriz para las probabilidades de los features, 
	                   // dividida por feature, clase y bin del histograma correspondiente.
	float *classProb; // Arreglo de probabilidades a priori de las clases.
};

struct DATOS {
    char *filename;      /* nombre del archivo para leer los datos */
    int N_IN;             /* N1: dimensiones de entrada */
    int PTOT;           /* cantidad TOTAL de patrones en el archivo .data */
    int PR;             /* cantidad de patrones de ENTRENAMIENTO */
                        /* cantidad de patrones de VALIDACION: PTOT - PR*/
    int PTEST;          /* cantidad de patrones de TEST (archivo .test) */

    /*matrices de datos*/
    float **data;                     /* train data */
    float **test;                     /* test  data */
    int *pred;                        /* salidas predichas ES UNA CLASE*/
    int *seq;      	       	      /* indice de acceso con la sequencia de presentacion de los patrones*/
};

int CONTROL;        /* nivel de verbosity */


/* -------------------------------------------------------------------------- */
/*define_matrix: reserva espacio en memoria para todas las matrices declaradas.
  Todas las dimensiones son leidas del archivo .nb en la funcion arquitec()  */
/* -------------------------------------------------------------------------- */
int define_matrix_nb(struct NB *nb){
  /*ALLOCAR ESPACIO PARA LAS MATRICES DEL CLASIFICADOR
   mínimo y máximo por feature, probabilidades por feature
   y probabilidades a priori. */
	int i, j; 
	
	nb -> featMin = (float *) calloc(nb -> N_IN, sizeof(float));
	nb -> featMax = (float *) calloc(nb -> N_IN, sizeof(float));
	
	nb -> featProb = (float ***) calloc(nb -> N_IN, sizeof(float **));
	nb -> classProb = (float *) calloc(nb -> N_Class, sizeof(float));
	
	if(!nb -> featMin || !nb -> featMax || !nb -> featProb || !nb -> classProb) return 1;
	
	for(i = 0; i < nb -> N_IN; i++){
		nb -> featProb[i] = (float **) calloc(nb -> N_Class, sizeof(float *));
		if(!nb -> featProb[i]) return 1;
		
		for(j = 0; j < nb -> N_Class; j++){
			nb -> featProb[i][j] = (float *) calloc (nb -> N_Bins, sizeof(float));
			if(!nb -> featProb[i][j]) return 1;
		}
	}

  return 0;
}

/* -------------------------------------------------------------------------- */
/*define_matrix_datos: reserva espacio en memoria para todas las matrices de datos declaradas.
  Todas las dimensiones son leidas del archivo .net en la funcion arquitec()  */
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

  if(datos->seq==NULL||datos->data==NULL||(datos->PTEST&&datos->test==NULL)||datos->pred==NULL) return 1;
  
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
/*arquitec: Lee el archivo .nb e inicializa el algoritmo en funcion de los valores leidos
  filename es el nombre del archivo .nb (sin la extension) */
/* ---------------------------------------------------------------------------------- */
int arquitec(char *filename,struct NB *nb,struct DATOS *datos){
  FILE *b;
  char filepat[100];
  /*bandera de error*/
  int error;

  time_t t;

  /*Paso 1:leer el archivo con la configuracion*/
  sprintf(filepat,"%s.nb",filename);
  b=fopen(filepat,"r");
  error=(b==NULL);
  if(error){
    printf("Error al abrir el archivo de parametros\n");
    return 1;
  }

  /* Dimensiones */
  fscanf(b,"%d",&nb->N_IN);
  fscanf(b,"%d",&nb->N_Class);
  datos->N_IN=nb->N_IN;

  /* Archivo de patrones: datos para train y para validacion */
  fscanf(b,"%d",&datos->PTOT);
  fscanf(b,"%d",&datos->PR);
  fscanf(b,"%d",&datos->PTEST);

  fscanf(b,"%d",&nb->N_Bins);

  /* Semilla para la funcion rand()*/
  fscanf(b,"%d",&nb->SEED);

  /* Nivel de verbosity*/
  fscanf(b,"%d",&CONTROL);

  fclose(b);


  /*Paso 2: Definir matrices para datos y parametros (e inicializarlos*/
  error=define_matrix_nb(nb);
  if(error){
    printf("Error en la definicion de matrices del clasificador\n");
    return 1;
  }
  error=define_matrix_datos(datos);
  if(error){
    printf("Error en la definicion de matrices de datos\n");
    return 1;
  }

  
  /* Chequear semilla para la funcion rand() */
  if(nb->SEED==0) nb->SEED=time(&t);

  /*Imprimir control por pantalla*/
  printf("\nNaive Bayes con histogramas:\nCantidad de entradas:%d",nb->N_IN);
  printf("\nCantidad de clases:%d",nb->N_Class);
  printf("\nArchivo de patrones: %s",filename);
  printf("\nCantidad total de patrones: %d",datos->PTOT);
  printf("\nCantidad de patrones de entrenamiento: %d",datos->PR);
  printf("\nCantidad de patrones de validacion: %d",datos->PTOT-datos->PR);
  printf("\nCantidad de bins del histograma: %d",nb->N_Bins);
  printf("\nCantidad de patrones de test: %d",datos->PTEST);
  printf("\nSemilla para la funcion rand(): %d",nb->SEED); 

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


//~ getIntervalIndex: Dados un índice de un atributo y un valor, devuelve el 
//~ índice del intervalo del histograma al que pertenece el valor para el atributo dado.
int getIntervalIndex(struct NB *nb, int attribute, float value){
	float minValue = nb -> featMin[attribute];
	float maxValue = nb -> featMax[attribute];
	int intervalIndex = (int) ((value - minValue) / (maxValue - minValue) * nb -> N_Bins);
	
	//Ajuste para que el valor máximo del atributo no se salga de rango.
	return min(intervalIndex, nb -> N_Bins - 1);
}

/* ------------------------------------------------------------------- */
/*Prob:Calcula la probabilidad de obtener el valor x para el input feature y la clase clase
  Aproxima las probabilidades por histogramas */
/* ------------------------------------------------------------------- */
float prob(struct NB *nb,float x,int feature,int clase){
	int intervalIndex = getIntervalIndex(nb, feature, x);
	
	return nb -> featProb[feature][clase][intervalIndex];
}
/* ------------------------------------------------------------------------------ */
/*output: calcula la probabilidad de cada clase dado un vector de entrada *input
  usa el log(p(x)) (sumado)
  devuelve la de mayor probabilidad                                               */
/* ------------------------------------------------------------------------------ */
int output(struct NB *nb,float *input){
  int i,k; 	
  float prob_de_clase;
  float max_prob=-1e40;
  int clase_MAP;
  
  for(k=0;k<nb->N_Class;k++) {
    prob_de_clase=0.;

    /*calcula la probabilidad de cada feature individual dada la clase y la acumula*/
    for(i=0;i<nb->N_IN;i++) prob_de_clase += log( prob(nb, input[i] ,i ,k ) );

    /*agrega la probabilidad a priori de la clase*/    
    prob_de_clase += log(nb -> classProb[k]);

    /*guarda la clase con prob maxima*/
    if (prob_de_clase>=max_prob){
      max_prob=prob_de_clase;
      clase_MAP=k;
    }
  }
  
  return clase_MAP;
}
/* ------------------------------------------------------------------------------ */
/*evaluar: calcula las clases predichas para un conjunto de datos
  la matriz S tiene que tener el formato adecuado ( definido en arquitec() )
  pat_ini y pat_fin son los extremos a tomar en la matriz
  usar_seq define si se accede a los datos directamente o a travez del indice seq
  los resultados (las propagaciones) se guardan en la matriz seq                  */
/* ------------------------------------------------------------------------------ */
float evaluar(struct NB *nb,struct DATOS *datos,float **S,int pat_ini,int pat_fin,int usar_seq){

  float mse=0.0;
  int nu;
  int patron;
  
  for (patron=pat_ini; patron < pat_fin; patron ++) {

   /*nu tiene el numero del patron que se va a presentar*/
    if(usar_seq) nu = datos->seq[patron];
    else         nu = patron;

    /*clase MAP para el patron nu*/
    datos->pred[nu]=output(nb,S[nu]);

    /*actualizar error*/
    if(S[nu][nb->N_IN]!=(float)datos->pred[nu]) mse+=1.;
  }
    

  mse /= ( (float)(pat_fin-pat_ini));

  if(CONTROL>3) {printf("End prop\n");fflush(NULL);}

  return mse;
}

/* --------------------------------------------------------------------------------------- */
/*train: ajusta los parametros del algoritmo a los datos de entrenamiento
         guarda los parametros en un archivo de control
         calcula porcentaje de error en ajuste y test                                      */
/* --------------------------------------------------------------------------------------- */
int train(struct NB *nb,struct DATOS *datos){

  int i,k;
  int class, attribute;
  int error;
  int N_TOTAL;
  float train_error,valid_error,test_error;
  FILE *fpredic;
  char filepat[100];

  /*Asigno todos los patrones del .data como entrenamiento porque este metodo (gaussianas) no requiere validacion*/
  //~ N_TOTAL=datos->PTOT;
  N_TOTAL=datos->PR; // En este problema sí hay validacion*/

  /*efectuar shuffle inicial de los datos de entrenamiento si SEED != -1 (y hay validacion)*/
  if(nb->SEED>-1 && N_TOTAL<datos->PTOT){
    srand((unsigned)nb->SEED);    
    shuffle(datos->PTOT,datos);
  }

	//~ Contamos la cantidad de elementos de cada clase 
	for(i = 0; i < N_TOTAL; i++){
		class = datos -> data[i][datos -> N_IN];
		nb -> classProb[class]++;
	}

	//~ Inicializamos el mínimo y máximo de cada feature al valor del primer elemento.
	for(attribute = 0; attribute < nb -> N_IN; attribute++)
		nb -> featMin[attribute] = nb -> featMax[attribute] = datos -> data[0][attribute];

	//~ Calculamos el mínimo y el máximo de cada feature.
	for(i = 0; i < N_TOTAL; i++){		
		for(attribute = 0; attribute < nb -> N_IN; attribute++){
			nb -> featMin[attribute] = min(nb -> featMin[attribute], datos -> data[i][attribute]);
			nb -> featMax[attribute] = max(nb -> featMax[attribute], datos -> data[i][attribute]);
		}
		
	}

	//~ Contamos la cantidad de elementos que caen en cada bin para cada atributo.
	for(i = 0; i < N_TOTAL; i++){
		class = datos -> data[i][datos -> N_IN];
		for(attribute = 0; attribute < nb -> N_IN; attribute ++){
			int intervalIndex = getIntervalIndex(nb, attribute, datos -> data[i][attribute]);
			nb -> featProb[attribute][class][intervalIndex]++;
		}
	}
	
	//~ Calculamos la probabilidad para cada clase, feature y bin, aplicando un m-estimate
	//~ con m = 1 y p = 1 / N_Bins.
	for(class = 0; class < nb -> N_Class; class++){
		int classCount = nb -> classProb[class];
		for(attribute = 0; attribute < nb -> N_IN; attribute ++){	
			for(i = 0; i < nb -> N_Bins; i++){
				nb -> featProb[attribute][class][i] = (nb -> featProb[attribute][class][i] + 1/nb -> N_Bins) / (classCount + 1);
			}
		}
	}
  
	/*Calcular probabilidad intrínseca de cada clase*/
	for(i = 0; i < nb -> N_Class; i++)
		nb -> classProb[i] = max(LOW, nb -> classProb[i] / N_TOTAL);
  
 
  /*calcular error de entrenamiento*/
  train_error=evaluar(nb,datos,datos->data,0,datos->PR,1);
  /*calcular error de validacion; si no hay, usar mse_train*/
  if(datos->PR==datos->PTOT) valid_error=train_error;
  else         valid_error=evaluar(nb,datos,datos->data,datos->PR,datos->PTOT,1);
  /*calcular error de test (si hay)*/
  if (datos->PTEST>0) test_error =evaluar(nb,datos,datos->test,0,datos->PTEST,0);
  else         test_error =0.;
  /*mostrar errores*/
  printf("\nFin del entrenamiento.\n\n");
  printf("Errores:\nEntrenamiento:%f%%\n",train_error*100.);
  printf("Validacion:%f%%\nTest:%f%%\n",valid_error*100.,test_error*100.);
  
	if(CONTROL){
		printf("\nProbabilidades a priori:\n");
		for(class = 0; class < nb -> N_Class; class++)
			printf("Clase %d: %f\n", class, nb -> classProb[class]);
	  
		printf("\n-- Probabilidades:\n");
		for(attribute = 0; attribute < datos -> N_IN; attribute++){
			printf("Atributo %d:\n", attribute);
			for(class = 0; class < nb -> N_Class; class++){
				printf("Clase %d:", class);
				for(i = 0; i < nb -> N_Bins; i++)
					printf("\t%f", nb -> featProb[attribute][class][i]);
				printf("\n");
			}
			printf("\n");
		}
	 
	}
  
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

  struct NB nb;
  struct DATOS datos;
  
  /* defino la estructura*/
  error=arquitec(argv[1],&nb,&datos);
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

  /* entreno la red*/
  error=train(&nb,&datos);                  
  if(error){
    printf("Error en el entrenamiento\n");
    return 1;
  }

  return 0;

}
/* ----------------------------------------------------------------------------------------------------- */

