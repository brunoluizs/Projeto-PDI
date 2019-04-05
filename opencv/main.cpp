#include <opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

Mat expandImgSize(Mat img){
	Mat expanded_img;

	//Funções para o cálculo do tamanho ideal para a DFT
	int m = 2*img.rows;
	int n = 2*img.cols;

	//Função para copiar a imagem e aumentar a borda, inserindo 0's ao redor da imagem.
	copyMakeBorder(img, expanded_img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//imshow("Processamento Digital de Imagens", teste);

	return expanded_img;
}


void dft(Mat img, string source){
	Mat dft_img;

	dft_img = expandImgSize(img);

	imshow("Processamento Digital de Imagens", dft_img);
}


int main(int argc, int **argv){

	//Nome da imagem
	string source = "../Debug/lenastripes.bmp";

	//Carregar imagem dentro da matriz
	Mat img;
	img = imread(source);

	//Verifica carregamento
	if (img.empty()){
		fprintf(stderr, "Erro: imagem nao encontrada.\n");
		getchar();
		exit(1);
	}

	//Cria nova janela
	namedWindow("Processamento Digital de Imagens", CV_WINDOW_AUTOSIZE);

	//Transformada Discreta de Fourier
	dft(img, source);
	
	//Espera click
	waitKey();

	return 0;

}