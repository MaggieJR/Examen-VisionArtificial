
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#define pi 3.14159265

using namespace cv;
using namespace std;

//Función para Obtener la matriz de la imagen y detecta si hay algun error 
Mat obtenerImagen(char NombreImagen[]) {

	Mat imagen; // Matriz que contiene nuestra imagen sin importar el formato
	

	/*********Lectura de la imagen*********/
	imagen = imread(NombreImagen);

	if (!imagen.data)
	{
		cout << "Error al cargar la imagen: " << NombreImagen << endl;
		exit(1);
	}
	return imagen; 
}

//Esta función muestra la imagen
void ShowImagen(Mat imagen) {
	/************Procesos*********/
	int fila_original = imagen.rows;
	int columna_original = imagen.cols;

	cout << "filas: " << fila_original << endl;//Lectura de cuantas filas

	cout << "columnas: " << columna_original << endl;//Lectura de cuantas columnas


	namedWindow("Hola Mundo", WINDOW_AUTOSIZE);//Creaci�n de una ventana
	imshow("Hola Mundo", imagen);//Muestra la imagen

	/************************/

	waitKey(0); //Funci�n para esperar

}

//Función para sacar imagenes a escala de grises por promedio  
void ShowImagenG(Mat imagenG) {

	Mat gray(imagenG.rows, imagenG.cols, CV_8UC1);
	/************Procesos*********/
	int fila_original = imagenG.rows;
	int columna_original = imagenG.cols;
	Mat histogramita(256, 1, CV_64F, Scalar(0));
	int nivel;
	int i;
	int j;

/************************************************************/
/******************POR PROMEDIO A GRISES********************/

	for (int i = 0; i < fila_original; i++)
	{
		for (int j = 0; j < columna_original; j++)
		{

			Vec3b pixel = imagenG.at<Vec3b>(i, j);
			uchar B = pixel[0];
			uchar G = pixel[1];
			uchar R = pixel[2];

			gray.at<uchar>(i, j) = ((B + G + R) / 3);
			
		}
	}
	
	ShowImagen(gray);


}

//No jala
void histograma(Mat imagen) {

}

//Generamos el Kernel con el valor sigma 
double **GKernelcito(int sigma, int kernelcito) {
//Mat matriz; 
	Mat matriz(kernelcito, kernelcito, CV_8UC1);
	int centro = (kernelcito - 1) / 2;

	double** matrizp = new double* [kernelcito];


	for (int i = 0; i < kernelcito; i++) {
		matrizp[i] = new double[kernelcito];
		for (int j = 0; j < kernelcito; j++) {
			int posx = i - centro;
			int posy = (j - centro) * -1;

			double valor = 1 / (2 * pi * sigma * sigma);
			valor = valor * exp(-(pow(posx, 2) + pow(posy, 2)) / 2 * pow(sigma, 2));
			matrizp[i][j] = valor;
			cout << valor << "\t\t";

		}
		cout << "\n";
	}
	return matrizp;
}

//Se pasa el filtro de Gauss
void PMatriz(Mat imagen, int kernel, int sigma) {
	int rows = imagen.rows;
	int cols = imagen.cols;
	int exceso = (kernel - 1) / 2;

	Mat grises(rows + exceso, cols + exceso, CV_8UC1);
	Mat Final(rows + exceso, cols + exceso, CV_8UC1);

	cvtColor(imagen, grises, COLOR_BGR2GRAY);

	double R, B, G, grisito;

	for (int i = 0; i < rows + exceso; i++) {
		for (int j = 0; j < cols + exceso; j++) {

			if (i >= rows || i < exceso) { // >=
				Final.at<uchar>(Point(j, i)) = uchar(0);


			}
			else if (j >= cols || j < exceso) {
				Final.at<uchar>(Point(j, i)) = uchar(0);

			}
			else {
				B = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[0];
				G = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[1];
				R = imagen.at<Vec3b>(Point(j - exceso, i - exceso)).val[2];
				grisito = (B + G + R) / 3;

				Final.at<uchar>(Point(j, i)) = uchar(grisito);
			}
		}
	}
	ShowImagen(Final);
}

void Canny(Mat imagen) {
	Mat gray(imagen.rows, imagen.cols, CV_8UC1);
	int fila_original = gray.rows;
	int columna_original = gray.cols;
	/**cv::Mat contorno;std::vector<cv::Mat> channels;cv::Mat hsv;**/
	cv::Mat contorno;
	cv::Canny(imagen, contorno, 40, 120);
	cv::imshow("Canny", contorno);
	waitKey(0);
}

int main() {

//Se coloca la imagen a procesar
	char NombreImagen[] = "zorro.jpg";
// Se genera la matriz con la función
	Mat imagen = obtenerImagen(NombreImagen);
//Se utiliza la funcion ShowImagen para mostrar la imagen original
	ShowImagen(imagen);
//Se utiliza la funcion ShowImagenG para mostrar la imagen a Gris
	ShowImagenG(imagen);
//Asignamos una variable para sigma
	int sigma;
//Se genera la vista apara pedir datos
	cout << "\n\t\t\t********* Filtro Gaussiano *********** \n\n";
//El usuario ingresa el valor de Sigma
	cout << "\t Elige un valor de sigma: ";
	cin >> sigma;
//El usuario ingresa el valor del Kernel
	int kernel;
	cout << "\n\tElige un tamaño para kernel (debe ser impar): ";
	cin >> kernel;
//Se implementa condicional para evitar que el kernel sea par y menor que 1
	while (kernel < 1 || kernel % 2 == 0) {
		cout << "No se aceptan Kernel pares o menores a 1 ";
		cin >> kernel;
	}
//Se implementa la fucnion que nos dara e imprimirá el kernel
	double** matriz_kernel = GKernelcito(sigma, kernel);
//Se implementa la función para el filtro Gussiano 
	PMatriz(imagen, kernel, sigma);
//Se implementa la función Canny
	Canny(imagen);

	return 1;

}


















