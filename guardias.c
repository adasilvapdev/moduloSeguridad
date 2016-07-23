/*
	PROYECTO DE SISTEMAS OPERATIVOS I-2016
	
	"SUPERMERCADOS FACYT"
	MÓDULO: GUARDIAS DE SEGURIDAD

	GRUPO #2:
		Sebastian Otero 	C.I. V-24.499.127
 		Solveig Arteaga 	C.I. V-24.498.716
 		José Abreu 			C.I. V-23.423.967
		Andrea Da Silva 	C.I. E-84.438.575
	
	Objetivos:
	- Abrir (8h) y cerrar (20h) el supermercado, tras recibir la orden del supervisor. 
	- Controlar el orden en las colas de productos regulados que expende el 
	  supermercado.
	- Cuando el supervisor da la orden de vender los productos, los guardias 
	  reparten tantos números como les haya entregado el supervisor a las 
	  personas de la cola.
	- Si el número de personas a las que se les venderán los productos regulados
	  (los que tienen tickets) supera a las de la cola, los guardias deben
	  esperar a que llegue al menos otra persona más para seguir repartiendo
	  (no se debe dar la orden de que pasen todos los que tienen tickets a
	  comprar los productos). En caso contrario, le avisarán a las personas en
	  la cola que se agotaron los productos (solo pasan los que tienen tickets)
	  y que si lo desean pueden volver en otro momento (los restantes sin ticket
	  no entran).
	- Dar acceso a servicio técnico al cuarto de grabación. Si es día de venta de
	  productos regulados, sino, se da el acceso antes de cerrar el supermercado 
	  (antes de las 20h)
	
	Nota. Módulos que se comunican con el módulo "Guardias de Seguridad":
	- Supervisor: A las 8am, el supervisor da la orden a los guardias de 
				  seguridad de abrir el supermercado para luego, 12 horas 
				  después exactamente, dar la orden de cerrarlo.
	- Servicio Técnico: Además, arrancar los procesos de seguridad tanto 
						para los datos de transacciones como las grabaciones 
						de las cámaras de vigilancia, para lo cual tienen que
						solicitar el acceso al cuarto de grabación a los guardias
						de seguridad. El respaldo se realiza diariamente, justo 
						antes de vender productos regulados o, si no se venden
						productos de este tipo, antes de cerrar el supermercado
						(antes de las 20h).
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#define N 9
#define true 1
#define false 0
#define Proveedor 0
#define Comprador 1
#define Supervisor 2
#define Guardia 3
#define Cajera 4
#define Reponedor 5
#define Mercadeo 6
#define Tecnico 7
#define Limpieza 8
#define Cliente 9
#define MAX_RUBROS 900
// Pedido de Rubro
typedef struct
{
	float pedido; // Almacena el Pedido
	float vendido; // Descontador de lo vendido
}pedidoR;

// Categoría, 0 Hortaliza,1 Bebida,2 Charcutería,3 Carnicería,4 Pescadería,5 Enlatado,6 Producto de Limpieza,7 Producto de Aseo Personal,8 Perfume.
typedef pedidoR categoria[8]; // Cada campo indica la cantidad en kg/unidad del rubro. 

// Semáforos
sem_t modulo[8];

// Globales
categoria rubros,list;

// Compartidas
volatile int hpedido = false;
volatile int vacio = true;
volatile int camiones = false;
volatile int open = false;
volatile int close = true;
volatile int regulados = true;
volatile int llenando = false;
volatile int ticket;

// Cabeceras de Módulos
void * _ThreadProveedor(void *arg);
void * _ThreadComprador(void *arg);
void * _ThreadSupervisor(void *arg);
void * _ThreadGuardia(void *arg);
void * _ThreadCajera(void *arg);
void * _ThreadReponedor(void *arg);
void * _ThreadMercadeo(void *arg);
void * _ThreadTecnico(void *arg);
void * _ThreadLimpieza(void *arg);
// Cabeceras de los procedimientos de los módulos
int _ListadoCierre();
void _LlenarCamiones();
int _EntregaTickets();


// NOTA. TODOS LOS OTROS MÓDULOS LOS DEJÉ AQUÍ PERO EL NUESTRO ES EL DE GUARDIA

// Principal
int main()
{
	int i;
	pthread_t h[8];
	for(i=0;i<N;i++)
	{
		list[i].pedido = list[i].vendido = 0;
		rubros[i].pedido = rubros[i].vendido = 0;
	}
	
	pthread_create(&h[0],NULL,_ThreadProveedor,NULL);	
	pthread_create(&h[1],NULL,_ThreadComprador,NULL);	
	pthread_create(&h[2],NULL,_ThreadSupervisor,NULL);	
	pthread_create(&h[3],NULL,_ThreadGuardia,NULL);	
	pthread_create(&h[5],NULL,_ThreadReponedor,NULL);	
	pthread_create(&h[7],NULL,_ThreadTecnico,NULL);	
	
	for(i=0;i<N;i++)
		pthread_join(h[i],NULL);
	
	return 0;
}

// Módulo Guardia
void * _ThreadGuardia(void *arg)
{

	while(true)
	{
		sem_wait(&modulo[Guardia]);
		puts("-- Modulo Guardia --");
		if(open && !close)
			puts("Abriendo el Supermercado\nSupermercado Abierto");
			if(regulados)	// Si es día de vender productos regulados porque hay productos regulados en almacén
			{
				puts("\n-- Hoy venta de productos regulados --");
				
				// Antes de vender productos regulados se debe dar acceso al cuarto de grabación al módulo de Soporte Técnico
				puts("\n-- Acceso al cuarto de grabación a Soporte Técnico --");
				sem_post(&modulo[Tecnico]);	// Envío de senhal a Soporte Técnico
				
				sem_wait(&modulo[Guardia]);	// Confirma que los de Soporte Técnico entró al cuarto de grabación

				// Luego se procede a la entrega de tickets de productos regulados
				sem_wait(&modulo[Supervisor]);	// Esperando senhal de Supervisor
					puts("\n-- El Guardia recibe tickets de productos regulados de pate del Supervisor --");

					// Cada día el supervisor cambia el número de tickets que entrega a los guardias de seguridad con un ramdom()
					// Esto se hace sobre la variable global tickets
					
					while(ticket)	
					{
						sem_wait(&modulo[Cliente]);							// Espera el cliente
							printf("\n-- Recibido ticket #%d", ticket); 	// El cliente recibió el ticket
							ticket--;										// Ahora hay un ticket menos
					}
				
					puts("\n Se agotaron los productos. (Solo pasan los que tienen tickets)\n Si desean, pueden volver en otro momento");

				sem_post(&modulo[Cajera]);	// Envía senhal a las cajeras para que paguen los de productos regulados
				
			}
		else{
			if(!open && close)
			{
				if(!regulados)
				{
					puts("-- Hoy no hay venta de productos regulados --");
					// Hoy no es día de productos regulados, así que antes de cerrar se debe dar acceso al cuarto de grabación al módulo de Soporte Técnico
					puts("\n-- Acceso al cuarto de grabación a Soporte Técnico al final del día --");
					sem_post(&modulo[Tecnico]);	// Envío de senhal a Soporte Técnico
				
					sem_wait(&modulo[Guardia]);	// Confirma que los de Soporte Técnico entró al cuarto de grabación
				}

				puts("Cerrando el Supermercado\nSupermercado Cerrado");
			}
		}
		puts("-----------------------\n");
	}
}





























// NOTA. EL MODULO SUPERVISOR ME LO PASO EL GRUPO DE SUPERVISOR
// Módulo Supervisor
void * _ThreadSupervisor(void *arg)
{
	while(true)
	{
		if(!vacio)
			sem_post(&modulo[Reponedor]);

		puts("-- Modulo Supervisor --");
		if(!hpedido && _ListadoCierre())	// Si no se ha hecho el pedido
		{
			puts("Procediendo a Omitir orden de pedido");	// Se emite la orden a los compradores
			hpedido = true;									// Se hace el pedido 
			sem_post(&modulo[Comprador]);					// Se llama al módulo comprador
		}else{ // Sino
			if(!open && close && hpedido  && !llenando) // Si está cerrado y ya se hizo el pedido y el listado de cierre está listo
			{
				open = true;
				close = false;
				puts("-- Dando Orden de Abrir a los Guardias --");		// Llamar a guardar para abrir
				sem_post(&modulo[Guardia]);							// Comunicación con guardias de seguridad
			}
		}
		getchar();
	}
}

/** Procedimientos del Módulo Supervisor **/
// -- Revisa el Listado de Cierre y llena la lista de pedidos
int _ListadoCierre()
{
	int i, pedido = false;	
	puts("Revisando Lista de Cierre");
	for(i=0;i<N;i++)
	{
		if(vacio)
		{
			list[i].pedido = MAX_RUBROS;
			if(i+1==N)
			{
				vacio = false;
				pedido = true;
			}
		}
		else if(rubros[i].vendido<(rubros[i].pedido*0.40)) // Si el rubro actual tiene stock menos del 40%
		{
			list[i].pedido = rubros[i].pedido - rubros[i].vendido; // Se pide lo restante
			pedido = true;
		}
	}
	return pedido;
}

