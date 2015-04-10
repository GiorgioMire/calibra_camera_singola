/* Assegnato da Giacomo il 1/4/2015

/* 	Esercizio #2*/
/*  Fase Preliminare pratica di Opencv */

/* 	Lo scopo di questo codice è leggere alcune immagini salvate
	cercare il pattern (la scacchiera) e calibrare la telecamera*/


/* Inclusione librerie*/
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include "opencv2/core/core.hpp"
 #include <opencv2/calib3d/calib3d.hpp>
 #include <dirent.h>
 #include <iostream>

/* namespace utilizzati*/
using namespace cv;
using namespace std;



/* Dichiarazione funzioni*/
/* Funzione che scrive sullo stream di uscita un testo*/
void scrivi(string messaggio);
/* Fuzione che produce un elenco dei nomi ei file contenuti nel percorso passato*/
vector<string> elencaFile(const char* );
/*Funzione che calcola la posizione 3d dei punti della scacchiera dato il lato del quadrato in centimetri e le dimensioni*/
std::vector<cv::Point3f> Create3DChessboardCorners(Size boardSize, float);

/* Funzione main*/
int main(int argc ,char* argv[]){
Mat frame;
Size boardSize(9,6);	
std::vector<Point3f> scacchiera3d=Create3DChessboardCorners(boardSize,2.5) ;
vector<vector<Point2f> > ImagePoints;
vector<vector<Point3f> > ObjectPoints;
vector<string> elenco=elencaFile(argv[1]);


for (std::vector<string>::iterator it = elenco.begin(); it != elenco.end(); ++it){
    std::cout << endl << *it<<endl<<endl;

     std::size_t found = (*it).find("immagine");
  if (found==0){cout<<endl<<"trovato in "<<(*it)<<endl;
vector<Point2f> corners;

string percorso=string(argv[1]);
string file=(string) *it;
string nomecompleto=percorso+file;
cout<<nomecompleto;
frame=imread(nomecompleto);
namedWindow("finestra",WINDOW_AUTOSIZE);
imshow("finestra",frame);
waitKey(100);

  bool trovata_scacchiera= findChessboardCorners(frame, boardSize, corners,

        CALIB_CB_ADAPTIVE_THRESH 
        //Use adaptive thresholding to convert the image to black and white,
		// rather than a fixed threshold level (computed from the average image brightness)
         + CALIB_CB_NORMALIZE_IMAGE
         //Normalize the image gamma with equalizeHist() before applying fixed or adaptive thresholding.
        + CALIB_CB_FAST_CHECK);
        //Run a fast check on the image that looks for chessboard corners, and shortcut the call if none is found.
        //This can drastically speed up the call in the degenerate condition when no chessboard is observed.);

	/*Disegno i corner della scacchiera*/

	drawChessboardCorners(frame, boardSize, Mat(corners), trovata_scacchiera);
	imshow("finestra",frame);
	waitKey(100);
	if(trovata_scacchiera){
		Mat grey;
		cvtColor(frame, grey, CV_BGR2GRAY);
		cornerSubPix(grey, corners, Size(11, 11), Size(-1, -1),
    TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 1000, 0.1));

		scrivi("scacchiera trovata");
		ImagePoints.push_back(corners);
		ObjectPoints.push_back(scacchiera3d);
scrivi("corner aggiunti");
	}
}
 



}

std::vector<cv::Mat> rvecs;
std::vector<cv::Mat> tvecs;
scrivi("arrivato fino a qui");
cout<<ObjectPoints.size()<<ImagePoints.size();
scrivi("Ecco");
Mat Matrice_K;
Mat Distorsione;

float rms=calibrateCamera( ObjectPoints, ImagePoints, frame.size(), 
		Matrice_K, Distorsione,rvecs,tvecs, CV_CALIB_FIX_ASPECT_RATIO,TermCriteria(CV_TERMCRIT_EPS,999, 0.01)
 );


cout<<rms;
cv::FileStorage storage("CALIBRAZIONE.yml", cv::FileStorage::WRITE);
storage << "K" << Matrice_K;
storage << "D" << Distorsione;
storage.release(); 



 Mat image;
 Mat imageUndistorted;
VideoCapture cap(0);
namedWindow("win1");
namedWindow("win2");
    while(1)
    {
        cap >> image;
        undistort(image, imageUndistorted, Matrice_K,Distorsione);

        imshow("win1", image);
        imshow("win2", imageUndistorted);
         if(waitKey(30) >= 0) break;
    }
cap.release();

return(0);	
}



 vector<string> elencaFile(const char* path){
 	cout<< *path;

vector<string> elenco;
DIR *dir;
struct dirent *ent;
if ((dir = opendir (path)) != NULL) {
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL) {
  	cout<<endl<<ent->d_name<<endl;
   elenco.push_back(ent->d_name);
  }
  closedir (dir);
} else {
  /* could not open directory */
 cout<<"errore";
  
}
return elenco;
}

void scrivi(string messaggio){
	cout<<endl<<messaggio<<endl;
};




std::vector<Point3f> Create3DChessboardCorners(Size boardSize, float squareSize)
{
// This function creates the 3D points of your chessboard in its own coordinate system
std::vector<Point3f> cornerstemp;
for( int i = 0; i < boardSize.height; i++ )
{
for( int j = 0; j < boardSize.width; j++ )
{
cornerstemp.push_back(Point3f(float(j*squareSize),
float(i*squareSize), 0));
}
}
return cornerstemp;
}
