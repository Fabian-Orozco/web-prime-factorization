# Servidor Web Concurrente

## Descripción general del diseño

Este proyecto desarrolla un servidor concurrente basándose en un servidor serial heredado; nuestra solución implementa concurrencia usando la metodología producer-consumer para generar un mapeo dinámico de las solicitudes y su manejo.

La metodología empleada simula una cadena de producción, en la cuál el despachador (httpConnectionHandler) mueve las solicitudes (sockets) de los clientes a una cola. Al ser un servidor concurrente, varias conexiones pueden conectarse y crear solicitud al mismo tiempo para esta cola.

Son los hilos de la Web App los que toman dichas solicitudes, para que  pasen luego a ser consumidos por la aplicación de factorización y generar los cálculos respectivos. Una vez terminado el paso anterior, el mismo hilo devuelve los resultados a los clientes.

![UML](/design/img/UML.png "Diagrama UML")

Para comprender el patrón que buscan llevar el programa puede ver el diagrama o consultar el código en pseudocódigo:

- [Client dispatcher](./pseudo/Client_Dispatcher.pseudo)
- [Servidor concurrente](./pseudo/concurrent_server.pseudo)
- [Factorizer](./pseudo/Factorizer.pseudo)
- [Fact Web App](./pseudo/FactWebApp.pseudo)
- [Http Connection Handler](./pseudo/httpConnectionHandler.pseudo)
- [Packer](./pseudo/Packer.pseudo)
- [Request Dispatcher](./pseudo/RequestDispatcher.pseudo)
