#include <opencv.hpp>

using namespace cv;

int main(int argc, int **argv){

	//Nome da Imagem
	string filename = "lena.bmp";

	//Carregar imagem dentro da matriz
	Mat img = imread(filename);

	//Verifica carregamento
	if (img.empty()){
		fprintf(stderr, "Erro: imagem %s nao encontrada.\n", filename.c_str());
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