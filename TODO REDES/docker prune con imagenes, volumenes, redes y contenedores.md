Si trasteas mucho con docker, siempre llega un momento en el que te das cuenta que tu disco está lleno de contenedores, imágenes y volúmenes colgados, que ya no están en uso y han quedado obsoletos en tu disco ocupando espacio. Si tienes muchos, es una labor muy tediosa tener que borrar uno a uno todos los contenedores e imágenes. Hay un modo sencillo de borrarlo, vamos a utilizar **prune**.

![](https://ugeek.github.io/blog/images-blog/docker.png)

## Detener todos los contenedores

Lo primero que haremos con un comando en bash, detener todos los contenedores para iniciar solo aquellos que queremos conservar

```
docker stop $(docker ps -q)
```

## Contenedores

Ya tenemos todos los contenedores detenidos. **Inicia aquellos que quieres conservar**, ya que **prune eliminará todos los contenedores detenidos**.

### Borrar Contenedores detenidos

```
docker container prune
```

Aparecerá un mensaje para que confirmes si realmente deseas borrarlos. Si no deseas que te pregunte, añade el flag **-f**.

```
docker container prune - f
```

## Imágenes

Ya has eliminado los contenedores que no deseabas y ahora habrán quedado imágenes **colgadas**, que no pertenecen a ningún contenedor.

Las imágenes colgadas son aquellas que en su día utilizaste con un contenedor y ahora no pertenece a ninguno, quedando en tu servidor.

También cada vez que actualizamos un contenedor, descarga una nueva imagen. La versión anterior queda en nuestro servidor **colgada**.

Docker no elimina las imágenes antiguas, ya que el objetivo de docker es que nosotros tengamos el control total de nuestros datos, así seremos nosotros los encargados de hacerlo.

### Ver imágenes colgadas

```
docker image ls -f "dangling=true"
```

### Borrar **únicamente las imagenes colgadas**

```
docker image prune
```

### Borrar imágenes que no están siendo utilizadas por ningún contenedor, en el momento de ejecutar el comando

```
docker image prune -a
```

## Volúmenes

Docker crea unos volúmenes para compartir información con el contenedor. Muchos de estos volúmenes, también quedan colgados como sucede con las imágenes. Nuevamente somos nosotros quien tenemos que borrar estos volúmenes en desuso.

### Listar volúmenes

```
docker volume ls
```

### Eliminar todos los volúmenes que no sean utilizados contenedor.

```
docker volume prune
```

## Redes

Docker crea tres redes predeterminadas, bridge,host y none. Es posible que nosotros hayamos creado mas redes y ahora queramos eliminarlas

### Listar redes

```
docker network ls
```

### Eliminar Redes no utilizadas

```
docker network prune
```

## Todo de una vez

Hay un método que te permitirá hacer todos los pasos anteriores de una vez.

### Borrar Contenedores, imágenes, redes colgados

```
docker system prune
```

La terminal te pedirá que confirmes si deseas eliminarlos

### Contenedores, imágenes, redes colgados detenidos

Borrar todos los contenedores **detenidos**, todas **las redes no utilizadas** por al menos un contenedor, **todas las imágenes sin al menos un contenedor** asociado a ellas y **todo el caché** de construcción.

```
docker system prune -a
```

### Añadir también los volumenes

Por defecto, no elimina los volúmenes. Si quieres incluir también estos

```
docker system prune --volumes
```

## Borrar Contenedores por tiempo

Si nos ponemos ha hacer pruebas montando contenedores, también podemos borrar estos por tiempo de creación. Por ejemplo, con este comando borraremos todos los contenedores creado hasta hace 2 horas

```
docker container prune --force --filter "until=2h"
```

## Ayudas

Puedes acceder a las ayudas desde tu terminal. Por ejemplo, la ayuda de **system prune**:

```
docker system prune --help.
```

## Fuente

-   [https://takacsmark.com/docker-prune/](https://takacsmark.com/docker-prune/)
-   [https://www.vidaxp.com/tecnologia/como-borrar-imagenes-contenedores-y-volumenes-docker/](https://www.vidaxp.com/tecnologia/como-borrar-imagenes-contenedores-y-volumenes-docker/)

