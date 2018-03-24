# SmartHome

**SmartHome** es la segunda versión de mi proyecto de **hardware** para la automatización del hogar, esta vez por comunicación inalámbrica y compatible con [**MySensors**](https://github.com/mysensors/MySensors/tree/master).

![Banner SmartHome](https://raw.github.com/giltesa/SmartHome/blob/master/banner.png)


## Introducción

En este repositorio encontraras la documentación para construir los diferentes nodos disponibles, eso incluye los esquemáticos, los circuitos, los gerbers, piezas en 3D, código fuente, etc.

La funcionalidad de los nodos varia dependiendo de la tarea para la que hayan sido construidos. Por ejemplo encontraras un nodo de interruptor táctil para sustituir los interruptores de la pared y que permite encender, apagar y *regular el brillo(en desarrollo)* de las lamparas del techo de las habitaciones. También encontraras un nodo para tomar mediciones de temperatura, humedad y luz así como capaz de detectar eventos como la apertura y cierre de puertas y ventanas. Un poco mas abajo hay disponible una lista de todos los nodos diseñados.


### Requisitos previos

Los nodos funcionan por comunicación inalámbrica RF a 2.4Ghz distinta del WiFi por lo que hace falta un Gateway capaz de convertir esa comunicación inalámbrica a cableada y viceversa, esto tiene como inconveniente que hace falta un aparato mas pero tiene como ventaja que no sobrecargaremos el router WiFi con una treintena de nodos conectados al mismo tiempo.

Ademas del Gateway también es necesario un software de gestión que controle todos los nodos, este software puede ser cualquiera que soporte nodos programados con la librería MySensors, en el apartado [controlador](https://www.mysensors.org/controller) hay documentada una tabla de softwares de gestión compatibles.


## Lista de nodos

Esta es la lista de todos los nodos con un estado de desarrollo avanzado pero no finalizado. Para consultar su estado actual o próximos nodos que desarrollare visitar este panel de trabajo: [Trello SmartHome](https://trello.com/b/6nWJ42Qx/smarthome)

* Interruptor táctil de relés
* Interruptor táctil de relé+triac
* Sensor multiuso
* Mando a distancia
* Regulador de luz LED RGBW


## Licencia

Este proyecto está licenciado bajo la licencia **Creative Commons** de tipo: **[Reconocimiento-NoComercial-CompartirIgual 4.0 Internacional (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/3.0/deed.es_ES)**. Consulte el archivo [LICENSE.md](LICENSE.md) para obtener más información.