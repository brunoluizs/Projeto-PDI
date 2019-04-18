#include <opencv.hpp>
#include <math.h>

#define PI 3.14159265358979323846

using namespace cv;
using namespace std;


void centerTransform(Mat img){
	unsigned char* data;
	int i, j;
	long long int pos = 0;

	data = img.ptr(0);

	int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);

	for (i = 0; i < m; i++){
		for (j = 0; j < n; j++){
			data[pos] = data[pos] * pow((double) -1, i+j);
			pos += 1;
		}
	}
}

Mat expandImgSize(Mat img){
	Mat expanded_img;

	//Funções para o cálculo do tamanho ideal para a DFT
	int m = getOptimalDFTSize(img.rows);
    int n = getOptimalDFTSize(img.cols);

	//Função para copiar a imagem e aumentar a borda, inserindo 0's ao redor da imagem.
	copyMakeBorder(img, expanded_img, 0, m - img.rows, 0, n - img.cols, BORDER_CONSTANT, Scalar::all(0));

	//imshow("Processamento Digital de Imagens", teste);

	return expanded_img;
}


/* Tentativa frustrada de fazer um FFT */
void fft(Mat input, Mat *fourier){
	unsigned char* inputData = input.ptr(0);
	float *inReal, *inImag;

	inReal = (float*) calloc (input.rows*input.cols, sizeof(float));
	inImag = (float*) calloc (input.rows*input.cols, sizeof(float));

	for (int cols = 0; cols < input.cols; cols++){
		for (int rows = 0; rows < input.rows; rows++){
			for (int k = 0; k < input.rows; k++){
				inReal[rows+(input.rows*cols)] += (inputData[k+(input.rows*cols)] * sin(2*PI*((rows*k/input.rows))));
				inImag[rows+(input.rows*cols)] -= (inputData[k+(input.rows*cols)] * cos(2*PI*((cols*k/input.cols))));
			}
		}
	}

	for (int rows = 0; rows < input.rows; rows++){
		for (int cols = 0; cols < input.cols; cols++){
			for (int k = 0; k < input.cols; k++){
				inReal[cols+(input.cols*rows)] += (inReal[k+(input.cols*rows)] * sin(2*PI*((rows*k/input.rows))));
				inImag[cols+(input.cols*rows)] -= (inImag[k+(input.cols*rows)] * cos(2*PI*((cols*k/input.cols))));
			}
		}
	}

	Mat fourierReal(input.rows, input.cols, CV_32F, inReal);
	Mat fourierImag(input.rows, input.cols, CV_32F, inImag);

	Mat fourier[] = {fourierReal, fourierImag};
}

void filtroZeroEUm(Mat filter){
	unsigned char *data;

	data = filter.ptr(0);

	unsigned char val = data[0];

	int total = filter.rows*filter.cols;
	
	for (int i = 0; i < total; i++){
			if (data[i] > 127)
				data[i] = 1;
			else
				data[i] = 0;
	}

}

void prep(Mat I){
    Mat padded = expandImgSize(I);
	//centerTransform(padded);

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};

	//fft(planes[0], planes);

	normalize(planes[0], planes[0], 0, 1, NORM_MINMAX);

    Mat complexImg;
    merge(planes, 2, complexImg);

    dft(complexImg, complexImg);

    split(complexImg, planes);

    magnitude(planes[0], planes[1], planes[0]);

	Mat magI = planes[0];
    magI += Scalar::all(1);
    log(magI, magI);

	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, NORM_MINMAX);

	Mat filter = imread("../data/filter2.png", CV_LOAD_IMAGE_GRAYSCALE);
                                           
    imshow("Input Image"       , I   );
    imshow("spectrum magnitude", magI);
	imshow("filter", filter);
    waitKey();

	filtroZeroEUm(filter);

	Mat filterD;
	filter.convertTo(filter, CV_32F);

	Mat test = Mat::zeros(planes[1].size(), CV_32F);

	Mat	newplanes[] = {magI.mul(filter), planes[1]};

	imshow("newplanezero", newplanes[0]);
    waitKey();

	Mat complexFilt;

	merge(newplanes, 2, complexFilt);

	idft(complexFilt, complexFilt);
	normalize(complexFilt, complexFilt, 0, 1, NORM_MINMAX);
	split(complexFilt, newplanes);

	imshow("result", newplanes[0]);
    waitKey();
}

int main(int argc, int **argv){

	//Nome da imagem
	string source = "../data/lenast.bmp";

	//Carregar imagem dentro da matriz
	Mat img = imread(source, CV_LOAD_IMAGE_GRAYSCALE);

	//Verifica carregamento
	if (img.empty()){
		fprintf(stderr, "Erro: imagem nao encontrada.\n");
		getchar();
		exit(1);
	}

	//Preparação para a aplicação do filtro
	prep(img);
	
	//Espera click
	waitKey();

	return 0;

}