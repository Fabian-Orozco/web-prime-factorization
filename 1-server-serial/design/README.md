# Servidor Web Concurrente

## Descripción general del diseño

Este proyecto desarrolla un servidor concurrente basándose en un servidor serial heredado; nuestra solución implementa concurrencia usando la metodología producer-consumer para generar un mapeo dinámico de las solicitudes y su manejo.

La metodología empleada simula una cadena de producción, en la cuál el despachador (httpConnectionHandler) mueve las solicitudes (sockets) de los clientes a una cola. Al ser un servidor concurrente, varias conexiones pueden conectarse y crear solicitud al mismo tiempo para esta cola.

Son los hilos de la Web App los que toman dichas solicitudes, para que  pasen luego a ser consumidos por la aplicación de factorización y generar los cálculos respectivos. Una vez terminado el paso anterior, el mismo hilo devuelve los resultados a los clientes.

![UML](/Proyecto01/design/Diseño.png "Diagrama UML")

### HttpConnectionHandler

Clase despachador, se encarga de recibir las solicitudes de todas las conexiones una vez el servidor es arrancado, y existen al mismo tiempo tantos connectionHandlers como la máxima cantidad de conexiones permitidas.

Extrae las solicitudes recibidas, creando un objeto HttpRequest que contiene las solicitudes a procesar y un HttpResponse que almanenará la respuesta de FactWebApp para el cliente.

### FactWebApp

FactWebApp es la clase a cargo de la factorización de los números, así como de entregar al usuario el resultado de la factorización mediante el HttpResponse. Se encarga de recibir los números, clasificarlos según su validez (número válido, negativo o entrada inválida) y realizar su correspondiente procesamiento.

## Mathmatician

Mathmatician contiene métodos utilitarios que realizan los cálculos de descomposición factorial para los números recibos en la solicitud hecha por el usuario.
