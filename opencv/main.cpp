#include <opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;


Mat centerTransform(Mat img){
	unsigned char* data;
	int i, j;
	long long int pos = 0;

	data = img.ptr(0);

	//Aplicar o (-1)^(x+y) a todos os 3 canais
	for (i = 0; i < img.rows*3; i++){
		for (j = 0; j < img.cols; j++){
			data[pos] = data[pos] * pow((double) -1, i+j);
			pos += 1;
		}
	}

	Mat centered_transform(img.rows, img.cols, CV_8UC3, data);

	return centered_transform;
}


Mat expandImgSize(Mat img){
	Mat centered_img = centerTransform(img);
	Mat expanded_img;

	//Funções para o cálculo do tamanho ideal para a DFT
	int m = 2*img.rows;
	int n = 2*img.cols;

	//Função para copiar a imagem e aumentar a borda, inserindo 0's ao redor da imagem.
	copyMakeBorder(img, expanded_img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//imshow("Processamento Digital de Imagens", teste);

	return expanded_img;
}


void filter(Mat img){
	//Divisão dos planos REAL [0] e COMPLEXO [1]
	Mat dft_img[2];

	dft_img[0] = expandImgSize(img);
	dft_img[1] = dft_img[1].zeros(dft_img[0].rows, dft_img[0].cols, CV_8U);

	imshow("Processamento Digital de Imagens", dft_img[0]);
}


int main(int argc, int **argv){

	//Nome da imagem
	string source = "../Debug/lena.bmp";

	//Carregar imagem dentro da matriz
	Mat img = imread(source);

	//Verifica carregamento
	if (img.empty()){
		fprintf(stderr, "Erro: imagem nao encontrada.\n");
		getchar();
		exit(1);
	}

	//Cria nova janela
	namedWindow("Processamento Digital de Imagens", CV_WINDOW_AUTOSIZE);

	//Show
	/*imshow("Processamento Digital de Imagens", img_in);
	waitKey();
	imshow("Processamento Digital de Imagens", img);
	waitKey();*/

	//Transformada Discreta de Fourier
	filter(img);
	
	//Espera click
	waitKey();

	return 0;

}