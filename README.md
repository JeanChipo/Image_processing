
#### Programme de traitement d'images en format .bmp, prenant en charge les formats .bmp8 (image en niveau de gris) et .bmp24 (image en couleur)

## Utilisation

### Avec les images de base
Pour l'instant, le programme utilise les images `lena_gray.bmp` et `lena_color.bmp` de base.
Pour effectuer les modifications sur ces images, il suffit d'executer main.exe
/!\ sous windows, il est nécessaire d'avoir un compilateur. Voir la page officielle de gnu si vous n'en avez pas encore installer un : https://gcc.gnu.org/install/index.html

### Avec une image customisée
Pour modifier l'image utilisé, il suffit de mettre votre fichier à la même arborescence que le programme ( `\[chemin d'installation\]/Image_processing/\[ICI\]` ), et de mettre votre nom de fichier à la place de `lena_gray.bmp` ou `lena_color.bmp` selon le type de bmp utilisé.

Ensuite, il sera nécessaire de compiler le programme, avec les commandes suivantes :
> gcc -c main.c \
> gcc -c bmp24.c \
> gcc -c bmp8.c \
> gcc -c egalisation.c \
> gcc main.o egalisation.o bmp24.o bmp8.o -o main.exe

Après cette étape, il suffit d'executer `main.exe`

## Fonctionnalités 
### Traitement d’une image en niveaux de gris :
* Affichage des données de l'image (largeur, hauteur, profondeur de couleur, taille)

* Inversion des couleurs : ![1.png](screenshots/1.png)

* Changement de luminosité : ![2.png](screenshots/2.png)

* Transformation en image binaire : ![3.png](screenshots/3.png)

* Filtres : 
	* box_blur
	* gaussian_blur
	* outline
	* emboss
	* sharpen
	* custom

![4.png](screenshots/4.png)

* Égalisation d’histogramme :
![10.png](screenshots/10.png)

### Traitement d’une image en couleur :
![5.png](screenshots/5.png)


* negative :

![6.png](screenshots/6.png)
	
* grayscale :

![7.png](screenshots/7.png)

* brightness :

![8.png](screenshots/8.png)

* Filtres de convolution :
	* boxBlur
	* gaussianBlur
	* outline
	* emboss
	* sharpen 
    * custom

![9.png](screenshots/9.png)

* Égalisation d’histogramme :
	![11.png](screenshots/11.png)

## Bugs connus

* Saisie d'un caractère / chaine de caractère lors de la demande d'une valeur entière
