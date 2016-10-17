# Icaro-X
###### Versión Modular

## Descripción  
Ésta versión utiliza los desarrollos de Icaro-X separándolos en módulos para su utilización en paralelo.  
Hay varias formas de proceder para alcanzar éste objetivo, **se exponen a continuación las ideas que hay actualmente**.

## Sistemas Posibles

#### Sistema 1  
Todos los módulos pueden escribir/leer.  
La cantidad máxima de sensores y actuadores por módulo  
está **pendiente de decisión**.
###### Flujo de Ejecución
En cada módulo se ejecuta lo siguiente
* [Sensado flags?]
* [sensar]
* [espero turno para comunicar valor, mientras leo los otros]
* [comunico valor]
* [verifico tener todos los valores]
* [determino resultado]
* [espero turno para comunicar resultado, mientras leo los otros]
* [comunico resultado]
* [verifico tener todos los resultados]
* [elijo el que más se repite y lo ejecuto]
* [al terminar, espero turno y comunico sensado Flag OK]

Éste es un desarrollo tentativo del flujo de trabajo, está sujeto a cambios si se encuentra una mejor opción.

##### Ventajas
* El sensado y ejecución se hacen de forma paralela en todos los casos
* No se termina un ciclo de sensado ni de ejecución hasta que todos los módulos hayan participado: el flujo de trabajo mantiene un orden.
* Todos los módulos tienen toda la información respecto a los sensados y la usan para determinar el próximo movimiento a realizar, entonces tenemos [Nº modulos] resultados, lo que nos da chance de comparar resultados y descartar malos funcionamientos.  
(se ejecuta el que más se repite)

##### Desventajas
* Al tener que esperar la respuesta de todos los módulos antes de proceder, un "conjunto de movimientos" termina cuando termina el movimiento más largo de ese conjunto.

#### Sistema 2
Un ***módulo "master"*** se encarga de todo el sensado, y tiene la capacidad de enviar mensajes por comunicación serial.

Varios **módulos "esclavos"*** se encargan de manejar los actuadores (servo motores) dependiendo del mensaje que reciben del módulo "master".

Se determina un tiempo ***"timeout"*** máximo en que un módulo "esclavo" puede llevar a cabo una acción determinada, éste tiempo afecta el timing del sistema completo.  
El sistema queda dividido así en ***"ciclos" de trabajo***.

###### Flujo de Ejecución
Se considera que ***el Master actúa*** y ***el Slave reacciona***.  

|Master|Slave|
|------|-----|
|comunica **timeout**|Para ejecución|
|sensado|espera mensaje|
|determina movimiento|espera mensaje|
|comunica movimiento|recibe mensaje|
|conteo de **timeout**|ejecuta movimiento|

##### Ventajas
* Se evita tratar Multi-Master.
* No se pierde tiempo en mensajes entre módulos, sólo interviene un mensaje.

##### Desventajas
* Sólo la ejecución es en paralelo, todo el sensado es en serie.
* No hay posibilidad de revisión de resultados, ya que lo maneja sólo el Master.
* la ejecución en paralelo sigue manejando un tiempo máximo. Éste tiempo máximo debe ser el movimiento más largo posible de los servos, lo cual nos limita incluso más que esperar el máximo movimiento en el sistema.

#### Sistema 3
Un sistema **Multi-Master** modularizado, manejando intervalos granulados de tiempo implementando un tiempo ***timeout***.  
Los **movimientos tendrán que ser relativos** respecto a la última posición del servo.  
Se comunica una **posición relativa** cada ***"timeout"*** tiempo.

###### Flujo de Ejecución
En cada módulo se ejecuta lo siguiente  
* [timeout]
* [sensado]
* [espero turno para comunicar valor, mientras leo los otros]
* [comunico valor]
* [verifico tener todos los valores]
* [determino resultado]
* [espero turno para comunicar resultado, mientras leo los otros]
* [comunico resultado]
* [verifico tener todos los resultados]
* [elijo el que más se repite y lo ejecuto]
* [espero timeout]

##### Ventajas respecto a los anteriores
* Determinando un ***timeout*** puedo hacer una relación directa entre la curva dibujada por el sensor y el movimiento de los actuadores.