// Módulo Proveedor
void * _ThreadProveedor(void *arg)
{
	while(true)
	{
		sem_wait(&modulo[Proveedor]);
		puts("-- Modulo Proveedor --");
		if(hpedido)
		{
			puts("Pedido Recibido\nLlenando Camiones");
			_LlenarCamiones();
			puts("Camiones Llenos\nEnviando Camiones");
			camiones = true;
			sem_post(&modulo[Comprador]);
		}
	}
}

/** Procedimientos del módulo Proveedor **/
void _LlenarCamiones()
{
	int i;
	for(i=0;i<N;i++)
	{
		rubros[i].pedido = list[i].pedido;
		rubros[i].vendido = list[i].pedido;
	}
}

// Módulo Comprador
void * _ThreadComprador(void *arg)
{
	while(true)
	{
		sem_wait(&modulo[Comprador]);
		puts("-- Modulo Comprador --");
		if(hpedido && !camiones)
		{
			// Hacer pedido al Proveedor
			puts("Haciendo Pedido");
			sem_post(&modulo[Proveedor]);
		}
		else if(camiones)
		{
			puts("Camiones Recibidos");
			camiones = false;
		}
	}
}

// Módulo Reponedor
void * _ThreadReponedor(void *arg)
{
	while(true)
	{
		sem_wait(&modulo[Reponedor]);
	}
}


// Módulo Tecnico
void * _ThreadTecnico(void *arg)
{
	while(true)
	{
		sem_wait(&modulo[Tecnico]);
	}
}
