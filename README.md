# Módulo de Seguridad

# PROYECTO DE SISTEMAS OPERATIVOS I-2016
	
	"SUPERMERCADOS FACYT"
	MÓDULO: GUARDIAS DE SEGURIDAD
	GRUPO #2:
		Sebastian Otero 	C.I. V-24.499.127
 		Solveig Arteaga 	C.I. V-24.498.716
 		José Abreu 			  C.I. V-23.423.967
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
	- Supervisor: A las 8am, el supervisor da la orden a los guardias de seguridad de abrir el supermercado para luego, 12 horas después exactamente, 
	              dar la orden de cerrarlo.

	- Servicio Técnico: Además, arrancar los procesos de seguridad tanto para los datos de transacciones como las grabaciones de las cámaras de vigilancia,
	                    para lo cual tienen que solicitar el acceso al cuarto de grabación a los guardias de seguridad. El respaldo se realiza diariamente, 
			    justo antes de vender productos regulados o, si no se venden productos de este tipo, antes de cerrar el supermercado (antes de las 20h).
