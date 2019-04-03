#include <opencv.hpp>

using namespace cv;

int main(int argc, int **argv){

	//Nome da Imagem
	//const char *filename = "lena.bmp";

	//Carregar imagem dentro da matriz
	Mat img;
	img = cvLoadImage("C:/Users/Alan/Documents/Visual Studio 2010/Projects/opencv/Debug/lena.bmp");

	//Verifica carregamento
	if (img.empty()){
		fprintf(stderr, "Erro: imagem %s nao encontrada.\n", "lena.bmp");
		getchar();
		exit(1);
	}

	//Cria nova janela
	namedWindow("Processamento Digital de Imagens", CV_WINDOW_AUTOSIZE);

	//Mostrar imagem
	imshow("Processamento Digital de Imagens", img);

	//Espera click
	waitKey();

	return 0;

}